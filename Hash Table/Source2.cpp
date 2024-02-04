#include "Header.h"

void initWord(Word& w, string word = "", string type = "", vector<string> meaning = {})
{
    w.word = word;
    w.type.push_back(type);
    w.meaning = meaning;
}

bool isInitialized(Word w)
{
    return !(w.word.empty() && w.type.empty() &&  (w.meaning.size() == 0 || w.meaning[0].empty()));
}

void initHashTable(HashTable& ht, int n)
{
    ht.table.resize(n);
    ht.numberOfElements = 0;
    ht.sizeEachRow.resize(n);
}

// Knuth's hash func
int hashFunction1(int val,  int tableSize)
{
    const double A = (sqrt(5) - 1) / 2;
    double frac = val * A - floor(val * A);
    int hash = floor(tableSize * frac);
    return hash;
}

// quadratic probing
int hashFunction2(int val, int tableSize, int hashTime)
{
    return (hashFunction1(val, tableSize) + int(pow(hashTime,2))) % tableSize;
}

int hashString(string str) {
    int hash = 0;
    int prime = 31;
    for (char c : str) {
        hash = hash * prime + c;
    }
    return hash;
}

//translate from Word to Integer
int encodeWord(Word w)
{
    int val = hashString(w.word);
    for (int i = 0; i < w.type.size(); ++i)
        val += hashString(w.type[i]);
    for (int i = 0; i < w.meaning.size(); ++i)
        val += hashString(w.meaning[i]);
    return val;
}

// ony use this function right after having initialized the hash table:
// add data from a big array to the table
void putToTable(vector<Word> arr, HashTable& ht)
{
    for (int i = 0; i < arr.size(); ++i)
    {
        int hashVal = hashFunction1(encodeWord(arr[i]), ht.table.size());
        ht.sizeEachRow[hashVal] += 1;
    }
    for (int i = 0; i < ht.table.size(); ++i)
    {
        Word w; initWord(w);
        ht.table[i].resize(pow(ht.sizeEachRow[i], 2));
    }
    for (int i = 0; i < arr.size(); ++i)
    {
        int hashVal1 = hashFunction1(encodeWord(arr[i]), ht.table.size());
        int hashVal2 = hashFunction2(hashVal1, pow(ht.sizeEachRow[hashVal1], 2), 2);
        ht.table[hashVal1][hashVal2].push_back(arr[i]);
    }
    ht.numberOfElements = arr.size();
}

bool lookUpTable(HashTable ht, Word w, list<Word>::iterator& it)
{
    int hashVal1 = hashFunction1(encodeWord(w), ht.table.size());
    int hashVal2 = hashFunction2(hashVal1, pow(ht.sizeEachRow[hashVal1], 2), 2);
    list <Word> l = ht.table[hashVal1][hashVal2];
    if (l.empty())
    {
        it = l.end();
        return true;
    }
    else 
    {
        for (list<Word>::iterator iter = l.begin(); iter != l.end(); ++iter)
            if (iter->word.compare(w.word))
            {
                it = iter;
                return true;
            }
        it = l.end();
        return false;
    }
}

void insertAnElement(HashTable& ht, Word w)
{
    int hashVal1 = hashFunction1(encodeWord(w), ht.table.size());
    int hashVal2 = hashFunction2(hashVal1, pow(ht.sizeEachRow[hashVal1], 2), 2);
    ht.table[hashVal1][hashVal2].push_back(w);
    ht.numberOfElements += 1;
}

void deleteAnElement(HashTable& ht, Word w)
{
    list<Word>::iterator iter;
    bool isExist = lookUpTable(ht, w, iter);
    if (!isExist)
        return;
    else    
    {
        int hashVal1 = hashFunction1(encodeWord(w), ht.table.size());
        int hashVal2 = hashFunction2(hashVal1, pow(ht.sizeEachRow[hashVal1], 2), 2);
        ht.table[hashVal1][hashVal2].erase(iter);
        ht.numberOfElements -= 1;
    }
}