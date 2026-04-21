#include <iostream>
#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

        //  Podstawowa klasa karty
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

void writeCards(vector<Card> v){
    for(auto c : v){
        if(c.getFigureType() == "Number"){
            cout<<c.getFaceValue()<<" of "<<c.getFaceType()<<endl;
        }else{
            cout<<c.getFigureType()<<" of "<<c.getFaceType()<<endl;
        }
    }
}

int Blackjack(int money){
    int bettingMoney;
    int splitBettingMoney;
    cout<<"Current bank amount: "<<money<<endl;
    cout<<"How much are you going to bet? (Note: The lowest denomination is of a white chip worth $1): "<<endl;
    while(money < bettingMoney < 1){
        cin>>bettingMoney;
        if(money < bettingMoney){
            cout<<"You can't bet the money you don't have"<<endl;
        }
        if(bettingMoney < 1){
            cout<<"You can't bet negative or zero amount of money"<<endl;
        }
        cout<<"How much are you betting?: ";
    }
        //  Vectory wszystkich kart, dealera i gracza
    vector<Card> playingCards;
    vector<Card> dealerCards;
    vector<Card> playerCards;
    vector<Card> splitPlayerCards;

    cout<<"You have chosen Blackjack"<<endl;
    cout<<"At this casino there are no Hole Cards and the Dealer doesn't check the face-down card until all player actions are complete"<<endl;
    cout<<"Are you going to approach the table where the dealer Hits or the table where he Stays at soft 17? (H/S): ";     //Wybór miękkiej 17
    string soft17;
    while(true){
        cin>>soft17;
        if(soft17 == "H" || soft17 == "S"){break;}
    }

        //  Wypełnienie vectora wszystkimi kartami używanymi podczas gry w Blackjack i potasowanie jej
    for(int i = 0; i < 4; i++){
        for(int j = 1; j <= 13; j++){
            Card playCard(0,"Spades");     //Podstawowa karta jest ostatnią wpisywaną aby zmniejszyć ilość wykonywanych działań

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
                playCard.setFaceValue(11);
                playCard.setFigureType("Ace");
            }else{
                playCard.setFaceValue(j);
                playCard.setFigureType("Number");
            }

            playingCards.push_back(playCard);
        }
    }
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

    bool isPlayerStaying = false;   //  Ta zmienna to flaga służąca do sprawdzania czy gracz skończył dobierać karty aby dealer mógł dobierać swoje i zakończyć nie "NATURALNĄ" grę.
    bool splitFlag = false;

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

        //  Wypisanie posiadanych kart gracza
    cout<<"Your cards: "<<endl;             
    writeCards(playerCards);

    while(true){    //  PĘTLA GRY
        //  Gdy gracz na wejściu ma 21 to dostaje 1.5 dodatkowych pieniędzy i kończy grę bo ma natural
    if(playerCards.at(0).getFaceValue() + playerCards.at(1).getFaceValue() == 21){
        cout<<"You got a NATURAL set of cards. You win with 150% bet money.";
        return money + bettingMoney * 1.5;
    }

        //  Logika wyborów gracza
    if(playerCards.size() == 2){
        cout<<"Double down (DD), Split (SP), Surrender (SU), ";
    }
    cout<<"Hit (H) or Stay (S)? : "<<endl;
    string playerChoice;
    while(true){
        cin>>playerChoice;
        if(playerCards.size() + splitPlayerCards.size() > 2){
            if(playerChoice == "H" || playerChoice == "S"){
                break;
            }
        }else{
            if(playerChoice == "H" || playerChoice == "S"){
                break;
            }else if(playerChoice == "SP"){
                if(playerCards.at(0).getFigureType() == playerCards.at(1).getFigureType() && playerCards.at(0).getFaceValue() == playerCards.at(1).getFaceValue()){
                    if(bettingMoney * 2 > money){
                        cout<<"You don't have enough money to split"<<endl;
                    }else{
                        splitPlayerCards.push_back(playerCards.back());
                        playerCards.pop_back();
                        playerCards.push_back(playingCards.back()); playingCards.pop_back();
                        splitPlayerCards.push_back(playingCards.back()); playingCards.pop_back();
                        splitFlag = true;
                        break;
                    }
                }else{
                    cout<<"Can't split the hand because the two cards in it are not the same face and value"<<endl;
                }
            }else if(playerChoice == "DD"){
                if(bettingMoney * 2 > money){
                    cout<<"You don't have enough money to double down"<<endl;
                }else{
                    break;
                }
            }else if(playerChoice == "SU"){
                cout<<"You have decided to surrender. You will recieve back half of your betting amount";
                return money + bettingMoney * 0.5;
                break;
            }
        }
    }

        //  CHOICES
    if(splitFlag){                          //  SPLIT   ==TODO==

    }else{
        if(playerChoice == "H"){            //  HIT
            playerCards.push_back(playingCards.back());
            playingCards.pop_back();
        }else if(playerChoice == "S"){      //  STAY
            isPlayerStaying = true;
        }else if(playerChoice == "DD"){     //  DOUBLE DOWN
            bettingMoney = bettingMoney * 2;
            playerCards.push_back(playingCards.back());
            playingCards.pop_back();
            isPlayerStaying = true;
        }
    }


        //  Wypisanie posiadanych kart gracza
    cout<<"Your cards: "<<endl;             
    writeCards(playerCards);
        //  Jeśli gracz wybrał split to wypisz też karty z drugiej talii
    if(splitPlayerCards.size() != 0){
        cout<<"Your split hand cards: "<<endl;
        writeCards(splitPlayerCards);
    }

        //  --->    DEALER    <---  == TODO ==


        //  Wypisanie posiadanych kart dealera z wypisywaniem ukrytej karty
    cout<<"Dealer's cards: "<<endl;
    writeCards(dealerCards);

    }

    return money;
}

int main(int argc, char const *argv[])
{
    Blackjack(5000);
    return 0;
}
