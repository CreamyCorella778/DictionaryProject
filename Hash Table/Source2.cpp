#include "Header.h"

void initWord(Word& w, string word, string type, vector<string> meaning)
{
    w.word = word;
    if (type != "")
        w.type.push_back(type);
    w.meaning = meaning;
}

bool isInitialized(Word w)
{
    return !(w.word.empty() && w.type.empty() &&  (w.meaning.size() == 0 || w.meaning[0].empty()));
}

void initHashTable(HashTable& ht, int n)
{
    ht.table.resize(n);
    ht.numberOfElements = 0;
    ht.sizeEachRow.resize(n);
}

// Knuth's hash func
int hashFunction1(int val,  int tableSize)
{
    const double A = (sqrt(5) - 1) / 2.0;
    double frac = val * A - floor(val * A);
    int hash = floor(tableSize * frac);
    return hash;
}

// quadratic probing
int hashFunction2(int val, int tableSize, int hashTime)
{
    return (hashFunction1(val, tableSize) + int(pow(hashTime,2))) % tableSize;
    /*
        unsigned long hash = 5381;
    for (char c : word) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c 
    return hash % table_size;
    */
}

int hashString(string str) {
    int hash = 0;
    int prime = 31;
    for (char c : str) {
        c = tolower(c);
        hash = hash * prime + c;
    }
    return hash;
}

//translate from Word to Integer
int encodeWord(Word w)
{
    int val = hashString(w.word);
    return val;
}

void printWord(Word w)
{
    cout << w.word << "  ";
    vector<string>::iterator it = w.meaning.begin();
    // int i = 0;
    for (string j : w.type)
    {
        cout << j << " ";
        while (it != w.meaning.end())
            if (*it == "")
            {
                ++it;
                break;
            }
            else
            {
                cout << *it << " ";
                ++it;
            }
        /*while (i < w.meaning.size() && w.meaning[i] != "")
            cout << w.meaning[i];*/
    }
}

string stringLWR(string a)
{
    string a_ = a;
    for (int i = 0; i < a.size(); ++i)
        a_[i] = tolower(a_[i]);
    return a_;
}

int noCaseSensitiveCompare(string a, string b)
{
    string a_ = stringLWR(a), b_ = stringLWR(b);
    return a_.compare(b_);
}

size_t noCaseSensitiveFind(string str, string strToFind)
{
    string str_ = stringLWR(str), strToFind_ = stringLWR(strToFind);
    return strToFind_.find(str_);
}

bool findSecondaryWordInMeaning(Word w, Word wordToFind)
{
    string meaningToFind = wordToFind.meaning[wordToFind.meaning.size() - 1];
    if (meaningToFind[0] != 127)
        return false;
    return noCaseSensitiveFind(w.word, meaningToFind) < meaningToFind.size();
}

// ony use this function right after having initialized the hash table:
// add data from a big array to the table
void putToTable(vector<Word> arr, HashTable& ht)
{
    for (int i = 0; i < arr.size(); ++i)
    {
        int hashVal = hashFunction1(encodeWord(arr[i]), ht.table.size());
        ht.sizeEachRow[hashVal] += 1;
    }
    for (int i = 0; i < ht.table.size(); ++i)
    {
        Word w; initWord(w);
        ht.table[i].resize(pow(ht.sizeEachRow[i], 2));
    }
    for (int i = 0; i < arr.size(); ++i)
    {
        int hashVal1 = hashFunction1(encodeWord(arr[i]), ht.table.size());
        int hashVal2 = hashFunction2(hashVal1, ht.table[hashVal1].size(), 2);
        ht.table[hashVal1][hashVal2].push_back(arr[i]);
    }
    ht.numberOfElements = arr.size();
}

bool lookUpTable(HashTable& ht, Word w, vector<list<Word>::iterator>& iters)
{
    int hashVal1 = hashFunction1(encodeWord(w), ht.table.size());
    int hashVal2 = hashFunction2(hashVal1, ht.table[hashVal1].size(), 2);
    list <Word>& l = ht.table[hashVal1][hashVal2];
    if (l.empty())
        return false;
    else 
    {
        bool isFound = false;
        for (list<Word>::iterator iter = l.begin(); iter != l.end(); ++iter)
            if (!noCaseSensitiveCompare(iter->word, w.word))
            {
                list<Word>::iterator it = iter;
                iters.push_back(it);
                isFound = true;
            }    
        if (!isFound)
            iters.push_back(l.end());
        return isFound;
    }
}

