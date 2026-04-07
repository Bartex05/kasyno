#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
using namespace std;

void table()
{
cout << "   |  0  |  00  |\n";
cout << "---------------------------------\n";
cout << "| 1R  | 2B  | 3R  |             |\n";
cout << "-------------------             |\n";
cout << "| 4B  | 5R  | 6B  |    tuzin    |\n";
cout << "-------------------             |\n";
cout << "| 7R  | 8B  | 9R  |      1      |\n";
cout << "-------------------             |\n";
cout << "| 10B | 11R | 12B |             |\n";
cout << "---------------------------------\n";
cout << "| 13R | 14B | 15R |             |\n";
cout << "-------------------             |\n";
cout << "| 16B | 17R | 18B |    tuzin    |\n";
cout << "-------------------             |\n";
cout << "| 19R | 20B | 21R |      2      |\n";
cout << "-------------------             |\n";
cout << "| 22B | 23R | 24B |             |\n";
cout << "---------------------------------\n";
cout << "| 25R | 26B | 27R |             |\n";
cout << "-------------------             |\n";
cout << "| 28B | 29R | 30B |    tuzin    |\n";
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
cout<<"Mozliwe zaklady: "<<endl;
cout<<"----------------------------\n";
cout<<"| jedna liczba | (1) | x36 |\n";
cout<<"----------------------------\n";
cout<<"| dwie liczby  | (2) | x18 |\n";
cout<<"----------------------------\n";
cout<<"| trzy liczby  | (3) | x12 |\n";
cout<<"----------------------------\n";
cout<<"|     tuzin    | (t) |  x3 |\n";
cout<<"----------------------------\n";
cout<<"|    kolumna   | (k) |  x3 |\n";
cout<<"----------------------------\n";
cout<<"|     kolor    | (c) |  x2 |\n";
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

int main()
{

//------------------------------------------------------------//

	//odp tak/nie
	string odp;

	//pieniadze
	int saldo=0;
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

		cout<<"Witaj czy chcesz zagrac w ruletke? [y/n]: ";
		getline(cin, odp);
		//cout<<"\n";
		this_thread::sleep_for(chrono::milliseconds(1000));

		if(odp!="y" && odp!="n")
		{
			cout<<"Nie ma takiej odpowiedzi"<<endl;
			this_thread::sleep_for(chrono::milliseconds(3000));
		}
	}

//------------------------------------------------------------

	if(odp=="y")
	{
		while(saldo==0 || saldo<0)
		{
			system("clear");

			cout<<"Wpisz swoje saldo poczatkowe: ";
			cin>>saldo;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			//cout<<"\n";
			this_thread::sleep_for(chrono::milliseconds(1000));

			if(saldo<0 || saldo==0)
			{
				cout<<"Saldo nie moze byc mniejsze lub rowne zero"<<endl;
				this_thread::sleep_for(chrono::milliseconds(3000));
			}
		}
	}

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
			kolor="czerwony";
		}
		if((los+2)%2==0)
		{
			kolor="czarny";
		}
		if(los==0 || los==37)
		{
			kolor="zielony";
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

		while(typ!="1" && typ!="2" && typ!="3" && typ!="t" &&  typ!="k" && typ!="c")
		{
			system("clear");

			zasady();

			cout<<"Jakiego typu zaklad chcesz postawic (1/2/3/t/k/c): ";
			getline(cin, typ);
			//cout<<"\n";
			this_thread::sleep_for(chrono::milliseconds(1000));

			if(typ!="1" && typ!="2" && typ!="3" && typ!="t" &&  typ!="k" && typ!="c")
			{
                        	cout<<"Nie ma takiej odpowiedzi"<<endl;
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
				cout<<"Wpisz liczbe ktora wybrales (0-36 lub 00): ";
				getline(cin, wartosc1);
				//cout<<"\n";
				if(wartosc1!="00")
				{
					x1=stoi(wartosc1);
				}
				this_thread::sleep_for(chrono::milliseconds(1000));

				if(wartosc1!="00" && (x1<0 || x1>36))
				{
                        		cout<<"Nie ma takiej odpowiedzi";
					cout<<"\n";
					cout<<"\n";
                        		this_thread::sleep_for(chrono::milliseconds(3000));
                        		//system("clear");
				}
			}

			while(suma<0 || suma>saldo)
			{
				system("clear");

				cout<<"Twoje saldo wynosi: "<<saldo;
				cout<<"\n";
				cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(2000));

				cout<<"Wpisz sume jaka chcesz postawic: ";
				cin>>suma;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			 	//cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(1000));

				if(suma>saldo)
				{
					cout<<"Suma nie moze byc wieksza niz saldo";
					cout<<"\n";
					this_thread::sleep_for(chrono::milliseconds(3000));
				}
				if(suma<0)
				{
					cout<<"Suma nie moze byc mniejsza od 0";
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
				cout<<"Wpisz pierwsza liczbe ktora wybrales (0-36 lub 00): ";
				getline(cin, wartosc1);
				//cout<<"\n";
				if(wartosc1!="00")
				{
					x1=stoi(wartosc1);
				}
				this_thread::sleep_for(chrono::milliseconds(1000));

				if(wartosc1!="00" && (x1<0 || x1>36))
				{
                        		cout<<"Nie ma takiej odpowiedzi";
					cout<<"\n";
					cout<<"\n";
                        		this_thread::sleep_for(chrono::milliseconds(3000));
                        		//system("clear");
				}
			}

			while(wartosc2!="00" && (x2<0 || x2>36) || wartosc2==wartosc1)
			{
				x2=-1;
				cout<<"Wpisz druga liczbe ktora wybrales (0-36 lub 00): ";
				getline(cin, wartosc2);
				//cout<<"\n";
				if(wartosc2!="00")
				{
					x2=stoi(wartosc2);
				}
				this_thread::sleep_for(chrono::milliseconds(1000));

				if(wartosc2!="00" && (x2<0 || x2>36))
				{
                        		cout<<"Nie ma takiej odpowiedzi";
					cout<<"\n";
					cout<<"\n";
                        		this_thread::sleep_for(chrono::milliseconds(3000));
                        		//system("clear");
				}

				if(wartosc2==wartosc1)
				{
                        		cout<<"Druga liczba nie moze byc taka sama jak pierwsza";
					cout<<"\n";
					cout<<"\n";
                        		this_thread::sleep_for(chrono::milliseconds(3000));
                        		//system("clear");
				}
			}

			while(suma<0 || suma>saldo)
			{
				system("clear");

				cout<<"Twoje saldo wynosi: "<<saldo;
				cout<<"\n";
				cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(2000));

				cout<<"Wpisz sume jaka chcesz postawic: ";
				cin>>suma;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			 	//cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(1000));

				if(suma>saldo)
				{
					cout<<"Suma nie moze byc wieksza niz saldo";
						cout<<"\n";
					this_thread::sleep_for(chrono::milliseconds(3000));
				}
				if(suma<0)
				{
					cout<<"Suma nie moze byc mniejsza od 0";
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
				cout<<"Wpisz pierwsza liczbe ktora wybrales (0-36 lub 00): ";
				getline(cin, wartosc1);
				//cout<<"\n";
				if(wartosc1!="00")
				{
					x1=stoi(wartosc1);
				}
				this_thread::sleep_for(chrono::milliseconds(1000));

				if(wartosc1!="00" && (x1<0 || x1>36))
				{
                        		cout<<"Nie ma takiej odpowiedzi";
					cout<<"\n";
					cout<<"\n";
                        		this_thread::sleep_for(chrono::milliseconds(3000));
                        		//system("clear");
				}
			}

			while(wartosc2!="00" && (x2<0 || x2>36) || wartosc2==wartosc1)
			{
				x2=-1;
				cout<<"Wpisz druga liczbe ktora wybrales (0-36 lub 00): ";
				getline(cin, wartosc2);
				//cout<<"\n";
				if(wartosc2!="00")
				{
					x2=stoi(wartosc2);
				}
				this_thread::sleep_for(chrono::milliseconds(1000));

				if(wartosc2!="00" && (x2<0 || x2>36))
				{
                        		cout<<"Nie ma takiej odpowiedzi";
					cout<<"\n";
					cout<<"\n";
                        		this_thread::sleep_for(chrono::milliseconds(3000));
                        		//system("clear");
				}

				if(wartosc2==wartosc1)
				{
                        		cout<<"Druga liczba nie moze byc taka sama jak pierwsza";
					cout<<"\n";
					cout<<"\n";
                        		this_thread::sleep_for(chrono::milliseconds(3000));
                        		//system("clear");
				}
			}

			while(wartosc3!="00" && (x3<0 || x3>36) || wartosc3==wartosc1 || wartosc3==wartosc2)
			{
				x3=-1;
				cout<<"Wpisz trzecia liczbe ktora wybrales (0-36 lub 00): ";
				getline(cin, wartosc3);
				//cout<<"\n";
				if(wartosc3!="00")
				{
					x3=stoi(wartosc3);
				}
				this_thread::sleep_for(chrono::milliseconds(1000));

				if(wartosc3!="00" && (x3<0 || x3>36))
				{
                        		cout<<"Nie ma takiej odpowiedzi";
					cout<<"\n";
					cout<<"\n";
                        		this_thread::sleep_for(chrono::milliseconds(3000));
                        		//system("clear");
				}

				if(wartosc3==wartosc1)
				{
                        		cout<<"Trzecia liczba nie moze byc taka sama jak pierwsza";
					cout<<"\n";
					cout<<"\n";
                        		this_thread::sleep_for(chrono::milliseconds(3000));
                        		//system("clear");
				}

				if(wartosc3==wartosc2)
				{
                        		cout<<"Trzecia liczba nie moze byc taka sama jak druga";
					cout<<"\n";
					cout<<"\n";
                        		this_thread::sleep_for(chrono::milliseconds(3000));
                        		//system("clear");
				}
			}

			while(suma<0 || suma>saldo)
			{
				system("clear");

				cout<<"Twoje saldo wynosi: "<<saldo;
				cout<<"\n";
				cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(2000));

				cout<<"Wpisz sume jaka chcesz postawic: ";
				cin>>suma;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			 	//cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(1000));

				if(suma>saldo)
				{
					cout<<"Suma nie moze byc wieksza niz saldo";
					cout<<"\n";
					this_thread::sleep_for(chrono::milliseconds(3000));
				}
				if(suma<0)
				{
					cout<<"Suma nie moze byc mniejsza od 0";
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

		if(typ=="t")
		{
			while(wartosct!="1" && wartosct!="2" && wartosct!="3")
			{
				cout<<"Wpisz tuzin ktory wybrales (1/2/3): ";
				getline(cin, wartosct);
				//cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(1000));

				if(wartosct!="1" && wartosct!="2" && wartosct!="3")
				{
                        		cout<<"Nie ma takiej odpowiedzi";
					cout<<"\n";
					cout<<"\n";
                        		this_thread::sleep_for(chrono::milliseconds(3000));
                        		//system("clear");
				}
			}

			while(suma<0 || suma>saldo)
			{
				system("clear");

				cout<<"Twoje saldo wynosi: "<<saldo;
				cout<<"\n";
				cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(2000));

				cout<<"Wpisz sume jaka chcesz postawic: ";
				cin>>suma;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			 	//cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(1000));

				if(suma>saldo)
				{
					cout<<"Suma nie moze byc wieksza niz saldo";
					cout<<"\n";
					this_thread::sleep_for(chrono::milliseconds(3000));
				}
				if(suma<0)
				{
					cout<<"Suma nie moze byc mniejsza od 0";
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
				cout<<"Wpisz kolumne ktora wybrales (1/2/3): ";
				getline(cin, wartosck);
				//cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(1000));

				if(wartosck!="1" && wartosck!="2" && wartosck!="3")
				{
                        		cout<<"Nie ma takiej odpowiedzi";
					cout<<"\n";
					cout<<"\n";
                        		this_thread::sleep_for(chrono::milliseconds(3000));
                        		//system("clear");
				}
			}

			while(suma<0 || suma>saldo)
			{
				system("clear");

				cout<<"Twoje saldo wynosi: "<<saldo;
				cout<<"\n";
				cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(2000));

				cout<<"Wpisz sume jaka chcesz postawic: ";
				cin>>suma;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			 	//cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(1000));

				if(suma>saldo)
				{
					cout<<"Suma nie moze byc wieksza niz saldo";
					cout<<"\n";
					this_thread::sleep_for(chrono::milliseconds(3000));
				}
				if(suma<0)
				{
					cout<<"Suma nie moze byc mniejsza od 0";
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
			while(wartoscc!="czarny" && wartoscc!="czerwony" && wartoscc!="zielony")
			{
				cout<<"Wpisz kolor ktory wybrales (czarny/czerwony/zielony): ";
				getline(cin, wartoscc);
				//cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(1000));

				if(wartoscc!="czarny" && wartoscc!="czerwony" && wartoscc!="zielony")
				{
                        		cout<<"Nie ma takiej odpowiedzi";
					cout<<"\n";
					cout<<"\n";
                        		this_thread::sleep_for(chrono::milliseconds(3000));
                        		//system("clear");
				}
			}

			while(suma<0 || suma>saldo)
			{
				system("clear");

				cout<<"Twoje saldo wynosi: "<<saldo;
				cout<<"\n";
				cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(2000));

				cout<<"Wpisz sume jaka chcesz postawic: ";
				cin>>suma;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			 	//cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(1000));

				if(suma>saldo)
				{
					cout<<"Suma nie moze byc wieksza niz saldo";
					cout<<"\n";
					this_thread::sleep_for(chrono::milliseconds(3000));
				}
				if(suma<0)
				{
					cout<<"Suma nie moze byc mniejsza od 0";
					cout<<"\n";
					this_thread::sleep_for(chrono::milliseconds(3000));
				}
				cout<<"\n";
			}

			saldo=saldo-suma;

			if(wartoscc!="zielony" && wartoscc==kolor)
			{
				win=1;
				suma=suma*2;
				saldo=saldo+suma;
			}
			if(wartoscc=="zielony" && wartoscc==kolor)
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

		cout<<"Ruletka wylosowala: "<<endl;
		this_thread::sleep_for(chrono::milliseconds(500));
		cout<<"Numer: "<<numer<<endl;
		this_thread::sleep_for(chrono::milliseconds(500));
		cout<<"Kolor: "<<kolor<<endl;
		this_thread::sleep_for(chrono::milliseconds(500));
		cout<<"Tuzin: "<<tuzin<<endl;
		this_thread::sleep_for(chrono::milliseconds(500));
		cout<<"Kolumna: "<<kolumna<<endl;
		this_thread::sleep_for(chrono::milliseconds(500));

		cout<<"\n";
		this_thread::sleep_for(chrono::milliseconds(3000));

//------------------------------------------------------------

		if(win==1)
		{
			cout<<"wygrales! "<<flush;
			//this_thread::sleep_for(chrono::milliseconds(1000));

			//cout<<"gratulacje! "<<flush;
			cout<<"\n";
			this_thread::sleep_for(chrono::milliseconds(2000));

			cout<<"twoje saldo wynosi teraz: "<<flush;
			this_thread::sleep_for(chrono::milliseconds(500));
			cout<<saldo<<flush;

			this_thread::sleep_for(chrono::milliseconds(3000));
		}

		if(win==0)
		{
			cout<<"przegrales :( "<<flush;
			//this_thread::sleep_for(chrono::milliseconds(1000));

			//cout<<"maybe next time..."<<flush;
			cout<<"\n"<<flush;
			this_thread::sleep_for(chrono::milliseconds(2000));


			cout<<"twoje saldo wynosi teraz: "<<flush;
			this_thread::sleep_for(chrono::milliseconds(500));
			cout<<saldo<<flush;
			this_thread::sleep_for(chrono::milliseconds(3000));
		}
		cout<<"\n";

//------------------------------------------------------------

		if(saldo==0 || saldo<0)
		{
			system("clear");
			cout<<"BANKRUT"<<endl;
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
			cout<<"Czy chcesz grac dalej? [y/n]: "<<flush;
			getline(cin, odp);
			//cout<<"\n";

			if(odp!="y" && odp!="n")
			{
				cout<<"Nie ma takiej odpowiedzi"<<endl;
				cout<<"\n";
				this_thread::sleep_for(chrono::milliseconds(3000));
			}
		}while(odp!="y" && odp!="n");

//------------------------------------------------------------

	}
	seeyou();

//------------------------------------------------------------//

}
