#pragma once
#include <iostream>
#include <iomanip>
#include <cctype>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <list>
#include <string>
#include <random>

#define EXPECTED_SIZE 36620
// Theorem: The expected number of levels in a skip list with n entries is O(log n)
#define MAX_LEVEL int(round(log2(EXPECTED_SIZE))) + 1 
#define PROBABILITY 0.5f

using namespace std;

int noCaseSensitiveCompare(string a, string b);

struct Word
{
    string word;
    vector<string> type;
    vector<string> meaning;

    bool operator<(Word b)
    {
        return noCaseSensitiveCompare(this->word, b.word) < 0;
    }

    bool operator==(Word b)
    {
        if (!noCaseSensitiveCompare(this->word, b.word))
            return false;
        if (b.type.size() != this->type.size())
            return false;
        for (int i = 0; i < this->type.size(); ++i)
            if (this->type[i].compare(b.type[i]) != 0)
                return false;
        if (b.meaning.size() != this->meaning.size())
            return false;
        for (int i = 0; i < this->meaning.size(); ++i)
            if (this->meaning[i].compare(b.meaning[i]) != 0)
                return false;
    }

    bool operator>(Word b)
    {
        return noCaseSensitiveCompare(this->word, b.word) > 0;
    }

    bool operator>=(Word b)
    {
        return !this->operator<(b);
    }

    bool operator<=(Word b)
    {
        return !this->operator>(b);
    }
};

struct Node
{
    Word data;
    vector<Node*> forward;
};

struct SkipList
{
    Node* header; // even before the head of the linked list
    int max_level; // limit of currentLevel
    int currentLevel; // node with highest level inserted in list so far
    float prob; // probability of skipping a node
};

//=============================================================
Word initWord(string word = "", string type = "", vector<string> meaning = {});
Node* createNode(Word data = initWord(), int level = 0);
void initList(SkipList& skl, int max_level = 6, float prob = 0.5);
void insertAnElement(SkipList& skl, Node*& node);
void putToList(SkipList& skl, vector<Word> data);
vector<Node**> searchForElement(SkipList skl, Word key);
bool deleteAnElement(SkipList& skl, Node*& node);
//-------------------------------------------------------------
bool isWordType(string str);
bool readDatabase(string fname, vector<Word>& data);
bool writeDatabase(string fname, vector<Word> data);
//-------------------------------------------------------------
void addAWord(SkipList& skl);
void editAWord(SkipList& skl);
void lookUpDictionary(SkipList skl);
int outputToFile(SkipList skl);
void deleteAWord(SkipList& skl);
//-------------------------------------------------------------
int finalProgram();