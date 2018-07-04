/*
1. ZADATAK

Izvršiti definiciju funkcija na način koji odgovara opisu (komentarima) datom neposredno uz pozive ili nazive funkcija. Možete dati komentar na bilo koju liniju code-a koju smatrate da bi trebalo unaprijediti ili da će eventualno uzrokovati grešku prilikom kompajliranja. Također, možete dodati dodatne funkcije koje će vam olakšati implementaciju programa.


1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NEĆE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ĆETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAĐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/
#include<iostream>
#include<iomanip>
#include<fstream>
using namespace std;

//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

char *crt = "\n-------------------------------------------\n";

enum vrstaDokumenta { PDF, DOC, TXT, HTML };
char *vrstaDokumentaTEXT[]={ "pdf", "doc", "txt", "html" };
enum prioritet { VISOK, SREDNJI, NIZAK };
char *prioritetTEXT[]={ "VISOK", "SREDNJI", "NIZAK" };
struct DatumVrijeme {
	int *_dan, *_mjesec, *_godina, *_sati, *_minuti;
	void Unos(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minuti = new int(minuti);
	}
	void Dealociraj() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
		delete _sati; _sati = nullptr;
		delete _minuti; _minuti = nullptr;

	}
	void Ispis() {
		cout << *_dan << "." << *_mjesec << "." << *_godina << " " << *_sati << ":" << *_minuti << endl;
	}
	bool poslije(DatumVrijeme od)
	{
		if (*_godina > *od._godina)
			return true;
		else if (*_godina == *od._godina)
		{
			if (*_mjesec > *od._mjesec)
				return true;
			if (*_mjesec == *od._mjesec)
			{
				if (*_dan > *od._dan)
					return true;
				if (*_dan == *od._dan)
				{
					if (*_sati > *od._sati)
						return true;
					if (*_sati == *od._sati)
					{
						if (*_minuti >= *od._minuti)
							return true;
					}
				}
			}
		}
		return false;
	}
	bool prije(DatumVrijeme d)
	{
		if (*_godina < *d._godina)
			return true;
		else if (*_godina == *d._godina)
		{
			if (*_mjesec < *d._mjesec)
				return true;
			if (*_mjesec == *d._mjesec)
			{
				if (*_dan < *d._dan)
					return true;
				if (*_dan == *d._dan)
				{
					if (*_sati < *d._sati)
						return true;
					if (*_sati == *d._sati)
					{
						if (*_minuti <= *d._minuti)
							return true;
					}
				}
			}
		}
		return false;
	}
};

struct Filter {
	char * _sadrzaj;
	prioritet _prioritet;
	void Unos(char * sadrzaj, prioritet prioritet) {
		int vel = strlen(sadrzaj) + 1;
		_sadrzaj = new char[vel];
		strcpy_s(_sadrzaj, vel, sadrzaj);
		_prioritet = prioritet;
	}
	void Dealociraj() {
		delete[] _sadrzaj; _sadrzaj = nullptr;
	}
	void Ispis() 
	{ 
		cout <<crt<<"Sadrzaj(struct filter): "<< _sadrzaj<<crt << " (" << _prioritet << ")" << endl; 
	}

};

struct Dokument {
	vrstaDokumenta _vrsta;
	char * _naziv;
	char * _sadrzaj;
	DatumVrijeme _kreiran;
	int _brojStranica;

	void Unos(vrstaDokumenta vrsta, char * naziv, DatumVrijeme kreiran) {
		int vel = strlen(naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, naziv);
		_vrsta = vrsta;
		_kreiran.Unos(*kreiran._dan, *kreiran._mjesec, *kreiran._godina, *kreiran._sati, *kreiran._minuti);
		_brojStranica = 0;//broj stranica se odredjuje prilikom dodavanja svakog novog sadrzaja dokumentu
		_sadrzaj = nullptr;
	}
	void Dealociraj() {
		delete[] _naziv; _naziv = nullptr;
		delete[] _sadrzaj; _sadrzaj = nullptr;
		_kreiran.Dealociraj();
	}

	void Ispis() {
		cout << crt << "Naziv: "<<_naziv << ", Vrsta: " << vrstaDokumentaTEXT[_vrsta] << "\nKreiran: ";
		_kreiran.Ispis();
		cout << crt << "Sadrzaj(struct dokument):"<<crt; /*<< _sadrzaj << crt << "Broj stranica (" << _brojStranica << ")" << crt;*/
		if (_sadrzaj == nullptr)
			cout << "";
		else
		{
			for (int i = 1; i < strlen(_sadrzaj); i++)
			{
				if (i % 30 == 0)
					cout << crt;
				cout << _sadrzaj[i - 1];
			}
		}
		cout << crt << "Broj stranica (" << _brojStranica << ")" << crt;
		
	}
	void DodajSadrzaj(char * sadrzaj)
	{
		if (sadrzaj == nullptr)
			return;
		if (_sadrzaj == nullptr)
		{
			int vel = strlen(sadrzaj) + 1;
			_sadrzaj = new char[vel];
			strcpy_s(_sadrzaj, vel, sadrzaj);
		}
		else
		{
			int vel = strlen(_sadrzaj) + strlen(sadrzaj) + 1;
			char *temp = new char[strlen(_sadrzaj)+1];
			strcpy_s(temp, strlen(_sadrzaj) + 1, _sadrzaj);
			delete[] _sadrzaj;
			_sadrzaj = new char[vel];
			strcpy_s(_sadrzaj, strlen(temp)+1, temp);
			strcat_s(_sadrzaj, vel, sadrzaj);
		}
		_brojStranica = strlen(_sadrzaj) / 30;
	}
	bool Ispravan(Filter *filteri, int brojfiltera)
	{
		if (_sadrzaj == nullptr)
			return false;
		char *naziv = _naziv;
		while (*naziv != '.')
		{
			if (!isalpha(*naziv))
				return false;
			
			naziv++;
		}
		char *ekstenzija = ++naziv;
		bool pronadjena = false;

		for (int i = 0; i < 4; i++)
			if (strcmp(vrstaDokumentaTEXT[i], ekstenzija) == 0)
				pronadjena = true;
		if (!pronadjena)	
			return false;
		

		for (int i = 0; i < brojfiltera; i++)
			if (strstr(_sadrzaj, filteri[i]._sadrzaj) != nullptr)
				return false;
			
		return true;
	}
};
struct Printer {
	char * _proizvodjac;
	char * _model;
	Dokument * _dokumenti;
	int _trenutnoDokumenata;
	Filter * _filteri;
	int _trenutnoFiltera;

