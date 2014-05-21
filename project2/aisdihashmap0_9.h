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



//Struktura Node bedaca szablonem dla elementu
template <class K, class V>
struct Node{
	typedef std::pair<K,V> Para;
	Node* pnext;		//wskaznik na nastepny element w pierscieniu
	Node* pprev;
	Node* lnext;		//wskaznik na nastepny element w hashmapie
	Node* lprev;
	Para dane;
	Node():pnext(NULL),pprev(NULL),lnext(NULL),lprev(NULL){};
	Node(const std::pair<K,V>& d):pnext(NULL),pprev(NULL),lnext(NULL),lprev(NULL),dane(d){};
};



//Rozmiar tablicy 
#define MAX 65535




/// A map with a similar interface to std::map.
template<class K, class V,
         unsigned hashFunc(const K&),
         int compFunc(const K&,const K&)=&_compFunc<K> >
class AISDIHashMap
{
public:
	typedef K key_type;
	typedef V value_type;
	typedef Node<key_type,value_type> HNode;
	typedef unsigned size_type;
protected:
	HNode* tablica[MAX];	//tablica przechowujaca indeksy
	size_type ile;			//przechowuje liczbe elementow mapy
	HNode* sentinel;
	
public:
	//konstruktor domyslny
	AISDIHashMap():ile(0){
		//utworzenie straznika dla pierscienia. Wskaznik na straznika jest w ostatniej komorce tablicy
		sentinel = new HNode();
		sentinel->pnext = sentinel;
		sentinel->pprev = sentinel;
		//wyzerowanie pozostalej czesci tablicy
		for(int i=0; i<MAX; i++)	tablica[i] = NULL;
	};
	
	//destruktor
	~AISDIHashMap(){
		std::cout << "wszedl do destruktora\n";
		if(!empty()) clear();
		std::cout << "przeszedl clear()\n";
		delete sentinel;
	};

	
	/// Coping constructor.
	explicit AISDIHashMap(const AISDIHashMap<K, V, hashFunc, compFunc>& a);

	
	/// const_iterator.
	class const_iterator : public std::iterator<std::forward_iterator_tag, std::pair<key_type, value_type> >
	{
	public:
		typedef std::pair<key_type,value_type> T;
	protected:
		HNode* node;
		const_iterator(HNode* x):node(x){};
		friend class AISDIHashMap;
	public:
		const_iterator(){};
		const_iterator(const const_iterator& it):node(it.node){};
		
		inline const T& operator*() const{
			return node->dane;
		};
		inline const T* operator->() const{
			return &(node->dane);
		};
		
		inline const bool operator==(const const_iterator& it) const{
			return node==it.node;
		};
		inline const bool operator!=(const const_iterator& it) const{
			return node!=it.node;
		};
		
		inline const_iterator& operator++(){
			if(node->pnext != sentinel)
				node = node->pnext;
			return *this;
		}
		
		inline const_iterator operator++(int){
			HNode* tmp = node;
			if(node->pnext !=sentinel)
				node = node->pnext;
			return tmp;
		}
		
		inline const_iterator& operator--(){
			if(node->pprev==sentinel)
				node = node->pprev;
			return *this;
		}
		
		inline const_iterator operator--(int){
			HNode* tmp = node;
			if(node->pprev != sentinel)
				node = node->pprev;
			return tmp;
		}
	};
	
	
	/// iterator.
	class iterator : public const_iterator
	{
	public:
		HNode* node;
		iterator(HNode* x):const_iterator(x){};
		friend class AISDIHashMap;
		typedef std::pair<key_type,value_type> T;
		iterator(){};
		iterator(const const_iterator& it):const_iterator(it){};
		iterator(const iterator& it){ node = it.node; };
		
		inline T& operator*() const{
			return node->dane;
		}
		inline T* operator->() const{
			return &(node->dane);
		}
		
		inline bool operator==(const iterator& it) const{
			return node == it.node;
		}
		inline bool operator!=(const iterator& it) const{
			return node != it.node;
		}
		
		inline iterator& operator++(){
			if(node->pnext != sentinel)
				node = node->pnext;
			return *this;
		}	
		inline iterator operator++(int){
			HNode* tmp = node;
			if(node->pnext != sentinel)
				node = node->pnext;
			return tmp;
		}
		inline iterator& operator--(){
			if(node->pprev != sentinel)
				node = node->pprev;
			return *this;
		}
		inline iterator operator--(int){
			HNode* tmp;
			if(node->pprev != sentinel)
				node = node->pprev;
			return tmp;
		}
	};

	/// Returns an iterator addressing the first element in the map.
	inline iterator begin(){
		return iterator(sentinel->pnext);
	}
	
	inline const_iterator begin() const{
		return const_iterator(sentinel->pnext);
	}

