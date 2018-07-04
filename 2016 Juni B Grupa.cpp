#include <iostream>
using namespace std;

//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

enum enumKursevi { HtmlCSSJavaScript, SoftwareEngeneeringFundamentals, MasteringSQL, WindowsSecurity };
char *charKursevi[] = { "HtmlCSSJavaScript", "SoftwareEngeneeringFundamentals", "MasteringSQL", "WindowsSecurity" };
char *crt = "\n--------------------------------------------\n";

bool Prestupna(int g) {
	return (g % 400 == 0 || g % 100 != 0) && g % 4 == 0;
}
struct Datum {
	int * _dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	bool Prije(Datum d)
	{
		if (*_godina < *d._godina)
			return 1;
		if (*_godina == *d._godina) {
			if (*_mjesec < *d._mjesec)
				return 1;
			if (*_mjesec == *d._mjesec) {
				if (*_dan < *d._dan)
					return 1;
			}
		}
		return 0;
	}

	bool Poslije(Datum d) {
		return !Prije(d);
	}

	int VratiBrojDana() {
		int MjesecDani[12] = { 31, 28 + Prestupna(*_godina), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

		int Ukupno = *_dan;
		for (int i = *_mjesec - 2; i >= 0; i--)
			Ukupno += MjesecDani[*_mjesec];

		for (int i = *_godina - 1; i >= 1920; i--)
			Ukupno += Prestupna(i) ? 366 : 365;

		return Ukupno;

	}
	int RazlikuDana(Datum d) {
		return abs(VratiBrojDana() - d.VratiBrojDana());
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina; }
	void Dealociraj() { delete _dan; delete _mjesec; delete _godina; }
};

struct Kurs {
	enumKursevi _kurs;
	Datum _pocetak;
	Datum _kraj;
	char * _imePredavaca;
	bool _aktivan;//SVAKI KURS JE NA POCETKU AKTIVAN

	void Unos(enumKursevi kurs, Datum d1, Datum d2, char *predavac) {
		int size = strlen(predavac) + 1;
		_imePredavaca = new char[size];
		strcpy_s(_imePredavaca, size, predavac);
		_pocetak.Unos(*d1._dan, *d1._mjesec, *d1._godina);
		_kraj.Unos(*d2._dan, *d2._mjesec, *d2._godina);
		_aktivan = true;
		_kurs = kurs;
	}
	void Ispis() {
		cout << charKursevi[_kurs] << " - ";
		_pocetak.Ispis();
		cout << " - ";
		_kraj.Ispis(); 
		cout << " - "  << endl;
	}
	void Dealociraj() {
		delete[] _imePredavaca; _imePredavaca = nullptr;
	}
};
struct Polaznik {
	int _polaznikID;
	char * _imePrezime;
	void Unos(int polaznikID, char * imePrezime) {
		_polaznikID = polaznikID;
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
	}
	void Ispis() { cout << _polaznikID << " " << _imePrezime << endl; }
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
	}
};
struct Polaganja {
	Polaznik _polaznik;
	Kurs _kurs;
	Datum _datumPolaganja;
	float _ostvareniUspjeh;
	void Unos(Polaznik p, Kurs k, Datum d, float uspjeh) {
		_ostvareniUspjeh = uspjeh;
		_polaznik.Unos(p._polaznikID, p._imePrezime);
		_kurs.Unos(k._kurs, k._pocetak, k._kraj, k._imePredavaca);
		_datumPolaganja.Unos(*d._dan, *d._mjesec, *d._godina);
	}
	void Ispis() {

		cout << _polaznik._imePrezime << " - " << charKursevi[_kurs._kurs] << " - ";
		_datumPolaganja.Ispis();
		cout << " - " << _ostvareniUspjeh << "% " << endl;
	}

};
struct SkillsCentar {
	char * _nazivCentra;
	Kurs * _kursevi[50];//JEDAN EDUKACIJSKI CENTAR MOZE NUDITI NAJVISE 50 KURSEVA
	int _trenutnoKurseva;
	Polaganja * _polaganja;
	int _trenutnoPolaganja;

	void Unos(char* naziv) {
		int vel = strlen(naziv) + 1;
		_nazivCentra = new char[vel];
		strcpy_s(_nazivCentra, vel, naziv);
		_trenutnoKurseva = 0;
		_trenutnoPolaganja = 0;
		_polaganja = nullptr;
		for (int i = 0; i < 50; i++)
		{
			_kursevi[i] = nullptr;
		}
	}

