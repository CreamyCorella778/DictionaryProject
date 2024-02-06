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
    const double A = (sqrt(5) - 1) / 2.0;
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
        int hashVal2 = hashFunction2(hashVal1, pow(ht.table[hashVal1].size(), 2), 2);
        ht.table[hashVal1][hashVal2].push_back(arr[i]);
    }
    ht.numberOfElements = arr.size();
}

bool lookUpTable(HashTable ht, Word w, vector<list<Word>::iterator>& iters)
{
    int hashVal1 = hashFunction1(encodeWord(w), ht.table.size());
    int hashVal2 = hashFunction2(hashVal1, pow(ht.table[hashVal1].size(), 2), 2);
    list <Word> l = ht.table[hashVal1][hashVal2];
    if (l.empty())
    {
        iters.push_back(l.end());
        return false;
    }
    else 
    {
        for (list<Word>::iterator iter = l.begin(); iter != l.end(); ++iter)
        {
            if (iter->word.compare(w.word) || iter->word.find(w.word))
                iters.push_back(iter);
            return true;
        }
        iters.push_back(l.end());
        return false;
    }
}

bool isARowFull(HashTable ht, int rowIdx)
{
    return  ht.table[rowIdx].size() == ht.sizeEachRow[rowIdx];
}

vector<Word> convertVectorOfListToVector(vector<list<Word>> vec)
{
    vector<Word> result;
    if (vec.empty())
        return result;
    // else
    
}

void reHashing(vector<list<Word> >& row, Word w)
{

}

void insertAnElement(HashTable& ht, Word w)
{
    int hashVal1 = hashFunction1(encodeWord(w), ht.table.size());
    if (isARowFull(ht, hashVal1))
    {

    }
    int hashVal2 = hashFunction2(hashVal1, pow(ht.table[hashVal1].size(), 2), 2);
    ht.table[hashVal1][hashVal2].push_back(w);
    ht.sizeEachRow[hashVal1]++;
    ht.numberOfElements++;
}

void deleteAnElement(HashTable& ht, Word w)
{
    vector<list<Word>::iterator> iters;
    bool isExist = lookUpTable(ht, w, iters);
    if (!isExist)
        return;
    else    
    {
        int hashVal1 = hashFunction1(encodeWord(w), ht.table.size());
        int hashVal2 = hashFunction2(hashVal1, pow(ht.table[hashVal1].size(), 2), 2);
        for (list<Word>::iterator it : iters)
        {
            ht.table[hashVal1][hashVal2].erase(it);
            ht.numberOfElements -= 1;
        }
    }
}

bool isIn(vector<int> arr, int val)
{
    if (arr.empty())
        return false;
    for (int i : arr)
        if (i == val)
            return true;
    return false;
}

vector<Word> convertTableIntoArr(HashTable ht)
{
    vector<Word> data;
    vector<int> hashHistory1, hashHistory2;
    for (int i = 0; i < ht.table.size(); ++i)
    {
        int hashVal1 = hashFunction1(i, ht.table.size());
        int hashTime = 1;
        while (isIn(hashHistory1, hashVal1))
            hashVal1 = hashFunction2(hashVal1, ht.table.size(), hashTime++);
        hashHistory1.push_back(hashVal1);
        for (int j = 0; j < ht.table[hashVal1].size(); ++j)
        {
            hashTime = 0;
            int hashVal2 = hashFunction2(j, ht.table[hashVal1].size(), hashTime);
            hashTime++;
            while (ht.table[hashVal1][hashVal2].empty() && isIn(hashHistory2, hashVal2))
                hashVal2 = hashFunction2(hashVal2, ht.table[hashVal1].size(), hashTime++);
            hashHistory2.push_back(hashVal2);
            for (list<Word>::iterator it = ht.table[hashVal1][hashVal2].begin(); it !=  ht.table[hashVal1][hashVal2].end(); ++it)
            {
                Word w = *it;
                data.push_back(w);
            }  
        }
        hashHistory2.clear();
    }
    return data;
}