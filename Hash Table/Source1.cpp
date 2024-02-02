#include "Header.h"

bool isContainingAlphaNum(string str)
{
    for (int i = 0; i < size(str); ++i)
        if (isalnum(str[i]))
            return false;
    return true;
}

bool readDatabase(string fname, HashTable &ht)
{
    ifstream fp;
    fp.open(fname, ios::in);
    if (!fp.is_open())
        return false;
    // else:
    string singleLine = "", temp = "";
    int count = 0;
    while (getline(fp, singleLine))
    {
        if (isContainingAlphaNum(singleLine))
            continue;
        // else:
        int pos1 = singleLine.find_first_of("  "), pos2 = singleLine.find_first_of(" ",  pos1 + 1);
         = singleLine.substr(0pos2 - pos1 - 1);

    }
}