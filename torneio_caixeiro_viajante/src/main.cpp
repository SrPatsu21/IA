#include <iostream>
#include <bits/stdc++.h>
#include <utility>
#include <vector>
#include <random>
#include <ctime>
#include <deque>
#include <tuple>
#include <numeric>

static const std::vector<char> CITY_NAMES = {'A','B','C','D','E'};
static const int MATRIX_SIZE = 5;

static const int DIST[MATRIX_SIZE][MATRIX_SIZE] = {
    { 0, 12,  3, 23,  1},
    {12,  0,  9, 18,  3},
    { 3,  9,  0, 89, 56},
    {23, 18, 89,  0, 45},
    { 1,  3, 56, 45,  0}
};

struct GAParams {
    int populationSize = 100;
    int generations = 10;
    int tournamentSize = 4;
    double mutationRate = 0.075;
    int elitismCount = (int)populationSize/generations;
    unsigned seed = std::random_device{}();
};

int pathDistance(std::vector<int> &path){
    int total = 0;
    for (int i = 0; i < ((int)path.size()-1); i++)
    {
        total += DIST[path[i]][path[i+1]];
    }
    total += DIST[path.back()][path.front()];
    return total;
}

std::vector<int> randomPath(std::mt19937& rng){
    std::vector<int> path(MATRIX_SIZE);
    std::vector<char> cities = CITY_NAMES;
    while (cities.size()){
        int next = rng() % cities.size();
        path.push_back(cities[next]);
        cities.erase(std::remove(cities.begin(), cities.end(), next), cities.end());
    }
    return path;
}

double fitness(std::vector<int>& path) {
    int distance = pathDistance(path);
    return 1.0 / (distance + 1e-9);
}

double fitness(int distance) {
    return 1.0 / (distance + 1e-9);
}


std::vector<std::vector<int>> initPopulation(int popSize, std::mt19937& rng) {
    std::vector<std::vector<int>> pop;
    pop.reserve(popSize);
    for (int i = 0; i < popSize; ++i) {
        pop.push_back(randomPath(rng));
    }
    return pop;
}

void mutate(std::vector<int>& ind, double mutationRate, std::mt19937& rng) {
    std::uniform_real_distribution<double> prob(0.0, 1.0);
    if (prob(rng) < mutationRate) {
        std::uniform_int_distribution<int> pos(0, (int)ind.size() - 1);
        int a = pos(rng);
        int b = pos(rng);
        // b need to be diferent
        while (b == a) {
            b = pos(rng);
        }
        std::swap(ind[a], ind[b]);
    }
}

std::vector<std::tuple<std::vector<int>, int, double>> tournamentSelection(const std::vector<std::vector<int>>& pop, GAParams &param) {
    std::vector<std::tuple<std::vector<int>, int, double>> elite;

    for (std::vector<int> path : pop)
    {
        int distance = pathDistance(path);
        elite.push_back({path, distance, fitness(distance)});
    }

    //sort
    std::sort(elite.begin(), elite.end(), [](const auto& a, const auto& b)
        {
            return std::get<2>(a) > std::get<2>(b);
        });

    //remove what is not elite
    while (elite.size() > param.elitismCount)
    {
        elite.pop_back();
    }
    return elite;
}

std::vector<std::vector<int>> makeCrossover(std::vector<std::tuple<std::vector<int>, int, double>> elite, std::mt19937& rng, GAParams &param)
{
    // total weight
    double totalWeight = 0.0;
    for (auto& t : elite) {
        totalWeight += std::get<2>(t); // double
    }

    if (totalWeight <= 0.0) {
        throw std::runtime_error("All weights are zero or negative.");
    }

    std::uniform_real_distribution<double> dist(0.0, totalWeight);

    std::vector<std::vector<int>> pop;
    while (pop.size() < param.populationSize)
    {
        double r1 = dist(rng);
        bool r1set = false;
        double r2 = dist(rng);
        bool r2set = false;
        std::vector<int> p1;
        std::vector<int> p2;

        double cumulative = 0.0;
        for (auto& e : elite) {
            if (r1set && r2set)
            {
                break;
            }
            cumulative += std::get<2>(e);
            if (!r1set && r1 < cumulative) {
                p1 = std::get<0>(e);
                r1set = true;
            }
            if (!r2set && r2 < cumulative) {
                p2 = std::get<0>(e);
                r2set = true;
            }
        }

        std::vector<int> newpath;
        if (p1.size() == p2.size())
        {
            std::uniform_real_distribution<int> oneOrAnother(0, 1);
            for (size_t i = 0; i < p1.size(); i++)
            {
                if (oneOrAnother(rng)) {
                    newpath.push_back(p1[i]);
                }else {
                    newpath.push_back(p2[i]);
                }
            }
            mutate(newpath, param.mutationRate, rng);
        }
        pop.push_back(newpath);
    }
    return pop;
}

void printPath(const std::vector<int>& path) {
    for (int i = 0; i < (int)path.size(); ++i) {
        std::cout << CITY_NAMES[path[i]] << " -> ";
    }
    std::cout << CITY_NAMES[path.front()] << "\n";
}

int main(){
    return 0;
}