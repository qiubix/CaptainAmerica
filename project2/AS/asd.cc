//
//                     **    UWAGA!   **
//
// Plik asd.cc przeznaczony jest tylko do wpisania wlasnych testow.
// Cala implementacja powinna znajdowac sie w pliku aisdihashmap.h

#include<iostream>
#include<string>
#include "aisdihashmap.h"

using namespace std;

/*int main()
{	
typedef AISDIHashMap<string, int, hashF, _compFunc> MyMap;
	// Miejsce na testy
	cout << "Testy:" << endl;
	MyMap mapka;


	string s("jedena");
	cout<<mapka.empty()<<endl;
	mapka.insert(make_pair(s,1000));
	cout<<mapka.empty()<<endl;
	cout<<mapka["jedena"]<<endl;
	cout << "mapka zawiera: " << mapka.size() << " elementow." << endl;
//cout<<mapka["dwa"]<<endl;
	mapka["dwa"]=2;
	mapka["dwaaa"]=3;
mapka["dwaa"]=4;
mapka["dwaaaa"]=5;
	//mapka.insert(make_pair("dwa", 2));

cout<<mapka["dwa"]<<endl;
	cout << "mapka zawiera: " << mapka.size() << " elementow." << endl;
	MyMap::iterator i=MyMap::iterator(mapka.begin());
	for(;i.node!=mapka.straznik;i.node=i.node->pnext)
{getchar();
		cout<<static_cast<void*>(i.node)<<endl;}
	MyMap mapka2(mapka);
	i=MyMap::iterator(mapka2.begin());
	for(;i.node!=mapka2.straznik;i.node=i.node->pnext)
{getchar();
		cout<<static_cast<void*>(i.node)<<endl;}
	i=MyMap::iterator(mapka2.begin());
	for(;i.node!=mapka2.straznik;i.node=i.node->pnext)
{getchar();
		cout<<i->first<<" "<<mapka2[i->first]<<endl;}
	i.node=i.node->pnext;
	MyMap::iterator i2;
	i2.node=i.node->pprev;
	mapka2.erase(i,i);
	cout<<mapka2.size()<<endl;
	getchar();
	mapka.erase(mapka.find("jedena"));
	cout << "mapka zawiera: " << mapka.size() << " elementow." << endl;

	mapka.erase("dwa");
cout << "mapka zawiera: " << mapka.size() << " elementow." << endl;


	mapka.erase("piec");






	cout << "mapka zawiera: " << mapka.size() << " elementow." << endl;

	getchar();
	return 0;

}*/
int main()
{
 typedef AISDIHashMap<string, int, hashF> MyMap;
 // Miejsce na testy> 
 cout << "Testy:" << endl;
 MyMap mapka;
 mapka["jeden"] = 1;
 mapka["dwa"] = 2;
 mapka["trzy"] = 3;

 MyMap mapeczka(mapka);
 MyMap::iterator it = mapka.find("jeden");
cout<<mapka.size()<<endl;
 cout << it->first << ' ' << it->second << endl;
 mapka.erase("jeden");
cout<<mapka.size()<<endl;
 cout << it->first << ' ' << it->second<<endl;
 return 0;
}
