#include<iostream>
#include<fstream>
using namespace std;

int BrIndeksa=1;

char * crt = "\n-----------------------------------------------------------------------------------\n";

struct Datum {
	int * _dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina<<endl ; }
	void Dealociraj() {
		delete _dan; delete _mjesec; delete _godina;
		_dan = _mjesec = _godina = nullptr;
	}
};

bool DatumIsti(Datum d1, Datum d2)
{
	if (d1._dan == d2._dan && d1._mjesec == d2._mjesec && d1._godina == d2._godina)
		return true;
	else
		return false;
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
	void Ispis() { _datumIzostanka.Ispis(); cout << _razlogIzostanka << " " << _razlogIzostanka; }
	void Opravdaj(char * razlogIzostanka) {
		if (_razlogIzostanka != nullptr)
			delete []_razlogIzostanka;
		int vel = strlen(razlogIzostanka) + 1;
		_razlogIzostanka = new char[vel];
		strcpy_s(_razlogIzostanka, vel, razlogIzostanka);
		_opravdano = true;
	}
};

char *GenerisiSljedeciBrojIndeksa()
{
	//2. GenerisiSljedeciBrojIndeksa - generise i vraca naredni broj indeksa

	char *temp=new char[9];
	strcpy_s(temp, strlen("IB140000") + 1, "IB140000");
	int brojCifara = 0;
	int brojIndeksa = BrIndeksa;

	while (brojIndeksa > 0)
	{
		brojCifara++;
		brojIndeksa /= 10;
	}

	_itoa_s(BrIndeksa, &temp[8 - brojCifara], 9, 10);//int(broj koji konvertujemo), lokacija u nizu odakle ce 
													//se poceti upisivati, buffer size, decimalni sistem(moze biti bin,oktalni...)

	return temp;

}


struct Student{
	//format broja indeksa: IB150051
	//osigurati automatsko generisanje broja indeksa pocevsi od IB150001
	char _brojIndeksa[9];
	char * _imePrezime;
	Izostanak * _izostanci;
	int _brojIzostanaka;

	void Unos(char *niz, char *ImeIPrezime)
	{
		//1. Unos

		strcpy_s(_brojIndeksa,strlen(GenerisiSljedeciBrojIndeksa())+1, GenerisiSljedeciBrojIndeksa());
		BrIndeksa++;

		_imePrezime = new char[strlen(ImeIPrezime) + 1];
		strcpy_s(_imePrezime, strlen(ImeIPrezime) + 1, ImeIPrezime);

		_izostanci = nullptr;
		_brojIzostanaka = 0;
	}

	

	void DodajIzostanak(Izostanak izostanak)
	{
		/*3. DodajIzostanak - dodaje novi izostanak u listu izostanaka.
		Onemoguciti ponavljanje izostanaka na nacin da se uvecava samo broj sati ukoliko
		korisnik (profesor) vise puta pokusa dodati izostanak na isti dan.
		*/
		bool istiDatum = false;

		if (_brojIzostanaka == 0)
		{
			_izostanci = new Izostanak;
			_izostanci->Unos(izostanak._datumIzostanka, izostanak._brojSati);
			_brojIzostanaka++;
			cout << "Izostanak uspjesno dodan!" << endl;
		}
		else
		{
			for (size_t i = 0; i < _brojIzostanaka; i++)
			{
				if (DatumIsti(izostanak._datumIzostanka, _izostanci[i]._datumIzostanka))
				{
					_izostanci[i]._brojSati += izostanak._brojSati;
					return;
				}
			}

			Izostanak *_izostanciTemp = new Izostanak[_brojIzostanaka];
			for (size_t i = 0; i < _brojIzostanaka; i++)
			{
				_izostanciTemp[i].Unos(_izostanci[i]._datumIzostanka, _izostanci[i]._brojSati);
				_izostanciTemp[i].Opravdaj(_izostanci[i]._razlogIzostanka);
			}

			delete _izostanci;
			_izostanci = new Izostanak[_brojIzostanaka + 1];
			for (size_t i = 0; i < _brojIzostanaka; i++)
			{
				_izostanci[i].Unos(_izostanciTemp[i]._datumIzostanka, _izostanciTemp[i]._brojSati);
				_izostanci[i].Opravdaj(_izostanciTemp[i]._razlogIzostanka);

			}

			_izostanci[_brojIzostanaka].Unos(izostanak._datumIzostanka, izostanak._brojSati);
			_brojIzostanaka++;
			cout << "Uspjesno dodan izostanak!" << endl;
		}
	}