	/// Returns an iterator that addresses the location succeeding the last element in a map.
	inline iterator end(){
		return iterator(sentinel);
	}
	
	inline const_iterator end() const{
		return const_iterator(sentinel);
	}
	
	
	/// Inserts an element into the map.
	/// @returns A pair whose bool component is true if an insertion was
	///          made and false if the map already contained an element
	///          associated with that key, and whose iterator component coresponds to
	///          the address where a new element was inserted or where the element
	///          was already located.
	std::pair<iterator, bool> insert(const std::pair<K, V>& entry){
		std::cout << "Poczatek inserta\n";
		HNode* tmp = new HNode(entry);
		std::cout << "Stworzyl HNode(entry)\n";
		size_type Index = hashFunc(entry.first);
		if(empty()){
			std::cout<< "If empty\n";
			tablica[Index] = tmp;
			std::cout << "1\n";
			tablica[Index]->pnext = sentinel;
			std::cout << "2\n";
			tablica[Index]->pprev = sentinel;
			std::cout << "3\n";
			sentinel->pnext = tablica[Index];
			std::cout << "4\n";
			sentinel->pprev = tablica[Index];
			std::cout << "5\n";
			iterator it(tablica[Index]);
			ile++;
			return std::make_pair(it,true);
		}
		//znajduja sie elementy w hashmapie
		else{
			//Nie ma w mapie elementow o takim hashu jak entry
			std::cout << "else 1\n";
			if(tablica[Index]==NULL){
				tablica[Index] = tmp;
				tablica[Index]->pnext = sentinel->pnext;
				tablica[Index]->pprev = sentinel;
				sentinel->pnext->pprev = tablica[Index];
				sentinel->pnext = tablica[Index];
				iterator it(tablica[Index]);
				ile++;
				return std::make_pair(it,true);
			}
			//sa w mapie elementy o takim hashu jak entry
			else{
				HNode* skoczek = tablica[Index];
				while(skoczek!=NULL){
					if(entry.second == skoczek->dane.second){
						skoczek->dane.second = entry.second;	//nadpisanie zawartosci
						delete tmp;								//usuniecie niepotrzebnie utworzonego elementu (bo nie musimy go wstawiac)
						return std::make_pair(skoczek,false);	
					}
					skoczek = skoczek->lnext;
				}
				//nie znaleziono identycznego elementu. dodanie elementu na poczatku minilisty
				skoczek = tablica[Index];		//ustawienie skoczka na poczatku minilisty
				tablica[Index] = tmp;
				tmp->lnext = skoczek;
				skoczek->lprev = tmp;
				skoczek = skoczek->lprev;
				ile++;
				return std::make_pair(skoczek, true);
			}
		}	
	}
   
   

	/// Returns an iterator addressing the location of the entry in the map
	/// that has a key equivalent to the specified one or the location succeeding the
	/// last element in the map if there is no match for the key.
	iterator find(const K& k){
		size_type Index = hashFunc(k);
		if(tablica[Index])
			return iterator(tablica[Index]);
		return iterator(sentinel);
	}
	
	const_iterator find(const K& k) const{
		size_type Index = hashFunc(k);
		if(tablica[Index])
			return const_iterator(tablica[Index]);
		return const_iterator(sentinel);
	}
 
	/// Inserts an element into a map with a specified key value
	/// if one with such a key value does not exist.
	/// @returns Reference to the value component of the element defined by the key.
	V& operator[](const K& k){
		iterator it = find(k);
		if(it!=end())
			return it->second;
		else{
			std::pair<iterator,bool> para = insert(std::make_pair(k,0));
			return para.first->second;
		}
	}

   
	/// Tests if a map is empty.
	bool empty( ) const{
		return ile<1;
	};

	
	/// Returns the number of elements in the map.
	size_type size() const{
		return ile;
	};

	
   /// Returns the number of elements in a map whose key matches a parameter-specified key.
   size_type count(const K& _Key) const;

