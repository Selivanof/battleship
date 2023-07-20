#include <iostream>
#include <cstdlib>
#include "player.h"

Player::Player(int id) {
    this->id = id;
	if (id == 0) name = "Player 1";
	else if (id == 1) name = "Player 2";
    missTurn = false;
    points = 0;
    grid = new Grid();
    opponentGrid = new Grid();
    ships = new Ship*[10];
    createShips();
}

Player::~Player(){
    delete grid;
    delete opponentGrid;
    for (int i = 0; i < 10; i++)
        delete ships[i];
    delete[] ships;
}

void Player::missNextTurn() {
    missTurn = true;
}

bool Player::mustMissNextTurn() {
    if (missTurn) {
        missTurn = false;
        return true;
    }
    return false;
}

string Player::getName() {
    return name;
}

Grid* Player::getGrid(){
    return grid;
}

Ship* Player::getShip(int index){
    return ships[index];
}

int Player::numberOfShipsThatAreSunk() {
    int sunkShips = 0;
    for (int i = 0; i < 10; i++) {
        if (ships[i]->isSunk())
            sunkShips++;
    }
    return sunkShips;
}

bool Player::canMoveSubmarine() {
    if (((Submarine*)ships[5])->canMove()){
        // Check if there is at least one location (consecutive squares) where the submarine can be moved
        char directions[] = {'r', 'd'};
        for (int k = 0; k < 2; k++){
            char direction = directions[k];
            for (int i = 0; i < 10; i++){
                for (int j = 0; j < 10; j++){
                    if (grid->shipCanBePlaced(i, j, direction, ships[5]))
                        return true;
                }
            }
        }
    }
    return false;
}

bool Player::canDoubleHit() {
    return ((Carrier*)ships[0])->canDoubleHit();
}

bool Player::acceptHit(int x, int y) {
    return grid->acceptHit(x, y);
}

void Player::createShips() {
    ships[0] = new Carrier();
    ships[1] = new Battleship();
    ships[2] = new Battleship();
    ships[3] = new Cruiser();
    ships[4] = new Cruiser();
    ships[5] = new Submarine(2);
    ships[6] = new Destroyer();
    ships[7] = new Destroyer();
    ships[8] = new Destroyer();
    ships[9] = new Destroyer();
};

string Player::gridToString(bool hide) {
    stringstream sstm;
    sstm << "     _______________________ \n"
         << "    |                       |\n"
         << "    |   0 1 2 3 4 5 6 7 8 9 |\n"
         << "    |                       |\n";
    for(int i = 0; i < 10; i++) {
        sstm << "    | " << ((char)('A' + i)) << " ";
        for(int j = 0; j < 10; j++) {
            if(grid->hasShip(i, j) && grid->isHit(i, j)) sstm << "# ";
            else if(!grid->hasShip(i, j) && grid->isHit(i, j)) sstm << "+ ";
            else if(grid->hasShip(i, j) && !hide) sstm << "# ";
            else sstm << "- ";
        }
        sstm << "|\n";
    }
    sstm << "    |_______________________|";
    string result = sstm.str();
    return result;
};

void Player::addPoints(int points){
    this->points += points;
}

void Player::removePoints(int points){
    this->points -= points;
}

int Player::getPoints(){
    return points;
}

ComputerPlayer::ComputerPlayer(int id) : Player(id) {

}

void ComputerPlayer::placeShips() {
    for (int i = 0; i < 10; i++){
        // for each ship figure out a random placement
        int x, y;
        char z;
        do {
            x = rand() % 10;
            y = rand() % 10;
            z = (rand() % 2 == 0) ? 'r' : 'd';
        } while(!grid->shipCanBePlaced(x, y, z, ships[i]));
        grid->placeShip(x, y, z, ships[i]);
    }
};

Square* ComputerPlayer::guessSquare() {
    int x, y;
    // find a random square that is not already hit
    do {
        x = rand() % 10;
        y = rand() % 10;
    } while(opponentGrid->isHit(x, y));
    return opponentGrid->getSquare(x, y);
}

