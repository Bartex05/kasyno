#include <iostream>
#include <random>
#include <map>
#include <ctime>

using namespace std;

// slotSymbols
map<char, int> slotSymbols = {
    {'X', 100},    // Diament
    {'7', 50},     // Jackpot
    {'G', 25},     // Gwiazda (Wild)
    {'D', 15},     // Dzwonek
    {'W', 10},     // Winogrono 
    {'P', 5},      // Pomarańcza
    {'L', 2},      // Lemon 
    {'C', 1}       // Cherry
};

bool slotIsCompatible(char a, char b) {
    
    if (a == 'G' || b == 'G') return true;
    
    return a == b;
}

char slotGetWinningSymbol(char a, char b, char c) {
    
    if (a != 'G') return a;
    if (b != 'G') return b;
    if (c != 'G') return c;
    
    return 'G';
}

void slotDisplayGrid(char grid[3][3])
{
    cout << endl << " --- SLOTS ---" << endl;
    
    for (int i = 0; i < 3; i++) 
    {
        cout << "  ";
        
        for (int j = 0; j < 3; j++) 
        {
            cout << "[" << grid[i][j] << "] ";
        }
        
        cout << endl;
    }
    cout << " -------------" << endl << endl;
}

int slotCheckLine(char a, char b, char c, int bet) {
    
    if (slotIsCompatible(a, b) && slotIsCompatible(b, c) && slotIsCompatible(a, c)) 
    {
        char winSymbol = slotGetWinningSymbol(a, b, c);
        
        int winAmount = bet * slotSymbols[winSymbol];
        
        cout << ">> WIN! Combination: " << a << b << c << " (Value: " << winSymbol << ")" << endl;
        
        return winAmount;
    }
    else
    {
        return 0;
    }
    
}

int slot(int money) 
{
    srand(time(0));
    char grid[3][3];
    
    char slotSymbols[3][10] = 
    {
        {'7', 'L', 'X', 'D', 'P', 'W', 'L', 'G', 'C', 'D'},
        {'7', 'P', 'L', 'D', 'X', 'G', 'W', 'L', 'P', 'C'},
        {'C', 'X', 'W', 'L', 'D', 'P', 'C', 'L', 'W', 'D'},
    };
    
	int balance = money;
    int bet;

    cout << "Welcome to the Casino!" << endl;
    cout << "Starting Balance: $" << balance << endl;

    while (balance > 0) 
    {
        cout << "Enter bet amount (anything else to quit): $";
        cin >> bet;

        if (bet =< 0) break;
        
        if (bet > balance) 
        {
            cout << "Not enough money!" << endl;
            continue;
        }

        balance -= bet;

        for (int i = 0; i < 3; i++) 
        {
            int randModulo = rand() % 10;
            
            grid[1][i] = slotSymbols[i][randModulo];
            
            switch(randModulo) 
            {
                case 0:
                grid[0][i] = slotSymbols[i][9];
                grid[2][i] = slotSymbols[i][1];
                break;
                
                case 9:
                grid[0][i] = slotSymbols[i][8];
                grid[2][i] = slotSymbols[i][0];
                break;
                
                default:
                grid[0][i] = slotSymbols[i][randModulo - 1];
                grid[2][i] = slotSymbols[i][randModulo + 1];
                break;
            }
        }
        
        slotDisplayGrid(grid);
        int win = 0;
        
        // Sprawdzanie rzędów
        win += slotCheckLine(grid[0][0], grid[0][1], grid[0][2], bet);
        win += slotCheckLine(grid[1][0], grid[1][1], grid[1][2], bet);
        win += slotCheckLine(grid[2][0], grid[2][1], grid[2][2], bet);
        
        // Sprawdzanie skosów
        win += slotCheckLine(grid[0][0], grid[1][1], grid[2][2], bet);
        win += slotCheckLine(grid[2][0], grid[1][1], grid[0][2], bet);
        
        if (win > 0) 
        {
            cout << "You won: $" << win << "!" << endl;
            balance += win;
        } 
        else 
        {
            cout << "No luck this time." << endl;
        }

        cout << "Current Balance: $" << balance << endl;
    }

    cout << "Game over! You have: $" << balance << endl;
    
    return 0;
}