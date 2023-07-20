#include <iostream>
#include <cstdlib>
#include "grid.h"
#include "player.h"

Grid::Grid() {
    squares = new Square**[10];
    for (int i = 0; i < 10; i++){
        squares[i] = new Square*[10];
        for (int j = 0; j < 10; j++)
            squares[i][j] = new Square(i, j);
    }
}

Grid::Grid(Player* player) {
    squares = new Square**[10];
    for (int i = 0; i < 10; i++){
        squares[i] = new Square*[10];
        for (int j = 0; j < 10; j++)
            squares[i][j] = new Square(i, j);
    }
    this->player = player;
}

Grid::~Grid() {
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++)
            delete squares[i][j];
        delete[] squares[i];
    }
    delete[] squares;
}

Square* Grid::getSquare(int x, int y){
    return squares[x][y];
}

bool Grid::acceptHit(int x, int y){
    squares[x][y]->hitSquare();
    return squares[x][y]->hasShip();
}

bool Grid::isHit(int x, int y){
    return squares[x][y]->isHit();
}

bool Grid::hasShip(int x, int y){
    return squares[x][y]->hasShip();
}

void Grid::placeShip(int x, int y, char shipDirection, Ship* ship){
    if (shipCanBePlaced(x, y, shipDirection, ship)){
        Square** shipSquares = new Square*[ship->getSize()];
        if (shipDirection == 'r'){
            for(int i = 0; i < ship->getSize(); i++){
                shipSquares[i] = squares[x][y+i];
                squares[x][y+i]->addShip();
            }
        }
        else if (shipDirection == 'd'){
            for(int i = 0; i < ship->getSize(); i++){
                shipSquares[i] = squares[x+i][y];
                squares[x+i][y]->addShip();
            }
        }
        ship->setSquares(shipSquares);
        delete[] shipSquares;
    }
    else{
        cout << "Error in ship placement of player " << player->getName() << "! Exiting..." << endl;
        std::exit(0);
    }
}

void Grid::removeShip(Ship* ship){
    for(int i = 0; i < ship->getSize(); i++)
        ship->getSquare(i)->removeShip();
}

bool Grid::shipCanBePlaced(int x, int y, char shipDirection, Ship* ship){
    if (shipDirection == 'r'){
        for(int i = 0; i < ship->getSize(); i++){
            if (x >= 0 && x < 10 && y+i >= 0 && y+i < 10){ // if square is inside grid bounds
                if (squares[x][y+i]->hasShip() || squares[x][y+i]->isHit()) // if square has ship or square is hit
                    return false;
            }
            else
                return false;
        }
    }
    if (shipDirection == 'd'){
        for(int i = 0; i < ship->getSize(); i++){
            if (x+i >= 0 && x+i < 10 && y >= 0 && y < 10){ // if square is inside grid bounds
                if (squares[x+i][y]->hasShip() || squares[x+i][y]->isHit()) // if square has ship or square is hit
                    return false;
            }
            else
                return false;
        }
    }
    return true;
}
