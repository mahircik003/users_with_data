#include <string>
#include <vector>
#include <iostream>
#include <utility>
#include <algorithm>
using namespace std;


template <class T>
class BST
{
private:
    // Define TreeNode for the second phase (Binary Search Tree)
    struct TreeNode
    {
        std::string key;
        T data;
        TreeNode *left;
        TreeNode *right;
        TreeNode(const std::string &k, const T &value) : key(k), data(value), left(NULL), right(NULL) {}
    };

    TreeNode *root;

public: 
    BST();
    ~BST();
    TreeNode *getRoot() { return root; }
    bool isEmpty() { return root == NULL; }

    BST &insert(const std::string key, const T &value);
    bool search(std::string value) const;
    void remove(std::string value);
    BST<T> *merge(BST<T> *bst);
    BST<T> *intersection(BST<T> *bst);
    std::vector<TreeNode> tree2vector(TreeNode *root);
    void print();

private:
    void print(TreeNode *node, std::string indent, bool last, bool isLeftChild); 

    void recursive_deletion(TreeNode *node)
    {
        if (!node)
            return;
        if (node)
        {
            recursive_deletion(node->left);
            recursive_deletion(node->right);
            delete node;
        }
    }

    void recursive_remove(TreeNode *&r, std::string key)
    {
        if (!r)
            return;

        else if (r->key > key)
            recursive_remove(r->left, key);
        else if (r->key < key)
            recursive_remove(r->right, key);
        else if (!r->right && !r->left)
        {
            delete r;
            r = NULL;
        }
        else if (!r->left)
        {
            TreeNode *temp = r;
            r = r->right;
            delete temp;
        }
        else if (!r->right)
        {
            TreeNode *temp = r;
            r = r->left;
            delete temp;
        }
        else
        {
            TreeNode *temp = getMin(r->right);
            r->key = temp->key;
            recursive_remove(r->right, temp->key);
        }
    }

    TreeNode *getMin(TreeNode *r)
    {
        while (r->left)
            r = r->left;

        return r;
    }

    void tree2vector(TreeNode *node, vector<TreeNode> &result)
    {

        if (!node)
            return;

        tree2vector(node->left, result);
        result.push_back(*node);
        tree2vector(node->right, result);
    }

    vector<BST<T>::TreeNode> bubble_sort(vector<BST<T>::TreeNode> vec)
    {
        int i, j;
        bool swapped;
        for (i = 0; i < vec.size() - 1; i++)
        {
            swapped = false;
            for (j = 0; j < vec.size() - i - 1; j++)
            {
                if (vec[j].key > vec[j + 1].key)
                {
                    swap(vec[j], vec[j + 1]);
                    swapped = true;
                }
            }

            // If no two elements were swapped
            // by inner loop, then break
            if (swapped == false)
                break;
        }

        return vec;
    }
};

// Constructor
template <class T>
BST<T>::BST() : root(NULL) {}

// Destructor
template <class T>
BST<T>::~BST()
{
    recursive_deletion(root);
}

// Insert function for BST.
template <class T>
BST<T> &BST<T>::insert(const string key, const T &value)
{
    TreeNode *node = new TreeNode(key, value);

    if (isEmpty())
    {
        root = node;
        return *this;
    }

    TreeNode *current = root;

    while (current)
    {
        if (key > current->key)
        {
            if (!current->right)
            {
                current->right = node;
                return *this;
            }

            current = current->right;
        }
        else
        {
            if (!current->left)
            {
                current->left = node;
                return *this;
            }
            current = current->left;
        }
    }
}

// Search function for BST.
template <class T>
bool BST<T>::search(std::string value) const
{
    TreeNode *current = root;

    while (current)
    {

        if (current->key == value)
            return true;

        if (value > current->key)
            current = current->right;

        else if (value < current->key)
            current = current->left;
    }
    return false;
}

// Remove a node from BST for given key. If key not found, do not change anything.
template <class T>
void BST<T>::remove(std::string key)
{
    recursive_remove(root, key);
}

// A helper function for converting a BST into vector.
template <class T>
vector<typename BST<T>::TreeNode> BST<T>::tree2vector(TreeNode *root)
{
    vector<TreeNode> result;

    tree2vector(root, result);

    return result;
}

// Merge two BST's and return merged BST.
template <class T>
BST<T> *BST<T>::merge(BST<T> *bst)
{
    vector<BST<T>::TreeNode> vec1 = tree2vector(root);
    vector<BST<T>::TreeNode> vec2 = tree2vector(bst->getRoot());

    vector<BST<T>::TreeNode> fresh;
    bool inArray = false;

    for (int i = 0; i < vec1.size(); i++)
    {
        fresh.push_back(vec1[i]);
    }

    for (int i = 0; i < vec2.size(); i++)
    {

        inArray = false;

        for (int k = 0; k < fresh.size(); k++)
        {

            if (vec2[i].key == fresh[k].key)
            {
                inArray = true;
                break;
            }
        }

        if (!inArray)
            fresh.push_back(vec2[i]);
    }

    fresh = bubble_sort(fresh);

    BST<T> *new_bst = new BST<T>;

    for (int i = 0; i < fresh.size(); i++)
    {
        new_bst->insert(fresh[i].key, fresh[i].data);
    }

    return new_bst;
}

// Intersect two BST's and return new BST.
template <class T>
BST<T> *BST<T>::intersection(BST<T> *bst)
{

    vector<BST<T>::TreeNode> vec1 = tree2vector(root);
    vector<BST<T>::TreeNode> vec2 = tree2vector(bst->getRoot());

    vector<BST<T>::TreeNode> fresh;

    for (int i = 0; i < vec2.size(); i++)
    {

        for (int k = 0; k < vec1.size(); k++)
        {

            if (vec2[i].key == vec1[k].key)
                fresh.push_back(vec2[i]);
        }
    }

    fresh = bubble_sort(fresh);

    BST<T> *new_bst = new BST<T>;

    for (int i = 0; i < fresh.size(); i++)
    {
        new_bst->insert(fresh[i].key, fresh[i].data);
    }

    return new_bst;
}


template <class T>
void BST<T>::print()
{
    print(root, "", true, false);
}

template <class T>
void BST<T>::print(TreeNode *node, string indent, bool last, bool isLeftChild)
{
    if (node != NULL)
    {
        cout << node->key << endl;
        print(node->left, indent, false, true);
        print(node->right, indent, true, false);
    }
}
