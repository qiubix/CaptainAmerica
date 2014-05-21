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
#include <iostream>

/// Default keys' comparing function for AISDIHashMap
/// @returns 0 if keys are equal, non-zero where keys are different
/// @param key1 First key to compare. 
/// @param key2 Second key to compare. 

const int N = 64000;

template <class Key>   
inline int _compFunc(const Key& key1,const Key& key2){
  return (key1==key2);
};

/// A map with a similar interface to std::map.
template<class K,class V,unsigned hashFunc(const K&),
        int compFunc(const K&,const K&)=&_compFunc<K> >
class AISDIHashMap{
public:
  struct Node;
  typedef K key_type;
  typedef V value_type;
  typedef unsigned int size_type;

private:
  Node *(HashT[N+1]); //tablica wskaznikow
  size_type ile_elementow;

public:
  AISDIHashMap();
  ~AISDIHashMap();

  //void test();

  /// Coping constructor.
  explicit AISDIHashMap(const AISDIHashMap<K, V, hashFunc, compFunc> &a);
  class const_iterator{
  protected:
    Node* node;
    friend class AISDIHashMap<key_type,value_type,hashFunc,compFunc>;
    const_iterator(Node* x) : node(x) {}

  public:
    typedef std::pair<key_type,value_type> T;

    const_iterator() {}
    const_iterator(const const_iterator& a) : node(a.node) {}

    const T& operator*() const{
      return node->data;
    }

    const T* operator->() const{
      return &(node->data);
    }

    // preincrementacja
    const_iterator& operator++(){
      node=node->nast;
      return *this;
    }    

    // postincrementacja
    const_iterator operator++(int){
      Node *tmp=node;
      node=node->nast;
      return tmp;      
    }
    // predekrementacja
    const_iterator& operator--(){
      node=node->poprz;
      return *this;
    }
    // postdekrementacja
    const_iterator operator--(int){
      Node *tmp=node;
      node=node->poprz;
      return tmp;
    }
    bool operator==(const const_iterator& a) const{
      return node == a.node;
    }

    bool operator!=(const const_iterator& a) const{
      return node != a.node;
    }
  };

  /// iterator.
  class iterator: public const_iterator{
    iterator(Node* x) : const_iterator(x) {}
     friend class AISDIHashMap;

  public:
     typedef std::pair<key_type,value_type> T;
     iterator() {}
     iterator(const iterator& a) { const_iterator::node = a.node; }

     T& operator*() const{
       return const_iterator::node->data;
     }
     T* operator->() const{
       return &(const_iterator::node->data);
     }

     iterator& operator++(){  // preincrementacja
       ++(*(const_iterator*)this);
       return (*this);
     }

     iterator operator++(int){  // postincrementacja
       iterator temp = *this;
       ++*this;
       return temp;
     }

     iterator& operator--(){  // predekrementacja
       --(*(const_iterator*)this);
       return (*this);
     }

     iterator operator--(int){  // postdekrementacja
       iterator temp = *this;
       --*this;
       return temp;
     }
   };

  /// Returns an iterator addressing the first element in the map.
  iterator begin() const{
    return HashT[N]->nast;
  }
  //inline const_iterator const_begin() const;

  /// Returns an iterator that addresses the location succeeding
  /// the last element in a map.
  inline iterator end(){
    return HashT[N];
  }
  //inline iterator const_end() const;
   
  /// Inserts an element into the map.
  /// @returns A pair whose bool component is true if an insertion was
  ///          made and false if the map already contained an element
  ///          associated with that key, and whose iterator component coresponds
  ///          to the address where a new element was inserted or where
  ///          the element was already located.
  std::pair<iterator, bool> insert(const std::pair<K, V>& entry);

  /// Returns an iterator addressing the location of the entry in the map
  /// that has a key equivalent to the specified one or the location succeeding
  /// the last element in the map if there is no match for the key.
  iterator find(const K& k);
  //const_iterator find(const K& k) const;
 
