#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>

using namespace std;

        //  Obiekty tej klasy będą wykorzystywane do zapisywania pieniędzy wygranych lub przegranych NA KOŃCU danej gry/stołu po czym będzie wpisywany do pliku players.txt
class Player{
    private:
        int money;                                   
        string nickname;
    public:
        Player(){                                           //  Defaults
            this->nickname = "placeholder";
            this->money = 5000;                             //  Startowa ilość kasy do ustalenia
        };
        Player(int id, string nickname, int money){         //  Konstruktor przy tworzeniu obiektu currentPlayer za pomocą danych z pliku players.txt
            this->nickname = nickname;
            this->money = money;
        }

        void setNickname(string nickname){
            this->nickname = nickname;
        }
        string getNickname(){                           
            return this->nickname;
        }
        void setMoney(int money){                       
            this->money = money;
        }
        int getMoney(){                                 
            return this->money;
        }
};

//  Funkcja sprawdzani istnienia gracza
bool fileExists(string playerName) {
    ifstream infile(playerName + ".txt");
    return infile.good();
}

bool isNumber(string str) {
    for (char c : str) {
        if (isdigit(c) == 0) return false;
    }
    return true;
}

//  Funkcja do tworzenia pliku nowego gracza
void createPlayer(string playerName, int money){
    if (!fileExists(playerName)) {
        ofstream file(playerName + ".txt", ios::out);
        cout << "Please set a password: ";
        string x;
        cin >> x;
        file << x;
        cout << "How much money do you want to start with?: ";
        while (true) {
            cin >> x;
            if (isNumber) {
                file << x;
            }
            else {
                cout << x << " is not a number. Enter amount: ";
            }
        }
        file.close();
    }
}

//  Funkcja do pobierania danych gracza z pliku o nazwie gracza i przekazywania danych do currentPlayer
Player getCurrentPlayer(string playerName){
    ifstream file(playerName + ".txt", ios::in);



    file.close();
}

int main(int argc, char const *argv[])
{

    return 0;
}
