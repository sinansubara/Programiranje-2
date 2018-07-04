#include<iostream>
#include<iomanip>
#include<fstream>
using namespace std;
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

enum enumKursevi { HtmlCSSJavaScript, SoftwareEngeneeringFundamentals, MasteringSQL, WindowsSecurity };
char *enumkurseviTEXT[]={ "HtmlCSSJavaScript", "SoftwareEngeneeringFundamentals", "MasteringSQL", "WindowsSecurity" };
char *crt = "\n--------------------------------------------\n";

struct Datum {
	int * _dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() { 
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	int sumaDana() {
		bool prestupna = ((*_godina % 400 == 0 || *_godina % 100 != 0) && *_godina % 4 == 0);
		int suma = 0, brDana[]{ 31,28 + prestupna,31,30,31,30,31,31,30,31,30,31 };
		for (int i = 1900; i < *_godina; i++) {
			if ((i % 4 == 0 && i % 100 != 0) || i % 400 == 0)
				suma += 366;
			else suma += 365;
		}
		for (int i = 1; i < *_mjesec; i++) {
			suma += brDana[i - 1];
		}
		return suma += *_dan;
	}
	bool poslije(Datum OD)
	{
		if (*_godina > *OD._godina)
			return true;
		if (*_godina == *OD._godina)
		{
			if (*_mjesec > *OD._mjesec)
				return true;
			if (*_mjesec == *OD._mjesec)
			{
				if (*_dan > *OD._dan)
					return true;
			}
		}
		return false;
	}
	bool prije(Datum DO)
	{
		if (*_godina < *DO._godina)
			return true;
		if (*_godina == *DO._godina)
		{
			if (*_mjesec < *DO._mjesec)
				return true;
			if (*_mjesec == *DO._mjesec)
			{
				if (*_dan < *DO._dan)
					return true;
			}
		}
		return false;
	}
};
bool PorediTermine(Datum kraj, Datum pocetak)
{
	if ((pocetak.sumaDana() - kraj.sumaDana()) < 0 || (pocetak.sumaDana() - kraj.sumaDana()) >5)
		return false;
	return true;
}
bool Ispravan(Datum kraj, Datum pocetak)
{
	if (kraj._godina < pocetak._godina)
		return true;
	if (kraj._godina == pocetak._godina)
	{
		if (kraj._mjesec < pocetak._mjesec)
			return true;
		if (kraj._mjesec == pocetak._mjesec)
		{
			if (kraj._dan <= pocetak._dan)
				return true;
		}
	}
	return false;
}


struct Kurs {
	enumKursevi _kurs;
	Datum _pocetak;
	Datum _kraj;
	char * _imePredavaca;
	bool _aktivan;//SVAKI KURS JE NA POCETKU AKTIVAN
	void Unos(enumKursevi kurs, Datum pocetak, Datum kraj, char * imepredavaca)
	{
		_kurs = kurs;
		_pocetak.Unos(*pocetak._dan, *pocetak._mjesec, *pocetak._godina);
		_kraj.Unos(*kraj._dan, *kraj._mjesec, *kraj._godina);
		int size = strlen(imepredavaca) + 1;
		_imePredavaca = new char[size];
		strcpy_s(_imePredavaca, size, imepredavaca);
		_aktivan = true;
	}
	void Ispis()
	{
		cout << "Kurs: " << enumkurseviTEXT[_kurs] << endl;
		cout << "Ime predavaca: " << _imePredavaca << endl;
		cout << "Pocetak: "; _pocetak.Ispis();
		cout << "Kraj: "; _kraj.Ispis();
		cout << "Aktivan: " << _aktivan << endl;
	}
	void Dealociraj() 
	{
		_pocetak.Dealociraj();
		_kraj.Dealociraj();
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
	void Ispis() { cout <<"Polaznik ID: "<< _polaznikID << " Ime prezime: " << _imePrezime << endl; }
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
	}
};
struct Polaganja {
	Polaznik _polaznik;
	Kurs _kurs;
	Datum _datumPolaganja;
	float _ostvareniUspjeh;
	void Unos(Polaznik polaznik, Kurs kurs, Datum datumpolaganja, float ostvareniuspjeh)
	{
		_polaznik.Unos(polaznik._polaznikID, polaznik._imePrezime);
		_kurs.Unos(kurs._kurs, kurs._pocetak, kurs._kraj, kurs._imePredavaca);
		_datumPolaganja.Unos(*datumpolaganja._dan, *datumpolaganja._mjesec, *datumpolaganja._godina);
		_ostvareniUspjeh = ostvareniuspjeh;
	}
	void Ispis()
	{
		_polaznik.Ispis();
		_kurs.Ispis();
		cout << "Datum: "; _datumPolaganja.Ispis();
		cout << "Ostvareni uspjeh: " << _ostvareniUspjeh << "%" << endl;
	}
	void Dealociraj()
	{
		_datumPolaganja.Dealociraj();
		_kurs.Dealociraj();
		_polaznik.Dealociraj();	
	}
};
struct SkillsCentar {
	char * _nazivCentra;
	Kurs * _kursevi[50];//JEDAN EDUKACIJSKI CENTAR MOZE NUDITI NAJVISE 50 KURSEVA
	int _trenutnoKurseva;
	Polaganja * _polaganja;
	int _trenutnoPolaganja;
	void Unos(char *nazivcentra)
	{
		int size = strlen(nazivcentra) + 1;
		_nazivCentra = new char[size];
		strcpy_s(_nazivCentra, size, nazivcentra);
		_trenutnoKurseva = 0;
		_trenutnoPolaganja = 0;
		_polaganja = nullptr;
	}
	void Dealociraj()
	{
		delete[] _nazivCentra; _nazivCentra = nullptr;
		for (int i = 0; i < _trenutnoKurseva; i++)
		{
			_kursevi[i]->Dealociraj();
		}
		
		for (int i = 0; i < _trenutnoPolaganja; i++)
		{
			_polaganja[i].Dealociraj();
		}
		delete[] _polaganja; _polaganja = nullptr;
	}
	void Ispis()
	{
		cout << crt << crt << "  ===NAZIV CENTRA: " << _nazivCentra <<"==="<< crt << crt;
		int brojacaktivnihkurseva = 1;
		int brojacneaktivnihkurseva = 1;
		for (int i = 0; i < _trenutnoKurseva-1; i++)
		{
			for (int j = i+1; j < _trenutnoKurseva; j++)
			{
				if (_kursevi[i]->_pocetak.sumaDana() > _kursevi[j]->_pocetak.sumaDana())
				{
					swap(_kursevi[i], _kursevi[j]);
				}
			}
		}
		cout << "AKTIVNI KURSEVI: " << crt;
		for (int i = 0; i < _trenutnoKurseva; i++)
		{
			if (_kursevi[i]->_aktivan)
			{
				cout << brojacaktivnihkurseva++ << ". ";
				_kursevi[i]->Ispis();
				cout << crt;
			}
		}
		cout << crt;
		cout << "NEAKTIVNI KURSEVI: " << crt;
		for (int i = 0; i < _trenutnoKurseva; i++)
		{
			if (_kursevi[i]->_aktivan==false)
			{
				cout << brojacneaktivnihkurseva++ << ". ";
				_kursevi[i]->Ispis();
				cout << crt;
			}
		}
		cout << crt;
		cout << "POLAGANJA: " << crt;
		for (int i = 0; i < _trenutnoPolaganja; i++)
		{
			cout << i + 1 << ". ";
			_polaganja[i].Ispis();
			cout << crt;
		}
			
		

	}
	bool DodajKurs(Kurs kurs)
	{
		if (_trenutnoKurseva == 50)
			return false;

		for (int i = 0; i < _trenutnoKurseva; i++)
		{
			if (_kursevi[i]->_kurs == kurs._kurs)
			{
				if (Ispravan(_kursevi[i]->_kraj, kurs._pocetak))
				{
					_kursevi[_trenutnoKurseva] = new Kurs;
					_kursevi[_trenutnoKurseva]->Unos(kurs._kurs, kurs._pocetak, kurs._kraj, kurs._imePredavaca);
					_trenutnoKurseva++;
					return true;
				}
			}
		}
		_kursevi[_trenutnoKurseva] = new Kurs;
		_kursevi[_trenutnoKurseva]->Unos(kurs._kurs, kurs._pocetak, kurs._kraj, kurs._imePredavaca);
		_trenutnoKurseva++;
		return true;
	}
	bool DodajPolaganje(Polaganja polaganje)
	{
		for (int i = 0; i < _trenutnoKurseva; i++)
		{
			if (_kursevi[i]->_kurs == polaganje._kurs._kurs&&_kursevi[i]->_aktivan)
			{
				if (PorediTermine(_kursevi[i]->_kraj, polaganje._kurs._pocetak)==false)
					return false;
			}
		}
		for (int i = 0; i < _trenutnoPolaganja; i++)
		{
			if ((strcmp(_polaganja[i]._polaznik._imePrezime, polaganje._polaznik._imePrezime) == 0) && (_polaganja[i]._polaznik._polaznikID == polaganje._polaznik._polaznikID) && (_polaganja[i]._kurs._kurs == polaganje._kurs._kurs))
			{
				if (_polaganja[i]._ostvareniUspjeh > polaganje._ostvareniUspjeh)
					return false;
			}

		}
		Polaganja *temp = new Polaganja[_trenutnoPolaganja + 1];
		for (int i = 0; i < _trenutnoPolaganja; i++)
		{
			temp[i].Unos(_polaganja[i]._polaznik, _polaganja[i]._kurs, _polaganja[i]._datumPolaganja, _polaganja[i]._ostvareniUspjeh);
			_polaganja[i].Dealociraj();
		}
		delete[] _polaganja;
		_polaganja = temp;
		_polaganja[_trenutnoPolaganja].Unos(polaganje._polaznik, polaganje._kurs, polaganje._datumPolaganja, polaganje._ostvareniUspjeh);
		_trenutnoPolaganja++;
		return true;

	}
	Polaganja *PolaganjaByDatum(Datum OD, Datum DO, int &brojacpolaganja)
	{
		for (int i = 0; i < _trenutnoPolaganja; i++)
		{
			if (_polaganja[i]._datumPolaganja.poslije(OD) && _polaganja[i]._datumPolaganja.prije(DO))
				brojacpolaganja++;
		}
		Polaganja *temp = new Polaganja[brojacpolaganja];
		for (int i = 0; i < _trenutnoPolaganja; i++)
		{
			if (_polaganja[i]._datumPolaganja.poslije(OD) && _polaganja[i]._datumPolaganja.prije(DO))
				temp[i].Unos(_polaganja[i]._polaznik, _polaganja[i]._kurs, _polaganja[i]._datumPolaganja, _polaganja[i]._ostvareniUspjeh);
		}
		return temp;
	}
	
};
float PretragaRekurzivno(int godina, enumKursevi kurs, SkillsCentar centar, int brojac=0, int validnih=0)
{
	if (brojac == centar._trenutnoPolaganja)
		return 0;
	
	if (centar._polaganja[brojac]._kurs._kurs == kurs && godina == *centar._polaganja[brojac]._datumPolaganja._godina)
	{
		validnih++;
		return (centar._polaganja[brojac]._ostvareniUspjeh + PretragaRekurzivno(godina, HtmlCSSJavaScript, centar, brojac + 1, validnih))/validnih;
	}
	PretragaRekurzivno(godina, HtmlCSSJavaScript, centar, brojac + 1, validnih);
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
	//jedan edukacijski centar moze nuditi vise istih kurseva (npr. MasteringSQL), ali se oni moraju realizovati u razlictim periodima (pocetak-kraj) tj. ne smiju se preklapati
	//novi kurs istog tipa (npr. MasteringSQL) se mora realizovati nakon prethodno dodanog npr. ako prethodni kurs MasteringSQL traje 01.03.2016 - 01.04.2016, onda se novi kurs MasteringSQL moze dodati jedino ako pocinje nakon 01.04.2016
	if (mostar.DodajKurs(sql)) {
		cout << crt << " KURS USPJESNO REGISTROVAN " << crt;
		sql.Ispis();
		cout << crt;

	}
	if (mostar.DodajKurs(softverski)) {
		cout << crt << " KURS USPJESNO REGISTROVAN " << crt;
		softverski.Ispis();
		cout << crt;

	}
	
	//nije moguce dodati polaganje onog kursa koji nije evidentiran 
	//(registrovan, ponudjen) u tom edukacijskom centru
	//moguce je polagati samo aktivne kurseve i to najkasnije 5 dana od dana 
	//zavrsetka (kraja) posljednje dodanog (aktivnog) kursa iste vrste 
	//(npr. MasteringSQL)


	//jedan polaznik moze vise puta polagati isti kurs, ali novi procenat mora 
	//biti veci od prethodno dodanog (na istom kursu)
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

	//PretragaRekurzivno - rekurzivna funkcija pronalazi prosjecni uspjeh koji su 
	//polaznici tokom godine (npr.2016) ostvarili na odredjenom kursu 
	//(npr. MasteringSQL)
	cout << "Prosjecan uspjeh na kursu MasteringSQL u 2016 godini je " << PretragaRekurzivno(2016, HtmlCSSJavaScript,mostar) << endl;
	//Ispis - ispisuje sve informacije o edukacijskom centru. prije ispisa sortirati
	//sve kurseve na osnovu pocetka odrzavanja kursa
	mostar.Ispis();

	//izvrsiti potrebne dealokacije
	datum1.Dealociraj();
	datum2.Dealociraj();
	datum3.Dealociraj();
	datum4.Dealociraj();
	datum5.Dealociraj();
	datum6.Dealociraj();

	softverski.Dealociraj();
	html.Dealociraj();
	sql.Dealociraj();
	security.Dealociraj();

	denis.Dealociraj();
	zanin.Dealociraj();
	indira.Dealociraj();

	denisHtml.Dealociraj();
	zaninHtml.Dealociraj();
	polaganjaByDatum->Dealociraj();

	mostar.Dealociraj();
	system("pause");
}
