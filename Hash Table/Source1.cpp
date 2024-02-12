#include "Header.h"

bool isNumber(char i)
{
    return 48 <= i && i <= 57;
}

bool isContainingURC(string str)
{
    string URC = {-17, -65, -67};
    return str.find(URC) < str.size();
}

void eraseURC(string& str)
{
    string URC = {-17, -65, -67};
    if (isContainingURC(str))
    {
        int pos = str.find(URC);
        str.erase(pos, URC.size());
    }
    return;
}

bool isOneURCApart(string a, string b) // URC = Unicode Replacement Character: ï¿½
{
    string URC = {-17, -65, -67};
    if (abs(int(a.size() - b.size())) != URC.size())
        return false;
    string temp = a.size() >  b.size() ? a : b, smaller = a.size() <=  b.size() ? a : b;
    temp.erase(temp.find(smaller), smaller.size()); 
    if (temp == URC)
        return true;
    else
        return false;
}

//bool isContaining(string str)
//{
//    string URC = { -17, -65, -67 };
//    return str.find(URC) < str.size();
//}
//
//void eraseURC(string& str)
//{
//    string URC = { -17, -65, -67 };
//    if (isContainingURC(str))
//    {
//        int pos = str.find(URC);
//        str.erase(pos, URC.size());
//    }
//    return;
//}
//
//bool isOneURCApart(string a, string b) // URC = Unicode Replacement Character: ï¿½
//{
//    string URC = { -17, -65, -67 };
//    if (abs(int(a.size() - b.size())) != URC.size())
//        return false;
//    string temp = a.size() > b.size() ? a : b, smaller = a.size() <= b.size() ? a : b;
//    temp.erase(temp.find(smaller), smaller.size());
//    if (temp == URC)
//        return true;
//    else
//        return false;
//}

bool isWordType(string str)
{
    vector<string> types = {"n.", "n.pl.", "v.", "adj.", "Adj.", "adv.", "Adv.", "abbr.", "Abbr.", "prep.", "comb.", "contr.", "int.", "symb.", "pron."};
    for (string i : types)
        if (str.find(i) < str.size() && (str == i || isOneURCApart(str, i)))
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
        if (isNumber(tempMeaning[i]) && tempMeaning[i + 1] == ' ')
        {   
            while (i + 1 < tempMeaning.size() && (tempMeaning[i] != '.' || tempMeaning[i + 1] != ' '))
            {
                if (tempMeaning[i] == '(')
                    while (i < tempMeaning.size() && tempMeaning[i] != ')')
                    {
                        container.push_back(tempMeaning[i]);
                        i++;
                    }
                if (i < tempMeaning.size())
                {
                    container.push_back(tempMeaning[i]);
                    i++;
                }
            }
            container.push_back(tempMeaning[i]);
            eraseURC(container);
            result.push_back(container);
            container.clear();
            i += 2;
        }
        else if (tempMeaning[i] == '(')
        {
            while (i < tempMeaning.size() && tempMeaning[i] != ')')
            {
                container.push_back(tempMeaning[i]);
                i++;
            }
            if (i < tempMeaning.size())
            {
                container.push_back(tempMeaning[i++]);
                container.push_back(tempMeaning[i++]);
            }
            eraseURC(container);
        }
        else if (tempMeaning[i] == '[')
        {
            while (i < tempMeaning.size() && tempMeaning[i] != ']')
            {
                container.push_back(tempMeaning[i]);
                i++;
            }
            if (i < tempMeaning.size())
                container.push_back(tempMeaning[i++]);
            eraseURC(container);
            result.push_back(container);
        }
        else
        {
            while (i + 1 < tempMeaning.size() && !(tempMeaning[i] == '.' && tempMeaning[i + 1] == ' '))
            {
                if (tempMeaning[i] == '(')
                    while (i < tempMeaning.size() && tempMeaning[i] != ')')
                    {
                        container.push_back(tempMeaning[i]);
                        i++;
                    }
                if (i < tempMeaning.size())
                {
                    container.push_back(tempMeaning[i]);
                    i++;
                }
            }
            container.push_back(tempMeaning[i]);
            if (isWordType(container))
            {
                eraseURC(container);
                extraTypes.push_back(container);
                i += 2;
                result.push_back("");
                if (i < tempMeaning.size())
                {
                    string nextMeaningToSplit = tempMeaning.substr(i, tempMeaning.length() - i);
                    vector<string> result1 = splitIntoMeanings(nextMeaningToSplit, extraTypes);
                    result.insert(result.end(), result1.begin(), result1.end());
                }
                container.clear();
                return result;
            } 
            else
            {
                eraseURC(container);
                result.push_back(container);
                container.clear();
                i += 2;
            }
        }
        
    }
    if (i < tempMeaning.size() && tempMeaning[i] == 127)
    {
        while (i < tempMeaning.size())
            container.push_back(tempMeaning[i++]);
        eraseURC(container);
        result.push_back(container);
    }
    return result;
}

string eliminateEndingNumberFromWord(string str)
{
    if (str.empty())
        return str;
    // else
    string result = ""; result.assign(str);
    for (int lastIndex = str.size() - 1; isNumber(result[lastIndex]); --lastIndex)
        result.pop_back();
    return result;
}

