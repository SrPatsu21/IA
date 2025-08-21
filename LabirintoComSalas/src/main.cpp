#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <stdexcept>
#include <bits/stdc++.h>
#include <sstream>
#include <nlohmann/json.hpp>

class Room;

class Vec2
{
public:
    int x = 0;
    int y = 0;
    Room* owner = nullptr;
    bool visited = false;
    std::vector<Vec2*> connections;

    Vec2(int x, int y, Room* owner){
        this->x=x;
        this->y=y;
        this->owner=owner;
    };

    void addConnection(Vec2* block) {
        connections.push_back(block);
    };

    void removeConnection(Vec2* target) {
        auto it = std::remove(connections.begin(), connections.end(), target);
        connections.erase(it, connections.end());
    }

    bool operator==(const Vec2& other) const {
        return x == other.x && y == other.y && owner == other.owner;
    }

    bool isNeighbor(Vec2* v2){
        int diff = std::abs(this->x-v2->x) + std::abs(this->y-v2->y);
        if (diff == 1)
        {
            return true;
        }
        return false;
    }
};

class Room
{
private:

    std::string name;
    std::vector<Vec2*> blocks;

public:
    Room(const std::string& name) : name(name) {}

    ~Room(){
        for (auto* b : blocks) delete b;
    };

    std::string getName(){
        return this->name;
    };
    std::vector<Vec2*> getBlocks(){
        return this->blocks;
    };
    void addBlock(Vec2* block) {
        blocks.push_back(block);
    }

    void removeBlock(Vec2* target) {
        auto it = std::find(blocks.begin(), blocks.end(), target);
        if (it != blocks.end()) {
            *it = std::move(blocks.back());
            blocks.pop_back();
        }
    }

    Vec2* findBlock(int x, int y) {
        for (auto* b : blocks)
            if (b->x == x && b->y == y) return b;
        return nullptr;
    }

};

int buildMaze(std::vector<Room*>& maze, const std::string& dataPath) {
    std::ifstream file(dataPath);
    if (!file) throw std::runtime_error("Could not open " + dataPath);

    nlohmann::json j;
    file >> j;

    // Step 1: Create rooms
    std::cout << "creating rooms: ";
    for (const auto& roomJson : j["rooms"]) {
        maze.push_back(new Room(roomJson["name"].get<std::string>()));
        std::cout << maze.back()->getName() << " | ";
    }

    std::cout << std::endl;

    std::cout << "creating blocks: ";
    // Step 2: Create blocks for each room
    for (auto& roomJson : j["rooms"]) {
        Room* current = nullptr;
        for (Room* mazeRoom : maze)
            if (mazeRoom->getName() == roomJson["name"]) current = mazeRoom;

        for (auto& blockJson : roomJson["blocks"]) {
            int x = blockJson["x"];
            int y = blockJson["y"];
            current->addBlock(new Vec2(x, y, current));
            std::cout << "(" << x << "," << y << ")" << " | ";
        }
    }

    std::cout << std::endl;

    // Step 3: Resolve connections
    std::cout << "creating connections: ";
    for (auto& roomJson : j["rooms"]) {
        Room* current = nullptr;
        for (Room* mazeRoom : maze){
            if (mazeRoom->getName() == roomJson["name"]) {
                current = mazeRoom;
            }
        }

        for (auto& blockJson : roomJson["blocks"]) {
            if (blockJson.contains("connections")) {
                int x = blockJson["x"];
                int y = blockJson["y"];
                Vec2* fromBlock = current->findBlock(x, y);

                for (auto& conn : blockJson["connections"]) {
                    std::string toRoomName = conn["room"];
                    int tx = conn["x"];
                    int ty = conn["y"];

                    // find target room
                    Room* targetRoom = nullptr;
                    for (Room* mazeRoom : maze){
                        if (mazeRoom->getName() == toRoomName) {
                            targetRoom = mazeRoom;
                        }
                    }

                    if (!targetRoom) {
                        throw std::runtime_error("Unknown room: " + toRoomName);
                    }

                    Vec2* toBlock = targetRoom->findBlock(tx, ty);
                    if (!toBlock){
                        throw std::runtime_error("Unknown block in " + toRoomName);
                    }

                    std::cout << fromBlock->owner->getName() << " to " << toBlock->owner->getName() << " | ";
                    fromBlock->addConnection(toBlock);
                }
            }
        }
    }

    std::cout << std::endl;

    return static_cast<int>(maze.size());
}

