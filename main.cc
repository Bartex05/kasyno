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

																	//	-----> GRACZ <-----
//  Obiekty tej klasy będą wykorzystywane do zapisywania pieniędzy wygranych lub przegranych NA KOŃCU danej gry/stołu po czym będzie wpisywany do pliku players.txt
class Player {
private:
	int money;
	string nickname, password;
public:
	Player() {                                           //  Defaults
		this->nickname = "placeholder";
		this->money = 5000;
		this->password = "placeholder";
	};
	Player(int id, string nickname, int money, string password) {         //  Konstruktor przy tworzeniu obiektu currentPlayer za pomocą danych z pliku gracza
		this->nickname = nickname;
		this->money = money;
		this->password = password;
	}

	void setNickname(string nickname) {
		this->nickname = nickname;
	}
	string getNickname() {
		return this->nickname;
	}
	void setMoney(int money) {
		this->money = money;
	}
	int getMoney() {
		return this->money;
	}
	void setPassword(string password) {
		this->password = password;
	}
	string getPassword() {
		return this->password;
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
void createPlayer(string playerName) {
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
		cout << "Player already exists. Please select login or create using a different username." << endl;
	}
}

//  Funkcja do pobierania danych gracza z pliku o nazwie gracza i przekazywania danych do currentPlayer
Player getCurrentPlayer(string playerName) {

	while(!fileExists(playerName)){
		cout << "Player does not exist. Please retry: " << endl;
		cin >> playerName;
	}

	ifstream file(playerName + ".txt", ios::in);
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
			currentPlayer.setPassword(password);
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
	return currentPlayer;
}

void overwritePlayerMoney(Player currentPlayer) {
	ofstream file(currentPlayer.getNickname() + ".txt", ios::out);
	file << currentPlayer.getPassword() << endl;
	file << currentPlayer.getMoney();
	file.close();
}
															//	-----> KONIEC GRACZA <-----

															//    ----->	POCZĄTEK BLACKJACKA	<-----
//  Podstawowa klasa karty z zmienną ukrytej karty dealer-a
class blackjackCard {
private:
	int faceValue;
	string faceType, figureType;
	bool isHidden;

public:
	blackjackCard(int faceValue, string faceType) {
		this->faceValue = faceValue;
		this->faceType = faceType;
		this->isHidden = false;
	}

