#include <iostream>
#include <bits/stdc++.h>
#include <utility>
#include <vector>
#include <random>
#include <ctime>
#include <deque>
#include <tuple>
#include <numeric>

static const std::vector<char> CITY_NAMES = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
static const int MATRIX_SIZE = 10;

static const int DIST[MATRIX_SIZE][MATRIX_SIZE] = {
    {  0, 12, 29, 22, 13, 17, 25, 19, 31, 28},
    { 12,  0, 21, 15, 11, 18, 14, 23, 27, 20},
    { 29, 21,  0, 28, 36, 26, 30, 33, 24, 32},
    { 22, 15, 28,  0, 19, 23, 17, 21, 29, 27},
    { 13, 11, 36, 19,  0, 14, 20, 18, 25, 22},
    { 17, 18, 26, 23, 14,  0, 16, 24, 28, 19},
    { 25, 14, 30, 17, 20, 16,  0, 22, 26, 21},
    { 19, 23, 33, 21, 18, 24, 22,  0, 27, 29},
    { 31, 27, 24, 29, 25, 28, 26, 27,  0, 15},
    { 28, 20, 32, 27, 22, 19, 21, 29, 15,  0}
};

struct GAParams {
    size_t populationSize = 10;
    size_t generations = 10;
    size_t tournamentSize = 4;
    double mutationRate = 0.2;
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
    if(distance > 30){
        distance *= 2;
    }
    return 1.0 / (distance + 1e-9);
}

double fitness(int distance) {
    if(distance > 30){
        distance *= 2;
    }
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

std::vector<int> tournamentSelection(std::vector<std::vector<int>>& pop, GAParams &param, std::mt19937& rng) {
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

std::vector<int> rouletteSelection(const std::vector<std::vector<int>>& pop, std::mt19937& rng) {
    std::vector<double> fits;
    fits.reserve(pop.size());
    double totalFit = 0.0;
    for (auto ind : pop) {
        double f = fitness(ind);
        fits.push_back(f);
        totalFit += f;
    }

    std::uniform_real_distribution<double> dist(0.0, totalFit);
    double pick = dist(rng);

    double acum = 0.0;
    for (size_t i=0; i<pop.size(); i++) {
        acum += fits[i];
        if (acum >= pick) {
            return pop[i];
        }
    }
    return pop.back();
}

std::vector<int> rankingSelection(std::vector<std::vector<int>> pop, std::mt19937& rng) {
    std::sort(pop.begin(), pop.end(), [](auto& a, auto& b){
        return fitness(a) < fitness(b); // menor para maior
    });

    int n = pop.size();
    int totalRank = n * (n+1) / 2;

    std::uniform_int_distribution<int> dist(1, totalRank);
    int pick = dist(rng);

    int acum = 0;
    for (int i=0; i<n; i++) {
        acum += (i+1);
        if (acum >= pick) {
            return pop[i];
        }
    }
    return pop.back();
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

std::vector<int> pmxCrossover(const std::vector<int>& p1, const std::vector<int>& p2, std::mt19937& rng) {
    int size = p1.size();
    std::vector<int> child(size, -1);

    std::uniform_int_distribution<int> cut(0, size-1);
    int a = cut(rng), b = cut(rng);
    if (a > b) std::swap(a, b);

    // Copy segment from p1
    for (int i = a; i <= b; ++i) child[i] = p1[i];

    // Fill remaining positions from p2
    for (int i = 0; i < size; ++i) {
        int gene = p2[i];
        if (std::find(child.begin() + a, child.begin() + b + 1, gene) != child.begin() + b + 1) continue;

        int pos = i;
        while (child[pos] != -1) {
            // Find the position of gene from p1
            int val = p1[pos];
            pos = std::distance(p2.begin(), std::find(p2.begin(), p2.end(), val));
        }
        child[pos] = gene;
    }
    return child;
}

void logGenerationStats(std::vector<std::vector<int>>& pop, int generation) {
    double bestFit = 0.0;
    int bestDist = std::numeric_limits<int>::max();
    double totalDist = 0.0;

    for (auto& ind : pop) {
        int dist = pathDistance(ind);
        double fit = fitness(dist);
        totalDist += dist;
        if (fit > bestFit) {
            bestFit = fit;
            bestDist = dist;
        }
    }

    double avgDist = totalDist / pop.size();

    // Print generation stats
    std::cout << "Geração " << generation
                << " | Melhor distância: " << bestDist
                << " | Média distância: " << avgDist << "\n";
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
    logGenerationStats(pop, gen + 1);
    }
}

int main(){
    GAParams param;
    param.generations = 40;
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