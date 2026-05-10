#include <iostream>
#include <string>
#include <ctime>
#include <cmath>

using namespace std;

// ZMIENNE GLOBALNE

int sabaccHandPlayer[15];
int sabaccHandDealer[15];

int sabaccSizePlayer = 0; 
int sabaccSizeDealer = 0;

int sabaccCreditsPlayer = 150; 
int sabaccCreditsDealer = 150; 

int sabaccPotSabaccGame = 0; 
int sabaccPotSabacc = 0; 
int sabaccCurrentBet = 0;

int sabaccCreditsStart = 0;

// STAŁE PROCENTOWE

const double sabaccPercentageEnter = 0.10;    
const double sabaccPercentageDraw = 0.03;    
const double sabaccPercentageSwap = 0.06;    
const double sabaccPercentageSabacc = 0.05;
const double  sabaccPercentageRaise = 0.05;

// FUNKCJE POMOCNICZE

int sabaccCalculateScore(int hand[], int size) {
    
    int total = 0;
    
    for (int i = 0; i < size; i++) total += hand[i];
    
    return total;
}

int sabaccAbsolute(int n) {
    
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

void sabaccPrintCard(int hand[], int size, bool hide) {
    
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
        else if (valueInt < 0) {
            
            symbol = "R";
        }

        if (hide) {
            
            valueString = "?";
            symbol = "?";
        } 
        else {
            
            valueString = to_string(sabaccAbsolute(valueInt));
        }

        // Padding
        string valueLeft = valueString;
        string valueRight = valueString;
        
        if (valueString.length() == 1) {
            
            valueLeft = valueString + " "; // Np. "5 "
            valueRight = " " + valueString; // Np. " 5"
        }

        lines[0] += "\u250c\u2500\u2500\u2500\u2500\u2500\u2510 ";
        //lines[0] += "┌─────┐ ";
        lines[1] += "\u2502" + valueLeft + "   \u2502 ";
        lines[2] += "\u2502  " + symbol + "  \u2502 ";
        lines[3] += "\u2502   " + valueRight + "\u2502 ";
        lines[4] += "\u2514\u2500\u2500\u2500\u2500\u2500\u2518 ";
    }

    for (int i = 0; i < 5; i++) {
        
        cout << lines[i] << endl;
    }
}

int sabaccGetCost(double sabaccPercentage) {
    return (int)ceil(sabaccCreditsStart * sabaccPercentage);
}

int sabaccValidateInput(string prompt, int minValue, int maxValue) {
    
    string inputString;
    int inputInt;
    
    while (true) {
        
        cout << prompt;
        cin >> inputString;
        
        try {
            
            inputInt = stoi(inputString);
            inputInt = floor(inputInt);
            
            if (minValue == 0 && maxValue == 0){
                
                if (inputInt > 0) {
                    return inputInt;
                }
                else {
                    
                    cout << "!> ERROR: Input positive number of money!" << endl;
                }
                
            } else {
                
                if (inputInt >= minValue && inputInt <= maxValue) {
                
                return inputInt;
                
                }
                else {
                    cout << "!> ERROR: Input number from " << minValue << " to " << maxValue << "!" << endl;
                }
            }
        } 
        catch (...) {
            
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "!> ERROR: Input a valid number!" << endl;
        }
    }
}

// LOGIKA DEALERA

void sabaccDealerAction() {
    
    if (sabaccSizeDealer == 0) return;

    int currentScore = sabaccCalculateScore(sabaccHandDealer, sabaccSizeDealer);
    int currentAbs = sabaccAbsolute(currentScore);
    
    int swapCost = sabaccGetCost(sabaccPercentageSwap);
    int drawCost = sabaccGetCost(sabaccPercentageDraw);

    // Wymiana
    
    if (sabaccCreditsDealer >= swapCost && currentAbs > 5) {
        
        int bestIndex = -1;
        int bestAbs = currentAbs;

        for (int i = 0; i < sabaccSizeDealer; i++) {
            
            int scoreWithoutCard = currentScore - sabaccHandDealer[i];
            
            if (sabaccAbsolute(scoreWithoutCard) < bestAbs) {
                
                bestAbs = sabaccAbsolute(scoreWithoutCard);
                bestIndex = i;
            }
        }

        if (bestIndex != -1) {
            
            sabaccHandDealer[bestIndex] = drawCard();
            
            sabaccCreditsDealer -= swapCost;
            sabaccPotSabaccGame += swapCost;
            
            cout << ">> Dealer paid $" << swapCost << " to swap a card." << endl;
            return; 
        }
    }

    // 2. Logika dobierania (Koszt 5)
    
    if (sabaccCreditsDealer >= drawCost && currentAbs > 7) {
        
        sabaccHandDealer[sabaccSizeDealer++] = drawCard();
        
        sabaccCreditsDealer -= drawCost;
        sabaccPotSabaccGame += drawCost;
        
       cout << ">> Dealer paid $" << drawCost << " to draw a card." << endl;
    }
}

