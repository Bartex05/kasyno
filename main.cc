#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <random>
#include <map>
#include <algorithm>

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
		cout << ">> Registration: " << endl;
		cout << "?> Please set a password: ";
		cin >> x;
		file << x << endl;
		cout << "?> What is your initial bet?: ";
		while (true) {
			cin >> x;
			if (isNumber(x)) {
				file << x;
				break;
			}
			else {
				cout << "!> " << x << " is not a number. Enter amount: ";
			}
		}
		cout << ">> User created successfully. You can log in." << endl;
		file.close();
	}
}

//  Funkcja do pobierania danych gracza z pliku o nazwie gracza i przekazywania danych do currentPlayer
Player getCurrentPlayer(string playerName) {
	ifstream file(playerName + ".txt", ios::in);
	Player currentPlayer;
	string playerData;
	file >> playerData;
	cout << "?> Enter your password: ";
	string password;
	while (true) {
		cin >> password;
		if (password == playerData) {
			file >> playerData;
			currentPlayer.setNickname(playerName);
			currentPlayer.setPassword(password);
			currentPlayer.setMoney(stoi(playerData));
			cout << ">> Login successful. Welcome, " << currentPlayer.getNickname() << "!" << endl  << endl;
			file.close();
			return currentPlayer;
		}
		else {
			cout << "!> Incorrect password. Please try again: ";
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

// SLOTY START -----------------------------------------------------------------------------------

// slotSymbols
map<char, int> slotSymbols = {
    {'X', 100},    // Diament
    {'7', 50},     // Jackpot
    {'G', 25},     // Gwiazda (Wild)
    {'D', 15},     // Dzwonek
    {'W', 10},     // Winogrono 
    {'P', 5},      // Pomarańcza
    {'L', 2},      // Lemon 
    {'C', 1}       // Cherry
};

bool slotIsCompatible(char a, char b) {
    
    if (a == 'G' || b == 'G') return true;
    
    return a == b;
}

char slotGetWinningSymbol(char a, char b, char c) {
    
    if (a != 'G') return a;
    if (b != 'G') return b;
    if (c != 'G') return c;
    
    return 'G';
}

void slotDisplayGrid(char grid[3][3])
{
    cout << endl << " --- SLOTS ---" << endl;
    
    for (int i = 0; i < 3; i++) 
    {
        cout << "  ";
        
        for (int j = 0; j < 3; j++) 
        {
            cout << "[" << grid[i][j] << "] ";
        }
        
        cout << endl;
    }
    cout << " -------------" << endl << endl;
}

int slotCheckLine(char a, char b, char c, int bet) {
    
    if (slotIsCompatible(a, b) && slotIsCompatible(b, c) && slotIsCompatible(a, c)) 
    {
        char winSymbol = slotGetWinningSymbol(a, b, c);
        
        int winAmount = bet * slotSymbols[winSymbol];
        
        cout << ">> WIN! Combination: " << a << b << c << " (Value: " << winSymbol << ")" << endl;
        
        return winAmount;
    }
    else
    {
        return 0;
    }
    
}

int slot(int money) 
{
    srand(time(0));
    char grid[3][3];
    
    char slotSymbols[3][10] = 
    {
        {'7', 'L', 'X', 'D', 'P', 'W', 'L', 'G', 'C', 'D'},
        {'7', 'P', 'L', 'D', 'X', 'G', 'W', 'L', 'P', 'C'},
        {'C', 'X', 'W', 'L', 'D', 'P', 'C', 'L', 'W', 'D'},
    };
    
	int balance = money;
    int bet;

	cout << "===> WELCOME TO SLOTS <===" << endl << endl;

    cout << ">> Starting Balance: " << balance << endl;

    while (balance > 0) 
    {
        cout << "?> Enter bet amount (0 to quit): ";

		while (!(cin >> bet)) { 
			cout << "?> Enter bet amount (0 to quit): ";
			cin.clear(); 
			cin.ignore(10000, '\n');
		}
		
        if (bet <= 0) break;
        
        if (bet > balance) 
        {
            cout << "!> Not enough money!" << endl;
            continue;
        }

        balance -= bet;

        for (int i = 0; i < 3; i++) 
        {
            int randModulo = rand() % 10;
            
            grid[1][i] = slotSymbols[i][randModulo];
            
            switch(randModulo) 
            {
                case 0:
                grid[0][i] = slotSymbols[i][9];
                grid[2][i] = slotSymbols[i][1];
                break;
                
                case 9:
                grid[0][i] = slotSymbols[i][8];
                grid[2][i] = slotSymbols[i][0];
                break;
                
                default:
                grid[0][i] = slotSymbols[i][randModulo - 1];
                grid[2][i] = slotSymbols[i][randModulo + 1];
                break;
            }
        }
        
        slotDisplayGrid(grid);
        int win = 0;
        
        // Sprawdzanie rzędów
        win += slotCheckLine(grid[0][0], grid[0][1], grid[0][2], bet);
        win += slotCheckLine(grid[1][0], grid[1][1], grid[1][2], bet);
        win += slotCheckLine(grid[2][0], grid[2][1], grid[2][2], bet);
        
        // Sprawdzanie skosów
        win += slotCheckLine(grid[0][0], grid[1][1], grid[2][2], bet);
        win += slotCheckLine(grid[2][0], grid[1][1], grid[0][2], bet);
        
        if (win > 0) 
        {
            cout << ">> You won: " << win << "!" << endl;
            balance += win;
        } 
        else 
        {
            cout << ">> No luck this time." << endl;
        }

        cout << ">> Current Balance: " << balance << endl;
    }

    cout << "!> Game over! You have: " << balance << endl;
    
    return balance;
}

// SLOTY END -------------------------------------------------------------------------------------

// SABACC START ----------------------------------------------------------------------------------

// ZMIENNE GLOBALNE

int sabaccHandPlayer[5];
int sabaccHandDealer[5];

int sabaccSizePlayer = 0; 
int sabaccSizeDealer = 0;

int sabaccCreditsPlayer = 150; 
int sabaccCreditsDealer = 150; 

int sabaccPotSabaccGame = 0; 
int sabaccPotSabacc = 0; 
int sabaccCurrentBet = 0;

int sabaccCreditsStart = 0;

// STAŁE PROCENTOWE

const double sabaccPercentageEnter = 0.10;    
const double sabaccPercentageDraw = 0.03;    
const double sabaccPercentageSwap = 0.06;    
const double sabaccPercentageSabacc = 0.05;
const double  sabaccPercentageRaise = 0.05;

// FUNKCJE POMOCNICZE

int sabaccCalculateScore(int hand[], int size) {
    
    int total = 0;
    
    for (int i = 0; i < size; i++) total += hand[i];
    
    return total;
}

int sabaccAbsolute(int n) {
    
    return (n < 0) ? -n : n;
}

int sabaccDrawCard() {
    
    int r = rand() % 62;
    
    // Sylopy (zera)
    if (r < 2) return 0; 

    // Zielone 1-10
    if (r < 32) return (rand() % 10) + 1; 
    
    // Czerwone 1-10
    return -((rand() % 10) + 1); 
}

void sabaccPrintCard(int hand[], int size, bool hide) {
    
    string lines[5] = {"", "", "", "", ""};

    for (int i = 0; i < size; i++) {
        
        int valueInt = hand[i];
        
        string symbol;
        string valueString;

        // Symbol
        if (valueInt == 0) {
            
            symbol = "S";
        }
        else if (valueInt > 0) { 
            
            symbol = "G";
        }
        else if (valueInt < 0) {
            
            symbol = "R";
        }

        if (hide) {
            
            valueString = "?";
            symbol = "?";
        } 
        else {
            
            valueString = to_string(sabaccAbsolute(valueInt));
        }

        // Padding
        string valueLeft = valueString;
        string valueRight = valueString;
        
        if (valueString.length() == 1) {
            
            valueLeft = valueString + " "; // Np. "5 "
            valueRight = " " + valueString; // Np. " 5"
        }

		//lines[0] += "\u250c\u2500\u2500\u2500\u2500\u2500\u2510 ";
        //lines[1] += "\u2502" + valueLeft + "   \u2502 ";
        //lines[2] += "\u2502  " + symbol + "  \u2502 ";
        //lines[3] += "\u2502   " + valueRight + "\u2502 ";
        //lines[4] += "\u2514\u2500\u2500\u2500\u2500\u2500\u2518 ";

        lines[0] += ".-----. ";
        lines[1] += "|" + valueLeft + "   | ";
        lines[2] += "|  " + symbol + "  | ";
        lines[3] += "|   " + valueRight + "| ";
        lines[4] += "'-----' ";
    }

    for (int i = 0; i < 5; i++) {
        
        cout << lines[i] << endl;
    }
}

int sabaccGetCost(double sabaccPercentage) {
    return (int)ceil(sabaccCreditsStart * sabaccPercentage);
}

int sabaccValidateInput(string prompt, int minValue, int maxValue) {
    
    string inputString;
    int inputInt;
    
    while (true) {
        
        cout << prompt;
        cin >> inputString;
        
        try {
            
            inputInt = stoi(inputString);
            inputInt = floor(inputInt);
            
            if (minValue == 0 && maxValue == 0){
                
                if (inputInt > 0) {
                    return inputInt;
                }
                else {
                    
                    cout << "!> ERROR: Input positive number of money!" << endl;
                }
                
            } else {
                
                if (inputInt >= minValue && inputInt <= maxValue) {
                
                return inputInt;
                
                }
                else {
                    cout << "!> ERROR: Input number from " << minValue << " to " << maxValue << "!" << endl;
                }
            }
        } 
        catch (...) {
            
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "!> ERROR: Input a valid number!" << endl;
        }
    }
}

// LOGIKA DEALERA

void sabaccDealerAction() {
    
    if (sabaccSizeDealer == 0) return;

    int currentScore = sabaccCalculateScore(sabaccHandDealer, sabaccSizeDealer);
    int currentAbs = sabaccAbsolute(currentScore);
    
    int swapCost = sabaccGetCost(sabaccPercentageSwap);
    int drawCost = sabaccGetCost(sabaccPercentageDraw);

    // Wymiana
    
    if (sabaccCreditsDealer >= swapCost && currentAbs > 5) {
        
        int bestIndex = -1;
        int bestAbs = currentAbs;

        for (int i = 0; i < sabaccSizeDealer; i++) {
            
            int scoreWithoutCard = currentScore - sabaccHandDealer[i];
            
            if (sabaccAbsolute(scoreWithoutCard) < bestAbs) {
                
                bestAbs = sabaccAbsolute(scoreWithoutCard);
                bestIndex = i;
            }
        }

        if (bestIndex != -1) {
            
            sabaccHandDealer[bestIndex] = sabaccDrawCard();
            
            sabaccCreditsDealer -= swapCost;
            sabaccPotSabaccGame += swapCost;
            
            cout << ">> Dealer paid $" << swapCost << " to swap a card." << endl;
            return; 
        }
    }

    // 2. Logika dobierania (Koszt 5)
    
    if (sabaccCreditsDealer >= drawCost && currentAbs > 7) {
        
        sabaccHandDealer[sabaccSizeDealer++] = sabaccDrawCard();
        
        sabaccCreditsDealer -= drawCost;
        sabaccPotSabaccGame += drawCost;
        
       cout << ">> Dealer paid $" << drawCost << " to draw a card." << endl;
    }
}

int sabaccDealerDecideBet() {
    
    int score = sabaccAbsolute(sabaccCalculateScore(sabaccHandDealer, sabaccSizeDealer));
    
    int limit = sabaccGetCost(sabaccPercentageEnter * 1.5);
    
    //Fold
    if (sabaccCurrentBet > limit && score > 10) return 0;
    
    // Raise return 2; 
    if (score <= 3 && (rand() % 100 < 30)) return 2;
    
    //Call
    return 1;
}

// FAZY GRY

void sabaccShowStatus(bool finalReveal) {
    
    cout << endl << "-----------------------------------------------------------" << endl;
    cout << " MONEY: $" << sabaccCreditsPlayer << " | TABLE POT: $" << sabaccPotSabaccGame << " | SABACC POT: $" << sabaccPotSabacc << endl;
    cout << "-----------------------------------------------------------" << endl;
    
    cout << "YOUR HAND:" << endl;
    
    sabaccPrintCard(sabaccHandPlayer, sabaccSizePlayer, false); 

    if (finalReveal) {

        cout << "Score: " << sabaccCalculateScore(sabaccHandPlayer, sabaccSizePlayer) << endl;
    }
    
    cout << "-----------------------------------------------------------" << endl;
    
    cout << "DEALER'S HAND:" << endl;
    
    sabaccPrintCard(sabaccHandDealer, sabaccSizeDealer, !finalReveal);

    if (finalReveal) {

        cout << "Score: " << sabaccCalculateScore(sabaccHandDealer, sabaccSizeDealer) << endl;
    }
    
    cout << "-----------------------------------------------------------" << endl;
}

bool sabaccHandleBetting() {
    
    cout << endl << "--- LICITATION ---" << endl;
    cout << "Current rate: $" << sabaccCurrentBet << endl;
    cout << "1. Call" << endl << "2. Raise" << endl << "3. Fold" << endl;
    
    int choice = sabaccValidateInput("?> Choice: ", 1, 3);

    if (choice == 3) {
        
        cout << ">> You folded. Dealer wins the money." << endl;
        
        sabaccCreditsDealer += sabaccPotSabaccGame; return false; 
    }
    if (choice == 2) {
        
        int raise = sabaccValidateInput("?> How much you raise?: ", 0, 0);
        
        sabaccCurrentBet += raise;
    }

    int sabaccActionDealer = sabaccDealerDecideBet();
    
    if (sabaccActionDealer == 0) {
        
        cout << ">> Dealer folded! You won $ " << sabaccPotSabaccGame << "1" << endl;
        
        sabaccCreditsPlayer += sabaccPotSabaccGame; return false;
    } 
    else if (sabaccActionDealer == 2) {
        
        int dRaise = sabaccGetCost(sabaccPercentageRaise) + (rand() % 10);

        sabaccCurrentBet += dRaise;
        
        cout << ">> Dealer raise for $" << dRaise << "!" << endl;
    } 
    else {
        
        cout << ">> Dealer calls." << endl;
    }

    sabaccCreditsPlayer -= sabaccCurrentBet;
    sabaccCreditsDealer -= sabaccCurrentBet;
   
    sabaccPotSabaccGame += (sabaccCurrentBet * 2);
   
    return true;
}

void sabaccHandleSpike() {
    
    int d1 = rand() % 6 + 1, d2 = rand() % 6 + 1;
    
    if (d1 == d2) {
        
        cout << ">> Dice roll: [" << d1 << "] [" << d2 << "] !!! SPIKE !!!" << endl;
        
        int oldP = sabaccSizePlayer, oldD = sabaccSizeDealer;
        
        sabaccSizePlayer = 0; sabaccSizeDealer = 0;
        
        for (int i = 0; i < oldP; i++) sabaccHandPlayer[sabaccSizePlayer++] = sabaccDrawCard();
        for (int i = 0; i < oldD; i++) sabaccHandDealer[sabaccSizeDealer++] = sabaccDrawCard();
    }
}

void sabaccPlayRound() {
    
    int costEnter = sabaccGetCost(sabaccPercentageEnter);
    int costSabacc = sabaccGetCost(sabaccPercentageSabacc);
    
    if (sabaccCreditsPlayer < costEnter + costSabacc) {
        cout << "!> Not enough credits to play!" << endl;
        sabaccCreditsPlayer = 0; return;
    }
    
    sabaccSizePlayer = 0; 
    sabaccSizeDealer = 0;
    
    sabaccPotSabaccGame = costEnter * 2;
    sabaccPotSabacc += costSabacc * 2;

    sabaccCreditsPlayer -= (costEnter + costSabacc); 
    sabaccCreditsDealer -= (costEnter + costSabacc); 
    
    sabaccCurrentBet = 0;

    sabaccHandPlayer[sabaccSizePlayer++] = sabaccDrawCard(); 
    sabaccHandPlayer[sabaccSizePlayer++] = sabaccDrawCard();
    sabaccHandDealer[sabaccSizeDealer++] = sabaccDrawCard(); 
    sabaccHandDealer[sabaccSizeDealer++] = sabaccDrawCard();

    for (int turn = 1; turn <= 3; turn++) {
        
        cout << endl << "========= ROUND " << turn << " =========" << endl;
        sabaccShowStatus(false);

        cout << endl << "--- YOUR MOVE ---" << endl;
        cout << "1. Draw ($" << sabaccGetCost(sabaccPercentageDraw) << ")" << endl << "2. Swap ($" << sabaccGetCost(sabaccPercentageSwap) << ")" << endl << "3. Wait" << endl;
        
        int choice = sabaccValidateInput("?> Choice: ", 1, 3);
        
        if (choice == 1 && sabaccCreditsPlayer >= 5) {
            
            sabaccHandPlayer[sabaccSizePlayer++] = sabaccDrawCard();
            
            sabaccCreditsPlayer -= sabaccGetCost(sabaccPercentageDraw); 
            sabaccPotSabaccGame += sabaccGetCost(sabaccPercentageDraw);
        } 
        else if (choice == 2 && sabaccSizePlayer > 0 && sabaccCreditsPlayer >= 10) {
            
            string cardPrompt = ">> Card to swap (1 - " + to_string(sabaccSizePlayer) + "): ";
            
            int card = sabaccValidateInput(cardPrompt, 1, sabaccSizePlayer);
            
            if (card > 0 && card <= sabaccSizePlayer) {
                
                sabaccHandPlayer[card-1] = sabaccDrawCard();
                
                sabaccCreditsPlayer -= sabaccGetCost(sabaccPercentageSwap); 
                sabaccPotSabaccGame += sabaccGetCost(sabaccPercentageSwap);
            }
        }

        sabaccDealerAction();

        if (!sabaccHandleBetting()) return;
        
        sabaccHandleSpike();
    }

    sabaccShowStatus(true);
    
    int sabaccScorePlayer = sabaccCalculateScore(sabaccHandPlayer, sabaccSizePlayer);
    int sabaccScoreDealer = sabaccCalculateScore(sabaccHandDealer, sabaccSizeDealer);
    int sabaccAbsolutePlayer = sabaccAbsolute(sabaccScorePlayer), sabaccAbsoluteDealer = sabaccAbsolute(sabaccScoreDealer);

    bool playerWins = (sabaccAbsolutePlayer < sabaccAbsoluteDealer) || (sabaccAbsolutePlayer == sabaccAbsoluteDealer && sabaccScorePlayer >= sabaccScoreDealer);

    if (playerWins && sabaccAbsolutePlayer <= 23) {
        
        cout << "!> YOU'RE A WINNER! You won $" << sabaccPotSabaccGame << "." << endl;
        sabaccCreditsPlayer += sabaccPotSabaccGame;
        
        if (sabaccScorePlayer == 0) {
            
            cout << "!> PURE SABACC! Bonus: $" << sabaccPotSabacc << "." << endl;
            
            sabaccCreditsPlayer += sabaccPotSabacc; 
            sabaccPotSabacc = 0;
        }
    } 
    else {
        
        cout << "!> YOU LOST. Dealer won." << endl;
        
        sabaccCreditsDealer += sabaccPotSabaccGame;
    }
}

int sabacc(int money) {
    
    srand(time(0));
    
    cout << "===> WELCOME IN CANTYNA - SABACC CORELLIAN SPIKE <===" << endl;
    
    sabaccCreditsStart = money;
    
    sabaccCreditsPlayer = sabaccCreditsStart;
    sabaccCreditsDealer = sabaccCreditsStart;
    
    char playAgain = 'y';
    
    while ((playAgain == 'y' || playAgain == 'Y') && sabaccCreditsPlayer > 0) {
        
        sabaccPlayRound();
        
        if (sabaccCreditsPlayer <= 0) {
            
            cout << endl << "!> Bankrut! You've been thrown out." << endl;
            break;
        }
        
        cout << endl << "?> Wanna play again? (y/n): ";
        
        cin >> playAgain;
    }

    return sabaccCreditsPlayer;
}

// SABACC END ------------------------------------------------------------------------------------

int main()
{
	Player currentPlayer;
	string loginChoice;
	while (true) {
		cout << "===> CHOOSE AN OPTION <===" << endl << "L. Login" << endl << "R. Register" << endl << "?> Choice: ";
		cin >> loginChoice;
		cout << endl;
		if (loginChoice == "L" || loginChoice == "l") {
			cout << "?> Enter your username: ";
			string playerName;
			cin >> playerName;
			if (fileExists(playerName)) {
				currentPlayer = getCurrentPlayer(playerName);
				break;
			}
			else {
				cout << "!> No account with that username exists. Please register first." << endl;
				continue;
			}
			
		}
		else if (loginChoice == "R" || loginChoice == "r") {
			cout << "?> Enter your desired username: ";
			string playerName;
			cin >> playerName;
			if(fileExists(playerName)){
				cout << "!> Account with that username already exists. Please choose a different username." << endl << endl;
			}
			else {
				createPlayer(playerName);
				break;
			}
		}
		else {
			cout << "!> Invalid choice. Please enter L to log in or R to register: ";
		}
	}
	cout << ">> Current bank: " << currentPlayer.getMoney() << endl << endl;
	string depositChoice = "";
	while (true) {
		if (currentPlayer.getMoney() < 100) {
			cout << "!> You don't have enough money to play. The minimum is $100." << endl;
		}
		cout << "?> Would you like to deposit money to your account? (Y/N): ";
		cin >> depositChoice;
		if (depositChoice == "Y" || depositChoice == "y") {
			cout << "Enter how much would you like to deposit: ";
			int depositAmount = 0;
			while (depositAmount <= 0) {
				cin >> depositAmount;
				if (depositAmount <= 0) {
					cout << "!> Deposit amount must be greater than 0. Please enter a valid amount: ";
				}
				else {
					currentPlayer.setMoney(currentPlayer.getMoney() + depositAmount);
					overwritePlayerMoney(currentPlayer);
					cout << ">> Transaction successful. Your new bank: " << currentPlayer.getMoney() << endl;
					break;
				}
			}
		}
		else if (depositChoice == "N" || depositChoice == "n") {
			if (currentPlayer.getMoney() < 100) {
				cout << "!> You don't have enough money to play and are not willing to deposit. You exit the casino." << endl;
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
		system("clear");
		
		cout << "Your username: "<< currentPlayer.getNickname() << endl;
		cout << "Current money: "<< currentPlayer.getMoney() << endl;

        cout<<endl<<"===> CHOOSE A GAME <==="<<endl;
    
        cout<<"1. Blackjack"<<endl;         
        cout<<"2. Roulette"<<endl;
		cout<<"3. Slot"<<endl;
		cout<<"4. Sabacc"<<endl;

        cout<<"0. Quit"<<endl;

        cout<<"?> Choice: ";

		//	Ten while służy do sprawdzania czy gra o takim numerze istnieje. Jest to określane przez zakres określony przy pierwszej strzałce w komentarzu poniżej.
        while(true){   
			if (gameChoice == 0) {
				cout << ">> We hope to see you again!" << endl;
				return 0;
			}
			if (gameChoice >= 1 && gameChoice <= 4	/*Tutaj dopisywane sprawdzanie liczby gry w postaci gameChoice >= 1 && gameChoice <= *Aktualna liczba możliwych gier* */) {         // <----------------------------
				break;
			}
			else if(gameChoice != -1){
				cout << "!> There is no game with that number." << endl;
			}

			//	Wartość wyboru gry jest pobierana jako ostatnia aby umożliwić czystą funkcjonalność ponownej gry dla funkcji bez wbudowanej logiki ponownej gry.				<----- WAŻNE !!! (Należy gameChoice ustawiać na -1 w logice ponownej gry gdy gracz nie chce jej powtarzać)
			cin >> gameChoice;
			system("clear");
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
			case 3:
                currentPlayer.setMoney(slot(currentPlayer.getMoney()));
				overwritePlayerMoney(currentPlayer);
				gameChoice = -1;
                break;
			case 4:
                currentPlayer.setMoney(sabacc(currentPlayer.getMoney()));
				overwritePlayerMoney(currentPlayer);
				gameChoice = -1;
                break;
            default:                        //Ze względu na zabezpieczony brak nieodpowiedniego case, default jest używany do powtarzania gry poprzez niezmianianą zmienną 'gameChoice' lub wprost niepoprawnie wprowadzoną wartość.
                cout<<"!> No game has been chosen or the input is invalid."<<endl;
                return 0;
        }
    }
}