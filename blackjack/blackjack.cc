#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>

using namespace std;

    //  Podstawowa klasa karty z zmienną ukrytej karty dealer-a
class Card{
    private:
        int faceValue;
        string faceType, figureType;
        bool isHidden;
    
    public:
        Card(int faceValue, string faceType){
            this->faceValue = faceValue;
            this->faceType = faceType;
            this->isHidden = false;
        }

        void setFaceValue(int faceValue){
            this->faceValue = faceValue;
        }
        int getFaceValue(){
            return this->faceValue;
        }
        void setFaceType(string faceType){
            this->faceType = faceType;
        }
        string getFaceType(){
            return this->faceType;
        }
        void setFigureType(string figureType){
            this->figureType = figureType;
        }
        string getFigureType(){
            return this->figureType;
        }
        void setIsHidden(bool isHidden){
            this->isHidden = isHidden;
        }
        bool getIsHidden(){
            return this->isHidden;
        }
};

    //  Funkcja wypisująca przekazany wektor kart
void writeCards(vector<Card> v){
    for(auto c : v){
        if(c.getFigureType() == "Number"){
            cout<<c.getFaceValue()<<" of "<<c.getFaceType()<<endl;
        }else{
            cout<<c.getFigureType()<<" of "<<c.getFaceType()<<endl;
        }
    }
}

    //  Funkcja sprawdzająca aktualną wratość wszystkich kart i przekazywaniu czy ich łączna wartość przebija 21 (wliczając logikę miekkich asów)
int handValue(vector<Card> v){
    int valueCounter = 0, aceAmount = 0;
    for(auto c: v){
        valueCounter += c.getFaceValue();
        if(c.getFigureType() == "Ace"){
            aceAmount++;
        }
    }
    if(aceAmount > 0 && valueCounter + 10 <= 21){
        valueCounter += 10;
    }
    return valueCounter;
}

    //  Główna funkcja zawierająca całką logikę grę
