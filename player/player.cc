#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>

using namespace std;

        //  Obiekty tej klasy będą wykorzystywane do zapisywania pieniędzy wygranych lub przegranych NA KOŃCU danej gry/stołu po czym będzie wpisywany do pliku players.txt
class Player{
    private:
        int money;                                   
        string nickname;
    public:
        Player(){                                           //  Defaults
            this->nickname = "placeholder";
            this->money = 5000;                             //  Startowa ilość kasy do ustalenia
        };
        Player(int id, string nickname, int money){         //  Konstruktor przy tworzeniu obiektu currentPlayer za pomocą danych z pliku players.txt
            this->nickname = nickname;
            this->money = money;
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
};

//  Funkcja sprawdzania istnienia gracza
bool fileExists(string playerName) {
    ifstream infile(playerName + ".txt");
    return infile.good();
}

bool isNumber(string str) {
    for (char c : str) {
        if (isdigit(c) == 0) return false;
    }
    return true;
}

//  Funkcja do tworzenia pliku nowego gracza
void createPlayer(string playerName){
    string x;
    if (!fileExists(playerName)) {
        ofstream file(playerName + ".txt", ios::out);
        cout << "Registration: " << endl;
        cout << "Please set a password: ";
        cin >> x;
        file << x << endl;
        cout << "What is your initial bet?: ";
        while (true) {
            cin >> x;
            if (isNumber(x)) {
                file << x;
                break;
            }
            else {
                cout << x << " is not a number. Enter amount: ";
            }
        }
        cout << "User created successfully. You can now log in." << endl;
        file.close();
    }
    else {
        cout << "Player already exists. Please select login or create using a different username."<<endl;
    }
}

//  Funkcja do pobierania danych gracza z pliku o nazwie gracza i przekazywania danych do currentPlayer
Player getCurrentPlayer(string playerName){
    if(!fileExists(playerName)){
        cout << "Player does not exist. Please create a new player." << endl;
    }
    else {
        ifstream file(playerName + ".txt", ios::in);
        cout << "Login: " << endl;
        Player currentPlayer;
        string playerData;
        file >> playerData;
		cout << "Enter your password: ";
        string password;
        while (true) {
            cin >> password;
            if (password == playerData) {
                file >> playerData;
                currentPlayer.setNickname(playerName);
                currentPlayer.setMoney(stoi(playerData));
                cout << "Login successful. Welcome, " << currentPlayer.getNickname() << "!" << endl;
                file.close();
                return currentPlayer;
            }
            else {
                cout << "Incorrect password. Please try again: ";
			}
        }
        file.close();
    }
}

int main(int argc, char const *argv[])
{
    Player currentPlayer;
    cout << "Would you like to log in or create a new player? (L/C)" << endl;
    string loginChoice;
    while (true) {
        cin >> loginChoice;
        if (loginChoice == "L") {
            cout << "Enter your username: ";
            string playerName;
            cin >> playerName;
            currentPlayer = getCurrentPlayer(playerName);
            break;
        }
        else if (loginChoice == "C") {
            cout << "Enter your desired username: ";
            string playerName;
            cin >> playerName;

            createPlayer(playerName);
			currentPlayer = getCurrentPlayer(playerName);
            break;
        }
        else {
            cout << "Invalid choice. Please enter L to log in or C to create a new player." << endl;
		}
    }
    return 0;
}
