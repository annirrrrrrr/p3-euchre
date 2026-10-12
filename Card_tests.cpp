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
TEST(test_get_suit_trump) {
	Card left(JACK, DIAMONDS);
	ASSERT_EQUAL(HEARTS, left.get_suit(HEARTS));
	ASSERT_EQUAL(DIAMONDS, left.get_suit());
	ASSERT_EQUAL(SPADES, Card(JACK, SPADES).get_suit(HEARTS));
}

TEST(test_is_face_or_ace) {
	ASSERT_FALSE(Card(TEN, HEARTS).is_face_or_ace());
	ASSERT_TRUE(Card(JACK, HEARTS).is_face_or_ace());
	ASSERT_TRUE(Card(ACE, HEARTS).is_face_or_ace());
}

TEST(test_is_right_bower) {
	ASSERT_TRUE(Card(JACK, HEARTS).is_right_bower(HEARTS));
	ASSERT_FALSE(Card(JACK, DIAMONDS).is_right_bower(HEARTS));
}

/////

TEST(test_is_left_bower) {
	ASSERT_TRUE(Card(JACK, CLUBS).is_left_bower(SPADES));
	ASSERT_FALSE(Card(JACK, SPADES).is_left_bower(SPADES));
	ASSERT_FALSE(Card(JACK, HEARTS).is_left_bower(SPADES));
}

TEST(test_is_trump) {
	ASSERT_TRUE(Card(JACK, DIAMONDS).is_trump(HEARTS));
	ASSERT_FALSE(Card(ACE, DIAMONDS).is_trump(HEARTS));
}

TEST(test_suit_next) {
	ASSERT_EQUAL(CLUBS, Suit_next(SPADES));
	ASSERT_EQUAL(SPADES, Suit_next(CLUBS));
	ASSERT_EQUAL(DIAMONDS, Suit_next(HEARTS));
	ASSERT_EQUAL(HEARTS, Suit_next(DIAMONDS));
}

TEST(test_stream_insert) {
	ostringstream oss;
	oss << Card(TEN, CLUBS);
	ASSERT_EQUAL(oss.str(), "Ten of Clubs");
}

TEST(test_stream_extract) {
	istringstream iss("Ace of Diamonds Nine of Spades");
	Card a;
	Card b;
	iss >> a >> b;
	ASSERT_EQUAL(Card(ACE, DIAMONDS), a);
	ASSERT_EQUAL(Card(NINE, SPADES), b);
}

TEST(test_less_greater) {
	ASSERT_TRUE(Card(NINE, DIAMONDS) < Card(TEN, SPADES));
	ASSERT_TRUE(Card(TEN, DIAMONDS) > Card(TEN, SPADES));
	ASSERT_FALSE(Card(TEN, HEARTS) < Card(TEN, HEARTS));
}

TEST(test_less_equal_greater_equal) {
	Card c(TEN, HEARTS);
	ASSERT_TRUE(c <= c);
	ASSERT_TRUE(c >= c);
	ASSERT_FALSE(Card(TEN, CLUBS) <= c);
}

TEST(test_equal_not_equal) {
	ASSERT_TRUE(Card(TEN, HEARTS) == Card(TEN, HEARTS));
	ASSERT_TRUE(Card(TEN, HEARTS) != Card(TEN, DIAMONDS));
}

TEST(test_card_less_trump) {
	ASSERT_TRUE(Card_less(Card(JACK, DIAMONDS), Card(JACK, HEARTS), HEARTS));
	ASSERT_TRUE(Card_less(Card(ACE, HEARTS), Card(JACK, DIAMONDS), HEARTS));
	ASSERT_TRUE(Card_less(Card(ACE, SPADES), Card(NINE, HEARTS), HEARTS));
	ASSERT_FALSE(Card_less(Card(JACK, HEARTS), Card(JACK, HEARTS), HEARTS));
}

TEST(test_card_less_led) {
	Card led_spade(KING, SPADES);
	ASSERT_TRUE(Card_less(Card(ACE, CLUBS), Card(NINE, SPADES), led_spade, HEARTS));

	Card led_left(JACK, DIAMONDS);
	ASSERT_TRUE(Card_less(Card(NINE, DIAMONDS), Card(TEN, CLUBS), led_left, HEARTS));
}

TEST_MAIN()
