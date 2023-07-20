#include <iomanip>
#include <iostream>
#include "game.h"

Game::Game(Deck* deck, Player** players) {
    this->deck = deck;
    this->players = players;
    score = new int[2];
    numTurns = 0;
    playerTurn = 0;
}

Game::~Game() {
    delete[] score;
}

int Game::getPlayerTurn() {
    return playerTurn;
}

int Game::getNumTurns() {
    return numTurns;
}

void Game::endTurn() {
    // Change to the other player
    playerTurn = 1 - playerTurn;
    // Check if the other player should play or if the turn is missed
    if (players[playerTurn]->mustMissNextTurn())
        playerTurn = 1 - playerTurn; // the turn is missed
    numTurns++;
}

void Game::printState() {
    cout << "     " << players[0]->getName() << " (" << players[0]->getPoints() << " points)"
        << setw(18) << players[1]->getName() << " (" << players[1]->getPoints() << " points)" << endl;
    stringstream ss(players[0]->gridToString(playerTurn != 0)), ss2(players[1]->gridToString(playerTurn == 0));
    string to, to2;
    while(getline(ss, to, '\n') && getline(ss2, to2, '\n'))
        cout << to << to2 << endl;
    cout << endl;
}

bool Game::playerHasNoShipsLeft(int index) {
    return (players[index]->numberOfShipsThatAreSunk() == 10);
}

int Game::getPoints(int index) {
    int points = players[index]->getPoints();
    points += (playerHasNoShipsLeft(index) ? 0 : 50);
    for (int i = 0; i < 10; i++) {
        Ship* ship = players[index]->getShip(i);
        switch (ship->getType()){
        case CARRIER:
            points += (ship->getSize() - ship->getNumberOfHits()) * 10;
            break;
        case BATTLESHIP:
            points += (ship->getSize() - ship->getNumberOfHits()) * 5;
            break;
        case CRUISER:
        case SUBMARINE:
        case DESTROYER:
            points += (ship->getSize() - ship->getNumberOfHits()) * 1;
            break;
        }
    }
    return points > 0 ? points : 0;
}
