#include <iostream>
#include <string>

using namespace std;

void Blackjack(){
    cout<<"Wybrano Blackjack"<<endl;
}

void Ruletka(){
    cout<<"Wybrano Ruletke"<<endl;
}

int main(int argc, char const *argv[])
{
    int gameChoice = -1;     //Ta zmienna jak i repeat jest umieszczona tutaj dla efektywnego powtarzania gry
    int gameChoiceRepeat = -1;
    while(true){
        cout<<"===>        WYBOR GRY        <==="<<endl;
    
        cout<<"1. Blackjack"<<endl;         
        cout<<"2. Ruletka"<<endl;           //Do dopisania więcej

        cout<<"0. Wyjdz"<<endl;

        cout<<"Prosze podac numer"<<endl;

        while(true){                        //Podczas dopisywania większej ilości gier tutaj trzeba dodać jej numer w postaci warunku
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
                gameChoice = -1;
                Blackjack();
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
}
