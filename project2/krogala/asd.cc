//
//                     **    UWAGA!   **
//
// Plik asd.cc przeznaczony jest tylko do wpisania wlasnych testow.
// Cala implementacja powinna znajdowac sie w pliku aisdihashmap.h

#include <iostream>
#include "aisdihashmap.h"

using namespace std;

int main()
{
// Miejsce na jakies testy
//tworzenie mapy
   AISDIHashMap<string, int> mapa;
   mapa.insert(make_pair("bla", 7));
   AISDIHashMap<string, int>::iterator i = mapa.begin();
   cout << i->first << " " << i->second << endl;
   return 0;
}
