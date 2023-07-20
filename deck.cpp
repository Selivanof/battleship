#include "deck.h"
#include "shuffler.h"

Deck::Deck()
{
    deckSize = 20;
    cards = new Card*[deckSize];

    cards[0] = new PointCard(0, " earns 1 point!", 1);
    cards[1] = new PointCard(1, " earns 1 point!", 1);
    cards[2] = new PointCard(2, " earns 3 points!", 3);
    cards[3] = new PointCard(3, " earns 3 points!", 3);
    cards[4] = new PointCard(4, " earns 5 points!", 5);
    cards[5] = new PointCard(5, " loses 1 point!", -1);
    cards[6] = new PointCard(6, " loses 1 point!", -1);
    cards[7] = new PointCard(7, " loses 3 points!", -3);
    cards[8] = new PointCard(8, " loses 3 points!", -3);
    cards[9] = new PointCard(9, " loses 5 points!", -5);
    cards[10] = new MoveCard(10, " does nothing..", DoNothing);
    cards[11] = new MoveCard(11, " does nothing..", DoNothing);
    cards[12] = new MoveCard(12, " plays again!", PlayTwice);
    cards[13] = new MoveCard(13, " plays again!", PlayTwice);
    cards[14] = new MoveCard(14, " plays again!", PlayTwice);
    cards[15] = new MoveCard(15, " plays again!", PlayTwice);
    cards[16] = new MoveCard(16, " misses the turn!", MissTurn);
    cards[17] = new MoveCard(17, " misses the turn!", MissTurn);
    cards[18] = new MoveCard(18, " misses the turn!", MissTurn);
    cards[19] = new MoveCard(19, " misses the turn!", MissTurn);

    shuffleCards();
}

Deck::~Deck(){
    for (int i = 0; i < deckSize; i++)
        delete cards[i];
    delete[] cards;
}

void Deck::shuffleCards()
{
    shuffle(cards, deckSize);
    currentIndex = 0;
}

Card* Deck::getNextCard()
{
    // Get the card which is currently on top
    Card* card = cards[currentIndex];
    currentIndex++;
    // Shuffle the cards if they are finished
    if (currentIndex == deckSize)
        shuffleCards();
    return card;
}

