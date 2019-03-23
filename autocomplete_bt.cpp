#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Node
{
  public:
    Node()
    {
        mContent = ' ';
        mMarker = false;
        right = nullptr;
        left = nullptr;
    }
    ~Node();
    char content()
    {
        return mContent;
    }
    void setContent(char c)
    {
        mContent = c;
    }
    bool wordMarker()
    {
        return mMarker;
    }
    void setWordMarker()
    {
        mMarker = true;
    }
    void appendRight(Node *node)
    {
        right = node;
    }
    void appendLeft(Node *node)
    {
        left = node;
    }
    Node *getRight()
    {
        return right;
    }
    Node *getLeft()
    {
        return left;
    }

  private:
    char mContent;
    bool mMarker;
    Node *right, *left;
};

Node *getSameLevel(Node *node, char c)
{
    if (node == nullptr)
        return nullptr;
    else if (node->content() == c)
        return node;
    else
        getSameLevel(node->getRight(), c);
}

class BinaryTree
{
  public:
    BinaryTree();
    void addWord(string);
    Node *searchWord(string, bool);
    bool autoComplete(string, vector<string> &);
    void parseTree(Node *, string &, vector<string> &, bool &);

  private:
    Node *root;
};

BinaryTree::BinaryTree()
{
    root = new Node();
}
void BinaryTree::addWord(string s)
{
    Node *current = root;
    int len = s.length();
    if (s.length() == 0)
        return;
    for (int i = 0; i < len; ++i)
    {
        Node *next = getSameLevel(current, s[i]);
        if (next != nullptr)
        {
            if (i < len - 1)
            {
                if (next->getLeft() == nullptr)
                {
                    Node *nnode = new Node();
                    nnode->setContent(s[i + 1]);
                    if (i == len - 1)
                        nnode->setWordMarker();
                    next->appendLeft(nnode);
                    //i++;
                }
                current = next->getLeft();
            }
        }
        else
        {
            Node *tmp = current;
            Node *nnode = new Node();
            nnode->setContent(s[i]);
            while (tmp->getRight() != nullptr)
                tmp = tmp->getRight();
            tmp->appendRight(nnode);
            current = tmp->getRight();
            i--;
        }
        if (i == len - 1)
            next->setWordMarker();
    }
}
Node *BinaryTree::searchWord(string s, bool word = true)
{
    Node *current = root->getRight(), *prev = nullptr;
    int len = s.length();
    for (int i = 0; i < len; ++i)
    {
        Node *next = getSameLevel(current, s[i]);
        if (next == nullptr)
            return nullptr;
        prev = next;
        current = next->getLeft();
    }
    if (word)
    {
        if (current->wordMarker() == true)
            return prev;
        else
            return nullptr;
    }
    else
    {
        //if(prev)
        return prev;
        //else
        //    return nullptr;
    }
}
bool BinaryTree::autoComplete(string s, vector<string> &res)
{
    Node *current = root;
    Node *tmp = searchWord(s, false);
    if (!tmp->getLeft())
        return false;

    current = tmp->getLeft();
    s += current->content();
    bool loop = true;
    parseTree(current, s, res, loop);
    return true;
}
void BinaryTree::parseTree(Node *current, string &s, vector<string> &res, bool &loop)
{
    if (loop)
    {
        if (current->wordMarker())
        {
            res.push_back(s);
            if (res.size() > 15)
                loop = false;
        }
        if (current->getLeft())
        {
            Node *left = current->getLeft();
            s += left->content();
            //cout << "*" << left->content();
            parseTree(left, s, res, loop);
        }
        else
        {
            s.erase(s.size() - 1);
            //return;
        }
        if (current->getRight())
        {
            Node *right = current->getRight();
            s += right->content();
            //cout << "#" << right->content();
            parseTree(right, s, res, loop);
        }
        else
        {
            s.erase(s.size() - 1);
            return;
        }
    }
}
bool loadDictionary(BinaryTree* bt, string filename)
{
    ifstream words;
    words.open(filename.c_str());
    if (!words.is_open()) {
        cout << "Dictionary file Not Open" << endl;
        return false;
    }
    while (!words.eof()) {
        char s[100];
        words >> s;
        bt->addWord(s);
    }
    return true;
}
int main()
{
    BinaryTree *bt = new BinaryTree();
    int mode;
    cout << "Loading dictionary" << endl;
    loadDictionary(bt, "words.txt");
    while (1) {
        cout << endl
             << endl;
        cout << "Interactive mode,press " << endl;
        cout << "1: Auto Complete Feature" << endl;
        cout << "2: Quit" << endl
             << endl;
        cin >> mode;
        switch (mode) {
        case 1: //Auto complete
        {

            string s;
            char addNew;
            cin >> s;
            transform(s.begin(), s.end(), s.begin(), ::tolower);
            vector<string> autoCompleteList;
            bt->autoComplete(s, autoCompleteList);
            if (autoCompleteList.size() == 0) {
                cout << "No suggestions" << endl;
                cout<<"Want to add this to the dictionary?(y/n): ";
                cin>>addNew;
                if(addNew == 'y'|| addNew == 'Y')
                {
                    bt->addWord(s);
                    cout<<"Word "<<s<<" added to the dictionary."<<endl;
                }
                else
                    cout<<"Word "<<s<<" was not added to the dictionary"<<endl;
                
            }
            else {
                cout << "Autocomplete reply :" << endl;
                for (int i = 0; i < autoCompleteList.size(); i++) {
                    cout << "\t \t " << autoCompleteList[i] << endl;
                }
            }
        }
            continue;
        case 2:
            delete bt;
            return 0;
        default:
            continue;
        }
    }
}
