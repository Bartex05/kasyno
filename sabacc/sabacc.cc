#include <iostream>
#include <string>
#include <ctime>

using namespace std;

// ZMIENNE GLOBALNE

int pHand[15]; 
int dHand[15];

int pSize = 0; 
int dSize = 0;

int pCredits = 150; 
int dCredits = 150; 

int gamePot = 0; 
int sabaccPot = 0; 
int currentBet = 0;

// FUNKCJE POMOCNICZE

int calculateScore(int hand[], int size) {
    
    int total = 0;
    
    for (int i = 0; i < size; i++) total += hand[i];
    
    return total;
}

int myAbs(int n) {
    
    return (n < 0) ? -n : n;
}

int drawCard() {
    
    int r = rand() % 62;
    
    // Sylopy (zera)
    if (r < 2) return 0; 

    // Zielone 1-10
    if (r < 32) return (rand() % 10) + 1; 
    
    // Czerwone 1-10
    return -((rand() % 10) + 1); 
}

string getCardName(int v) {
    
    if (v == 0) return "Sylop (0)";
    if (v > 0) return "Green +" + to_string(v);
    
    return "Red " + to_string(v);
}

void printCard(int hand[], int size, bool hide) {
    
    string lines[5] = {"", "", "", "", ""};

    for (int i = 0; i < size; i++) {
        
        int valueInt = hand[i];
        
        string symbol;
        string valueString;

        // Symbol
        if (valueInt == 0) {
            
            symbol = "S";
        }
        else if (valueInt > 0) { 
            
            symbol = "G";
        }
        else {
            
            symbol = "R";
        }

        if (hide) {
            
            valueString = "?";
            symbol = "?";
        } 
        else {
            
            valueString = to_string(myAbs(valueInt));
        }

        // Padding
        string valueLeft = valueString;
        string valueRight = valueString;
        
        if (valueString.length() == 1) {
            
            valueLeft = valueString + " "; // Np. "5 "
            valueRight = " " + valueString; // Np. " 5"
        }

        lines[0] += "┌─────┐ ";
        lines[1] += "│" + valueLeft + "   │ ";
        lines[2] += "│  " + symbol + "  │ ";
        lines[3] += "│   " + valueRight + "│ ";
        lines[4] += "└─────┘ ";
    }

    for (int i = 0; i < 5; i++) {
        
        cout << lines[i] << endl;
    }
}

// LOGIKA DEALERA

void dealerAction() {
    
    if (dSize == 0) return;

    int currentScore = calculateScore(dHand, dSize);
    int currentAbs = myAbs(currentScore);

    // 1. Logika wymiany (Koszt 10)
    
    if (dCredits >= 10 && currentAbs > 5) {
        
        int bestIndex = -1;
        int bestAbs = currentAbs;

        for (int i = 0; i < dSize; i++) {
            
            int scoreWithoutCard = currentScore - dHand[i];
            
            if (myAbs(scoreWithoutCard) < bestAbs) {
                
                bestAbs = myAbs(scoreWithoutCard);
                bestIndex = i;
            }
        }

        if (bestIndex != -1) {
            
            dHand[bestIndex] = drawCard();
            
            dCredits -= 10;
            gamePot += 10;
            
            cout << ">> Dealer paid $10 and changed a card." << endl;
            return; 
        }
    }

    // 2. Logika dobierania (Koszt 5)
    
    if (dCredits >= 5 && currentAbs > 7) {
        
        dHand[dSize++] = drawCard();
        
        dCredits -= 5;
        gamePot += 5;
        
        cout << ">> Dealer paid $10 and draw a card." << endl;
    }
}

int dealerDecideBet() {
    
    int score = myAbs(calculateScore(dHand, dSize));
    
    //Fold
    if (currentBet > 20 && score > 8) return 0;
    
    // Raise
    if (score <= 3 && (rand() % 100 < 30)) return 2; 
    
    //Call
    return 1;
}

// FAZY GRY

void showStatus(bool finalReveal) {
    
    cout << endl << "-----------------------------------------------------------" << endl;
    cout << " MONEY: $" << pCredits << " | TABLE POT: $" << gamePot << " | SABACC POT: $" << sabaccPot << endl;
    cout << "-----------------------------------------------------------" << endl;
    
    cout << "YOUR HAND:" << endl;
    
    printCard(pHand, pSize, false); 

    if (finalReveal) {

        cout << "Score: " << calculateScore(pHand, pSize) << endl;
    }
    
    cout << "-----------------------------------------------------------" << endl;
    
    cout << "DEALER'S HAND:" << endl;
    
    printCard(dHand, dSize, !finalReveal);

    if (finalReveal) {

        cout << "Score: " << calculateScore(dHand, dSize) << endl;
    }
    
    cout << "-----------------------------------------------------------" << endl;
}

