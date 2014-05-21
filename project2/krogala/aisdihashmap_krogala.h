/*************************
autor: Krzysztof Rogala
*************************/

#include <iterator>
#include <string>

template<class K, class V>
class LNode {
public:
	std::pair<K,V> data;
        LNode<K,V> *next;
	LNode<K,V> *prev;
	int indeks;
	LNode(const std::pair<K,V>& d, int i) : next(NULL), prev(NULL), data(d), indeks(i) {}
	LNode(const std::pair<K,V>& d, LNode<K,V>* p, LNode<K,V>* n, int i) : next(n), prev(p), data(d), indeks(i) {}
};

const unsigned SZ = 65599;
unsigned hash(const std::string& str) {
  unsigned long ret = 0;
  int c;
  int i=0;
  while (i<str.size())  {
    c = str[i];
    ret = c + (ret << 6) + (ret << 16) - ret;
    i++;
  }
  return ret%SZ;
}

template <class Key> inline int _compFunc(const Key& key1, const Key& key2) { return (key1==key2); };

template<class K, class V, unsigned hashFunc(const K&)=&hash, int compFunc(const K&,const K&)=&_compFunc<K> >
class AISDIHashMap {
  protected:
	  //lista dowiazan. identyczna jak ListMap, ale dwukierunkowa
	class Lista {
  public:
   LNode<K,V> *first;
   Lista() : first(NULL) {
	   first = new LNode<K,V>(std::make_pair(K(),V()), NULL, NULL, -1);
   }
   Lista(const Lista& l) {
     LNode<K,V> *ln = l.first;
     first = new LNode<K,V>(ln->data, NULL, NULL, ln->indeks);
	 LNode<K,V> *prev = first;
	 ln = ln->next;
     while(ln!=NULL) {
       LNode<K,V> *nowy = new LNode<K,V>(ln->data, prev, NULL, ln->indeks);
	   prev->next = nowy;
          prev = prev->next;
	   ln = ln->next;
     }
   }
   ~Lista() {}
  std::pair<LNode<K,V>*, bool> insert(const std::pair<K, V>& entry, int i) {
	  if(first->next==NULL) {
		  first->next = new LNode<K,V>(entry, NULL, NULL, i);
		  first->next->prev = first;
		  return std::make_pair(first->next, true);
	  }
	  LNode<K,V> *ln = first;
	  LNode<K,V> *nowy;
	  while(ln->next!=NULL && (ln->data).first<entry.first) { 
		  ln = ln->next;
	  }
	  if((ln->data).first == entry.first) return std::make_pair(ln, false);
	  if((ln->data).first < entry.first) {
		nowy = new LNode<K,V>(entry, ln, ln->next, i);
		if(ln->next!=NULL) ln->next->prev = nowy;
		ln->next = nowy;
	  }
	  else {
		  nowy = new LNode<K,V>(entry, ln->prev, ln, i);
		  ln->prev->next = nowy;
		  ln->prev = nowy;
		  
	  }
	  return std::make_pair(nowy, true);
  }

  LNode<K,V>* unsafe_insert(const std::pair<K, V>& entry, int i) {
	  return insert(entry, i).first;
  }

  LNode<K,V>* find(const K& k) {
	  if(first->next==NULL) return NULL;
	  LNode<K,V> *ln = first;
	  while(ln->next!=NULL && (ln->data).first!=k) { ln=ln->next; }
	  if((ln->data).first==k && ln!=first) return ln;
	  return NULL;
  }
  LNode<K,V>* erase(LNode<K,V> *l) {
	  if(l==NULL) return NULL;
	  LNode<K,V> *ln = first->next;
	  while(ln!=NULL && ln!=l) {
		  ln=ln->next;
	  }
	  if(ln==NULL) return NULL;
	  LNode<K,V> *ret = ln->next;
	  LNode<K,V> *prv = ln->prev;
	  prv->next = ret;
	  if(ret!=NULL) ret->prev = prv;
	  delete ln;
	  if(ret!=NULL) return ret;
	  return prv;
  }
  int erase(const K& key) {
	  LNode<K,V> *ln = first->next;
	  while(ln!=NULL && (ln->data).first!=key) ln=ln->next;
	  if(ln==NULL) return 0;
	  ln->prev->next = ln->next;
	  if(ln->next!=NULL) ln->next->prev = ln->prev;
	  delete ln;
	  return 1;
  }
  void clear() {
	  LNode<K,V> *ln = first->next;
	  LNode<K,V> *p = first;
	  if(ln==NULL) delete p;
	  while(ln!=NULL) {
		  ln = ln->next;
		  if(p!=NULL) delete p;
		  p = ln;
	  }
  }
  bool eq(const Lista& another) {
	  LNode<K,V> *ln = first->next;
	  LNode<K,V> *aln = another.first->next;
	  while(ln!=NULL) {
		  if(aln==NULL) return false;
		  if(ln->data.first!=aln->data.first || ln->data.second!=aln->data.second) return false;
		  ln=ln->next;
		  aln=aln->next;
	  }
	  if(aln!=NULL) return false;
	  return true;
  }