	void Unos(char * proizvodjac, char * model) {
		int vel = strlen(proizvodjac) + 1;
		_proizvodjac = new char[vel];
		strcpy_s(_proizvodjac, vel, proizvodjac);
		vel = strlen(model) + 1;
		_model = new char[vel];
		strcpy_s(_model, vel, model);
		_trenutnoDokumenata = 0;
		_trenutnoFiltera = 0;
		_filteri = nullptr;
		_dokumenti = nullptr;
	}
	void Dealociraj() {
		delete[] _proizvodjac; _proizvodjac = nullptr;
		delete[] _model; _model = nullptr;
		for (size_t i = 0; i < _trenutnoDokumenata; i++)
			_dokumenti[i].Dealociraj();
		delete[]_dokumenti; _dokumenti = nullptr;//ako ne radilo ove zagrade na delete
		for (size_t i = 0; i < _trenutnoFiltera; i++)
			_filteri[i].Dealociraj();
		delete[]_filteri; _filteri = nullptr;//ako ne radilo ove zagrade na delete
	}
	void Ispis()
	{
		cout << "Proizvodjac: " << _proizvodjac << ", Model: " << _model;
		for (int i = 0; i < _trenutnoDokumenata; i++)
			if (_dokumenti[i].Ispravan(_filteri,_trenutnoFiltera))
				_dokumenti[i].Ispis();
				
		
	}
	bool DodajFilter(char *zabranjene, prioritet prio)
	{
		for (int i = 0; i < _trenutnoFiltera; i++)
		{
			if (strcmp(zabranjene, _filteri[i]._sadrzaj) == 0)
			{
				cout << "Pronadjen isti filter!" << endl;
				return false;
			}
		}
		if (_trenutnoFiltera == 0)
		{
			_filteri = new Filter;
			_filteri->Unos(zabranjene, prio);
			_trenutnoFiltera++;
			return true;
		}
		Filter *temp = new Filter[_trenutnoFiltera + 1];
		for (int i = 0; i < _trenutnoFiltera; i++)
		{
			temp[i].Unos(_filteri[i]._sadrzaj, _filteri[i]._prioritet);
			_filteri[i].Dealociraj();
		}
		
		delete[] _filteri;
		_filteri = new Filter[_trenutnoFiltera+1];
		for (int i = 0; i < _trenutnoFiltera; i++)
		{
			_filteri[i].Unos(temp[i]._sadrzaj, temp[i]._prioritet);
			temp[i].Dealociraj();
		}
		_filteri[_trenutnoFiltera].Unos(zabranjene, prio);
		delete[]temp; 
		_trenutnoFiltera++;
		return true;
	}
	/*
	DA BI PRINTER ISPRINTAO NEKI DOKUMENT MORAJU BITI ZADOVOLJENA SLJEDEĆA PRAVILA:
	1. NAZIV DOKUMENTA MOŽE SADRŽAVATI SAMO SLOVA, A EKSTENZIJA MOŽE BITI SAMO NEKA OD OZNAČENIH SA vrstaDokumenta
	2. SADRŽAJ DOKUMENTA U SEBI NE SMIJE POSJEDOVATI ZABRANJENE RIJEČ DEFINISANE FILTERIMA OSIM AKO SU ONE OZNAČENE SA NISKIM PRIORITETOM (PRIRITET NIZAK)

	UKOLIKO NEKI OD NAVEDENIH USLOVA NIJE ZADOVOLJEN FUNKCIJA PRINTAJ DOKUMENT TREBA SAČUVATI U NIZ _dokumenti, ALI TAJ DOKUMENT NE TREBA PRINTATI.
	UKOLIKO DOKUMENT ISPUNJAVA SVE USLOVE ZA PRINTANJE POTREBNO JE NA KONZOLU ISPISATI SADRŽAJ DOKUMENTA KOJI SE PRINTA, A SA CRT LINIJOM ODVAJATI STRANICE DOKUMENTA
	*/
	void Printaj(Dokument dokument)
	{
		if (dokument.Ispravan(_filteri, _trenutnoFiltera))
			dokument.Ispis();

		Dokument *temp = new Dokument[_trenutnoDokumenata + 1];


		for (int i = 0; i < _trenutnoDokumenata; i++)
		{
			temp[i].Unos(_dokumenti[i]._vrsta, _dokumenti[i]._naziv, _dokumenti[i]._kreiran);
			temp[i].DodajSadrzaj(_dokumenti[i]._sadrzaj); // treba se kopirat i sadrzaj jer to unos ne radi
			_dokumenti[i].Dealociraj();
		}

		delete[] _dokumenti;
		_dokumenti = temp;

		_dokumenti[_trenutnoDokumenata].Unos(dokument._vrsta, dokument._naziv, dokument._kreiran);
		_dokumenti[_trenutnoDokumenata].DodajSadrzaj(dokument._sadrzaj);

		_trenutnoDokumenata++;
	}
	float GetProsjecanBrojStranicaPoDatumuIVrsti(DatumVrijeme OD, DatumVrijeme DO, vrstaDokumenta vrsta)
	{
		float ukupnostranica = 0, ispravnih = 0;
		for (int i = 0; i < _trenutnoDokumenata; i++)
		{
			if (_dokumenti[i].Ispravan(_filteri, _trenutnoFiltera) && _dokumenti[i]._kreiran.poslije(OD) && _dokumenti[i]._kreiran.prije(DO) && _dokumenti[i]._vrsta == vrsta)
			{
				ukupnostranica += _dokumenti[i]._brojStranica;
				ispravnih++;
			}
		}
		return ukupnostranica / ispravnih;
	}
	Filter * GetNakoristenijiFilter()
	{
		int brojacfiltera = 0, maxkoristenfilter = 0, indeks = 0;

		for (int i = 0; i < _trenutnoFiltera; i++)
		{
			brojacfiltera = 0;
			for (int j = 0; j < _trenutnoDokumenata; j++)
			{
				if (_dokumenti[j]._sadrzaj != nullptr)
				{
					char *filterSadrzaj = _filteri[i]._sadrzaj;
					char *dokumentSadrzaj = _dokumenti[j]._sadrzaj;

					while (strstr(dokumentSadrzaj, filterSadrzaj) != nullptr)
					{
						brojacfiltera++;
						dokumentSadrzaj += strlen(filterSadrzaj);
					}
				}
			}

			if (brojacfiltera > maxkoristenfilter)
			{
				maxkoristenfilter = brojacfiltera;
				indeks = i;
			}
		}
		return &_filteri[indeks];
	}
	
};
void ZamijeniIPrintaj(Printer printer, char znak, int brojac = 0)
{
	if (brojac >= printer._trenutnoDokumenata)
		return;

	for (int i = 0; i < printer._trenutnoFiltera; i++)
	{
		if (printer._dokumenti[brojac]._sadrzaj != nullptr)
		{
			char *filtersadrzaj = printer._filteri[i]._sadrzaj;
			char *dokumentsadrzaj = printer._dokumenti[brojac]._sadrzaj;
			char *poknafilter = strstr(dokumentsadrzaj, filtersadrzaj);
			while (poknafilter != nullptr)
			{
				while (isalpha(*poknafilter))
				{
					*poknafilter = znak;
					poknafilter++;
				}
				poknafilter += strlen(filtersadrzaj);
				poknafilter = strstr(dokumentsadrzaj, filtersadrzaj);
			}

		}
	}
	printer._dokumenti[brojac].Ispis();
	ZamijeniIPrintaj(printer, znak, brojac + 1);
}