	void setFaceValue(int faceValue) {
		this->faceValue = faceValue;
	}
	int getFaceValue() {
		return this->faceValue;
	}
	void setFaceType(string faceType) {
		this->faceType = faceType;
	}
	string getFaceType() {
		return this->faceType;
	}
	void setFigureType(string figureType) {
		this->figureType = figureType;
	}
	string getFigureType() {
		return this->figureType;
	}
	void setIsHidden(bool isHidden) {
		this->isHidden = isHidden;
	}
	bool getIsHidden() {
		return this->isHidden;
	}
};

//  Funkcja wypisująca przekazany wektor kart
void blackjackWriteCards(vector<blackjackCard> v) {
	/*for (auto c : v) {
		if (c.getFigureType() == "Number") {
			cout << c.getFaceValue() << " of " << c.getFaceType() << endl;
		}
		else {
			cout << c.getFigureType() << " of " << c.getFaceType() << endl;
		}
	}
	cout << endl;
	*/
	
	vector<string> lines(5, "");
	for (auto c : v) {
		if (c.getIsHidden() == true) {
			lines[0] += " -----  ";
			lines[1] += "|     | ";
			lines[2] += "|  ?  | ";
			lines[3] += "|     | ";
			lines[4] += " -----  ";
			c.setIsHidden(false);
		}
		else {
			//	Górna i dolna część karty która nie potrzebuje żadnej adaptacji ani sprawdzania wartości
			lines[0] += " -----  ";
			lines[4] += " -----  ";

			//	Symbol karty
			if (c.getFaceType() == "Hearts") {
				lines[2] += "|  H  | ";
			}
			else if (c.getFaceType() == "Diamonds") {
				lines[2] += "|  D  | ";
			}
			else if (c.getFaceType() == "Clubs") {
				lines[2] += "|  S  | ";
			}
			else if (c.getFaceType() == "Spades") {
				lines[2] += "|  C  | ";
			}

			//	Sprawdzanie czy karta jest liczbowa czy figurowa, jeśli liczbowa to wypisuje liczbę a jeśli figurowa to wypisuje znak figury
			if (c.getFigureType() == "Number") {
				//	Różna logika dla liczby dwucyfrowej i jednocyfrowej aby zachować estetykę karty
				if (c.getFaceValue() < 10) {
					lines[1] += "|" + to_string(c.getFaceValue()) + "    | ";
					lines[3] += "|    " + to_string(c.getFaceValue()) + "| ";
				}
				else {
					lines[1] += "|" + to_string(c.getFaceValue()) + "   | ";
					lines[3] += "|   " + to_string(c.getFaceValue()) + "| ";
				}
			}
			else {
				//	Każdy rodzaj figury jest jest wypisywany w jednej, pojedynczej z czterech liter więc nie trzeba rozróżniać jedno i dwucyfrowych wartości
				if (c.getFigureType() == "Ace") {
					lines[1] += "|A    | ";
					lines[3] += "|    A| ";
				}
				else if (c.getFigureType() == "Jack") {
					lines[1] += "|J    | ";
					lines[3] += "|    J| ";
				}
				else if (c.getFigureType() == "Queen") {
					lines[1] += "|Q    | ";
					lines[3] += "|    Q| ";
				}
				else if (c.getFigureType() == "King") {
					lines[1] += "|K    | ";
					lines[3] += "|    K| ";
				}
			}
		}
	}
	cout << lines[0] << endl;
	cout << lines[1] << endl;
	cout << lines[2] << endl;
	cout << lines[3] << endl;
	cout << lines[4] << endl;
	
}

//  Funkcja sprawdzająca aktualną wratość wszystkich kart i przekazywaniu czy ich łączna wartość przebija 21 (wliczając logikę miekkich asów)
int blackjackHandValue(vector<blackjackCard> v) {
	int valueCounter = 0, aceAmount = 0;
	for (auto c : v) {
		valueCounter += c.getFaceValue();
		if (c.getFigureType() == "Ace") {
			aceAmount++;
		}
	}
	if (aceAmount > 0 && valueCounter + 10 <= 21) {
		valueCounter += 10;
	}
	return valueCounter;
}

//  Główna funkcja zawierająca całką logikę grę
int Blackjack(int money) {

	system("cls");

	cout << "You have chosen Blackjack" << endl;
	cout << "At this casino there are no Hole Cards and the Dealer doesn't check the face-down card until all player actions are complete" << endl;

	int bettingMoney = 0;
	string betCheckNegative = "";

	cout << "Current bank amount: " << money << endl;
	cout << "(Note: The lowest denomination is of a white chip worth $1 so non-whole numbers are not allowed)" << endl;
	while (money < bettingMoney || bettingMoney < 100) {
		cout << "How much are you betting?: ";
		cin >> betCheckNegative;
		if (betCheckNegative[0] == '-') {	//	Wykorzystuje betCheckNegative jako string aby pominąć błędy związane z wykorzystaniem cin dla jednocześnie ujemnych i decymalnych wartości
			cout << "You can't bet negative amount of money" << endl;
			continue;
		}
		else {
			bettingMoney = stoi(betCheckNegative);
		}
		bettingMoney = floor(bettingMoney);
		if (money < bettingMoney) {
			cout << "You can't bet the money you don't have" << endl;
			continue;
		}
		if (bettingMoney < 100) {
			cout << "The minimum bet is 100" << endl;
			continue;
		}
	}

	//  Vectory wszystkich kart, dealera i gracza
	vector<blackjackCard> playingCards;
	vector<blackjackCard> dealerCards;
	vector<blackjackCard> playerCards;
	vector<blackjackCard> splitPlayerCards;

	cout << "Are you going to approach the table where the dealer Hits soft 17 or the table where he Stays at soft 17? (H/S): ";     //   Wybór miękkiej 17
	string soft17;
	while (true) {
		cin >> soft17;
		if (soft17 == "H" || soft17 == "S") {
			break;
			cout << soft17 << " is not a valid option. Please choose again: ";
		}
	}

	system("cls");

	//  Wypełnienie vectora wszystkimi kartami używanymi podczas gry w Blackjack i potasowanie jej
	for (int i = 0; i < 4; i++) {
		for (int j = 1; j <= 13; j++) {
			blackjackCard playCard(0, "Spades");     //   Podstawowa karta jest ostatnią wpisywaną aby zmniejszyć ilość wykonywanych działań

			if (i == 0) { playCard.setFaceType("Hearts"); }
			else if (i == 1) { playCard.setFaceType("Diamonds"); }
			else if (i == 2) { playCard.setFaceType("Clubs"); }

			if (j > 10) {
				playCard.setFaceValue(10);
				if (j == 11) {
					playCard.setFigureType("Jack");
				}
				else if (j == 12) {
					playCard.setFigureType("Queen");
				}
				else {
					playCard.setFigureType("King");
				}
			}
			else if (j == 1) {
				playCard.setFaceValue(1);
				playCard.setFigureType("Ace");
			}
			else {
				playCard.setFaceValue(j);
				playCard.setFigureType("Number");
			}

			playingCards.push_back(playCard);
		}
	}

	//  Losowe tasowanie kart
	random_device rd;
	mt19937 g(rd());
	shuffle(playingCards.begin(), playingCards.end(), g);

	//  --->    PIERWSZE ROZDANIE KART  <---
	for (int i = 0; i < 2; i++) {
		dealerCards.push_back(playingCards.back());
		if (i == 1) {
			dealerCards.back().setIsHidden(true);
		}
		playingCards.pop_back();
		playerCards.push_back(playingCards.back());
		playingCards.pop_back();
	}

	//  Wypisanie posiadanych kart dealera bez wypisywania ukrytej karty
	cout << "Dealer's cards: " << endl;
	blackjackWriteCards(dealerCards);

	//  Gdy gracz na wejściu ma 21 to dostaje 1.5 dodatkowych pieniędzy i kończy grę bo ma natural
	if (blackjackHandValue(playerCards) == 21) {
		cout << "Your cards: " << endl;
		blackjackWriteCards(playerCards);
		if (blackjackHandValue(dealerCards) == 21) {
			cout << "Dealer cards: " << endl;
			blackjackWriteCards(dealerCards);
			cout << "Both you and the dealer have a natural hand. Your bet is returned" << endl;
			return money;
		}
		else {
			cout << "You got a NATURAL set of cards. You win with 150% bet money.";
			return money + bettingMoney * 1.5;
		}
	}

	bool splitFlag = false;

	//  Ta zmienna istnieje po to aby wiedzieć kiedy gracz postanowił "STAY" na obu taliach 0 - brak wyboru lub "HIT" na obu, 1 - "STAY" lub "BUST" na pierwszej talii, 2 - "STAY" lub "BUST" na drugiej talii
	int splitStay = 0;

	bool bust = false;

	while (true) {        //  ---> PĘTLA GRY <---

		//  Wypisanie posiadanych kart gracza
		if (splitFlag == false) {
			cout << "Your cards: " << endl;
		}
		else {
			cout << "Your first hand cards: " << endl;
		}
		blackjackWriteCards(playerCards);

		//  Logika wykorzystująca blackjackHandValue do sprawdzenia czy gracz przegrał
		if (blackjackHandValue(playerCards) > 21) {
			//  Gdy nie ma się dzielonych kart i się przebiło 21 to się z automatu przegrywa ale reszta kodu poza pętlą gry wciąż się odbywa aby gracz mógł zobaczyć karty dealera
			if (splitFlag == false) {
				bust = true;
				break;
			}
			else {
				//  Ale jeśli ma dzielone karty "SPLIT" to druga talia może być wciąż wygrana jeśli druga co obsługuje ten if oraz pierwsza funkcja po wyjściu z pętli gry
				cout << "First hand bust!" << endl;
				if (splitStay == 2) {
					bust = true;
					break;
				}
				splitStay = 1;
			}
		}

		//  Jeśli gracz wybrał split to wypisz też karty z drugiej talii i sprawdź czy nie przebił 21 (aplikują się te same zasady co w funkcji powyżej dla pierwszej ale nie jedynej talii)
		if (splitFlag == true) {
			cout << "Your second hand cards: " << endl;
			blackjackWriteCards(splitPlayerCards);

			if (blackjackHandValue(splitPlayerCards) > 21) {
				cout << "Second hand bust!" << endl;
				if (splitStay == 1) {
					bust = true;
					break;
				}
				splitStay = 2;
			}
		}

		//  Jeśli gracz ma dwie karty to ma więcej możliwości które mu / jej wypisane
		if (playerCards.size() == 2) {
			cout << "Double down (DD), Split (SP), Surrender (SU), ";
		}

		string playerChoice, splitPlayerChoice;

		//  Jak nie ma dwóch kart to niezależnie od ilości kart wypisuje te dwie opcje
		cout << "Hit (H) or Stay (S)? : " << endl;
		if (splitStay != 1 && splitFlag == true) {
			cout << "(First hand): ";
			while (true) {
				cin >> playerChoice;
				if (playerChoice == "H") {    //  Z jakiegoś powodu którego nie potrafię rozgarnąć gdy zmienna ze stringiem jest zapisana w postaci playerChoice == "H" || playerChoice == "S" to ta funkcja nie działa więc rozpisuję ją w ten sposób
					break;
				}
				else if (playerChoice == "S") {
					break;
				}
				else {
					cout << playerChoice << " is not a valid choice" << endl;
				}
			}
		}
		//  "HIT" i "STAY" dla drugiej talii kart
		if (splitStay != 2 && splitFlag == true) {
			cout << "(Second hand): ";
			cin >> splitPlayerChoice;
			while (true) {
				cin >> splitPlayerChoice;
				if (splitPlayerChoice == "H") {   //  Ten sam problem aplikuje się tutaj
					break;
				}
				else if (splitPlayerChoice == "S") {
					break;
				}
				else {
					cout << splitPlayerChoice << " is not a valid choice" << endl;
				}
			}
		}

		//  Jak talia jest "SPLIT" to ten playerChoice jest nieaktywny
		if (splitFlag == false) {
			cin >> playerChoice;
		}

		//  --->    LOGIKA WYBORÓW  <---
		if (splitFlag == true) {  //  SPLIT LOGIC
			if (splitStay != 1) {
				if (playerChoice == "H") {
					playerCards.push_back(playingCards.back());
					playingCards.pop_back();
				}
				else if (playerChoice == "S") {
					if (splitStay == 2) {
						break;
					}
					else {
						splitStay = 1;
					}
				}
			}
			if (splitStay != 2) {
				if (splitPlayerChoice == "H") {
					splitPlayerCards.push_back(playingCards.back());
					playingCards.pop_back();
				}
				else if (splitPlayerChoice == "S") {
					if (splitStay == 1) {
						break;
					}
					else {
						splitStay = 2;
					}
				}
			}
		}
		else if (playerChoice == "H") {        //  HIT
			playerCards.push_back(playingCards.back());
			playingCards.pop_back();
			blackjackWriteCards(playerCards);
		}
		else if (playerChoice == "S") {      //  STAY
			break;
		}
		else if (playerCards.size() == 2) {
			if (playerChoice == "SU") {       //  SURRENDER
				cout << "You have decided to surrender. You will recieve back half of your betting amount";
				return money + bettingMoney * 0.5;
			}
			else if (playerChoice == "DD") {
				if (bettingMoney * 2 > money) {
					cout << "You don't have enough money to double down" << endl;
				}
				else {
					bettingMoney = bettingMoney * 2;
					playerCards.push_back(playingCards.back());
					playingCards.pop_back();
					break;
				}
			}
			else if (playerChoice == "SP") { //  SPLIT CREATION (Ten wybór zawiera tylko stworzenie drugiej talii. Nie posiada logiki dalszej gry, dlatego pierwsza opcja w logice wyborów sprawdza flagę splitowanej ręki)
				if (playerCards.at(0).getFigureType() == playerCards.at(1).getFigureType() && playerCards.at(0).getFaceValue() == playerCards.at(1).getFaceValue()) {
					splitPlayerCards.push_back(playerCards.back());
					playerCards.pop_back();
					playerCards.push_back(playingCards.back());
					playingCards.pop_back();
					splitPlayerCards.push_back(playingCards.back());
					playingCards.pop_back();
					splitFlag = true;
				}
			}
			else {
				cout << playerChoice << " is not a valid choice" << endl;
			}
		}
		else {
			cout << playerChoice << " is not a valid choice" << endl;
		}
	}
	//  --->    DEALER    <---
//  Zmienna do późniejszego przekazania wyniku gry dla gracza
	bool dealerBust = false;

	//  Cała logika dealera zgodnie z zasadami Amerykańskiego blackjacka
	if (soft17 == "H") {
		while (blackjackHandValue(dealerCards) <= 17) {
			if (blackjackHandValue(dealerCards) != 17) {
				dealerCards.push_back(playingCards.back());
				playingCards.pop_back();
			}
			else {
				int valueCounter = 0, aceAmount = 0;
				for (auto i : dealerCards) {
					valueCounter += i.getFaceValue();
					if (i.getFigureType() == "Ace") {
						aceAmount++;
					}
				}
				if (aceAmount > 0 && valueCounter + 10 == 17) {
					dealerCards.push_back(playingCards.back());
					playingCards.pop_back();
				}
			}
		}
	}
	else if (soft17 == "S") {
		while (blackjackHandValue(dealerCards) < 17) {
			dealerCards.push_back(playingCards.back());
			playingCards.pop_back();
		}

	}

	//  Ostateczne sprawdzanie czy dealer przebił 21
	if (blackjackHandValue(dealerCards) > 21) {
		dealerBust = true;
	}

	//  Wypisanie posiadanych kart dealera z wypisywaniem ukrytej karty
	cout << endl << "Dealer's cards: " << endl;
	blackjackWriteCards(dealerCards);

	cout << endl << "Your hand: " << endl;
	blackjackWriteCards(playerCards);

	cout << endl;

	if (bust == true && dealerBust == true) {
		cout << "Both you and the dealer bust however house rules state that in this situation player loses their bet." << endl;
		this_thread::sleep_for(chrono::milliseconds(3000));
		system("cls");
		return money - bettingMoney;
	}
	else if (bust == true) {
		cout << "You bust and lost your bet." << endl;
		this_thread::sleep_for(chrono::milliseconds(5000));
		system("cls");
		return money - bettingMoney;
	}
	else if (bust == false && dealerBust == true) {
		cout << "Dealer bust and you didn't. You win." << endl;
		this_thread::sleep_for(chrono::milliseconds(5000));
		system("cls");
		return money + bettingMoney;
	}
	else if (bust == false && blackjackHandValue(dealerCards) > blackjackHandValue(playerCards)) {
		cout << "Dealer has a better hand than you. You lost your bet." << endl;
		this_thread::sleep_for(chrono::milliseconds(5000));
		system("cls");
		return money - bettingMoney;
	}
	else if (bust == false && blackjackHandValue(dealerCards) < blackjackHandValue(playerCards)) {
		cout << "You have a better hand than dealer. You won." << endl;
		this_thread::sleep_for(chrono::milliseconds(5000));
		system("cls");
		return money + bettingMoney;
	}
	else if (bust == false && blackjackHandValue(dealerCards) == blackjackHandValue(playerCards)) {
		cout << "You and the dealer have the same hand value. Your bet is returned." << endl;
		this_thread::sleep_for(chrono::milliseconds(5000));
		system("cls");
	}
	return money;
}
															//	----->    KONIEC BLACKJACKA    <-----

//---ruletka-----------------------------------------------------------------------------------------

void table()
{
cout << "   |  0  |  00  |\n";
cout << "---------------------------------\n";
cout << "| 1R  | 2B  | 3R  |             |\n";
cout << "-------------------             |\n";
cout << "| 4B  | 5R  | 6B  |    dozen    |\n";
cout << "-------------------             |\n";
cout << "| 7R  | 8B  | 9R  |      1      |\n";
cout << "-------------------             |\n";
cout << "| 10B | 11R | 12B |             |\n";
cout << "---------------------------------\n";
cout << "| 13R | 14B | 15R |             |\n";
cout << "-------------------             |\n";
cout << "| 16B | 17R | 18B |    dozen    |\n";
cout << "-------------------             |\n";
cout << "| 19R | 20B | 21R |      2      |\n";
cout << "-------------------             |\n";
cout << "| 22B | 23R | 24B |             |\n";
cout << "---------------------------------\n";
cout << "| 25R | 26B | 27R |             |\n";
cout << "-------------------             |\n";
cout << "| 28B | 29R | 30B |    dozen    |\n";
cout << "-------------------             |\n";
cout << "| 31R | 32B | 33R |      3      |\n";
cout << "-------------------             |\n";
cout << "| 34B | 35R | 36B |             |\n";
cout << "---------------------------------\n";
cout << "|     |     |     |      |      |\n";
cout << "|     |     |     |  R   |   B  |\n";
cout << "| 1st | 2st | 3st |      |      |\n";
cout << "---------------------------------\n";
}



void bets()
{
cout<<"Possible bets: "<<endl;
cout<<"----------------------------\n";
cout<<"|  one number   | (1) | x36 |\n";
cout<<"----------------------------\n";
cout<<"| two numbers   | (2) | x18 |\n";
cout<<"----------------------------\n";
cout<<"| three numbers | (3) | x12 |\n";
cout<<"----------------------------\n";
cout<<"|     dozen     | (d) |  x3 |\n";
cout<<"----------------------------\n";
cout<<"|    column     | (k) |  x3 |\n";
cout<<"----------------------------\n";
cout<<"|     color     | (c) |  x2 |\n";
cout<<"----------------------------\n";
}



void zasady()
{
	bets();
	cout<<"\n"<<"\n";
	this_thread::sleep_for(chrono::milliseconds(3000));

	table();
	cout<<"\n";
	this_thread::sleep_for(chrono::milliseconds(3000));
}

void seeyou()
{
	system("clear");
	cout<<"See you";

	cout<<"\n"<<"\n";
	getchar();
	system("clear");
}

int ruletka(int saldo)
{
//------------------------------------------------------------//

	//odp tak/nie
	string odp;

	//pieniadze
	//int saldo=0;
	int suma=0;
	int mnoznik;

	//losowanie
	int los;
	string numer;
	string kolor;
	string tuzin;
	string kolumna;

	//zaklady
	string typ;
	string wartosc1;
	int x1;
	string wartosc2;
	int x2;
	string wartosc3;
	int x3;
	string wartosck;
	string wartosct;
	string wartoscc;
	int win;

//------------------------------------------------------------

	system("clear");
	this_thread::sleep_for(chrono::milliseconds(500));

//------------------------------------------------------------

	while(odp!="y" && odp!="n")
	{
		system("clear");

		cout<<"Welcome, do you want to play roulette? [y/n]: ";
		getline(cin, odp);
		//cout<<"\n";
		this_thread::sleep_for(chrono::milliseconds(1000));

		if(odp!="y" && odp!="n")
		{
			cout<<"There is no such answer"<<endl;
			this_thread::sleep_for(chrono::milliseconds(3000));
		}
	}

//------------------------------------------------------------

	/*if(odp=="y")
	{
		while(saldo==0 || saldo<0)
		{
			system("clear");

			cout<<"Enter your starting balance: ";
			cin>>saldo;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			//cout<<"\n";
			this_thread::sleep_for(chrono::milliseconds(1000));

			if(saldo<0 || saldo==0)
			{
				cout<<"Balance cannot be less than or equal to zero"<<endl;
				this_thread::sleep_for(chrono::milliseconds(3000));
			}
		}
	} */

//------------------------------------------------------------

	while(odp=="y")
	{
		system("clear");

		//czyszczenie wartosci
		typ="0";
		numer="x";
		tuzin="x";
		kolumna="x";
		kolor="x";
		x1=-1;
		x2=-1;
		x3=-1;
		wartosc1="";
		wartosc2="";
		wartosc3="";
		wartosck="";
		wartosct="";
		wartoscc="";
		win=-1;
		suma=-1;

//------------------------------------------------------------

		//losowanie
		srand(time(0));
		los = rand()%38;

		//los=0;

		//wartosc
		if(los==37)
		{
			numer="00";
		}
		else
		{
			numer = to_string(los);
		}

		//kolor
		if((los+2)%2==1 && los!=37)
		{
			kolor="red";
		}
		if((los+2)%2==0)
		{
			kolor="black";
		}
		if(los==0 || los==37)
		{
			kolor="green";
		}

		//tuzin
		if(los>=1 && los<=12)
		{
			tuzin="1";
		}
		if(los>=13 && los<=24)
		{
			tuzin="2";
		}
		if(los>=25 && los<=36)
		{
			tuzin="3";
		}

		//kolumna
		if((los+2)%3==0 && los!=0 && los!=37)
		{
			kolumna="1";
		}
		if((los+1)%3==0 && los!=0 && los!=37)
		{
			kolumna="2";
		}
		if(los%3==0 && los!=0 && los!=37)
		{
			kolumna="3";
		}

//------------------------------------------------------------

		while(typ!="1" && typ!="2" && typ!="3" && typ!="d" &&  typ!="k" && typ!="c")
		{
			system("clear");

			zasady();

			cout<<"What type of bet do you want to place (1/2/3/d/k/c): ";
			getline(cin, typ);
			//cout<<"\n";
			this_thread::sleep_for(chrono::milliseconds(1000));

			if(typ!="1" && typ!="2" && typ!="3" && typ!="d" &&  typ!="k" && typ!="c")
			{
                        	cout<<"There is no such answer"<<endl;
                        	this_thread::sleep_for(chrono::milliseconds(3000));
                        	//system("clear");
			}
			cout<<"\n";
		}

//--------------------------------------------------------

		if(typ=="1")
		{
			while(wartosc1!="00" && (x1<0 || x1>36))
			{
				x1=-1;
			cout<<"Enter the number you chose (0-36 or 00): ";
			getline(cin, wartosc1);
			//cout<<"\n";
			if(wartosc1!="00")
			{
				x1=stoi(wartosc1);
			}
			this_thread::sleep_for(chrono::milliseconds(1000));

			if(wartosc1!="00" && (x1<0 || x1>36))
			{
                        		cout<<"There is no such answer";
                        		//system("clear");
				}
			}

			while(suma<0 || suma>saldo)
			{
				system("clear");

			cout<<"Your balance is: "<<saldo;
			cout<<"\n";
			cout<<"\n";
			this_thread::sleep_for(chrono::milliseconds(2000));

			cout<<"Enter the amount you want to bet: ";
				cin>>suma;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			 	//cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(1000));

				if(suma>saldo)
				{
				cout<<"The amount cannot be greater than the balance";
					cout<<"\n";
					this_thread::sleep_for(chrono::milliseconds(3000));
				}
				if(suma<0)
				{
				cout<<"The amount cannot be less than 0";
					cout<<"\n";
					this_thread::sleep_for(chrono::milliseconds(3000));
				}
				cout<<"\n";
			}

			saldo=saldo-suma;

			if(wartosc1==numer)
			{
				win=1;
				suma=suma*36;
				saldo=saldo+suma;
			}
			else
			{
				win=0;
			}
		}

//---------------------------------------------------------

		if(typ=="2")
		{
			while(wartosc1!="00" && (x1<0 || x1>36))
			{
				x1=-1;
				cout<<"Enter the first number you chose (0-36 or 00): ";
				getline(cin, wartosc1);
				//cout<<"\n";
				if(wartosc1!="00")
				{
					x1=stoi(wartosc1);
				}
				this_thread::sleep_for(chrono::milliseconds(1000));

				if(wartosc1!="00" && (x1<0 || x1>36))
				{
                    cout<<"There is no such answer";
					cout<<"\n";
					cout<<"\n";
                    this_thread::sleep_for(chrono::milliseconds(3000));
                    //system("clear");
				}
			}

			while(wartosc2!="00" && (x2<0 || x2>36) || wartosc2==wartosc1)
			{
				x2=-1;
			cout<<"Enter the second number you chose (0-36 or 00): ";
				getline(cin, wartosc2);
				//cout<<"\n";
				if(wartosc2!="00")
				{
					x2=stoi(wartosc2);
				}
				this_thread::sleep_for(chrono::milliseconds(1000));

				if(wartosc2!="00" && (x2<0 || x2>36))
				{
                    cout<<"There is no such answer";
					cout<<"\n";
					cout<<"\n";
                    this_thread::sleep_for(chrono::milliseconds(3000));
                    //system("clear");
				}

				if(wartosc2==wartosc1)
				{
                    cout<<"Second number cannot be the same as the first";
					cout<<"\n";
					cout<<"\n";
                    this_thread::sleep_for(chrono::milliseconds(3000));
                    //system("clear");
				}
			}

			while(suma<0 || suma>saldo)
			{
				system("clear");

			cout<<"Your balance is: "<<saldo;
			cout<<"\n";
			cout<<"\n";
			this_thread::sleep_for(chrono::milliseconds(2000));

			cout<<"Enter the amount you want to bet: ";
				cin>>suma;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			 	//cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(1000));

				if(suma>saldo)
				{
					cout<<"The amount cannot be greater than the balance";
						cout<<"\n";
					this_thread::sleep_for(chrono::milliseconds(3000));
				}
				if(suma<0)
				{
					cout<<"The amount cannot be less than 0";
					cout<<"\n";
					this_thread::sleep_for(chrono::milliseconds(3000));
				}
				cout<<"\n";
			}

			saldo=saldo-suma;

			if(wartosc1==numer || wartosc2==numer)
			{
				win=1;
				suma=suma*18;
				saldo=saldo+suma;
			}
			else
			{
				win=0;
			}
		}

//----------------------------------------------------------

		if(typ=="3")
		{
			while(wartosc1!="00" && (x1<0 || x1>36))
			{
				x1=-1;
			cout<<"Enter the first number you chose (0-36 or 00): ";
				getline(cin, wartosc1);
				//cout<<"\n";
				if(wartosc1!="00")
				{
					x1=stoi(wartosc1);
				}
				this_thread::sleep_for(chrono::milliseconds(1000));

				if(wartosc1!="00" && (x1<0 || x1>36))
				{
                        		cout<<"There is no such answer";
					cout<<"\n";
					cout<<"\n";
                        		this_thread::sleep_for(chrono::milliseconds(3000));
                        		//system("clear");
				}
			}

			while(wartosc2!="00" && (x2<0 || x2>36) || wartosc2==wartosc1)
			{
				x2=-1;
				cout<<"Enter the second number you chose (0-36 or 00): ";
				getline(cin, wartosc2);
				//cout<<"\n";
				if(wartosc2!="00")
				{
					x2=stoi(wartosc2);
				}
				this_thread::sleep_for(chrono::milliseconds(1000));

				if(wartosc2!="00" && (x2<0 || x2>36))
				{
                        		cout<<"There is no such answer";
					cout<<"\n";
					cout<<"\n";
                        		this_thread::sleep_for(chrono::milliseconds(3000));
                        		//system("clear");
				}

				if(wartosc2==wartosc1)
				{
                        		cout<<"Second number cannot be the same as the first";
					cout<<"\n";
					cout<<"\n";
                        		this_thread::sleep_for(chrono::milliseconds(3000));
                        		//system("clear");
				}
			}

			while(wartosc3!="00" && (x3<0 || x3>36) || wartosc3==wartosc1 || wartosc3==wartosc2)
			{
				x3=-1;
			cout<<"Enter the third number you chose (0-36 or 00): ";
				getline(cin, wartosc3);
				//cout<<"\n";
				if(wartosc3!="00")
				{
					x3=stoi(wartosc3);
				}
				this_thread::sleep_for(chrono::milliseconds(1000));

				if(wartosc3!="00" && (x3<0 || x3>36))
				{
                        		cout<<"There is no such answer";
					cout<<"\n";
					cout<<"\n";
                        		this_thread::sleep_for(chrono::milliseconds(3000));
                        		//system("clear");
				}

				if(wartosc3==wartosc1)
				{
                        		cout<<"Third number cannot be the same as the first";
					cout<<"\n";
					cout<<"\n";
                        		this_thread::sleep_for(chrono::milliseconds(3000));
                        		//system("clear");
				}

				if(wartosc3==wartosc2)
				{
                        		cout<<"Third number cannot be the same as the second";
					cout<<"\n";
					cout<<"\n";
                        		this_thread::sleep_for(chrono::milliseconds(3000));
                        		//system("clear");
				}
			}

			while(suma<0 || suma>saldo)
			{
				system("clear");

				cout<<"Your balance is: "<<saldo;
				cout<<"\n";
				cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(2000));

				cout<<"Enter the amount you want to bet: ";
				cin>>suma;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			 	//cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(1000));

				if(suma>saldo)
				{
					cout<<"The amount cannot be greater than the balance";
					cout<<"\n";
					this_thread::sleep_for(chrono::milliseconds(3000));
				}
				if(suma<0)
				{
					cout<<"The amount cannot be less than 0";
					cout<<"\n";
					this_thread::sleep_for(chrono::milliseconds(3000));
				}
				cout<<"\n";
			}

			saldo=saldo-suma;

			if(wartosc1==numer || wartosc2==numer || wartosc3==numer)
			{
				win=1;
				suma=suma*12;
				saldo=saldo+suma;
			}
			else
			{
				win=0;
			}
		}

//-----------------------------------------------------------

		if(typ=="d")
		{
			while(wartosct!="1" && wartosct!="2" && wartosct!="3")
			{
			cout<<"Enter the dozen you chose (1/2/3): ";
			getline(cin, wartosct);
			//cout<<"\n";
			this_thread::sleep_for(chrono::milliseconds(1000));

			if(wartosct!="1" && wartosct!="2" && wartosct!="3")
			{
                        		cout<<"There is no such answer";
					cout<<"\n";
					cout<<"\n";
                        		this_thread::sleep_for(chrono::milliseconds(3000));
                        		//system("clear");
				}
			}

			while(suma<0 || suma>saldo)
			{
				system("clear");

				cout<<"Your balance is: "<<saldo;
				cout<<"\n";
				cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(2000));

				cout<<"Enter the amount you want to bet: ";
				cin>>suma;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			 	//cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(1000));

				if(suma>saldo)
				{
					cout<<"The amount cannot be greater than the balance";
					cout<<"\n";
					this_thread::sleep_for(chrono::milliseconds(3000));
				}
				if(suma<0)
				{
					cout<<"The amount cannot be less than 0";
					cout<<"\n";
					this_thread::sleep_for(chrono::milliseconds(3000));
				}
				cout<<"\n";
			}

			saldo=saldo-suma;

			if(wartosct==tuzin)
			{
				win=1;
				suma=suma*3;
				saldo=saldo+suma;
			}
			else
			{
				win=0;
			}
		}

//-----------------------------------------------------------

