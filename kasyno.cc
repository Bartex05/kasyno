#include <iostream>

using namespace std;

int main()
{
<<<<<<< Updated upstream

=======
    int gameChoice = -1;     //Ta zmienna jak i repeat jest umieszczona tutaj dla efektywnego powtarzania gry
    string gameChoiceRepeat;
    Player player;          //Zabiera z pliku jeśli istnieje a jak nie to tworzy defaultowego gracza TODOOOOOOOO
    while(true){
        cout<<"===>        WYBOR GRY        <==="<<endl;
    
        cout<<"1. Blackjack"<<endl;         
        cout<<"2. Ruletka"<<endl;           //Do dopisania więcej

        cout<<"0. Wyjdz"<<endl;

        cout<<"Prosze podac numer"<<endl;

        while(gameChoice < 0){                        //Podczas dopisywania większej ilości gier tutaj trzeba dodać jej numer w postaci warunku
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
                int moneyToSend = Blackjack(player.getMoney());
                player.setMoney(moneyToSend);
                    cout<<"Do you want to play again? (Y/N): "<<endl;
                while(true){
                    cin>>gameChoiceRepeat;
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
>>>>>>> Stashed changes
}