  /// Inserts an element into a map with a specified key value
  /// if one with such a key value does not exist.
  /// @returns Reference to the value component of the element defined
  ///          by the key.
  V& operator[](const K& k);   

  /// Tests if a map is empty.
  bool empty( ) const {return !((bool)ile_elementow); }

  /// Returns the number of elements in the map.
  size_type size() const {return ile_elementow;}

  /// Returns the number of elements in a map whose key matches
  /// a parameter-specified key.
  size_type count(const K& _Key) const{
    if(find(_Key)!=end()) //jesli znaleziono element o tym kluczu
      return 1;
    return 0;             //jesli nie znaleziono
  }

  /// Removes an element from the map.
  /// @returns The iterator that designates the first element
  /// remaining beyond any elements removed.
  iterator erase(iterator i);
   
  /// Removes a range of elements from the map.
  /// The range is defined by the key values of the first and last iterators
  /// first is the first element removed and last is the element just beyond
  /// the last elemnt removed.
  /// @returns The iterator that designates the first element remaining
  /// beyond any elements removed.
  iterator erase(iterator first, iterator last);
   
  /// Removes an element from the map.
  /// @returns The number of elements that have been removed from the map.
  ///          Since this is not a multimap it should be 1 or 0.
  size_type erase(const K& key);

  /// Erases all the elements of a map.
  void clear();
    
  struct Node{
    std::pair<K, V> data;
    Node *nast, *poprz;
    bool last;      //true, gdy jedyny element o tym kluczu w tablicy
    bool sentinel;  //true, tylko dla straznika
    Node(): nast(0), poprz(0){}
    Node(std::pair<K, V> d, Node* p, Node* n, bool l, bool s=false):
      data(d) , poprz(p) , nast(n), last(l), sentinel(s){}
  };
};



////////////////////////////////////////////////////////////////////////////////
///  DEFINICJE FUNKCJI
////////////////////////////////////////////////////////////////////////////////

//Konstruktor domyslny
template<class K, class V, unsigned hashFunc(const K&),
        int compFunc(const K&,const K&)>
AISDIHashMap<K,V,hashFunc,compFunc>::
AISDIHashMap(): ile_elementow(0){
  //zerujemy na poczatku cala tablice
  Node **wsk = HashT;
  for(int i=0; i<N;i++,wsk++){
    *wsk=0;
  }

  //ustawiamy wartosci straznikowi (wsk pokazuje na wskaznik na straznika)
  (*wsk) = new Node;
  (*wsk)->nast = (*wsk)->poprz = (*wsk); //nast i poprz na samego siebie
  (*wsk)->last=true;
  (*wsk)->sentinel=true;
  
}

//Destruktor
template<class K, class V, unsigned hashFunc(const K&),
        int compFunc(const K&,const K&)>
AISDIHashMap<K,V,hashFunc,compFunc>::
~AISDIHashMap(){
  clear();  //czyscimy cala tablice
  delete HashT[N];
}

//Konstruktor kopiujacy
template<class K, class V, unsigned hashFunc(const K&),
        int compFunc(const K&,const K&)>
AISDIHashMap<K,V,hashFunc,compFunc>::
AISDIHashMap(const AISDIHashMap<K, V, hashFunc, compFunc>& a): ile_elementow(0){
  //zerujemy cala tablice
  Node **wsk = HashT;
  for(int i=0; i<N ; i++){
    *(wsk++) = 0;
  }
  (*wsk) = new Node;  
  (*wsk)->last=true;
  (*wsk)->sentinel=true;

  Node **tablica = HashT[N],    //wskaznik po tablicy hashujacej (this)
       *wsk1=HashT[N],          //wskaznik po pierscieniu (this)
       *wsk2=a.HashT[N]->nast;  //wskaznik po pierscieniu (a)

  while(wsk2!=a.HashT[N]){
    wsk1->nast = new Node(wsk2->data, wsk1, 0, wsk2->last, false);
    //jesli wsk2 pokazuje na pierwszy element pod tym hashem, to trzeba
    //wprowadzic odpowiednie zmiany w tablicy hashujacej
    if(wsk2->prev->last)
      HashT[hashFunc(wsk2->data->first)] = wsk1->nast;
    wsk1=wsk1->nast;
    wsk2=wsk2->nast;
  }    
    
  wsk1->nast=HashT[N];
  wsk1->nast->poprz=wsk1;

  std::cout<<"TEST::Konstruktor kopiujacy"<<std::endl;
}

