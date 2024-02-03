#include "Header.h"

bool isContainingAlphaNum(string str)
{
    for (int i = 0; i < size(str); ++i)
        if (isalnum(str[i]))
            return true;
    return false;
}

bool readDatabase(string fname, vector<Word>& data)
{
    ifstream fp;
    fp.open(fname, ios::in);
    if (!fp.is_open())
        return false;
    // else:
    string currentLine = "", lineBefore = "";
    Word temp; initWord(temp);
    int count = 0;
    while (getline(fp, currentLine))
    {
        if (!size(currentLine) || !isContainingAlphaNum(currentLine) || !currentLine.compare(lineBefore))
            continue;
        // else:
        int pos1 = currentLine.find("  "), pos2 = currentLine.find_first_of(" ",  pos1 + 2);
        temp.word = currentLine.substr(0, pos1);
        temp.type.push_back(currentLine.substr(pos1 + 2, pos2 - pos1 - 1));
        size_t pos3 = currentLine.find("[", pos2 + 1);
        if (pos3 == string::npos)
        {
            pos3 = currentLine.size();
            temp.meaning = currentLine.substr(pos2 + 1, pos3 - pos2 - 1);
        }
        else
        {

        }

        lineBefore = currentLine;
    }
}