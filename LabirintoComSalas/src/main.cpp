#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <bits/stdc++.h>
#include <nlohmann/json.hpp>

struct Room;
struct Connection
{
    int weigth;
    Room* next;
};

class Room
{
private:

std::string name;
std::vector<Connection> connections;

public:
    Room(std::string name){
        this->name = name;
    };

    ~Room(){
    };

    void setName(std::string name){
        this->name = name;
    };
    void addConnection(Connection connection){
        this->connections.push_back(connection);
    };
    void removeConnection(Room* target) {
        auto it = std::remove_if(
            connections.begin(),
            connections.end(),
            [target](const Connection& conn) {
                return conn.next == target;
            }
        );
        connections.erase(it, connections.end());
    };
    std::string getName(){
        return this->name;
    };
    std::vector<Connection> getConnections(){
        return this->connections;
    };
};

int buildMaze(std::vector<Room*>& maze, const std::string& dataPath) {
    std::ifstream file(dataPath);
    if (!file) {
        throw std::runtime_error("Could not open " + dataPath);
        return 0;
    }

    nlohmann::json j;
    file >> j;

    // Create all roomsconn
    for (const auto& room : j["rooms"]) {
        maze.push_back(new Room(room["name"].get<std::string>()));
        std::cout << " add " << maze.back()->getName() << "  | ";
    }

    std::cout << std::endl;

    // Add connections
    for (auto& room : j["rooms"]) {

        Room* current = nullptr;
        for (Room* mazeRoom : maze)
        {
            if (mazeRoom->getName() == room["name"])
            {
                std::cout << " found " << mazeRoom->getName() << ":" << std::endl;
                current = mazeRoom;
                break;
            }
        }

        for (auto& conn : room["connections"]) {
            std::string toName = conn["to"];
            int weight = conn["weight"];

            for (Room* mazeRoom : maze)
            {
                if (mazeRoom->getName() == toName)
                {
                    current->addConnection({ weight, mazeRoom });
                    std::cout << " | " << current->getName() << " connection to " << toName;
                }
            }
            if (current->getConnections().size() == 0 || current->getConnections().back().next->getName() != toName)
            {
                throw std::runtime_error("Unknown room name in connection: " + toName);
                return 0;
            }
        }
        std::cout << ";" << std::endl;
    }

    return static_cast<int>(maze.size());
}

void printMaze(std::vector<Room*>& maze) {
    std::cout << "Printing maze:";
    for (Room* room : maze) {
        std::cout << "Room: " << room->getName() << "\n";

        std::vector<Connection> conns = room->getConnections();
        if (conns.empty()) {
            std::cout << "  No connections\n";
        } else {
            for (auto& conn : conns) {
                std::cout << "  -> " << conn.next->getName()
                << " (weight: " << conn.weigth << ")\n";
            }
        }
        std::cout << "\n";
    }
}

int main() {
    std::vector<Room*> maze;
    buildMaze(maze, "./room.json");
    printMaze(maze);
    return 0;
}