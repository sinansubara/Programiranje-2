#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;


char * crt = "\n----------------------------------------------------\n";
struct Datum {
	int * _dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina; }
	void Dealociraj() {
		delete _dan; delete _mjesec; delete _godina;
		_dan = _mjesec = _godina = nullptr;
	}
	int sumaDana()
	{
		int brDana[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
		int s = 0;
		for (int i = 0; i < *_godina; i++)
		{
			if ((i % 4 == 0 && i % 100 != 0) || i % 400 == 0)
				s += 366;
			else
				s += 365;
		}
		for (int i = 0; i < *_mjesec; i++)
		{
			if (((*_godina % 4 == 0 && *_godina % 100 != 0) || *_godina % 400 == 0) && i == 2)
				s += 29;
			else
				s += brDana[i - 1];
		}
		s += *_dan;
		return s;
	}
};
int razlika(Datum prvi, Datum drugi)
{
	return prvi.sumaDana() - drugi.sumaDana();
}
struct Izostanak {
	char * _razlogIzostanka;
	Datum _datumIzostanka;
	int _brojSati;
	bool _opravdano;//pravdanje zahtijeva da se evidentira i razlog izostanka	
	void Unos(Datum datumIzostanka, int brojSati) {
		_datumIzostanka.Unos(*datumIzostanka._dan, *datumIzostanka._mjesec, *datumIzostanka._godina);
		_brojSati = brojSati;
		_opravdano = false;
		_razlogIzostanka = new char;
		_razlogIzostanka[0] = '\0';
	}
	void Dealociraj() { delete[]_razlogIzostanka; _razlogIzostanka = nullptr; }
	void Ispis() { _datumIzostanka.Ispis(); cout << " " << _brojSati << " " << _razlogIzostanka << endl; }
	void Opravdaj(char * razlogIzostanka) {
		if (_razlogIzostanka != nullptr)
			delete _razlogIzostanka;
		int vel = strlen(razlogIzostanka) + 1;
		_razlogIzostanka = new char[vel];
		strcpy_s(_razlogIzostanka, vel, razlogIzostanka);
		_opravdano = true;
		_brojSati = 0;
	}
};
struct Student {
	//format broja indeksa: IB150051
	//osigurati automatsko generisanje broja indeksa pocevsi od IB150001
	char _brojIndeksa[9];
	char * _imePrezime;
	Izostanak * _izostanci;
	int _brojIzostanaka;
	//Potpisi funkcija trebaju odgovarati onima u main-u
	//1. Unos
	//2. Dealociraj
	/*3. DodajIzostanak - dodaje novi izostanak u listu izostanaka.	Onemoguciti ponavljanje izostanaka na nacin da se uvecava samo broj sati ukoliko
	korisnik (profesor) vise puta pokusa dodati izostanak na isti dan.*/
	//4. Ispis - ispisuje izostanke u formatu prikazanom na slici (nije obavezno da ispis bude identican, ali je svakako pozeljno). 
	//Prilikom svakog ispisa, izostanke je potrebno spasiti u tekstualni fajl sa istim nazivom kao i broj indeksa.
	//5. BinarnaPretraga - koristeci binarnu pretragu, na osnovu datuma, pronalazi i vraca pokazivac na izostanak. Ukoliko trazeni 
	//izostanak ne postoji funkcija vraca nullptr.