	/// Removes an element from the map.
	/// @returns The iterator that designates the first element remaining beyond any elements removed.
	/*iterator erase(iterator i){
		if(i == end())
			return i;
		(i.node->pnext)->pprev = i.node->pprev;
		(i.node->pprev)->pnext = i.node->pnext;
		if(i.node->lprev == NULL){
			if(i.node->lnext == NULL)
				tablica[hashFunc(i->first)] = NULL;
			else{
				tablica[hashFunc(i->first)] = i.node->lnext;
				i.node->lnext->lprev = NULL;
			}
		}
		else{
			i.node->lprev->lnext = i.node->lnext;
			if(i.node->lnext != NULL)
				i.node->lnext->lprev = i.node->lprev;
		}
		HNode* temp;
		temp = i.node;
		i.node = i.node->pnext;
		delete temp;
		--ile;
		return i++;
	}*/
	iterator erase(iterator i)
	{
	std::cout << "wszedl do erase(iterator)\n";
	if (i.node==sentinel) //sprawdzam czy przypadkiem nie chcemy usunac straznika
		return i;
	std::cout << "1\n";
	if (i.node->lnext!=NULL){ //sprawdzam czy malej liscie nie jest to ostatni element
		std::cout<<"pod ifem\n";
		//if(i.node->lnext == NULL) {	std::cout << "1\n";	}
		//if(i.node->lnext->lprev == NULL) {std::cout << "2\n";}
		if(i.node->lprev == NULL) {std::cout<< "3\n";}
		std::cout << "4\n";
		i.node->lnext->lprev=i.node->lprev;
		std::cout << "5\n";
		}
	std::cout << "2\n";
	if (i.node->lprev==NULL){ //czy nie jest to pierwszy element w malej liscie
		std::cout << "2-0\n";
		tablica[hashFunc(i->first)]=i.node->lnext; 
		std::cout << "2-1\n";
	}
	else{ 
		i.node->lprev->lnext=i.node->lnext;
		std::cout << "2-2\n";
	}
	//usuniete z malej listy
	std::cout << "3\n";
	i.node->pprev->pnext=i.node->pnext;
	i.node->pnext->pprev=i.node->pprev;
	//usuniete z pierscienia
	i.node=i.node->pnext;
	std::cout << "koniec erase(iterator)\n";
	return i;
	};
	
			
	
   
   /// Removes a range of elements from the map.
   /// The range is defined by the key values of the first and last iterators
   /// first is the first element removed and last is the element just beyond the last elemnt removed.
   /// @returns The iterator that designates the first element remaining beyond any elements removed.
	iterator erase(iterator first, iterator last){
		std::cout << "wszedl do erase (first,last)\n";
		while(first.node != last.node){
			std::cout << "kupa\n";
			first = erase(first);
		}
		std::cout << "po while\n";
		if(first == end())
			return first;
		std::cout << "prawie koniec\n";
		return erase(first);
	}
   
	/// Removes an element from the map.
	/// @returns The number of elements that have been removed from the map.
	///          Since this is not a multimap itshould be 1 or 0.
	size_type erase(const K& key){
		iterator it = find(key);
		return (it!=end());
	}
		

	/// Erases all the elements of a map.
	void clear( ){
		std::cout << "wszedl do clear\n";
		erase(begin(),end());
	};
};


/*
template<class K>
inline unsigned hashF(const K& k)
{
	std::cout<<"wszedl do hashF\n";
	unsigned h=static_cast<unsigned int>(k.size());
	for(int i=0;i<static_cast<unsigned int>(k.size());i++)
	    h=(h<<5)^(h>>27)^k[i];
	std::cout<<"zaraz koniec hashF\n";
	return h%65535;	
};*/
template<class K>
inline unsigned hashF(const K& k)
{
std::cout<<"Wszedl bla bal do hashF\n";
 unsigned h=static_cast<unsigned int>(k.size());
 std::cout<<"1\n";
 for(int i=0;i<static_cast<unsigned int>(k.size());i++){
	std::cout<<"wszedl do for\n";
     h=(h<<5)^(h>>27)^k[i];
	 std::cout<<"koniec obrotu petli\n";
	 }
std::cout<<"juz prawie koniec hashF\n";
 return h%MAX; 
};


///Alternatywny insert
/*
HNode* tmp = new HNode(entry);
		size_type Index = hashFunc(entry.first);
		if(empty()){
			tmp->pnext = Sentinel;
			tmp->pprev = Sentinel;
			Sentinel->pnext = tmp;
			Sentinel->pprev = tmp;
			tablica[Index] = tmp;
			iterator it(tablica[Index]);
			++ile;
			return std::make_pair(it, true);
		}
		else{
			if(!tablica[Index]){
				tmp->pnext = Sentinel->pnext;
				tmp->pprev = Sentinel;
				Sentinel->pnext->pprev = tmp;
				Sentinel->pnext = tmp; 
				tablica[Index] = tmp;
				iterator it(tablica[Index]);
				++ile;
				return std::make_pair(it,true);
			}
			else{
				HNode* skoczek = tablica[Index];
				while(skoczek != NULL){
					if(skoczek->dane.second == entry.second){
						delete tmp;
						skoczek->dane.second = entry.second;
						return std::make_pair(iterator(skoczek), false);
					}
					skoczek = skoczek->lnext;
				}
				skoczek = tablica[Index];
				tmp->lnext = skoczek;
				skoczek->lprev = tmp;
				tmp->pnext = Sentinel->pnext;
				tmp->pprev = Sentinel;
				Sentinel->pnext->pprev = tmp;
				Sentinel->pnext = tmp;
				tablica[Index] = tmp;
				skoczek = skoczek->lprev;
				++ile;
				return std::make_pair(iterator(skoczek), true);
			}
		}
		*/