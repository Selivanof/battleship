#include <cstdlib>
#include "player.h"
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

enum GuessTactic { Search, Destroy, Random, EdgeCheck, PreviousHits};//Different modes that determine the smartplayer's guess method
//Detailed explanations of these are included below, at the respective places in guessSquare()
GuessTactic currentTactic = Search;//The current guess method


int coordinates = 00;//Saves the coordinates of the desirable square so that they are returned by guessSquare
//(also used to check the square that was hit last round and as a "cursor" of sorts during destroy mode)
int lastSuccessfulHit = 00; //The last successfully hit square

int LayoutIndex = 0;

int probabilities[10][10];//A 2D array that emulates a 10x10 grid and stores the probabilities of a ship being at each square

int destroyInitiator = 00; //The coordinates of the square that initiated "Destroy" mode
list<char> directionPriority;// Stores the priority of the directions the smartplayer should guess when in Destroy mode
char currentShipOrientation;//The orientation of the ship that is targeted by Destroy Mode

vector<int> edgeSquares;//The squares near the edges of already hit "ships"

vector<int> currentHits;//The squares hit this round (that have a ship)
vector<vector <int>> previousHits;//The squares his previous round (that have a ship)
int currGameIndex = 1;//The current consecutive game that smartPlayer is playing

vector<int> inEveryGame;//Holds the coordinates of the squares that had a ship every single game before



void squaresInEveryGame()//Checks for squares that appear in every single vector in previousHits and adds their coordinates to inEveryGame
{
	if (previousHits.size() > 3)
	{
		map<int, int> M;
		for (int k = 1; k < previousHits.size(); k++)
		{
				for (int i = 0; i<previousHits[k].size(); i++) {


				if (M.find(previousHits[k][i]) == M.end()) {
					M[previousHits[k][i]] = 1;
				}


				else {
					M[previousHits[k][i]]++;
				}
			}
		}




		for (auto& it : M) {
			if (it.second == previousHits.size() - 1)
			{
				inEveryGame.push_back(it.first);
			}
		}
	}

}

SmartPlayer::SmartPlayer(int id) : Player(id) {
	name = "Team 046";

	currGameIndex++;//We start the next game
	previousHits.push_back(currentHits);//add the squares that had a ship in the previous round in previousHits

	//Resetting our variables to the default values
	currentTactic = Search;

	coordinates = 00;
	lastSuccessfulHit = 00;
	inEveryGame.clear();

	destroyInitiator = 00;
	edgeSquares.clear();
	directionPriority.clear();

	currentHits.clear();

	squaresInEveryGame();
}

void SmartPlayer::placeShips() {

	if (currGameIndex != 1)
	{
		if (!(previousHits.empty()))
		{
			if (previousHits[previousHits.size() - 1].size() != 31)//If we didn't win last game
			{
				LayoutIndex++;//Change our layout
			}
		}
	}

	if (LayoutIndex > 3) LayoutIndex = 0;//If our layout index is out of bounds, reset it

	switch (LayoutIndex)
	{
	case 0:
		grid->placeShip(8, 2, 'r', ships[0]);
		grid->placeShip(1, 3, 'r', ships[1]);
		grid->placeShip(1, 8, 'd', ships[2]);
		grid->placeShip(3, 2, 'd', ships[3]);
		grid->placeShip(6, 5, 'r', ships[4]);
		grid->placeShip(5, 9, 'd', ships[5]);
		grid->placeShip(0, 0, 'd', ships[6]);
		grid->placeShip(2, 0, 'r', ships[7]);
		grid->placeShip(4, 0, 'r', ships[8]);
		grid->placeShip(4, 4, 'r', ships[9]);
		break;
	case 1:
		grid->placeShip(9, 2, 'r', ships[0]);
		grid->placeShip(5, 0, 'd', ships[1]);
		grid->placeShip(1, 9, 'd', ships[2]);
		grid->placeShip(7, 9, 'd', ships[3]);
		grid->placeShip(0, 6, 'r', ships[4]);
		grid->placeShip(1, 0, 'd', ships[5]);
		grid->placeShip(6, 8, 'r', ships[6]);
		grid->placeShip(2, 4, 'd', ships[7]);
		grid->placeShip(6, 4, 'r', ships[8]);
		grid->placeShip(4, 2, 'r', ships[9]);
		break;
	case 2:
		grid->placeShip(1, 0, 'd', ships[0]);
		grid->placeShip(4, 9, 'd', ships[1]);
		grid->placeShip(9, 6, 'r', ships[2]);
		grid->placeShip(0, 5, 'r', ships[3]);
		grid->placeShip(9, 2, 'r', ships[4]);
		grid->placeShip(7, 1, 'r', ships[5]);
		grid->placeShip(0, 9, 'd', ships[6]);
		grid->placeShip(0, 2, 'r', ships[7]);
		grid->placeShip(1, 4, 'd', ships[8]);
		grid->placeShip(8, 0, 'd', ships[9]);
		break;
	case 3:
		grid->placeShip(8, 4, 'r', ships[0]);
		grid->placeShip(2, 2, 'd', ships[1]);
		grid->placeShip(4, 6, 'r', ships[2]);
		grid->placeShip(6, 1, 'r', ships[3]);
		grid->placeShip(0, 4, 'd', ships[4]);
		grid->placeShip(1, 7, 'd', ships[5]);
		grid->placeShip(0, 0, 'r', ships[6]);
		grid->placeShip(9, 0, 'r', ships[7]);
		grid->placeShip(5, 6, 'r', ships[8]);
		grid->placeShip(3, 0, 'd', ships[9]);
		break;
	}
}

