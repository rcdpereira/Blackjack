#include <iostream>
#include <array>
#include <algorithm>
#include <ctime>
#include <random>

// Determine the ranks each card can have
enum class CardRank
// Create the cards ranks
{
	rank_2,
	rank_3,
	rank_4,
	rank_5,
	rank_6,
	rank_7,
	rank_8,
	rank_9,
	rank_10,
	rank_J,
	rank_Q,
	rank_K,
	rank_A,

	max_ranks
};

// Determine the suits each card can have
enum class CardSuit
// Create the cards suits
{
	suit_clubs,
	suit_diamonds,
	suit_spades,
	suit_hearts,

	max_suits
};

// Each card should have a rank and a suit
struct Card
{
	CardRank rank{};
	CardSuit suit{};
};

struct Player 
{
	int points{};
	int aces{};
};

// User-defined types that will appear several times
using deck_type = std::array<Card, 52>;
using index_type = deck_type::size_type;

// Print each card with its suit (e.g. 2 of Hearts, King of Clubs)
void printCard(const Card& card)
{
	switch (card.rank)
	{
	case CardRank::rank_2:	std::cout << '2';		break;
	case CardRank::rank_3:	std::cout << '3';		break;
	case CardRank::rank_4:	std::cout << '4';		break;
	case CardRank::rank_5:	std::cout << '5';		break;
	case CardRank::rank_6:	std::cout << '6';		break;
	case CardRank::rank_7:	std::cout << '7';		break;
	case CardRank::rank_8:	std::cout << '8';		break;
	case CardRank::rank_9:	std::cout << '9';		break;
	case CardRank::rank_10:	std::cout << "10";		break;
	case CardRank::rank_J:	std::cout << "Jack";	break;
	case CardRank::rank_Q:	std::cout << "Queen";	break;
	case CardRank::rank_K:	std::cout << "King";	break;
	case CardRank::rank_A:	std::cout << "Ace";		break;
	default:
		std::cout << '?';
		break;
	}
	switch (card.suit)
	{
	case CardSuit::suit_clubs:		std::cout << " of Clubs";		break;
	case CardSuit::suit_diamonds:	std::cout << " of Diamonds";	break;
	case CardSuit::suit_spades:		std::cout << " of Spades";		break;
	case CardSuit::suit_hearts:		std::cout << " of Hearts";		break;

	default:
		std::cout << '?';
		break;
	}
}

//Create a deck with the 52 cards
deck_type createDeck()
{
	deck_type deck{};

	index_type card{ 0 };
	auto suits{ static_cast<int>(CardSuit::max_suits) };
	auto ranks{ static_cast<int>(CardRank::max_ranks) };

	// Use nested for-loops to populate the deck array
	for (int suit{ 0 }; suit < suits; ++suit)
	{
		for (int rank{ 0 }; rank < ranks; ++rank)
		{
			deck[card].suit = static_cast<CardSuit>(suit);
			deck[card].rank = static_cast<CardRank>(rank);
			++card;
		}
	}

	return deck;
}

void printDeck(const deck_type& deck)
{
	for (const auto& card : deck)
	{
		printCard(card);
		std::cout << ' ';
	}
}

void shuffleDeck(deck_type& deck)
{
	// mt is static so it only gets seeded once.
	static std::mt19937_64 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

	std::shuffle(deck.begin(), deck.end(), mt);
}

int getCardValue(const Card& card)
{
	int cardValue{ 0 };
	switch (card.rank)
	{
	case CardRank::rank_2:	cardValue = 2;	break;
	case CardRank::rank_3:	cardValue = 3;	break;
	case CardRank::rank_4:	cardValue = 4;	break;
	case CardRank::rank_5:	cardValue = 5;	break;
	case CardRank::rank_6:	cardValue = 6;	break;
	case CardRank::rank_7:	cardValue = 7;	break;
	case CardRank::rank_8:	cardValue = 8;	break;
	case CardRank::rank_9:	cardValue = 9;	break;
	case CardRank::rank_10:	cardValue = 10;	break;
	case CardRank::rank_J:	cardValue = 10;	break;
	case CardRank::rank_Q:	cardValue = 10;	break;
	case CardRank::rank_K:	cardValue = 10;	break;
	case CardRank::rank_A:	cardValue = 11;	break;
	default:
		std::cout << '?';
		break;
	}
	return cardValue;
}

void trackAces(const deck_type& deck, int& cardOnDeck, Player& player)
{
	if (deck[cardOnDeck].rank == CardRank::rank_A)
		++player.aces;
}

int changeAceValue(Player& player, int& winPoints)
{
	const int aceDiff{ 10 }; // Result of 11 - 1: changing aces values can remove 10 points

	while (player.aces > 0)
	{
		player.points = player.points - aceDiff;
		--player.aces;
		if (player.points <= winPoints)
			return player.points;
	}
	return player.points;
}

