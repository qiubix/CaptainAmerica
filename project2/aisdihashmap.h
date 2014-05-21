/** 
@file aisdihashmap.h

AISDIHashMap and related functions interface.
      
@author
Pawel Cichocki

@date  
last revision
- 2006.03.24 Pawel Cichocki: wrote it

COPYRIGHT:
Copyright (c) 2006 Instytut Informatyki, Politechnika Warszawska
ALL RIGHTS RESERVED
*******************************************************************************/


#include <utility>
#include <string>
#include <iostream>
#include <iterator>

#define PRINT(x) std::cout << #x"\n";


/// Default keys' comparing function for AISDIHashMap - it uses
/// operator== by default.
/// @returns 0 if keys are equal, non-zero otherwise.
/// @param key1 First key to compare. 
/// @param key2 Second key to compare. 
template <class Key>   
inline int _compFunc(const Key& key1,const Key& key2)
{
   return (key1==key2);
};




#define MAX 64000

/// A map with a similar interface to std::map.
template<class K, class V,
         unsigned hashFunc(const K&),
         int compFunc(const K&,const K&)=&_compFunc<K> >
class AISDIHashMap
{
public:
	typedef K key_type;
	typedef V value_type;
	typedef unsigned size_type;
	typedef std::pair<key_type,value_type> Para;
	
	
	//struktura opakowujaca element hashmapy. Kazdy element przechowuje wskaznik na nastepny i poprzedni w piercieniu
	//oraz nastepny i poprzedni w miniliscie hashmapy
	struct HNode{
		HNode* pnext;
		HNode* pprev;
		HNode* lnext;
		HNode* lprev;
		Para dane;
		HNode():pnext(NULL),pprev(NULL),lnext(NULL),lprev(NULL){};
		HNode(const std::pair<K,V>& d):pnext(NULL),pprev(NULL),lnext(NULL),lprev(NULL),dane(d){};
	};
	
protected:
	HNode* tablica[MAX];	//tablica wskaznikow na wezly pierscienia
	HNode* Sentinel;		//straznik pierscienia

public: 
	//konstruktor domyslny HashMapy. Ustawia odpowiednio straznika
	AISDIHashMap(){
		//PRINT(konstruktor);
		//utworzenie nowego elementu
		Sentinel = new HNode();
		Sentinel->pnext = Sentinel;
		Sentinel->pprev = Sentinel;
		for(size_type i=0; i<MAX; i++) tablica[i] = NULL;
	}
	
	//destruktor HashMapy. Wywoluje clear
	~AISDIHashMap(){
		//PRINT(~AISDIHashMap);
		if(!empty()) clear();
		delete Sentinel;
	}

	/// Coping constructor.
	explicit AISDIHashMap(const AISDIHashMap<K, V, hashFunc, compFunc>& a);

	/// const_iterator.
	class const_iterator : public std::iterator<std::forward_iterator_tag, Para >
	{
		friend class AISDIHashMap;
	public: 
		HNode* node;
		typedef std::pair<key_type, value_type> T;
		const_iterator(){}
		const_iterator(HNode* x):node(x){}
		const_iterator(const const_iterator& a){node = a.node;}
		
		inline const T* operator->() const{
			return &(node->dane);
		}
		inline const T& operator*() const{
			return node->dane;
		}
		inline bool operator==(const const_iterator& a) const{
			return node == a.node;
		}
		inline bool operator!=(const const_iterator& a) const{
			return node != a.node;
		}
		
		const_iterator& operator++(){
			if(node->pnext != Sentinel)
				node = node->pnext;
			return *this;
		}
		const_iterator operator++(int){
			if(node->pnext != Sentinel){
				HNode* temp = new HNode();
				temp = node;
				node = node->pnext;
				return temp;
			}
			return this;
		}	
		const_iterator& operator--(){
			if(node->pprev !=Sentinel)
				return const_iterator(node->pprev);
		}
		const_iterator operator--(int);
	};
	/// iterator.
	class iterator : public const_iterator
	{
	public:
		friend class AISDIHashMap;
		HNode* node;
		typedef std::pair<key_type, value_type> T;
		iterator(){}
		iterator(HNode* x):const_iterator(x){}
		iterator(const iterator& a){node = a.node; }
		iterator(const const_iterator& a) : const_iterator(a){}
		
		inline T& operator*(){
			return node->dane;
		}
		inline T* operator->(){
			return &(node->dane);
		}
		inline bool operator==(const iterator& a){
			return node == a.node;
		}
		inline bool operator!=(const iterator& a ){
			return node != a.node;
		}
		iterator& operator++(){
			if(node->pnext != Sentinel)
				node = node->pnext;
			return *this;
		}
		iterator operator++(int){
			HNode* tmp = node;
			if(node->pnext != Sentinel)
				node = node->pnext;
			return tmp;
		}
		iterator& operator--(){
			if(node->pprev != Sentinel)
				node = node->pprev;
			return *this;
		}
		iterator operator--(int){
			HNode* tmp = node;
			if(node->pprev != Sentinel)
				node = node->pprev;
			return tmp;
		}
	};

	friend class const_iterator;
	friend class iterator;
	
	/// Returns an iterator addressing the first element in the map.
	inline iterator begin(){
		iterator it;
		it.node = Sentinel->pnext;
		return it;
	}
	inline const_iterator begin() const{
		const_iterator it;
		it.node = Sentinel->pnext;
		return it;
	}

