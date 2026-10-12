#include <cassert>
#include <string>
#include <vector>
#include <assert.h>
#include <algorithm>
#include "Player.hpp"

using namespace std;

class SimplePlayer : public Player{
    private: 
    string name;
    vector <Card> hand;

    public:
    SimplePlayer(const string &name_in) : name(name_in){}
    
    //EFFECTS returns player's name
    const string &get_name() const override {
        return name;
    }
    
    //REQUIRES player has less than MAX_HAND_SIZE cards
    //EFFECTS  adds Card c to Player's hand
    void add_card(const Card &c) override{
        if (hand.size() < MAX_HAND_SIZE ){
            hand.push_back(c);
        }
    }

    //REQUIRES round is 1 or 2
    //MODIFIES order_up_suit
    //EFFECTS If Player wishes to order up a trump suit then return true and
    //  change order_up_suit to desired suit.  If Player wishes to pass, then do
    //  not modify order_up_suit and return false.
    bool make_trump(const Card &upcard, bool is_dealer, int round, Suit &order_up_suit) const override{
        Suit desired_suit = upcard.get_suit();
        if (round == 2){
            desired_suit = Suit_next(upcard.get_suit());
        }

        int count = 0;
        for (const Card &card : hand){
            if (card.is_face_or_ace () && card.is_trump(desired_suit))
            count ++;
        }

        if ((round == 1 && count >= 2) ||
            (round == 2 && (count >= 1 || is_dealer))) {
            order_up_suit = desired_suit;  // Add this.
            return true;
        }

        return false;
    }

    //REQUIRES Player has at least one card
    //EFFECTS  Player adds one card to hand and removes one card from hand
    void add_and_discard(const Card &upcard) override {
        hand.push_back(upcard);
        Suit trump = upcard.get_suit();

        int lowest_index = 0;
        for (int i = 0; i < static_cast<int>(hand.size()); i++){
            if (Card_less(hand[i], hand[lowest_index], trump)){
                lowest_index = i;
            }
        }
        hand.erase(hand.begin() + lowest_index);
    }

    //REQUIRES Player has at least one card
    //EFFECTS  Leads one Card from Player's hand according to their strategy
    //  "Lead" means to play the first Card in a trick.  The card
    //  is removed the player's hand.
    Card lead_card(Suit trump) override{
        int best_index = -1;

        //find the highest non-trump card
        for (int i = 0; i < static_cast<int>(hand.size()); i++){
            if (!hand[i].is_trump(trump)){
                if (best_index == -1 || Card_less(hand[best_index], hand[i], trump)){
                    best_index = i;
                }
            }
        }

        //if no non-trump cards, find the highest trump
        if (best_index == -1){
            best_index = 0;
            for (int i = 1; i < static_cast<int>(hand.size()); i++){
                if (Card_less(hand[best_index], hand[i], trump)){
                    best_index = i;
                }
            }
        }

        Card chosen = hand[best_index];
        hand.erase(hand.begin() + best_index);
        return chosen;
    }

    //REQUIRES Player has at least one card
    //EFFECTS  Plays one Card from Player's hand according to their strategy.
    //  The card is removed from the player's hand.
    Card play_card(const Card &led_card, Suit trump) override {
        Suit led_suit = led_card.get_suit(trump);
        int chosen_index = -1;

        // Find the highest card that follows suit.
        for (int i = 0; i < static_cast<int>(hand.size()); ++i) {
            if (hand[i].get_suit(trump) == led_suit) {
                if (chosen_index == -1 ||
                    Card_less(hand[chosen_index], hand[i], trump)) {
                    chosen_index = i;
                }
            }
        }

        // If we cannot follow suit, find the lowest card.
        if (chosen_index == -1) {
            chosen_index = 0;
            for (int i = 1; i < static_cast<int>(hand.size()); ++i) {
                if (Card_less(hand[i], hand[chosen_index], trump)) {
                    chosen_index = i;
                }
            }
        }

        Card chosen = hand[chosen_index];
        hand.erase(hand.begin() + chosen_index);
        return chosen;
    }

};

class HumanPlayer : public Player{
    private: 
    string name;
    vector<Card> hand;

    public:
    HumanPlayer (const string &name_in) : name(name_in){}

    const string &get_name() const override {
        return name;
    }
    //REQUIRES player has less than MAX_HAND_SIZE cards
    //EFFECTS  adds Card c to Player's hand

    void add_card(const Card &c) override{
        hand.push_back(c);
        sort(hand.begin(), hand.end());
    }

    //REQUIRES round is 1 or 2
    //MODIFIES order_up_suit
    //EFFECTS If Player wishes to order up a trump suit then return true and
    //  change order_up_suit to desired suit.  If Player wishes to pass, then do
    //  not modify order_up_suit and return false.
    bool make_trump(const Card &upcard, bool is_dealer,
                    int round, Suit &order_up_suit) const override {
        print_hand();
        cout << "Human player " << name
            << ", please enter a suit, or \"pass\":\n";

        string decision;
        cin >> decision;

        if (decision != "pass") {
            order_up_suit = string_to_suit(decision);
            return true;
        }

        return false;
    }

    //REQUIRES Player has at least one card
    //EFFECTS  Player adds one card to hand and removes one card from hand.
    void add_and_discard(const Card &upcard) override {
        print_hand();
        cout << "Discard upcard: [-1]\n";
        cout << "Human player " << name << ", please select a card to discard: \n";

        int index;
        cin >> index;

        if (index != -1) {
            hand[index] = upcard;
            sort(hand.begin(), hand.end());
        }
    }

    //REQUIRES Player has at least one card
    //EFFECTS  Leads one Card from Player's hand according to their strategy
    //  "Lead" means to play the first Card in a trick.  The card
    //  is removed the player's hand.
    Card lead_card(Suit trump) override {
    print_hand();
    cout << "Human player " << name
         << ", please select a card:\n";

    int index;
    cin >> index;

    Card chosen = hand[index];
    hand.erase(hand.begin() + index);
    return chosen;
    }

    //REQUIRES Player has at least one card
    //EFFECTS  Plays one Card from Player's hand according to their strategy.
    //  The card is removed from the player's hand.
    Card play_card(const Card &led_card, Suit trump) override {
        print_hand();
        cout << "Human player " << name
            << ", please select a card:\n";

        int index;
        cin >> index;

        Card chosen = hand[index];
        hand.erase(hand.begin() + index);
        return chosen;
    }

    void print_hand() const {
    for (size_t i=0; i < hand.size(); ++i)
        cout << "Human player " << name << "'s hand: "
            << "[" << i << "] " << hand[i] << "\n";
}

};


//EFFECTS: Returns a pointer to a player with the given name and strategy
//To create an object that won't go out of scope when the function returns,
//use "return new Simple(name)" or "return new Human(name)"
//Don't forget to call "delete" on each Player* after the game is over
Player * Player_factory(const std::string &name, 
                        const std::string &strategy) {
  // We need to check the value of strategy and return 
  // the corresponding player type.
    if (strategy == "Simple") {
    // The "new" keyword dynamically allocates an object.
    return new SimplePlayer(name);
    }
  // Repeat for each other type of Player
    if (strategy == "Human") {
    // The "new" keyword dynamically allocates an object.
    return new HumanPlayer(name);
    }
  // Invalid strategy if we get here

  assert(false);
  return nullptr;
}

//EFFECTS: Prints player's name to os
std::ostream & operator<<(std::ostream &os, const Player &p) {
    os << p.get_name();
    return os;
}
