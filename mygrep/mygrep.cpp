#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;


// Ensimmäinen inkrementti. Tehdään jos komennossa ei ole muita argumentteja.
void inkrementti1() {

	string isoLause, etsittava;

	cout << "Give a string from which to search for: ";
	getline(cin,isoLause);

	cout << "Give search string: ";
	getline(cin, etsittava);

	size_t sijainti = isoLause.find(etsittava); // Etsii annetusta lauseesta etsittävän sanan.


	// Suoritetaan jos etsittävä löytyi lauseesta.
	if (sijainti != string::npos) {
		cout << endl << '"' << etsittava << '"' << " found in " << '"' << isoLause << '"' << " in position " << sijainti << endl;
	}
	else {
		cout << endl << '"' << etsittava << '"' << " NOT found in " << '"' << isoLause << '"' << endl;
	}
}



// Aliohjelma joka tarkistaa mitkä optionit löytyy toisesta argumentista.
void optioidenTarkastus(string optiot, bool& oMuuttuja, bool& lMuuttuja, bool& rMuuttuja, bool& iMuuttuja) {
	if (optiot.rfind("-o", 0) == 0) {	//Varmistaa että argumentissa on kohta "-o" alussa.
		for (char ch : optiot.substr(2)) {	// Jos argumentissa on joku optioneista, se muuttaa sen tilan todeksi pääohjelmaa varten.
			if (ch == 'o')
				oMuuttuja = true;
			if (ch == 'l')
				lMuuttuja = true;
			if (ch == 'r')
				rMuuttuja = true;
			if (ch == 'i')
				iMuuttuja = true;
		}
	}
}



//Aliohjelma joka muuttaa tarkistettavan rivin ja etsittävän sanan pieneksi jotta se lause voidaan etsiä vaikka kirjaimet olisivat erikokoisia.
//Tämä on käytössä vain jos "i" option on käytössä.
void pieneksiMuuttaminen(string& rivi, string& etsittava) {
	transform(rivi.begin(), rivi.end(), rivi.begin(), ::tolower);
	transform(etsittava.begin(), etsittava.end(), etsittava.begin(), ::tolower);
}



int main( int argc, char* argv[]) {

	setlocale(LC_ALL, "fi-FI");
	
	string optiot, etsittava, tiedosto;
	bool oMuuttuja = false;
	bool lMuuttuja = false;
	bool rMuuttuja = false;
	bool iMuuttuja = false;



	// Ajetaan jos muita argumentteja ei käytetä.
	if (argc < 2) {
		inkrementti1();
		return 1;
	}

	// Virheilmoitus jos argumentteja käytetään mutta niitä on liian vähän.
	else if (argc < 3) {
		cout << "Give at least 2 arguments! mygrep.exe <options> <string> <file name>" << endl;
		return 1;
	}

	// Ajetaan jos argumentteja käytetään mutta ei optioneita.
	else if (argc < 4) {
			etsittava = argv[1];
			tiedosto = argv[2];
	}

	// Ajetaan jos optioneita käytetään.
	else if (argc > 3) {
			optiot = argv[1];
			etsittava = argv[2];
			tiedosto = argv[3];
			optioidenTarkastus(optiot, oMuuttuja, lMuuttuja, rMuuttuja, iMuuttuja); // Tarkistetaan optioiden oikeellisuus ja mitä käytetään.
			
	}



	// Tiedoston avaus ja tarikistus.
	try {
		ifstream avattuTiedosto(tiedosto);

		if (!avattuTiedosto) {
			throw runtime_error("Cant find file named " + tiedosto);
		}


		string rivi;
		int rivinumero = 0;
		int loydetytrivit = 0;

		// Ohjelma tulostaa kaikki halutut rivit etsittävän sanan perusteella ja sen mukaan mitä optioneita toisessa argumentissa on käytetty.
		while (getline(avattuTiedosto, rivi)) {
			rivinumero++;
			if (iMuuttuja == true) {	//Muutetaan haettu rivi ja etsittävä merkkijono pieniksi kirjaimiksi jotta kaikki rivit löytyy kirjainten koosta huolimatta.
				pieneksiMuuttaminen(rivi, etsittava);
			}
			if (rivi.find(etsittava) != string::npos && rMuuttuja == false) { // Etsitään merkkijonoa rivistä ja ajetaan ohjelma jos se löytyy (paitsi jos "r" option on käytössä).
				if (lMuuttuja == true) {	// tulostetaan myös rivi numerot jos "l" option on käytössä.
					cout << rivinumero << ":	";
				}
				cout << rivi << endl;
				loydetytrivit++;
			}
			else if (rMuuttuja == true) {	// Tulostaa rivin jos siitä ei löytynyt etsittävää merkkijonoa ja "r" option on käytössä.
				if (lMuuttuja == true) {	// tulostetaan myös rivi numerot jos "l" option on käytössä.
					cout << rivinumero << ":	";
				}
				cout << rivi << endl;
				loydetytrivit++;
			}
		}

		// Tulostaa vielä lopussa löydettyjen rivien määrän jos "o" option on käytössä.
		if (oMuuttuja == true && rMuuttuja == false) {
			cout << endl << "Occurrences of lines containing " << '"' << etsittava << '"' << ": " << loydetytrivit << endl;
		}
		else if (oMuuttuja == true && rMuuttuja == true) {
			cout << endl << "Occurrences of lines NOT containing " << '"' << etsittava << '"' << ": " << loydetytrivit << endl;
		}

		avattuTiedosto.close();

	}

	//error viesti jos tiedostoa ei löydy
	catch (const exception& e) {
			cerr << "Error: " << e.what() << endl;
			return 1;

	}

	return 0;
}