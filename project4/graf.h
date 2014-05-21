// graf.h
// Zawiera deklaracje klas obslugujacych graf


#ifndef GRAF_H
#define GRAF_H

#include <iostream>
#include <sstream>
#include <vector> 

typedef std::pair<double,double> Punkt;		// Punkt zdefiniowany przez 2 wspolrzedne
struct Wierzcholek{
	Punkt p;
	int numer;
	Wierzcholek(int nr = 0): numer(nr) {}
	Wierzcholek(const Punkt& pp, int i = 0): p(pp), numer(i) { }
};
struct Krawedz{
	Wierzcholek* poczatek;
	Wierzcholek* koniec;
	double waga;		//waga krawedzi zczytywana z wejscia
	double odl;			//geometryczna odleglosc od poczatku do konca
	Krawedz(double w1 = 0, double w2 = 0): poczatek(NULL), koniec(NULL), waga(w1), odl(w2) {} 
};

// klasa przechowujaca graf, posiada metody wypisania grafu
// oraz znajdowania najkrotszych sciezek
class Graf{
	std::vector<Krawedz> E;
public:
	Graf();
	~Graf();
	void dodaj(int, int, double);
	double sciezka_wagi(int, int);
	double sciezka_geom(int, int);
	void wypisz();
};







#endif //GRAF_H
