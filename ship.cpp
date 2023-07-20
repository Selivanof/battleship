#include "ship.h"

Ship::Ship(){
    this->shipSize = 0;
    squares = NULL;
}

Ship::Ship(int shipSize, ShipType shipType){
    this->shipSize = shipSize;
    this->shipType = shipType;
    squares = new Square*[shipSize];
}

Ship::~Ship(){
    if (squares != NULL)
        delete[] squares;
}

void Ship::setSize(int shipSize){
    this->shipSize = shipSize;
}

void Ship::setType(ShipType shipType){
    this->shipType = shipType;
}

int Ship::getSize(){
    return shipSize;
}

ShipType Ship::getType(){
    return shipType;
}

Square* Ship::getSquare(int index){
    return squares[index];
}

void Ship::setSquares(Square** squares){
    for(int i = 0; i < shipSize; i++){
        this->squares[i] = squares[i];
    }
}

bool Ship::hasSquare(Square* square){
    for (int i = 0; i < shipSize; i++){
        if (squares[i]->equals(square))
            return true;
    }
    return false;
}

int Ship::getNumberOfHits(){
    int numHits = 0;
    for (int i = 0; i < shipSize; i++){
        if (squares[i]->isHit())
            numHits++;
    }
    return numHits;
}

bool Ship::isSunk() {
    return getNumberOfHits() == shipSize;
}

Carrier::Carrier() : Ship(6, CARRIER) {
    doubleHits = 1;
}

bool Carrier::canDoubleHit(){
    return doubleHits > 0;
}

void Carrier::doubleHit(){
    doubleHits--;
}

Battleship::Battleship() : Ship(4, BATTLESHIP) {

}

Cruiser::Cruiser() : Ship(3, CRUISER) {

}

Submarine::Submarine(int moves) : Ship(3, SUBMARINE) {
    this->moves = moves;
}

bool Submarine::canMove(){
    return moves > 0 && getNumberOfHits() == 0;
}

void Submarine::makeMove(){
    moves--;
}

Destroyer::Destroyer() : Ship(2, DESTROYER) {

}
