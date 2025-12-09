#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>
#include <cctype>

using namespace std;

enum class Suit { Clubs, Diamonds, Hearts, Spades };

enum class Rank {
    Two = 2, Three, Four, Five, Six, Seven, Eight, Nine, Ten,
    Jack = 10, Queen = 10, King = 10, Ace = 11
};

struct Card {
    Suit suit;
    Rank rank;
};

string suitToString(Suit s) {
    switch (s) {
        case Suit::Clubs:    return "C";
        case Suit::Diamonds: return "D";
        case Suit::Hearts:   return "H";
        case Suit::Spades:   return "S";
    }
    return "?";
}

string rankToString(Rank r) {
    int v = static_cast<int>(r);
    if (v >= 2 && v <= 10) return to_string(v);
    if (r == Rank::Jack)  return "J";
    if (r == Rank::Queen) return "Q";
    if (r == Rank::King)  return "K";
    if (r == Rank::Ace)   return "A";
    return "?";
}

string cardToString(const Card& c) {
    return rankToString(c.rank) + suitToString(c.suit);
}

// ----------------------- Deck ----------------------------
class Deck {
private:
    vector<Card> cards;
    mt19937 rng;

public:
    Deck() {
        rng.seed(static_cast<unsigned int>(time(nullptr)));
        initialize();
    }

    void initialize() {
        cards.clear();
        for (int s = 0; s < 4; ++s) {
            for (int r = 2; r <= 10; ++r) {
                Card c;
                c.suit = static_cast<Suit>(s);
                c.rank = static_cast<Rank>(r);
                cards.push_back(c);
            }
            Card j{ static_cast<Suit>(s), Rank::Jack };
            Card q{ static_cast<Suit>(s), Rank::Queen };
            Card k{ static_cast<Suit>(s), Rank::King };
            Card a{ static_cast<Suit>(s), Rank::Ace };
            cards.push_back(j);
            cards.push_back(q);
            cards.push_back(k);
            cards.push_back(a);
        }
    }

    void shuffle() {
        std::shuffle(cards.begin(), cards.end(), rng);
    }

    Card deal() {
        if (cards.empty()) {
            initialize();
            shuffle();
        }
        Card c = cards.back();
        cards.pop_back();
        return c;
    }
};

// ----------------------- Hand ----------------------------
class Hand {
private:
    vector<Card> cards;

public:
    void clear() { cards.clear(); }

    void addCard(const Card& c) { cards.push_back(c); }

    size_t size() const { return cards.size(); }

    const vector<Card>& getCards() const { return cards; }

    Card removeCard(size_t index) {
        Card c = cards.at(index);
        cards.erase(cards.begin() + static_cast<long>(index));
        return c;
    }

    int getValue() const {
        int total = 0;
        int aceCount = 0;
        for (const Card& c : cards) {
            if (c.rank == Rank::Ace) {
                total += 11;
                aceCount++;
            } else {
                total += static_cast<int>(c.rank);
            }
        }
        // Make some aces count as 1 if needed
        while (total > 21 && aceCount > 0) {
            total -= 10;
            aceCount--;
        }
        return total;
    }

    bool containsAce() const {
        for (const Card& c : cards) {
            if (c.rank == Rank::Ace) return true;
        }
        return false;
    }

    bool isSoft() const {
        int total = 0;
        int aceCount = 0;
        for (const Card& c : cards) {
            if (c.rank == Rank::Ace) {
                total += 11;
                aceCount++;
            } else {
                total += static_cast<int>(c.rank);
            }
        }
        return (aceCount > 0 && total <= 21);
    }

    bool isBust() const { return getValue() > 21; }

    bool isBlackjack() const { return cards.size() == 2 && getValue() == 21; }

    void show(bool hideFirstCard = false) const {
        for (size_t i = 0; i < cards.size(); ++i) {
            if (i == 0 && hideFirstCard) {
                cout << "?? ";
            } else {
                cout << cardToString(cards[i]) << " ";
            }
        }
        if (!hideFirstCard) {
            cout << "(" << getValue() << ")";
        }
        cout << endl;
    }
};

// -------------------- UI helpers ------------------------
void showWelcome() {
    cout << "=====================================\n";
    cout << "           CASINO BLACKJACK          \n";
    cout << "=====================================\n";
    cout << "Rules (program version):\n";
    cout << " - Single player vs. dealer.\n";
    cout << " - Dealer hits on 16 or less, stands on 17+.\n";
    cout << " - Doubling down and splitting pairs allowed.\n";
    cout << " - Blackjack (Ace + 10-value) pays 3:2.\n";
    cout << "=====================================\n\n";
}

