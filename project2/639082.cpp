//
//                     **    UWAGA!   **
//
// Plik asd.cc przeznaczony jest tylko do wpisania wlasnych testow.
// Cala implementacja powinna znajdowac sie w pliku aisdihashmap.h

#include <iostream>
#include <string>
#include "aisdihashmap.h"

using namespace std;

void print(AISDIHashMap<string,string>::iterator &i)
{
	cout << i->first << "," << i->second << '\n';
}

int main()
{
//	Testy uzytkownika
//
//	***********************************************************************
//	**Test konstruktora, operatora [] i podstawowych funkcji iteratora**
//
//	***********************************************************************
//
//	Wykona sie nastepujacy kod:

	AISDIHashMap<string, string> m;
	m["2"] = "Merry";
	m["4"] = "Jane";
	m["8"] = "Korwin";
	m["4"] = "Magdalena";
	m["5"] = "Kant";
	m["6"] = "Hegel";
	m["7"] = "Ingarden";
	m["9"] = "Nitche";
	m["10"] = "Kaczynski";

	for(AISDIHashMap < string, string>::iterator i =m.begin();i!=m.end();++i) print(i);
	cout << "It works! \n\n";
//	2, Merry
//	4, Magdalena
//	5, Kant
//	6, Hegel
//	7, Ingarden
//	8, Korwin
//	9, Nitche
//	10, Kaczynski
//
//	***********************************************************************
//	*******************Test preinkrementacji iteratora*********************
//
//	***********************************************************************
//
//	Wykona sie nastepujacy kod:

	AISDIHashMap< string, string >::iterator l=m.begin();
	for(int k=0; k<m.size()+5; ++k)
	        {
	        print(l);
	        ++l;
	        }

//	2, Merry
//	4, Magdalena
//	5, Kant
//	6, Hegel
//	7, Ingarden
//	8, Korwin
//	9, Nitche
//	10, Kaczynski
//	0, 
//	2, Merry
//	4, Magdalena
//	5, Kant
//	6, Hegel
//
//	***********************************************************************
//	*******************Test postinkrementacji iteratora*********************
//
//	***********************************************************************
//
//	Wykona sie nastepujacy kod:

	l=m.begin();
	for(int k=0; k<m.size()+5; ++k)
	        {
	        print(l);
	        l++;
	        }

//	2, Merry
//	4, Magdalena
//	5, Kant
//	6, Hegel
//	7, Ingarden
//	8, Korwin
//	9, Nitche
//	10, Kaczynski
//	0, 
//	2, Merry
//	4, Magdalena
//	5, Kant
//	6, Hegel
//
//	***********************************************************************
//
//	********************Test predekrementacji iteratora*********************
//
//	***********************************************************************
//
//	Wykona sie nastepujacy kod:

	l=m.end();
	for(int k=0; k<m.size()+5; ++k)
	        {
	        print(l);
	        --l;
	        }

//	0, 
//	10, Kaczynski
//	9, Nitche
//	8, Korwin
//	7, Ingarden
//	6, Hegel
//	5, Kant
//	4, Magdalena
//	2, Merry
//	2, Merry
//	2, Merry
//	2, Merry
//	2, Merry
//
//	***********************************************************************
//
//	********************Test postdekrementacji iteratora*********************
//
//	***********************************************************************
//
//	Wykona sie nastepujacy kod:
//
	l=m.end();
	for(int k=0; k<m.size()+5; ++k)
	        {
	        print(l);
	        l--;
	        }
	cout << "It works! \n\n";
//	0, 
//	10, Kaczynski
//	9, Nitche
//	8, Korwin
//	7, Ingarden
//	6, Hegel
//	5, Kant
//	4, Magdalena
//	2, Merry
//	2, Merry
//	2, Merry
//	2, Merry
//	2, Merry
//
//	***********************************************************************
//
//	Test funkcji erase
//
//	***********************************************************************
//
//	Wykona sie nastepujacy kod:
	m.erase("2");
	m.erase("10");
	m.erase("5");
	m.erase("100");
	for(AISDIHashMap < string, string >::iterator i =m.begin();i!=m.end();++i) print(i);
	cout << "It works! \n\n";
//	erase
//	erase
//	erase
//	erase
//	po
//	4, Magdalena
//	6, Hegel
//	7, Ingarden
//	8, Korwin
//	9, Nitche
//
//	***********************************************************************
//
//	Test funkcji find i erase
//
//	***********************************************************************
//
//	Wykona sie nastepujacy kod:
	AISDIHashMap<string,string>::iterator odtad=m.find("7");
	AISDIHashMap<string,string>::iterator dotad=m.find("8");
	m.erase(odtad,dotad);
	for(AISDIHashMap < string, string >::iterator i =m.begin();i!=m.end();++i) print(i);

//	4, Magdalena
//	6, Hegel
//	8, Korwin
//	9, Nitche
//
//	***********************************************************************
//
//	Test funkcji clear
//
//	***********************************************************************
//
//	wykona sie nastepujacy kod:
	m.clear();
	for(AISDIHashMap < string, string >::iterator i =m.begin();i!=m.end();++i) print(i);
//
//	***********************************************************************
//
//	 test konstruktora kopijacego
//
//	***********************************************************************

//	Wykona sie nastepujacy kod:
	m["7"] = "drunk";
	cout << "It works! \n\n";
	m["56"] = "hedge";
	cout << "It works! \n\n";
	m["28"] = "juggle";
	cout << "It works! \n\n";
	m["16"] = "lappet";
	cout << "It works! \n\n";
	m["4"] = "overture";
	cout << "It works! \n\n";
	cout << "It works! \n\n";
	//AISDIHashMap < int, string > n=m;
	//n.erase(7);

	for(AISDIHashMap < string, string >::iterator i =m.begin();i!=m.end();++i) print(i);

	//for(AISDIHashMap < int, string >::iterator i =n.begin();i!=n.end();++i) print(i);
//
//	Tu ¿yjê
//	4, overture
//	7, drunk
//	16, lappet
//	28, juggle
//	56, hedge
//
//	4, overture
//	7, drunk
//	16, lappet
//	28, juggle
//	56, hedge
//
//
//	4, overture
//	16, lappet
//	28, juggle
//	56, hedge




}
