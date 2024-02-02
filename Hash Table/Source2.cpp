#include "Header.h"

void initWord(Word &w)
{
    w.word = "";
    w.type = "";
    w.meaning.push_back("");
    w.origin = "";
}

// Knuth's hash func
int hashFunction(int val,  int tableSize)
{
    const double A = (sqrt(5) - 1) / 2;
    double frac = val * A - floor(val * A);
    int hash = floor(tableSize * frac);
    return hash;
}

