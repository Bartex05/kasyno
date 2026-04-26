#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <random>
#include <map>

using namespace std;

class Player{
    private:
        int id;
        int money;                                   
        string nickname;
    public:
        Player(){                                           //  Defaults
            this->id = 0;
            this->nickname = "placeholder";
            this->money = 5000;                             //  Startowa ilość kasy do ustalenia
        };
        Player(int id, string nickname, int money){         //  Konstruktor przy tworzeniu obiektu currentPlayer za pomocą danych z pliku players.txt
            this->id = id;
            this->nickname = nickname;
            this->money = money;
        }

        void setId(int id){                                 //  Zapisywanie i pobieranie wartości
            this->id = id;
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
        
        int addPlayer(string);
        int loadPlayer();
};

int Blackjack(int bank){
    int bettingMoney;
    cin>>bettingMoney;
    bank = bank - bettingMoney;
        //GRA
    cout<<"Wybrano Blackjack"<<endl;

    return bank + bettingMoney;
}

void Ruletka(){
    cout<<"Wybrano Ruletke"<<endl;
}

int main()
{
    
}