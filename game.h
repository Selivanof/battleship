#ifndef GAME_H
#define GAME_H

#include "deck.h"
#include "player.h"

/**
 * Class denoting the game.
 */
class Game
{
private:
    Deck* deck;
    Player** players;
    int numTurns;
    int playerTurn;
    int* score;

public:
    /**
     * Initializes a game setting the the deck and the players.
     * The score is also initialized.
     *
     * @param deck the deck of the game.
     * @param players the players of the game.
     */
    Game(Deck* deck, Player** players);

    /**
     * Deletes the game by deleting the score. Warning: this method does not also delete
     * the deck and the players! These are deleted by the main function.
     */
    ~Game();

    /**
     * Returns the number of turns of the game.
     *
     * @return the number of turns of the game.
     */
    int getNumTurns();

    /**
     * Returns the turn of the game (either 0 or 1).
     *
     * @return the turn of the game.
     */
    int getPlayerTurn();

    /**
     * Ends the current turn by giving the turn to the other player (or applying the result of a card).
     */
    void endTurn();

    /**
     * Prints the state of the game including all players and their grid.
     */
    void printState();

    /**
     * Checks whether the player of which the index is given has no ships left.
     *
     * @param index the index of the player to check whether its has any ships left.
     * @return true if the player of which the index is given has no ships left, or false otherwise.
     */
    bool playerHasNoShipsLeft(int index);

    /**
     * Returns the points of a player given its index. Apart from the points gathered during the game,
     * at the end of the game if the player has lost, then he/she earns 0 points. If the player has won,
     * then he/she earns 50 points for the victory, as well as 10 points for every square of a Carrier ship
     * that is not hit, 5 points for every square of a Battleship ship that is not hit, and 1 point for
     * every square of any other ship that is not hit. These points are added to the already computed points
     * of the player from the start of the game.
     *
     * @param index the index of a player, either 0 or 1.
     * @return the points of a player given its index.
     */
    int getPoints(int index);
};

#endif // GAME_H
