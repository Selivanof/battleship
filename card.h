#ifndef CARD_H
#define CARD_H

#include <string>
#include "player.h"

/**
 * Class denoting a card. This class has to be extended in order to
 * provide the type of card (PointCard or MoveCard).
 */
class Card {
private:
    int id;

protected:
    string text;

public:
    /**
     * Initializes the card with its id and text.
     *
     * @param id the unique identifier of the card.
     * @param text the text of the card.
     */
    Card(int id, string text);

    /**
     * Deletes the card (does nothing).
     * You must not implement this method.
     */
    virtual ~Card(){};

    /**
     * Returns the id of the card.
     *
     * @return the id of the card.
     */
    int getId();

    /**
     * Returns the text of the card.
     *
     * @return the text of the card.
     */
    string getText();

    /**
     * Implements the action of the card (point action or move action).
     * This function has to be implemented in classes extending this card class.
     *
     * @param player the player that has drawn the card.
     * @param opponent the opponent that participates in the card action.
     */
    virtual void cardAction(Player* player, Player* opponent) = 0;
};

/**
 * Class denoting a card of type "point". When the player draws this card, a
 * change in points takes place.
 */
class PointCard : public Card {
private:
    int points;

public:
    /**
     * Initializes the point card.
     *
     * @param id the unique identifier of the card.
     * @param text the text of the card.
     * @param points the number of points relevant to this card.
     */
    PointCard(int id, string text, int points);

    /**
     * Implements the point action between the player and the opponent and player.
     * Transfers the number of points of this card from the player to the opponent.
     *
     * Hint: there are positive and negative points indicating whether the points must
     *       be added or removed. When adding a number of points to the player, the same
     *       number of points exactly must be removed from the opponent.
     *
     * @param player the player that has drawn the card.
     * @param opponent the opponent that participates in the card action.
     */
    void cardAction(Player* player, Player* opponent);
};

/**
 * Enumeration used to represent the different types of moves.
 */
enum MoveType {DoNothing, PlayTwice, MissTurn};

/**
 * Class denoting a card of type move. When the player draws this card, then
 * an action is made that concerns a move in the game.
 */
class MoveCard : public Card {
private:
    MoveType type;

public:
    /**
     * Initializes the move card.
     *
     * @param id the unique identifier of the card.
     * @param text the text of the card.
     * @param type the type of the move of this card.
     */
    MoveCard(int id, string text, MoveType type);

    /**
     * Implements the move action for the player. Selects which player is going to miss a turn.
     * and calls the missNextTurn method of the player or the opponent accordingly.
     *
     * @param player the player that has drawn the card.
     * @param opponent the opponent that participates in the card action.
     */
    void cardAction(Player* player, Player* opponent);
};

#endif
