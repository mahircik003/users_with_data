#include <iostream>
#include <string>
#include <vector>
#include <utility>
using namespace std;

template <class T>
class Trie
{
private:
    struct TrieNode
    {
        static const int ALPHABET_SIZE = 128;
        TrieNode *children[ALPHABET_SIZE];
        char keyChar;
        bool isEndOfKey;
        T *data;

        TrieNode(char c) : keyChar(c), isEndOfKey(false), data(NULL)
        {

            for (int i = 0; i < ALPHABET_SIZE; i++)
            {
                children[i] = NULL;
            }
        }
    };

    TrieNode *root;

public: 
    Trie();
    ~Trie();

    Trie &insert(const string &username);
    void remove(std::string username);
    T *search(std::string username);
    void findStartingWith(std::string prefix, std::vector<T *> &results);
    void wildcardSearch(const std::string &wildcardKey, std::vector<T *> &results);
    void print();

private:                                                         
    void print(const std::string &primaryKey);                     
    void printTrie(TrieNode *node, const std::string &currentKey); 

    void deleteTrieNode(struct TrieNode *node)
    {

        if (!node)
            return;

        for (int i = 0; i < node->ALPHABET_SIZE; i++)
        {

            deleteTrieNode(node->children[i]);
        }

        delete node->data;
        delete node;
    }

    void recursive_vectorization(struct Trie::TrieNode *node, std::string name, vector<T *> &results)
    {
        if (!node)
            return;

        name += node->keyChar;

        if (node->isEndOfKey)
        {
            results.push_back(node->data);
        }

        for (int i = 0; i < node->ALPHABET_SIZE; i++)
        {
            recursive_vectorization(node->children[i], name, results);
        }
    }

    void function(TrieNode *node, std::string wildcardKey, int index, vector<T *> &results)
    {

        if (!node || index >= wildcardKey.size())
            return;

        if (node == root)
        {
            for (int i = 0; i < node->ALPHABET_SIZE; i++)
            {
                function(node->children[i], wildcardKey, index, results);
            }
        }

        if (wildcardKey[index] == '*')
        {

            while (wildcardKey[index] == '*')
                index++; // passing through all * because *==**...**

            index--; // because we will stop at a card coming after *****

            index++;

            for (int i = 0; i < node->ALPHABET_SIZE; i++)
            {
                function(node, wildcardKey, index, results);
                function(node->children[i], wildcardKey, index, results);

                index--;

                function(node->children[i], wildcardKey, index, results);

                index++;

                if (node->isEndOfKey && index == wildcardKey.size() && !(find(results.begin(), results.end(), node->data) != results.end()))
                {
                    results.push_back(node->data); // check if it is alredy in vector first
                }
            }
        }

        if (wildcardKey[index] == '?')
        {
            index++;

            for (int i = 0; i < node->ALPHABET_SIZE; i++)
            {
                function(node->children[i], wildcardKey, index, results);
            }

            if (node->isEndOfKey && !(find(results.begin(), results.end(), node->data) != results.end()))
            {
                results.push_back(node->data); // check if it is alredy in vector first
            }
        }

        else if (wildcardKey[index] == node->keyChar)
        {
            index++;

            for (int i = 0; i < node->ALPHABET_SIZE; i++)
            {
                function(node->children[i], wildcardKey, index, results);
            }

            if (node->isEndOfKey && index == wildcardKey.size() && !(find(results.begin(), results.end(), node->data) != results.end()))
            {
                results.push_back(node->data); // check if already in vector first
            }
        }

        else
            return;
    }
};

template <class T>
Trie<T>::Trie() : root(new TrieNode('\0')) {}

template <class T>
Trie<T>::~Trie()
{
    deleteTrieNode(root);
}

template <class T>
Trie<T> &Trie<T>::insert(const string &key)
{
    TrieNode *current = root;

    int index = 0;
    while (index < key.size())
    { // when the index is equal to the size, we stop the search

        if (current->children[key[index]] && key[index] == current->children[key[index]]->keyChar);
         // checking the letters that are present
        else if (!current->children[key[index]])
            current->children[key[index]] = new TrieNode(key[index]);

        if (index == key.size() - 1)
        {
            if (!current->children[key[index]]->isEndOfKey)
            {
                current->children[key[index]]->isEndOfKey = true;
                if (!current->children[key[index]]->data)
                    current->children[key[index]]->data = new T(key);
            }
        }

        current = current->children[key[index]];
        index++;
    }
    return *this;
}

template <class T>
T *Trie<T>::search(std::string username)
{
    TrieNode *current = root;

    int index = 0;

    while (index < username.size())
    { // when the index is equal to the size, we stop the search

        if (current->children[username[index]] && username[index] == current->children[username[index]]->keyChar)
        {

            if (index == username.size() - 1 && current->children[username[index]]->isEndOfKey == true)
            {

                return current->children[username[index]]->data;
            }

            current = current->children[username[index]];
            index++;
        }
        else
            return NULL;
    }
    return NULL;

}

template <class T>
void Trie<T>::remove(std::string username)
{

    TrieNode *current = root;

    int index = 0;

    while (index < username.size())
    { // when the index is equal to the size, we stop the search

        if (current->children[username[index]] && username[index] == current->children[username[index]]->keyChar)
        {

            if (index == username.size() - 1 && current->children[username[index]]->isEndOfKey)
            {

                current->children[username[index]]->isEndOfKey = false;
            }

            current = current->children[username[index]];
            index++;
        }
        else
            return;
    }

    return;
}

template <class T>
void Trie<T>::findStartingWith(string prefix, vector<T *> &results)
{

    TrieNode *current = root;

    int index = 0;

    while (index < prefix.size())
    { // when the index is equal to the size, we stop the search

        if (current->children[prefix[index]] && prefix[index] == current->children[prefix[index]]->keyChar)
        {

            current = current->children[prefix[index]];
            index++;
        }
        else
            return;
    }
    // by the end of loop we either reach the node with last letter or return.

    for (int i = 0; i < current->ALPHABET_SIZE; i++)
    {
        if (current->children[i])
            recursive_vectorization(current->children[i], prefix, results);
    }
    //    results vector should be filled.
}

template <class T>
void Trie<T>::wildcardSearch(const std::string &wildcardKey, std::vector<T *> &results)
{
    function(root, wildcardKey, 0, results);
}


template <class T>
void Trie<T>::print()
{
    print("");
}

template <class T>
void Trie<T>::print(const std::string &primaryKey)
{
    if (primaryKey.empty())
    {
        
        printTrie(root, "");
    }
    else
    {
        
        TrieNode *primaryNode = root;
        for (int i = 0; i < primaryKey.length(); i++)
        {

            int index = primaryKey[i];
            if (!primaryNode->children[index])
            {
                std::cout << "{}" << std::endl; // Primary key not found, nothing to print.
                return;
            }
            primaryNode = primaryNode->children[index];
        }
    }
}

template <class T>
void Trie<T>::printTrie(TrieNode *node, const std::string &currentKey)
{
    if (!node)
    {
        return;
    }
    if (node->isEndOfKey)
    {
        std::cout << currentKey << std::endl;
        std::cout << "Watched Movies:" << std::endl;
        if (node->data)
        {
            node->data->printMovies();
        }
    }
    for (int i = 0; i < 128; ++i)
    {
        if (node->children[i])
        {
            printTrie(node->children[i], currentKey + static_cast<char>(i));
        }
    }
}