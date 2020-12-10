#ifndef MAP_H
#define MAP_H

// Hash table implementation of UnorderedMap ADT. The container is generic
// Use external chaining with linked lists for collision resolution 

#include <cstdlib>
#include <functional> // for std::hash<T>
#include <iostream>
using namespace std;

template <typename KEY, typename T, typename H = std::hash<KEY> >
class UnorderedMap{
private:
	struct Entry{
		Entry(){}
		Entry(KEY newKey, T newData) : key(newKey), data(newData){}
		KEY key;
		T data;
		Entry *next;
	};    

	Entry ** array; // pointers to the hash table, each table entry is a pointer that used to point to a linked list of Entries.
    Entry *tailEnd;
	int tablesize;  // table size
	int entrysize;  // total number of entries
	void rehash();  // double the hashtable when the load factor reaches 2. 

public:
	// Constructs an empty unordered_map object, containing no elements and with a entry size of zero
	// Notice the hash table with a default size 101 is constructed but empty. 
	UnorderedMap();

	// Copy constructor The object is initialized to have the same contents and properties as the UnorderedMap object rhs. 
	UnorderedMap(const UnorderedMap& rhs); 

	// Assignment operator
	UnorderedMap& operator=(const UnorderedMap& rhs);

	// Destructs the container object. This calls each of the contained element's destructors, 
	// and dealocates all the storage capacity allocated by the UnorderedMap container.
	~UnorderedMap();

	// A simple forward iterator, traverse the collection in one direction. The order of traversal is not specified. 
	// But, in any case, the range that goes from its begin to its end covers all the elements in the container. 
	class Iterator{
	public:
		Iterator(){}
		explicit Iterator( Entry *cur):_cur(cur) {}
		Entry & operator*();		
		Entry * operator->();
		Iterator operator++(int); 
		bool operator!=(Iterator);
		bool operator==(Iterator);
	private:
		Entry * _cur;
	};

	// Returns an iterator pointing to the first element in the UnorderedMap container 
	// Note that an UnorderedMap object makes no guarantees on which specific element is considered its first element. 
	// But, in any case, the range that goes from its begin to its end covers all the elements in the container. 
	Iterator begin() const;

	// Returns an iterator pointing to the past-the-end element in the unordered_map container 
	// Note that an UnorderedMap object makes no guarantees on which order its elements follow. 
	// But, in any case, the range that goes from its begin to its end covers all the elements in the container. 
	Iterator end() const;

	// Inserts new elements in the unordered_map.
	// Each element is inserted only if its key is not equivalent to the key of any other element already in the container 
	// (keys in an UnorderedMap are unique). function returns true if new element is inserted, false otherwise. 
	bool insert(const KEY&, const T&);

	// Removes from the UnorderedMap container a single element 
	// retur true if the element is erased, false otherwise
	bool erase (const KEY&);

	// Searches the container for an element with the given key and returns an iterator to it if found, 
	// otherwise it returns an iterator to UnorderedMap::end() (the element past the end of the container).
	Iterator find(const KEY&) const;

	// If k matches the key of an element in the container, the function returns a reference to its mapped value.
	// If k does not match the key of any element in the container, the function inserts a new element with that key 
	// and returns a reference to its mapped value. Notice that this always increases the container size by one, 
	// even if no mapped value is assigned to the element (the element is constructed using its default constructor).
	T& operator[](const KEY&);

	// Returns the number of elements in the UnorderedMap container.
	int size(){ return entrysize; }

	// Returns the table size; included for verifying rehash operation. 
	int tsize(){ return tablesize; }
};

// check if the number n is prime
bool isPrime(int n);

// find the next prime number following given n
int nextPrime(int n);

#include "unorderedmap.cpp" // must include source file for template compilation
#endif
