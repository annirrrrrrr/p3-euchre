#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>
#include <sstream>

using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}

// Add more test cases here
//passed
TEST(test_left_bower) { 
  Card c(JACK, DIAMONDS);
  ASSERT_TRUE(c.is_left_bower(HEARTS));
  ASSERT_TRUE(c.is_trump(HEARTS));
  ASSERT_EQUAL(HEARTS, c.get_suit(HEARTS));
}
//passed
TEST(test_trump_order) {
  ASSERT_TRUE(Card_less(Card(JACK, DIAMONDS), Card(JACK, HEARTS), HEARTS));
  ASSERT_TRUE(Card_less(Card(ACE, HEARTS), Card(JACK, DIAMONDS), HEARTS));
  ASSERT_TRUE(Card_less(Card(ACE, SPADES), Card(NINE, HEARTS), HEARTS));
}
//passed
TEST(test_led_suit) {
  Card led(KING, SPADES);
  ASSERT_TRUE(Card_less(Card(ACE, CLUBS), Card(NINE, SPADES), led, HEARTS));
}
//passed
TEST(test_led_left_bower) {
  Card led(JACK, DIAMONDS);
  ASSERT_TRUE(Card_less(Card(NINE, DIAMONDS), Card(TEN, CLUBS), led, HEARTS));
}

//extra tests for Card Bug 3
TEST(test_card_equal) {
    Card a(ACE, HEARTS);
    Card same(ACE, HEARTS);
    Card different_suit(ACE, SPADES);
    Card different_rank(KING, HEARTS);
    Card different_both(QUEEN, CLUBS);

    ASSERT_TRUE(a == same);
    ASSERT_TRUE(a == a);
    ASSERT_FALSE(a == different_suit);
    ASSERT_FALSE(a == different_rank);
    ASSERT_FALSE(a == different_both);
}




TEST_MAIN()