string getWordType(string currentLine, string& tempMeaning)
{
    string wordType, container;
    int pos1 = 0, pos2 = currentLine.find_first_of(" ", pos1 + 1);
    do
    {
        container = currentLine.substr(pos1, pos2 - pos1);
        if (isWordType(container))
        {
            if (currentLine[pos2 + 1] == '&')
            {
                int pos2_5 = currentLine.find_first_of(" ",  pos2 + 3);
                container = currentLine.substr(pos1, pos2_5 - pos1);
                pos2 = pos2_5;
            }
            wordType = container;
            container = currentLine.substr(pos2 + 1, currentLine.size() - pos2 - 1);
            tempMeaning += container;
            break;
        }
        else
        {
            tempMeaning += container;
            pos1 = pos2 + 1;
            if (pos1 - 1 == currentLine.find_last_of(" "))
            {
                tempMeaning += " ";
                tempMeaning += currentLine.substr(pos1, currentLine.size() - pos1);
                pos2 = currentLine.size();
            }
            else
                pos2 = currentLine.find_first_of(" ", pos1);
            if (pos2 < currentLine.size())
                tempMeaning += " ";
        }
    } while (pos2 < currentLine.size());
    eraseURC(wordType);
    return wordType;
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
    string tempMeaning = "";
    Word temp; initWord(temp);
    int count = 0;
    while (getline(fp, currentLine))
    {
        if (size(currentLine) <= 1 || !currentLine.compare(lineBefore))
            continue;
        // else, process the line read:
        int pos0 = 0; // the beginning of a line

        // the word/vocabulary
        int pos1 = currentLine.find("  ");
        temp.word = currentLine.substr(pos0, pos1);
        temp.word = eliminateEndingNumberFromWord(temp.word);
        eraseURC(temp.word);

        // the word type
        string meaningToSplit, wordType = getWordType(currentLine.substr(pos1 + 2, currentLine.size() - pos1 - 2), meaningToSplit);
        if (wordType != "")
            temp.type.push_back(wordType);

        // the meaning of the word/vocabulary
        temp.meaning = splitIntoMeanings(meaningToSplit, temp.type);

        // finish the data processing
        data.push_back(temp);
        lineBefore = currentLine;
        initWord(temp);
        temp.type.clear();
    }
}

bool writeDatabase(string fname, vector<Word> data)
{
    ofstream fp;
    fp.open(fname, ios::trunc);
    if (!fp.is_open())
        return false;
    // else
    for (Word i : data)
    {
        fp << i.word << "  ";
        vector<string>::iterator it = i.meaning.begin();
        for (string j : i.type)
        {
            fp << j << " ";
            while (it != i.meaning.end() && *it != "")
                fp << *it << " ";
        }
    }
    return true;
}

int menu1_InputFromFile(vector<Word>& data)
{
    int choice = 0;
    cout << "Chon chuc nang:" << endl;
    cout << "1. Nhap du lieu tu tep" << endl;
    cout << "So khac. Khong lam gi nua: ";
    cin >> choice;
    if (choice != 1)
    {
        cout << "Chuc ban may man lan sau." << endl;
        return -1;
    }
    else
    {
        string fname; int count = 0; bool isRead = false;
        cin.ignore();
        do
        {
            cout << "Nhap ten tep trong thu muc hien hanh cua chuong trinh hoac nhap duong dan tuyet doi den tep: ";
            getline(cin, fname);
            isRead = readDatabase(fname, data);
            if (!isRead) count++;
        } while (!isRead && count < 5);
        if (count == 5)
        {
            cout << "Khong the mo tep! Chuc ban may man lan sau" << endl;
            return -1;
        }
        // else   
        cout << "Du lieu da nhan thanh cong!" << endl;
        return 0;
    }
}

int menu2_Processing(HashTable& ht)
{
    int choice = 0;
    do
    {
        cout << "Chon chuc nang:" << endl;
        cout << "1. Tra cuu nghia cua mot tu." << endl;
        cout << "2. Them mot tu vao tu dien." << endl;
        cout << "3. Sua nghia cua mot tu." << endl;
        cout << "4. Xoa mot tu." << endl;
        cout << "5. Luu tu dien ra tep .txt." << endl;
        cout << "So khac. Khong lam gi nua: ";
        cin >> choice;
        if (choice != 1)
        {
            cout << "Chuc ban may man lan sau." << endl;
            return -1;
        }
        else
        {
            cin.ignore();
            switch(choice)
            {
                case 1: 
                    lookUpDictionary(ht);
                    break;
                case 2:
                    addAWord(ht);
                    break;
                case 3:
                    editAWord(ht);
                    break;
                case 4:
                    deleteAWord(ht);
                    break;
                case 5:
                    outputToFile(ht);
                    break;
                default:
                {
                    cout << "Chuc ban may man lan sau." << endl;
                    return -1;
                }
            }
        }
    } while (true);
    return 0;
}

int finalProgram()
{
    vector<Word> data;
    if (menu1_InputFromFile(data) == -1)
        return 0; // Input from file
    HashTable ht; initHashTable(ht, TABLE_SIZE);
    putToTable(data, ht);      // Put words to table
    cout << "Da dua du lieu nhan duoc vao co so du lieu." << endl;
    menu2_Processing(ht);
    return 0;
}