#include <bits/stdc++.h>

struct Connect;

class Room
{
private:

std::string name;
std::vector<Connect> connections;

public:
    Room(std::string name){
        this->name = name;
    };

    ~Room(){
    };
};

struct Connect
{
    int weigth;
    Room* next;
};

void buildMaze(std::string dataPath){

};

int main() {

    return 0;
}