	bool DodajKurs(Kurs kurs) {

		for (int i = _trenutnoKurseva - 1; i >= 0; i--)
		{
			if (_kursevi[i]->_kurs == kurs._kurs)
			{
				if (!kurs._pocetak.Poslije(_kursevi[i]->_kraj))
					return 0;
				break;
			}
		}

		_kursevi[_trenutnoKurseva] = new Kurs;
		_kursevi[_trenutnoKurseva]->Unos(kurs._kurs, kurs._pocetak, kurs._kraj, kurs._imePredavaca);
		_trenutnoKurseva++;
		return 1;
	}
	void Ispis() {
		cout << "Naziv centra: " << _nazivCentra << endl;
		int BrojacKurseva = 1;
		for (int i = 0; i < _trenutnoKurseva; i++)
		{

			if (_kursevi[i]->_aktivan) {
				cout << BrojacKurseva++ << ". ";
				_kursevi[i]->Ispis();
			}
		}
		for (int i = 0; i < _trenutnoPolaganja; i++)
		{
			cout << i + 1 << ". ";
			_polaganja[i].Ispis();
		}
	}
	//nije moguce dodati polaganje onog kursa koji nije evidentiran (registrovan, ponudjen) u tom edukacijskom centru
	//moguce je polagati samo aktivne kurseve i to najkasnije 5 dana od dana zavrsetka (kraja) posljednje dodanog (aktivnog) 
	//kursa iste vrste (npr. MasteringSQL)
	//jedan polaznik moze vise puta polagati isti kurs, ali novi procenat mora biti veci od prethodno dodanog 
	//(na istom kursu)

	bool DodajPolaganje(Polaganja polaganje) {


		for (int i = _trenutnoKurseva - 1; i >= 0; i--)
		{
			if (_kursevi[i]->_kurs == polaganje._kurs._kurs && _kursevi[i]->_aktivan) {

				if (polaganje._datumPolaganja.RazlikuDana(_kursevi[i]->_kraj) < 5)
					return false;
				break;
			}
		}
		for (int i = _trenutnoPolaganja - 1; i >= 0; i--)
		{
			if (strcmp(_polaganja[i]._polaznik._imePrezime, polaganje._polaznik._imePrezime) == 0 &&
				_polaganja[i]._polaznik._polaznikID == polaganje._polaznik._polaznikID &&
				_polaganja[i]._kurs._kurs == polaganje._kurs._kurs) {
				if (_polaganja[i]._ostvareniUspjeh < polaganje._ostvareniUspjeh)
					return 0;
				break;
			}
		}
		Polaganja *temp = new Polaganja[_trenutnoPolaganja + 1];

		for (int i = 0; i < _trenutnoPolaganja; i++)
		{
			temp[i].Unos(_polaganja[i]._polaznik, _polaganja[i]._kurs, _polaganja[i]._datumPolaganja, _polaganja[i]._ostvareniUspjeh);
		}
		delete[] _polaganja;

		_polaganja = temp;
		_polaganja[_trenutnoPolaganja].Unos(polaganje._polaznik, polaganje._kurs, polaganje._datumPolaganja, polaganje._ostvareniUspjeh);
		_trenutnoPolaganja++;
		return 1;
	}

	Polaganja *PolaganjaByDatum(Datum d1, Datum d2, int &broj)
	{
		for (int i = 0; i < _trenutnoPolaganja; i++)
		{
			if (_polaganja[i]._datumPolaganja.Poslije(d1) && _polaganja[i]._datumPolaganja.Prije(d2))
				broj++;
		}

		Polaganja * temp = new Polaganja[broj];

		for (int i = 0; i < _trenutnoPolaganja; i++)
		{
			if (_polaganja[i]._datumPolaganja.Poslije(d1) && _polaganja[i]._datumPolaganja.Prije(d2))
				temp[i].Unos(_polaganja[i]._polaznik, _polaganja[i]._kurs, _polaganja[i]._datumPolaganja, _polaganja[i]._ostvareniUspjeh);

		}
		return temp;
	}
	void Dealociraj() {
		delete[] _nazivCentra; _nazivCentra = nullptr;
		for (int i = 0; i < _trenutnoKurseva; i++)
		{
			_kursevi[i]->Dealociraj();
		}

		delete[] _polaganja; _polaganja = nullptr;
	}
};

float PretragaRekurzivno(int godina, enumKursevi kurs, SkillsCentar centar, int brojac = 0, int zadovoljavaju = 0) {
	if (centar._trenutnoPolaganja == brojac)
		return 0;
	if (centar._polaganja[brojac]._kurs._kurs == kurs && godina == *centar._polaganja[brojac]._datumPolaganja._godina) {
		zadovoljavaju++;
		return (centar._polaganja[brojac]._ostvareniUspjeh + PretragaRekurzivno(godina, MasteringSQL, centar, brojac + 1, zadovoljavaju)
			) / zadovoljavaju;
	}
	PretragaRekurzivno(godina, kurs, centar, brojac + 1, zadovoljavaju);
}

