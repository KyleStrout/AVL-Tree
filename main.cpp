using namespace std;
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <sstream>
#include <algorithm>



// Node object that holds name and Gator ID as well as height in the tree

struct Node
{
	string NAME;
    int height;
	long ID;
	Node* left = NULL;
	Node* right = NULL;
    bool check = false;
    Node(string _NAME, int _height, long _ID, Node* _left, Node* _right)
    {
        NAME = _NAME;
        height = _height;
        ID = _ID;
        left = _left;
        right = _right;
    }
    Node(string _NAME, int _height, long _ID)
    {
        NAME = _NAME;
        height = _height;
        ID = _ID;
        left = NULL;
        right = NULL;
    }
    Node(string _NAME, long _ID)
    {
        NAME = _NAME;
        ID = _ID;
        height = 0;
        left = NULL;
        right = NULL;
    }



};

// Helper function for getHeight, takes two numbers and returns the maximum
int max(int x, int y)
{
    if (x > y)
        return x;
    else
        return y;
}

// function to get the height of a node in the tree (concept from tree slides)
int getHeight(Node* root)
{
    if (root == NULL)
        return 0;
    if (root->left == NULL && root->right == NULL && root != NULL)
    {
        return 1;
    }
    else
    {
        return 1 + max(getHeight(root->left), getHeight(root->right));
    }
}

// gets the balance factor of the children of a root
int getBalance(Node* root)
{
    if (root == NULL)
        return 0;
    else
    {
        return getHeight(root->left) - getHeight(root->right);
    }
}

/* Set's the height of the given node by calling getHeight on the same node */
void setHeight(Node* node)
{
    if (node == NULL)
    {
        return;
    }
    else
    {
        node->height = getHeight(node);
    }
}

//rotate function from stepik
Node* rotateLeft(Node* node)
{
    Node* grandchild = node->right->left;
    Node* newParent = node->right;
    newParent->left = node;
    node->right = grandchild;
    setHeight(node);
    setHeight(grandchild);
    return newParent;
}

// rotate function from stepik
Node* rotateRight(Node* node)
{
    Node* grandchild = node->left->right;
    Node* newParent = node->left;
    newParent->right = node;
    node->left = grandchild;
    setHeight(node);
    setHeight(grandchild);
    return newParent;
}

// rotate function from stepik
Node* rotateLeftRight(Node* node)
{
    node->left = rotateLeft(node->left);
    return rotateRight(node);
}

// rotate function from stepik
Node* rotateRightLeft(Node* node)
{
    node->right = rotateRight(node->right);
    return rotateLeft(node);
}

/* AVL Tree Data Structure Class */
class AVLTree
{
    Node* root = NULL;
public:
    Node* insert(Node* root, string _NAME, long _ID);
    Node* findNew(Node* node);
    Node* getRoot();
    bool setNull(Node* node, long _ID);
    bool remove(Node* root, long _ID);
    void searchID(Node* _root, long _ID);
    void searchNAMEHelper(Node* _root, string _name, vector<Node*>& nodes);
    void searchNAME(Node* _root, string _name);
    void preOrderHelper(Node* _root, vector<Node*>& nodes);
    void preOrder(Node* _root);
    void postOrderHelper(Node* _root, vector<Node*>& nodes);
    void postOrder(Node* _root);
    void inOrderHelper(Node* _root, vector<Node*>& nodes);
    void inOrder(Node* _root);
    void levelCount(Node* root);
    void removeNHelper(Node* _root, vector<Node*>& nodes);
    void removeN(Node* _root, int n);

};

/* 1. If root is null, root = root
   2. Recursively finds the place for it, then inserts the node, sets the height, and checks the balance
   3. If unbalanced rotates the tree recursively 
   4. Returns a node, idk why*/
// side note: This was a great feeling when it finally worked! :D
Node* AVLTree::insert(Node* _root, string _NAME, long _ID)
{
    if (_root == NULL)
    {
        
        Node* temp = new Node(_NAME, _ID);
        if (root == NULL)
        {
            this->root = temp;
            cout << "successful" << endl;
            return root;
        }
        cout << "successful" << endl;
        return temp;
    }
    else if (_root->ID == _ID)
    {
        cout << "unsuccessful" << endl;
        return _root;
    }
    else if (_ID < _root->ID)
    {
        _root->left = insert(_root->left, _NAME, _ID);
    }
    else if (_ID > _root->ID)
    {
        _root->right = insert(_root->right, _NAME, _ID);
    }

    setHeight(_root);

    if (getBalance(_root) < -1)
    {
        if (_ID > _root->right->ID)
        {
            return rotateLeft(_root);
            
        }
        else
        {
            _root->right = rotateRight(_root->right);

            _root = rotateLeft(_root);
            root = _root;
            return _root;
        }
            
    }
    if (getBalance(_root) > 1)
    {
        if (_ID < _root->left->ID)
        {
            return rotateRight(_root);

        }
        else
        {
            _root->left = rotateLeft(_root->left);
            _root = rotateRight(_root);
            root = _root;
            return _root;
            //return rotateLeftRight(_root);
        }
    }
    root = _root;
    return _root;
}