void main() {
	DatumVrijeme prije3Dana; prije3Dana.Unos(3, 2, 2017, 10, 15);
	DatumVrijeme danas1; danas1.Unos(6, 2, 2017, 10, 15);
	DatumVrijeme danas2; danas2.Unos(6, 2, 2017, 10, 16);
	DatumVrijeme za10Dana; za10Dana.Unos(16, 2, 2017, 10, 15);
	
	Dokument ispitPRII, ispitMAT, ispitUIT, ispitUITDrugiRok;
	ispitPRII.Unos(DOC, "ispitPRII.doc", prije3Dana);
	ispitMAT.Unos(DOC, "ispitMAT.doc", danas1);
	ispitUIT.Unos(DOC, "ispitUIT.doc", danas2);
	ispitUITDrugiRok.Unos(PDF, "ispitUITDrugiRok.pdf", za10Dana);

	
	/*BROJ STRANICA DOKUMENTA SE AUTOMATSKI ODREĐUJE PRILIKOM DODAVANJA SADRŽAJA. ZA POTREBE ISPITA PRETPOSTAVLJAMO DA NA JEDNU
	STRANICU MOŽE STATI 30 ZNAKOVA UKLJUČUJUĆI I RAZMAKE*/
	ispitPRII.DodajSadrzaj("Programiranje ili racunarsko programiranje (engl. programming) jeste vjestina pomocu koje ");
	ispitPRII.DodajSadrzaj("korisnik stvara i izvrsava algoritme koristeci odredene programske jezike da bi ... ");
	cout << "Broj stranica -> " << ispitPRII._brojStranica << endl;

	ispitPRII.Ispis();//ISPISUJE SVE PODATKE O DOKUMENTU

	ispitMAT.DodajSadrzaj("matematika se razvila iz potrebe da se obavljaju proracuni u trgovini, vrse mjerenja zemljista i predvidaju ");
	ispitMAT.DodajSadrzaj("astronomski dogadaji, i ove tri primjene se mogu dovesti u vezu sa grubom podjelom matematike na RAT i ");

	
	Printer hp3200; 
	hp3200.Unos("HP", "3200");

	/*PRINTER NEĆE DOZVOLITI PRINTANJE DOKUMENATA U ČIJEM SADRŽAJU SE NALAZI NEKA OD ZABRANJENIH RIJEČI DEFINISANIH FILTERIMA*/
	if (hp3200.DodajFilter("RE", NIZAK)) //VISOK :)
		cout << "Filter dodan!" << endl;
	if (hp3200.DodajFilter("RAT", VISOK))
		cout << "Filter dodan!" << endl;
	if (hp3200.DodajFilter("UBITI", VISOK))
		cout << "Filter dodan!" << endl;
	if (hp3200.DodajFilter("MRZITI", SREDNJI))
		cout << "Filter dodan!" << endl;
	if (hp3200.DodajFilter("RE", SREDNJI))/* ONEMOGUĆITI PONAVLJANJE FILTERA, BEZ OBZIRA STO SU RAZLICITOG PRIORITETA*/
		cout << "Filter dodan!" << endl;

	
	hp3200.Printaj(ispitPRII);
	hp3200.Printaj(ispitMAT);
	hp3200.Printaj(ispitUIT);
	hp3200.Printaj(ispitUITDrugiRok);

	//PROSJEČAN BROJ STRANICA = UKUPAN BROJ STRANICA / BROJ PRINTANIH DOKUMENATA
	cout << "Prosjecan broj printanih stranica je -> " << hp3200.GetProsjecanBrojStranicaPoDatumuIVrsti(prije3Dana, danas2, DOC) << crt;
	
	//VRAĆA FILTER KOJI SE NAJČEŠĆE KORISTIO U DOKUMENTIMA
	Filter * f = hp3200.GetNakoristenijiFilter();
	cout << "Najcesce koristeni sadrzaj filtera je -> " << f->_sadrzaj << crt;

	//PORED VRIJEDNOSTI SVIH OSTALIH ATRIBUTA, ISPISUJE SAMO ONE DOKUMENTE KOJI ZADOVOLJAVAJU SVA PRETHODNO DEFINISANA PRAVILA
	hp3200.Ispis();

	/*MIJENJA ZNAKOVE ZABRANJENOG SADRŽAJA DOKUMENTA SA POSLANIM KARAKTEROM
	(BEZ OBZIRA NA PRIORITET) I NAKON TOGA PRINTA SADRŽAJ DOKUMENATA; npr.
	UBITI POSTAJE -----.
	RIJEŠITI REKURZIJOM*/
	ZamijeniIPrintaj(hp3200, '-'); //OSTALE PARAMETRE DODAJTE PO POTREBI


	//DEALKOACIJE
	prije3Dana.Dealociraj(); 
	danas1.Dealociraj(); 
	danas2.Dealociraj(); 
	za10Dana.Dealociraj();
	
	
	ispitMAT.Dealociraj(); 
	ispitPRII.Dealociraj(); 
	ispitUIT.Dealociraj(); 
	ispitUITDrugiRok.Dealociraj();

	hp3200.Dealociraj();

	system("PAUSE");
}