	void Unos(char * brojIndexa, char * imePrezime)
	{
		strcpy_s(_brojIndeksa, strlen(brojIndexa) + 1, brojIndexa);
		_imePrezime = new char[strlen(imePrezime) + 1];
		strcpy_s(_imePrezime, strlen(imePrezime) + 1, imePrezime);
		_izostanci = nullptr;
		_brojIzostanaka = 0;
	}
	void Dealociraj()
	{
		delete[]_imePrezime; _imePrezime = nullptr;
		delete[]_izostanci; _izostanci = nullptr;

	}
	void DodajIzostanak(Izostanak novi)
	{

		for (int i = 0; i < _brojIzostanaka; i++)
		{
			if (razlika(_izostanci[i]._datumIzostanka, novi._datumIzostanka) == 0)
			{
				_izostanci[i]._brojSati += novi._brojSati;
				return;
			}
		}
		Izostanak * temp = new Izostanak[_brojIzostanaka + 1];
		for (int i = 0; i < _brojIzostanaka; i++)
			temp[i] = _izostanci[i];
		temp[_brojIzostanaka].Unos(novi._datumIzostanka, novi._brojSati);
		_brojIzostanaka++;
		delete[]_izostanci;
		_izostanci = temp;
		temp = nullptr;

	}
	void Ispis()
	{
		cout << crt << endl;
		cout << "Broj indexa --> " << _brojIndeksa << endl;
		cout << "Ime i prezime --> " << _imePrezime << endl;
		cout << crt << setw(20) << "Izostanci" << endl << crt << endl;
		for (int i = 0; i < _brojIzostanaka; i++)
		{
			_izostanci[i].Ispis(); cout << endl;
		}
		cout << crt << endl << crt << endl;

		char temp[15];
		strcpy_s(temp, strlen(_brojIndeksa) + 1, _brojIndeksa);
		strcat_s(temp, ".txt");

		char *NazivFajla = new char[strlen(temp) + 1];
		strcpy_s(NazivFajla, strlen(temp) + 1, temp);

		ofstream Upis(NazivFajla, ios::out);

		if (!Upis.fail())
		{
			Upis << crt << endl;
			Upis << "Broj indexa --> " << _brojIndeksa << endl;
			Upis << "Ime i prezime --> " << _imePrezime << endl;
			Upis << crt << setw(20) << "Izostanci" << endl << crt << endl;
			for (int i = 0; i < _brojIzostanaka; i++)
			{
				Upis << *_izostanci[i]._datumIzostanka._dan << "/" << *_izostanci[i]._datumIzostanka._mjesec << "/" << *_izostanci[i]._datumIzostanka._godina; Upis << " " << _izostanci[i]._brojSati << " " << _izostanci[i]._razlogIzostanka << endl;
			}
			Upis << crt << endl << crt << endl;
			Upis.close();
		}
		else
			cout << "Error pri otvaranju fajla..." << endl;
	}
	void Sortiraj()
	{
		bool sort; int vel = _brojIzostanaka;
		do
		{
			sort = false;
			for (int i = 0; i < _brojIzostanaka - 1; i++)
			{
				if (razlika(_izostanci[i]._datumIzostanka, _izostanci[i + 1]._datumIzostanka) > 0)
				{
					Izostanak * temp = new Izostanak;
					temp->Unos(_izostanci[i]._datumIzostanka, _izostanci[i]._brojSati);
					_izostanci[i].Dealociraj();
					_izostanci[i].Unos(_izostanci[i + 1]._datumIzostanka, _izostanci[i + 1]._brojSati);
					_izostanci[i + 1].Dealociraj();
					_izostanci[i + 1].Unos(temp->_datumIzostanka, temp->_brojSati);
					temp->Dealociraj();
					sort = true;
				}
			}
			vel -= 1;
		} while (sort == true && vel>1);


	}
	Izostanak * BinarnaPretraga(Datum d)
	{
		Sortiraj();
		int prvi = 0, zadnji = _brojIzostanaka - 1, srednji;

		while (prvi <= zadnji)
		{
			srednji = (prvi + zadnji) / 2;
			if (razlika(_izostanci[srednji]._datumIzostanka, d) > 0)
				zadnji = srednji - 1;
			if (razlika(_izostanci[srednji]._datumIzostanka, d) < 0)
				prvi = srednji + 1;
			if (razlika(_izostanci[srednji]._datumIzostanka, d) == 0)
				return &_izostanci[srednji];
		}
		return nullptr;

	}
};
//1. PronadjiNajveciNeopravdaniIzostanak - rekurzivna funkcija koja vraca pokazivac na neopravdani izostanak koji ima najveci broj sati
//2. GenerisiSljedeciBrojIndeksa - generise i vraca naredni broj indeksa
Izostanak *  PronadjiNajveciNeopravdaniIzostanak(Student s, int brIzostanaka, int sati, int pozicija)
{
	if (brIzostanaka < 0)
		return &s._izostanci[pozicija];
	if (s._izostanci[brIzostanaka]._brojSati > sati)
	{
		sati = s._izostanci[brIzostanaka]._brojSati;
		pozicija = brIzostanaka;
	}
	return PronadjiNajveciNeopravdaniIzostanak(s, brIzostanaka - 1, sati, pozicija);
}
int br = 150001;
char * GenerisiSljedeciBrojIndeksa()
{
	char temp[9], broj[7];
	strcpy_s(temp, strlen("IB") + 1, "IB");
	_itoa_s(br, broj, 10);
	strcat_s(temp, broj);
	char * brojIndexa = new char[strlen(temp) + 1];
	strcpy_s(brojIndexa, strlen(temp) + 1, temp);
	br += 1;
	return brojIndexa;
}
void main() {
	Datum jucer, prije5Dana, prije10Dana;
	jucer.Unos(12, 6, 2016);
	prije5Dana.Unos(8, 6, 2016);
	prije10Dana.Unos(3, 6, 2016);

	Student denis;
	denis.Unos(GenerisiSljedeciBrojIndeksa(), "Denis Music");

	Izostanak izostanakJucer, izostanakPrije5Dana, izostanakPrije10Dana;

	izostanakJucer.Unos(jucer, 5);
	denis.DodajIzostanak(izostanakJucer);

	izostanakPrije5Dana.Unos(prije5Dana, 3);
	izostanakPrije5Dana.Opravdaj("Odsutan zbog bolesti - gripa");
	denis.DodajIzostanak(izostanakPrije5Dana);
	izostanakPrije5Dana.Unos(prije5Dana, 6);
	denis.DodajIzostanak(izostanakPrije5Dana);

	izostanakPrije10Dana.Unos(prije10Dana, 1);
	denis.DodajIzostanak(izostanakPrije10Dana);
	izostanakPrije10Dana.Unos(prije10Dana, 1);
	denis.DodajIzostanak(izostanakPrije10Dana);

	denis.Ispis();
	Izostanak * p = denis.BinarnaPretraga(prije10Dana);
	if (p != nullptr)
	{
		cout << "Binarnom pretragom pronadjen izostanak -> ";
		p->Ispis();
	}
	else
		cout << "Ne postoji izostanak sa tim datumom" << endl;
	denis.Ispis();
	Izostanak * p2 = PronadjiNajveciNeopravdaniIzostanak(denis, denis._brojIzostanaka - 1, 0, 0);
	cout << "Rekurzivno pronadjen najveci neopravdani izostanak -> ";
	p2->Ispis();
	//cout << endl;
	//dealocirati zauzetu memoriju

	jucer.Dealociraj(); prije5Dana.Dealociraj(); prije10Dana.Dealociraj();
	denis.Dealociraj();
	p = nullptr;
	p2 = nullptr;
	system("pause");
}