// Returns the root of the tree structure
Node* AVLTree::getRoot()
{
    return root;
}


/* Finds the rightmose node in the right subtree of the left child */
Node* AVLTree::findNew(Node* node)
{
    if (node->right == NULL)
    { 
        return node;
    }
    while (true)
    {
        if (node->right == NULL)
        {
            Node* temp = node; // gets the last node in the right subtree
            //node->right = NULL; // sets its right = NULL
            return temp;
        }
        else
        {
            node = node->right;
        }
    }
    return node;
}

// I don't think I ever used this function so just ignore it.
bool AVLTree::setNull(Node* node, long _ID)
{
    Node * temp = findNew(node);
    temp = NULL;
    return temp;
}


/* 1. First checks through the tree to see if the node exists.
*  2. If it does, then checks if it's the root.
*  3. If not the root uses a queue to delete the node and update the tree.
*  4. Prints successful and returns true if deleted.
*  5. Prints unsuccessful and returns false if doesn't exist.
*/
bool AVLTree::remove(Node* _root, long _ID)
{
    if (_root == NULL)
    {
        cout << "unsuccessful" << endl;
        return false;
    }
    else if (_ID < _root->ID)
    {
        return remove(_root->left, _ID);
    }
    else if (_ID > _root->ID)
    {
        return remove(_root->right, _ID);
    }
   else
    {
        queue<Node*> q;
        q.push(root);
        // if the root needs to be deleted
        if (_ID == root->ID)
        {
            if (root->left == NULL && root->right == NULL) // if root has no children, the root is removed
            {
                root = NULL;
                cout << "successful" << endl;
                return true;
            }
            else if (root->left != NULL && root->right == NULL) // if the root has one other node on the left
            {
                root = root->left;
                cout << "successful" << endl;
                return true;
            }
            else if (root->left == NULL && root->right != NULL)
            {
                root = root->right;
                cout << "successful" << endl;
                return true;
            }
            else
            {
                Node* rightMost = findNew(root->left);
                if (root->left->right == NULL)
                {
                    
                    Node* rootRight = root->right;
                    Node* newRoot = root;
                    root = rightMost;
                    root->right = rootRight;
                }
                else
                {
                    Node* temp2 = root->left;
                    Node* temp3 = root;
                    Node* parent = root->left;
                    root = rightMost;

                    while (parent->right != NULL)
                    {

                        if (parent->right->ID == rightMost->ID)
                        {
                            parent->right = rightMost->left;
                            break;
                        }
                        parent = parent->right;
                    }

                    rightMost->left = temp3->left;
                    rightMost->right = temp3->right;
                    
                }
                cout << "successful" << endl;
                return true;
            }
        }
        while (!q.empty())
        {
            Node* temp = q.front();
            if (temp->right != NULL && temp->right->ID == _ID) 
            {
                if (temp->right->right != NULL && temp->right->left != NULL) // if the node has two children
                {
                    Node* rightMost = findNew(temp->right->left); // right most, 
                    if (rightMost->right == NULL && temp->right->left->right == NULL)
                    {
                        //right left
                        Node* temp2 = temp->right;
                        temp->right = rightMost;
                        rightMost->right = temp2->right;
                    }
                    else
                    {
                        Node* temp2 = temp->right;
                        Node* parent = temp->right->left;
                        temp->right = rightMost;

                        while (parent->right != NULL)
                        {

                            if (parent->right->ID == rightMost->ID)
                            {
                                parent->right = rightMost->left;
                                break;
                            }
                            parent = parent->right;
                        }

                        rightMost->left = temp2->left;
                        rightMost->right = temp2->right;
                    }
                    //rightMost->left = temp->left->left;
                    //temp->right = rightMost;
                    cout << "successful" << endl;
                    return true;
                }
                else if (temp->right->right != NULL) // if the node has one child (right)
                {
                    temp->right = temp->right->right;
                    cout << "successful" << endl;
                    return true;
                }
                else if (temp->right->left != NULL) // if the node has one child (left)
                {
                    temp->right = temp->right->left;
                    cout << "successful" << endl;
                    return true;
                }
                else if(temp->right->right == NULL && temp->right->left == NULL) //if the node has no children
                {
                    temp->right = NULL;
                    cout << "successful" << endl;
                    return true;
                }
            }
            else if (temp->left != NULL && temp->left->ID == _ID)
            {
                if (temp->left->left != NULL && temp->left->right != NULL)
                {
                    Node* rightMost = findNew(temp->left->left);
                    if (rightMost->right == NULL && temp->left->left->right == NULL)
                    {
                        //right left
                        Node* temp2 = temp->left;
                        temp->left = rightMost;
                        rightMost->right = temp2->right;
                    }
                    else
                    {
                        Node* temp2 = temp->left;
                        Node* parent = temp->left->left;
                        temp->left = rightMost;
 
                        while (parent->right != NULL)
                        {
                            
                            if (parent->right->ID == rightMost->ID)
                            {
                                parent->right = rightMost->left;
                                break;
                            }
                            parent = parent->right;
                        }

                        rightMost->left = temp2->left;
                        rightMost->right = temp2->right;
                    }
                    cout << "successful" << endl;
                    return true;
                    
                }
                else if (temp->left->left != NULL && temp->left->right == NULL)
                {
                    temp->left = temp->left->left;
                    cout << "successful" << endl;
                    return true;
                }
                else if (temp->left->right != NULL && temp->left->left == NULL)
                {
                    temp->left = temp->left->right;
                    cout << "successful" << endl;
                    return true;
                }
                else if(temp->left->left == NULL && temp->left->right == NULL)
                {
                    temp->left = NULL;
                    cout << "successful" << endl;
                    return true;
                }
            }
            q.pop();
            if (_ID < temp->ID)
            {
                q.push(temp->left);
            }
            if (_ID > temp->ID)
            {
                q.push(temp->right);
            }
        }
    }
    
    

}

