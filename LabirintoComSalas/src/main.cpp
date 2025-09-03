#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <stdexcept>
#include <bits/stdc++.h>
#include <sstream>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <limits>

//A*
const double COST_PER_MOVE = 1.0;

class Room;

class Vec2
{
public:
    int x = 0;
    int y = 0;
    Room* owner = nullptr;
    bool visited = false;
    std::vector<Vec2*> connections;

    // A*
    double cost = 1e9;   // cost from start (init large)
    double heu = 0;     // heuristic
    double total_estimated_cost = 1e9;   // cost + heuristic
    Vec2* parent = nullptr;

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
        std::cout << std::endl;
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
        std::cout << std::endl;
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

std::deque<Vec2*> depthFirstSearch(Vec2* start, Vec2* destination, const std::vector<std::vector<Vec2*>>& grid){
    std::deque<Vec2*> path;
    path.push_back(start);
    start->visited = true;

    while(path.back() != destination){
        path.back()->visited = true;

        Vec2* next = nullptr;
        while (next == nullptr)
        {
            for (Vec2* block : path.back()->owner->getBlocks()) {
                if (!block->visited && path.back()->isNeighbor(block)) {
                    next = block;
                    break;
                }
            }


            if (next == nullptr)
            {
                for (Vec2* neighbor : path.back()->connections) {
                    if (!neighbor->visited) {
                        next = neighbor;
                        break;
                    }
                }
            }

            if (next == nullptr)
            {
                path.pop_back();
                if (path.empty())
                {
                    return path;
                }
            }
        }
        path.push_back(next);
        std::cout << "===================================================================" << " path size:" << path.size() << std::endl;
        printGrid(grid);
    }
    return path;
};

double heuristic(Vec2* vec, Vec2* dest){
    return std::abs(vec->x - dest->x) + std::abs(vec->y - dest->y);
}

std::deque<Vec2*> greedyBestFirstSearch(Vec2* start, Vec2* destination, const std::vector<std::vector<Vec2*>>& grid){
    std::deque<Vec2*> path;
    std::vector<Vec2*> openList;

    start->visited = true;
    start->parent = nullptr;
    openList.push_back(start);

    while (!openList.empty()) {
        std::sort(openList.begin(), openList.end(), [destination](Vec2* a, Vec2* b){
            return heuristic(a, destination) < heuristic(b, destination);
        });

        Vec2* current = openList.front();
        openList.erase(openList.begin());

        if (current == destination) {
            std::deque<Vec2*> finalPath;
            while (current) {
                finalPath.push_front(current);
                current = current->parent;
            }
            return finalPath;
        }

        std::vector<Vec2*> neighbors;
        for (Vec2* block : current->owner->getBlocks()) {
            if (!block->visited && current->isNeighbor(block)) {
                neighbors.push_back(block);
            }
        }
        for (Vec2* neighbor : current->connections) {
            if (!neighbor->visited) {
                neighbors.push_back(neighbor);
            }
        }

        for (Vec2* neighbor : neighbors) {
            neighbor->visited = true;
            neighbor->parent = current;
            openList.push_back(neighbor);
        }

        std::cout << "=================================================================== " << " exploring (" << current->x << "," << current->y << ") "
                << " openList size: " << openList.size() << std::endl;
        printGrid(grid);
    }

    // caminho não encontrado
    return std::deque<Vec2*>();
}

// Path reconstruction
std::deque<Vec2*> reconstructPath(Vec2* current) {
    std::deque<Vec2*> path;
    while (current) {
        path.push_front(current);
        current = current->parent;
    }
    return path;
}

std::deque<Vec2*> aStarSearch(Vec2* start, Vec2* destination, const std::vector<std::vector<Vec2*>>& grid){

    // Open list = nodes to explore
    std::vector<Vec2*> openList;
    openList.push_back(start);

    start->cost = 0;
    start->heu = heuristic(start, destination);
    start->total_estimated_cost = start->cost + start->heu;

    while (!openList.empty()) {

        // Pick node with lowest total_estimated_cost
        auto bestIt = std::min_element(openList.begin(), openList.end(),
            [](Vec2* a, Vec2* b) { return a->total_estimated_cost < b->total_estimated_cost; });
        Vec2* current = *bestIt;
        openList.erase(bestIt);

        if (current == destination) {
            std::deque<Vec2*> path;
            while (current) {
                path.push_front(current);
                current = current->parent;
            }
            return path;
        }

        current->visited = true;

        // Collect neighbors
        std::vector<Vec2*> neighbors;
        for (Vec2* block : current->owner->getBlocks()) {
            if (current->isNeighbor(block)) {
                neighbors.push_back(block);
            }
        }
        for (Vec2* neighbor : current->connections) {
            neighbors.push_back(neighbor);
        }

        for (Vec2* neighbor : neighbors) {
            if (!neighbor->visited) {
                double tentative_cost = current->cost + COST_PER_MOVE;

                if (tentative_cost < neighbor->cost) {
                    neighbor->parent = current;
                    neighbor->cost = tentative_cost;
                    neighbor->heu = heuristic(neighbor, destination);
                    neighbor->total_estimated_cost = neighbor->cost + neighbor->heu;

                    // If not in open list, add it
                    if (std::find(openList.begin(), openList.end(), neighbor) == openList.end()) {
                        openList.push_back(neighbor);
                    }
                }
            }
        }

        std::cout << "=================================================================== " << " exploring (" << current->x << "," << current->y << ") " << " openList size: " << openList.size() << std::endl;
        printGrid(grid);
    }

    std::deque<Vec2*> emptyPath;
    return emptyPath;
};

int main(int argc, char* argv[]) {
    if (argc < 6) {
        std::cerr << "Usage: " << argv[0] << " <algorithm> <start_x> <start_y> <dest_x> <dest_y> (7x6)" << std::endl;
        std::cerr << "Algorithms: bfs | dfs | greedy | astar" << std::endl;
        return 1;
    }

    std::vector<Room*> maze;
    buildMaze(maze, "./room.json");
    std::vector<std::vector<Vec2 *>> grid = buildGrid(maze);

    std::string algorithm = argv[1];
    int sx = std::stoi(argv[2]);
    int sy = std::stoi(argv[3]);
    int dx = std::stoi(argv[4]);
    int dy = std::stoi(argv[5]);

    // Validate start/destination
    if (sy < 0 || sy >= (int)grid.size() || sx < 0 || sx >= (int)grid[0].size() || !grid[sy][sx]) {
        std::cerr << "Invalid start coordinates!" << std::endl;
        return 1;
    }
    if (dy < 0 || dy >= (int)grid.size() || dx < 0 || dx >= (int)grid[0].size() || !grid[dy][dx]) {
        std::cerr << "Invalid destination coordinates!" << std::endl;
        return 1;
    }

    Vec2* start = grid[sy][sx];
    Vec2* destination = grid[dy][dx];

    std::cout << "Algorithm: " << algorithm << std::endl;
    std::cout << "Start: (" << start->x << ", " << start->y << ")" << std::endl;
    std::cout << "Destination: (" << destination->x << ", " << destination->y << ")" << std::endl;

    std::deque<Vec2 *> path;

    if (algorithm == "bfs") {
        path = breadthFirstSearch(start, destination, grid);
    } else if (algorithm == "dfs") {
        path = depthFirstSearch(start, destination, grid);
    } else if (algorithm == "greedy") {
        path = greedyBestFirstSearch(start, destination, grid);
    } else if (algorithm == "astar") {
        path = aStarSearch(start, destination, grid);
    } else {
        std::cerr << "Unknown algorithm: " << algorithm << std::endl;
        return 1;
    }

    //* solution
    std::cout << "Solution:" << std::endl;
    std::cout << "path size " << path.size() << std::endl;
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