  unsigned sz() {
	  LNode<K,V> *ln = first->next;
	  unsigned ret=0;
	  while(ln!=NULL) { ++ret; ln=ln->next; }
	  return ret;
  }
  bool empty() { return first->next==NULL; }
  LNode<K,V> *begin() { return first; }
  bool operator==(const Lista& a) const { return eq(a); }
} tab[SZ+1];
//koniec definicji listy
public:
   typedef K key_type;
   typedef V value_type;
   typedef unsigned size_type;

   AISDIHashMap() {}	//konstruktor (wola konstruktor domyslny listy)
   ~AISDIHashMap() {	//destrutkor
	   clear();
   }
   AISDIHashMap(const AISDIHashMap<K, V, hashFunc, compFunc>& a) {
	   for(int i=0; i<=SZ; i++) tab[i] = a.tab[i];
   }
//iteratory, analogiczne do ListMap, tylko jeden wskaznik wiecej
   class const_iterator : public std::iterator<std::forward_iterator_tag, std::pair<K, V> > {
   protected:
	   friend class AISDIHashMap;
	   LNode<K,V>* node;
	   int indeks;
	   Lista *tab;
	   const_iterator(LNode<K,V>* x, int i, Lista *t) : node(x), indeks(i), tab(t) {}
   public:
	   const_iterator() : node(NULL), indeks(-1), tab(NULL) {}
	   const_iterator(const const_iterator& x) : node(x.node), indeks(x.indeks), tab(x.tab) {}
	   const std::pair<K,V>& operator*() const {
         return node->data;
	   }
	   inline const std::pair<K,V>* operator->() const {
		   return &(node->data);
	   }
	   // preincrementacja
	   inline const_iterator& operator++() {
		   if(node->next!=NULL) {
			   node = node->next;
			   return *this;
		   }
		   ++indeks;
		   while(indeks<=SZ && tab[indeks].begin()->next==NULL) indeks++;
		   if(indeks>SZ) {
			   indeks = SZ;
			   node = tab[SZ].begin();
			   return *this;
		   }
		   node = tab[indeks].begin()->next;
		   return *this;
	   }

       // postincrementacja
	   inline const_iterator operator++(int) { return ++(*this); }
       // predekrementacja
	   inline const_iterator& operator--() {
		   if(node->prev!=NULL && node->prev->prev!=NULL) {
	            node = node->prev;
		     return *this;
		   }
		   --indeks;
		   while(indeks>=0 && tab[indeks].begin()->next==NULL) --indeks;
		   node = tab[indeks].begin()->next;
		   if(indeks==0) return *this;
		   while(node->next!=NULL) node = node->next;
		   return *this;
	   }
       // postdekrementacja
	   inline const_iterator operator--(int) { return --(*this); }

	   inline bool operator==(const const_iterator& a) const { return node == a.node; }
          inline bool operator!=(const const_iterator& a) const { return node != a.node; }
	   inline bool operator<(const const_iterator& a) const {
		   if(indeks<a.indeks) return true;
		   if(indeks>a.indeks) return false;
		   if(node->data.first < a.node->data.first) return true;
		   return false;
	   }

   };

   class iterator : public const_iterator {
   protected:
	   friend class AISDIHashMap;
	   iterator(LNode<K,V> *n, int i, Lista* t) : const_iterator(n, i, t) {}
   public:
	   iterator() {}
	   iterator(const const_iterator& a) : const_iterator(a) {}
          iterator(const iterator& a) { const_iterator::node = a.node; this->indeks = a.indeks; this->tab = a.tab; }
	   std::pair<K,V>& operator*() { return this->node->data; }
	   inline std::pair<K,V>* operator->() const { return &(this->node->data); }

      iterator& operator++() {
         ++(*(const_iterator*)this);
         return (*this);
	  }

      iterator operator++(int) {
         iterator temp = *this;
         ++*this;
         return temp;
      }