// from tree slides on BST traversals, inOrder traversal 
void inOrder(Node* root)
{
    if (root == NULL)
    {
        cout << "";
    }
    else
    {
        inOrder(root->left);
        cout << root->ID << " ";
        inOrder(root->right);
    }
}

/*Searches through the tree for the given ID, returns the NAME of node that has it
if the node is not in the tree, prints unsuccessful. */
void AVLTree::searchID(Node* _root, long _ID)
{
    if (_root == NULL)
    {
        cout << "unsuccessful" << endl;
        return;
    }
    else if (_ID < _root->ID)
    {
        return searchID(_root->left, _ID);
    }
    else if (_ID > _root->ID)
    {
        return searchID(_root->right, _ID);
    }
    else
    {
        cout << _root->NAME << endl;
    }
}

// searches through tree in preOrder, finds names equal to target and pushes them to a vector
void AVLTree::searchNAMEHelper(Node* _root, string _name, vector<Node*>& nodes)
{
    if (_root == NULL)
    {
        return;
    }
    if (_root->NAME == _name)
    {
        nodes.push_back(_root);
    }
    searchNAMEHelper(_root->left, _name, nodes);
    searchNAMEHelper(_root->right, _name, nodes);
}

// calls searchNameHelper and updates vector then traverses through vector and prints elements ID's
void AVLTree::searchNAME(Node* root, string _name)
{
    vector<Node*> nodes;
    searchNAMEHelper(root, _name, nodes);
    if (nodes.empty())
    {
        cout << "unsuccessful" << endl;
        return;
    }
    else
    {
        for (int i = 0; i < nodes.size(); i++)
        {
            cout << nodes.at(i)->ID << endl;
        }
    }
    nodes.clear();
}

// updates a vector with nodes in preOrder order
void AVLTree::preOrderHelper(Node* _root, vector<Node*>& nodes)
{
    if (_root == NULL)
    {
        return;
    }
    nodes.push_back(_root);
    preOrderHelper(_root->left, nodes);
    preOrderHelper(_root->right, nodes);
}

// calls preOrderHelper and then traverses through nodes and prints names.
void AVLTree::preOrder(Node* _root)
{
    vector<Node*> nodes;
    preOrderHelper(_root, nodes);
    for (int i = 0; i < nodes.size(); i++)
    {
        if (i == nodes.size() - 1)
        {
            cout << nodes.at(i)->NAME << endl;
        }
        else
        {
            cout << nodes.at(i)->NAME << ", ";
        }
    }
}

// updates a vector with nodes in postOrder order 
void AVLTree::postOrderHelper(Node* _root, vector<Node*>& nodes)
{
    if (_root == NULL)
        return;
    postOrderHelper(_root->left, nodes);
    postOrderHelper(_root->right, nodes);
    nodes.push_back(_root);
}

// calls postOrderHelper and then traverses through nodes and prints names.
void AVLTree::postOrder(Node* _root)
{
    vector<Node*> nodes;
    postOrderHelper(_root, nodes);
    for (int i = 0; i < nodes.size(); i++)
    {
        if (i == nodes.size() - 1)
        {
            cout << nodes.at(i)->NAME << endl;
        }
        else
        {
            cout << nodes.at(i)->NAME << ", ";
        }
    }
}

