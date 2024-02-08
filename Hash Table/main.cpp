#include "Header.h"

int main()
{
    vector<Word> data;
    if (menu1_InputFromFile(data) == -1)
        return 0; // Input from file
    HashTable ht;
    putToTable(data, ht);      // Put words to table
    menu2_Processing(ht);
    return 0;
}