bool isIn(vector<int> arr, int val)
{
    if (arr.empty())
        return false;
    for (int i : arr)
        if (i == val)
            return true;
    return false;
}

bool isARowFull(HashTable ht, int rowIdx)
{
    return  ht.table[rowIdx].size() <= ht.sizeEachRow[rowIdx];
}

vector<Word> convertVectorOfListToVector(vector<list<Word>> vec)
{
    vector<Word> result;
    if (vec.empty())
        return result;
    // else
    int hashTime = 0;
    vector<int> hashHistory;
    for (int j = 0; j < vec.size(); ++j)
    {
        int hashVal = hashFunction2(j, vec.size(), hashTime);
        hashTime++;
        while (vec[hashVal].empty() && isIn(hashHistory, hashVal))
            hashVal = hashFunction2(hashVal, vec[hashVal].size(), hashTime++);
        hashHistory.push_back(hashVal);
        for (list<Word>::iterator it = vec[hashVal].begin(); it !=  vec[hashVal].end(); ++it)
        {
            Word w = *it;
            result.push_back(w);
        }  
    }
    return result;
}

void reHashing(vector<list<Word>>& row, int hashVal1, Word w)
{
    vector<Word> container = convertVectorOfListToVector(row);
    container.push_back(w);
    int oldSize = row.size();
    row.clear();
    row.resize(pow(oldSize, 2));
    for (int i = 0; i < container.size(); ++i)
    {
        int hashVal2 = hashFunction2(hashVal1, row.size(), 2);
        row[hashVal2].push_back(container[i]);
    }
    container.clear();
}

void insertAnElement(HashTable& ht, Word w)
{
    int hashVal1 = hashFunction1(encodeWord(w), ht.table.size());
    if (isARowFull(ht, hashVal1))
    {    
        reHashing(ht.table[hashVal1], hashVal1, w);
        ht.sizeEachRow[hashVal1]++;
        ht.numberOfElements++;
        return;
    }
    // else
    int hashVal2 = hashFunction2(hashVal1, ht.table[hashVal1].size(), 2);
    ht.table[hashVal1][hashVal2].push_back(w);
    ht.sizeEachRow[hashVal1]++;
    ht.numberOfElements++;
}

//void deleteAnElement(HashTable& ht, Word w)
//{
//    vector<Word> lookUpResult;
//    bool isExist = lookUpTable(ht, w, lookUpResult);
//    if (!isExist)
//        return;
//    else    
//    {
//        int hashVal1 = hashFunction1(encodeWord(w), ht.table.size());
//        int hashVal2 = hashFunction2(hashVal1, ht.table[hashVal1].size(), 2);
//        for (list<Word>::iterator it : iters)
//        {
//            ht.table[hashVal1][hashVal2].erase(it);
//            ht.numberOfElements -= 1;
//            ht.sizeEachRow[hashVal1] -= 1;
//        }
//    }
//}

vector<Word> convertTableIntoArr(HashTable ht)
{
    vector<Word> data;
    vector<int> hashHistory1, hashHistory2;
    for (int i = 0; i < ht.table.size(); ++i)
    {
        int hashVal1 = hashFunction1(i, ht.table.size());
        int hashTime = 1;
        while (isIn(hashHistory1, hashVal1))
            hashVal1 = hashFunction2(hashVal1, ht.table.size(), hashTime++);
        hashHistory1.push_back(hashVal1);
        for (int j = 0; j < ht.table[hashVal1].size(); ++j)
        {
            hashTime = 0;
            int hashVal2 = hashFunction2(j, ht.table[hashVal1].size(), hashTime);
            hashTime++;
            while (ht.table[hashVal1][hashVal2].empty() && isIn(hashHistory2, hashVal2))
                hashVal2 = hashFunction2(hashVal2, ht.table[hashVal1].size(), hashTime++);
            hashHistory2.push_back(hashVal2);
            for (list<Word>::iterator it = ht.table[hashVal1][hashVal2].begin(); it !=  ht.table[hashVal1][hashVal2].end(); ++it)
            {
                Word w = *it;
                data.push_back(w);
            }  
        }
        hashHistory2.clear();
    }
    return data;
}

