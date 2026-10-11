#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}

// Add more test cases here
TEST(test_left_bower) {
  Card c(JACK, DIAMONDS);
  ASSERT_TRUE(c.is_left_bower(HEARTS));
  ASSERT_TRUE(c.is_trump(HEARTS));
  ASSERT_EQUAL(HEARTS, c.get_suit(HEARTS));
}

TEST(test_trump_order) {
  ASSERT_TRUE(Card_less(Card(JACK, DIAMONDS), Card(JACK, HEARTS), HEARTS));
  ASSERT_TRUE(Card_less(Card(ACE, HEARTS), Card(JACK, DIAMONDS), HEARTS));
  ASSERT_TRUE(Card_less(Card(ACE, SPADES), Card(NINE, HEARTS), HEARTS));
}

TEST(test_led_suit) {
  Card led(KING, SPADES);
  ASSERT_TRUE(Card_less(Card(ACE, CLUBS), Card(NINE, SPADES), led, HEARTS));
}

TEST(test_led_left_bower) {
  Card led(JACK, DIAMONDS);
  ASSERT_TRUE(Card_less(Card(NINE, DIAMONDS), Card(TEN, CLUBS), led, HEARTS));
}

TEST_MAIN()
