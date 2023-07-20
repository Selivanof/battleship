#ifndef SHIP_H
#define SHIP_H

#include "square.h"

/**
 * Enumeration used to represent the different types of ships.
 */
enum ShipType {CARRIER, BATTLESHIP, CRUISER, SUBMARINE, DESTROYER};

/**
 * Class that implements a ship of the game.
 */
class Ship
{
private:
    Square** squares; // the squares of the ship

protected:
    int shipSize; // the size of the ship
    ShipType shipType; // the type of the ship

public:
    /**
     * Initializes a ship setting its size to zero and initializing its squares to NULL and its type to CARRIER.
     */
    Ship();

    /**
     * Initializes a ship setting its size and its type and initializing its squares to an array of size shipSize.
     *
     * @param shipSize the size of the ship.
     * @param shipType the type of the ship.
     */
    Ship(int shipSize, ShipType shipType);

    /**
     * Deletes this ship by deleting the squares array (but not the individual squares).
     */
    ~Ship();

    /**
     * Sets the size of the ship to the one given.
     *
     * @param shipSize the size of the ship to be set.
     */
    void setSize(int shipSize);

    /**
     * Sets the type of the ship to the one given.
     *
     * @param shipType the type of the ship to be set.
     */
    void setType(ShipType shipType);

    /**
     * Returns the size of the ship.
     *
     * @return the size of the ship.
     */
    int getSize();

    /**
     * Returns the type of the ship.
     *
     * @return the type of the ship.
     */
    ShipType getType();

    /**
     * Sets the squares of the ship to the ones given.
     *
     * Hint: we have to use a loop to copy each individual square from the array given to the array squares of this ship.
     *
     * @param squares the squares to be added to the ship.
     */
    void setSquares(Square** squares);

    /**
     * Returns a square of the ship given its index.
     *
     * @param index the index of the square to be returned.
     * @return a square of the ship given its index.
     */
    Square* getSquare(int index);

    /**
     * Checks if the ship occupies a specific square that is given.
     *
     * Hint: we can use the equals method of the Square to check if the square given is equal to another square.
     *
     * @param square the square to be checked if it occupies the ship.
     * @return true if the ship occupies the square that is given, or false otherwise.
     */
    bool hasSquare(Square* square);

    /**
     * Finds and returns the number of hit squares in this ship.
     *
     * Hint: we can use the isHit method of the Square to check if a square of the ship is hit.
     *
     * @return the number of hits in this ship.
     */
    int getNumberOfHits();

    /**
     * Checks if the ship is sunk, which happens is the number of hits is equal to the size of the ship.
     *
     * @return true if the ship is sunk, or false otherwise.
     */
    bool isSunk();
};

/**
 * Class that implements a carrier.
 */
class Carrier : public Ship {
    int doubleHits; // the number of double hits performed by this carrier
public:
    /**
     * Initializes a carrier setting its size to six, setting the shipType, and setting the number of double hits to one.
     */
    Carrier();

    /**
     * Checks if the ship can perform a double hit or not. The ship can perform a double hit
     * if the number of double hits variable is larger than zero.
     *
     * @return true if the ship can perform a double hit, or false otherwise.
     */
    bool canDoubleHit();

    /**
     * Tracks a double hit performed, by subtracting one from the corresponding variable (i.e. reducing its value by one).
     */
    void doubleHit();
};

/**
 * Class that implements a battleship.
 */
class Battleship : public Ship {
public:
    /**
     * Initializes a battleship setting its size to four and setting the shipType.
     */
    Battleship();
};

/**
 * Class that implements a cruiser.
 */
class Cruiser : public Ship {
public:
    /**
     * Initializes a cruiser setting its size to three and setting the shipType.
     */
    Cruiser();
};

/**
 * Class that implements a submarine.
 */
class Submarine : public Ship {
    int moves;
public:
    /**
     * Initializes a submarine setting its size to three, setting the shipType, and setting the number of moves to the ones given.
     *
     * @param moves the number of moves that this ship can make (to initialize the corresponding variable).
     */
    Submarine(int moves);

    /**
     * Checks if the ship can perform a move or not. The ship can perform a move
     * if the number of moves variable is larger than zero, and only if the ship has no hits.
     *
     * @return true if the ship can perform a move, or false otherwise.
     */
    bool canMove();

    /**
     * Tracks a move performed, by subtracting one from the corresponding variable (i.e. reducing its value by one).
     */
    void makeMove();
};

/**
 * Class that implements a destroyer.
 */
class Destroyer : public Ship {
public:
    /**
     * Initializes a destroyer setting its size to two and setting the shipType.
     */
    Destroyer();
};

#endif
