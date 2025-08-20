#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <bits/stdc++.h>
#include <sstream>
#include <nlohmann/json.hpp>
#include <sstream>

class Room;

const int MAZE_WIDTH = 8;
const int MAZE_HEIGH = 7;

class Vec2
{
public:
    int x = 0;
    int y = 0;
    Room* owner = nullptr;
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

std::string printMaze(const std::vector<Room*>& maze) {
    std::ostringstream out;

    for (Room* room : maze) {
        out << "Room " << room->getName() << ":\n";

        for (const Vec2* block : room->getBlocks()) {
            out << "  Block (" << block->x << ", " << block->y << ")";

            if (!block->connections.empty()) {
                out << " -> ";
                for (size_t i = 0; i < block->connections.size(); ++i) {
                    const Vec2* conn = block->connections[i];
                    out << conn->owner->getName()
                        << "(" << conn->x << "," << conn->y << ")";
                    if (i + 1 < block->connections.size())
                        out << ", ";
                }
            }

            out << "\n";
        }

        out << "\n";
    }

    return out.str();
}

void breadthFirstSearch();
void depthFirstSearch();
void greedyBestFirstSearch();
void aSearch();

int main() {
    std::vector<Room*> maze;
    buildMaze(maze, "./room.json");
    std::cout << printMaze(maze) << std::endl;
    return 0;
}