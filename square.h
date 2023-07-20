#ifndef SQUARE_H
#define SQUARE_H

#include <sstream>

using namespace std;

/**
 * Class that implements a square of the game.
 */
class Square
{
private:
    int x; // the horizontal coordinate of the square
    int y; // the vertical coordinate of the square
    bool ship; // boolean indicating if the square has a ship (true) or not (false)
    bool hit;  // boolean indicating if the square has been hit (true) or not (false)

public:
    /**
     * Initializes a square indicating that it does not have a ship and that it has not been hit.
     *
     * @param x the horizontal coordinate of the square to be set.
     * @param y the vertical coordinate of the square to be set.
     */
    Square(int x, int y);

    /**
     * Returns the horizontal coordinate of the square.
     *
     * @return the horizontal coordinate of the square.
     */
    int getX();

    /**
     * Returns the vertical coordinate of the square.
     *
     * @return the vertical coordinate of the square.
     */
    int getY();

    /**
     * Adds a part of a ship to this square by setting the corresponding boolean variable to true.
     */
    void addShip();

    /**
     * Removes the ship from this square by setting the corresponding boolean variable to false.
     */
    void removeShip();

    /**
     * Hits this square by setting the corresponding boolean variable to true.
     */
    void hitSquare();

    /**
     * Checks if the square is occupied by a ship.
     *
     * @return true if the square is occupied by a ship, or false otherwise.
     */
    bool hasShip();

    /**
     * Checks if the square has accepted a hit.
     *
     * @return true if the square is hit, or false otherwise.
     */
    bool isHit();

    /**
     * Checks if the square given is equal to this square. Two squares are equal if
     * they have the same horizontal and vertical coordinates.
     *
     * @param square the square to check if it is equal to this square.
     * @return true if the square given is equal to this square, or false otherwise.
     */
    bool equals(Square* square);

    /**
     * Returns a string representation of this square. For example, for the square with
     * coordinates (2, 4), the string representation is "C4".
     *
     * @return the square as string.
     */
    string toString();
};

#endif // SQUARE_H
