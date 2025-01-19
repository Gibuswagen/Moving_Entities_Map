#include <iostream>
#include <string> //string functions
#include <cstdlib> //random function
#include <vector> //introduce vectors
#include <ctime> //for the time() function
#include <random> //different random function
#include <algorithm> // library that helps to remove an entity

using namespace std;
// Base Class of all entities
class Entity {
public:
    char symbol;
    int row;
    int col;
    // Getter for row
    int getRow() const {
        return row;
    }
    // Setter for row
    void setRow(int r) {
        row = r;
    }
    // Getter for column
    int getCol() const {
        return col;
    }
    // Setter for column
    void setCol(int c) {
        col = c;
    }
    virtual void getInfo() {
        cout << "Entity info" << endl;
    };
    virtual ~Entity() {}
};
//Human entity which has a name and health
class Human : public Entity {
private:
    int health = 100;
    string name;
public:
    Human(string n){
        symbol = '@';
        name = n;
    }
    void getInfo() {
        cout << "Type: Human" << endl;
        cout << "Name: " << name << endl;
        cout << "Health: " << health << endl;
    }
    // Getter for health
    int getHealth(){
        return health;
    }
    // Setter for health
    void setHealth(int hp) {
        health = hp;
    }
};
//Dragon entity with health
class Dragon : public Entity {
private:
    int health = 100;
public:
    Dragon(){
        symbol = '#';
    }
    void getInfo() {
        cout << "Type: Dragon" << endl;
        cout << "Health: " << health << endl;
    }
    // Getter for health
    int getHealth(){
        return health;
    }
    // Setter for health
    void setHealth(int hp) {
        health = hp;
    }
};
//Monster entity with strength and health
class Monster : public Entity {
private:
    int health = 100;
    int strength = rand() % 6;
public:
    Monster(){
        symbol = '*';
    }
    void getInfo() {
        cout << "Type: Monster" << endl;
        cout << "Strength: " << strength << endl;
        cout << "Health: " << health << endl;
    }
    // Getter for health
    int getHealth(){
        return health;
    }
    // Setter for health
    void setHealth(int hp) {
        health = hp;
    }
    // Getter for strength
    int getStrength() const {
        return strength;
    }
};
//Stone entity, cannot move,  so only has row and col from base class
class Stone : public Entity {
public:
    Stone(){
        symbol = 'S';
    }
    void getInfo() {
        cout << "Type: Stone" << endl;
    }
};
//Hole entity, cannot move, has depth as well as base class variables
class Hole : public Entity {
private:
    int depth = rand() % 21;
public:
    Hole(){
        symbol = 'O';
    }
    void getInfo() {
        cout << "Type: Hole" << endl;
        cout << "Depth: " << depth << endl;
    }
    // Getter for depth
    int getDepth() const{
        return depth;
    }
};
//Function that displays the 10x10 grid
void displayMap(string map[][11]) {
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            cout << map[i][j] << " ";
        }
        cout << endl;
    }
}
//Function that creates all possible coordinates of 10x10 grid, shuffles and take 12 elements of a vector.
void setCoords(vector<Entity*>& entities){
    //Create a struct that stores x,y coords.
    struct Coordinate{
        int x,y;
    };
    vector<Coordinate> v;
    Coordinate c;
    //Push every possible combination of a 10x10 grid to a vector
    for (int i = 1; i <= 10; i++){
        for (int j = 1; j <= 10; j++){
            c.x=i;
            c.y=j;
            v.push_back(c);
        }
    }
    //shuffle elements in a vector
    random_device rd;
    mt19937 gen(rd());
    shuffle(v.begin(), v.end(), gen);
    //assign to the entities
    int counter = 0;
    for (auto& entity : entities) {
        Coordinate c = v[counter];
        counter+=1;
        entity->setRow(c.x);
        entity->setCol(c.y);
    }
}
//Move parsed entity to a random adjacent cell.
void moveToAdjacent(string map [][11], Entity* entity,int row ,int col){
    // Check if any adjacent cells are empty
    bool unoccupied = false;
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = col - 1; j <= col + 1; j++) {
            // Ignore the current position
            if (i == row && j == col) {
                continue;
            }
                // Check if adjacent cell is within the grid and empty
                if (i >= 1 && i <= 10 && j >= 1 && j <= 10) {
                    if (map[i][j] == ".") {
                        unoccupied = true;
                        break;
                    }
                }
        }
        if (unoccupied) {
            break;
        }
        }
        if (unoccupied) {
            // Move entity to a random adjacent empty cell
            int newRow, newCol;
            do {
                int randIndex = rand() % 8;
                newRow = row - 1 + randIndex / 3;
                newCol = col - 1 + randIndex % 3;
            } while (map[newRow][newCol] != ".");
            // Move entity to the new coordinates
            map[newRow][newCol] = entity->symbol;
            map[row][col] = ".";
            entity->setRow(newRow);
            entity->setCol(newCol);
        }
}
//Function responsible for moving each type of entity according to their requirements:
//Human -> move right, if cant -> move randomly to a close cell.
//Dragon -> move randomly to a close cell.
//Monster -> randomly move to a empty cell (up,down,left,right) with a step of strength value , if cant -> move randomly to a close cell.
//Delete entity if health is 0.
void moveEntity(string map[][11], vector<Entity*>& entities) {
    for (auto& entity : entities) {
        if (auto human = dynamic_cast<Human*>(entity)) {
            int row = human->getRow();
            int col = human->getCol();
            // Check if human is at the edge of the room or the cell to the right is occupied
            if (col == 10 || map[row][col + 1] != ".") {
                moveToAdjacent(map,human,row,col);
                int currentHP = human->getHealth();
                human->setHealth(currentHP - 1);
            } else {
                // Human is not at the edge, move one cell to the right
                map[row][col + 1] = human->symbol;
                map[row][col] = ".";
                human->setCol(col + 1);
                int currentHP = human->getHealth();
                human->setHealth(currentHP - 1);
            }
            //Check if the human has 0 HP and remove it from the entities list if it does
            if (human->getHealth() <= 0) {
                entities.erase(remove(entities.begin(), entities.end(), human), entities.end());
                map[human->getRow()][human->getCol()]=".";
                delete human;
            }
        } else if (auto dragon = dynamic_cast<Dragon*>(entity)) {
            int row = dragon->getRow();
            int col = dragon->getCol();
            moveToAdjacent(map,dragon,row,col);
            int currentHP = dragon->getHealth();
            dragon -> setHealth(currentHP - (rand() % 6));
            // Check if the dragon has 0 HP and remove it from the entities list if it does
            if (dragon->getHealth() <= 0) {
                entities.erase(remove(entities.begin(), entities.end(), dragon), entities.end());
                map[dragon->getRow()][dragon->getCol()]=".";
                delete dragon;
            }
        }else if(auto monster = dynamic_cast<Monster*>(entity)){
            int row = monster->getRow();
            int col = monster->getCol();
            int step = monster->getStrength();
            //Possible cells
            vector<pair<int,int>> directions = {
                make_pair(row+step,col), //Right
                make_pair(row-step,col), //Left
                make_pair(row,col+step), //Up
                make_pair(row,col-step), //Down
            };
            // Find unoccupied cells
            vector<pair<int, int>> unoccupiedCells;
            //Collect possible moves
            for(const auto& dir: directions){
                int newRow = dir.first;
                int newCol = dir.second;
                if (newRow >= 1 && newRow <= 10 && newCol >= 1 && newCol <= 10){
                    if (map[newRow][newCol] == "."){
                        unoccupiedCells.push_back(make_pair(newRow, newCol));
                    }
                }
            }
            //If not empty -> select random possible cell
            if (!unoccupiedCells.empty()) {
                int randomIndex = rand() % unoccupiedCells.size();
                int newRow = unoccupiedCells[randomIndex].first;
                int newCol = unoccupiedCells[randomIndex].second;
                // Move monster to the new coordinates
                map[newRow][newCol] = monster->symbol;
                map[row][col] = ".";
                monster->setRow(newRow);
                monster->setCol(newCol);
                int currentHP = monster->getHealth();
                monster -> setHealth(currentHP - monster->getStrength());
            }else if (unoccupiedCells.empty() && step > 0) {
                //If no legal strength moves -> move to adjacent cell
                moveToAdjacent(map,monster,row,col);
                int currentHP = monster->getHealth();
                monster -> setHealth(currentHP - monster->getStrength());
            }
            // Check if the monster has 0 HP and remove it from the entities list if it does
            if (monster->getHealth() <= 0) {
                entities.erase(remove(entities.begin(), entities.end(), monster), entities.end());
                map[monster->getRow()][monster->getCol()]=".";
                delete monster;
            }
        }
    }
}
//Function that initiates the game, creates the map, entities and terminal options
void menu(){
    //Main map displaying every entity upon start.
    string map[11][11] = {
        {" ", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"},
        {"0", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."},
        {"1", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."},
        {"2", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."},
        {"3", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."},
        {"4", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."},
        {"5", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."},
        {"6", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."},
        {"7", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."},
        {"8", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."},
        {"9", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."},
    };
    //Create vector of entities and add them.
    vector<Entity*> entities;
    entities.push_back(new Human("Harold"));
    entities.push_back(new Human("David"));
    entities.push_back(new Human("Clare"));
    entities.push_back(new Dragon());
    entities.push_back(new Dragon());
    entities.push_back(new Monster());
    entities.push_back(new Monster());
    entities.push_back(new Monster());
    entities.push_back(new Hole());
    entities.push_back(new Hole());
    entities.push_back(new Stone());
    entities.push_back(new Stone());

    //Set starting positions
    setCoords(entities);
    //Set symbols of entities on set coordinates.
    for (const auto& entity : entities) {
        map[entity->getRow()][entity->getCol()] = entity -> symbol;
    }
    //Terminal menu with option from 1 to 5.
    int input = 0;
    while (input != 5){
        displayMap(map);
        cout << "1. Display Room" << endl;
        cout << "2. Move all the animated entities" << endl;
        cout << "3. Display the entity state by coordinates" << endl;
        cout << "4. Reset the room" << endl;
        cout << "5. Finish" << endl;
        cin >> input;
        switch (input) 
        {   
            //Display room
            case 1:
            displayMap(map);
            break;
        
            //Move entities
            case 2:
            cout <<"----------------------"<<endl;
            moveEntity(map,entities);
            break;

            //Output info of an entity on chosen coordinates
            case 3:{
            cout <<"----------------------"<<endl;
            displayMap(map);
            int row;
            int col;
            cout << "Input desired row" << endl;
            cin >> row;
            cout << "Input desired column" << endl;
            cin >> col;
            displayMap(map);
            cout <<"----------------------"<<endl;
            //Check for such existing coordinate.
            bool entityFound = false;
            for (const auto& entity : entities) {
                if (entity->getRow() == row+1 && entity->getCol() == col+1 ){
                    cout << endl;
                    cout << "Entity at "<< "(" << row << "," << col << ")"<< endl;
                    entity->getInfo(); 
                    entityFound = true;
                    break;    
                }   
            }
            if (!entityFound){
                cout << "No entity present on this coordinates"<<endl;
            }
            cout<<endl;
            break;
            }
            //Delete entities, reset the display and start again.
            case 4:
            cout <<"----------------------"<<endl;
            // Clean up memory
            for (auto& entity : entities) {
                delete entity;
            }
            //Set all cells to empty.
            for (int i = 1; i < 11; i++) {
                for (int j = 1; j < 11; j++) {
                    map[i][j] = ".";
                }
            }
            //Reset
            menu();
        }
    }
    cout <<"----------------------"<<endl;
    cout<< "Finished..."<<endl;
}
int main() {
    srand(time(0));
    menu();
    return 0;
}