int sabaccDealerDecideBet() {
    
    int score = sabaccAbsolute(sabaccCalculateScore(sabaccHandDealer, sabaccSizeDealer));
    
    int limit = sabaccGetCost(sabaccPercentageEnter * 1.5);
    
    //Fold
    if (sabaccCurrentBet > limit && score > 10) return 0;
    
    // Raise return 2; 
    if (score <= 3 && (rand() % 100 < 30)) return 2;
    
    //Call
    return 1;
}

// FAZY GRY

void sabaccShowStatus(bool finalReveal) {
    
    cout << endl << "-----------------------------------------------------------" << endl;
    cout << " MONEY: $" << sabaccCreditsPlayer << " | TABLE POT: $" << sabaccPotSabaccGame << " | SABACC POT: $" << sabaccPotSabacc << endl;
    cout << "-----------------------------------------------------------" << endl;
    
    cout << "YOUR HAND:" << endl;
    
    sabaccPrintCard(sabaccHandPlayer, sabaccSizePlayer, false); 

    if (finalReveal) {

        cout << "Score: " << sabaccCalculateScore(sabaccHandPlayer, sabaccSizePlayer) << endl;
    }
    
    cout << "-----------------------------------------------------------" << endl;
    
    cout << "DEALER'S HAND:" << endl;
    
    sabaccPrintCard(sabaccHandDealer, sabaccSizeDealer, !finalReveal);

    if (finalReveal) {

        cout << "Score: " << sabaccCalculateScore(sabaccHandDealer, sabaccSizeDealer) << endl;
    }
    
    cout << "-----------------------------------------------------------" << endl;
}

bool sabaccHandleBetting() {
    
    cout << endl << "--- LICITATION ---" << endl;
    cout << "Current rate: $" << sabaccCurrentBet << endl;
    cout << "1. Call" << endl << "2. Raise" << endl << "3. Fold" << endl;
    
    int choice = sabaccValidateInput("?> Choice: ", 1, 3);

    if (choice == 3) {
        
        cout << ">> You folded. Dealer wins the money." << endl;
        
        sabaccCreditsDealer += sabaccPotSabaccGame; return false; 
    }
    if (choice == 2) {
        
        int raise = sabaccValidateInput("?> How much you raise?: ", 0, 0);
        
        sabaccCurrentBet += raise;
    }

    int sabaccActionDealer = sabaccDealerDecideBet();
    
    if (sabaccActionDealer == 0) {
        
        cout << ">> Dealer folded! You won $ " << sabaccPotSabaccGame << "1" << endl;
        
        sabaccCreditsPlayer += sabaccPotSabaccGame; return false;
    } 
    else if (sabaccActionDealer == 2) {
        
        int dRaise = sabaccGetCost(sabaccPercentageRaise) + (rand() % 10);

        sabaccCurrentBet += dRaise;
        
        cout << ">> Dealer raise for $" << dRaise << "!" << endl;
    } 
    else {
        
        cout << ">> Dealer calls." << endl;
    }

    sabaccCreditsPlayer -= sabaccCurrentBet;
    sabaccCreditsDealer -= sabaccCurrentBet;
   
    sabaccPotSabaccGame += (sabaccCurrentBet * 2);
   
    return true;
}

void handleSpike() {
    
    int d1 = rand() % 6 + 1, d2 = rand() % 6 + 1;
    
    if (d1 == d2) {
        
        cout << ">> Dice roll: [" << d1 << "] [" << d2 << "] !!! SPIKE !!!" << endl;
        
        int oldP = sabaccSizePlayer, oldD = sabaccSizeDealer;
        
        sabaccSizePlayer = 0; sabaccSizeDealer = 0;
        
        for (int i = 0; i < oldP; i++) sabaccHandPlayer[sabaccSizePlayer++] = drawCard();
        for (int i = 0; i < oldD; i++) sabaccHandDealer[sabaccSizeDealer++] = drawCard();
    }
}

