#include "Pack.hpp"
#include "unit_test_framework.hpp"
#include <sstream>

#include <iostream>

using namespace std;

TEST(test_pack_default_ctor) {
    Pack pack;
    Card first = pack.deal_one();
    ASSERT_EQUAL(NINE, first.get_rank());
    ASSERT_EQUAL(SPADES, first.get_suit());
}


TEST(test_pack_default_order) {
    Pack pack;

    for (int s = SPADES; s <= DIAMONDS; ++s) {
        for (int r = NINE; r <= ACE; ++r) {
            ASSERT_FALSE(pack.empty());

            Card c = pack.deal_one();
            ASSERT_EQUAL(c.get_rank(), static_cast<Rank>(r));
            ASSERT_EQUAL(c.get_suit(), static_cast<Suit>(s));
        }
    }

    ASSERT_TRUE(pack.empty());
}

TEST(test_pack_reset) {
    Pack pack;
    pack.deal_one();
    pack.deal_one();
    pack.deal_one();

    pack.reset();

    ASSERT_FALSE(pack.empty());
    ASSERT_EQUAL(pack.deal_one(), Card(NINE, SPADES));

    // Reset should also work after dealing the whole pack.
    for (int i = 1; i < 24; ++i) {
        pack.deal_one();
    }
    ASSERT_TRUE(pack.empty());

    pack.reset();
    ASSERT_FALSE(pack.empty());
    ASSERT_EQUAL(pack.deal_one(), Card(NINE, SPADES));
}

TEST(test_pack_input_constructor) {
    ostringstream text;

    // Use a different order so a constructor that ignores input fails.
    for (int s = DIAMONDS; s >= SPADES; --s) {
        for (int r = ACE; r >= NINE; --r) {
            text << Card(static_cast<Rank>(r), static_cast<Suit>(s))
                 << "\n";
        }
    }

    istringstream input(text.str());
    Pack pack(input);

    for (int s = DIAMONDS; s >= SPADES; --s) {
        for (int r = ACE; r >= NINE; --r) {
            ASSERT_FALSE(pack.empty());
            ASSERT_EQUAL(
                pack.deal_one(),
                Card(static_cast<Rank>(r), static_cast<Suit>(s)));
        }
    }

    ASSERT_TRUE(pack.empty());
}

TEST(test_pack_shuffle_order) {
    Pack pack;

    // Original card positions after seven in-shuffles.
    const int positions[] = {
        16, 8, 0, 17, 9, 1, 18, 10,
        2, 19, 11, 3, 20, 12, 4, 21,
        13, 5, 22, 14, 6, 23, 15, 7
    };

    pack.shuffle();

    for (int i = 0; i < 24; ++i) {
        int original = positions[i];
        Rank rank = static_cast<Rank>(NINE + original % 6);
        Suit suit = static_cast<Suit>(original / 6);

        ASSERT_FALSE(pack.empty());
        ASSERT_EQUAL(pack.deal_one(), Card(rank, suit));
    }

    ASSERT_TRUE(pack.empty());
}

TEST(test_pack_shuffle_resets_dealing) {
    Pack pack;

    // Exhaust the pack before shuffling.
    for (int i = 0; i < 24; ++i) {
        pack.deal_one();
    }
    ASSERT_TRUE(pack.empty());

    pack.shuffle();

    ASSERT_FALSE(pack.empty());
    ASSERT_EQUAL(pack.deal_one(), Card(KING, CLUBS));

    for (int i = 1; i < 24; ++i) {
        pack.deal_one();
    }
    ASSERT_TRUE(pack.empty());
}

TEST(test_pack_reset_keeps_shuffled_order) {
    Pack pack;
    pack.shuffle();

    Card first = pack.deal_one();
    Card second = pack.deal_one();
    pack.deal_one();

    pack.reset();

    ASSERT_EQUAL(pack.deal_one(), first);
    ASSERT_EQUAL(pack.deal_one(), second);
}

TEST_MAIN()
