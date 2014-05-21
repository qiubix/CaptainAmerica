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

template<class K, class V>
struct Node
{
	typedef std::pair<K,V> Para;
	Node* hlnext;   // kolejny element na hash_liœcie
	Node* hlprev;   // poprzedni element na hash_liœcie   
	Node* pnext;    // kolejny element w pierœcieniu
	Node* pprev;    // poprzedni element w pierœcieniu
	Para data;
	Node() : pnext(NULL), pprev(NULL), hlnext(NULL), hlprev(NULL) {}
	Node(const std::pair<K,V>& d) : pnext(NULL), pprev(NULL), hlnext(NULL), hlprev(NULL), data(d) {}
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
	typedef Node<key_type,value_type> HNode;
protected:
	HNode* tab[65536];
	int rozmiar;
public:

	AISDIHashMap()
	{
		tab[65535]=new HNode();
		tab[65535]->pprev=tab[65535];
		tab[65535]->pnext=tab[65535];
		for(int i=0;i<65535;i++)
			tab[i]=NULL;
		rozmiar=0;
	};
	
	~AISDIHashMap()
	{
		clear();
		delete tab[65535];
		//delete []tab;
	};

	/// Coping constructor.
	explicit AISDIHashMap(const AISDIHashMap<K, V, hashFunc, compFunc>& a)
	{
		tab[65535]=new HNode();
		tab[65535]->pprev=tab[65535];
		tab[65535]->pnext=tab[65535];
		iterator i=a.end();
		i=i.node->pprev;
		while(i!=a.end());
		{
			insert(std::make_pair(i->first,i->second));
			i=i.node->pprev;
		}
	};

	/// const_iterator.
	class const_iterator : public std::iterator<std::forward_iterator_tag, std::pair<key_type, value_type> >
	{
	public:
		typedef std::pair<key_type,value_type> T;
		HNode* node;
		friend class AISDIHashMap;
		const_iterator(HNode* x) : node(x) {}
		const_iterator() {}
		const_iterator(const const_iterator& x) : node(x.node) {}

		inline const T& operator*() const
		{
			return node->data;
		}
		inline const T* operator->() const
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
	};
	/// Iterator.
	class iterator : public const_iterator
	{
	public:
		HNode* node;      
		iterator(HNode* x) : const_iterator(x) {}
		friend class AISDIHashMap;
		typedef std::pair<key_type,value_type> T;
		iterator() {}
		iterator(const const_iterator& x) : const_iterator(x) {}
		iterator(const iterator& x) {node=x.node;}

		inline T& operator*() const
		{
			return node->data;
		}
		inline T* operator->() const
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
	};

	/// Returns an iterator addressing the first element in the map.
	inline iterator begin()
	{
		return iterator(tab[65535]->pnext);
	};
	
	inline const_iterator begin() const
	{
		return const_iterator(tab[65535]->pnext);
	};
	
	/// Returns an iterator that addresses the location succeeding the last element in a map.
	inline iterator end()
	{
		return const_iterator(tab[65535]);
	};
	
	inline const_iterator end() const
	{
		return const_iterator(tab[65535]);
	};
   
	/// Inserts an element into the map.
	/// @returns A pair whose bool component is true if an insertion was
	///          made and false if the map already contained an element
	///          associated with that key, and whose iterator component coresponds to
	///          the address where a new element was inserted or where the element
	///          was already located.
	std::pair<iterator, bool> insert(const std::pair<K, V>& entry)
	{
		HNode* ins=new HNode(entry);
		unsigned Indeks=hashFunc(entry.first);
		if(tab[Indeks]==NULL)
			tab[Indeks]=ins;
		else
		{
			HNode* temp=tab[Indeks];
			while (temp!=NULL)
			{
				if (temp->data.first==entry.first)
				{
					temp->data.second=entry.second;
					delete ins;
					return std::make_pair(iterator(temp),false);
				}
				temp=temp->hlnext;
			}
			temp=tab[Indeks];
			tab[Indeks]=ins;
			ins->hlprev=NULL;
			ins->hlnext=temp;
			temp->hlprev=ins;
		}
		/*
		Node* prev=tab[MAX-1];
		Node* next=prev->pnext;
		*/
		rozmiar++;
		iterator i=end();
		HNode* next=i.node->pnext;
		i.node->pnext=ins;
		ins->pnext=next;
		next->pprev=ins;
		ins->pprev=i.node;
		return std::make_pair(iterator(ins),true);
	};

	/// Returns an iterator addressing the location of the entry in the map
	/// that has a key equivalent to the specified one or the location succeeding the
	/// last element in the map if there is no match for the key.
	iterator find(const K& k)
	{
		unsigned Indeks=hashFunc(k);
		HNode* temp;
		temp=tab[Indeks];
		while(temp!=NULL)
		{
			if(temp->data.first==k)
				return iterator(temp);
			else
				temp=temp->hlnext;
		}
		return end();
	};
	
	const_iterator find(const K& k) const
	{
		unsigned Indeks=hashFunc(k);
		HNode* temp;
		temp=tab[Indeks];
		while(temp!=NULL)
		{
			if(temp->data->first==k)
				return const_iterator(temp);
			else
				temp=temp->hlnext;
		}
		return end();
	};
	
	/// Inserts an element into a map with a specified key value
	/// if one with such a key value does not exist.
	/// @returns Reference to the value component of the element defined by the key.
	V& operator[](const K& k)
	{
		iterator i=find(k);
		if(i!=end())
		{
			return i->second;
		}
		else
		{
			std::pair<iterator, bool> p=insert(std::make_pair(k, 0));
			return p.first->second;
		}
	};

	/// Tests if a map is empty.
	bool empty() const
	{
		return rozmiar<1;
	};
	
	/// Returns the number of elements in the map.
	size_type size() const
	{
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
		if(i==end())
			return i;
		(i.node->pprev)->pnext=i.node->pnext;
		(i.node->pnext)->pprev=i.node->pprev;
		if(i.node->hlprev==NULL)
		{
			if(i.node->hlnext==NULL)
				tab[hashFunc(i->first)]=NULL;
			else
			{
				tab[hashFunc(i->first)]=i.node->hlnext;
				i.node->hlnext->hlprev=NULL;
			}
		}
		else
		{
			i.node->hlprev->hlnext=i.node->hlnext;
			if (i.node->hlnext!=NULL)
				i.node->hlnext->hlprev=i.node->hlprev;
		}
		rozmiar--;
		HNode *temp;
		temp=i.node;
		i.node=i.node->pnext;
		delete temp;
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
	
	/// Erases all the elements of a map.
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
	return h%65535;	
};