int BestGuess()//Finds the most probable square based on the probabilities array
{
	int max = 0; //The maximum different ways a ship can be placed at a square (based on the integers in probabilities)
	int x_max = 0; //The x coordinate of the square with the maximum ways a ship can be placed
	int y_max = 0; //The y coordinate of the square with the maximum ways a ship can be placed

	for (int a = 0; a < 10; a++)
	{
		for (int b = 0; b < 10; b++)
		{
			if (probabilities[a][b] > max)
			{
				max = probabilities[a][b];
				x_max = a;
				y_max = b;
			}
		}
	}
	return x_max * 10 + y_max;// Returns the coordinates as a single integer, in the form of xy
}

bool SquareInLimits(int x, int y)//Checks if the square with the given coordinates is within grid limits - returns true if it is - false otherwise
{
	if (x >= 0 && x < 10 && y >= 0 && y < 10)
	{
		return true;
	}
	return false;
}

void addEdgeSquare(int x, int y)//Adds the correct squares to the edgeSquares vector (based on edge square coordinates)
{

	if (currentShipOrientation == 'v')//If the ship was vertical, it adds the squares below and above the edge
	{

		if (SquareInLimits(x, y - 1)) edgeSquares.push_back(x * 10 + y - 1);
		if (SquareInLimits(x, y + 1)) edgeSquares.push_back(x * 10 + y + 1);
	}
	else//If it was horizontal, it adds the left-right squares
	{

		if (SquareInLimits(x + 1, y)) edgeSquares.push_back((x + 1) * 10 + y);
		if (SquareInLimits(x - 1, y)) edgeSquares.push_back((x - 1) * 10 + y);
	}

}


//Checks if addEdgeSquare was last called with the given x and y coordinates (works only if edgeSquares is not empty)
bool lastAddedEdge(int x, int y)
{
	int lastEdgeCoordinates = 00;
	if (currentShipOrientation == 'v')
	{

		if (SquareInLimits(x, y - 1)) lastEdgeCoordinates = (x * 10 + y - 1);
		if (SquareInLimits(x, y + 1)) lastEdgeCoordinates = (x * 10 + y + 1);
	}
	else
	{

		if (SquareInLimits(x + 1, y)) lastEdgeCoordinates = ((x + 1) * 10 + y);
		if (SquareInLimits(x - 1, y)) lastEdgeCoordinates = ((x - 1) * 10 + y);
	}

	if(!(edgeSquares.empty()))  if (lastEdgeCoordinates == edgeSquares.back()) return true;
	return false;
}


