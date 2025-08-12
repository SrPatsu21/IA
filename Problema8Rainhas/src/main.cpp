#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <set>

const int TABLE_WIDTH = 8-1, TABLE_HEIGHT = 8-1, N_QUEENS = 8;
const std::pair<int, int> TABLE= {TABLE_WIDTH, TABLE_HEIGHT};

void nextRow(std::pair<int, int>& index){
    if (index.second < TABLE_HEIGHT)
    {
        index.second++;
    }
}

void nextLine(std::pair<int, int>& index){
    if (index.first < TABLE_WIDTH)
    {
        index.first++;
    }else
    {
        if (index.second < TABLE_HEIGHT)
        {
            index.first=0;
            nextRow(index);
        }
    }
}

bool isSafe(const std::vector<std::pair<int,int>>& queens, const std::pair<int,int>& index) {
    for (auto q : queens) {
        if (q.first == index.first || q.second == index.second) return false;
        if (std::abs(q.first - index.first) == std::abs(q.second - index.second)) return false;
    }
    return true;
}

void removeQueen(std::vector<std::pair<int, int>>& queens, std::pair<int, int>& index){
    if (queens.empty())
    {
        return;
    }
    std::cout << "Queen " << queens.size() << " removed at (" << queens.back().first << "," << queens.back().second << ")" << std::endl;
    index = {queens.back().first, queens.back().second};
    nextLine(index);
    if (index == TABLE && !isSafe(queens, index))
    {
        queens.pop_back();
        removeQueen(queens, index);
    }else
    {
        queens.pop_back();
    }
}

void tryPlaceQueen(std::vector<std::pair<int, int>>& queens, std::pair<int, int>& index){
    if (queens.size() < N_QUEENS)
    {
        while (!isSafe(queens, index) && index != TABLE){
            nextLine(index);
        }
        if (index == TABLE && !isSafe(queens, index))
        {
            removeQueen(queens, index);
        }else{
            //place queen
            queens.push_back({index.first, index.second});
            std::cout << "Queen placed at (" << index.first << "," << index.second << ")" << std::endl;
        }
        //next
        tryPlaceQueen(queens, index);
    }else{
        std::cout << "Queens (" << queens.size() << "/" << N_QUEENS << ")" << std::endl;
        bool pq = false;
        for (int j = 0; j < TABLE_HEIGHT+1; j++)
        {
            for (int i = 0; i < TABLE_WIDTH+1; i++)
            {
                for (auto q : queens)
                {
                    if (q.first == i && q.second == j)
                    {
                        pq = true;
                    }
                }
                if (!pq)
                {
                    std::cout << " 0 ";
                }else
                {
                    pq = false;
                    std::cout << " 1 ";
                }
            }
            std::cout << std::endl;
        }
    }
}

int main() {
    std::vector<std::pair<int, int>> queens;
    std::pair<int, int> index = {0, 0};
    tryPlaceQueen(queens, index);
    return 0;
}