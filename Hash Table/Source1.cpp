#include "Header.h"

bool isContainingAlphaNum(string str)
{
    for (int i = 0; i < size(str); ++i)
        if (isalnum(str[i]))
            return true;
    return false;
}

bool isWordType(string str)
{
    vector<string> types = {"n", "n.pl", "v", "adj", "Adj", "adv", "Adv", "abbr", "Abbr", "prep", "comb", "contr", "int", "symb", "pron"};
    for (string i : types)
        if (str.find(i) && str.size() - i.size() == 1)
            return true;
    return false;
}

vector<string> splitIntoMeanings(string tempMeaning, vector<string>& extraTypes) 
{
    vector<string> result;
    if (tempMeaning.empty())
        return result;
    // else
    int i = 0;
    string container = "";
    while (i < tempMeaning.size() && tempMeaning[i] != 127)
    {
        if (isdigit(tempMeaning[i]) && tempMeaning[i + 1] == ' ')
        {   //while (!(isdigit(tempMeaning[i]) && tempMeaning[i + 1] == ' ') || !(tempMeaning[i] == 127))
            while (tempMeaning[i] != '.' || tempMeaning[i + 1] != ' ')
            {
                container.push_back(tempMeaning[i]);
                i++;
            }
            result.push_back(container);
            container.clear();
            i += 2;
        }
        else if (tempMeaning[i] == '(')
        {
            while (tempMeaning[i] != ')')
            {
                container.push_back(tempMeaning[i]);
                i++;
            }
            container.push_back(tempMeaning[i++]);
            container.push_back(tempMeaning[i++]);
        }
        else if (tempMeaning[i] == '[')
        {
            while (tempMeaning[i] != ']')
            {
                container.push_back(tempMeaning[i]);
                i++;
            }
            container.push_back(tempMeaning[i++]);
            result.push_back(container);
        }
        else
        {
            while (!(tempMeaning[i] == '.' && tempMeaning[i + 1] == ' '))
            {
                container.push_back(tempMeaning[i]);
                i++;
            }
            if (isWordType(container))
            {
                extraTypes.push_back(container);
                i += 2;
                result.push_back("->");
                vector<string> result1 = splitIntoMeanings(tempMeaning.substr(i,  tempMeaning.length() - i), extraTypes);
                result.insert(result.end(), result1.begin(), result1.end());
            } 
            result.push_back(container);
            container.clear();
            i += 2;
        }
        
    }
    if (i < tempMeaning.size() && tempMeaning[i] == 127)
    {
        while (i < tempMeaning.size())
            container.push_back(tempMeaning[i++]);
        result.push_back(container);
    }
    return result;
    // }
    // else
    // {
    //     while (i < tempMeaning.size() && tempMeaning[i] != 127)
    //     {
    //         container.push_back(tempMeaning[i]);
    //         i++;
    //     }
    //     if (i < tempMeaning.size() && tempMeaning[i] == 127)
    //     {
    //         container.clear();
    //         while (i < tempMeaning.size())
    //             container.push_back(tempMeaning[i++]);
    //         result.push_back(container);
    //     }
    //     return result;
    // }
}

string eliminateEndingNumberFromWord(string str)
{
    if (str.empty())
        return str;
    // else
    string result = ""; result.assign(str);
    for (int lastIndex = str.size() - 1; isdigit(result[lastIndex]); --lastIndex)
        result.pop_back();
    return result;
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
        // else, process the line read:
        int pos0 = 0; // the beginning of a line

        // the word/vocabulary
        int pos1 = currentLine.find("  ");
        temp.word = currentLine.substr(pos0, pos1);
        temp.word = eliminateEndingNumberFromWord(temp.word);

        // the word type
        int pos2 = currentLine.find_first_of(" ",  pos1 + 2);
        temp.type.push_back(currentLine.substr(pos1 + 2, pos2 - pos1 - 1));
        if (!isWordType(temp.type[0]))
        {
            int pos2_5 = currentLine.find_first_of(" ",  pos2 + 1);
            temp.type[0] += currentLine.substr(pos2, pos2_5 - pos2 - 1);
            pos2 = pos2_5;
        }

        // the meaning of the word/vocabulary
        temp.meaning = splitIntoMeanings(currentLine.substr(pos2), temp.type);

        // finish the data processing
        data.push_back(temp);
        lineBefore = currentLine;
    }
}

vector<Word> convertHashIntoArr(HashTable ht)
{
    
}

bool writeDatabase(string fname, vector<Word> data)
{

}