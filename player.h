#ifndef PLAYER_H
#define PLAYER_H

#include "grid.h"

/**
 * Class that implements a player of the game.
 */
class Player
{
protected:
    int id;
    string name;
    bool missTurn; // indicates whether the player misses the next turn
    int points; // the points of the player during the game
    Ship** ships; // the ships of the player
    Grid* grid; // the grid of the player
    Grid* opponentGrid; // the grid of the opponent, used to keep track of the hits of the player

public:
    /**
     * Initializes a player setting the id and the name, and setting missTurn to false and points to zero.
     * The two grids (one for the player and one for keeping track of the opponent's grid)
     * and the ships are also initialized.
     *
     * @param id the id of the player.
     */
    Player(int id);

    /**
     * Deletes this player, by deleting also the grids and the ships.
     */
    virtual ~Player();

    /**
     * Instructs this player to miss the next turn, by setting the missTurn variable to true;
     */
    void missNextTurn();

    /**
     * Returns whether the player must miss the next turn and resets the missTurn variable to false.
     *
     * @return true if the player must miss the next turn, or false otherwise.
     */
    bool mustMissNextTurn();

    /**
     * Returns the name of the player.
     *
     * @return the name of the player.
     */
    string getName();

    /**
     * Returns the grid of the player.
     *
     * @return the grid of the player.
     */
    Grid* getGrid();

    /**
     * Returns a ship of the player given its index.
     *
     * @param index the index of the ship to be returned.
     * @return a ship of the player given its index.
     */
    Ship* getShip(int index);

    /**
     * Returns the number of ships of the player that are sunk.
     *
     * @return the number of ships of the player that are sunk.
     */
    int numberOfShipsThatAreSunk();

    /**
     * Accepts a hit at the location provided. This method calls the acceptHit method of the Grid object.
     *
     * @param x the horizontal coordinate of the hit.
     * @param y the vertical coordinate of the hit.
     * @return true if there is a ship in the square, or false otherwise.
     */
    bool acceptHit(int x, int y);

    /**
     * Checks if the submarine of the player can move, by calling the canMove method of Submarine.
     *
     * @return true if the submarine can move, or false otherwise.
     */
    bool canMoveSubmarine();

    /**
     * Checks if the carrier can perform a double hit, by calling the canDoubleHit method of Carrier.
     *
     * @return true if the carrier can perform a double hit, or false otherwise.
     */
    bool canDoubleHit();

    /**
     * Creates the ships of the player by calling the different constructors.
     */
    void createShips();

    /**
     * Returns a string representation of the player's grid.
     *
     * @param hide boolean indicating whether the ships should be shown (false) or remain hidden (true).
     * @return the grid of the player as string.
     */
    string gridToString(bool hide);

    /**
     * Adds the points given to the points of the player.
     *
     * @param points the points to be added to the points of the player.
     */
    void addPoints(int points);

    /**
     * Remove the points given from the points of the player.
     *
     * @param points the points to be removed from the points of the player.
     */
    void removePoints(int points);

    /**
     * Returns the number of points of the player.
     *
     * @return the number of points of the player.
     */
    int getPoints();

    /**
     * Places the ships of the player in different positions in the player's grid.
     * Calls the placeShip method for each ship placement, giving as input the coordinates
     * (x, y), the direction (r for right or d for down) and the size of the ship.
     *
     * This method has to be implemented from all derived classes of Player.
     */
    virtual void placeShips() = 0;

    /**
     * Guesses a square of the opponent's grid.
     *
     * This method has to be implemented from all derived classes of Player.
     *
     * @return the square that is guessed by the player.
     */
    virtual Square* guessSquare() = 0;

    /**
     * Decides whether to move the submarine to a new location in the grid and, if yes, it moves it.
     *
     * This method has to be implemented from all derived classes of Player.
     *
     * @return true if the submarine is moved, or false otherwise.
     */
    virtual bool moveSubmarine() = 0;

    /**
     * Decides whether to perform a double hit using the carrier of the player and, if yes, it performs it.
     *
     * This method has to be implemented from all derived classes of Player.
     *
     * @return true if the double hit is performed, or false otherwise.
     */
    virtual bool doubleHit() = 0;
};

/**
 * Class that implements a computer player that plays randomly.
 */
class ComputerPlayer : public Player
{
public:
    /**
     * Initializes a computer player calling the constructor of the player.
     *
     * @param id the id of the player.
     */
    ComputerPlayer(int id);

    /**
     * Places the ships of the player in different positions in the player's grid. Finds
     * different positions at random (x, y, and direction for each ship) and checks if they are
     * valid for placing the ship using the shipCanBePlaced method of the grid object. If yes, it
     * calls the placeShip method for each ship placement, giving as input the coordinates (x, y),
     * the direction (r for right or d for down) and the size of the ship.
     */
    void placeShips();

    /**
     * Guesses a square for the player randomly. This method continuously guesses a square
     * until it finds a non-hit square at the opponentGrid. When a valid guess is found,
     * the method returns the square that is guessed.
     *
     * @return the square that is guessed by the player.
     */
    Square* guessSquare();

    /**
     * Decides whether to move the submarine to a new location in the grid at random with probability
     * 0.5. If yes, it moves the submarine to a new location at random as long as it is valid (checked
     * using the shipCanBePlaced method of the grid object).
     *
     * @return true if the submarine is moved, or false otherwise.
     */
    bool moveSubmarine();

    /**
     * Decides whether to perform a double hit using the carrier of the player at random with probability
     * 0.5. If yes, it returns true, otherwise it returns false.
     *
     * @return true if the double hit is performed, or false otherwise.
     */
    bool doubleHit();
};

/**
 * Class that implements a human player that plays according to the input given by the console.
 */
class HumanPlayer : public Player
{
public:
    /**
     * Initializes a human player calling the constructor of the player.
     *
     * @param id the id of the player.
     */
    HumanPlayer(int id);

    /**
     * Places the ships of the player in different positions in the player's grid given by the user.
     */
    void placeShips();

    /**
     * Returns a square to hit given by the user.
     *
     * @return the square that is guessed by the player.
     */
    Square* guessSquare();

    /**
     * Decides whether to move the submarine to a new location in the grid and moves it to the location
     * according to the input given by the user.
     *
     * @return true if the submarine is moved, or false otherwise.
     */
    bool moveSubmarine();

    /**
     * Decides whether to perform a double hit according to the value given by the user.
     *
     * @return true if the double hit is performed, or false otherwise.
     */
    bool doubleHit();
};

/**
 * Class that implements a smart player that plays according to the state of the game.
 */
class SmartPlayer : public Player
{
public:
    /**
     * Initializes a smart player calling the constructor of the player.
     *
     * @param id the id of the player.
     */
    SmartPlayer(int id);

    /**
     * Places the ships of the smart player in different positions in the player's grid.
     */
    void placeShips();

    /**
     * Returns a square to hit according to a smart strategy.
     *
     * @return the square that is guessed by the player.
     */
    Square* guessSquare();

    /**
     * Decides whether to move the submarine to a new location in the grid and moves it to the location
     * according to a smart strategy.
     *
     * @return true if the submarine is moved, or false otherwise.
     */
    bool moveSubmarine();

    /**
     * Decides whether to perform a double hit according to a smart strategy.
     *
     * @return true if the double hit is performed, or false otherwise.
     */
    bool doubleHit();
};

#endif // PLAYER_H
