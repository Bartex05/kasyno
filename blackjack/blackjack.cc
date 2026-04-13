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

int Blackjack(int money){
        //  Vectory wszystkich kart, dealera i gracza
    vector<Card> playingCards;
    vector<Card> dealerCards;
    vector<Card> playerCards;

    cout<<"Does dealer hit or stand on soft 17? (H/S): ";     //Wybór miękkiej 17
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

        //  Rozdzielanie pierwszych dwóch kart dealera i gracza
    for(int i = 0; i < 2; i++){
        dealerCards.push_back(playingCards.back());
        if(i == 1){
            dealerCards.back().setIsHidden(true);
        }
        playingCards.pop_back();
        playerCards.push_back(playingCards.back());
        playingCards.pop_back();
    }

        //  Wybór gracza


        //  Logika gry i akcji Dealera
    if(playerCards.at(0).getFaceValue() + playerCards.at(1).getFaceValue() == 21){      //Gdy gracz na wejściu ma 21 to dostaje 1.5 dodatkowych pieniędzy i kończy grę
        return money*2.5;
    }
    return money;
}

int main(int argc, char const *argv[])
{
    Blackjack(5000);
    return 0;
}
