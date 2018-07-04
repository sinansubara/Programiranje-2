#include<iostream>

using namespace std;
enum Kompanija{ Pegasus, TurkishAirlines, AustrianAirlines, FlyEmirates };
char *kompanijaChar[] = { "Pegasus", "TurkishAirlines", "AustrianAirlines", "FlyEmirates" };
int ID = 1;

struct Putnik{
	int _putnikID;
	char * _imePrezime;
	float _brojPredjenihMilja;
	void Unos(char * imePrezime){
		_putnikID = ID++;
		_imePrezime = new char[strlen(imePrezime) + 1];
		strcpy_s(_imePrezime, strlen(imePrezime) + 1, imePrezime);
		_brojPredjenihMilja = 0;

	}

	void Info(){
		cout << "[" << _putnikID << "] " << _imePrezime <<
			" (" << _brojPredjenihMilja << " milja)" << endl;
	}

	void Dealociraj(){
		delete[] _imePrezime; 
		_imePrezime = nullptr;
	}
	void DodajPredjeneMilje(int brojMilja){ _brojPredjenihMilja += brojMilja; }
};

struct Rezervacija{
	Putnik * _putnik;
	int _oznakaSjedista;
	float _cijena;
	void Unos(Putnik putnik, int oznaka, float cijena){
		_putnik = new Putnik;
		_putnik->_putnikID = putnik._putnikID;
		_putnik->_imePrezime = new char[strlen(putnik._imePrezime) + 1];
		strcpy_s(_putnik->_imePrezime, strlen(putnik._imePrezime) + 1, putnik._imePrezime);
		_putnik->_brojPredjenihMilja = putnik._brojPredjenihMilja;
		_oznakaSjedista = oznaka;
		_cijena = cijena;

	}

	void Info(){
		_putnik->Info();
		cout << "Sjediste: " << _oznakaSjedista << " Cijena: "
			<< _cijena << endl;
	}

	void Dealociraj(){ 
		_putnik->Dealociraj();
		delete _putnik;
		_putnik = nullptr;
	}
};

struct Let{
	Kompanija * _kompanija;
	char * _pocetak; //pocetna lokacija
	char * _destinacija;
	Rezervacija * _rezervacije;
	int _brojMjesta; //maksimalan broj mjesta na letu  
	float _brojMilja; //odnosi se na duzinu leta – broj predjenih milja
	float _cijenaKarte;

	void Unos(Kompanija kompanija, char * pocetak, char * destinacija,
		int brojMjesta, float brojMilja, float cijena){
		_kompanija = new Kompanija;
		*_kompanija = kompanija;
		_pocetak = new char[strlen(pocetak) + 1];
		strcpy_s(_pocetak, strlen(pocetak) + 1, pocetak);
		_destinacija = new char[strlen(destinacija) + 1];
		strcpy_s(_destinacija, strlen(destinacija) + 1, destinacija);
		_brojMjesta = brojMjesta;
		_brojMilja = brojMilja;
		_cijenaKarte = cijena;
		Putnik *temp = new Putnik;
		temp->Unos("<SLOBODNO MJESTO>");
		_rezervacije = new Rezervacija[_brojMjesta];
		for (int i = 0; i < _brojMjesta; i++)
		{
			_rezervacije[i].Unos(*temp, i, cijena);
		}
	}

	void Dealociraj(){
		/*definisati funkciju vodeci racuna o oslobadjanju svih resursa koji su alocirani za potrebe objekta tipa Let*/
		/*delete _kompanija;*/
		delete[] _pocetak;
		delete[] _destinacija;
		for (int i = 0; i < _brojMjesta; i++)
		{
			_rezervacije[i]._putnik->Dealociraj();
			
		}
		delete[] _rezervacije;
		_kompanija = nullptr;
		_pocetak = nullptr;
		_destinacija = nullptr;
		_rezervacije = nullptr;

	}

	bool AddRezervaciju(Putnik * p){

		/*na samom pocetku, funkcija treba ispisati listu svih SLOBODNIH sjedista na letu, 
		te omoguciti odabir nekog do njih. prilikom formiranja cijene karte voditi racuna o broju predjenih milja; 
		ako je putnik presao od  10000 do 50000 milja onda ostvaruje popust od 10% na punu cijenu karte (_cijenaKarte), 
		od 50000 do 100000 milja ostvaruje popust od 20% na punu cijenu karte, 
		a preko 100000 milja ostvaruje  popust od 30% na punu cijenu karte. 
		onemoguciti dodavanje rezervacija sa identicnim putnicima (isti putnikID i imePrezime). 
		u zavisnosti  od uspjesnosti funkcija vraca true ili false */
		
		cout << "Na letu kompanijom	"<<kompanijaChar[*_kompanija] <<" "<<_pocetak << " - " << _destinacija << "." << endl;
		
		for (int i = 0; i < _brojMjesta; i++)
		{
			if (_rezervacije[i]._putnik->_putnikID == p->_putnikID && strcmp(_rezervacije[i]._putnik->_imePrezime, p->_imePrezime) == 0)
			{
				cout << "Putnik sa tim ID-om, imenom i prezimenom je vec izvrsio rezervaciju" << endl;
				return false;
			}
		}
		
		for (int i = 0; i < _brojMjesta; i++)
		{
			if (strcmp(_rezervacije[i]._putnik->_imePrezime, "<SLOBODNO MJESTO>") == 0)
			cout << "Mjesto sa oznakom " << i << " je slobodno" << endl;
		}
		
		int index = 0;
		
		do{
			cout << "Unesite broj mjesta " << endl;
			cin >> index;
			/*cin.ignore();*/
		} while (index>=_brojMjesta || strcmp(_rezervacije[index]._putnik->_imePrezime, "<SLOBODNO MJESTO>") != 0 );
		

		float cijenaK = 10;
		if (p->_brojPredjenihMilja>10000 && p->_brojPredjenihMilja < 50000)
			cijenaK = _cijenaKarte*0.9;

		if (p->_brojPredjenihMilja>50001 && p->_brojPredjenihMilja < 100000)
			cijenaK = _cijenaKarte*0.8;

		if (p->_brojPredjenihMilja>100000)
			cijenaK = _cijenaKarte*0.7;

		
		_rezervacije[index].Unos(*p, index, cijenaK);
		
		/*cout << _rezervacije[index]._putnik->_imePrezime <<" izvrsio je rezervaciju i cijena karte je "<<_rezervacije[index]._cijena << endl;
		*/
		_rezervacije[index].Info();
		_rezervacije[index]._putnik->DodajPredjeneMilje(_brojMilja);
		return true;


	}