      iterator& operator--() {
         --(*(const_iterator*)this);
         return (*this);
      }

	  iterator operator--(int) {
         iterator temp = *this;
         --*this;
         return temp;
      }
   };
   
   inline iterator begin() {
     int i=0;
     while(i<SZ && tab[i].begin()->next==NULL) i++;
     if(i==SZ) return end();
     return iterator(tab[i].begin()->next, i, tab);
   }
   inline const_iterator begin() const {
     int i=0;
     while(i<=SZ && tab[i].begin()->next==NULL) i++;
     if(i==SZ) return end();
     return const_iterator(tab[i].begin()->next, i, tab);
   }
   inline iterator end() { return iterator(tab[SZ].begin(), SZ, tab); }
   inline const_iterator end() const { return const_iterator(tab[SZ].begin(), SZ, tab); }
//wstawianie do hashmapy   
   std::pair<iterator, bool> insert(const std::pair<K, V>& entry) {
	   int indeks = hashFunc(entry.first);
	   std::pair<LNode<K,V>*, bool> para = tab[indeks].insert(entry, indeks);
	   return std::make_pair(iterator(para.first, indeks, tab), para.second);
   }
//wstawianie do mapy
   iterator unsafe_insert(const std::pair<K, V>& entry) { 
	   int indeks = hashFunc(entry.first);
	   return tab[indeks].insert(entry, indeks).first;
   }
//szukanie elementu o kluczu k
   iterator find(const K& k) {
	   LNode<K,V> *ln = tab[hashFunc(k)].find(k);
	   if(ln==NULL) {
		   return this->end();
	   }
	   return iterator(ln, hashFunc(k), tab);
   }
   const_iterator find(const K& k) const {
	   LNode<K,V> *ln = tab[hashFunc(k)].find(k);
	   if(ln==NULL) {
		   return this->end();
	   }
	   return const_iterator(ln, hashFunc(k), tab);
   }
//udostepnienie wartosci o kluczu k, lub wstawienie jak nie ma
   V& operator[](const K& k) {
	   int indeks = hashFunc(k);
	   LNode<K,V>* ln = tab[indeks].insert(std::make_pair(k, V()), indeks).first;
	   return ln->data.second;
   }
//czy mapa jest pusta
   bool empty( ) {
	   int i=0;
	   while(i<SZ) {
		   if(!tab[i].empty()) return false;
		   i++;
	   }
	   return true;
   }
//liczba elementow w mapie
   size_type size() {
	   unsigned ret=0;
	   for(int i=0; i<SZ; i++) {
		   ret += tab[i].sz();
	   }
	   return ret;
   }
//liczba elementow o kluczu _Key (1 lub 00
   size_type count(const K& _Key) {
	   int indeks = hashFunc(_Key);
	   LNode<K,V> *ln = tab[indeks].find(_Key);
	   return ln!=NULL;
   }
//usuwanie elementu
   iterator erase(iterator i) {
	   if(i.node==tab[i.indeks].begin()) return end();
	   int indeks = i.indeks;
	   int key = i->first;	   
	   LNode<K,V>* ln = tab[indeks].erase(i.node);
	   if(ln==NULL) return this->end();
	   if(ln->data.first>key) return iterator(ln, ln->indeks, tab);
	   while(tab[++indeks].begin()->next==NULL);
	   ln = tab[indeks].begin()->next;
	   return iterator(ln, ln->indeks, tab);
   }
//usuwanie zakresowe
   iterator erase(iterator first, iterator last) {
	   if(first==end()) return end();
	   if(!(first<last)) return end();
	   iterator i = first;
	   while(i!=last) {
		   tab[i.indeks].erase(i.node);
		   i++;
	   }
	   return last;
   }
//usuwanie elementu o kluczu key
   size_type erase(const K& key) {
	   int indeks = hashFunc(key);
	   return tab[indeks].erase(key);
   }
//czyszczenie mapy
   protected:
   void clear( ) {
	   for(int i=0; i<=SZ; i++) {
		   tab[i].clear();
	   }
   }
   public:
   bool info_eq(const AISDIHashMap<K,V,hashFunc, compFunc>& a) {
     for(int i=0; i<SZ; ++i) { if(!(tab[i].eq(a.tab[i]))) return false; }
     return true;
   }
   bool struct_eq(const AISDIHashMap<K, V, hashFunc, compFunc>& a) {
     return info_eq(a);
   }
   bool operator==(const AISDIHashMap<K,V,hashFunc, compFunc>& a) {
      return this->info_eq(a);
   }
};