//Czyszczenie calej tablicy
template<class K, class V, unsigned hashFunc(const K&),
        int compFunc(const K&,const K&)>
void AISDIHashMap<K,V,hashFunc,compFunc>::clear(){
  //ustawiamy wskaznik na pierwszy element w liscie i usuwamy elementy
  /*Node *cur=HashT[N]->nast, *next;
  for(size_type i=0; i<ile_elementow; i++){
    next=cur->nast;
    delete cur;
    cur=next;
    std::cout<<"TEST::clear: Usunalem "<<i+1<<". element"<<std::endl;
  }*/
  
  iterator x = begin();
  for(size_type i=0;i<ile_elementow;i++){
    delete (x++).node;
    std::cout<<"TEST::clear: Usunalem "<<i+1<<". element"<<std::endl;
  }  
  
  //ustawienie wskaznikow w strazniku
  HashT[N]->nast = HashT[N]->poprz = HashT[N];

  //zerujemy cala tablice
  Node **wsk = HashT;
  for(int i=0; i<N ; i++){
    *(wsk++) = 0;
  }
}

//Wstawianie elementu
template<class K, class V, unsigned hashFunc(const K&),
        int compFunc(const K&,const K&)>
std::pair<typename AISDIHashMap<K,V,hashFunc,compFunc>::iterator, bool>
AISDIHashMap<K,V,hashFunc,compFunc>::
insert(const std::pair<K, V>& entry){
  unsigned index=hashFunc(entry.first);

  if(HashT[index]==0){
    //szukamy nastepnego elementu
    Node** wsk=&HashT[index+1];
    while((*wsk)==0) wsk++;

    //wstawiamy element przed odszukany element
    HashT[index]=new Node (entry, (*wsk)->poprz, (*wsk), true, false);
    (*wsk)->poprz=HashT[index];
    HashT[index]->poprz->nast=HashT[index];

    ile_elementow++;
    std::cout<<"TEST::insert: Dodaje pod unikalnego hasha"<<std::endl;
    
    return std::make_pair(iterator(HashT[index]),true);
  }
  
  else{
  //jesli cos juz bylo pod tym hashem to wstawiamy nowy element przed tego
  //z hasha
    Node *wsk=HashT[index];
    while(!(wsk->last)){
      if(compFunc(wsk->data.first , entry.first) == 0){
        std::cout<<"TEST::insert: Element o tym kluczu juz istnieje"<<std::endl;
        return std::make_pair(iterator(wsk),false);
      }
      wsk=wsk->nast;
    }

    //sprawdzamy czy ostatni element pod tym hashem nie ma takiego klucza
    if(compFunc(wsk->data.first , entry.first) == 0){
      std::cout<<"TEST::insert: Element o tym kluczu juz istnieje"<<std::endl;
      return std::make_pair(iterator(wsk),false);
    }

    //skoro nie ma elementow o tym kluczu to dodajemy nowy element
    wsk=HashT[index];
    HashT[index]=new Node(entry,wsk->poprz,wsk,false,false);
    wsk->poprz->nast=HashT[index];
    wsk->poprz=HashT[index];

    ile_elementow++;    
    std::cout<<"TEST::insert: Dodaje pod tego samego hasha"<<std::endl;
    
    return std::make_pair(iterator(wsk->poprz),true);
  }
}

//Szukanie elementu
template<class K, class V, unsigned hashFunc(const K&),
        int compFunc(const K&,const K&)>
