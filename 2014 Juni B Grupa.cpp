#include<iostream>
#include<fstream>
using namespace std;

char * crt = "\n----------------------------------------------------\n";
char * crt2 = "-------------------------------";
float BrojRacuna =6621166;

//Potpisi funkcija trebaju odgovarati onima u main-u
//1. Unos
//2. Dealociraj
//3. DodajArtikal - dodaje novi artikal u listu artikala zajedno sa njegovom kolicinom.
//Onemoguciti ponavljanje artikala na nacin da se uvecava samo kolicina ukoliko
//korisnik vise puta pokusa dodati isti artikal.
////4. Ispis - ispisuje racun u formatu prikazanom na slici (nije obavezno da bude
//identican, ali je svakako pozeljno).Prilikom svakog ispisa, racun
//je potrebno spasiti u tekstualni fajl sa istim nazivom kao i broj racuna.
////5. BinarnaPretraga - koristeci binarnu pretragu, na osnovu naziva, pronalazi i vraca
//pokazivac na artikal.Ukoliko trazeni artikal ne postoji funkcija vraca nullptr.
//1. PronadjiNajskupljiArtikal - rekurzivna funkcija koja vraca pokazivac na artikal sa najvecom ukupnom cijenom (cijena*kolicina)
//2. GenerisiSljedeciBrojRacuna - generise i vraca naredni broj racuna


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
	void Ispis() { cout << _naziv <<"  " << _cijena << " KM"; }

	

};
char *GenerisiSljedeciBrojRacuna()
{
	//format broja racuna: 006005491
	//osigurati automatsko generisanje broja racuna pocevsi od 000000001
	char *temp = new char[10];
	strcpy_s(temp, strlen("RN0000000") + 1, "RN0000000");
	int brojacDecimala = 0;

	int brojRacuna = BrojRacuna;
	while (brojRacuna > 0)
	{
		brojRacuna /= 10;
		brojacDecimala++;
	}
	_itoa_s(BrojRacuna, &temp[9-brojacDecimala],strlen(temp)+1, 10);
	
	BrojRacuna++;
	return temp;	
}

struct Racun {
	
	char _brojRacuna[10];
	Datum _datumKreiranja;
	Artikal * _artikli;
	int * _kolicine;//cuva informaciju o kolicini svakog artikla na racunu
	int _brojArtikala;


	void Unos(char *niz, Datum dat)
	{
		strcpy_s(_brojRacuna, 10, niz);

		_datumKreiranja.Unos(*dat._dan, *dat._mjesec, *dat._godina);

		_brojArtikala = 0;

	}

	void ispis()
	{
		cout << crt << "Broj racuna: " << _brojRacuna << "\t\t";
		cout << "Datum: ";
		_datumKreiranja.Ispis();
		cout << crt;
		for (size_t i = 0; i < _brojArtikala; i++)
		{
			cout << _artikli[i]._naziv << " x" << _kolicine[i] << crt2 << _artikli[i]._cijena*_kolicine[i] << "KM" << endl;
		}
	}

	void DodajArtikal(Artikal art, int kol)
	{
		// 3. DodajArtikal - dodaje novi artikal u listu artikala zajedno sa njegovom kolicinom.
		//Onemoguciti ponavljanje artikala na nacin da se uvecava samo kolicina ukoliko
		//korisnik vise puta pokusa dodati isti artikal.
		if (_brojArtikala == 0)
		{
			_artikli = new Artikal;
			_artikli->Unos(art._naziv, art._cijena);
			_kolicine = new int(kol);
			_brojArtikala++;
			cout << "Artikal uspjesno dodan" << endl;

		}
		else
		{
			if (_brojArtikala > 0){

				int lokacija = 0;
				bool pronadjen = false;

				for (size_t i = 0; i < _brojArtikala; i++){
					if (strcmp(art._naziv, _artikli[i]._naziv) == 0 || art._cijena == _artikli[i]._cijena)
					{
						pronadjen = true;
						lokacija = i;
						cout << "Isti pronadjen!" << endl;
						break;
					}

				}



				if (pronadjen == true){
					_kolicine[lokacija] += kol;
					return;
				}
				else
				{
					Artikal *artTemp = new Artikal[_brojArtikala];

					for (size_t i = 0; i < _brojArtikala; i++)
						artTemp[i].Unos(_artikli[i]._naziv, _artikli[i]._cijena);

					delete _artikli;
					_artikli = new Artikal[_brojArtikala + 1];

					for (size_t i = 0; i < _brojArtikala; i++)
						_artikli[i].Unos(artTemp[i]._naziv, artTemp[i]._cijena);


					int *kolTemp = new int[_brojArtikala];
					for (size_t i = 0; i < _brojArtikala; i++)
						kolTemp[i] = _kolicine[i];

					delete _kolicine;
					_kolicine = new int[_brojArtikala + 1];

					for (size_t i = 0; i < _brojArtikala; i++)
						_kolicine[i] = kolTemp[i];

					_artikli[_brojArtikala].Unos(art._naziv, art._cijena);
					_kolicine[_brojArtikala] = kol;
					_brojArtikala++;
					cout << "Artikal uspjesno dodan!" << endl;

				}
			}
		}
	}