double getBet(double bankroll) {
    double bet;
    while (true) {
        cout << "You have $" << bankroll << ". Enter your bet: ";
        cin >> bet;
        if (!cin) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }
        if (bet <= 0) {
            cout << "Bet must be positive.\n";
        } else if (bet > bankroll) {
            cout << "You cannot bet more than your bankroll.\n";
        } else {
            break;
        }
    }
    return bet;
}

// ---------------- Basic strategy engine -----------------
enum class BasicAction { Hit, Stand, DoubleDown, Split };

string actionToString(BasicAction a) {
    switch (a) {
        case BasicAction::Hit:        return "HIT";
        case BasicAction::Stand:      return "STAND";
        case BasicAction::DoubleDown: return "DOUBLE DOWN";
        case BasicAction::Split:      return "SPLIT";
    }
    return "";
}

int cardValueForStrategy(const Card& c) {
    if (c.rank == Rank::Ace) return 11;
    return static_cast<int>(c.rank);
}

bool isPair(const Hand& h) {
    if (h.size() != 2) return false;
    const auto& cards = h.getCards();
    return cardValueForStrategy(cards[0]) == cardValueForStrategy(cards[1]);
}

int dealerUpValue(const Hand& dealer) {
    const auto& cards = dealer.getCards();
    if (cards.empty()) return 0;
    // Second card is face up in our display
    int v = cardValueForStrategy(cards[1]);
    if (v > 10) v = 10; // treat J/Q/K as 10
    return v;
}

// Implements the rules from your handout (soft hand, doubling, splitting)
BasicAction basicStrategySuggestion(const Hand& hand,
                                    int dealerUp,
                                    bool canDouble,
                                    bool canSplit) {
    int total = hand.getValue();
    bool soft = hand.isSoft();

    // --- Splitting rules ---
    if (canSplit && isPair(hand)) {
        const auto& cards = hand.getCards();
        int pairVal = cardValueForStrategy(cards[0]);

        // Never split 10s, 5s or 4s
        if (pairVal == 10 || pairVal == 5 || pairVal == 4) {
            // fall through to normal strategy
        } else if (pairVal == 8 || pairVal == 11) {
            // always split 8s and Aces
            return BasicAction::Split;
        } else {
            // other pairs: split vs dealer 2–7
            if (dealerUp >= 2 && dealerUp <= 7) {
                return BasicAction::Split;
            }
        }
    }

    // --- Soft hands ---
    if (soft) {
        if (total <= 17) {
            // always hit soft 17 or less
            return BasicAction::Hit;
        } else if (total == 18) {
            // hit soft 18 vs dealer 9 or 10 (incl. J/Q/K) or Ace
            if (dealerUp == 9 || dealerUp == 10 || dealerUp == 11) {
                return BasicAction::Hit;
            } else {
                return BasicAction::Stand;
            }
        } else {
            // soft 19+ stand
            return BasicAction::Stand;
        }
    }

    // --- Hard hands + doubling rules ---
    if (canDouble) {
        if (total == 11) {
            // always double 11
            return BasicAction::DoubleDown;
        }
        if (total == 10) {
            // double 10 unless dealer 10 or Ace
            if (dealerUp != 10 && dealerUp != 11) {
                return BasicAction::DoubleDown;
            }
        }
        if (total == 9) {
            // double 9 vs dealer 2–6
            if (dealerUp >= 2 && dealerUp <= 6) {
                return BasicAction::DoubleDown;
            }
        }
    }

    // --- Hard totals: hit/stand ---
    if (total <= 8) return BasicAction::Hit;

    if (total == 9) {
        return BasicAction::Hit;
    }

    if (total >= 10 && total <= 11) {
        return BasicAction::Hit;
    }

    if (total >= 12 && total <= 16) {
        // stand vs 2–6, otherwise hit
        if (dealerUp >= 2 && dealerUp <= 6) return BasicAction::Stand;
        return BasicAction::Hit;
    }

    // 17 or more – stand
    return BasicAction::Stand;
}

