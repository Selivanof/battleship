#ifndef GRID_H
#define GRID_H

#include "ship.h"
class Player;

/**
 * Class that implements a grid of the game.
 */
class Grid {
private:
    Player* player; // the owner of the grid (used for debugging purposes)
    Square*** squares; // a 10 x 10 grid of squares
public:
    /**
     * Initializes this grid, by creating a 10 x 10 array of squares.
     */
    Grid();

    /**
     * Initializes this grid, by creating a 10 x 10 array of squares.
     *
     * @param player the owner of the grid (used for debugging purposes).
     */
    Grid(Player* player);

    /**
     * Deletes this grid, by deleting also all squares.
     */
    ~Grid();

    /**
     * Returns a square given its coordinates x and y.
     *
     * @param x the horizontal coordinate of the square.
     * @param y the vertical coordinate of the square.
     * @return the square of which the coordinates are given.
     */
    Square* getSquare(int x, int y);

    /**
     * Accepts a hit at the location provided. This method calls the hitSquare method of the relevant grid square.
     * And then it returns a boolean indicating whether there is a ship in the square.
     *
     * @param x the horizontal coordinate of the hit.
     * @param y the vertical coordinate of the hit.
     * @return true if there is a ship in the square, or false otherwise.
     */
    bool acceptHit(int x, int y);

    /**
     * Checks if a square x, y has accepted a hit.
     *
     * @param x the horizontal coordinate of the square.
     * @param y the vertical coordinate of the square.
     * @return true if the square is hit, or false otherwise.
     */
    bool isHit(int x, int y);

    /**
     * Checks if a square x, y is occupied by a ship.
     *
     * @param x the horizontal coordinate of the square.
     * @param y the vertical coordinate of the square.
     * @return true if the square is occupied by a ship, or false otherwise.
     */
    bool hasShip(int x, int y);

    /**
     * Checks whether a ship can be placed in the given position and direction. This is based
     * on two conditions. Firstly, all squares must be inside grid bounds. Secondly, all squares
     * must not be already occupied by other ships.
     *
     * @param x the horizontal coordinate of the first square of the ship.
     * @param y the vertical coordinate of the first square of the ship.
     * @param shipDirection the direction of the ship as a char ('r' or 'd').
     * @param ship the ship to be placed on the grid.
     * @return true if the ship can be placed on the grid, or false otherwise.
     */
    bool shipCanBePlaced(int x, int y, char shipDirection, Ship* ship);

    /**
     * Places a ship of the player given its position and its direction.
     *
     * @param x the horizontal coordinate of the first square of the ship.
     * @param y the vertical coordinate of the first square of the ship.
     * @param shipDirection the direction of the ship as a char ('r' or 'd').
     * @param ship the ship to be placed on the grid.
     */
    void placeShip(int x, int y, char shipDirection, Ship* ship);

    /**
     * Removes a ship from the grid, by removing it from all its squares.
     *
     * @param ship the ship to be removed from the grid.
     */
    void removeShip(Ship* ship);
};

#endif // GRID_H