int Blackjack(int money){

    int bettingMoney;

    cout<<"Current bank amount: "<<money<<endl;
    cout<<"(Note: The lowest denomination is of a white chip worth $1 so you can't bet non-whole numbers): "<<endl;
    while(money < bettingMoney && bettingMoney < 1){
        cout<<"How much are you betting?: ";
        cin>>bettingMoney;
        if(money < bettingMoney){
            cout<<"You can't bet the money you don't have"<<endl;
        }
        if(bettingMoney < 1){
            cout<<"You can't bet negative or zero amount of money"<<endl;
        }
    }

        //  Vectory wszystkich kart, dealera i gracza
    vector<Card> playingCards;
    vector<Card> dealerCards;
    vector<Card> playerCards;
    vector<Card> splitPlayerCards;

    cout<<"You have chosen Blackjack"<<endl;
    cout<<"At this casino there are no Hole Cards and the Dealer doesn't check the face-down card until all player actions are complete"<<endl;
    cout<<"Are you going to approach the table where the dealer Hits soft 17 or the table where he Stays at soft 17? (H/S): ";     //   Wybór miękkiej 17
    string soft17;
    while(true){
        cin>>soft17;
        if(soft17 == "H" || soft17 == "S"){break;}
    }

        //  Wypełnienie vectora wszystkimi kartami używanymi podczas gry w Blackjack i potasowanie jej
    for(int i = 0; i < 4; i++){
        for(int j = 1; j <= 13; j++){
            Card playCard(0,"Spades");     //   Podstawowa karta jest ostatnią wpisywaną aby zmniejszyć ilość wykonywanych działań

            if(i == 0){playCard.setFaceType("Hearts");}
            else if(i == 1){playCard.setFaceType("Diamonds");}
            else if(i == 2){playCard.setFaceType("Clubs");}

            if(j > 10){
                playCard.setFaceValue(10);
                if(j==11){
                    playCard.setFigureType("Jack");
                }else if(j==12){
                    playCard.setFigureType("Queen");
                }else{
                    playCard.setFigureType("King");
                }
            }else if(j == 1){
                playCard.setFaceValue(1);
                playCard.setFigureType("Ace");
            }else{
                playCard.setFaceValue(j);
                playCard.setFigureType("Number");
            }

            playingCards.push_back(playCard);
        }
    }

        //  Losowe tasowanie kart
    random_device rd;
    mt19937 g(rd());
    shuffle(playingCards.begin(),playingCards.end(),g);

        //  --->    PIERWSZE ROZDANIE KART  <---
    for(int i = 0; i < 2; i++){
        dealerCards.push_back(playingCards.back());
        if(i == 1){
            dealerCards.back().setIsHidden(true);
        }
        playingCards.pop_back();
        playerCards.push_back(playingCards.back());
        playingCards.pop_back();
    }

        //  Wypisanie posiadanych kart dealera bez wypisywania ukrytej karty
    cout<<"Dealer's cards: "<<endl;
    for(auto pCard : dealerCards){
        if(pCard.getIsHidden() == false){
            if(pCard.getFigureType() == "Number"){
                cout<<pCard.getFaceValue()<<" of "<<pCard.getFaceType()<<endl;
            }else{
                cout<<pCard.getFigureType()<<" of "<<pCard.getFaceType()<<endl;
            }
        }else{
            cout<<"* FACE DOWN *"<<endl;
        }
    }

        //  Gdy gracz na wejściu ma 21 to dostaje 1.5 dodatkowych pieniędzy i kończy grę bo ma natural
    if(handValue(playerCards) == 21){
        cout<<"Your cards: "<<endl;             
        writeCards(playerCards);
        if(handValue(dealerCards) == 21){
            cout<<"Dealer cards: "<<endl;             
            writeCards(dealerCards);
            cout<<"Both you and the dealer have a natural hand. Your bet is returned"<<endl;
            return money;
        }else{
            cout<<"You got a NATURAL set of cards. You win with 150% bet money.";
            return money + bettingMoney * 1.5;
        }
    }

    bool splitFlag = false;

        //  Ta zmienna istnieje po to aby wiedzieć kiedy gracz postanowił "STAY" na obu taliach 0 - brak wyboru lub "HIT" na obu, 1 - "STAY" lub "BUST" na pierwszej talii, 2 - "STAY" lub "BUST" na drugiej talii
    int splitStay = 0;

    bool bust = false;

    while(true){        //  ---> PĘTLA GRY <---

            //  Wypisanie posiadanych kart gracza i sprawdzenie czy nie przebił 21
        if(splitFlag == false){
            cout<<"Your cards: "<<endl;
        }else{
            cout<<"Your first hand cards: "<<endl;
        }
        writeCards(playerCards);

            //  Logika wykorzystująca handValue do sprawdzenia czy gracz przegrał
        if(handValue(playerCards) > 21){
                //  Gdy nie ma się dzielonych kart i się przebiło 21 to się z automatu przegrywa ale reszta kodu poza pętlą gry wciąż się odbywa aby gracz mógł zobaczyć karty dealera
            if(splitFlag == false){
                cout<<"You bust"<<endl;
                bust = true;
                break;
            }else{
                //  Ale jeśli ma dzielone karty "SPLIT" to druga talia może być wciąż wygrana jeśli druga co obsługuje ten if oraz pierwsza funkcja po wyjściu z pętli gry
                cout<<"First hand bust!"<<endl;
                if(splitStay == 2){
                    bust = true;
                    break;
                }
                splitStay = 1;
            }
        }

            //  Jeśli gracz wybrał split to wypisz też karty z drugiej talii i sprawdź czy nie przebił 21 (aplikują się te same zasady co w funkcji powyżej dla pierwszej ale nie jedynej talii)
        if(splitFlag == true){
            cout<<"Your second hand cards: "<<endl;
            writeCards(splitPlayerCards);
            
            if(handValue(splitPlayerCards) > 21){
                cout<<"Second hand bust!"<<endl;
                if(splitStay == 1){
                    bust = true;
                    break;
                }
                splitStay = 2;
            }
        }

            //  Jeśli gracz ma dwie karty to ma więcej możliwości które mu / jej wypisane
        if(playerCards.size() == 2){    
            cout<<"Double down (DD), Split (SP), Surrender (SU), ";
        }

        string playerChoice, splitPlayerChoice;

            //  Jak nie ma dwóch kart to niezależnie od ilości kart wypisuje te dwie opcje
        cout<<"Hit (H) or Stay (S)? : "<<endl;
        if(splitStay != 1 && splitFlag == true){
            cout<<"(First hand): ";
            while(true){
                cin>>playerChoice;
                if(playerChoice == "H"){    //  Z jakiegoś powodu którego nie potrafię rozgarnąć gdy zmienna ze stringiem jest zapisana w postaci playerChoice == "H" || playerChoice == "S" to ta funkcja nie działa więc rozpisuję ją w ten sposób
                    break;
                }else if(playerChoice == "S"){
                    break;
                }else{
                    cout<<playerChoice<<" is not a valid choice"<<endl;
                }
            }
        }
            //  "HIT" i "STAY" dla drugiej talii kart
        if(splitStay != 2 && splitFlag == true){
            cout<<"(Second hand): ";
            cin>>splitPlayerChoice;
            while(true){
                cin>>splitPlayerChoice;
                if(splitPlayerChoice == "H"){   //  Ten sam problem aplikuje się tutaj
                    break;
                }else if(splitPlayerChoice == "S"){
                    break;
                }else{
                    cout<<splitPlayerChoice<<" is not a valid choice"<<endl;
                }
            }
        }

            //  Jak talia jest "SPLIT" to ten playerChoice jest nieaktywny
        if(splitFlag == false){
            cin>>playerChoice;
        }

            //  --->    LOGIKA WYBORÓW  <---
        if(splitFlag == true){  //  SPLIT LOGIC
            if(splitStay != 1){
                if(playerChoice == "H"){
                    playerCards.push_back(playingCards.back());
                    playingCards.pop_back();
                }else if(playerChoice == "S"){
                    if(splitStay == 2){
                        break;
                    }else{
                        splitStay = 1;
                    }
                }
            }
            if(splitStay != 2){
                if(splitPlayerChoice == "H"){
                    splitPlayerCards.push_back(playingCards.back());
                    playingCards.pop_back();
                }else if(splitPlayerChoice == "S"){
                    if(splitStay == 1){
                        break;
                    }else{
                        splitStay = 2;
                    }
                }
            }
        }else if(playerChoice == "H"){        //  HIT
            playerCards.push_back(playingCards.back());
            playingCards.pop_back();
            writeCards(playerCards);
        }else if(playerChoice == "S"){      //  STAY
            break;
        }else if(playerCards.size() == 2){
            if(playerChoice == "SU"){       //  SURRENDER
                cout<<"You have decided to surrender. You will recieve back half of your betting amount";
                return money + bettingMoney * 0.5;
            }else if(playerChoice == "DD"){
                if(bettingMoney * 2 > money){
                    cout<<"You don't have enough money to double down"<<endl;
                }else{
                    bettingMoney = bettingMoney * 2;
                    playerCards.push_back(playingCards.back());
                    playingCards.pop_back();
                    break;
                }
            }else if(playerChoice == "SP"){ //  SPLIT CREATION (Ten wybór zawiera tylko stworzenie drugiej talii. Nie posiada logiki dalszej gry, dlatego pierwsza opcja w logice wyborów sprawdza flagę splitowanej ręki)
                if(playerCards.at(0).getFigureType() == playerCards.at(1).getFigureType() && playerCards.at(0).getFaceValue() == playerCards.at(1).getFaceValue()){
                    splitPlayerCards.push_back(playerCards.back());
                    playerCards.pop_back();
                    playerCards.push_back(playingCards.back());
                    playingCards.pop_back();
                    splitPlayerCards.push_back(playingCards.back());
                    playingCards.pop_back();
                    splitFlag = true;
                }
            }else{
                cout<<playerChoice<<" is not a valid choice"<<endl;
            }
        }else{
                cout<<playerChoice<<" is not a valid choice"<<endl;
        }
    }
            //  --->    DEALER    <---
        //  Zmienna do późniejszego przekazania wyniku gry dla gracza
    bool dealerBust = false;

        //  Cała logika dealera zgodnie z zasadami Amerykańskiego blackjacka
    if(soft17 == "H"){
        while(handValue(dealerCards) <= 17){
            if(handValue(dealerCards) != 17){
                dealerCards.push_back(playingCards.back());
                playingCards.pop_back();
            }else{
                int valueCounter = 0, aceAmount = 0;
                for(auto i : dealerCards){
                    valueCounter += i.getFaceValue();
                    if(i.getFigureType() == "Ace"){
                        aceAmount++;
                    }
                }
                if(aceAmount > 0 && valueCounter + 10 == 17){
                    dealerCards.push_back(playingCards.back());
                    playingCards.pop_back();
                }
            }
        }
    }else if(soft17 == "S"){
        while(handValue(dealerCards) < 17){
            dealerCards.push_back(playingCards.back());
            playingCards.pop_back();
        }

    }

        //  Ostateczne sprawdzanie czy dealer przebił 21
    if(handValue(dealerCards) > 21){
        dealerBust = true;
    }

        //  Wypisanie posiadanych kart dealera z wypisywaniem ukrytej karty
    cout<<"Dealer's cards: "<<endl;
    writeCards(dealerCards);

    if(bust == true && dealerBust == true){
        cout<<"Both you and the dealer bust however house rules state that in this situation player loses their bet."<<endl;
        return money - bettingMoney;
    }else if(bust == true){
        cout<<"You bust and lost your bet."<<endl;
        return money - bettingMoney;
    }else if(bust == false && dealerBust == true){
        cout<<"Dealer bust and you didn't. You win."<<endl;
        return money + bettingMoney;
    }else if(bust == false && handValue(dealerCards) > handValue(playerCards)){
        cout<<"Dealer has a better hand than you. You lost your bet."<<endl;
        return money - bettingMoney;
    }else if(bust == false && handValue(dealerCards) < handValue(playerCards)){
        cout<<"You have a better hand than dealer. You won."<<endl;
        return money + bettingMoney;
    }

    return money;
}

int main(int argc, char const *argv[])
{
    Blackjack(5000);
    return 0;
}
