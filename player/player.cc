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
        string getNickname(){                           
            return this->nickname;
        }
};

Player getPlayer(string nickname){                      //  Pobieranie danych gracza z pliku players.txt
    ifstream txtfile;
    txtfile.open("players.txt");

    string line;
    if(txtfile.is_open()){                              //  Jak plik został otwarty to wyszukuje nazwy gracza w pliku players.txt i tworzy obiekt DYNAMICZNY aktualnego gracza currentPlayer
        while(getline(txtfile, line)){
            int res = line.find(nickname);
            if(res != string::npos){
                if(res-1 == ',' && res+nickname.length() == ','){
                    auto pos = line.find(",");
                    int id = stoi(line.substr(0,pos));
                    line.erase(0,pos+1);
                    pos = line.find(",");
                    string nickname = line.substr(0,pos);
                    line.erase(0,pos+1);
                    int money = stoi((line));
                    Player tempPlayer(id,nickname,money);
                    return tempPlayer;
                }
            }
        }
    }else{                                              //  Jak jest błąd pliku to przekazuje błąd i wychodzi z funkcji
        cerr<<"File failed to open or doesn't exist and failed to create current player"<<endl;
    }
    txtfile.close();
}

Player createPlayer(){
    Player currentPlayer = getPlayer("TestPlayer");
    cout<<currentPlayer.getNickname();
}

int main(int argc, char const *argv[])
{

    return 0;
}
