/** 
@file asd.cc

Plik do modyfikacji w w ramach cwiczenia 4 z AISDI.
Zawiera niekompletne implementacje metod klasy TreeMap.
Jest tez prosta funkcja testujaca (void test()), ktora
jest wolana w funkcji main. Mozna w niej zaimplementowac
wlasne testy.
NALEZY ZMODYFIKOWAC I UZUPELNIC CIALA METOD KLASY TreeMap.
    
@author
Pawel Cichocki, Michal Nowacki

@date  
last revision
- 2005.12.01 Pawel Cichocki: TreeNodeDetail class
- 2005.12.01 Michal Nowacki: lab #4
- 2005.11.17 Michal Nowacki: lab #3, copy operator and constructor
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
#include <climits> 		//INT_MAX
#include <iostream>


#define PRINT(x) std::cout << #x"\n";


#ifdef _SUNOS
#include "/home/common/dyd/aisdi/tree/TreeMap.h" 
#else
#include "TreeMap.h"
#endif

/// A helper class.
class TreeMapDetail //Helper
{
protected:
	friend class TreeMap;
	typedef int Key;
	typedef std::string Val;
	
public:
	//Kopiuje cale poddrzewo danego wezla
	static TreeNode* insert_all(TreeNode* current)
	{
		if(current == NULL) return NULL;		//puste drzewo albo trafilismy na koniec galezi
		TreeNode* tmp = new TreeNode(current->data);
		tmp->left = insert_all(current->left);	//wstawia lewe poddrzewo do lewego dziecka utworzonego elementu
		if(tmp->left != NULL)	//jesli byl jakis element po lewej stronie od aktualnego
			tmp->left->parent = tmp;
		if(current->right != current){
			tmp->right = insert_all(current->right);	//wstawia prawe poddrzewo do prawego dziecka wstawionego elementu
			if(tmp->right != NULL)
				tmp->right->parent = tmp;
		}
		return tmp;
	}
	
	static TreeNode* uns_insert(TreeNode* &current, TreeNode* &procreator, const std::pair<Key, Val>& entry)
	{
		if(current == NULL){	//wezel nie istnieje 
			current = new TreeNode(entry);
			current->parent = procreator;
			return current;
		}
		//klucz w wezle jest mniejszy od klucza, ktory chcemy wstawic, wiec musimy wstawic na prawo od niego
		else if(current->data.first < entry.first) return uns_insert(current->right,current,entry);
		//klucz w wezle jest wiekszy od klucza, ktory chcemy wstawic, wiec wstawiamy na prawo od niego
		else return uns_insert(current->left,current,entry);
	}
	
	static TreeNode* find_key(TreeNode* current, const Key& k)
	{
		if(current == NULL || current->data.first == k) return current;	//rodzic byl lisciem, czyli nie znaleziono wezla o podanym kluczu, albo dany wezel ma szukany klucz
		if(current->data.first < k) return find_key(current->right, k);	//klucz jest wiekszy od aktualnego wezla. Trzeba szukac w prawym poddrzewie
		if(current->data.first > k) return find_key(current->left, k);		//klucz jest mniejszy od aktualnego wezla. Trzeba szukac w lewym poddrzewie
		return current;						//klucze sa rowne - odnaleziono wezel o takim kluczu
	}
	
	static TreeNode* find_previous(TreeNode* current)
	{
		if(current->left != NULL){
			current = current->left;
			while(current->right != NULL) current = current->right;
			return current;
		}
		if(current->right == current) return current;
		
		TreeNode* tmp;
		if(current->parent->right != current->parent){
			tmp = current->parent;
			while(tmp->left == current){
				current = tmp;
				tmp = tmp->parent;
			}
			return tmp;
		}
		else
			return current;
	}
	static TreeNode* find_next(TreeNode* current)
	{
		if(current->right != NULL){
			if(current->right == current) return current;
			current = current->right;
			while(current->left != NULL) current = current->left;
			return current;
		}
		TreeNode* tmp;
		if(current->parent->right != current->parent){
			tmp = current->parent;
			while(tmp->right == current){
				current = tmp;
				tmp = tmp->parent;
			}
			return tmp;
		}
		else
			return current->parent;
	}	
	
	static TreeNode* find_min(TreeNode* current)
	{
		if(current->left != NULL) find_min(current->left);
		return current;
	}
	static TreeNode* find_max(TreeNode* current)
	{
		if(current->right != NULL) find_max(current->right);
		return current;
	}

	static void delete_all(const TreeNode* current){
		if(current != NULL){
			delete_all(current->left);
			delete_all(current->right);
			delete current;
		}
	}
	
	static bool check_struct(TreeNode* first, TreeNode* second)
	{
		if(first==NULL && second==NULL) return true;
		if(first==NULL || second==NULL) return false;
		if(first->right==first && second->right==second) return true;
		if(first->right==first || second->right==second) return false;
		if( (first->data==second->data) && check_struct(first->left,second->left) && check_struct(first->right,second->right) ) return true;
		return false;
	}
};

//////////////////////////////////////////////////////////////////////////////
// TreeMap and TreeMap::iterator methods
//////////////////////////////////////////////////////////////////////////////


//Konstruktor domyslny  TreeMap
//Tworzy straznika, przypisujac mu mo¿liwie najwiekszy klucz, i pusta wartosc
TreeMap::TreeMap()
{
	root = new TreeNode(std::make_pair(INT_MAX,""));
};

/// Content of existing TreeMap object is copied into the new object. 
TreeMap::TreeMap( const TreeMap& m )
{
	root = new TreeNode(std::make_pair(INT_MAX,""));
	root->left = TreeMapDetail::insert_all(m.root->left);
	if(root->left != NULL) root->left->parent = root;
};


//Destruktor TreeMap
//Wywoluje metode sprzatajaca clear() i usuwa straznika
TreeMap::~TreeMap()
{
	clear();
	delete root;
};


// Inserts an element into the map.
// @returns A pair whose bool component is true if an insertion was
//          made and false if the map already contained an element
//          associated with that key, and whose iterator component coresponds to
//          the address where a new element was inserted or where the element
//          was already located.
std::pair<TreeMap::iterator, bool> TreeMap::insert(const std::pair<Key, Val>& entry)
{
	TreeNode* tmp = root->left;
	while(tmp != NULL){
		if(tmp->data.first<entry.first){
			if(tmp->right == NULL){	//nie ma prawego dziecka, wiec wstawiamy
				tmp->right = new TreeNode(entry);
				tmp->right->parent = tmp;
				return std::make_pair(iterator(tmp->right), true);
			}
			else{//ten weze³ ma prawe dziecko
				tmp = tmp->right;	//przechodzimy do prawego dziecka
				continue;			//kolejny obrót pêtli while
			}
		}
		if(tmp->data.first>entry.first){
			if(tmp->left == NULL){	//nie ma lewego dziecka, wiec wstawiamy
				tmp->left = new TreeNode(entry);
				tmp->left->parent = tmp;
				return std::make_pair(iterator(tmp->left), true);
			}
			else{//ten wezel ma lewe dziecko
				tmp = tmp->left;	//przechodzimy do lewego dziecka
				continue;			//kolejny obrot while
			}
		}
		//doszlismy tu, wiec tmp->data.first == entry.first, czyli mamy juz element o takim kluczu
		tmp->data.second = entry.second;	//nadpisanie wartosci
		return std::make_pair(iterator(tmp), false);
	}
	//tmp == NULL, czyli root->left == NULL
	//oznacza to, ze nie ma zadnych elementow w drzewie, czyli wstawiamy pierwszy
	root->left = new TreeNode(entry);
	root->left->parent = root;
	return std::make_pair(iterator(tmp),true);
}


// Inserts an element into the map.
// This method assumes there is no value asociated with
// such a key in the map.
TreeMap::iterator TreeMap::unsafe_insert(const std::pair<Key, Val>& entry)
{
	TreeNode* tmp = TreeMapDetail::uns_insert(root->left, root, entry);
	return iterator(tmp);
}


// Returns an iterator addressing the location of the entry in the map
// that has a key equivalent to the specified one or the location succeeding the
// last element in the map if there is no match for the key.
TreeMap::iterator TreeMap::find(const Key& k)
{
	//PRINT(find(k));
	TreeNode* tmp = TreeMapDetail::find_key(root->left, k);
	if(tmp != NULL)
		return iterator(tmp);
	return end();
}

TreeMap::const_iterator TreeMap::find(const Key& k) const
{
	//PRINT(find(k));
	TreeNode* tmp = TreeMapDetail::find_key(root->left, k);
	if(tmp != NULL)
		return iterator(tmp);
	return end();
}

// Inserts an element into a map with a specified key value
// if one with such a key value does not exist.
// @returns Reference to the value component of the element defined by the key.
TreeMap::Val& TreeMap::operator[](const Key& k)
{
	//PRINT(operator []);
	iterator it = find(k);
	if(it == end()) it = unsafe_insert(std::make_pair(k, Val()));
	return it->second;
}

// Tests if a map is empty.
bool TreeMap::empty( ) const
{
   return root->left==NULL;
}

// Returns the number of elements in the map.
TreeMap::size_type TreeMap::size( ) const
{
	size_type licznik = 0;
	iterator skoczek = begin();
	for(;skoczek!=end();skoczek++) licznik++;
	return licznik;
}

// Returns the number of elements in a map whose key matches a parameter-specified key.
TreeMap::size_type TreeMap::count(const Key& _Key) const
{
	TreeNode* tmp = TreeMapDetail::find_key(root->left,_Key);
	if(tmp == NULL)	return 0;
	return 1;
}



// Removes an element from the map.
// @returns The iterator that designates the first element remaining beyond any elements removed.
TreeMap::iterator TreeMap::erase(TreeMap::iterator i)	
{
	TreeNode* pierwszy;
	TreeNode* drugi;
	pierwszy=i.node;
	++i;
	//brak dzieci w usuwanym
	if(pierwszy->left==NULL&&pierwszy->right==NULL)
	{
		if(pierwszy->parent==root)
			root->left=NULL;
		else if(pierwszy->parent->left==pierwszy)
			pierwszy->parent->left=NULL;
		else
			pierwszy->parent->right=NULL;
		delete pierwszy;
		return i;
	}
	//dwoje dzieci
	if(pierwszy->left!=NULL&&pierwszy->right!=NULL)
	{
		drugi=pierwszy->left;
		while(drugi->right!=NULL)
			drugi=drugi->right;
		if(drugi==pierwszy->left)
		{
			if(pierwszy->parent->left==pierwszy)
				pierwszy->parent->left=drugi;
			else
				pierwszy->parent->right=drugi;
			drugi->parent=pierwszy->parent;
			drugi->right=pierwszy->right;
			drugi->right->parent=drugi;
			delete pierwszy;
		}
		else
		{
			drugi->right=pierwszy->right;
			pierwszy->right->parent=drugi;
			drugi->parent->right=drugi->left;
			if(drugi->left!=NULL)
				drugi->left->parent=drugi->parent;
			drugi->left=pierwszy->left;
			pierwszy->left->parent=drugi;
			drugi->parent=pierwszy->parent;
			if(pierwszy->parent->left==pierwszy)
				pierwszy->parent->left=drugi;
			else
				pierwszy->parent->right=drugi;		
			delete pierwszy;
		}
		return i;
	}
	//jedno dziecko
	else
	{
		if(pierwszy->left!=NULL)
			if(pierwszy->parent->left==pierwszy)
			{
				pierwszy->parent->left=pierwszy->left;
				pierwszy->left->parent=pierwszy->parent;
			}
			else
			{
				pierwszy->parent->right=pierwszy->left;
				pierwszy->left->parent=pierwszy->parent;
			}
		else
			if(pierwszy->parent->left==pierwszy)
			{
				pierwszy->parent->left=pierwszy->right;
				pierwszy->right->parent=pierwszy->parent;
			}
			else
			{
				pierwszy->parent->right=pierwszy->right;
				pierwszy->right->parent=pierwszy->parent;
			}
		delete pierwszy;
	}
	return i;
}
	


// Removes a range of elements from the map.
// The range is defined by the key values of the first and last iterators
// first is the first element removed and last is the element just beyond the last elemnt removed.
// @returns The iterator that designates the first element remaining beyond any elements removed.
TreeMap::iterator TreeMap::erase(TreeMap::iterator f, TreeMap::iterator l)
{
	while(f != l) f = erase(f);
	return f;
}

// Removes an element from the map.
// @returns The number of elements that have been removed from the map.
//          Since this is not a multimap itshould be 1 or 0.
TreeMap::size_type TreeMap::erase(const Key& key)
{
	iterator it = find(key);
	if(it == end() ) return 0;
	erase(it);
	return 1;
}

// Erases all the elements of a map.
void TreeMap::clear( )
{
	TreeMapDetail::delete_all(root->left);
	root->left = NULL;
}

bool TreeMap::struct_eq(const TreeMap& another) const
{
	if(root->left == NULL && root->right == NULL) return true;
	if(root->left == NULL) return false;
	if((another.root)->left == NULL) return false;
	return TreeMapDetail::check_struct(root->left,(another.root)->left);
}

bool TreeMap::info_eq(const TreeMap& another) const
{
	iterator skoczek1 = begin();
	iterator skoczek2 = another.begin();
	for(;skoczek1!=end() && skoczek2!=another.end();++skoczek1,++skoczek2)
		if( (skoczek1->first != skoczek2->first) || (skoczek1->second != skoczek2->second) ) return false;
	if( skoczek1==end() && skoczek2==another.end() ) return true;
	return false;
}

// preincrement
TreeMap::const_iterator& TreeMap::const_iterator::operator ++()
{
	node = TreeMapDetail::find_next(node);
	return *this;
}

// postincrement
TreeMap::const_iterator TreeMap::const_iterator::operator++(int)
{
	TreeNode* tmp = node;
	node = TreeMapDetail::find_next(node);
	return const_iterator(tmp);
}

// predecrement
TreeMap::const_iterator& TreeMap::const_iterator::operator--()
{
	node = TreeMapDetail::find_previous(node);
	return *this;
}

// postdecrement
TreeMap::const_iterator TreeMap::const_iterator::operator--(int)
{
	TreeNode* tmp = node;
	node = TreeMapDetail::find_previous(node);
	return const_iterator(tmp);
}


/// Assignment operator copy the source elements into this object.
TreeMap& TreeMap::operator=(const TreeMap& other)
{
	if(&other != this){
		this->clear();
		root->left = TreeMapDetail::insert_all(other.root->left);
		root->left->parent = root;
	}
	return *this;
}
      
/// Returns an iterator addressing the first element in the map
TreeMap::iterator TreeMap::begin()
{
	TreeNode* skoczek = root;
	while(skoczek->left != NULL)
		skoczek = skoczek->left;
	return iterator(skoczek);
}

TreeMap::const_iterator TreeMap::begin() const
{
	TreeNode* skoczek = root;
	while(skoczek->left != NULL)
		skoczek = skoczek->left;
	return const_iterator(skoczek);
}

/// Returns an iterator that addresses the location succeeding the last element in a map
TreeMap::iterator TreeMap::end()
{
	return iterator(root);
}

/// Returns an iterator that addresses the location succeeding the last element in a map
TreeMap::const_iterator TreeMap::end() const
{ 
	return const_iterator(root);
}

//////////////////////////////////////////////////////////////////////////////
// Tests
//////////////////////////////////////////////////////////////////////////////

/// A helper function that outputs a key-value pair.
void print(const std::pair<int, std::string>& p)
{
   std::cout<<p.first<<", "<<p.second<<std::endl;
}

#include <map>

/// The big mean test function ;)
void test()
{
   // A typedef used by the test.
   //typedef std::map<int, std::string> TEST_MAP;
   //typedef SmallMap<int, std::string> TEST_MAP;
   typedef TreeMap TEST_MAP;

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
//   std::cout << "AISDI cwiczenie 4: wchodze do funkcji main." << std::endl;
//   test();
//   // Biblioteka z bardziej rygorystyczna wersja tych testow bedzie udostepniona na nastepnych zajeciach.
//   Test2();
//   //system("PAUSE");
//   return EXIT_SUCCESS;
//}