void sabaccPlayRound() {
    
    int costEnter = sabaccGetCost(sabaccPercentageEnter);
    int costSabacc = sabaccGetCost(sabaccPercentageSabacc);
    
    if (sabaccCreditsPlayer < costEnter + costSabacc) {
        cout << "!> Not enough credits to play!" << endl;
        sabaccCreditsPlayer = 0; return;
    }
    
    sabaccSizePlayer = 0; 
    sabaccSizeDealer = 0;
    
    sabaccPotSabaccGame = costEnter * 2;
    sabaccPotSabacc += costSabacc * 2;

    sabaccCreditsPlayer -= (costEnter + costSabacc); 
    sabaccCreditsDealer -= (costEnter + costSabacc); 
    
    sabaccCurrentBet = 0;

    sabaccHandPlayer[sabaccSizePlayer++] = drawCard(); 
    sabaccHandPlayer[sabaccSizePlayer++] = drawCard();
    sabaccHandDealer[sabaccSizeDealer++] = drawCard(); 
    sabaccHandDealer[sabaccSizeDealer++] = drawCard();

    for (int turn = 1; turn <= 3; turn++) {
        
        cout << endl << "========= ROUND " << turn << " =========" << endl;
        sabaccShowStatus(false);

        cout << endl << "--- YOUR MOVE ---" << endl;
        cout << "1. Draw ($" << sabaccGetCost(sabaccPercentageDraw) << ")" << endl << "2. Swap ($" << sabaccGetCost(sabaccPercentageSwap) << ")" << endl << "3. Wait" << endl;
        
        int choice = sabaccValidateInput("?> Choice: ", 1, 3);
        
        if (choice == 1 && sabaccCreditsPlayer >= 5) {
            
            sabaccHandPlayer[sabaccSizePlayer++] = drawCard();
            
            sabaccCreditsPlayer -= sabaccGetCost(sabaccPercentageDraw); 
            sabaccPotSabaccGame += sabaccGetCost(sabaccPercentageDraw);
        } 
        else if (choice == 2 && sabaccSizePlayer > 0 && sabaccCreditsPlayer >= 10) {
            
            string cardPrompt = ">> Card to swap (1 - " + to_string(sabaccSizePlayer) + "): ";
            
            int card = sabaccValidateInput(cardPrompt, 1, sabaccSizePlayer);
            
            if (card > 0 && card <= sabaccSizePlayer) {
                
                sabaccHandPlayer[card-1] = drawCard();
                
                sabaccCreditsPlayer -= sabaccGetCost(sabaccPercentageSwap); 
                sabaccPotSabaccGame += sabaccGetCost(sabaccPercentageSwap);
            }
        }

        sabaccDealerAction();

        if (!sabaccHandleBetting()) return;
        
        handleSpike();
    }

    sabaccShowStatus(true);
    
    int sabaccScorePlayer = sabaccCalculateScore(sabaccHandPlayer, sabaccSizePlayer);
    int sabaccScoreDealer = sabaccCalculateScore(sabaccHandDealer, sabaccSizeDealer);
    int sabaccAbsolutePlayer = sabaccAbsolute(sabaccScorePlayer), sabaccAbsoluteDealer = sabaccAbsolute(sabaccScoreDealer);

    bool playerWins = (sabaccAbsolutePlayer < sabaccAbsoluteDealer) || (sabaccAbsolutePlayer == sabaccAbsoluteDealer && sabaccScorePlayer >= sabaccScoreDealer);

    if (playerWins && sabaccAbsolutePlayer <= 23) {
        
        cout << "!> YOU'RE A WINNER! You won $" << sabaccPotSabaccGame << "." << endl;
        sabaccCreditsPlayer += sabaccPotSabaccGame;
        
        if (sabaccScorePlayer == 0) {
            
            cout << "!> PURE SABACC! Bonus: $" << sabaccPotSabacc << "." << endl;
            
            sabaccCreditsPlayer += sabaccPotSabacc; 
            sabaccPotSabacc = 0;
        }
    } 
    else {
        
        cout << "!> YOU LOST. Dealer won." << endl;
        
        sabaccCreditsDealer += sabaccPotSabaccGame;
    }
}

int sabacc(int money) {
    
    srand(time(0));
    
    cout << "=== WELCOME IN CANTYNA - SABACC CORELLIAN SPIKE ===" << endl;
    
    sabaccCreditsStart = money;
    
    sabaccCreditsPlayer = sabaccCreditsStart;
    sabaccCreditsDealer = sabaccCreditsStart;
    
    char playAgain = 'y';
    
    while ((playAgain == 'y' || playAgain == 'Y') && sabaccCreditsPlayer > 0) {
        
        sabaccPlayRound();
        
        if (sabaccCreditsPlayer <= 0) {
            
            cout << endl << "!> Bankrut! You've been thrown out." << endl;
            break;
        }
        
        cout << endl << "?> Wanna play again? (y/n): ";
        
        cin >> playAgain;
    }
    return 0;
}