bool handleBetting() {
    
    cout << endl << "--- LICITATION ---" << endl;
    cout << "Current rate: $" << currentBet << endl;
    cout << "1. Call" << endl << "2. Raise" << endl << "3. Fold" << endl << "?> Choice: ";
    
    int choice; cin >> choice;

    if (choice == 3) {
        
        cout << ">> You folded. Dealer wins the money." << endl;
        
        dCredits += gamePot; return false; 
    }
    if (choice == 2) {
        
        int raise; cout << "How much you raise?: "; cin >> raise;
        
        currentBet += raise;
    }

    int dAction = dealerDecideBet();
    
    if (dAction == 0) {
        
        cout << ">> Dealer folded! You won $ " << gamePot << "1" << endl;
        
        pCredits += gamePot; return false;
    } 
    else if (dAction == 2) {
        
        int dRaise = (rand() % 10) + 1;

        currentBet += dRaise;
        
        cout << ">> Dealer raise for $" << dRaise << "!" << endl;
    } 
    else {
        
        cout << ">> Dealer calls." << endl;
    }

    pCredits -= currentBet;
    dCredits -= currentBet;
   
    gamePot += (currentBet * 2);
   
    return true;
}

void handleSpike() {
    
    int d1 = rand() % 6 + 1, d2 = rand() % 6 + 1;
    
    if (d1 == d2) {
        
        cout << ">> Dice roll: [" << d1 << "] [" << d2 << "] !!! SPIKE !!!" << endl;
        
        int oldP = pSize, oldD = dSize;
        
        pSize = 0; dSize = 0;
        
        for (int i = 0; i < oldP; i++) pHand[pSize++] = drawCard();
        for (int i = 0; i < oldD; i++) dHand[dSize++] = drawCard();
    }
}

void playRound() {
    
    pSize = 0; dSize = 0;
    gamePot = 0; currentBet = 0;

    // KOSZTY WEJŚCIOWE
    
    pCredits -= 15; dCredits -= 15; 
    gamePot += 20; sabaccPot += 10;

    pHand[pSize++] = drawCard(); 
    pHand[pSize++] = drawCard();
    dHand[dSize++] = drawCard(); 
    dHand[dSize++] = drawCard();

    for (int turn = 1; turn <= 3; turn++) {
        
        cout << endl << "========= ROUND " << turn << " =========" << endl;
        showStatus(false);

        cout << endl << "--- YOUR MOVE ---" << endl;
        cout << "1. Take card ($5)" << endl << "2. Exchange card ($10)" << endl << "3. Wait" << endl << "?> Choice: ";
        
        int choice; cin >> choice;
        
        if (choice == 1 && pCredits >= 5) {
            
            pHand[pSize++] = drawCard();
            pCredits -= 5; gamePot += 5;
        } 
        else if (choice == 2 && pSize > 0 && pCredits >= 10) {
            
            cout << "Card to change (1-" << pSize << "): ";
            
            int idx; cin >> idx;
            
            if (idx > 0 && idx <= pSize) {
                
                pHand[idx-1] = drawCard();
                pCredits -= 10; gamePot += 10;
            }
        }

        dealerAction();

        if (!handleBetting()) return;
        
        handleSpike();
    }

    showStatus(true);
    
    int pScore = calculateScore(pHand, pSize);
    int dScore = calculateScore(dHand, dSize);
    int pAbs = myAbs(pScore), dAbs = myAbs(dScore);

    bool playerWins = (pAbs < dAbs) || (pAbs == dAbs && pScore >= dScore);

    if (playerWins && pAbs <= 23) {
        
        cout << "YOU'RE A WINNER! You won $" << gamePot << "." << endl;
        pCredits += gamePot;
        
        if (pScore == 0) {
            
            cout << "PURE SABACC! Bonus: $" << sabaccPot << "." << endl;
            
            pCredits += sabaccPot; 
            sabaccPot = 0;
        }
    } 
    else {
        
        cout << "YOU LOST. Dealer won." << endl;
        
        dCredits += gamePot;
    }
}

int main() {
    
    srand(time(0));
    
    cout << "=== WELCOME IN CANTYNA - SABACC CORELLIAN SPIKE ===" << endl;
    
    char playAgain = 'y';
    
    while ((playAgain == 'y' || playAgain == 'Y') && pCredits > 0) {
        
        playRound();
        
        if (pCredits <= 0) {
            
            cout << endl << "Bankrut! You've been thrown out." << endl;
            break;
        }
        
        cout << endl << "Wanna play again? (y/n): ";
        
        cin >> playAgain;
    }
    return 0;
}