		if(typ=="k")
		{
			while(wartosck!="1" && wartosck!="2" && wartosck!="3")
			{
			cout<<"Enter the column you chose (1/2/3): ";
			getline(cin, wartosck);
			//cout<<"\n";
			this_thread::sleep_for(chrono::milliseconds(1000));

			if(wartosck!="1" && wartosck!="2" && wartosck!="3")
			{
                        		cout<<"There is no such answer";
					cout<<"\n";
					cout<<"\n";
                        		this_thread::sleep_for(chrono::milliseconds(3000));
                        		//system("clear");
				}
			}

			while(suma<0 || suma>saldo)
			{
				system("clear");

				cout<<"Your balance is: "<<saldo;
				cout<<"\n";
				cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(2000));

				cout<<"Enter the amount you want to bet: ";
				cin>>suma;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			 	//cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(1000));

				if(suma>saldo)
				{
					cout<<"The amount cannot be greater than the balance";
					cout<<"\n";
					this_thread::sleep_for(chrono::milliseconds(3000));
				}
				if(suma<0)
				{
					cout<<"The amount cannot be less than 0";
					cout<<"\n";
					this_thread::sleep_for(chrono::milliseconds(3000));
				}
				cout<<"\n";
			}

			saldo=saldo-suma;

			if(wartosck==kolumna)
			{
				win=1;
				suma=suma*3;
				saldo=saldo+suma;
			}
			else
			{
				win=0;
			}
		}