	/// Returns an iterator that addresses the location succeeding the last element in a map.
	inline iterator end(){
		iterator it;
		it.node = Sentinel;
		return it;
	}
	inline const_iterator end() const{
		const_iterator it;
		it.node = Sentinel;
		return it;
	}
   
	/// Inserts an element into the map.
	/// @returns A pair whose bool component is true if an insertion was
	///          made and false if the map already contained an element
	///          associated with that key, and whose iterator component coresponds to
	///          the address where a new element was inserted or where the element
	///          was already located.
	std::pair<iterator, bool> insert(const std::pair<K, V>& entry){
		iterator it = find(entry.first);
		if(it.node != Sentinel){
			return std::make_pair(it,false);
		}
		else{
			//utworzenie nowego elementu
			HNode* tmp = new HNode(entry);
			//wstawienie go na poczatku pierscienia
			Sentinel->pnext->pprev = tmp;
			tmp->pnext = Sentinel->pnext;
			Sentinel->pnext = tmp;
			tmp->pprev = Sentinel;
			//wstawianie do minilisty
			size_type Index = hashFunc(entry.first);		//znajduje miejsce w tablicy hashujacej
			//nastepna wartosc na miniliscie po wstawianym elemencie to ten akurat znajdujacy sie w danym polu tablicy
			tmp->lnext = tablica[Index];					
			tablica[Index] = tmp;							//nowy element zostaje wpisany w pole tablicy
			if(tmp->lnext != NULL)							//jesli w polu tablicy jakis element sie znajdowal
				tmp->lnext->lprev = tmp;					//...to jego poprzednim elementem na miniliscie bedzie ten wstawiany
			return std::make_pair(iterator(tmp), true);
		}
	}

	/// Returns an iterator addressing the location of the entry in the map
	/// that has a key equivalent to the specified one or the location succeeding the
	/// last element in the map if there is no match for the key.
	iterator find(const K& k){
		size_type Index = hashFunc(k);
		iterator it;
		it.node = tablica[Index];
		while(it.node != NULL){
			if(it->first == k)
				return it;
			else
				it.node = it.node->lnext;
		}
		return end();
	}
	const_iterator find(const K& k) const{
		size_type Index = hashFunc(k);
		const_iterator it;
		it.node = tablica[Index];
		while(it.node != NULL){
			if(it->first == k)
				return it;
			else
				it.node = it.node->lnext;
		}
		return end();
	}
 
	/// Inserts an element into a map with a specified key value
	/// if one with such a key value does not exist.
	/// @returns Reference to the value component of the element defined by the key.
	V& operator[](const K& k){
		(insert(std::make_pair(k,0)).first)->second;
		return (insert(std::make_pair(k,0)).first)->second;
	}

	/// Tests if a map is empty.
	bool empty( ) const{
		return Sentinel->pnext == Sentinel;
	}

	/// Returns the number of elements in the map.
	size_type size() const{
		size_type ile;
		for(HNode* skoczek = Sentinel->pnext; skoczek != Sentinel; skoczek = skoczek->pnext) ile++;
		return ile;
	}

	/// Returns the number of elements in a map whose key matches a parameter-specified key.
	size_type count(const K& _Key) const;

	/// Removes an element from the map.
	/// @returns The iterator that designates the first element remaining beyond any elements removed.
	iterator erase(iterator i){
		//sprawdzenie, czy nie chcemy usunac straznika
		if(i==end()) return i;
		//dla elementow, ktore nie sa na koncu minilisty
		if(i.node->lnext != NULL)
			i.node->lnext->lprev = i.node->lprev;
		//dla elementow, ktore nie sa pierwsze na miniliscie
		if(i.node->lprev != NULL)
			i.node->lprev->lnext = i.node->lnext;
		else	//jesli pierwsze na miniliscie , to trzeba przepisac do tablicy
			tablica[hashFunc(i->first)] = i.node->lnext;
		//dla wszystkich elementow
		i.node->pprev->pnext = i.node->pnext;
		i.node->pnext->pprev = i.node->pprev;
		i.node = i.node->pnext;
		//delete i.node;
		return i;
	}
   
	/// Removes a range of elements from the map.
	/// The range is defined by the key values of the first and last iterators
	/// first is the first element removed and last is the element just beyond the last elemnt removed.
	/// @returns The iterator that designates the first element remaining beyond any elements removed.
	iterator erase(iterator first, iterator last){
		if(first.node==last.node)
			return last;
		while(first.node != last.node){
				first = erase(first);
		return last;
		}
	}
   
	/// Removes an element from the map.
	/// @returns The number of elements that have been removed from the map.
	///          Since this is not a multimap itshould be 1 or 0.
	size_type erase(const K& key){
		iterator it = find(key);
		if(it == end())	return 0;
		erase(it);
		return 1;	
	};

	/// Erases all the elements of a map.
	void clear( ){
		erase(begin(),end());
	};
};


template<class K>
inline unsigned hashF(const K& k){
	unsigned h=static_cast<unsigned int>(k.size()); 
	for(int i=0;i<static_cast<unsigned int>(k.size());i++){
		h=(h<<5)^(h>>27)^k[i];
	}
return h%MAX; 
};
