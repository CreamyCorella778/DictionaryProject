#include "Header.h"

Word initWord(string word /* = "" */, string type /* = "" */, vector<string> meaning /* = {} */)
{
    Word w;
    w.word = word;
    if (type != "")
        w.type.push_back(type);
    w.meaning = meaning;
    return w;
}

bool isInitialized(Word w)
{
    return !(w.word.empty() && w.type.empty() &&  (w.meaning.size() == 0 || w.meaning[0].empty()));
}

Node* createNode(Word data /* = initWord() */, bool isValidWordInDict /* = false */)
{
    Node* node = new Node;
    if (isInitialized(data))
        node->data.push_back(data);
    node->isCompleteWordInDict = isValidWordInDict;
    return node;
}

void insertAnElement(Node*& root, Word w)
{
    if (!root)
        root = createNode(initWord());
    Node* current = root;
    for (int i = 0; i < w.word.size(); ++i)
    {
        if (current->children.find(tolower(w.word[i])) == current->children.end())
            current->children[tolower(w.word[i])] = createNode(initWord());
        current = current->children[tolower(w.word[i])];
    }
    current->isCompleteWordInDict = true;
    current->data.push_back(w);
    /*
    if (!current->isCompleteWordInDict)
    {
        current->isCompleteWordInDict = true;
        current->data = w;
    }
    */
}

Node* putToTrie(vector<Word> data)
{
    Node* root = nullptr;
    for (Word i : data)
        insertAnElement(root, i);
    return root;
}

Node** searchForElement(Node* root, string word)
{
    if (!root)
        return nullptr;
    Node* current = root;
    for (int i = 0; i < word.size(); ++i)
    {
        if (current->children.find(tolower(word[i])) == current->children.end())
            return nullptr;
        current = current->children[tolower(word[i])];
    }
    if (current->isCompleteWordInDict)
    {
        Node** result = new Node*;
        *result = current;
        return result;
    }    
    else
        return nullptr;
}

bool deleteAnElement(Node*& root, string word)
{
    if (!root)
        return false;
    Node* current = root;
    vector<Node*> traverseHistory; // traverseHistory.size() == word.size()
    for (int i = 0; i < word.size(); ++i)
    {
        if (current->children.find(tolower(word[i])) == current->children.end())
            return false;
        traverseHistory.push_back(current);
        current = current->children[tolower(word[i])];
    }
    if (!(current->isCompleteWordInDict))
        return false;
    if (current->children.empty())
    {
        delete current;
        current = nullptr;
        return true;
    }
    current->isCompleteWordInDict = false;
    for (int i = traverseHistory.size() /* = word.size() */- 2; i >= 0; --i)
    {
        current = traverseHistory[i];
        if (!current->isCompleteWordInDict)
        {
            delete current;
            current = nullptr;
        }
        else
            return true;
    }
}