void addAWord(HashTable& ht)
{
    string word, type, meaning;
    vector<string> types, meanings;
    cout << "Nhap tu moi can them: "; getline(cin, word);
    int flag = 1;
    do
    {
        cout << "Nhap tu loai cua tu. Tu loai co dang \"<tu_loai>.\": "; getline(cin, type);
        if (!isWordType(type))
        {
            cout << "Tu loai khong hop le. Hay nhap lai." << endl;
            continue;
        }
        types.push_back(type);
        int count = 0;
        cout << "Ung voi tu loai nay cua tu thi tu moi co bao nhieu nghia? "; cin >> count; cin.ignore();
        for (int i = 0; i < count; ++i)
        {
            cout << "Nhap nghia. Nghia nen di kem voi cac chu thich ve tu: "; getline(cin, meaning);
            meanings.push_back(meaning);
        }
        cout << "Tu moi co con tu loai khac nua khong?\n0. Khong\t\t\t\t\t\tSo khac. Co: "; cin >> flag; cin.ignore();
        if (flag)
            meanings.push_back("");
    } while (flag);
    Word w; initWord(w, word, type, meanings);
    w.type = types;
    insertAnElement(ht, w);
    cout << "Da them thanh cong." << endl;
}

void editAWord(HashTable& ht)
{
    string word, type, meaning;
    vector<string> types, meanings;
    cout << "Nhap tu moi can sua: "; getline(cin, word);
    Word w; initWord(w, word); vector<list<Word>::iterator> iters;
    bool found = lookUpTable(ht, w, iters);
    if (!found)
    {
        cout << "Khong ton tai tu. Chuc ban may man lan sau." << endl;
        return;
    }
    int index = 0, choice = 0;
    cout << "Nhap so thu tu tuong ung voi tu ban muon sua:" << endl;
    for (list<Word>::iterator it : iters)
    {
        cout << index << ". ";
        printWord(*it);
        cout << endl;
        ++index;
    }
    cout << "So khac. Huy, khong sua nua: "; cin >> choice;
    cin.ignore();
    if (choice >= iters.size() || choice < 0)
    {
        cout << "Chuc ban may man lan sau." << endl;    
        return;
    }
    //else
        //w = *iters[choice];
    int choice_ = 0;
    cout << "Ban muon sua nghia cua tu nhu the nao? Nhap so tuong ung voi lua chon:\n1. Tao mot nghia hoan toan moi cho tu\n2. Sua chi mot trong so cac nghia cua tu\nSo khac. Huy, khong sua nua: ";
    cin >> choice_;
    cin.ignore();
    switch (choice_)
    {
        case 1:
        {
            int flag = 1;
            do
            {
                iters[choice]->meaning.clear();
                iters[choice]->type.clear();
                cout << "Nhap tu loai cua tu. Tu loai co dang \"<tu_loai>.\": "; getline(cin, type);
                if (!isWordType(type))
                {
                    cout << "Tu loai khong hop le. Hay nhap lai." << endl;
                    continue;
                }
                types.push_back(type);
                int count = 0;
                cout << "Ung voi tu loai nay cua tu thi tu moi co bao nhieu nghia? "; cin >> count; cin.ignore();
                for (int i = 0; i < count; ++i)
                {
                    cout << "Nhap nghia. Nghia nen di kem voi cac chu thich ve tu: "; getline(cin, meaning);
                    meanings.push_back(meaning);
                }
                cout << "Tu moi co con tu loai khac nua khong?\n0. Khong\t\t\t\t\t\tSo khac. Co: "; cin >> flag;
                cin.ignore();
                if (flag)
                    meanings.push_back("");
            } while (flag);
            word = iters[choice]->word;
            initWord(*iters[choice], word, type, meanings);
            iters[choice]->type = types;
        }
        break;
        case 2:
        {
            int _choice = 1; index = 1;
            cout << "Chon nghia cua tu de sua:" << endl;
            for (string i : iters[choice]->meaning)
                if (i != "")
                {
                    cout << index << i << endl;
                    ++index;
                }
                else
                    ++index;
            cout << "So khac. Huy, khong sua nua: ";
            cin >> _choice; cin.ignore();
            if (_choice - 1 >= iters[choice]->meaning.size() - iters[choice]->type.size() + 1 || _choice - 1 < 0)
            {
                cout << "Chuc ban may man lan sau." << endl;
                return;
            }
            // else
            cout << "Nhap nghia moi. Khong nhap gi va nhan Enter de xoa nghia da chon: "; getline(cin, meaning);
            iters[choice]->meaning[_choice - 1] = meaning;
        }
        break;
        default:
        {
            cout << "Chuc ban may man lan sau." << endl;
            return;
        }
    }
    // int hashVal1 = hashFunction1(encodeWord(w), ht.table.size());
    // int hashVal2 = hashFunction2(hashVal1, ht.table[hashVal1].size(), 2);
    // list<Word>::iterator it = ht.table[hashVal1][hashVal2].begin();
    // while (it != ht.table[hashVal1][hashVal2].end() && it->word != iters[choice]->word)
    //     ++it;
    // *it = *iters[choice];
    cout <<  "Da sua thanh cong! Tu moi duoc sua nghia la:" << endl;
    printWord(*iters[choice]);
    cout << endl;
}