// Ask the player for action, checking which moves are legal
char getPlayerChoice(bool canHit, bool canStand,
                     bool canDouble, bool canSplit) {
    while (true) {
        cout << "Choose action: ";
        cout << "(H)it";
        if (canStand)  cout << ", (S)tand";
        if (canDouble) cout << ", (D)ouble";
        if (canSplit)  cout << ", S(P)lit";
        cout << ": ";

        char ch;
        cin >> ch;
        ch = static_cast<char>(toupper(static_cast<unsigned char>(ch)));

        if (ch == 'H' && canHit)    return ch;
        if (ch == 'S' && canStand)  return ch;
        if (ch == 'D' && canDouble) return ch;
        if (ch == 'P' && canSplit)  return ch;

        cout << "Invalid choice, try again.\n";
    }
}

// ------------------------- main -------------------------
int main() {
    Deck deck;
    deck.shuffle();

    double bankroll = 100.0;
    char playAgain = 'Y';
    showWelcome();

    while (playAgain == 'Y' && bankroll > 0.0) {
        cout << "\n===== NEW ROUND =====\n";
        double baseBet = getBet(bankroll);

        // subtract initial bet from bankroll
        bankroll -= baseBet;

        Hand dealer;
        vector<Hand>  playerHands(1);
        vector<double> bets(1, baseBet);
        vector<bool>   handFinished(1, false);
        vector<bool>   fromSplitAces(1, false);

        // Initial deal: player and dealer
        playerHands[0].addCard(deck.deal());
        dealer.addCard(deck.deal());
        playerHands[0].addCard(deck.deal());
        dealer.addCard(deck.deal());

        cout << "Dealer's hand: ";
        dealer.show(true);
        cout << "Your hand: ";
        playerHands[0].show(false);

        int dUpVal = dealerUpValue(dealer);
        BasicAction sugg =
            basicStrategySuggestion(playerHands[0], dUpVal,
                /*canDouble*/ bankroll >= bets[0] && playerHands[0].size() == 2,
                /*canSplit*/  bankroll >= bets[0] && playerHands.size() == 1
                               && isPair(playerHands[0]));
        cout << "[Basic Strategy Suggestion] "
             << actionToString(sugg) << "\n";

        bool playerBlackjack      = playerHands[0].isBlackjack();
        bool dealerBlackjackFlag  = dealer.isBlackjack();

        // ----------- Natural blackjack check --------------
        if (playerBlackjack || dealerBlackjackFlag) {
            cout << "\n--- Checking for Blackjack ---\n";
            cout << "Dealer's full hand: ";
            dealer.show(false);

            if (playerBlackjack && dealerBlackjackFlag) {
                cout << "Both you and the dealer have Blackjack. Push.\n";
                // return original bet
                bankroll += baseBet;
            } else if (playerBlackjack) {
                cout << "You have Blackjack! You win 1.5x your bet.\n";
                bankroll += baseBet * 2.5;  // stake + 1.5× winnings
            } else {
                cout << "Dealer has Blackjack. You lose.\n";
                // bet already removed
            }
        } else {
            // ------------- Player actions (supports split) -------------
            for (size_t h = 0; h < playerHands.size(); ++h) {
                while (!handFinished[h] && !playerHands[h].isBust()) {
                    cout << "\n--- Playing Hand " << (h + 1) << " ---\n";
                    cout << "Dealer shows: ";
                    dealer.show(true);
                    cout << "Your hand: ";
                    playerHands[h].show(false);

                    bool canHit    = true;
                    bool canStand  = true;
                    bool canDouble = (playerHands[h].size() == 2 &&
                                      bankroll >= bets[h] &&
                                      !fromSplitAces[h]);
                    // Only allow one split total (playerHands.size()==1)
                    bool canSplit  = (playerHands.size() == 1 &&
                                      playerHands[h].size() == 2 &&
                                      isPair(playerHands[h]) &&
                                      bankroll >= bets[h]);

                    int dUp = dealerUpValue(dealer);
                    BasicAction suggestion =
                        basicStrategySuggestion(playerHands[h], dUp,
                                                canDouble, canSplit);
                    cout << "[Basic Strategy Suggestion] "
                         << actionToString(suggestion) << "\n";

                    char choice =
                        getPlayerChoice(canHit, canStand,
                                        canDouble, canSplit);

                    if (choice == 'H') {
                        Card c = deck.deal();
                        cout << "You draw: " << cardToString(c) << "\n";
                        playerHands[h].addCard(c);
                        cout << "Your hand: ";
                        playerHands[h].show(false);
                        if (playerHands[h].isBust()) {
                            cout << "You bust on this hand.\n";
                            handFinished[h] = true;
                        }
                    } else if (choice == 'S') {
                        handFinished[h] = true;
                    } else if (choice == 'D' && canDouble) {
                        // Double bet, one more card only
                        bankroll -= bets[h];
                        bets[h] *= 2;
                        Card c = deck.deal();
                        cout << "You double down and draw: "
                             << cardToString(c) << "\n";
                        playerHands[h].addCard(c);
                        cout << "Your hand: ";
                        playerHands[h].show(false);
                        if (playerHands[h].isBust()) {
                            cout << "You bust on this hand.\n";
                        }
                        handFinished[h] = true;
                    } else if (choice == 'P' && canSplit) {
                        cout << "You choose to split the pair.\n";

                        // Pay additional bet for new hand
                        bankroll -= bets[h];

                        // Create new hand from second card
                        Hand newHand;
                        Card moved = playerHands[h].removeCard(1);
                        newHand.addCard(moved);

                        // Deal one extra card to each split hand
                        playerHands[h].addCard(deck.deal());
                        newHand.addCard(deck.deal());

                        double newBet = bets[h];
                        bool splitAces = (cardValueForStrategy(moved) == 11);

                        // Insert new hand right after current
                        playerHands.insert(playerHands.begin()
                                           + static_cast<long>(h + 1), newHand);
                        bets.insert(bets.begin()
                                    + static_cast<long>(h + 1), newBet);
                        handFinished.insert(handFinished.begin()
                                    + static_cast<long>(h + 1), false);
                        fromSplitAces.insert(fromSplitAces.begin()
                                    + static_cast<long>(h + 1), splitAces);

                        fromSplitAces[h] = splitAces;

                        cout << "Hand 1 after split: ";
                        playerHands[h].show(false);
                        cout << "Hand 2 after split: ";
                        playerHands[h + 1].show(false);

                        if (splitAces) {
                            // Split aces: only one card per hand, no hits
                            handFinished[h]     = true;
                            handFinished[h + 1] = true;
                            cout << "Split aces: each hand gets "
                                    "one card only, no further hits.\n";
                        }
                    }
                }
            }

            // ------------- Dealer plays (only if some hand alive) -------------
            bool allBust = true;
            for (const auto& h : playerHands) {
                if (!h.isBust()) { allBust = false; break; }
            }

            if (allBust) {
                cout << "\nAll your hands busted. Dealer wins automatically.\n";
            } else {
                cout << "\nDealer's turn...\n";
                cout << "Dealer's hand: ";
                dealer.show(false);

                while (dealer.getValue() < 17) {
                    Card c = deck.deal();
                    cout << "Dealer draws: " << cardToString(c) << "\n";
                    dealer.addCard(c);
                    cout << "Dealer's hand: ";
                    dealer.show(false);
                }

                int dealerTotal = dealer.getValue();
                bool dealerBust = dealer.isBust();

                // --------- Resolve each hand separately -------------
                for (size_t h = 0; h < playerHands.size(); ++h) {
                    cout << "\n--- Result for Hand " << (h + 1) << " ---\n";
                    cout << "Your hand: ";
                    playerHands[h].show(false);
                    cout << "Dealer: ";
                    dealer.show(false);

                    if (playerHands[h].isBust()) {
                        cout << "You busted. You lose this bet of $"
                             << bets[h] << ".\n";
                    } else if (dealerBust) {
                        cout << "Dealer busts. You win!\n";
                        bankroll += bets[h] * 2; // stake + win
                    } else {
                        int pTotal = playerHands[h].getValue();
                        if (pTotal > dealerTotal) {
                            cout << "You beat the dealer!\n";
                            bankroll += bets[h] * 2;
                        } else if (pTotal < dealerTotal) {
                            cout << "Dealer wins this hand.\n";
                        } else {
                            cout << "Push on this hand. "
                                    "Your bet is returned.\n";
                            bankroll += bets[h];
                        }
                    }
                }
            }
        }

        cout << "Your bankroll: $" << bankroll << "\n";
        if (bankroll <= 0.0) {
            cout << "You are out of money. Game over.\n";
            break;
        }

        cout << "Play another round? (Y/N): ";
        cin >> playAgain;
        playAgain =
            static_cast<char>(toupper(static_cast<unsigned char>(playAgain)));
    }

    cout << "Thanks for playing Casino Blackjack!\n";
    return 0;
}