typename AISDIHashMap<K,V,hashFunc,compFunc>::iterator
AISDIHashMap<K,V,hashFunc,compFunc>::
find(const K& klucz){
  unsigned index = hashFunc(klucz);
  Node *wsk=HashT[index];
  if(HashT[index]==0){
    std::cout<<"TEST::find: nie znalezniono elementu"<<std::endl;
    return end();
  }

  //sprawdzamy czy sie zagadza klucz az dojdziemy do ostatniego
  while(!(wsk->last)){
    if(compFunc(wsk->data.first, klucz) == 0){
      std::cout<<"TEST::find: znaleziono element"<<std::endl;
      return iterator(wsk);
    }
    wsk=wsk->nast;
  }

  //sprawdzamy ostatniego
  if(compFunc(wsk->data.first, klucz) == 0){
    std::cout<<"TEST::find: znaleziono element"<<std::endl;
    return iterator(wsk);
  }  

  std::cout<<"TEST::find: nie znalezniono elementu"<<std::endl;
  return end();

}

//Usuwanie elementu wskazywanego przez iterator
template<class K, class V, unsigned hashFunc(const K&),
        int compFunc(const K&,const K&)>
typename AISDIHashMap<K,V,hashFunc,compFunc>::iterator
AISDIHashMap<K,V,hashFunc,compFunc>::
erase(AISDIHashMap<K,V,hashFunc,compFunc>::iterator i){
  //koniec gdy proba usuniecia straznika
  if(i==end())
    return i;

  //przewiazanie wskaznikow
  Node *tmp=i.node->nast;
  tmp->poprz=i.node->poprz;
  i.node->poprz->nast=tmp;

  //sprawdzanie czy element mial unikalnego hasha, czyli czy byl pierwszym
  //elementem o tym hashu i jednoczesnie ostatnim; jesli tak to zerujemy
  //pole w tablicy hashujacej o tym indeksie
  if(i.node->last && i.node->poprz->last){
    HashT[hashFunc(i.node->data.first)]=0;
  }
  //sprawdzamy czy element byl ostatnim elementem pod tym hashem,
  //ale nie jedynym; jesli tak to ustawiamy poprzedni elementow jako ostatni
  if(i.node->last && !(i.node->poprz->last)){
    i.node->poprz->last=true;
  }
  //usuwamy element
  delete i.node;
  ile_elementow--;
  
  std::cout<<"TEST::erase(iter): Usunalem element"<<std::endl; 
  return tmp;
}

//Usuwanie elementu o zadanym kluczu
template<class K, class V, unsigned hashFunc(const K&),
        int compFunc(const K&,const K&)>
typename AISDIHashMap<K,V,hashFunc,compFunc>::size_type
AISDIHashMap<K,V,hashFunc,compFunc>::erase(const K& key){
  iterator i = find(key);
  if(i==end())
    return 0;

  std::cout<<"TEST::erase(key): Usunalem element"<<std::endl; 
  erase(i);
  return 1;
}

//Usuwanie przedzialu elementow
template<class K, class V, unsigned hashFunc(const K&),
        int compFunc(const K&,const K&)>
typename AISDIHashMap<K,V,hashFunc,compFunc>::iterator
AISDIHashMap<K,V,hashFunc,compFunc>::erase(iterator first, iterator last){
  /*Node *tmp=first.node->poprz;
  for(iterator i=first;i!=last;){
    std::cout<<"TEST::erase(range): Usunalem element"<<std::endl;   
    //jesli element jest jedynym elementem o tym hashu
    if(i.node->last && i.node->poprz->last)

    delete (i++).node;
    ile_elementow--;
  }
  tmp->nast=last.node;
  last.node->poprz=tmp;*/
  for(iterator i=first;i!=last;)
    erase(i++);
    
  return last;  
}

//Operator []
template<class K, class V, unsigned hashFunc(const K&),
        int compFunc(const K&,const K&)>
V& AISDIHashMap<K,V,hashFunc,compFunc>::operator[](const K& k){
  iterator i = find(k);
  //jesli znaleziono element o podanym kluczu
  if(i!=end())
    return i.node->data.second;

  //jesli nie znaleziono elementu o tym kluczu
  V v; 
  iterator j=(insert(std::make_pair(k,v))).first;

  return j.node->data.second;
}