//------------------------------------------------------------

		if(typ=="c")
		{
			while(wartoscc!="black" && wartoscc!="red" && wartoscc!="green")
			{
			cout<<"Enter the color you chose (black/red/green): ";
			getline(cin, wartoscc);
			//cout<<"\n";
			this_thread::sleep_for(chrono::milliseconds(1000));

			if(wartoscc!="black" && wartoscc!="red" && wartoscc!="green")
			{
                        		cout<<"There is no such answer";
					cout<<"\n";
					cout<<"\n";
                        		this_thread::sleep_for(chrono::milliseconds(3000));
                        		//system("clear");
				}
			}

			while(suma<0 || suma>saldo)
			{
				system("clear");

				cout<<"Your balance is: "<<saldo;
				cout<<"\n";
				cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(2000));

				cout<<"Enter the amount you want to bet: ";
				cin>>suma;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			 	//cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(1000));

				if(suma>saldo)
				{
					cout<<"The amount cannot be greater than the balance";
					cout<<"\n";
					this_thread::sleep_for(chrono::milliseconds(3000));
				}
				if(suma<0)
				{
					cout<<"The amount cannot be less than 0";
					cout<<"\n";
					this_thread::sleep_for(chrono::milliseconds(3000));
				}
				cout<<"\n";
			}

			saldo=saldo-suma;

			if(wartoscc!="green" && wartoscc==kolor)
			{
				win=1;
				suma=suma*2;
				saldo=saldo+suma;
			}
			if(wartoscc=="green" && wartoscc==kolor)
			{
				win=1;
				suma=suma*18;
				saldo=saldo+suma;
			}
			if(wartoscc!=kolor)
			{
				win=0;
			}
		}