void lookUpDictionary(HashTable ht)
{
    string word; Word w; vector<list<Word>::iterator> iters;
    cout << "Nhap tu can tra: "; getline(cin, word);
    initWord(w, word); 
    bool isSuccess = lookUpTable(ht, w, iters);
    if (!isSuccess)
    {
        cout << "Khong tim thay tu. Chuc ban may man lan sau." << endl;
        return;
    }
    // else
    cout << "Day la cac tu toi tim duoc:" << endl;
    for (list<Word>::iterator it : iters)
    {
        printWord(*it);
        cout << endl;
    }
    return;
}

void deleteAWord(HashTable& ht)
{
    string word; Word w; vector<list<Word>::iterator> iters;
    cout << "Nhap tu can tra: "; getline(cin, word);
    initWord(w, word); 
    bool isExist = lookUpTable(ht, w, iters);
    if (!isExist)
    {
        cout << "Khong tim thay tu. Chuc ban may man lan sau." << endl;
        return;
    }
    // else
    cout << "Nhap so thu tu tuong ung voi tu ban muon xoa:" << endl;
    int index = 0, choice = 0;
    for (list<Word>::iterator it : iters)
    {
        cout << index << ". ";
        printWord(*it);
        cout << endl;
        ++index;
    }
    cout << "So khac. Huy, khong sua nua: ";
    cin >> choice;
    if (choice >= iters.size() || choice < 0)
    {
        cout << "Chuc ban may man lan sau." << endl;
        return;
    }
    //else
    int hashVal1 = hashFunction1(encodeWord(w), ht.table.size());
    int hashVal2 = hashFunction2(hashVal1, ht.table[hashVal1].size(), 2);
    // list<Word>::iterator it = ht.table[hashVal1][hashVal2].begin();
    // while (it != ht.table[hashVal1][hashVal2].end() && it->word != iters[choice]->word)
    //     ++it;
    ht.table[hashVal1][hashVal2].erase(iters[choice]);
    --ht.numberOfElements;
    --ht.sizeEachRow[hashVal1];
    cout << "Da xoa thanh cong." << endl;
}

int outputToFile(HashTable ht)
{
    vector<Word> data = convertTableIntoArr(ht);
    string fname; int count = 0; bool isWritten = false;
    do
    {
        cout << "Nhap ten tep trong thu muc hien hanh cua chuong trinh hoac nhap duong dan tuyet doi den tep: ";
        getline(cin, fname);
        isWritten = writeDatabase(fname, data);
        if (!isWritten) count++;
    } while (!isWritten && count < 5);
    if (count == 5)
    {
        cout << "Khong the mo tep! Chuc ban may man lan sau" << endl;
        return -1;
    }
    // else   
    cout << "Ghi du lieu ra tep thanh cong!" << endl;
    return 0;
}