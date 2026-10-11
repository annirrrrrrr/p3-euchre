#include "Player.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}

TEST(test_make_trump_round_one) {
    Player *p = Player_factory("Alice", "Simple");
    p->add_card(Card(QUEEN, HEARTS));
    p->add_card(Card(NINE, CLUBS));

    Suit suit = SPADES;
    ASSERT_FALSE(p->make_trump(Card(NINE, HEARTS), false, 1, suit));
    ASSERT_EQUAL(suit, SPADES);

    p->add_card(Card(JACK, DIAMONDS));
    ASSERT_TRUE(p->make_trump(Card(NINE, HEARTS), false, 1, suit));
    ASSERT_EQUAL(suit, HEARTS);

    delete p;
}

TEST(test_make_trump_round_two) {
    Player *p = Player_factory("Bob", "Simple");
    p->add_card(Card(ACE, HEARTS));

    Suit suit = SPADES;
    ASSERT_FALSE(p->make_trump(Card(NINE, HEARTS), false, 2, suit));
    ASSERT_EQUAL(suit, SPADES);

    p->add_card(Card(KING, DIAMONDS));
    ASSERT_TRUE(p->make_trump(Card(NINE, HEARTS), false, 2, suit));
    ASSERT_EQUAL(suit, DIAMONDS);

    delete p;
}

TEST(test_dealer_round_two) {
    Player *p = Player_factory("Bob", "Simple");
    p->add_card(Card(NINE, HEARTS));

    Suit suit = HEARTS;
    ASSERT_TRUE(p->make_trump(Card(ACE, SPADES), true, 2, suit));
    ASSERT_EQUAL(suit, CLUBS);

    delete p;
}

TEST(test_add_and_discard) {
    Player *p = Player_factory("Alice", "Simple");
    p->add_card(Card(ACE, SPADES));
    p->add_card(Card(KING, CLUBS));
    p->add_card(Card(NINE, DIAMONDS));
    p->add_card(Card(QUEEN, HEARTS));
    p->add_card(Card(TEN, HEARTS));

    p->add_and_discard(Card(ACE, HEARTS));

    // Nine of Diamonds should be gone.
    ASSERT_EQUAL(p->lead_card(HEARTS), Card(ACE, SPADES));
    ASSERT_EQUAL(p->lead_card(HEARTS), Card(KING, CLUBS));
    ASSERT_EQUAL(p->lead_card(HEARTS), Card(ACE, HEARTS));
    ASSERT_EQUAL(p->lead_card(HEARTS), Card(QUEEN, HEARTS));
    ASSERT_EQUAL(p->lead_card(HEARTS), Card(TEN, HEARTS));

    delete p;
}

TEST(test_lead_non_trump) {
    Player *p = Player_factory("Alice", "Simple");
    p->add_card(Card(JACK, HEARTS));
    p->add_card(Card(NINE, CLUBS));
    p->add_card(Card(ACE, SPADES));

    ASSERT_EQUAL(p->lead_card(HEARTS), Card(ACE, SPADES));
    ASSERT_EQUAL(p->lead_card(HEARTS), Card(NINE, CLUBS));
    ASSERT_EQUAL(p->lead_card(HEARTS), Card(JACK, HEARTS));

    delete p;
}

TEST(test_lead_all_trump) {
    Player *p = Player_factory("Alice", "Simple");
    p->add_card(Card(ACE, HEARTS));
    p->add_card(Card(JACK, DIAMONDS));
    p->add_card(Card(JACK, HEARTS));

    ASSERT_EQUAL(p->lead_card(HEARTS), Card(JACK, HEARTS));
    ASSERT_EQUAL(p->lead_card(HEARTS), Card(JACK, DIAMONDS));
    ASSERT_EQUAL(p->lead_card(HEARTS), Card(ACE, HEARTS));

    delete p;
}

TEST(test_play_follow_suit) {
    Player *p = Player_factory("Bob", "Simple");
    p->add_card(Card(NINE, CLUBS));
    p->add_card(Card(KING, CLUBS));
    p->add_card(Card(JACK, HEARTS));

    Card led(TEN, CLUBS);
    ASSERT_EQUAL(p->play_card(led, HEARTS), Card(KING, CLUBS));
    ASSERT_EQUAL(p->play_card(led, HEARTS), Card(NINE, CLUBS));

    delete p;
}

TEST(test_play_cannot_follow) {
    Player *p = Player_factory("Bob", "Simple");
    p->add_card(Card(NINE, HEARTS));
    p->add_card(Card(TEN, CLUBS));
    p->add_card(Card(ACE, DIAMONDS));

    ASSERT_EQUAL(
        p->play_card(Card(KING, SPADES), HEARTS),
        Card(TEN, CLUBS));

    delete p;
}

TEST(test_play_left_bower) {
    Player *p = Player_factory("Bob", "Simple");
    p->add_card(Card(JACK, DIAMONDS));
    p->add_card(Card(NINE, DIAMONDS));
    p->add_card(Card(ACE, HEARTS));

    ASSERT_EQUAL(
        p->play_card(Card(KING, DIAMONDS), HEARTS),
        Card(NINE, DIAMONDS));

    ASSERT_EQUAL(
        p->play_card(Card(NINE, HEARTS), HEARTS),
        Card(JACK, DIAMONDS));

    delete p;
}

TEST_MAIN()
