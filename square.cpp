#include <iostream>
#include "square.h"

Square::Square(int x, int y) {
    this->x = x;
    this->y = y;
    ship = false;
    hit = false;
}

int Square::getX() {
    return x;
}

int Square::getY(){
    return y;
}

void Square::addShip() {
    ship = true;
}

void Square::removeShip() {
    ship = false;
}

void Square::hitSquare() {
    hit = true;
}

bool Square::hasShip() {
    return ship;
}

bool Square::isHit() {
    return hit;
}

bool Square::equals(Square* square) {
    return (x == square->x && y == square->y);
}

string Square::toString() {
    stringstream sstm;
    sstm << ((char)('A' + x)) << y;
    return sstm.str();
};
