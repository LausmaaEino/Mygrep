#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;


// Ensimm�inen inkrementti. Tehd��n jos komennossa ei ole muita argumentteja.
void inkrementti1() {

	string isoLause, etsittava;

	cout << "Give a string from which to search for: ";
	getline(cin,isoLause);

	cout << "Give search string: ";
	getline(cin, etsittava);

	size_t sijainti = isoLause.find(etsittava); // Etsii annetusta lauseesta etsitt�v�n sanan.


	// Suoritetaan jos etsitt�v� l�ytyi lauseesta.
	if (sijainti != string::npos) {
		cout << endl << '"' << etsittava << '"' << " found in " << '"' << isoLause << '"' << " in position " << sijainti << endl;
	}
	else {
		cout << endl << '"' << etsittava << '"' << " NOT found in " << '"' << isoLause << '"' << endl;
	}
}



// Aliohjelma joka tarkistaa mitk� optionit l�ytyy toisesta argumentista.
void optioidenTarkastus(string optiot, bool& oMuuttuja, bool& lMuuttuja, bool& rMuuttuja, bool& iMuuttuja) {
	if (optiot.rfind("-o", 0) == 0) {	//Varmistaa ett� argumentissa on kohta "-o" alussa.
		for (char ch : optiot.substr(2)) {	// Jos argumentissa on joku optioneista, se muuttaa sen tilan todeksi p��ohjelmaa varten.
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



//Aliohjelma joka muuttaa tarkistettavan rivin ja etsitt�v�n sanan pieneksi jotta se lause voidaan etsi� vaikka kirjaimet olisivat erikokoisia.
//T�m� on k�yt�ss� vain jos "i" option on k�yt�ss�.
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



	// Ajetaan jos muita argumentteja ei k�ytet�.
	if (argc < 2) {
		inkrementti1();
		return 1;
	}

	// Virheilmoitus jos argumentteja k�ytet��n mutta niit� on liian v�h�n.
	else if (argc < 3) {
		cout << "Give at least 2 arguments! mygrep.exe <options> <string> <file name>" << endl;
		return 1;
	}

	// Ajetaan jos argumentteja k�ytet��n mutta ei optioneita.
	else if (argc < 4) {
			etsittava = argv[1];
			tiedosto = argv[2];
	}

	// Ajetaan jos optioneita k�ytet��n.
	else if (argc > 3) {
			optiot = argv[1];
			etsittava = argv[2];
			tiedosto = argv[3];
			optioidenTarkastus(optiot, oMuuttuja, lMuuttuja, rMuuttuja, iMuuttuja); // Tarkistetaan optioiden oikeellisuus ja mit� k�ytet��n.
			
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

		// Ohjelma tulostaa kaikki halutut rivit etsitt�v�n sanan perusteella ja sen mukaan mit� optioneita toisessa argumentissa on k�ytetty.
		while (getline(avattuTiedosto, rivi)) {
			rivinumero++;
			if (iMuuttuja == true) {	//Muutetaan haettu rivi ja etsitt�v� merkkijono pieniksi kirjaimiksi jotta kaikki rivit l�ytyy kirjainten koosta huolimatta.
				pieneksiMuuttaminen(rivi, etsittava);
			}
			if (rivi.find(etsittava) != string::npos && rMuuttuja == false) { // Etsit��n merkkijonoa rivist� ja ajetaan ohjelma jos se l�ytyy (paitsi jos "r" option on k�yt�ss�).
				if (lMuuttuja == true) {	// tulostetaan my�s rivi numerot jos "l" option on k�yt�ss�.
					cout << rivinumero << ":	";
				}
				cout << rivi << endl;
				loydetytrivit++;
			}
			else if (rMuuttuja == true) {	// Tulostaa rivin jos siit� ei l�ytynyt etsitt�v�� merkkijonoa ja "r" option on k�yt�ss�.
				if (lMuuttuja == true) {	// tulostetaan my�s rivi numerot jos "l" option on k�yt�ss�.
					cout << rivinumero << ":	";
				}
				cout << rivi << endl;
				loydetytrivit++;
			}
		}

		// Tulostaa viel� lopussa l�ydettyjen rivien m��r�n jos "o" option on k�yt�ss�.
		if (oMuuttuja == true && rMuuttuja == false) {
			cout << endl << "Occurrences of lines containing " << '"' << etsittava << '"' << ": " << loydetytrivit << endl;
		}
		else if (oMuuttuja == true && rMuuttuja == true) {
			cout << endl << "Occurrences of lines NOT containing " << '"' << etsittava << '"' << ": " << loydetytrivit << endl;
		}

		avattuTiedosto.close();

	}

	//error viesti jos tiedostoa ei l�ydy
	catch (const exception& e) {
			cerr << "Error: " << e.what() << endl;
			return 1;

	}

	return 0;
}