//------------------------------------------------------------

		cout<<"The roulette drew: "<<endl;
		this_thread::sleep_for(chrono::milliseconds(500));
		cout<<"Number: "<<numer<<endl;
		this_thread::sleep_for(chrono::milliseconds(500));
		cout<<"Color: "<<kolor<<endl;
		this_thread::sleep_for(chrono::milliseconds(500));
		cout<<"Dozen: "<<tuzin<<endl;
		this_thread::sleep_for(chrono::milliseconds(500));
		cout<<"Column: "<<kolumna<<endl;
		this_thread::sleep_for(chrono::milliseconds(500));

		cout<<"\n";
		this_thread::sleep_for(chrono::milliseconds(3000));

//------------------------------------------------------------

		if(win==1)
		{
			cout<<"You won! "<<flush;
			//this_thread::sleep_for(chrono::milliseconds(1000));

			//cout<<"Congratulations! "<<flush;
			cout<<"\n";
			this_thread::sleep_for(chrono::milliseconds(2000));

			cout<<"Your balance is now: "<<flush;
			this_thread::sleep_for(chrono::milliseconds(500));
			cout<<saldo<<flush;

			this_thread::sleep_for(chrono::milliseconds(3000));
		}

		if(win==0)
		{
			cout<<"You lost :( "<<flush;
			//this_thread::sleep_for(chrono::milliseconds(1000));

			//cout<<"Maybe next time..."<<flush;
			cout<<"\n"<<flush;
			this_thread::sleep_for(chrono::milliseconds(2000));


			cout<<"Your balance is now: "<<flush;
			this_thread::sleep_for(chrono::milliseconds(500));
			cout<<saldo<<flush;
			this_thread::sleep_for(chrono::milliseconds(3000));
		}
		cout<<"\n";

