#ifndef DECK_H
#define DECK_H

#include "card.h"

/**
 * Class that implements a deck of cards. It shuffles the cards and
 * provides the next card when required.
 */
class Deck
{
private:
    Card** cards;
    int deckSize; // the number of cards in the deck
    int currentIndex; // holds the current distance from the top of the card deck

public:
    /**
     * Initializes this deck of cards creating different cards.
     */
    Deck();

    /**
     * Destroys this deck of cards by deleting also all the cards.
     */
    ~Deck();

    /**
     * Returns the next card from the top of the deck. If the returned card is the
     * last card of the deck, then (upon returning) the cards are shuffled.
     *
     * @return Card* the card from the top of the deck.
     */
    Card* getNextCard();

    /**
     * Shuffles the cards of the deck.
     */
    void shuffleCards();
};

#endif // DECK_H