	void Ispis()
	{
		//4. Ispis - ispisuje izostanke u formatu prikazanom na slici (nije obavezno da ispis bude identican, ali je svakako pozeljno).
		//Prilikom svakog ispisa, izostanke je potrebno spasiti u tekstualni fajl sa istim nazivom kao i broj indeksa.
		cout << crt << "Br. Indeksa          Ime i prezime\t\tDatum Izostanka\t\tBroj Sati " << crt;
		for (size_t i = 0; i < _brojIzostanaka; i++)
		{
			cout << _brojIndeksa << "             " << _imePrezime << "\t\t" << *_izostanci[i]._datumIzostanka._dan << "." <<
				*_izostanci[i]._datumIzostanka._mjesec << "." << *_izostanci[i]._datumIzostanka._godina<< "\t\t" << _izostanci[i]._brojSati << endl;
		}

		char NazivFajla[15];
		strcpy_s(NazivFajla, strlen(_brojIndeksa)+1, _brojIndeksa);
		strcat_s(NazivFajla, ".txt");

		ofstream Fajl(NazivFajla, ios::ate);
		Fajl << crt << "Br. Indeksa          Ime i prezime\t\tDatum Izostanka\t\tBroj Sati " << crt;
		for (size_t i = 0; i < _brojIzostanaka; i++)
		{
			Fajl << _brojIndeksa << "             " << _imePrezime << "\t\t"<< *_izostanci[i]._datumIzostanka._dan<<"."<<
				*_izostanci[i]._datumIzostanka._mjesec << "." << *_izostanci[i]._datumIzostanka._godina<< "\t\t" << _izostanci[i]._brojSati << endl;
		}
		Fajl.close();

	}
	

