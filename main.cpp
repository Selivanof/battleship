#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "game.h"

using namespace std;

int main() {
    srand(time(NULL));

    // Initialize all objects
    Deck* deck = new Deck();
    Player** players = new Player*[2];
    Game* game = new Game(deck, players);

    // Initialize players and set up the ships on the grid
    players[0] = new SmartPlayer(0);
    players[1] = new ComputerPlayer(1);
    players[0]->placeShips();
    players[1]->placeShips();

    int playerTurn = game->getPlayerTurn();
    do {
        cout << "Turn of " << players[playerTurn]->getName() << endl << endl;
        game->printState();

        // Start the turn and draw a card if the turn is a multiple of 5
        string endprint = "";
        if (game->getNumTurns() % 15 == 0){
            Card* card = deck->getNextCard();
            endprint += players[playerTurn]->getName() + card->getText();
            card->cardAction(players[playerTurn], players[1 - playerTurn]);
        }

        // Check if the player can move the submarine
        if (players[playerTurn]->canMoveSubmarine()) {
            // Check if the player wants to move the submarine
            if (players[playerTurn]->moveSubmarine()) {
                // Move the submarine
                ((Submarine*)players[playerTurn]->getShip(5))->makeMove();
                cout << players[playerTurn]->getName() << " moves the submarine!" << endl << endl;
                game->printState();
            }
        }

        // Guess a position at the grid of the opponent
        Square* p = players[playerTurn]->guessSquare();
        if(p->isHit()) {
            cout << "Error in guess of player " << players[playerTurn]->getName() << "! Exiting..." << endl;
            std::exit(0);
        }
        p->hitSquare();
        cout << players[playerTurn]->getName() << " shoots at " << p->toString();
        if (players[1 - playerTurn]->acceptHit(p->getX(), p->getY())){
            cout << ". It's a Hit!" << endl << endl;
            p->addShip();
        }
        else
            cout << ". It's a Miss!" << endl << endl;
        game->printState();

        // Check if the player can perform a double hit (from the carrier)
        if (players[playerTurn]->canDoubleHit()){
            // Check if the player wants to perform a double hit
            if (players[playerTurn]->doubleHit()) {
                // Perform a double hit by making a new guess
                ((Carrier*)players[playerTurn]->getShip(0))->doubleHit();
                Square* p = players[playerTurn]->guessSquare();
                if(p->isHit()) {
                    cout << "Error in guess of player " << players[playerTurn]->getName() << "! Exiting..." << endl;
                    std::exit(0);
                }
                p->hitSquare();
                cout << players[playerTurn]->getName() << " shoots at " << p->toString();
                if (players[1 - playerTurn]->acceptHit(p->getX(), p->getY())){
                    cout << ". It's a Hit!" << endl << endl;
                    p->addShip();
                }
                else
                    cout << ". It's a Miss!" << endl << endl;
                game->printState();
            }
        }

        // Ends the turn and starts the next turn
        cout << endprint << endl;
        game->endTurn();
        playerTurn = game->getPlayerTurn();
        // Uncomment the following line to add a delay
        // for(int i = 0; i < 100000000; i++);
    } while (!(game->playerHasNoShipsLeft(0) || game->playerHasNoShipsLeft(1)));

    // Print the winner and the final score of the game
    if (game->playerHasNoShipsLeft(0))
        cout << players[1]->getName() << " has won! (Score: " << game->getPoints(0) << "-" << game->getPoints(1) << ")" << endl;
    else
        cout << players[0]->getName() << " has won! (Score: " << game->getPoints(0) << "-" << game->getPoints(1) << ")" << endl;

    // Delete all objects
    delete game;
    for (int i = 0; i < 2; i++)
        delete players[i];
    delete[] players;
    delete deck;

    return 0;
}