	bool RemoveRezervacija(int oznakaSjedista){
		/*funkcija uklanja rezervaciju na sjedistu cija je oznaka proslijedjena kao parametar funkcije, 
		te vraca vrijednost. U slucaju da oznaceno mjesto nije rezervisano ili ne postoji na tom letu, funkcija vraca vrijednost false */
		if (oznakaSjedista < 0 || oznakaSjedista >= _brojMjesta){
			cout << "To mjesto ne postoji na ovom letu" << endl;
			return false;
		}
		if (strcmp(_rezervacije[oznakaSjedista]._putnik->_imePrezime, "<SLOBODNO MJESTO>") == 0)
		{
			cout << "Mjesto sa tom oznakom je vec slobodno" << endl;
			return false;
		}
		_rezervacije[oznakaSjedista]._putnik->_brojPredjenihMilja -= _brojMilja;
		Putnik *temp = new Putnik;
		temp->Unos("<SLOBODNO MJESTO>");
		
		_rezervacije[oznakaSjedista].Unos(*temp, oznakaSjedista, _cijenaKarte);
		
		return true;
		
	}



};


float GetMiljeByKompanija(Let * letovi, int maxLetova, Kompanija k, Putnik p){
	float ukupnoMilja = 0;
	/*funkcija vraca broj milja koje je putnik p ostvario putujuci sa kompanijom k*/
	for (int i = 0; i < maxLetova; i++)
	{
		if (*letovi[i]._kompanija == k)
		{
			for (int j = 0; j < letovi[i]._brojMjesta; j++)
			{
				if (strcmp(letovi[i]._rezervacije[j]._putnik->_imePrezime,p._imePrezime)==0)
				{
					ukupnoMilja += letovi[i]._brojMilja;
				}
			}
		}
	}
	return ukupnoMilja;
}

void main(){

	/*provjeriti validnost izvrsenja svih dostupnih funkcionalnosti. vrijednosti koje koristite prilikom testiranja mogu biti predefinisane
	tj. ne morate omogucavati korisniku da ih inicijalizuje.*/

	Putnik a,b,c;
	a.Unos("Putnik 1");
	b.Unos("Putnik 2");
	c.Unos("Putnik 3");

	a.DodajPredjeneMilje(23456);
	b.DodajPredjeneMilje(95678);
	c.DodajPredjeneMilje(133033);
	a.Info();
	b.Info();
	c.Info();
	
	Let *letovi = new Let[3];

	letovi[0].Unos(Pegasus, "Sarajevo","Minken", 3,450, 344);
	letovi[1].Unos(Pegasus, "Sarajevo", "Bahame", 5, 3500, 485);
	letovi[2].Unos(Pegasus, "Sarajevo", "New York", 3, 4461, 590);

	letovi[0].AddRezervaciju(&a);
	letovi[0].AddRezervaciju(&b);
	letovi[0].AddRezervaciju(&c);

	letovi[1].AddRezervaciju(&a);
	letovi[1].AddRezervaciju(&b);
	letovi[1].AddRezervaciju(&c);

	letovi[2].AddRezervaciju(&a);
	letovi[2].AddRezervaciju(&b);
	letovi[2].AddRezervaciju(&c);

	if (letovi[1].RemoveRezervacija(2))
		cout << "Rezervacija je uspjesno otkazana" << endl;
	else
		cout << "Rezervacija nije otkazana" << endl;

	cout << "Putnik 1 je kompanijom " << kompanijaChar[Pegasus] << " presao " << GetMiljeByKompanija(letovi, 3, Pegasus, a) << endl;
	cout << "Putnik 1 je kompanijom " << kompanijaChar[TurkishAirlines] << " presao " << GetMiljeByKompanija(letovi, 3, TurkishAirlines, a) << endl;
	cout << "Putnik 2 je kompanijom " << kompanijaChar[Pegasus] << " presao " << GetMiljeByKompanija(letovi, 3, Pegasus, b) << endl;
	cout << "Putnik 3 je kompanijom " << kompanijaChar[Pegasus] << " presao " << GetMiljeByKompanija(letovi, 3, Pegasus, c) << endl;

	system("pause > NULL");
}
