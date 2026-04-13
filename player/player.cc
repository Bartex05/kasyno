#include <iostream>
#include <string>
#include <fstream>
#include <vector>

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
        Player(int id, string nickname, int money){     //  Konstruktor przy tworzeniu obiektu currentPlayer za pomocą danych z pliku players.txt
            this->id = id;
            this->nickname = nickname;
            this->money = money;
        }
        void setMoney(int money){                       //  Zapisywanie i pobieranie wartości
            this->money = money;
        }
        int getMoney(){                                 
            return this->money;
        }
        void setNickname(string nickname){
            this->nickname = nickname;
        }
        string getNickname(){                           
            return this->nickname;
        }
        void setId(int id){
            this->id = id;
        }
};


int main(int argc, char const *argv[])
{

    return 0;
}
