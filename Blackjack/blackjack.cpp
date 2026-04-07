#include <iostream>
#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

        // Podstawowa klasa karty
class Card{
    private:
        int faceValue;
        string faceType;
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
        void setIsHidden(bool isHidden){
            this->isHidden = isHidden;
        }
        bool getIsHidden(){
            return this->isHidden;
        }
};

void Blackjack(){
        // Vectory wszystkich kart, dealera i gracza
    vector<Card> playingCards;
    vector<Card> dealerCards;
    vector<Card> playerCards;
        // Wypełnienie vectora wszystkimi kartami używanymi podczas gry w Blackjack i potasowanie jej
    for(int i = 0; i < 4; i++){
        for(int j = 1; j <= 13; j++){
            Card playCard(0,"Pik");     //Podstawowa karta jest ostatnią wpisywaną aby zmniejszyć ilość wykonywanych działań

            if(i == 0){playCard.setFaceType("Kier");}
            else if(i == 1){playCard.setFaceType("Karo");}
            else if(i == 2){playCard.setFaceType("Trefl");}

            if(j > 10){
                playCard.setFaceValue(10);
            }else{
                playCard.setFaceValue(j);
            }

            playingCards.push_back(playCard);
        }
    }
    random_device rd;
    mt19937 g(rd());
    shuffle(playingCards.begin(),playingCards.end(),g);

    // Rozdzielanie pierwszych dwóch kart dealera i gracza
    for(int i = 0; i < 2; i++){
        dealerCards.push_back(playingCards.back());
        playingCards.pop_back();
        playerCards.push_back(playingCards.back());
        playingCards.pop_back();
    }
}

int main(int argc, char const *argv[])
{
    Blackjack();
    return 0;
}
