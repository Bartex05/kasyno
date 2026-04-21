#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// --- ZMIENNE GLOBALNE ---
int pHand[15], dHand[15];
int pSize = 0, dSize = 0;
int pCredits = 150, dCredits = 150; 
int gamePot = 0, sabaccPot = 0, currentBet = 0;

// --- FUNKCJE POMOCNICZE ---

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
    if (r < 2) return 0; // Sylopy
    if (r < 32) return (rand() % 10) + 1; // Zielone 1-10
    return -((rand() % 10) + 1); // Czerwone 1-10
}

string getCardName(int v) {
    if (v == 0) return "Sylop (0)";
    if (v > 0) return "Zielona +" + to_string(v);
    return "Czerwona " + to_string(v);
}

// --- LOGIKA AI DEALERA ---

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
            cout << ">> Dealer zaplacil 10 cr i wymienil karte." << endl;
            return; 
        }
    }

    // 2. Logika dobierania (Koszt 5)
    if (dCredits >= 5 && currentAbs > 7) {
        dHand[dSize++] = drawCard();
        dCredits -= 5;
        gamePot += 5;
        cout << ">> Dealer zaplacil 5 cr i dobral karte." << endl;
    }
}

int dealerDecideBet() {
    int score = myAbs(calculateScore(dHand, dSize));
    if (currentBet > 20 && score > 8) return 0; // Fold
    if (score <= 3 && (rand() % 100 < 30)) return 2; // Raise
    return 1; // Call
}

// --- FAZY GRY ---

void showStatus(bool finalReveal) {
    cout << "\n-----------------------------------------------------------" << endl;
    cout << " KREDYTY: " << pCredits << " | PULA: " << gamePot << " | SABACC POT: " << sabaccPot << endl;
    cout << "-----------------------------------------------------------" << endl;
    
    cout << "Twoja reka: ";
    for (int i = 0; i < pSize; i++) cout << "[" << getCardName(pHand[i]) << "] ";
    cout << " (Suma: " << calculateScore(pHand, pSize) << ")" << endl;

    if (finalReveal) {
        cout << "Reka Dealera: ";
        for (int i = 0; i < dSize; i++) cout << "[" << getCardName(dHand[i]) << "] ";
        cout << " (Suma: " << calculateScore(dHand, dSize) << ")" << endl;
    } else {
        cout << "Dealer posiada " << dSize << " kart." << endl;
    }
    cout << "-----------------------------------------------------------" << endl;
}

bool handleBetting() {
    cout << "\n--- FAZA LICYTACJI ---" << endl;
    cout << "Aktualna stawka: " << currentBet << endl;
    cout << "1. Sprawdz (Call)\n2. Podbij (Raise)\n3. Pasuj (Fold)\nWybor: ";
    int choice; cin >> choice;

    if (choice == 3) {
        cout << ">> Spasowales. Dealer zgarnia pule." << endl;
        dCredits += gamePot; return false; 
    }
    if (choice == 2) {
        int raise; cout << "O ile podbijasz?: "; cin >> raise;
        currentBet += raise;
    }

    int dAction = dealerDecideBet();
    if (dAction == 0) {
        cout << ">> Dealer spasowal! Wygrywasz " << gamePot << " kredytow." << endl;
        pCredits += gamePot; return false;
    } else if (dAction == 2) {
        int dRaise = (rand() % 10) + 1;
        currentBet += dRaise;
        cout << ">> Dealer podbija o " << dRaise << "!" << endl;
    } else {
        cout << ">> Dealer sprawdza." << endl;
    }

    pCredits -= currentBet;
    dCredits -= currentBet;
    gamePot += (currentBet * 2);
    return true;
}

void handleSpike() {
    int d1 = rand() % 6 + 1, d2 = rand() % 6 + 1;
    if (d1 == d2) {
        cout << ">> Rzut koscmi Spike: [" << d1 << "] [" << d2 << "] !!! SPIKE !!!" << endl;
        int oldP = pSize, oldD = dSize;
        pSize = 0; dSize = 0;
        for (int i = 0; i < oldP; i++) pHand[pSize++] = drawCard();
        for (int i = 0; i < oldD; i++) dHand[dSize++] = drawCard();
    }
}

void playRound() {
    pSize = 0; dSize = 0;
    gamePot = 0; currentBet = 0;

    // --- KOSZTY WEJŚCIOWE ---
    pCredits -= 15; dCredits -= 15; 
    gamePot += 20; sabaccPot += 10;

    pHand[pSize++] = drawCard(); 
    pHand[pSize++] = drawCard();
    dHand[dSize++] = drawCard(); 
    dHand[dSize++] = drawCard();

    for (int turn = 1; turn <= 3; turn++) {
        cout << "\n========= TURA " << turn << " =========" << endl;
        showStatus(false);

        cout << "\n--- TWOJA AKCJA ---" << endl;
        cout << "1. Dobierz (5 cr)\n2. Wymien karte (10 cr)\n3. Czekaj\nWybor: ";
        int choice; cin >> choice;
        if (choice == 1 && pCredits >= 5) {
            pHand[pSize++] = drawCard();
            pCredits -= 5; gamePot += 5;
        } else if (choice == 2 && pSize > 0 && pCredits >= 10) {
            cout << "Nr karty do wymiany (1-" << pSize << "): ";
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
        cout << "WYGRANA! Zgarniasz " << gamePot << " cr." << endl;
        pCredits += gamePot;
        if (pScore == 0) {
            cout << "CZYSTY SABACC! Bonus: " << sabaccPot << " cr." << endl;
            pCredits += sabaccPot; 
            sabaccPot = 0;
        }
    } else {
        cout << "PRZEGRANA. Dealer wygrywa runde." << endl;
        dCredits += gamePot;
    }
}

int main() {
    srand(time(0));
    cout << "=== WITAJ W KANTYNIE - SABACC CORELLIAN SPIKE ===" << endl;
    char playAgain = 't';
    while ((playAgain == 't' || playAgain == 'T') && pCredits > 0) {
        playRound();
        if (pCredits <= 0) {
            cout << "\nBankrut! Zostales wyrzucony z kantyny." << endl;
            break;
        }
        cout << "\nCzy chcesz zagrac kolejna runde? (t/n): ";
        cin >> playAgain;
    }
    return 0;
}