bool ComputerPlayer::moveSubmarine() {
    if (rand() % 2 == 0){ // used to decide randomly whether to move or not
        grid->removeShip(ships[5]);
        // figure out a random placement
        int x, y;
        char z;
        do {
            x = rand() % 10;
            y = rand() % 10;
            z = (rand() % 2 == 0) ? 'r' : 'd';
        } while(!grid->shipCanBePlaced(x, y, z, ships[5]));
        // place the ship to the new position
        grid->placeShip(x, y, z, ships[5]);
        return true;
    }
    return false;
}

bool ComputerPlayer::doubleHit() {
    return (rand() % 2 == 0); // used to decide randomly whether to double hit or not
}

HumanPlayer::HumanPlayer(int id) : Player(id) {

}

void HumanPlayer::placeShips() {
    cout << name << ", place your ships!" << endl;
    string answer;
    do {
        cout << "Do you want to place the ships or leave it at random (p/r)? ";
        cin >> answer;
    } while(answer != "p" && answer != "r");

    if (answer == "p") {
        for (int i = 0; i < 10; i++){
            cout << gridToString(false) << endl;
            // for each ship figure out its placement
            cout << "Place ship no " << (i+1) << " (size: " << ships[i]->getSize() << "). ";
            string place;
            int x, y;
            char z;
            bool placeIsNotValid = false;
            do {
                cout << "Give square and direction (e.g. C4r or B5d): ";
                cin >> place;
                x = place[0] - 'A';
                y = place[1] - '0';
                z = place[2];
                placeIsNotValid = (place.length() != 3 || x < 0 || x > 9 || y < 0 || y > 9 || (z != 'r' && z != 'd'));
            } while(placeIsNotValid || !grid->shipCanBePlaced(x, y, z, ships[i]));
            grid->placeShip(x, y, z, ships[i]);
        }
    }
    else{
        for (int i = 0; i < 10; i++){
            // for each ship figure out a random placement
            int x, y;
            char z;
            do {
                x = rand() % 10;
                y = rand() % 10;
                z = (rand() % 2 == 0) ? 'r' : 'd';
            } while(!grid->shipCanBePlaced(x, y, z, ships[i]));
            grid->placeShip(x, y, z, ships[i]);
        }
    }
};

Square* HumanPlayer::guessSquare() {
    string guess;
    int x, y;
    bool guessIsNotValid = false;
    do {
        cout << name << ", give your guess (e.g. C4): ";
        cin >> guess;
        x = guess[0] - 'A';
        y = guess[1] - '0';
        guessIsNotValid = (guess.length() != 2 || x < 0 || x > 9 || y < 0 || y > 9);
    } while(guessIsNotValid || opponentGrid->isHit(x, y));
    return opponentGrid->getSquare(x, y);
}

bool HumanPlayer::moveSubmarine() {
    string answer;
    do {
        cout << name << ", do you want to move the submarine (y/n)? ";
        cin >> answer;
    } while(answer != "y" && answer != "n");

    if (answer == "y"){
        grid->removeShip(ships[5]);
        string place;
        int x, y;
        char z;
        bool placeIsNotValid = false;
        do {
            cout << "Give the new location of the submarine (e.g. C4r or B5d): ";
            cin >> place;
            x = place[0] - 'A';
            y = place[1] - '0';
            z = place[2];
            placeIsNotValid = (place.length() != 3 || x < 0 || x > 9 || y < 0 || y > 9 || (z != 'r' && z != 'd'));
        } while(placeIsNotValid || !grid->shipCanBePlaced(x, y, z, ships[5]));
        grid->placeShip(x, y, z, ships[5]);
        return true;
    }
    return false;
}

bool HumanPlayer::doubleHit() {
    string answer;
    do {
        cout << name << ", do you want to perform a double hit (y/n)? ";
        cin >> answer;
    } while(answer != "y" && answer != "n");

    return (answer == "y");
}

