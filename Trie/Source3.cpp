#include "Header.h"

vector<char> getAlphabetFromFile(string fname)
{
    vector<char> aphb;
    ifstream fp;
    fp.open(fname, ios::in);
    if (!fp.is_open())
        return aphb;
    string a;
    while (getline(fp, a))
    {
        char charr = 0;
        if (a != "space")
            charr = a[0];
        else
            charr = ' ';
        aphb.push_back(charr);
    }    
    return aphb;

}

void printWord(Word w)
{
    cout << w.word << "  ";
    vector<string>::iterator it = w.meaning.begin();
    // int i = 0;
    if (w.type.empty())
        while (it != w.meaning.end() && *it != "")
            {
                cout << *it << " ";
                it++;
            }
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

bool isIn(vector<int> arr, int val)
{
    if (arr.empty())
        return false;
    for (int i : arr)
        if (i == val)
            return true;
    return false;
}

vector<Word> convertTrieIntoArr(Node* root)
{
    vector<Word> mainData;
    vector<char> chars = getAlphabetFromFile(ALPHABET_FILENAME);
    if (!root)
        return mainData;
    if (root->isCompleteWordInDict)
        mainData.insert(mainData.end(), root->data.begin(), root->data.end()); // <=> mainData.push_back(root->data)
    for (int i : chars)
        if (root->children.find(i) == root->children.end())
            continue;
        else
        {
            vector<Word> subdata = convertTrieIntoArr(root->children[i]);
            mainData.insert(mainData.end(), subdata.begin(), subdata.end());
        }
    return mainData;
}

void addAWord(Node*& root)
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
            cout << "Nhap nghia. Nghia nen di kem voi cac chu thich ve tu. Ket thuc nghia nen co dau cham: "; 
            getline(cin, meaning);
            string meaning__ = to_string(i + 1) + " " + meaning;
            meanings.push_back(meaning__);
        }
        cout << "Tu moi co con tu loai khac nua khong?\n0. Khong\t\t\t\t\t\tSo khac. Co: "; cin >> flag; cin.ignore();
        if (flag)
            meanings.push_back("");
    } while (flag);
    Word w = initWord(word, type, meanings);
    w.type = types;
    insertAnElement(root, w);
    cout << "Da them thanh cong." << endl;
}

void editAWord(Node*& root)
{
    string word, type, meaning;
    vector<string> types, meanings;
    cout << "Nhap tu moi can sua: "; getline(cin, word);
    Node** searchResult = searchForElement(root, word);
    if (!searchResult)
    {
        cout << "Khong ton tai tu. Chuc ban may man lan sau." << endl;
        return;
    }
    int index = 0, choice = 0;
    //cout << "Nhap so thu tu tuong ung voi tu ban muon sua:" << endl;
    for (Word i : (*searchResult)->data)
    {
        cout << index << ". ";
        printWord(i);
        cout << endl;
        ++index;
    }
    cout << "So khac. Huy, khong sua nua: "; cin >> choice;
    cin.ignore();
    if (choice >= (*searchResult)->data.size() || choice < 0)
    {
        cout << "Chuc ban may man lan sau." << endl;    
        return;
    }
    //else
        //w = searchResult[choice];
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
                (*searchResult)->data[choice].meaning.clear();
                (*searchResult)->data[choice].type.clear();
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
                    cout << "Nhap nghia. Nghia nen di kem voi cac chu thich ve tu. Ket thuc nghia nen co dau cham: "; 
                    getline(cin, meaning);
                    string meaning__ = to_string(i + 1) + " " + meaning;
                    meanings.push_back(meaning__);
                }
                cout << "Tu moi co con tu loai khac nua khong?\n0. Khong\t\t\t\t\t\tSo khac. Co: "; cin >> flag;
                cin.ignore();
                if (flag)
                    meanings.push_back("");
            } while (flag);
            word = (*searchResult)->data[choice].word;
            (*searchResult)->data[choice] = initWord(word, type, meanings);
            (*searchResult)->data[choice].type = types;
        }
        break;
        case 2:
        {
            int _choice = 1; index = 1;
            cout << "Chon nghia cua tu de sua:" << endl;
            for (string i : (*searchResult)->data[choice].meaning)
                if (i != "")
                {
                    cout << index << i << endl;
                    ++index;
                }
                else
                    ++index;
            cout << "So khac. Huy, khong sua nua: ";
            cin >> _choice; cin.ignore();
            if (_choice - 1 >= (*searchResult)->data[choice].meaning.size() - (*searchResult)->data[choice].type.size() + 1 || _choice - 1 < 0)
            {
                cout << "Chuc ban may man lan sau." << endl;
                return;
            }
            // else
            cout << "Nhap nghia moi. Khong nhap gi va nhan Enter de xoa nghia da chon: "; getline(cin, meaning);
            (*searchResult)->data[choice].meaning[_choice - 1] = meaning;
        }
        break;
        default:
        {
            cout << "Chuc ban may man lan sau." << endl;
            return;
        }
    }

    cout <<  "Da sua thanh cong! Tu moi duoc sua nghia la:" << endl;
    printWord((*searchResult)->data[choice]);
    cout << endl;
}

void lookUpDictionary(Node* root)
{
    string word; 
    cout << "Nhap tu can tra: "; getline(cin, word);
    Node** searchResult = searchForElement(root, word);
    if (!searchResult)
    {
        cout << "Khong tim thay tu. Chuc ban may man lan sau." << endl;
        return;
    }
    // else
    cout << "Day la cac tu toi tim duoc:" << endl;
    for (Word i : (*searchResult)->data)
    {
        printWord(i);
        cout << endl;
    }
    return;
}

void deleteAWord(Node*& root)
{
    string word; Word w; 
    cout << "Nhap tu can tra: "; getline(cin, word);
    Node** searchResult = searchForElement(root, word);
    if (!searchResult)
    {
        cout << "Khong tim thay tu. Chuc ban may man lan sau." << endl;
        return;
    }
    // else
    cout << "Nhap so thu tu tuong ung voi tu ban muon xoa:" << endl;
    int index = 0, choice = 0;
    for (Word i : (*searchResult)->data)
    {
        cout << index << ". ";
        printWord(i);
        cout << endl;
        ++index;
    }
    cout << "So khac. Huy, khong xoa nua: ";
    cin >> choice;
    if (choice >= (*searchResult)->data.size() || choice < 0)
    {
        cout << "Chuc ban may man lan sau." << endl;
        return;
    }
    //else
    if ((*searchResult)->data.size() == 1)
        if (!deleteAnElement(root, (*searchResult)->data[choice].word))
            cout << "Xoa that bai. Chuc ban may man lan sau." << endl;
        else
            cout << "Da xoa thanh cong." << endl;
    else
    {
        (*searchResult)->data.erase((*searchResult)->data.begin() + choice);
        cout << "Da xoa thanh cong." << endl;
    }
    return;
}

int outputToFile(Node* root)
{
    vector<Word> data = convertTrieIntoArr(root);
    string fname; int count = 0; bool isWritten = false;
    do
    {
        cout << "Nhap ten tep trong thu muc hien hanh cua chuong trinh hoac nhap duong dan tuyet doi den tep: ";
        getline(cin, fname);
        isWritten = writeDatabase(fname, data);
        if (!isWritten) count++;
    } while (!isWritten && count < 5);
    if (!isWritten)
    {
        cout << "Khong the mo tep! Chuc ban may man lan sau" << endl;
        return -1;
    }
    // else   
    cout << "Ghi du lieu ra tep thanh cong!" << endl;
    return 0;
}