void main() {

	/*
	1. BROJ I TIPOVI PARAMETARA MORAJU BITI IDENTICNI ONIMA U TESTNOM CODE-U (OSIM UKOLIKO IMATE OPRAVDANU POTREBU ZA MODIFIKACIJOM). U SUPROTNOM SE RAD NEĆE BODOVATI
	2. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE. TAKOĐER, UKLONITE SVE DIJELOVE CODE-A KOJI NISU IMPLEMENTIRANI
	3. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
	4. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ĆETE KOPIRATI VAŠA RJEŠENJA)
	5. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAĐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
	*/

	Datum datum1, datum2, datum3, datum4, datum5, datum6;
	datum1.Unos(26, 11, 2015);
	datum2.Unos(29, 5, 2016);
	datum3.Unos(5, 6, 2016);
	datum4.Unos(15, 8, 2016);
	datum5.Unos(13, 7, 2016);
	datum6.Unos(22, 9, 2016);

	Kurs softverski, html, sql, security;
	softverski.Unos(SoftwareEngeneeringFundamentals, datum1, datum2, "Emina Junuz");
	html.Unos(HtmlCSSJavaScript, datum2, datum3, "Larisa Tipura");
	sql.Unos(MasteringSQL, datum3, datum4, "Jasmin Azemovic");
	security.Unos(WindowsSecurity, datum3, datum4, "Adel Handzic");

	Polaznik denis, zanin, indira;
	denis.Unos(1, "Denis Music");
	zanin.Unos(2, "Zanin Vejzovic");
	indira.Unos(3, "Indira Hamulic");

	Polaganja denisHtml, zaninHtml, indiraSql;
	denisHtml.Unos(denis, html, datum5, 61);
	//61 predstavlja ostvareni uspjeh/procenat, a uspjesno polozenim se smatra svaki kurs na kome je polaznik ostvari vise od 55%
	zaninHtml.Unos(zanin, html, datum6, 93);

	SkillsCentar mostar;
	mostar.Unos("Skills Center Mostar");
	//jedan edukacijski centar moze nuditi vise istih kurseva (npr. MasteringSQL), ali se oni moraju realizovati u 
	//razlictim periodima (pocetak-kraj) tj. ne smiju se preklapati
	//novi kurs istog tipa (npr. MasteringSQL) se mora realizovati nakon prethodno dodanog npr. ako prethodni kurs
	//MasteringSQL traje 01.03.2016 - 01.04.2016, onda se novi kurs MasteringSQL moze dodati jedino ako pocinje nakon 01.04.2016
	if (mostar.DodajKurs(softverski)) {
		cout << crt << "KURS USPJESNO REGISTROVAN" << crt;
		softverski.Ispis();
		cout << crt;
	}
	if (mostar.DodajPolaganje(denisHtml)) {
		cout << crt << "POLAGANJE EVIDENTIRANO" << crt;
		denisHtml.Ispis();
	}

	Datum OD, DO;
	int brojPolaganja = 0;
	OD.Unos(1, 6, 2016);
	DO.Unos(1, 8, 2016);
	//PolaganjaByDatum – vraca niz polaganja koja su uspjesno realizovana u periodu OD-DO
	Polaganja * polaganjaByDatum = mostar.PolaganjaByDatum(OD, DO, brojPolaganja);
	cout << "U periodu od ";
	OD.Ispis();
	cout << " do ";
	DO.Ispis();
	cout << " uspjesno je realizovano " << brojPolaganja << " polaganja--->>>";
	for (size_t i = 0; i < brojPolaganja; i++)
		polaganjaByDatum[i].Ispis();

	//PretragaRekurzivno - rekurzivna funkcija pronalazi prosjecni uspjeh koji su polaznici tokom godine (npr.2016) ostvarili na odredjenom kursu (npr. MasteringSQL)
	cout << "Prosjecan uspjeh na kursu MasteringSQL u 2016 godini je " << PretragaRekurzivno(2016, HtmlCSSJavaScript, mostar) << endl;
	//Ispis - ispisuje sve informacije o edukacijskom centru. prije ispisa sortirati sve kurseve na osnovu pocetka odrzavanja kursa
	mostar.Ispis();

	mostar.Dealociraj();


	denis.Dealociraj();
	indira.Dealociraj();
	zanin.Dealociraj();

	softverski.Dealociraj();
	html.Dealociraj();
	sql.Dealociraj();
	security.Dealociraj();

	datum1.Dealociraj();
	datum2.Dealociraj();
	datum3.Dealociraj();
	datum4.Dealociraj();
	datum5.Dealociraj();
	datum6.Dealociraj();

	////izvrsiti potrebne dealokacije
	system("pause");
}
