#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <cstdlib>
#include <iostream>
#include "Avl.h"
#define INIT_SIZE 10
template<typename T>
class HashTable {
public:
    HashTable();
    HashTable(int size);
    virtual ~HashTable();
    HashTable(const HashTable &other) = delete;
    HashTable &operator=(const HashTable &other) = delete;
    void insert(const T &data);
    void remove(const T &data);
    T *find(const T &data);
    int hashFunction(const T &data);
    void clear();
    void merge(HashTable<T>& other_hash_table);

private:
    int table_size;
    int num_of_elements;
    Avl<T>* table;
    void enlarge();
    void reduce();

};

template<class T>
HashTable<T>::HashTable() {
    table = new Avl<T>[INIT_SIZE];
    table_size = INIT_SIZE;
    num_of_elements = 0;
}

template<class T>
HashTable<T>::HashTable(int size) {
    table = new Avl<T>[size];
    table_size = size;
}


template<class T>
HashTable<T>::~HashTable() {
    delete[] table;
}

template<class T>
void HashTable<T>::insert(const T &data)
{
    int index = hashFunction(data);
    table[index].insert(data);
    num_of_elements++;
    if (num_of_elements >= table_size)
        enlarge();
}

template<class T>
void HashTable<T>::remove(const T &data)
{
    int index = hashFunction(data);
    table[index].remove(data);
    num_of_elements--;
    if (num_of_elements * 4 <= table_size && table_size > 10)
        reduce();
}

template<class T>
T *HashTable<T>::find(const T &data)
{
    int index = hashFunction(data);
    T* result = table[index].find(data);
    return result;
}

template<class T>
void HashTable<T>::merge(HashTable<T>& other_hash_table)
{
    T temp_data;
    for (int i =0 ; i < other_hash_table.table_size ; i++) {
        while (other_hash_table.table[i].getRoot() != NULL){
            temp_data = other_hash_table.table[i].getRootData();
            insert(temp_data);
            temp_data.setHimselfPointer(nullptr);
            other_hash_table.table[i].remove(temp_data);
        }
    }
}

template<class T>
int HashTable<T>::hashFunction(const T &data)
{
    return data % table_size;
}

template<class T>
void HashTable<T>::enlarge()
{
    table_size = table_size*2;
    //create new array
    Avl<T> *temp_table = new Avl<T>[table_size];
    T* temp_data = nullptr;
    for (int i=0;i<table_size/2;i++){
        while (table[i].getRoot() != NULL) {
            temp_data = table[i].getRootData();
            temp_table[hashFunction(*temp_data)].insert(*temp_data);
            temp_data->setHimselfPointer(nullptr);
            table[i].remove(*temp_data);
        }
    }
    delete[] table;
    table = new Avl<T>[table_size];
    for (int i=0;i<table_size;i++){
        while (temp_table[i].getRoot() != NULL) {
            temp_data = temp_table[i].getRootData();
            table[i].insert(*temp_data);
            temp_data->setHimselfPointer(nullptr);
            temp_table[i].remove(*temp_data);
        }
    }
    delete[] temp_table;
}

template<class T>
void HashTable<T>::reduce()
{
    table_size = table_size/2;
    //create new array
    Avl<T> *temp_table = new Avl<T>[table_size];
    T temp_data;
    for (int i=0;i<table_size*2;i++){
        while (table[i].getRoot() != NULL) {
            temp_data = table[i].getRootData();
            temp_table[hashFunction(temp_data)].insert(temp_data);
            temp_data.setHimselfPointer(nullptr);
            table[i].remove(temp_data);
        }
    }
    delete[] table;
    table = new Avl<T>[table_size];
    for (int i=0;i<table_size;i++){
        while (temp_table[i].getRoot() != NULL) {
            temp_data = temp_table[i].getRootData();
            table[i].insert(temp_data);
            temp_data.setHimselfPointer(nullptr);
            temp_table[i].remove(temp_data);
        }
    }
    delete[] temp_table;
}


#endif //HASHTABLE_H