//------------------------------------------------------------

		if(saldo==0 || saldo<0)
		{
			system("clear");
			cout<<"BANKRUPTCY"<<endl;
			cout<<"\n";

			getchar();
			//this_thread::sleep_for(chrono::milliseconds(5000));

			break;
		}

//------------------------------------------------------------

		this_thread::sleep_for(chrono::milliseconds(2000));
		cout<<"\n";

		do{
			//system("clear")
			cout<<"Do you want to continue playing? [y/n]: "<<flush;
			getline(cin, odp);
			//cout<<"\n";

			if(odp!="y" && odp!="n")
			{
				cout<<"There is no such answer"<<endl;
				cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(3000));
			}
		}while(odp!="y" && odp!="n");

//------------------------------------------------------------

	}
	seeyou();

//------------------------------------------------------------//

	return saldo;

}

//---ruletka end----------------------------------------------------------------------------------

int main()
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
	cout << "Current bank: " << currentPlayer.getMoney() << endl;
	string depositChoice = "";
	while (true) {
		if (currentPlayer.getMoney() < 100) {
			cout << "You don't have enough money to play. The minimum is $100." << endl;
		}
		cout << "Would you like to deposit money to your account? (Y/N): ";
		cin >> depositChoice;
		if (depositChoice == "Y") {
			cout << "How much would you like to deposit?" << endl;
			int depositAmount = 0;
			while (depositAmount <= 0) {
				cin >> depositAmount;
				if (depositAmount <= 0) {
					cout << "Deposit amount must be greater than 0. Please enter a valid amount: " << endl;
				}
				else {
					currentPlayer.setMoney(currentPlayer.getMoney() + depositAmount);
					overwritePlayerMoney(currentPlayer);
					cout << "Transaction successful. Your new bank: " << currentPlayer.getMoney() << endl;
					break;
				}
			}
		}
		else if (depositChoice == "N") {
			if (currentPlayer.getMoney() < 100) {
				cout << "You don't have enough money to play and are not willing to deposit. You exit the casino." << endl;
				return 0;
			}
			else {
				break;
			}
		}
	}
	
	//	Ta zmienna jest umieszczona w tym nadrzędnym zakresie dla efektywnego powtarzania gry
    int gameChoice = -1;

	//  Ten gracz jest jeszcze do zmiany dlatego że będzie to w przyszłości tworzone z pliku wprowadzanego do vectora wszystkich istniejących gracze			<--- TODO
    Player player;

    while(true){
        cout<<"===>        WYBOR GRY        <==="<<endl;
    
        cout<<"1. Blackjack"<<endl;         
        cout<<"2. Roulette"<<endl;           //Do dopisania więcej

        cout<<"0. Quit"<<endl;

        cout<<"Please select a number to choose a game: ";

		//	Ten while służy do sprawdzania czy gra o takim numerze istnieje. Jest to określane przez zakres określony przy pierwszej strzałce w komentarzu poniżej.
        while(true){   
			if (gameChoice == 0) {
				cout << "We hope to see you again!" << endl;
				return 0;
			}
			if (gameChoice >= 1 && gameChoice <= 2	/*Tutaj dopisywane sprawdzanie liczby gry w postaci gameChoice >= 1 && gameChoice <= *Aktualna liczba możliwych gier* */) {         // <----------------------------
				break;
			}
			else if(gameChoice != -1){
				cout << "There is no game with that number." << endl;
			}

			//	Wartość wyboru gry jest pobierana jako ostatnia aby umożliwić czystą funkcjonalność ponownej gry dla funkcji bez wbudowanej logiki ponownej gry.				<----- WAŻNE !!! (Należy gameChoice ustawiać na -1 w logice ponownej gry gdy gracz nie chce jej powtarzać)
			cin >> gameChoice;
        }

		//	String dla wszystkich gier które nie mają wbudowanej funkcjonalności powtarzania gry.
		string playAgain;

		//	Ten switch służy do wyboru gry. Jak dodaje się grę to trzeba ją tutaj włożyć wraz z potencjalną logiką powtarzania gry. Jeśli ta logika już jest w funkcji gry to nie trzeba. Wystarczy wywołać.
        switch(gameChoice){
            case 1:
                currentPlayer.setMoney(Blackjack(currentPlayer.getMoney()));
				overwritePlayerMoney(currentPlayer);
				cout << "Would you like to play again?: (Y/N)" << endl;
				while (true) {
					cin >> playAgain;
					if(playAgain == "Y") {
						break;
					}
					else if (playAgain == "N") {
						gameChoice = -1;
						break;
					}
					else {
						cout << playAgain << " is not a valid choice. Please choose again: " << endl;
					}
				}
				break;
            case 2:
                currentPlayer.setMoney(ruletka(currentPlayer.getMoney()));
				overwritePlayerMoney(currentPlayer);
				gameChoice = -1;
                break;
            default:                        //Ze względu na zabezpieczony brak nieodpowiedniego case, default jest używany do powtarzania gry poprzez niezmianianą zmienną 'gameChoice' lub wprost niepoprawnie wprowadzoną wartość.
                cout<<"No game has been chosen or the input is invalid."<<endl;
                return 0;
        }
    }
}