int playBlackjack(deck_type& deck)
{
	// Shuffle deck before start playing
	shuffleDeck(deck);

	// Variables for game points rules
	int winPoints{ 21 };
	int minDealerPoints{ 17 };

	// Initialize the players
	Player dealer{};
	Player gambler{};

	// Dealer draws the first card on the deck for table
	int cardOnDeck{ 0 };
	std::cout << "Dealer got ";
	printCard(deck[cardOnDeck]);
	dealer.points = getCardValue(deck[cardOnDeck]);
	std::cout << " - Table Points: " << dealer.points << '\n';
	// Keep track of aces
	trackAces(deck, cardOnDeck, dealer);
	
	// Move to the next card on the deck
	++cardOnDeck;

	// Dealer gets first card for gambler
	std::cout << "You have ";
	printCard(deck[cardOnDeck]);
	// Keep track of aces
	trackAces(deck, cardOnDeck, gambler);

	// Gambler's first card points
	gambler.points = getCardValue(deck[cardOnDeck]);
	
	// Dealer gets second card for gambler
	++cardOnDeck;
	std::cout << " and ";
	printCard(deck[cardOnDeck]);
	// Keep track of aces
	trackAces(deck, cardOnDeck, gambler);

	// Gambler's total points
	gambler.points += getCardValue(deck[cardOnDeck]);
	// Check if gambler needs to change the ace values, if available
	// To cover when two aces are draw for gambler in the begining of the game
	if (gambler.points > winPoints && gambler.aces != 0)
	{
		gambler.points = changeAceValue(gambler, winPoints);
	}

	std::cout << " - Total Points: " << gambler.points << '\n';

	// Check if gambler wins
	if (gambler.points == winPoints)
	{
		std::cout << "*** WINNER!!! You have 21! ***\n";
		return 0;
	}

	// If gambler didn't win, continue playing	
	
	// Gambler will decide to hit or stand
	char hitOrStand{'h'};
	while (hitOrStand == 'h')
	{
		std::cout << "Hit or Stand? [h]/[s]: ";
		std::cin >> hitOrStand;

		if (hitOrStand == 'h')
		{
			++cardOnDeck;
			std::cout << "You got ";
			printCard(deck[cardOnDeck]);
			// Keep track of aces
			trackAces(deck, cardOnDeck, gambler);

			gambler.points += getCardValue(deck[cardOnDeck]);

			// Check if gambler needs to change the ace values, if available
			if (gambler.points > winPoints && gambler.aces != 0)
			{
				gambler.points = changeAceValue(gambler, winPoints);
			}

			std::cout << " - Total Points: " << gambler.points << '\n';
			
			if (gambler.points > winPoints)
			{
				std::cout << "* Table wins! You lose... *\n";
				return 0;
			}

			else if (gambler.points == winPoints)
			{
				std::cout << "*** WINNER!!! You have 21! ***\n";
				return 0;
			}
		}
	}
	/*
	do
	{
		std::cout << "Hit or Stand? [h]/[s]: ";
		std::cin >> hitOrStand;

		if (hitOrStand == 'h')
		{
			gambler.points += getCardValue(deck[cardOnDeck]);
			std::cout << "You have " << gambler.points << " points.\n";
			++cardOnDeck;

			if (gambler.points > winPoints)
			{
				std::cout << "Table wins! You lose...\n";
				return 0;
			} 

			if (gambler.points == 21)
			{
				std::cout << "You win!!!";
				return 0;
			}
		}
	} while (hitOrStand == 'h');
	*/
	//After player has finished, dealer continues to draw cards for table
	while (dealer.points < minDealerPoints)
	{
		++cardOnDeck;
		std::cout << "Dealer got ";
		printCard(deck[cardOnDeck]);
		dealer.points += getCardValue(deck[cardOnDeck]);
		// Keep track of aces
		trackAces(deck, cardOnDeck, dealer);
		
		// If dealer has more than 21 points, check if it has anys aces
		// If so, call function to change ace from 11 to 1
		if (dealer.points > winPoints && dealer.aces !=0)
		{
			dealer.points = changeAceValue(dealer, winPoints);
		}

		std::cout << " - Table Points: " << dealer.points << '\n';
	}

	// Check who the winner is
	if (dealer.points > winPoints)
	{
		std::cout << "*** You win!!! ***\n";
		return 0;
	}

	else if (gambler.points > dealer.points)
	{
		std::cout << "*** You win!!! ***\n";
		return 0;
	}
	else if (gambler.points == dealer.points)
	{
		std::cout << "It's a tie!\n";
		return 0;
	}
	else
	{
		std::cout << "* Table wins! You lose... *\n";
		return 0;
	}
}

int playAgain(deck_type deck)
{
	char playAgain{ 'y' };
	while (playAgain == 'y')
	{
		std::cout << "\nPlay again? [y]/[n]: ";
		std::cin >> playAgain;
		if (playAgain == 'n')
			return 0;
		std::cout << '\n';
		playBlackjack(deck);
	}
}

int main()
{
	auto deck{ createDeck() };

	playBlackjack(deck);

	playAgain(deck);
	
	std::cin.get();
	return 0;
}