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
    string type;
    string origin;
    vector<string> meaning;
};

struct HashTable
{
    vector <vector <list <Word>>> table;
    int numberOfElements;
    vector<int> sizeEachRow;
};

