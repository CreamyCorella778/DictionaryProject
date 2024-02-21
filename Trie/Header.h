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
