#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <list>
#include <string>

using namespace std;

struct Word
{
    string word;
    vector<string> type;
    vector<string> meaning;
};

struct HashTable
{
    vector <vector <list <Word>>> table;
    int numberOfElements;
    vector<int> sizeEachRow;
};

//---------------------------------------------------------
void initWord(Word& w, string word = "", string type = "", vector<string> meaning = {});
bool readDatabase(string fname, vector<Word>& data);
bool writeDatabase(string fname, vector<Word> data);
//--------------
void putToTable(vector<Word> arr, HashTable& ht);
bool lookUpTable(HashTable ht, Word w, vector<list<Word>::iterator>& iters);
void insertAnElement(HashTable& ht, Word w);
void deleteAnElement(HashTable& ht, Word w);
//-------------
void addAWord(HashTable& ht);
void editAWord(HashTable& ht);
void lookUpDictionary(HashTable ht);
void deleteAWord(HashTable& ht);
//---------------
void menu();
