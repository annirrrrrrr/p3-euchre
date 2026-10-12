#include "Pack.hpp"
#include <array>
#include <string>
using namespace std;

Pack::Pack() {
	next = 0;
	int index = 0;
	for (int s = SPADES; s <= DIAMONDS; ++s) {
		for (int r = NINE; r <= ACE; ++r) {
			cards[index] = Card(static_cast<Rank>(r), static_cast<Suit>(s));
			++index;
		}
	}
}

Pack::Pack(std::istream &packin) {
	next = 0;
	for (int i = 0; i < PACK_SIZE; ++i) {
		packin >> cards[i];
	}
}

Card Pack::deal_one() {
	Card c = cards[next];
	next++;
	return c;
}

void Pack::reset() {
	next = 0;
}

void Pack::shuffle() {
	int half = PACK_SIZE / 2;
	for (int k = 0; k < 7; k++) {
		std::array<Card, PACK_SIZE> temp = cards;
		for (int i = 0; i < half; i++) {
			cards[2 * i] = temp[half + i];
			cards[2 * i + 1] = temp[i];
		}
	}
	next = 0;
}

bool Pack::empty() const {
	return next >= PACK_SIZE;
}