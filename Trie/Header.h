#pragma once
#include <iostream>
#include <iomanip>
#include <cctype>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <unordered_map>
#include <string>
#include <random>

#define ALPHABET_SIZE 30
#define ALPHABET_FILENAME "characters appearing in words.txt"

using namespace std;

int noCaseSensitiveCompare(string a, string b);

struct Word
{
    string word;
    vector<string> type;
    vector<string> meaning;

    // bool operator<(Word b)
    // {
    //     return noCaseSensitiveCompare(this->word, b.word) < 0;
    // }

    // bool operator==(Word b)
    // {
    //     if (!noCaseSensitiveCompare(this->word, b.word))
    //         return false;
    //     if (b.type.size() != this->type.size())
    //         return false;
    //     for (int i = 0; i < this->type.size(); ++i)
    //         if (this->type[i].compare(b.type[i]) != 0)
    //             return false;
    //     if (b.meaning.size() != this->meaning.size())
    //         return false;
    //     for (int i = 0; i < this->meaning.size(); ++i)
    //         if (this->meaning[i].compare(b.meaning[i]) != 0)
    //             return false;
    // }

    // bool operator>(Word b)
    // {
    //     return noCaseSensitiveCompare(this->word, b.word) > 0;
    // }

    // bool operator>=(Word b)
    // {
    //     return !this->operator<(b);
    // }

    // bool operator<=(Word b)
    // {
    //     return !this->operator>(b);
    // }
};

struct Node
{
    Word data;
    unordered_map <char, Node*> children;
    bool isCompleteWordInDict;
};

//==========================================================
bool isWordType(string str);
Word initWord(string word = "", string type = "", vector<string> meaning = {});
Node* createNode(Word data = initWord(), bool isValidWordInDict = false);
bool readDatabase(string fname, vector<Word>& data);
bool writeDatabase(string fname, vector<Word> data);
int finalProgram();
//-----------------------------------------------------------
void insertAnElement(Node*& root, Word w);
Node** searchForElement(Node* root, string word);
bool deleteAnElement(Node*& root, string word);
Node* putToTrie(vector<Word> data);
//-----------------------------------------------------------
void addAWord(Node*& root);
void editAWord(Node*& root);
void lookUpDictionary(Node* root);
void deleteAWord(Node*& root);
int outputToFile(Node* root);