// updates a vector of nodes inOrder order
void AVLTree::inOrderHelper(Node* _root, vector<Node*>& nodes)
{
    if (_root == NULL)
        return;
    inOrderHelper(_root->left, nodes);
    nodes.push_back(_root);
    inOrderHelper(_root->right, nodes);
}

// calls inOrder Helper then traverses through vector and prints names inOrder
void AVLTree::inOrder(Node* _root)
{
    vector<Node*> nodes;
    inOrderHelper(_root, nodes);
    for (int i = 0; i < nodes.size(); i++)
    {
        if (i == nodes.size() - 1)
        {
            cout << nodes.at(i)->NAME << endl;
        }
        else
        {
            cout << nodes.at(i)->NAME << ", ";
        }
    }
}
// takes in the root of the tree and prints out the amount of levels in the tree.
void AVLTree::levelCount(Node* _root)
{
    if (root == NULL)
    {
        cout << "0" << endl;
    }
    else
    {
        // Citation: Module 3 Problem Solving Video For levelOrder with a queue.
        queue<Node*> q;
        int level = 0;

        if (_root != NULL)
        {
            q.push(_root);
        }

        while (!q.empty())
        {


            //cout << q.front()->ID << " ";
            int temp = q.size();
            for (int i = 0; i < temp; i++)
            {
                if (q.front()->left != NULL)
                {
                    q.push(q.front()->left);
                }
                if (q.front()->right != NULL)
                {
                    q.push(q.front()->right);
                }
                q.pop();
            }
            level++;

        }
        cout << level << endl;
    }
    
}

// pushes nodes inOrder to a vector to be used in removeN
void AVLTree::removeNHelper(Node* _root, vector<Node*>& nodes)
{
    if (_root == NULL)
    {
        return;
    }
    removeNHelper(_root->left, nodes);
    nodes.push_back(_root);
    removeNHelper(_root->right, nodes);
}

// calls NHelper then traverses through nodes inOrder and removes the Nth one
void AVLTree::removeN(Node* _root, int n)
{
    vector<Node*> nodes;
    removeNHelper(_root, nodes);

    if (nodes.empty())
    {
        cout << "unsuccessful" << endl;
        return;
    }
    else
    {
        for (int i = 0; i < nodes.size(); i++)
        {
            if (i == n)
            {
                remove(root, nodes.at(i)->ID);
                return;
            }
        }
    }

}




int main()
{
    
    int numCommands;
    cin >> numCommands;
    AVLTree t;
    string input;
    string one;
    string two;
    string three;
    for (int i = 0; i <= numCommands; i++)
    {

        getline(cin, input);
        int oneEnd = input.find(' ');
        one = input.substr(0, input.find(' '));
        //cout << two << endl;
        //cout << three << endl;
        if (one == "insert")
        {
            
            two = input.substr(input.find('\"'), input.rfind('\"') - input.find(' '));
            three = input.substr(input.rfind('\"') + 2);
            two.erase(remove(two.begin(), two.end(), '\"'), two.end());
            bool check = true;
            for (int i = 0; i < two.size(); i++)
            {
                if (isdigit(two[i]))
                {
                    check = false;
                }

            }
            if (!check)
            {
                cout << "unsuccessful" << endl;
            }
            else if (two.empty() || !two.find_first_of("1234567890!@#$%^&*()-_{}[];:><,.?/|'\""))
            {
                cout << "unsuccessful" << endl;
            }
            else if (three.empty() || (three.front() == 48) &&  (three.size() && three.size() != 8) 
                || !three.find_first_of("!@#$%^&*()-_{}[];:><,.?/|'\"") 
                || (three.front() != 48 && three.size() != 8))
            {
                cout << "unsuccessful" << endl;
            }
            else
            {
                long ID = stol(three);
                t.insert(t.getRoot(), two, ID);
            }

            
        }
        else if (one == "remove")
        {
            two = input.substr(input.find(' ') + 1);
            long ID = stol(two);
            t.remove(t.getRoot(), ID);
        }
        else if (one == "search")
        {
            two = input.substr(input.find(' ') + 1);
            if (two.front() == '\"')
            {
                two.erase(remove(two.begin(), two.end(), '\"'), two.end());
                t.searchNAME(t.getRoot(), two);
            }
            else
            {
                long ID = stol(two);
                t.searchID(t.getRoot(), ID);
            }
        }
        else if (one == "printLevelCount")
        {
            t.levelCount(t.getRoot());
        }
        else if (one == "printInorder")
        {
            t.inOrder(t.getRoot());
        }
        else if (one == "printPreorder")
        {
            t.preOrder(t.getRoot());
        }
        else if (one == "printPostorder")
        {
            t.postOrder(t.getRoot());
        }
        else if (one == "removeInorder")
        {
            two = input.substr(input.find(' ') + 1);
            int n = stoi(two);
            t.removeN(t.getRoot(), n);
        }

        
    }




	return 0;
}