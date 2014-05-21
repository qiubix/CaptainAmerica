/** 
@file asd.cc

Plik do modyfikacji w ramach cwiczenia z AISDI.
Zawiera niekompletne implementacje metod klasy ListMap,
oraz mape podobna koncepcyjnie do tej z cwicznia 1 (SmallMap)
zaimplementowana w jescze bardziej szczatkowy sposob.
Jest tez prosta funkcja testujaca (void test()), ktora
jest wolana w funkcji main. Mozna w niej zaimplementowac
wlasne testy.
NALEZY ZMODYFIKOWAC I UZUPELNIC CIALA METOD KLASY LISTMAP.
    
@author
Pawel Cichocki, Michal Nowacki

@date  
last revision
- 2006.01.06 Michal Nowacki: wersja polska
- 2005.11.17 Michal Nowacki: constructor
- 2005.11.04 Pawel Cichocki: copied comments from the header
- 2005.11.03 Pawel Cichocki: const_iterator done properly now
- 2005.10.27 Pawel Cichocki: cosmetic changes
- 2005.10.26 Michal Nowacki: removed some method bodies
- 2005.10.25 Pawel Cichocki: wrote it

COPYRIGHT:
Copyright (c) 2005 Instytut Informatyki, Politechnika Warszawska
ALL RIGHTS RESERVED
*******************************************************************************/

#include <assert.h>
#include <algorithm>

#include <iostream>

#ifdef _SUNOS
#include "/home/common/dyd/aisdi/liniowe/ListMap.h" 
#else
#include "ListMap.h"
#endif

//////////////////////////////////////////////////////////////////////////////
// ListMap and ListMap::iterator methods
//////////////////////////////////////////////////////////////////////////////

ListMap::ListMap()
{
	//utworzenie nowego elementu pierscienia i zainicjowanie go.	
	first = new Node(std::make_pair(-1,"SENTINEL"), NULL, NULL);
	//first jest straznikiem, i reprezentuje element nastepny po ostatnim i poprzedni przed pierwszym. 
	first->next = first;
	first->prev = first;
	//dla straznika InternalDataPointer ustawiam na niego samego, zeby odroznic go od pozostalych elementow.
	first->internalDataPointer = first;
};


//konstruktor kopiujacy
ListMap::ListMap( const ListMap& m )
{
	first = new Node(std::make_pair(-1,"SENTINEL"),NULL, NULL);
	first->next = first;
	first->prev = first;
	first->internalDataPointer = first;
	if(!m.empty()){
		for(const_iterator i=m.begin(); i != m.end(); i++)
			unsafe_insert(std::make_pair(i->first, i->second));
	}
};


ListMap::~ListMap()
{
	//usuniecie wszystkich elementow poza pierwszym za ostatnim
   	clear();
	//usuniecie pierwszego za ostatnim, czyli firsta
	delete first;
};


// Wstawienie elementu do mapy.
// @returns Para, której komponent bool jest równy true gdy wstawienie zostalo
//          dokonane, równy false gdy element identyfikowany przez klucz
//          juz istnial w mapie. Iterator ustawiony jest na ten wstawiony 
//          lub istniejacy juz w mapie element.
std::pair<ListMap::iterator, bool> ListMap::insert(const std::pair<Key, Val>& entry)
{
	//wyszukujemy dany klucz
	iterator it;
	it = find(entry.first);
	
	if(it == end())	//it zostal ustawiony przez find(), ktora ustawia na end() kiedy nie znajdzie elementu
		return std::make_pair(unsafe_insert(entry), true);	//...czyli trzeba wstawic. Mozemy uzyc unsafe_entry(), bo nie ma takiego elementu w mapie
	
	//jesli zostal znaleziony, to nadpisujemy Val w odnalezionym elemencie, bo nie moga istniec 2 rozne elementy o tym samym kluczu 
	it->second = entry.second;
	return std::make_pair(it, false);
}


// Wstawienie elementu do mapy.
// Metoda zaklada, ze w mapie nie wystepuje element identyfikowany przez key
ListMap::iterator ListMap::unsafe_insert(const std::pair<Key, Val>& entry)
{
	//utworzenie wstawianego obiektu
	Node* temp;
	temp = new Node(std::make_pair(entry.first, entry.second));
	temp->internalDataPointer = NULL;
	
	iterator tail = end();
	iterator start = begin();

	//jesli nie ma elementow w mapie
	if(empty()){
		first->next = temp;
		first->prev = temp;
		first = temp;
		first->prev = tail.node;
		first->next = tail.node;
		return iterator(first);
	}
	//wstawianie na poczatku
	if(start->first > entry.first){
		temp->next = first;
		temp->prev = first->prev;
		first->prev->next = temp;
		first->prev = temp;
		first = temp;
		return iterator(first);
	}
	
	//wstawianie w srodku. Trzeba najpierw znalezc miejsce do wstawienia.
	while(start != tail){
		if(start->first > entry.first) break;	//przerywamy w momencie gdy klucz wstawianego wpisu jest wiekszy od klucza wskazywanego aktualnie przez iterator. 
		++start;
	}
	//doszlismy do konca lub przeszukanie zostalo przerwane, bo znaleziono miejsce, gdzie nalezy wstawic. Start przechowuje wskazanie na miejsce przed ktorym wstawiamy.
	temp->next = start.node;
	start.node->prev->next = temp;
	temp->prev = start.node->prev;
	start.node->prev = temp;
	start.node = temp;
	return start;
	
}


