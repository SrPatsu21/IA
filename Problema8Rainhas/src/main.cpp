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
    }else{
        if (index.first < TABLE_WIDTH)
        {
            index.first++;
        }
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

bool isSafe(std::vector<std::pair<int, int>>& queens, std::pair<int, int>& index){
    for (auto q : queens)
    {
        if (q.first == index.first)
        {
            nextLine(index);
            return false;
        }
        if (q.second == index.second)
        {
            nextRow(index);
            return false;
        }
        //check right diagonal
        int d_size = std::min(TABLE_WIDTH - q.first, TABLE_HEIGHT - q.second);
        for (int i = 1; i < d_size ; i++)
        {
            if (q.first+i == index.first && q.second+i == index.second)
            {
                nextLine(index);
                return false;
            }
        }
        //check reverse right diagonal
        d_size = std::min(TABLE_WIDTH - q.first, q.second + 1);
        for (int i = 1; i < d_size ; i++)
        {
            if (q.first+i == index.first && q.second-i == index.second)
            {
                nextLine(index);
                return false;
            }
        }
        //check left diagonal
        d_size = std::min(q.first + 1, TABLE_HEIGHT - q.second);
        for (int i = 1; i < d_size ; i++)
        {
            if (q.first-i == index.first && q.second+i == index.second)
            {
                nextLine(index);
                return false;
            }
        }
        //check reverse left diagonal
        d_size = std::min(q.first + 1, q.second + 1);
        for (int i = 1; i < d_size ; i++)
        {
            if (q.first-i == index.first && q.second-i == index.second)
            {
                nextLine(index);
                return false;
            }
        }
    }
    return true;
}

void tryPlaceQueen(std::vector<std::pair<int, int>>& queens, std::pair<int, int>& index){
    if (queens.size() < N_QUEENS)
    {
        while (!isSafe(queens, index) && index != TABLE){}
        if (index == TABLE && !isSafe(queens, index))
        {
            std::cout << "Queen removed at (" << queens.back().first << "," << queens.back().second << ")" << std::endl;
            index = {queens.end().base()->first, queens.back().second};
            nextLine(index);
            queens.pop_back();
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
    std::pair<int, int> index = {0, 1};
    tryPlaceQueen(queens, index);
    return 0;
}