//Removes the current direction with the most priority from the directionPriority list and resets the coordinates
//It also calls addEdgeSquare if we already know the ship's direction
void removeCurrentDirection()
{
	if (!(directionPriority.empty()))
	{
		directionPriority.pop_front();
	}

	coordinates = destroyInitiator;
	if (currentShipOrientation != 'u')//If the ship's direction is already found - (Since we want to remove the current direction we've reached the edge of the ship)
	{

		//If the first hit of the ship happened to be on the edge and destroy mode checked the direction
		//in which the ship extends BEFORE checking the opposite one (e.g. if the ship is extending to the north, the
		//first hit was on its most southern square and we checked north before checking south), then we should add to edgeSquares
		//the squares next to destroy initiator. This will have happened if the last added coordinates to edgeSquares matches
		//the one corresponding to the lastSuccessfulHit (which would have been on the other side of the ship).
		if (lastAddedEdge(lastSuccessfulHit / 10, lastSuccessfulHit % 10))
		{
			addEdgeSquare(destroyInitiator / 10, destroyInitiator % 10);
		}
		else
		{
			addEdgeSquare(lastSuccessfulHit / 10, lastSuccessfulHit % 10);
		}
	}
}

Square* SmartPlayer::guessSquare() {

	if (opponentGrid->hasShip(coordinates / 10, coordinates % 10))//If the last hit was successful
	{
		lastSuccessfulHit = coordinates;

		if (currentTactic != Destroy) //If Destroy mode is not already active
		{
			destroyInitiator = coordinates;//Save the corrent coordinates to the destroyInitiator
			currentTactic = Destroy;//Activate destroy mode
		}
	}
	else//If the square we hit last round didn't have a ship
	{
		if (!(currentHits.empty()))
		{
			currentHits.pop_back();//Remove it from currentHits
		}

	}

	//Clears the probabilities array
	for (int a = 0; a < 10; a++)
	{
		for (int b = 0; b < 10; b++)
		{
			probabilities[a][b] = 0;
		}
	}


	//Calculates probabilities by finding every possible way each ship can be placed
	for (int i = 0; i < 10; i++)//For every ship
	{
		for (int x = 0; x < 10; x++)//For every x
		{
			for (int y = 0; y < 10; y++)//For every y
			{
				if (opponentGrid->shipCanBePlaced(x, y, 'r', ships[i]))//If the ship can be placed right (starting on that square)
				{
					for (int j = 0; j < ships[i]->getSize(); j++)//Add +1 probability to every square it would occupy
					{
						probabilities[x][y + j] ++;
					}
				}
				if (opponentGrid->shipCanBePlaced(x, y, 'd', ships[i]))//Same as above but for down placement
				{
					for (int j = 0; j < ships[i]->getSize(); j++)
					{
						probabilities[x + j][y] ++;
					}
				}
			}
		}
	}


	// -----DESTROY MODE-----
	//
	//This mode is activated after smartPlayer lands a successful hit (that occured in one of the other modes). It basically starts checking every direction
	//around that square until it lands another successful hit. When this happens, it marks the ship's orientation, and it keeps searching only in the 2 directions
	//relevant to that orientation. It stops when it lands an unsuccessful hit on both sides of that ship, at which point it switches to EdgeCheck mode.
	if (currentTactic == Destroy)
	{
		if (coordinates == destroyInitiator)//Ship just discovered
		{
			currentShipOrientation = 'u';//We don't know the ship's direction
			directionPriority.clear();//Clear any previous priorities that might still be in the list (for extra safety)

			int North = coordinates / 10; //Distance North
			int South = 9 - North;  //Distance South
			int West = coordinates % 10;//Distance West
			int East = 9 - West;  //Distance East

			//Create an array of pairs - each distance is paired with the respective direction character (so it can be sorted and saved in directionPriority)
			pair <int, char> distances[4] = { make_pair(North,'n'), make_pair(South, 's'), make_pair(West,'w'), make_pair(East,'e') };

			//Make a vector to save the items of the array in
			vector <pair <int, char>> distancesVector;

			for (int i = 0; i < 4; i++)
			{
				distancesVector.push_back(distances[i]);//We put the pairs of the distance array into the vector
			}

			sort(distancesVector.begin(), distancesVector.end());//We sort the vector based on the distance (first element of the pair)

			for (int i = 0; i < 4; i++)
			{
				directionPriority.push_front(distancesVector[i].second);//We add the directions to the directionPriority list after they have been sorted
				//This way our smartplayer will prioritize the directions that have the most empty space
			}
		}
		else//We are already in the process of destroying a ship
		{
			if (lastSuccessfulHit != coordinates) //If we didn't hit a ship last round
			{
				removeCurrentDirection();//The smartplayer should not keep searching that way - remove the direction from the list
			}
			else if (currentShipOrientation == 'u')//We hit the ship but we didn't know its direction
			{
				if (directionPriority.front() == 'n' || directionPriority.front() == 's')
				{
					directionPriority.remove('e');
					directionPriority.remove('w');

					currentShipOrientation = 'v';
				}
				else//Same as above but for horizontal
				{
					directionPriority.remove('s');
					directionPriority.remove('n');

					currentShipOrientation = 'h';
				}


				//If there is only 1 direction left it means the first hit was on the edge of the ship
				//because the smartPlayer has already checked the opposite way and didn't find anything - so it removed the opposite direction
				//(otherwise the opposite direction should still be in the list)
				if (directionPriority.size() == 1)
				{
					addEdgeSquare(destroyInitiator / 10, destroyInitiator % 10);
				}
			}
		}

		if (!(directionPriority.empty()))//If we still have at least one direction to search
		{
			bool foundSquare = false;
			while (foundSquare == false)
			{
				if (directionPriority.empty())
				{
					//If during the loop the directionPriority list got empty
					foundSquare = true;//Break the loop
					currentTactic = PreviousHits;//Switch to edgeCheck mode
				}
				else
				{
					switch (directionPriority.front())
					{
					case 'n':
						if (SquareInLimits(coordinates / 10 - 1, coordinates % 10))
						{
							if (opponentGrid->isHit(coordinates / 10 - 1, coordinates % 10))
							{
								if (opponentGrid->hasShip(coordinates / 10 - 1, coordinates % 10))//If the following square north is occupied by a ship
								{
									//We set the coordinates to those of the occupied square so that when the loop runs again it will keep going north
									//We basically want to "bypass" any occupied squares that are already hit cause they might belong to the
									//ship we are currently trying to destroy
									coordinates = coordinates - 10;
								}
								else removeCurrentDirection();
							}
							else
							{
								//Found a valid square
								coordinates -= 10;
								foundSquare = true;
							}
						}
						else  removeCurrentDirection();
						break;
					case 's'://Same as above
						if (SquareInLimits(coordinates / 10 + 1, coordinates % 10))
						{
							if (opponentGrid->isHit(coordinates / 10 + 1, coordinates % 10))
							{
								if (opponentGrid->hasShip(coordinates / 10 + 1, coordinates % 10))
								{
									coordinates = coordinates + 10;
								}
								else removeCurrentDirection();
							}
							else
							{
								coordinates += 10;
								foundSquare = true;
							}
						}
						else
						{
							removeCurrentDirection();
						}
						break;

					case 'w': //Same as above
						if (SquareInLimits(coordinates / 10, coordinates % 10 - 1))
						{
							if (opponentGrid->isHit(coordinates / 10, coordinates % 10 - 1))
							{
								if (opponentGrid->hasShip(coordinates / 10, coordinates % 10 - 1))
								{
									coordinates = coordinates - 1;
								}
								else
								{
									removeCurrentDirection();
								}
							}
							else
							{
								coordinates -= 1;
								foundSquare = true;
							}
						}
						else
						{
							removeCurrentDirection();
						}
						break;
					case 'e': //Same as above
						if (SquareInLimits(coordinates / 10, coordinates % 10 + 1))
						{
							if (opponentGrid->isHit(coordinates / 10, coordinates % 10 + 1))
							{
								if (opponentGrid->hasShip(coordinates / 10, coordinates % 10 + 1))
								{
									coordinates = coordinates + 1;
								}
								else
								{
									removeCurrentDirection();
								}
							}
							else
							{
								coordinates += 1;
								foundSquare = true;
							}
						}
						else
						{
							removeCurrentDirection();
						}
						break;
					}
				}
			}
		}
		else//If we searched all directions switch to EdgeCheck mode
		{
			currentTactic = PreviousHits;
		}



	}

	//We hit every square that had a ship in all of the previous rounds
	if (currentTactic == PreviousHits)
	{
		bool foundSquare = false;
		while (foundSquare == false)
		{
			if (inEveryGame.size() == 0)
			{
				foundSquare = true;
				currentTactic = EdgeCheck;
			}
			else
			{
				if (!(opponentGrid->isHit(inEveryGame[0] / 10, inEveryGame[0] % 10)))//Valid square - we can hit it
				{
					coordinates = inEveryGame[0];
					foundSquare = true;
				}
				inEveryGame.erase(inEveryGame.begin());//We remove that square from the vector (either because we are gonna hit or because it is invalid)
			}
		}

	}


	// -----EDGE CHECK MODE-----
	//
	//Destroy mode does not know if every square it sucessfully hit in a straight line belongs to the same ship. To check for
	//touching ships, we check only the squares near the edges of that line, instead of all the adjacent squares.
	//If only 2 ships are adjacent (and we've hit both of them at least once in a straight line), it is certain that the remaining
	// squares will be next to one of those edges. The chances that the remaining squares are NOT
	//near the edges are really small (and requires at least 3 adjacent ships), so we use this method which is far more efficient
	//than checking every single adjacent square.
	//Not checking at all is not an option and the reason for that is explained in RANDOM MODE
	if (currentTactic == EdgeCheck)
	{
		bool foundSquare = false;
		while (foundSquare == false)
		{
			if (edgeSquares.size() == 0)
			{
				foundSquare = true;
				currentTactic = Search;
			}
			else
			{
				if (!(opponentGrid->isHit(edgeSquares[0] / 10, edgeSquares[0] % 10)))//Valid square - we can hit it
				{
					coordinates = edgeSquares[0];
					foundSquare = true;
				}
				edgeSquares.erase(edgeSquares.begin());//We remove that square from the vector (either because we are gonna hit or because it is invalid)
			}
		}

	}





	// -----SEARCH ALL MODE-----
	//
	//Uses the probability array to find the best guessing location. The probability array works like this:
	//The program goes through every ship and checks if can be placed at every square of the grid, both vertically and horizontally
	//If it can, it adds +1 to the array elements with the same x and y (array[x][y]) as EVERY square it would occupy. It does this for every ship.
	//The largest number in the array indicates the most probably location, so BestGuess returns the coordinates of that array element (in the form of xy)
	if (currentTactic == Search)
	{
		coordinates = BestGuess();
	}

	if (opponentGrid->isHit(coordinates / 10, coordinates % 10))//When SearchAll fails, we start hitting random squares (should be very rare)
	{
		currentTactic = Random;
	}

	// -----RANDOM MODE-----
	//
	//A failsafe method, when SearchAll fails
	//This happens very rarely (3 or more ships must be next to each other for this to happen). Example:
	//Here we have 3 destroyers in a _|_ formation.
	//
	// 	 ----------
	//   ----#-----
	//   --#####---
	//   ----------
	//
	//If the smartPlayer finds the 5 squares that are connected, it will not find the second square of the vertical destroyer
	//Now if SearchAll happens to hit that square there will be no problem. But there is a possibility that it will
	//hit all the squares adjacent to the one we want. In that case SearchAll will assume there is no way for
	//a ship to be placed in a single square, because to that function the grid is like this:
	//
	// 	 ----+-----
	//   ---+@+----
	//   --+++++---
	//   ----------
	//
	//There is no reason to search the @ marked square. In that case, random mode is activated when SearchAll runs out of squares.
	//
	//Note that if Edge Check mode did not exist, the same would happen far more often (e.g. every time a destroyer was touching another ship)
	//Based on tests with random ship placements, this happened more than 50% of the times. Adding Edge Check lowered that percentage to about 4%
	if (currentTactic == Random)
	{
		int x, y;
		do {
			x = rand() % 10;
			y = rand() % 10;
		} while (opponentGrid->isHit(x, y));
		coordinates = x * 10 + y;//Returns random coordinates
	}


	currentHits.push_back(coordinates);

	return opponentGrid->getSquare(coordinates / 10, coordinates % 10);
}


bool SmartPlayer::moveSubmarine() {
	return false;
}

bool SmartPlayer::doubleHit() {
	return true;
}