	Artikal *BinarnaPretraga(char *pretraga)
	{
		int prvi = 0;
		int zadnji = _brojArtikala - 1;
		int srednji;
		bool pronadjen = false;


		Artikal *temp = new Artikal[_brojArtikala + 1];
		for (size_t i = 1; i <= _brojArtikala; i++)
		{
			temp[i] = _artikli[i - 1];
		}


		//sortiranje po nazivu(bubble sort)
		for (size_t i = 0; i < _brojArtikala - 1; i++)
		{
			for (size_t j = i + 1; j < _brojArtikala; j++)
			{
				if (strcmp(_artikli[j]._naziv, _artikli[i]._naziv) < 0)
				{
					Artikal temp;
					temp.Unos(_artikli[i]._naziv, _artikli[i]._cijena);
					_artikli[i].Dealociraj();

					_artikli[i].Unos(_artikli[j]._naziv, _artikli[j]._cijena);

					_artikli[j].Dealociraj();
					_artikli[j].Unos(temp._naziv, temp._cijena);
				}
			}
		}

		cout << "Sortirano: " << endl;
		for (size_t i = 0; i < _brojArtikala; i++)
		{
			_artikli[i].Ispis();
			cout << endl;
		}

		//binary search
		while ((pronadjen == false) && prvi <= zadnji)
		{
			srednji = (prvi + zadnji) / 2;

			if (strcmp(pretraga, _artikli[srednji]._naziv) < 0)
				zadnji = srednji - 1;
			if (strcmp(_artikli[srednji]._naziv, pretraga) == 0)
				return &_artikli[srednji];
			if (strcmp(pretraga, _artikli[srednji]._naziv)>0)
				prvi = srednji + 1;
		}


	}


	void IspisTxt()
	{
		char NazivFajla[20];
		strcpy_s(NazivFajla, _brojRacuna);
		strcat_s(NazivFajla, ".txt");

		ofstream Fajl(NazivFajla,ios::ate);

		Fajl << crt << "Broj racuna: " << _brojRacuna << "\t\t";
		Fajl << "Datum: ";
		Fajl<<*_datumKreiranja._dan<<"."<<*_datumKreiranja._mjesec<<"."<<*_datumKreiranja._godina;
		Fajl << crt;
		for (size_t i = 0; i < _brojArtikala; i++)
		{
			Fajl << _artikli[i]._naziv << " x" << _kolicine[i] << crt2 << _artikli[i]._cijena*_kolicine[i] << "KM" << endl;
		}		
		Fajl.close();

		cout << "Uspjesno upisano u fajl-->" << NazivFajla << endl;
	}
};



Artikal *PronadjiNajskupljiArtikal(Artikal *niz, int max,int &lok)
{
	float najskuplji = niz[max]._cijena;
	if (max == -1)
	{
		return &niz[lok];
	}
	if (najskuplji < niz[max - 1]._cijena)
	{
		lok = max-1;
		najskuplji = niz[max-1]._cijena;
		return PronadjiNajskupljiArtikal(niz, max - 1,lok);
	}
	else
		return PronadjiNajskupljiArtikal(niz, max - 1,lok);
	

}



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
	racun.DodajArtikal(cigarete, 2);
	racun.DodajArtikal(sok, 10);
	racun.DodajArtikal(cigarete, 5);

	
	racun.ispis();

	Artikal * p1 = racun.BinarnaPretraga("Cokolada");
	cout << "Binarnom pretragom pronadjen artikal -> ";
	p1->Ispis();
	cout << endl;

	int lok = 0;
	Artikal * p2 = PronadjiNajskupljiArtikal(racun._artikli,racun._brojArtikala,lok);
	cout << "Rekurzivno pronadjen najskuplji artikal -> ";
	p2->Ispis();
	cout << endl;

	racun.IspisTxt();
	
	system("pause");
}
