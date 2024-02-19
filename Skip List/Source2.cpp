#include "Header.h"

Word initWord(string word, string type, vector<string> meaning)
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

Node* createNode(Word data /* = initWord() */, int level /* = 0 */)
{
    Node* node = new Node;
    node->data = data;
    if (level)
        node->forward.resize(level, nullptr);
    return node;
}

void initList(SkipList& skl, int max_level /* = 6 */, float prob /* = 0.5 */)
{
    skl.header = createNode(initWord(/* empty word */), max_level + 1);
    skl.max_level = max_level;
    skl.currentLevel = 0;
    skl.prob = prob;
}

// Level of nodes in a skip list should be randomized in order to provide equality
int randomLevel(SkipList skl)
{
    int level = 0;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> distribution(0, 1); // random floats with uniform distribution
    while (distribution(gen) < skl.prob && level < skl.max_level) 
        level += 1;
    return level;
}

// void printList(SkipList skl)
// {
//     for (Node* i = skl.header->forward[0]; i != nullptr; i = i->forward[0])
//         printWord(i->data);
// }

void insertAnElement(SkipList& skl, Node*& node)
{
    vector<Node*> positionToInsert(skl.max_level + 1);
    Node* current = skl.header;

    // Find suitable position in the list to insert
    for (int i = skl.currentLevel; i >= 0; --i)
    {
        while (current->forward[i] && current->forward[i]->data < node->data)
            current = current->forward[i];
        positionToInsert[i] = current;
    }

    // After this step, positionToInsert[i] is the node right before the suitable position to insert in layer i.
    // Ought to find the node right after that suitable position, which is the next one of the current from the last step
    current = current->forward[0]; 
    //if (current && current->data == node->data) // detect duplicate
    //    return;
    int level = randomLevel(skl);

    // Handle case where we need to increase the number of levels => Layers above current level is null => 
    if (level > skl.currentLevel)
    {
        for (int i = skl.currentLevel + 1; i <= level; ++i)
            positionToInsert[i] = skl.header;
        skl.currentLevel = level;
    }

    // Insert a node after a node like regular singly linked list
    for (int i = 0; i <= level; ++i)
    {
        node->forward.push_back(positionToInsert[i]->forward[i]);
        positionToInsert[i]->forward[i] = node;
    }
    //node->forward.resize(level + 1, nullptr);

    //// Insert a node after a node like regular singly linked list
    //for (int i = 0; i <= level; ++i)
    //{
    //    node->forward[i] = positionToInsert[i]->forward[i];
    //    positionToInsert[i]->forward[i] = node;
    //}
}

void putToList(SkipList& skl, vector<Word> data)
{
    for (Word i : data)
    {
        Node* node = createNode(i);
        insertAnElement(skl, node);
    }
}

vector<Node**> searchForElement(SkipList skl, Word key)
{
    //vector<Node*> positionToSearch(skl.max_level + 1);
    vector<Node**> searchResult;
    Node* current = skl.header;

    // Find suitable position in the list to search
    for (int i = skl.currentLevel; i >= 0; --i)
    {
        while (current->forward[i] && current->forward[i]->data < key)
            current = current->forward[i];
        //positionToSearch[i] = current;
    }

    current = current->forward[0]; 

    // Collect needed data
    while (!noCaseSensitiveCompare(current->data.word, key.word))
    {    
        Node** pointerToElement = new Node*;
        *pointerToElement = current;
        searchResult.push_back(pointerToElement);
        current = current->forward[0];
    }   
    return searchResult;
}

bool deleteAnElement(SkipList& skl, Node*& node)
{
    if (!skl.header->forward[0])
        return false;

    vector<Node*> positionToDelete(skl.max_level + 1);
    Node* current = skl.header;

    // Find suitable position in the list to delete
    for (int i = skl.currentLevel; i >= 0; --i)
    {
        while (current->forward[i] && current->forward[i]->data < node->data)// || (current->forward[i]->data >= node->data && !(current->forward[i] == node))))
            current = current->forward[i];
        positionToDelete[i] = current;
    }

    current = current->forward[0];
    if (current->data > node->data)
        return false;
    // positionToDelete[0] = current;
    while (current && current != node)
    {
        positionToDelete[0] = current;
        current = current->forward[0];
    }
    // After this step, positionToInsert[i] is the node right before the node to delete in layer i.
    // Ought to find the node to delete, which is the next one of the current from the last step
    // current = current->forward[0]; 
    if (!current || !(current == node))
        return false;

    // Delete a node afer a node like regular singly linked list    
    for (int i = 0; i <= skl.currentLevel; ++i)
    {
        if (!(positionToDelete[i]->forward[i] == current))
            break;
        positionToDelete[i]->forward[i] = current->forward[i];
    }
    delete current;
    current = nullptr;

    // Handle case when the deleted node has the highest level in the list and is the only node in that level (other than the header)
    while (skl.currentLevel > 0 && !skl.header->forward[skl.currentLevel])
        --skl.currentLevel;
    return true;
}

