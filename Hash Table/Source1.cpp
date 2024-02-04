#include "Header.h"

bool isContainingAlphaNum(string str)
{
    for (int i = 0; i < size(str); ++i)
        if (isalnum(str[i]))
            return true;
    return false;
}

vector<string> splitIntoMeanings(string tempMeaning) 
{
    vector<string> result;
    int i = 0;
    string container = "";
    if (isdigit(tempMeaning[i]) && tempMeaning[i + 1] == ' ')
    {
        while (i < tempMeaning.size() && tempMeaning[i] != 127)
        {
            if (isdigit(tempMeaning[i]) && tempMeaning[i + 1] == ' ')
                //while (!(isdigit(tempMeaning[i]) && tempMeaning[i + 1] == ' ') || !(tempMeaning[i] == 127))
                while (tempMeaning[i] != '.')
                {
                    container.push_back(tempMeaning[i]);
                    i++;
                }
            else if ()
            result.push_back(container);
            container.clear();
            ++i;
        }
        if (i < tempMeaning.size() && tempMeaning[i] == 127)
        {
            while (i < tempMeaning.size())
                container.push_back(tempMeaning[i++]);
            result.push_back(container);
        }
        return result;
    }
    else
    {
        while (i < tempMeaning.size() && tempMeaning[i] != 127)
        {
            container.push_back(tempMeaning[i]);
            i++;
        }
        if (i < tempMeaning.size() && tempMeaning[i] == 127)
        {
            container.clear();
            while (i < tempMeaning.size())
                container.push_back(tempMeaning[i++]);
            result.push_back(container);
        }
        return result;
    }
}

bool readDatabase(string fname, vector<Word>& data)
{
    ifstream fp;
    fp.open(fname, ios::in);
    if (!fp.is_open())
        return false;
    // else:
    string currentLine = "", lineBefore = "";
    /* temporary container for meaning part of a word */
    string tempMeaning = ""  ;
    Word temp; initWord(temp);
    int count = 0;
    while (getline(fp, currentLine))
    {
        if (!size(currentLine) || !isContainingAlphaNum(currentLine) || !currentLine.compare(lineBefore) || size(currentLine) == 1)
            continue;
        // else:
        int pos1 = currentLine.find("  "), pos2 = currentLine.find_first_of(" ",  pos1 + 2);
        temp.word = currentLine.substr(0, pos1);
        temp.type.push_back(currentLine.substr(pos1 + 2, pos2 - pos1 - 1));
        size_t pos3 = currentLine.find("[", pos2 + 1);
        if (pos3 == string::npos)
        {
            pos3 = currentLine.size();
            tempMeaning = currentLine.substr(pos2 + 1, pos3 - pos2 - 1);
        }
        else
        {

        }

        lineBefore = currentLine;
    }
}