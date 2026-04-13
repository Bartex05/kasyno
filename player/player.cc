#include <iostream>
#include <string>
#include <fstream>

using namespace std;

        //  Obiekty tej klasy będą wykorzystywane do zapisywania pieniędzy wygranych lub przegranych NA KOŃCU danej gry/stołu po czym będzie wpisywany do pliku players.txt
class Player{
    private:
        int id;
        int money;
        string nickname;
    public:
        Player(int id, string nickname){                //  Konstruktor przy tworzeniu nowego obiektu gracza
            this->id = id;
            this->nickname = nickname;
            this->money = 5000;                         //  Startowa ilość kasy do ustalenia
        }
        Player(int id, string nickname, int money){     //  Konstruktor przy tworzeniu obiektu gracza z pliku players.txt
            this->id = id;
            this->nickname = nickname;
            this->money = money;
        }
        void setMoney(int money){                       //  Zapisywanie pieniędzy w formacie wstawiania nowej wartości
            this->money = money;
        }
        int getMoney(){                                 //  Pobieranie pieniędzy
            return this->money;
        }
        string getNickname(){                           //  Pobieranie nazwy gracza (ustalanie nazwy gracza można zrobić tylko przy tworzeniu obiektu klasy)
            return this->nickname;
        }
};

int main(int argc, char const *argv[])
{
    
    return 0;
}