	Izostanak *BinarnaPretraga(Datum dat1)
	{

		//5. BinarnaPretraga - koristeci binarnu pretragu, na osnovu datuma, pronalazi i vraca pokazivac na izostanak. 
		//Ukoliko trazeni izostanak ne postoji funkcija vraca nullptr.
		int prvi = 0;
		int zadnji = _brojIzostanaka-1;
		bool pronadjen = false;
		int srednji;

		cout << "Sortiram...." << endl;
		//bubble sort!
		/*int N = _brojIzostanaka;
		int indikatorZamjene = 0;
		do
		{
			indikatorZamjene = 1;
			for (size_t i = 0; i < N-1; i++)
			{
				if (*_izostanci[i]._datumIzostanka._dan>*_izostanci[i + 1]._datumIzostanka._dan &&
					*_izostanci[i]._datumIzostanka._mjesec >= *_izostanci[i + 1]._datumIzostanka._mjesec)
				{
					Izostanak *spremi = new Izostanak;
					spremi->Unos(_izostanci[i]._datumIzostanka, _izostanci[i]._brojSati);
					spremi->Opravdaj(_izostanci[i]._razlogIzostanka);

					_izostanci[i].Dealociraj();
					_izostanci[i].Unos(_izostanci[i + 1]._datumIzostanka, _izostanci[i + 1]._brojSati);
					_izostanci[i].Opravdaj(_izostanci[i + 1]._razlogIzostanka);

					_izostanci[i + 1].Dealociraj();
					_izostanci[i + 1].Unos(spremi->_datumIzostanka, spremi->_brojSati);
					_izostanci[i + 1].Opravdaj(spremi->_razlogIzostanka);

					spremi->Dealociraj();
					delete spremi;

					indikatorZamjene = 0;

				}
				
			}
			N = N - 1;
		} while (N>1 && indikatorZamjene==0);*/


		for (size_t i = 0; i <= _brojIzostanaka-1; i++)//sortiranje
		{
			for (size_t j = i+1; j <_brojIzostanaka; j++)
			{
				if (*_izostanci[j]._datumIzostanka._dan < *_izostanci[i]._datumIzostanka._dan &&
					*_izostanci[j]._datumIzostanka._mjesec<= *_izostanci[i]._datumIzostanka._mjesec)
				{
					Izostanak *temp = new Izostanak;
					temp->Unos(_izostanci[i]._datumIzostanka, _izostanci[i]._brojSati);
					temp->Opravdaj(_izostanci[i]._razlogIzostanka);

					_izostanci[i].Dealociraj();

					_izostanci[i].Unos(_izostanci[j]._datumIzostanka, _izostanci[j]._brojSati);
					_izostanci[i].Opravdaj(_izostanci[j]._razlogIzostanka);

					_izostanci[j].Dealociraj();
					_izostanci[j].Unos(temp->_datumIzostanka, temp->_brojSati);
					_izostanci[j].Opravdaj(temp->_razlogIzostanka);
					delete temp;
				}
			}
		
		}
	
		for (size_t i = 0; i < _brojIzostanaka; i++)
			_izostanci[i].Ispis();

		while (pronadjen==false && prvi<=zadnji)
		{
			srednji = (prvi + zadnji) / 2;

			if (*dat1._dan < *_izostanci[srednji]._datumIzostanka._dan && *dat1._mjesec<=*_izostanci[srednji]._datumIzostanka._mjesec)
				zadnji = srednji + 1;

			if (*dat1._dan == *_izostanci[srednji]._datumIzostanka._dan && *dat1._mjesec==*_izostanci[srednji]._datumIzostanka._mjesec)
			{
				pronadjen = true;
				return &_izostanci[srednji];
			}

			if (*dat1._dan>*_izostanci[srednji]._datumIzostanka._dan && *dat1._mjesec>=*_izostanci[srednji]._datumIzostanka._mjesec)
				prvi = srednji + 1;
		}

		return nullptr;
	}
			
};




Izostanak *PronadjiNajveciNeopravdaniIzostanak(Student s1, int max, int &lok)
{
	//1. PronadjiNajveciNeopravdaniIzostanak - rekurzivna funkcija koja vraca pokazivac na neopravdani izostanak koji ima najveci broj sati
	Izostanak *najveci = &s1._izostanci[lok];

	if (max <=0)
		return najveci;
	if (max >= 0 && s1._izostanci[max-1]._brojSati> najveci->_brojSati)
	{
		lok = max-1;
		return PronadjiNajveciNeopravdaniIzostanak(s1, max - 1, lok);
	}
	else
	{
		return PronadjiNajveciNeopravdaniIzostanak(s1, max - 1, lok);
	}


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
	izostanakPrije5Dana.Unos(prije5Dana, 2);
	denis.DodajIzostanak(izostanakPrije5Dana);

	izostanakPrije10Dana.Unos(prije10Dana, 1);
	denis.DodajIzostanak(izostanakPrije10Dana);
	izostanakPrije10Dana.Unos(prije10Dana, 1);
	denis.DodajIzostanak(izostanakPrije10Dana);
	
	denis.Ispis();
	Izostanak * p = denis.BinarnaPretraga(jucer);
	cout << "Binarnom pretragom pronadjen izostanak -> ";
	p->Ispis();

	int lok=0;
	Izostanak * p2 = PronadjiNajveciNeopravdaniIzostanak(denis,denis._brojIzostanaka,lok);
	cout << "Rekurzivno pronadjen najveci neopravdani izostanak -> ";
	p2->Ispis();
	cout << endl;
	//dealocirati zauzetu memoriju

	system("pause");
}
