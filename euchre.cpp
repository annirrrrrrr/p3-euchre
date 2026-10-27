// Complete euchre.cpp game driver. Build with Card.cpp, Pack.cpp, Player.cpp.
#include "Card.hpp"
#include "Pack.hpp"
#include "Player.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Validate before indexing argv or creating any players.
// On success, points receives an integer in [1, 100].
static bool valid_arguments(int argc, char **argv, int &points) {
  if (argc != 12) {
    return false;
  }
  istringstream input(argv[3]);
  char extra;
  if (!(input >> points) || (input >> extra) || points < 1 || points > 100) {
    return false;
  }
  string shuffle_option = argv[2];
  if (shuffle_option != "shuffle" && shuffle_option != "noshuffle") {
    return false;
  }
  for (int i = 5; i <= 11; i += 2) {
    string strategy = argv[i];
    if (strategy != "Simple" && strategy != "Human") {
      return false;
    }
  }
  return true;
}

static void print_usage() {
  cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
       << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
       << "NAME4 TYPE4" << endl;
}

class Game {
public:
  // argv has already been validated; pack_input is an open, valid pack file.
  Game(istream &pack_input, bool shuffle_in, int points_in, char **argv)
    : pack(pack_input), shuffle_enabled(shuffle_in), points_to_win(points_in),
      dealer(0), hand_number(0), scores{0, 0} {
    for (int i = 0; i < 4; ++i) {
      players.push_back(Player_factory(argv[4 + 2 * i], argv[5 + 2 * i]));
    }
  }

  // Game owns the four pointers returned by Player_factory.
  ~Game() {
    for (Player *player : players) {
      delete player;
    }
  }

  // Copying an owning Game would cause its players to be deleted twice.
  Game(const Game &) = delete;
  Game &operator=(const Game &) = delete;

  void play() {
    while (scores[0] < points_to_win && scores[1] < points_to_win) {
      cout << "Hand " << hand_number << '\n';
      cout << *players[dealer] << " deals\n";
      prepare_pack();
      Card upcard = deal();
      cout << upcard << " turned up\n";
      Suit trump = SPADES; // make_trump replaces this before any trick.
      int maker = make_trump(upcard, trump);
      int team_zero_tricks = play_hand(trump);
      score_hand(maker, team_zero_tricks);
      dealer = (dealer + 1) % 4;
      ++hand_number;
    }
    int winner = scores[0] >= points_to_win ? 0 : 1;
    print_team(winner);
    cout << " win!\n";
  }

private:
  vector<Player *> players; // Seats 0..3; partners have equal index % 2.
  Pack pack;
  bool shuffle_enabled;
  int points_to_win;
  int dealer;
  int hand_number;
  int scores[2]; // Team 0: seats 0 and 2. Team 1: seats 1 and 3.

  void prepare_pack() {
    if (shuffle_enabled) {
      pack.shuffle(); // Pack already performs seven in-shuffles and resets.
    }
    else {
      pack.reset();
    }
  }

  // Deal eight batches starting left of dealer, then return the upcard.
  Card deal() {
    for (int batch = 0; batch < 8; ++batch) {
      int seat = (dealer + 1 + batch) % 4;
      int count = batch % 2 == 0 ? 3 : 2;
      if (batch >= 4) {
        count = 5 - count;
      }
      for (int card = 0; card < count; ++card) {
        players[seat]->add_card(pack.deal_one());
      }
    }
    return pack.deal_one();
  }

  // Return the maker's seat and set trump through its reference parameter.
  int make_trump(const Card &upcard, Suit &trump) {
    for (int round = 1; round <= 2; ++round) {
      for (int offset = 1; offset <= 4; ++offset) {
        int seat = (dealer + offset) % 4;
        if (players[seat]->make_trump(upcard, seat == dealer, round, trump)) {
          cout << *players[seat] << " orders up " << trump << '\n';
          if (round == 1) {
            players[dealer]->add_and_discard(upcard);
          }
          cout << '\n';
          return seat;
        }
        cout << *players[seat] << " passes\n";
      }
    }
    assert(false); // Legal players must order up by round-two dealer's turn.
    return -1;
  }

  // Return the winning seat. Keep led_card fixed for all three comparisons.
  int play_trick(int leader, Suit trump) {
    Card led_card = players[leader]->lead_card(trump);
    cout << led_card << " led by " << *players[leader] << '\n';
    Card best_card = led_card;
    int winner = leader;
    for (int offset = 1; offset < 4; ++offset) {
      int seat = (leader + offset) % 4;
      Card played = players[seat]->play_card(led_card, trump);
      cout << played << " played by " << *players[seat] << '\n';
      if (Card_less(best_card, played, led_card, trump)) {
        best_card = played;
        winner = seat;
      }
    }
    cout << *players[winner] << " takes the trick\n\n";
    return winner;
  }

  // Five tricks consume all five cards in every hand; no hand clearing needed.
  int play_hand(Suit trump) {
    int leader = (dealer + 1) % 4;
    int team_zero_tricks = 0;
    for (int trick = 0; trick < 5; ++trick) {
      leader = play_trick(leader, trump);
      if (leader % 2 == 0) {
        ++team_zero_tricks;
      }
    }
    return team_zero_tricks;
  }

  // Print partners in command-line order (lower seat first).
  void print_team(int team) const {
    cout << *players[team] << " and " << *players[team + 2];
  }

  void score_hand(int maker, int team_zero_tricks) {
    int winner = team_zero_tricks >= 3 ? 0 : 1;
    int winning_tricks = winner == 0 ? team_zero_tricks : 5 - team_zero_tricks;
    print_team(winner);
    cout << " win the hand\n";
    if (winner != maker % 2) {
      scores[winner] += 2;
      cout << "euchred!\n";
    }
    else if (winning_tricks == 5) {
      scores[winner] += 2;
      cout << "march!\n";
    }
    else {
      ++scores[winner];
    }
    for (int team = 0; team < 2; ++team) {
      print_team(team);
      cout << " have " << scores[team] << " points\n";
    }
    cout << '\n';
  }
};

int main(int argc, char **argv) {
  int points = 0;
  if (!valid_arguments(argc, argv, points)) {
    print_usage();
    return 1;
  }
  ifstream pack_input(argv[1]);
  if (!pack_input.is_open()) {
    cout << "Error opening " << argv[1] << '\n';
    return 1;
  }
  for (int i = 0; i < argc; ++i) {
    cout << argv[i] << ' ';
  }
  cout << '\n';
  Game game(pack_input, string(argv[2]) == "shuffle", points, argv);
  game.play();
  return 0; // The local Game is destroyed here, deleting all four players.
}