// Zwraca iterator addresuj¹cy element w mapie dla którego klucz jest równy 
// szukanemu kluczowi lub element za ostatnim gdy szukanego klucza brak w mapie.
ListMap::iterator ListMap::find(const Key& k)
{
	iterator skoczek = first;
	iterator tail = end();
	while(skoczek != tail){
		if(skoczek->first > k) return tail;
		if(skoczek->first == k) return skoczek;
		++skoczek;
	}
	return tail;
}

ListMap::const_iterator ListMap::find(const Key& k) const
{
	const_iterator skoczek = first;
	const_iterator tail = end();
	while(skoczek != tail){
		if(skoczek->first > k) return tail;
		if(skoczek->first == k) return skoczek;
		++skoczek;
	}
	return tail;
}

// Udostepnia wartosc powiazana z kluczem key. Wstawia element do mapy jesli 
// nie istnial.
// @returns Referencje do wartosci powiazanej z kluczem.
ListMap::Val& ListMap::operator[](const Key& k)
{
	iterator i = find(k);
	if(i==end())	//nie znaleziono elementu o takim kluczu, czyli trzeba go wstawic
		return insert(std::make_pair(k,"")).first->second;
	else
		return i->second;
}


// Sprawdzenie czy mapa jest pusta.
bool ListMap::empty( ) const
{
	return (first->prev == first);
}


// Zwraca liczbe elementów w mapie.
ListMap::size_type ListMap::size( ) const
{
	int size = 0;
	iterator skoczek = first;
	while(skoczek != end()){
		++size;
		++skoczek;
	}
	return size;
}


// Zwraca liczbe elementów skojarzonych z kluczem key.
ListMap::size_type ListMap::count(const Key& _Key) const
{
	if(find(_Key) == end()) return 0;
	else return 1;  // this is not a multimap
}


// Usuwa element z mapy.
// @returns iterator adresujacy pierwszy element za usuwanym.
ListMap::iterator ListMap::erase(ListMap::iterator i)
{
	if(i == end()) return i;
	//sprawdzamy, w ktorym miejscu mapy jest iterator. 
	if(i.node == first){	//musimy rozwazyc ten przypadek, zeby na nowo ustawic firsta po usunieciu elementu
		first = first->next;
		first->prev = i.node->prev;
		i.node->prev->next = first;
		delete i.node;
	}
	else{			//usuwamy gdzies ze srodka
		i.node->next->prev = i.node->prev;
		i.node->prev->next = i.node->next;
		delete i.node;
	}
	return ++i;
}


// Usuwa zakres elementów z mapy.
// Zakres jest zdefiniowany poprzez iteratory first i last
// first jest okresla pierwszy element do usuniecia, a last okresla element 
// po ostatnim usunietym elemencie.
// @returns iterator adresujacy pierwszy element za usuwanym.
ListMap::iterator ListMap::erase(ListMap::iterator f, ListMap::iterator l)
{
	if(f==l) return erase(l);
	while(f != l)	erase(f);
	if(f==end())	return f;
	return erase(f);
}


// Usuwa element z mapy.
// @returns Ilosc usunietych elementów.
//          (nie jest to multimapa, wiec moze byc to wartosc 1 lub 0 )
ListMap::size_type ListMap::erase(const Key& key)
{
	iterator i = find(key);
	if(i == end()) return 0;
	else
		erase(i);
	return 1;
}


// Usuniecie wszystkich elementów z mapy.
void ListMap::clear()
{
	//wywoluje metode erase dla przedzialu okreslonego przez iteratory
	//ustawiajac granice na begin() i end() usuwamy wszystkie elementy
	//poza straznikiem
	erase(begin(), end());
	
	//trzeba teraz poprawnie ustawic straznika
	iterator tail = end();
	first = tail.node;
	first->next = first;
	first->prev = first;
	return;
}

// Porównanie strukturalne map.
// Czy reprezentacja danych jest identyczna.
// Zwraca true jeœli wewnêtrzne struktury map s¹ identyczne.
bool ListMap::struct_eq(const ListMap& another) const
{
	return info_eq(another);
}