std::vector<std::vector<Vec2*>> buildGrid(const std::vector<Room*>& maze) {
    // find the bounds of the maze
    int maxX = 0, maxY = 0;
    for (auto* room : maze) {
        for (auto* block : room->getBlocks()) {
            if (block->x > maxX) {
                maxX = block->x;
            }
            if (block->y > maxY) {
                maxY = block->y;
            }
        }
    }

    // create empty grid
    std::vector<std::vector<Vec2*>> grid(maxY + 1, std::vector<Vec2*>(maxX + 1, nullptr));

    // fill grid with blocks
    for (auto* room : maze) {
        for (auto* block : room->getBlocks()) {
            grid[block->y][block->x] = block;
        }
    }

    return grid;
}

void printGrid(const std::vector<std::vector<Vec2*>>& grid) {
    for (const auto& row : grid) {
        for (Vec2* cell : row) {
            if (!cell) {
                std::cout << ". "; // empty
            } else if (cell->visited) {
                std::cout << "X "; // visited
            } else {
                std::cout << "O "; // not visited
            }
        }
        std::cout << "\n";
    }
}

void printGridResult(const std::vector<std::vector<Vec2*>>& grid, std::deque<Vec2*> path) {
    for (const auto& row : grid) {
        for (Vec2* cell : row) {
            bool isPath = false;
            for(Vec2* v2 : path){
                if (v2 == cell)
                {
                    isPath = true;
                    break;
                }
            }
            if (!cell) {
                std::cout << ". "; // empty
            } else if (isPath) {
                std::cout << "X "; // visited
            } else {
                std::cout << "O "; // not visited
            }
        }
        std::cout << "\n";
    }
}

std::deque<Vec2*> breadthFirstSearch(Vec2* start, Vec2* destination, const std::vector<std::vector<Vec2*>>& grid){
    std::deque<std::deque<Vec2*>> paths;
    paths.push_back({ start });
    std::cout << "===================================================================" << " actives:" << paths.size() << std::endl;
    printGrid(grid);
    if (start == destination)
    {
        return paths.back();
    }

    while (!paths.empty())
    {
        auto path = paths.front();
        paths.pop_front();

        if (path.back() == destination)
        {
            return path;
        }

        if (!path.back()->visited){

            path.back()->visited=true;

            for (Vec2* block : path.back()->owner->getBlocks()) {
                if (!block->visited && path.back()->isNeighbor(block)) {
                    auto newPath = path;
                    newPath.push_back(block);
                    paths.push_back(newPath);
                }
            }
            for (Vec2* neighbor : path.back()->connections) {
                if (!neighbor->visited) {
                    auto newPath = path;
                    newPath.push_back(neighbor);
                    paths.push_back(newPath);
                }
            }
        }
        std::cout << "===================================================================" << " actives:" << paths.size() << std::endl;
        printGrid(grid);
    }

    std::deque<Vec2*> fail = {};
    return fail;
};

void depthFirstSearch();
void greedyBestFirstSearch();
void aStarSearch();

int main() {
    std::vector<Room*> maze;
    buildMaze(maze, "./room.json");

    std::vector<std::vector<Vec2 *>> grid = buildGrid(maze);

    Vec2* start =grid[0][0];
    Vec2* destination = grid[6][7];
    std::cout << "start:" << "(" << start->x << ", " << start->y << ") ";
    std::cout << " destination:" << "(" << destination->x << ", " << destination->y << ") ";
    std::cout << std::endl;

    //* breadthFirstSearch
    std::deque<Vec2 *> path = breadthFirstSearch(start, destination, grid);
    std::cout << "Solution:" << std::endl;
    for (Vec2* node : path)
    {
        if (node) {
            std::cout << "" << node->owner->getName() <<"(" << node->x << ", " << node->y << ") |";
        }
    }
    std::cout << std::endl;
    printGridResult(grid, path);
    std::cout << std::endl;

    return 0;
}