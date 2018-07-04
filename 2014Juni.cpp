#include<iostream>
#include<fstream>
using namespace std;


char * crt = "\n----------------------------------------------------\n";
struct Datum {
	int * _dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() {
		delete _dan; delete _mjesec; delete _godina;
		_dan = _mjesec = _godina = nullptr;
	}
};
struct Artikal {
	char * _naziv;
	float _cijena;
	void Unos(char * naziv, float cijena) {
		int vel = strlen(naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, naziv);
		_cijena = cijena;
	}
	void Dealociraj() { delete[]_naziv; _naziv = nullptr; }
	void Ispis() { cout << _naziv << " " << _cijena << " KM" << endl; }
};
struct Racun {
	//format broja racuna: 006005491
	//osigurati automatsko generisanje broja racuna pocevsi od 000000001
	char _brojRacuna[10];
	Datum _datumKreiranja;
	Artikal * _artikli;
	int * _kolicine;//cuva informaciju o kolicini svakog artikla na racunu
	int _brojArtikala;

	void Unos(char * brojracuna, Datum datum)
	{
		int size = strlen(brojracuna) + 1;
		strcpy_s(_brojRacuna, size, brojracuna);
		_datumKreiranja.Unos(*datum._dan, *datum._mjesec, *datum._godina);
		_artikli = nullptr;
		_kolicine = nullptr;
		_brojArtikala = 0;

	}
	void DodajArtikal(Artikal artikal, int kolicina)
	{
		if (_brojArtikala == 0)
		{
			_artikli = new Artikal;
			_artikli->Unos(artikal._naziv, artikal._cijena);
			_kolicine = new int(kolicina);
			_brojArtikala++;
			cout << "Artikal uspjesno dodan" << endl;
		}
		else
		{
			for (int i = 0; i < _brojArtikala; i++)
			{
				if (strcmp(_artikli[i]._naziv, artikal._naziv) == 0)
				{
					_kolicine[i] += kolicina;
					return;
				}
			}
			Artikal *tempartikal = new Artikal[_brojArtikala + 1];
			int *tempkolicina = new int[_brojArtikala + 1];
			for (int i = 0; i < _brojArtikala; i++)
			{
				tempartikal[i].Unos(_artikli[i]._naziv, _artikli[i]._cijena);
				_artikli[i].Dealociraj();
				tempkolicina[i] = _kolicine[i];
			}
			delete _artikli;
			delete _kolicine;
			_artikli = tempartikal;
			_kolicine = tempkolicina;
			_artikli[_brojArtikala].Unos(artikal._naziv, artikal._cijena);
			_kolicine[_brojArtikala] = kolicina;
			_brojArtikala++;
		}
	}
	void Ispis()
	{
		float suma = 0;
		cout << "Broj racuna: " << _brojRacuna << " Datum: "; _datumKreiranja.Ispis();
		cout << crt;
		for (int i = 0; i < _brojArtikala; i++)
		{
			cout << _artikli[i]._naziv << "  Kolicina: " << _kolicine[i] << "  Cijena" << _artikli[i]._cijena*_kolicine[i] << endl;
			suma += _artikli[i]._cijena*_kolicine[i];
		}
		cout << crt << "Broj artikala: " << _brojArtikala << " UKUPNO ZA PLATIT: " << suma << endl;
	}
	Artikal *BinarnaPretraga(char *naziv)
	{
		for (int i = 0; i < _brojArtikala-1; i++)
		{
			for (int j = i + 1; j < _brojArtikala; j++)
			{
				if (strcmp(_artikli[i]._naziv, _artikli[j]._naziv) > 0)
				{
					swap(_artikli[i], _artikli[j]);
					swap(_kolicine[i], _kolicine[j]);
				}
			}
		}
		cout <<crt<< "Sortirano: ";
		for (int i = 0; i < _brojArtikala; i++)
		{
			_artikli[i].Ispis();
			cout << endl;
		}
		int prvi = 0;
		int zadnji = _brojArtikala - 1;
		int srednji;
		bool pronadjen = false;
		while (pronadjen == false && prvi <= zadnji)
		{
			srednji = (prvi + zadnji) / 2;
			if (strcmp(naziv, _artikli[srednji]._naziv) < 0)
				zadnji = srednji - 1;
			if (strcmp(naziv, _artikli[srednji]._naziv) == 0)
				return &_artikli[srednji];
			if (strcmp(naziv, _artikli[srednji]._naziv) > 0)
				prvi = srednji + 1;

		}
	}
	
