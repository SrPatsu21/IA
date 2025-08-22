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
    size_t populationSize = 100;
    size_t generations = 10;
    size_t tournamentSize = 4;
    double mutationRate = 0.075;
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
    // 0 1 2 3 4
    std::iota(path.begin(), path.end(), 0);
    //random and make
    std::shuffle(path.begin(), path.end(), rng);
    return path;
}

double fitness(std::vector<int>& path) {
    int distance = pathDistance(path);
    return 1.0 / (distance + 1e-9);
}

double fitness(int distance) {
    return 1.0 / (distance + 1e-9);
}

std::vector<std::vector<int>> initPopulation( GAParams &param, std::mt19937& rng) {
    std::vector<std::vector<int>> pop;
    pop.reserve(param.populationSize);
    for (size_t i = 0; i < param.populationSize; ++i) {
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

std::vector<int> tournamentSelection(const std::vector<std::vector<int>>& pop, GAParams &param, std::mt19937& rng) {
    std::uniform_int_distribution<int> pick(0, (int)pop.size()-1);
    //init
    std::vector<int> best = pop[pick(rng)];
    double bestFit = fitness(best);
    //find the best
    for (size_t i=1; i < param.tournamentSize; i++) {
        auto cand = pop[pick(rng)];
        double f = fitness(cand);
        if (f > bestFit) {
            best = cand;
            bestFit = f;
        }
    }
    return best;
}

std::vector<int> orderCrossover(const std::vector<int>& p1, const std::vector<int>& p2, std::mt19937& rng) {
    int size = p1.size();
    std::vector<int> child(size, -1);
    std::uniform_int_distribution<int> cut(0, size-1);
    int a = cut(rng), b = cut(rng);
    if (a > b) {
        std::swap(a,b);
    }

    // copy p1
    for (int i=a; i<=b; i++)
    {
        child[i] = p1[i];
    }

    // mix with p2
    int idx = (b+1)%size;
    for (int j=0;j<size;j++) {
        int c = p2[(b+1+j)%size];
        if (find(child.begin(), child.end(), c) == child.end()) {
            child[idx] = c;
            idx = (idx+1)%size;
        }
    }
    return child;
}

void printPath(const std::vector<int>& path) {
    for (int i = 0; i < (int)path.size(); ++i) {
        std::cout << CITY_NAMES[path[i]] << " -> ";
    }
    std::cout << CITY_NAMES[path.front()] << "\n";
}

void findBest(std::vector<std::vector<int>> &pop, std::vector<int> &best, double &bestFit, GAParams &param, std::mt19937 &rng){
    for (size_t gen=0; gen < param.generations; gen++) {
        std::vector<std::vector<int>> newPop;

        // elitism
        auto elite = best;

        while (newPop.size() < param.populationSize) {
            auto p1 = tournamentSelection(pop, param, rng);
            auto p2 = tournamentSelection(pop, param, rng);
            auto child = orderCrossover(p1,p2,rng);
            mutate(child, param.mutationRate, rng);
            newPop.push_back(child);
        }

        pop = move(newPop);

        // find new best
        for (auto& ind : pop) {
            double f = fitness(ind);
            if (f > bestFit) {
                bestFit = f;
                best = ind;
            }
        }
    }
}

int main(){
    GAParams param;
    param.generations = 30;
    std::mt19937 rng(param.seed);

    auto pop = initPopulation(param, rng);

    std::vector<int> best = pop[0];
    double bestFit = fitness(best);

    findBest(pop, best, bestFit, param, rng);

    std::cout << "Melhor distância: " << pathDistance(best) << "\n";
    std::cout << "Melhor caminho: ";
    printPath(best);
    return 0;
}