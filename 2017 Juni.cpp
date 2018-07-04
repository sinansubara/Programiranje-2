#include <iostream>
#include <memory>
using namespace std;
/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NEĆE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB160061.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ĆETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAĐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

char *crt = "\n-------------------------------------------\n";

enum eNacinStudiranja { REDOVAN, DL };
enum eRazred { PRVI = 1, DRUGI, TRECI, CETVRTI };
void prebacistr(char* &a, char * b)
{
	if (a == nullptr)
	{
		int vel = strlen(b) + 1;
		a = new char[vel];
		strcpy_s(a, vel, b);
	}
	else
	{
		int vel = strlen(a) + strlen(b) + 1;
		char *temp = new char[vel];
		strcpy_s(temp, vel, a);
		strcat_s(temp, vel, b);
		delete[] a;
		a = temp;
	}
}

void dodajbroj(char * &n, int broj)
{
	char buffer[5];
	_itoa_s(broj, buffer, 10);
	prebacistr(n, buffer);
}
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
	char * GetDatumKaoNizKaraktera()
	{
		char * datum = nullptr;
		dodajbroj(datum, *_dan);
		prebacistr(datum, ".");
		dodajbroj(datum, *_mjesec);
		prebacistr(datum, ".");
		dodajbroj(datum, *_godina);
		prebacistr(datum, " ");
		dodajbroj(datum, *_sati);
		prebacistr(datum, ":");
		dodajbroj(datum, *_minuti);
		return datum;
	}
	bool veciOD(DatumVrijeme d)
	{
		if (*_godina > *d._godina)
			return true;
		else if (*_godina == *d._godina)
		{
			if (*_mjesec > *d._mjesec)
				return true;
			if (*_mjesec == *d._mjesec)
			{
				if (*_dan > *d._dan)
					return true;
				if (*_dan == *d._dan)
				{
					if (*_sati > *d._sati)
						return true;
					if (*_sati == *d._sati)
					{
						if (*_minuti >= *d._minuti)
							return true;
					}
				}
			}
		}
		return false;
	}
};

const DatumVrijeme rokZaPrijavu = { new int(5), new int(7), new int(2017), new int(12), new int(30) };


struct Predmet {
	char * _naziv;
	int _ocjena;
	DatumVrijeme * _datumUnosa;
	void Unos(char * naziv, int ocjena, DatumVrijeme datumUnosa) {
		int vel = strlen(naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, naziv);
		_ocjena = ocjena;
		_datumUnosa = new DatumVrijeme;
		_datumUnosa->Unos(*datumUnosa._dan, *datumUnosa._mjesec, *datumUnosa._godina, *datumUnosa._sati, *datumUnosa._minuti);
	}
	void Dealociraj() {
		delete[] _naziv; _naziv = nullptr;
		_datumUnosa->Dealociraj(); delete _datumUnosa;
	}

	void Ispis() {
		//kreirati funkciju GetDatumKaoNizKaraktera() koja vraca vrijednosti atributa strukture datum kao niz karaktera
		cout << _naziv << " (" << _ocjena << ") " << _datumUnosa->GetDatumKaoNizKaraktera() << endl;
	}
};

struct Uspjeh {
	eRazred _razred;
	Predmet * _predmeti;
	int _brojPredmeta;
	void Unos(eRazred razred) {
		_razred = razred;
		_predmeti = nullptr;
		_brojPredmeta = 0;
	}
	void Dealociraj() {
		for (size_t i = 0; i < _brojPredmeta; i++)
			_predmeti[i].Dealociraj();
		delete[] _predmeti; _predmeti = nullptr;
	}

	void Ispis() {
		cout << crt << "Razred -> " << _razred << crt;
		for (size_t i = 0; i < _brojPredmeta; i++)
			_predmeti[i].Ispis();
	}
	void DodajPredmet(Predmet predmet)
	{
		//funkcija koja proširuje niz za jedan elemenat  
		Predmet * noviNiz = new Predmet[_brojPredmeta + 1];

		for (int i = 0; i < _brojPredmeta; ++i)
			noviNiz[i].Unos(_predmeti[i]._naziv,_predmeti[i]._ocjena, *_predmeti[i]._datumUnosa);
		
		noviNiz[_brojPredmeta].Unos(predmet._naziv, predmet._ocjena, *predmet._datumUnosa);

		if (_predmeti != nullptr)
			delete[] _predmeti;

		_predmeti = noviNiz;
		_brojPredmeta++;
	}
};

struct Kandidat {
	eNacinStudiranja _nacinStudiranja;
	char * _imePrezime;
	shared_ptr<Uspjeh> _uspjeh[4];

	void Unos(eNacinStudiranja nacinStudiranja, char * imePrezime) {
		int vel = strlen(imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, imePrezime);
		_nacinStudiranja = nacinStudiranja;
		for (size_t i = 0; i < 4; i++)
			_uspjeh[i] = nullptr;
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < 4; i++)
			if (_uspjeh[i] != nullptr)
				_uspjeh[i]->Dealociraj();

	}
	void Ispis() {
		cout << crt << _imePrezime << " " << _nacinStudiranja;
		for (size_t i = 0; i < 4; i++)
			if (_uspjeh[i] != nullptr)
				_uspjeh[i]->Ispis();

	}
	/*
	uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
	prilikom dodavanja onemoguciti:
	- dodavanje predmeta za razrede koji nisu definisani enumeracijom,
	- dodavanje istoimenih predmeta na nivou jednog razreda,
	- dodavanje predmeta nakon dozvoljenog roka za prijavu (rokZaPrijavu).
	razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II razred, pa onda za I razred i sl.). Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
	*/
	bool DodajPredmet(eRazred razred, Predmet predmet)
	{
		if (razred != PRVI && razred != DRUGI && razred != TRECI && razred != CETVRTI)
			return false;
		if (predmet._datumUnosa->veciOD(rokZaPrijavu))
			return false;
		if (_uspjeh[razred - 1] == nullptr)
		{
			_uspjeh[razred - 1] = make_shared<Uspjeh>();
			_uspjeh[razred - 1]->Unos(razred);
			_uspjeh[razred - 1]->DodajPredmet(predmet);
			return true;
		}
		for (int i = 0; i < _uspjeh[razred - 1]->_brojPredmeta; i++)
		{
			if (strcmp(_uspjeh[razred - 1]->_predmeti[i]._naziv, predmet._naziv) == 0)
				return false;
		}
		_uspjeh[razred - 1]->DodajPredmet(predmet);
		return true;
	}
};

