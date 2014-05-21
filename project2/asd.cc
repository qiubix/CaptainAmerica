//
//                     **    UWAGA!   **
//
// Plik asd.cc przeznaczony jest tylko do wpisania wlasnych testow.
// Cala implementacja powinna znajdowac sie w pliku aisdihashmap.h

#include<iostream>
#include<string>
#include "aisdihashmap.h"

using namespace std;

int main()
{
   // Miejsce na testy
   cout << "Testy:" << endl;
   typedef AISDIHashMap<string, int, hashF, _compFunc> MyMap;
   MyMap testmapa;
   std::cout << "Utworzyl testmape\n";
   //pair<int,string> p;
   //p.first = 1;
   //p.second = "Moj pierwszy test\n";
   //testmapa.insert(p);
   testmapa.empty();
   std::cout << "przeszedl empty\n";
   //testmapa.insert(make_pair("moj pierwszy hui",1));
   
   return 0;
}
