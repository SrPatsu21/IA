#include <iostream>
#include <bits/stdc++.h>
#include <utility>
#include <vector>
#include <random>
#include <ctime>

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
    int elitismCount = 1;
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
    int d = pathDistance(path);
    return 1.0 / (d + 1e-9);
}

std::vector<std::vector<int>> initPopulation(int popSize, std::mt19937& rng) {
    std::vector<std::vector<int>> pop;
    pop.reserve(popSize);
    for (int i = 0; i < popSize; ++i) {
        pop.push_back(randomPath(rng));
    }
    return pop;
}

std::vector<int> orderCrossover(const std::vector<int>& p1, const std::vector<int>& p2, std::mt19937& rng) {
    std::uniform_int_distribution<int> cutDist(0, (int)p1.size() - 1);
    int c1 = cutDist(rng);
    int c2 = cutDist(rng);
    if (c1 > c2){
        std::swap(c1, c2);
    }

    std::vector<int> child(p1.size(), -1);
    // copia segmento de p1
    for (int i = c1; i <= c2; ++i){
        child[i] = p1[i];
    }

    // preenche com a ordem de p2, pulando os já presentes
    auto contains = [&](int city) {
        return find(child.begin(), child.end(), city) != child.end();
    };
    int idx = (c2 + 1) % (int)p1.size();
    for (int j = 0; j < (int)p1.size(); ++j) {
        int cand = p2[(c2 + 1 + j) % (int)p1.size()];
        if (!contains(cand)) {
            child[idx] = cand;
            idx = (idx + 1) % (int)p1.size();
        }
    }
    return child;
}

int main(){
    return 0;
}