	//Potpisi funkcija trebaju odgovarati onima u main-u
	//1. Unos
	//2. Dealociraj
	//3. DodajArtikal - dodaje novi artikal u listu artikala zajedno sa njegovom kolicinom.
	//Onemoguciti ponavljanje artikala na nacin da se uvecava samo kolicina ukoliko
	//korisnik vise puta pokusa dodati isti artikal.
	//4. Ispis - ispisuje racun u formatu prikazanom na slici (nije obavezno da bude
	//identican, ali je svakako pozeljno).Prilikom svakog ispisa, racun
	//je potrebno spasiti u tekstualni fajl sa istim nazivom kao i broj racuna.
	//5. BinarnaPretraga - koristeci binarnu pretragu, na osnovu naziva, pronalazi i vraca
	//pokazivac na artikal.Ukoliko trazeni artikal ne postoji funkcija vraca nullptr.
};
Artikal *PronadjiNajskupljiArtikal(Artikal *niz, int max, int &lok)
{
	if (max < 0)
		return &niz[lok];
	float najskuplji = niz[max]._cijena;
	if (niz[max - 1]._cijena > najskuplji)
	{
		lok = max - 1;
		najskuplji = niz[max - 1]._cijena;
		return PronadjiNajskupljiArtikal(niz, max - 1, lok);
	}
	else
		return PronadjiNajskupljiArtikal(niz, max - 1, lok);
}
int BR = 1;
char *GenerisiSljedeciBrojRacuna()
{
	char *niz = new char[10];
	int brojracuna = BR, decimala = 0;
	strcpy_s(niz, 10, "000000000");
	while (brojracuna != 0)
	{
		brojracuna /= 10;
		decimala++;
	}
	_itoa_s(BR, &niz[9-decimala], 10, 10);
	BR++;
	return niz;
}
//1. PronadjiNajskupljiArtikal - rekurzivna funkcija koja vraca pokazivac na artikal sa najvecom ukupnom cijenom (cijena*kolicina)
//2. GenerisiSljedeciBrojRacuna - generise i vraca naredni broj racuna

void main() {
	//za eventualne nejasnoce analizirati testni program i ispise na slici
	Datum danas; danas.Unos(13, 6, 2016);
	Artikal cigarete, cokolada, sok;
	cigarete.Unos("Cigarete", 2.23);
	cokolada.Unos("Cokolada", 1.23);
	sok.Unos("Cappy", 2.10);
	Racun racun;
	racun.Unos(GenerisiSljedeciBrojRacuna(), danas);
	racun.DodajArtikal(cokolada, 5);
	racun.DodajArtikal(sok, 10);
	racun.DodajArtikal(cigarete, 2);
	racun.DodajArtikal(cigarete, 5);
	racun.Ispis();
	Artikal * p1 = racun.BinarnaPretraga("Cokolada");
	cout << "Binarnom pretragom pronadjen artikal -> ";
	p1->Ispis();
	racun.Ispis();
	int lok;
	Artikal * p2 = PronadjiNajskupljiArtikal(racun._artikli,racun._brojArtikala, lok);
	cout << "Rekurzivno pronadjen najskuplji artikal -> ";
	p2->Ispis();
	cout << endl;
	//dealocirati zauzetu memoriju
	

	system("PAUSE");
}