void main()
{
	DatumVrijeme datum19062017_1015, datum20062017_1115, datum30062017_1215, datum05072017_1231;
	datum19062017_1015.Unos(19, 6, 2017, 10, 15);
	datum20062017_1115.Unos(20, 6, 2017, 11, 15);
	datum30062017_1215.Unos(30, 6, 2017, 12, 15);
	datum05072017_1231.Unos(5, 7, 2017, 12, 31);
	cout << "Test datuM: ";
	cout << datum19062017_1015.GetDatumKaoNizKaraktera() << endl;//9.6.2017 10:15

	Predmet Matematika, Fizika, Hemija, Engleski;
	//2 - ocjena na predmetu; datum - datum evidentiranja uspjeha na predmetu jer postoji krajnji rok za evidentiranje
	Matematika.Unos("Matematika", 2, datum19062017_1015);
	Fizika.Unos("Fizika", 5, datum20062017_1115);
	Hemija.Unos("Hemija", 2, datum20062017_1115);
	Engleski.Unos("Engleski", 5, datum05072017_1231);
	cout << "Test Predmet: ";
	Engleski.Ispis();
	int brojKandidata = 2;

	Kandidat * prijave2017 = new Kandidat[brojKandidata];
	prijave2017[0].Unos(DL, "Jasmin Azemovic");
	prijave2017[1].Unos(REDOVAN, "Indira Hamulic");

	
	if (prijave2017[0].DodajPredmet(DRUGI, Engleski))//ne bi trebao dodati jer je prosao postavljeni rok za dodavanje predmeta
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[0].DodajPredmet(DRUGI, Matematika))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[0].DodajPredmet(PRVI, Fizika))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[0].DodajPredmet(PRVI, Hemija))
		cout << "Predmet uspjesno dodan!" << crt;

	Matematika._ocjena = 5;
	Hemija._ocjena = 3;

	if (prijave2017[1].DodajPredmet(PRVI, Matematika))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[1].DodajPredmet(PRVI, Matematika))//ne bi trebalo ponovo dodati Matematiku!
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[1].DodajPredmet(TRECI, Hemija))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[1].DodajPredmet(DRUGI, Engleski))
		cout << "Predmet uspjesno dodan!" << crt;

	/*
	koristeci Lambda izraz kreirati funkciju koja ce vratiti uspjeh kandidata 
	koji je ostvario najveci prosjek (na nivou razreda, a ne ukupni prosjek). 
	ukoliko vise kandidata ima isti prosjek funkcija vraca uspjeh (najboljeg razreda) 
	prvog pronadjenog kandidata
	*/
	auto najboljiUspjeh = [prijave2017, brojKandidata]()
	{
		float  x = 0;
		shared_ptr<Uspjeh> rezultat = nullptr;
		for (int i = 0; i < brojKandidata; i++)
		{
			Kandidat kandidat = prijave2017[i];

			for (int j = 0; j < 4; j++)
			{
				shared_ptr<Uspjeh> uspjeh = kandidat._uspjeh[j];
				if (uspjeh != nullptr)
				{
					int suma = 0;
					for (int z = 0; z < uspjeh->_brojPredmeta; z++)
					{
						suma += uspjeh->_predmeti[z]._ocjena;
					}
					float prosjek = float(suma) / uspjeh->_brojPredmeta;
					if (prosjek >= x)
					{
						x = prosjek;
						rezultat = uspjeh;
					}
				}
			}
		}
		return rezultat;
	};
	shared_ptr<Uspjeh> najbolji = najboljiUspjeh();
	cout << "Najbolji uspjeh ----> ";
	najbolji->Ispis();


	///*
	//napisati rekurzivnu funkciju koja ce vratiti pokazivac na kandidata sa najvecom ocjenom na predmetu koji je proslijedjen kao parametar. ukoliko je vise kandidata ostvarilo istu ocjenu, funkcija treba da vrati onog kandidata koji je prvi evidentirao tu ocjenu (ako je isto vrijeme evidentiranja, onda funkcija vraca kandidata koji je prvi u nizu). u slucaju da niti jedan kandidat nije evidentirao trazeni predmet funkcija vraca nullptr. u nastavku je prikazan primjer poziva rekurzivne funkcije, a ostale parametre dodajte po potrebi.
	//*/
	//Kandidat * kandidatSaNajboljomOcjenom = rekNajboljaOcjena(prijave2017, brojKandidata, "Matematika");
	datum19062017_1015.Dealociraj();
	datum20062017_1115.Dealociraj();
	datum30062017_1215.Dealociraj();
	datum05072017_1231.Dealociraj();
	Matematika.Dealociraj();
	Fizika.Dealociraj();
	Hemija.Dealociraj();
	Engleski.Dealociraj();

	for (size_t i = 0; i < brojKandidata; i++)
	{
		prijave2017[i].Ispis();
		prijave2017[i].Dealociraj();
	}
	delete[] prijave2017;
	prijave2017 = nullptr;

	system("pause");
}
