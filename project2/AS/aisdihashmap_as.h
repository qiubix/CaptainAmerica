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

#include <string>
#include <utility>
#include <iostream>
#include <iterator>
#define MAX 65536

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
	typedef std::pair<key_type, value_type> T;
	
	struct HNode
	{
		HNode* hlnext;   // kolejny element na hash_liœcie (malej liscie)
		HNode* hlprev;   // poprzedni element na hash_liœcie (malej liscie)
		HNode* pnext;    // kolejny element w pierœcieniu
		HNode* pprev;    // poprzedni element w pierœcieniu
		T data;
		HNode() : pnext(NULL), pprev(NULL), hlnext(NULL), hlprev(NULL) {}
		HNode(const std::pair<K,V>& d) : pnext(NULL), pprev(NULL), hlnext(NULL), hlprev(NULL), data(d) {}
	};
	
	
	HNode* straznik;
	HNode* tab[MAX];

	AISDIHashMap()
	{
		straznik=new HNode();
		straznik->pprev=straznik;
		straznik->pnext=straznik;
		for(int i=0;i<MAX;i++)
			tab[i]=NULL;
	};
	
	~AISDIHashMap()
	{
		clear();
		delete straznik;
		delete []tab;
	};

	/// Coping constructor.
	explicit AISDIHashMap(const AISDIHashMap<K, V, hashFunc, compFunc>& a)
	{
		straznik=new HNode();
		straznik->pprev=straznik;
		straznik->pnext=straznik;
		for(int i=0;i<MAX;i++)
			tab[i]=NULL;
		const_iterator i=const_iterator(a.begin());
		for(;i.node!=a.straznik;/*i.node=i.node->pnext*/i++)
		{
			insert(std::make_pair(i->first,i->second));
		}
	};

	class const_iterator : public std::iterator<std::forward_iterator_tag, T >
	{
	friend class AISDIHashMap;
	public:
		typedef std::pair<key_type,value_type> P;
		HNode* node;
		const_iterator(HNode* x) : node(x) {}
		const_iterator() {}
		const_iterator(const const_iterator& x) : node(x.node) {}

		inline const P& operator*() const
		{
			return node->data;
		}
		inline const P* operator->() const
		{
			return &(node->data);
		}
		inline bool operator==(const const_iterator& x) const
		{
			return node==x.node;
		}
		inline bool operator!=(const const_iterator& x) const
		{
			return node!=x.node;
		}
		const_iterator& operator++()
		{
			node=node->pnext;
			return *this;
		}
		const_iterator operator++(int)
		{
			const_iterator temp=*this;
			node=node->pnext;
			return temp;
		}
		const_iterator& operator--()
		{
			node=node->pprev;
			return *this;
		}
		const_iterator operator--(int)
		{
			const_iterator temp=*this;
			node=node->pprev;
			return temp;
		}
	};
	/// Iterator.
	class iterator : public const_iterator
	{
	public:
		typedef std::pair<key_type,value_type> P;
		HNode* node;      
		iterator(HNode* x) : const_iterator(x) {}
		friend class AISDIHashMap;
		iterator() {}
		iterator(const const_iterator& x) : const_iterator(x) {}
		iterator(const iterator& x) {node=x.node;}

		inline P& operator*() const
		{
			return node->data;
		}
		inline P* operator->() const
		{
			return &(node->data);
		}
		inline bool operator==(const const_iterator& x) const
		{
			return node==x.node;
		}
		inline bool operator!=(const const_iterator& x) const
		{
			return node!=x.node;
		}
				iterator& operator++()
		{
			const_iterator::node=const_iterator::node->pnext;
			return *this;
		}
		iterator operator++(int)
		{
			iterator temp=*this;
			const_iterator::node=const_iterator::node->pnext;
			return temp;
		}
		iterator& operator--()
		{
			const_iterator::node=const_iterator::node->pprev;
			return *this;
		}
		iterator operator--(int)
		{
			iterator temp=*this;
			const_iterator::node=const_iterator::node->pprev;
			return temp;
		}
	};
	
	friend class iterator;
	friend class const_iterator;
	
	/// Returns an iterator addressing the first element in the map.
	inline iterator begin()
	{
		iterator i;
		i.node=straznik->pnext;
		return i;
	};
	
	inline const_iterator begin() const
	{
	   	const_iterator i;
		i.node=straznik->pnext;
		return i;
	};
	
	/// Returns an iterator that addresses the location succeeding the last element in a map.
	inline iterator end()
	{
		iterator i;
		i.node=straznik;
		return i;
	};
	
	inline const_iterator end() const
	{
		const_iterator i;
		i.node=straznik;
		return i;
	};
	
	/// Inserts an element into the map.
	/// @returns A pair whose bool component is true if an insertion was
	///          made and false if the map already contained an element
	///          associated with that key, and whose iterator component coresponds to
	///          the address where a new element was inserted or where the element
	///          was already located.
	std::pair<iterator, bool> insert(const std::pair<K, V>& entry)
	{
		iterator i=find(entry.first);
		if (i.node!=straznik) return std::make_pair(i,(bool)false);
		HNode* Insert=new HNode(entry);
		Insert->pprev=straznik;
		Insert->pnext=straznik->pnext;
		Insert->pnext->pprev=Insert;
		straznik->pnext=Insert;
		//wstawiony do pierscienia
		unsigned Indeks=hashFunc(entry.first);
		Insert->hlnext=tab[Indeks];
		tab[Indeks]=Insert;
		if (Insert->hlnext!=NULL)
			Insert->hlnext->hlprev=Insert;
		//wstawiony do malej listy
		return std::make_pair(iterator(Insert),(bool)true);
	};
	
	/// Returns an iterator addressing the location of the entry in the map
	/// that has a key equivalent to the specified one or the location succeeding the
	/// last element in the map if there is no match for the key.
	iterator find(const K& k)
	{
		iterator i;
		i.node=tab[hashFunc(k)];
		while(i.node!=NULL)
		{
			if(i->first==k)
				return i;
			else
				i.node=i.node->hlnext;
			}
		return end();
	};
	
	const_iterator find(const K& k) const
	{
		const_iterator i;
		i.node=tab[hashFunc(k)];
		while(i.node!=NULL)
		{
			if(i->first==k)
				return i;
			else
				i.node=i.node->hlnext;
		}
		return end();
	};
	
	/// Inserts an element into a map with a specified key value
	/// if one with such a key value does not exist.
	/// @returns Reference to the value component of the element defined by the key.
	V& operator[](const K& k)
	{
		(insert(std::make_pair(k,0)).first)->second;
		return (insert(std::make_pair(k,0)).first)->second;
		/*iterator i=find(k);
		if(i!=end())
		{
			return i->second;
		}
		else
		{
			std::pair<iterator, bool> p=insert(std::make_pair(k, 0));
			return p.first->second;
		}*/
	};
	
	/// Tests if a map is empty.
	bool empty() const
	{
		return straznik->pnext==straznik;
	}
	
	/// Returns the number of elements in the map.
	size_type size() const
	{
		int rozmiar=0;
		for(HNode* temp=straznik->pnext;temp!=straznik;temp=temp->pnext) rozmiar++;
		return rozmiar;
	};
	
	/// Returns the number of elements in a map whose key matches a parameter-specified key.
	size_type count(const K& _Key) const
	{
		if(find(_Key)!=end()) 
			return 1;
		return 0;
	};
	
	/// Removes an element from the map.
	/// @returns The iterator that designates the first element remaining beyond any elements removed.
	iterator erase(iterator i)
	{
		if (i.node==straznik) //sprawdzam czy przypadkiem nie chcemy usunac straznika
			return i;
		if (i.node->hlnext!=NULL) //sprawdzam czy malej liscie nie jest to ostatni element
			i.node->hlnext->hlprev=i.node->hlprev;
		if (i.node->hlprev==NULL) //czy nie jest to pierwszy element w malej liscie
			tab[hashFunc(i->first)]=i.node->hlnext;	
		else 
			i.node->hlprev->hlnext=i.node->hlnext;
		//usuniete z malej listy
		i.node->pprev->pnext=i.node->pnext;
		i.node->pnext->pprev=i.node->pprev;
		//usuniete z pierscienia
		i.node=i.node->pnext;
		return i;
	};
	
	/// Removes a range of elements from the map.
	/// The range is defined by the key values of the first and last iterators
	/// first is the first element removed and last is the element just beyond the last elemnt removed.
	/// @returns The iterator that designates the first element remaining beyond any elements removed.
	iterator erase(iterator first, iterator last)
	{
		while(first.node!=last.node)
		if (first!=end())
			first=erase(first);
		else
			return first;
		return last;
	};
	
	/// Removes an element from the map.
	/// @returns The number of elements that have been removed from the map.
	///          Since this is not a multimap itshould be 1 or 0.
	size_type erase(const K& key)
	{
		iterator i=find(key);
        	if (i==end()) 
			return 0;
        	erase(i);
       	return 1;
	};
	
	/// Erases all the elements of a map. poza stra¿nikiem.
	void clear()
	{
		erase(begin(),end());
	};
};

template<class K>
inline unsigned hashF(const K& k)
{
	unsigned h=static_cast<unsigned int>(k.size());
	for(int i=0;i<static_cast<unsigned int>(k.size());i++)
	    h=(h<<5)^(h>>27)^k[i];
	return h%MAX;	
};
