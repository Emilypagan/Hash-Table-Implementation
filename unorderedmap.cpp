/** CS515 Assignment 10
 * File: unorderedmap.cpp
 * Name: Mily Pagan
 * Section: 02
 * Date: 11/17/20
 * Collaboration Declaration: piazza */

/////////////////////////////////////////////////////////
//rehash
template <typename KEY, typename T, typename H>
void UnorderedMap<KEY, T, H>::rehash() {
    int n = tablesize; //old array tablesize
    tablesize = tablesize*2;
    if (!isPrime(tablesize)) {
        tablesize = nextPrime(tablesize);
    }
    Entry **newArray = new Entry*[tablesize]; //new array for map
    for (int i = 0; i < tablesize; i++) {
        newArray[i] = NULL; //initializing variables
    }

    for (int i = 0; i < n; i++) {
        if (array[i] != NULL) {
            Entry *prev = NULL;
            Entry *cur = array[i];
            while (cur != NULL) {
                prev = cur;
                cur = cur->next;
                KEY &newkey = prev->key;
                T &data = prev->data;
                //entering the original data into newArray
                std::size_t key = H()(newkey);
                int hashindex = key % tablesize;
                Entry *previous = NULL;
                Entry *current = newArray[hashindex];
                while (current != NULL) {
                    previous = current;
                    current = current->next;
                }
                if (current == NULL) {
                    current = new Entry(newkey, data);
                    current->next = NULL;
                    if (previous == NULL) {
                        newArray[hashindex] = current;
                    } else {
                        previous->next = current;
                    }
                }
            } //while loop end
        } //end of if statement
    }

    //deallocate old array
    for (int i = 0; i < n; i++) {
        if (array[i] != NULL) {
            Entry *prev = NULL;
            Entry *cur = array[i];
            while (cur != NULL) {
                prev = cur;
                cur = cur->next;
                delete prev;
            }
        }
    }
    delete[] array;

    //assign to newArray
    array = newArray;
}
/////////////////////////////////////////////////////////
//constructor
template <typename KEY, typename T, typename H>
UnorderedMap<KEY, T, H>::UnorderedMap() {
    tailEnd = new Entry(); //dummy end() Entry
    tablesize = 101; //initial table size
    entrysize = 0; //empty table
    array = new Entry *[tablesize]; //array of linked lists
    for (int i = 0; i < tablesize; i++) {
        array[i] = NULL; //initializing variables
    }
}
/////////////////////////////////////////////////////////
//copy constructor
template <typename KEY, typename T, typename H>
UnorderedMap<KEY, T, H>::UnorderedMap(const UnorderedMap& rhs) {
    tailEnd = new Entry();
    tablesize = rhs.tablesize;
    entrysize = rhs.entrysize;
    array = new Entry *[tablesize];
    for (int i = 0; i < tablesize; i++) {
        array[i] = NULL;
    }

    for (int i = 0; i < rhs.tablesize; i++) {
        if (rhs.array[i] != NULL) {
            Entry *prev = NULL;
            Entry *cur = rhs.array[i];
            while (cur != NULL) {
                prev = cur;
                cur = cur->next;
                insert(prev->key, prev->data);
            }
        }
    }
}
/////////////////////////////////////////////////////////
//assignment operator
template <typename KEY, typename T, typename H>
UnorderedMap<KEY,T,H>& UnorderedMap<KEY,T,H>::operator=(const UnorderedMap& rhs) {

    return *this;
}
/////////////////////////////////////////////////////////
//destructor
template <typename KEY, typename T, typename H>
UnorderedMap<KEY, T, H>::~UnorderedMap() {
    for (int i = 0; i < tablesize; i++) {
        if (array[i] != NULL) {
            Entry *prev = NULL;
            Entry *cur = array[i];
            while (cur != NULL) {
                prev = cur;
                cur = cur->next;
                delete prev;
            }
        }
    }
    delete tailEnd;
    delete[] array;
}
/////////////////////////////////////////////////////////
//begin iterator
template <typename KEY, typename T, typename H>
typename UnorderedMap<KEY, T, H>::Iterator UnorderedMap<KEY, T, H>::begin() const {
    // stub code; need to implement !!!
    int i = 0;
    while (array[i] == NULL) {
        i++;
    }
    Entry *cur = array[i];
    return Iterator(cur);

}
/////////////////////////////////////////////////////////
//end iterator
template <typename KEY, typename T, typename H>
typename UnorderedMap<KEY, T, H>::Iterator UnorderedMap<KEY, T, H>::end() const {
    // stub code; need to implement !!!
    return Iterator(tailEnd);
}
/////////////////////////////////////////////////////////
//insert
template <typename KEY, typename T, typename H>
bool UnorderedMap<KEY, T, H>::insert(const KEY & newkey, const T & data) {

    int loadFactor = (entrysize+1)/tablesize;
    if (loadFactor >= 2) {
        rehash();
    }

    if (find(newkey) == end()) {
        std::size_t key = H()(newkey);
        int hashindex = key % tablesize;
        Entry *prev = NULL;
        Entry *cur = array[hashindex];
        while (cur != NULL) {
            prev = cur;
            cur = cur->next;
        }
        if (cur == NULL) {
            cur = new Entry(newkey, data);
            cur->next = NULL;
            if (prev == NULL) {
                array[hashindex] = cur;
            } else {
                prev->next = cur;
            }
        }
        entrysize++;
        return true;
    }
    return false;
}
/////////////////////////////////////////////////////////
//erase
template <typename KEY, typename T, typename H>
bool UnorderedMap<KEY, T, H>::erase(const KEY& newkey) {
    std::size_t key = H()(newkey);
    int hashindex = key % tablesize;
    Entry *prev = NULL;
    Entry *cur = array[hashindex];
    if (cur == NULL || cur->key != newkey) {
        return false;
    }
    while (cur->next != NULL) {
        prev = cur;
        cur = cur->next;
    }
    if (prev != NULL) {
        prev->next = cur->next;
    }
    delete cur;
    entrysize--;
    return true;
}
/////////////////////////////////////////////////////////
//find
template <typename KEY, typename T, typename H>
typename UnorderedMap<KEY, T, H>::Iterator UnorderedMap<KEY, T, H>::find( const KEY & x ) const {
    std::size_t key = H()(x);
    int hashindex = key % tablesize; //get the index location of key x
    Entry *cur = array[hashindex];
    while (cur != NULL) { //search for the desired key x
        if (cur->key == x) {
            return Iterator(cur);
        }
        cur = cur->next;
    }
    return end(); //return end() iterator if cannot find key x
}
/////////////////////////////////////////////////////////
//index [] operator
template<typename KEY, typename T, typename H>
T& UnorderedMap<KEY, T, H>::operator[](const KEY & k){
    // stub code; need to implement !!!
    static T temp = T();
    Entry* e;
    Iterator it = find(k);
    if(it != end()){
        e = &*it;
        return e->data;
    } else {
        insert(k,temp);
        it = find(k);
        e = &*it;
        return e->data;
    }
}
/////////////////////////////////////////////////////////
// Internal method to test if a positive number is prime (not efficient)
bool isPrime( int n ){
    if( n == 2 || n == 3 )
        return true;
    if( n == 1 || n % 2 == 0 )
        return false;
    for( int i = 3; i * i <= n; i += 2 )
        if( n % i == 0 )
            return false;
    return true;
}
/////////////////////////////////////////////////////////
// Internal method to return a prime number at least as large as n.
// Assumes n > 0.
int nextPrime( int n ){
    if( n % 2 == 0 )
        n++;
    for( ; !isPrime( n ); n += 2 )
        ;
    return n;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//dereference operator
template <typename KEY, typename T, typename H>
typename UnorderedMap<KEY, T, H>::Entry& UnorderedMap<KEY, T, H>::Iterator::operator*(){
    return *_cur;
}
/////////////////////////////////////////////////////////
//reference operator
template <typename KEY, typename T, typename H>
typename UnorderedMap<KEY, T, H>::Entry* UnorderedMap<KEY, T, H>::Iterator::operator->(){
    return _cur;
}
/////////////////////////////////////////////////////////
//postfix operator
template <typename KEY, typename T, typename H>
typename UnorderedMap<KEY, T, H>::Iterator UnorderedMap<KEY, T, H>::Iterator::operator++(int){
    // stub code; need to implement !!!
    Iterator result = *this;
    _cur = _cur->next;
    return result;
    //return Iterator(NULL);
}
/////////////////////////////////////////////////////////
//not equal to operator
template <typename KEY, typename T, typename H>
bool UnorderedMap<KEY, T, H>::Iterator::operator!=(Iterator it){
    return _cur != it._cur;
}
/////////////////////////////////////////////////////////
//equality operator
template <typename KEY, typename T, typename H>
bool UnorderedMap<KEY, T, H>::Iterator::operator==(Iterator it){
    return _cur == it._cur;
}
/////////////////////////////////////////////////////////