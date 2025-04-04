#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


typedef struct {        //structs to make card and hand data easier to use (lie)
    const char *face;
    int value;
} Card;

typedef struct {
    Card cards[10];
    int count;
} Hand;
int calculateHandValue(const Hand *hand);
void initializeDeck(Card deck[]) {          //making the deck arrays for the game to use
    const char *faces[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    int values[] = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};
    
    for (int i = 0; i < 52; i++) {
        deck[i].face = faces[i % 13];
        deck[i].value = values[i % 13];     //calling upon specific variable in the struct using . as a pointer
    }
}
void shuffleDeck(Card deck[]) {         // using rand in a for loop to randomise all the cards in the deck

    for (int i = 51; i > 0; i--) {
        int j = rand() % (i + 1);
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}
void dealCard(Hand *hand, Card deck[], int *currentCard) {      //function to deal cards to player and dealer with the card from the "top" of the deck and adds to the deck position by 1 so it doesnt always just draw the top card

    hand->cards[hand->count++] = deck[(*currentCard)++];

}

void displayHand(const Hand *hand, const char *label, bool hideCard) {         //hides the dealers first card while showing all the other cards dealed

    printf("%s's hand: ", label);

    for (int i = 0; i < hand->count; i++) {     // if the loop is on the first loop it hides the card

        if (i == 0 && hideCard) {

            printf("[??] ");

        } else {

            printf("[%s] ", hand->cards[i].face);

        }
    }

    if (!hideCard) {           //makes sure only the dealers face up cards are calculated 

        printf("= %d points\n", calculateHandValue(hand));

    } else {

        printf("\n");

    }
}

int calculateHandValue(const Hand *hand) {          //calculates the total value while making sure aces are used right pain in the ass

    int total = 0, aces = 0;
    
    for (int i = 0; i < hand->count; i++) {

        total += hand->cards[i].value;
        if (hand->cards[i].value == 11) aces++;

    }
    
    while (total > 21 && aces > 0) {

        total -= 10;
        aces--;

    }
    
    return total;
}

bool getYesNoInput(const char *prompt) {            //handles inputs using case switches and makes sure it still works with capitals
    char choice;
    while (1) {

        printf("%s", prompt);

        scanf(" %c", &choice);

        while (getchar() != '\n'); 
        
        if (choice == 'y' || choice == 'Y') return true;
        if (choice == 'n' || choice == 'N') return false;
        if (choice == 'h' || choice == 'H') return true;
        if (choice == 's' || choice == 'S') return false;
        printf("Invalid input\n");

    }
}
void clearScreen() {            //clears the screen everytime i feel like it this only works on windows apparently dont know how to fix that
    #ifdef _WIN32
    
        system("cls");
    
    #endif
    }

    void playGame() {

        Card deck[52];  //creates dick
    
        Hand player = {0}, dealer = {0};        //initalises hand for dealer and player
    
        int currentCard = 0;        //initialises card count
        
        initializeDeck(deck);       //initialses deck also shuffle
    
        shuffleDeck(deck);
        
        // Initial deal
        dealCard(&player, deck, &currentCard);
        dealCard(&dealer, deck, &currentCard);      //gives out card in proper casino style
        dealCard(&player, deck, &currentCard);
        dealCard(&dealer, deck, &currentCard);
        
        clearScreen();      //clear screen
    
        printf("\n~~~~ New Game ~~~\n");
    
        displayHand(&dealer, "Dealer", true);       //sets the bool for hidden card 
    
        displayHand(&player, "Player", false);
        
        
        while (calculateHandValue(&player) < 21 && getYesNoInput("\nHit? (H/S): ")) {       //asks if player wants to hit or stand
    
            dealCard(&player, deck, &currentCard);
    
            clearScreen();      //this is to update the screen with the new hands
    
            printf("\n~~~~~~New Game~~~~~~~\n");
    
            displayHand(&dealer, "Dealer", true);
            displayHand(&player, "Player", false);
    
        }
        
        int playerTotal = calculateHandValue(&player);
    
        if (playerTotal > 21) {
    
            printf("Bust You lose\n");         //checks if you bust lol
            return;
    
        }
        
        // Dealer's turn
        clearScreen();
    
        
    
        displayHand(&dealer, "Dealer", false);      //changes bool of hidden hand to false so it now displays
    
        while (calculateHandValue(&dealer) < 17) {          //dealer hits till 17
    
            printf("Dealer hits\n");
            dealCard(&dealer, deck, &currentCard);
            displayHand(&dealer, "Dealer", false);
    
        }
        
    
        // Determine winner
        int dealerTotal = calculateHandValue(&dealer);
        printf("\nFinal scores:\nPlayer: %d\nDealer: %d\n", playerTotal, dealerTotal);
        
    
        if (dealerTotal > 21) printf("You win\n");
        else if (playerTotal > dealerTotal) printf("You win\n");            //checks whos the winner
        else if (dealerTotal > playerTotal) printf("Dealer wins.\n");
        else printf("Tie.\n");
    }
    
    int main() {
        srand((unsigned int)time(NULL));        //seed generator for rand
        
        do {
            playGame();
        } while (getYesNoInput("\nWant to play again? (Y/N): "));       //play again input
        
        return 0;
    }