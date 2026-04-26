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
<<<<<<< HEAD
    int gameChoice = -1;     //Ta zmienna jak i repeat jest umieszczona tutaj dla efektywnego powtarzania gry
    string gameChoiceRepeat;
    while(true){
        cout<<"===>        WYBOR GRY        <==="<<endl;
    
        cout<<"1. Blackjack"<<endl;         
        cout<<"2. Ruletka"<<endl;           //Do dopisania więcej

=======
<<<<<<< Updated upstream

=======
    int gameChoice = -1;     //Ta zmienna jak i repeat jest umieszczona tutaj dla efektywnego powtarzania gry
    string gameChoiceRepeat;
    Player player;          //Zabiera z pliku jeśli istnieje a jak nie to tworzy defaultowego gracza TODOOOOOOOO
    while(true){
        cout<<"===>        WYBOR GRY        <==="<<endl;
    
        cout<<"1. Blackjack"<<endl;         
        cout<<"2. Ruletka"<<endl;           //Do dopisania więcej

>>>>>>> 55a73f527eed1c1ae6827fee94daa57c90bc6c19
        cout<<"0. Wyjdz"<<endl;

        cout<<"Prosze podac numer"<<endl;

<<<<<<< HEAD
        while(true){                        //Podczas dopisywania większej ilości gier tutaj trzeba dodać jej numer w postaci warunku
=======
        while(gameChoice < 0){                        //Podczas dopisywania większej ilości gier tutaj trzeba dodać jej numer w postaci warunku
>>>>>>> 55a73f527eed1c1ae6827fee94daa57c90bc6c19
            if(gameChoice == 0){
                return 0;
            }
            cin>>gameChoice;
            if(gameChoice >= 1 && gameChoice <= 2/*Tutaj dopisywane sprawdzanie liczby gry w postaci gameChoice >= 1 && gameChoice <= *Aktualna liczba możliwych gier* */){         // <----------------------------
                break;
            }else if(gameChoice != 0){
                cout<<"Nie ma gry o takim numerze"<<endl;
            }
        }

        switch(gameChoice){
            case 1:                         //Podczas dopisywania większej ilości gier tutaj trzeba dodać jej case w tym samym numerze co w menu tekstowym                          // <----------------------------
<<<<<<< HEAD
                Blackjack(5000);
                cout<<"Do you want to play again? (Y/N): "<<endl;
                    while(true){
                        cin>>gameChoiceRepeat;
=======
                int moneyToSend = Blackjack(player.getMoney());
                player.setMoney(moneyToSend);
                    cout<<"Do you want to play again? (Y/N): "<<endl;
                while(true){
                    cin>>gameChoiceRepeat;
>>>>>>> 55a73f527eed1c1ae6827fee94daa57c90bc6c19
                    if(gameChoiceRepeat == "Y"){
                        break;
                    }
                    if(gameChoiceRepeat == "N"){
                        gameChoice = -1;
                    }
                }
                
                break;
            case 2:
                gameChoice = -1;
                Ruletka();
                break;
            default:                        //Ze względu na zabezpieczony brak nieodpowiedniego case, default jest używany do powtarzania gry poprzez niezmianianą zmienną 'gameChoice'
                cout<<"Brak wybranej gry"<<endl;
                return 0;
        }
    }
    return 0;
<<<<<<< HEAD
=======
>>>>>>> Stashed changes
>>>>>>> 55a73f527eed1c1ae6827fee94daa57c90bc6c19
}