// Porównanie informacyjne map.
// Czy informacje trzymane w mapach s¹ identyczne.
// Zwraca true jeœli mapy zwieraj¹ takie same pary klucz-wartoœæ.
bool ListMap::info_eq(const ListMap& another) const
{
	iterator i(first);
	iterator j(another.first);
	for( ; i!=end(); i++,j++){
		if(j == end()) return false;
		if(i->first != j->first || i->second != j->second) return false;
	}
	if( i==end() && j!=another.end()) return false;
	return true;
}


// preincrementacja
ListMap::const_iterator& ListMap::const_iterator::operator++()
{
	if(node->next->internalDataPointer == NULL)
		node = node->next;
	return *this;
}

// postincrementacja
ListMap::const_iterator ListMap::const_iterator::operator++(int)
{
	const_iterator tensam(*this);
	if(node->next->internalDataPointer == NULL)
		node = node->next;
	return tensam;
}

ListMap::const_iterator& ListMap::const_iterator::operator--()
{
	if(node->prev->internalDataPointer == NULL)	
		node = node->prev;
	return *this;
}

// postincrementacja
ListMap::const_iterator ListMap::const_iterator::operator--(int)
{
	const_iterator tensam(*this);
	if(node->prev->internalDataPointer == NULL)
		node = node->prev;
	return tensam;
}

/// Zwraca iterator addresuj¹cy pierwszy element w mapie.
ListMap::iterator ListMap::begin()
{ 
	return iterator(first);
}

/// Zwraca iterator addresujacy pierwszy element w mapie.
ListMap::const_iterator ListMap::begin() const
{ 
	return const_iterator(first);
}

/// Zwraca iterator addresuj¹cy element za ostatnim w mapie.   
ListMap::iterator ListMap::end()
{ 
	return iterator(first->prev);
}

/// Zwraca iterator addresuj¹cy element za ostatnim w mapie.   
ListMap::const_iterator ListMap::end() const
{ 
	return const_iterator(first->prev);
}

//////////////////////////////////////////////////////////////////////////////
// SmallMap
//////////////////////////////////////////////////////////////////////////////

/// Przyk³ad map'y z implementacj¹ podobn¹ do std::map.
/// To jest jedynie przyk³ad!!!
/// Normalnie implementacja powinna opieraæ siê o drzewo lub tablicê haszuj¹c¹.
template <class Key, class Val>
class SmallMap
{
   std::pair<Key, Val> tab[2];
   int isOcupied[2];
public:

   SmallMap()
   {
      for(unsigned i=0; i<2; ++i)
      {
         isOcupied[i]=0;
      }
   }

   typedef std::pair<Key, Val>* iterator;   ///< Ka¿dy wskaznik jest te¿ iteratorem.

   iterator begin() { return tab; }
   iterator end() { return tab+2; }

   Val& operator[](const Key& k)
   {
      static Val val;
      for(unsigned i=0; i<2; ++i)
      {
         if(isOcupied[i]&&tab[i].first==k)
            return tab[i].second;
      }
      // create
      for(unsigned i=0; i<2; ++i)
      {
         if(!isOcupied[i])
         {
            tab[i].first=k;
            isOcupied[i]=1;
            return tab[i].second;
         }
      }
      //std::cout<<"Out of space! You should not put more than two Key-Value pairs into the SmallMap.\n";
      std::cerr<<"Out of space! You should not put more than two Key-Value pairs into the SmallMap.\n";
      //assert(0);
      return val;   // Ma³o sensowne, ale to jest tylko przyk³¹d
   }
};
      

//////////////////////////////////////////////////////////////////////////////
// Testy
//////////////////////////////////////////////////////////////////////////////

/// Funckcja pomocnicza do wypisania elementów.
void print(const std::pair<int, std::string>& p)
{
   std::cout<<p.first<<", "<<p.second<<std::endl;
}

#include <map>

/// Testy u¿ytkownika
void test()
{
   // A typedef used by the test.
   //typedef std::map<int, std::string> TEST_MAP;
   //typedef SmallMap<int, std::string> TEST_MAP;
   typedef ListMap TEST_MAP;

   std::cout << "Testy uzytkownika" << std::endl;

        TEST_MAP m;  
   
   m[2] = "Merry";
   m[4] = "Jane";
   m[8] = "Korwin";
   m[4] = "Magdalena";

   for_each(m.begin(), m.end(), print );
   //system("PAUSE");
}

//////////////////////////////////////////////////////////////////////////////
// main - jest w pliku /home/common/dyd/aisdi/z2/main.cc
//////////////////////////////////////////////////////////////////////////////

//int main()
//{
//   std::cout << "AISDI cwiczenie 2: wchodze do funkcji main." << std::endl;
//   test();
//   // Biblioteka z bardziej rygorystyczna wersja tych testow bedzie udostepniona na nastepnych zajeciach.
//   Test2();
//   //system("PAUSE");
//   return EXIT_SUCCESS;
//}
