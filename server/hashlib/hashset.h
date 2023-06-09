#include "../linkedlib/linkedlist.h"
#include <functional>
#include "../include/pack109.hpp"

using namespace pack109;

template <class T>
class HashSet {
  private:
    // The backbone of the hash set. This is an array of Linked List pointers.
    linkedlist::LinkedList<T>** array;

    // The number of buckets in the array
    size_t size; 

    // Generate a prehash for an item with a given size
    unsigned long prehash(T item);

  public:
    
    // Initialize an empty hash set, where size is the number of buckets in the array
    HashSet(size_t size);
    HashSet();
    // Free all memory allocated by the hash set
    ~HashSet();

    // Hash an unsigned long into an index that fits into a hash set
    unsigned long hash(T item);

    // Insert item in the set. Return true if the item was inserted, false if it wasn't (i.e. it was already in the set)
    // Recalculate the load factor after each successful insert (round to nearest whole number).
    // If the load factor exceeds 70 after insert, resize the table to hold twice the number of buckets.
    bool insert(T item);

    // Remove an item from the set. Return true if it was removed, false if it wasn't (i.e. it wasn't in the set to begin with)
    bool remove(T item);

    // Return true if the item exists in the set, false otherwise
    bool contains(T item);

    // Resize the underlying table to the given size. Recalculate the load factor after resize
    void resize(size_t new_size);

    // Returns the number of items in the hash set
    size_t len();

    // Returns the number of items the hash set can hold before reallocating
    size_t capacity();

    // Print Table. You can do this in a way that helps you implement your hash set.
    void print();

    //ADDITIONAL METHODS FOR THE FINAL

    //inserts the k/v pair into the hash map. Returns true if the key already existed,
    //and replaces the stored value with the supplied value. Returns false if the key did not exist already.
    bool insert(string key, vec value);

   // struct_file get(string key);
};



template <class T>
bool HashSet<T>::insert(T item)
{
  if(this->contains(item)==true)
  {
    return false;
  }
  unsigned long hashed = hash(item);
  this->array[hashed]->insertAtTail(item);
  int buckets = 0;
  for(int i =0;i<this->size;i++)
  {
    if(this->array[i]->length>0)
    {
      buckets++;
    }
  }
  double loadfactor = (double)(buckets)/(double)(this->size);
  if(loadfactor>=0.70)
  {
    this->resize(this->size*2);
  }
  return true;
}

// inserts the k/v pair into the hash map. Returns true if the key already existed,
// and replaces the stored value with the supplied value. Returns false if the key did not exist already.
template<class T>
bool HashSet<T>::insert(string key,vec value){
    struct file_struct newFile = {key,value};
    if(this-> contains(newFile) == true){
      std::cout << "contains";
      this->remove(newFile);

      return true;
    }


    unsigned long hashed = hash(newFile);
    this->array[hashed]->insertAtTail(newFile);
    int buckets = 0;
  for(int i =0;i<this->size;i++)
  {
    if(this->array[i]->length>0)
    {
      buckets++;
    }
  }
  double loadfactor = (double)(buckets)/(double)(this->size);
  if(loadfactor>=0.70)
  {
    this->resize(this->size*2);
  }


    return false;
}

// template<class T>
// struct_file HashSet<T>::get(string key){
//     unsigned long hashed = hash(key);

//     if (contains(name)) {
//         struct File find;
//         for (int i = 0; i < size; i++) {
//             find = this->array[i]->getFileFromName(name);
//             if (find.name == name) {
//                 vec bytes = pack109::serialize((struct File) find);
//                 return bytes;
//             }
//         }

//     }

// }



template <class T>
unsigned long HashSet<T>::prehash(T item) 
{
    int s = sizeof(item.name);
    int k = 0;
    unsigned long h = 5381;
    char* myptr = (char*)&(item.name);
    while (k<s) 
    {
      char* c = (char*)(myptr+k); 
	    h = ((h << 5) + h) + *c;
      k++;
    }
    return h;
}

template <class T>
HashSet<T>::HashSet(size_t size)
{
  this->array = new linkedlist::LinkedList<T>*[size];
  for(int i =0;i<size;i++)
  {
    this->array[i] = new linkedlist::LinkedList<T>();
  }
  this->size=size;
}

template<class T>
HashSet<T>::HashSet()
{
  this->array = new linkedlist::LinkedList<T>*[10];
  for(int i =0;i<10;i++)
  {
    this->array[i] = new linkedlist::LinkedList<T>();
  }
  this->size=10;
}

template <class T>
HashSet<T>::~HashSet()
{
  for(int i = 0;i<this->size;i++)
  {
    array[i]->~LinkedList();
  }
}

template <class T>
unsigned long HashSet<T>::hash(T item)
{
  unsigned long pre = this->prehash(item);
  unsigned long s = (unsigned long)(pre%this->size);
  return s;
}


//removes item in hashmap, if it does not exist, returns false
//else it returns true 
template <class T>
bool HashSet<T>::remove(T item)
{
  string name = "null";
  vec temp_vec = {0};
  struct file_struct null_file = {name ,temp_vec};
  if(this->contains(item)==false)
  {
    printf("\nret first false in remove hash");
    return false;
  }
  unsigned long hashed = hash(item);
  T s = this->array[hashed]->remove(item);
  if(s == null_file)
  {
        printf("\nret second false in remove hash");

    return false;
  }

    printf("\nret third false in remove hash");

  return true;
}

template <class T>
bool HashSet<T>::contains(T item)
{
  unsigned long hashed = hash(item);
  bool res = this->array[hashed]->containsItem(item);
  return res;
}

template <class T>
void HashSet<T>::resize(size_t new_size)
{
  linkedlist::LinkedList<T>** old = this->array;
  T element[this->len()];
  size_t limit = this->size;
  size_t currentindex = 0;
  for(size_t i =0;i<limit;i++)
  {
    if(this->array[i]->length>0)
    {
      for(int j = 0;j<this->array[i]->length;j++)
      {
        element[currentindex++] = this->array[i]->itemAtIndex(j);
      }
    }
  }
  this->size = new_size;
  this->array = new linkedlist::LinkedList<T>*[new_size];
  for(int i =0;i<new_size;i++)
  {
    this->array[i] = new linkedlist::LinkedList<T>();
  }
  for(int i =0;i<currentindex;i++)
  {
    this->insert(element[i]);
  }
  for(int i = 0;i<limit;i++)
  {
    old[i]->~LinkedList();
  }
}

template <class T>
size_t HashSet<T>::len()
{
  size_t len = 0;
  for(int i =0;i<this->size;i++)
  {
    len += this->array[i]->length;
  }
  return len;
}


template <class T>
size_t HashSet<T>::capacity()
{
  return (size_t)(0.7*(double)this->size) - 1;
}

template <class T>
void HashSet<T>::print()
{
  for(int i =0;i<this->size;i++)
  {
    printf("List %d: ",i+1);
    this->array[i]->printList();
    printf("\n");
  }
}
