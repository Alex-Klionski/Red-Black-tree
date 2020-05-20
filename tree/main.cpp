#include <bits/stdc++.h>
using namespace std;

enum Color {red, black};

class Node
{
public:
    int data;
    bool color;
    Node *left, *right, *parent;

    Node(int data)
    {
       this->data = data;
       left = right = parent = NULL;
       this->color = red;
    }

    // check if node is left child of parent
    bool isOnLeft() { return this == parent->left; }

    Node *sibling()
    {
        if(parent == NULL)
            return NULL;
        if (isOnLeft())
            return parent->right;

        return parent->left;
    }
    // moves node down and moves given node in its place
    void moveDown(Node *nParent)
    {
        if (parent != NULL)
        {
            if (isOnLeft())
                parent->left = nParent;
            else
                parent->right = nParent;
        }
        nParent->parent = parent;
        parent = nParent;
    }
    bool hasRedChild()
    {
        return (left != NULL and left->color == red) or
           (right != NULL and right->color == red);
    }
};

class RBTree
{
private:
    Node *root;

protected:
    void rotateLeft(Node *&);
    void rotateRight(Node *&);
    void deleteNode(Node *);
    void fixInsertViolation(Node *&);
    void fixDoubleBlack(Node *);
    Node* inSuccessorNode(Node* );
    Node* PredecessorNode(Node* );
    void findPreSuc(Node*, Node*&, Node*&, int);
public:
    RBTree() { root = NULL; }
    void insertValue(int);
    void deleteValue(int);
    void inorder();
    void levelOrder();
    void inOrderSuccessor(int);
    void Predecessor(int);
    void printSucc(int);
};

void inorderHelper(Node *root)
{
    if (root == NULL)
        return;

    inorderHelper(root->left);
    cout << root->data << "  ";
    inorderHelper(root->right);
}


Node* BSTInsert(Node* root, Node *pt)
{
    if (root == NULL)
       return pt;

    if (pt->data < root->data)
    {
        root->left  = BSTInsert(root->left, pt);
        root->left->parent = root;
    }
    else if (pt->data > root->data)
    {
        root->right = BSTInsert(root->right, pt);
        root->right->parent = root;
    }

    return root;
}

void levelOrderHelper(Node *root)
{
    if (root == NULL)
        return;

    std::queue<Node *> q;
    q.push(root);

    while (!q.empty())
    {
        Node *temp = q.front();
        cout << temp->data << "  ";
        q.pop();

        if (temp->left != NULL)
            q.push(temp->left);

        if (temp->right != NULL)
            q.push(temp->right);
    }
}

void RBTree::rotateLeft(Node *&pt)
{
    Node *pt_right = pt->right;

    pt->right = pt_right->left;

    if (pt->right != NULL)
        pt->right->parent = pt;

    pt_right->parent = pt->parent;

    if (pt->parent == NULL)
        root = pt_right;

    else if (pt == pt->parent->left)
        pt->parent->left = pt_right;

    else
        pt->parent->right = pt_right;

    pt_right->left = pt;
    pt->parent = pt_right;
}

void RBTree::rotateRight(Node *&pt)
{
    Node *pt_left = pt->left;

    pt->left = pt_left->right;

    if (pt->left != NULL)
        pt->left->parent = pt;

    pt_left->parent = pt->parent;

    if (pt->parent == NULL)
        root = pt_left;

    else if (pt == pt->parent->left)
        pt->parent->left = pt_left;

    else
        pt->parent->right = pt_left;

    pt_left->right = pt;
    pt->parent = pt_left;
}

void RBTree::fixInsertViolation(Node *&pt)
{
    Node *parent_pt = NULL;
    Node *grand_parent_pt = NULL;

    while ((pt != root) && (pt->color != black) &&
           (pt->parent->color == red))
    {

        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;

        /*  Case : A
            Parent of pt is left child of Grand-parent of pt */
        if (parent_pt == grand_parent_pt->left)
        {

            Node *uncle_pt = grand_parent_pt->right;

            /* Case : 1
               The uncle of pt is also red
               Only Recoloring required */
            if (uncle_pt != NULL && uncle_pt->color == red)
            {
                grand_parent_pt->color = red;
                parent_pt->color = black;
                uncle_pt->color = black;
                pt = grand_parent_pt;
            }

            else
            {
                /* Case : 2
                   pt is right child of its parent
                   Left-rotation required */
                if (pt == parent_pt->right)
                {
                    rotateLeft(parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                /* Case : 3
                   pt is left child of its parent
                   Right-rotation required */
                rotateRight(grand_parent_pt);
                swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }

        /* Case : B
           Parent of pt is right child of Grand-parent of pt */
        else
        {
            Node *uncle_pt = grand_parent_pt->left;

            /*  Case : 1
                The uncle of pt is also red
                Only Recoloring required */
            if ((uncle_pt != NULL) && (uncle_pt->color == red))
            {
                grand_parent_pt->color = red;
                parent_pt->color = black;
                uncle_pt->color = black;
                pt = grand_parent_pt;
            }
            else
            {
                /* Case : 2
                   pt is left child of its parent
                   Right-rotation required */
                if (pt == parent_pt->left)
                {
                    rotateRight(parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                /* Case : 3
                   pt is right child of its parent
                   Left-rotation required */
                rotateLeft(grand_parent_pt);
                swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }
    }

    root->color = black;
}
void RBTree::insertValue(int data)
{
    Node *pt = new Node(data);
    root = BSTInsert(root, pt);
    fixInsertViolation(pt);
}

/* DELETE*/

// find node that do not have a left child
// in the subtree of the given node
Node *successor(Node *x)
{
    Node *temp = x;

    while (temp->left != NULL)
        temp = temp->left;

    return temp;
}
// find node that replaces a deleted node in BST
Node *BSTreplace(Node *x) {
    // when node have 2 children
    if (x->left != NULL and x->right != NULL)
        return successor(x->right);

    // when leaf
    if (x->left == NULL and x->right == NULL)
        return NULL;

    // when single child
    if (x->left != NULL)
        return x->left;
    else
        return x->right;
}
void RBTree::deleteNode(Node* v)
{
    Node *u = BSTreplace(v);
    // True when u and v are both black
    bool uvBlack = ((u == NULL or u->color == black) and (v->color == black));
    Node *parent = v->parent;

    if (u == NULL) {
      // u is NULL therefore v is leaf
      if (v == root) {
        // v is root, making root null
        root = NULL;
      } else {
        if (uvBlack) {
          // u and v both black
          // v is leaf, fix double black at v
          fixDoubleBlack(v);
        } else {
          // u or v is red
          if (v->sibling() != NULL)
            // sibling is not null, make it red"
            v->sibling()->color = red;
        }

        // delete v from the tree
        if (v->isOnLeft()) {
          parent->left = NULL;
        } else {
          parent->right = NULL;
        }
      }
      delete v;
      return;
    }

    if (v->left == NULL or v->right == NULL) {
      // v has 1 child
      if (v == root) {
        // v is root, assign the value of u to v, and delete u
        v->data = u->data;
        v->left = v->right = NULL;
        delete u;
      } else {
        // Detach v from tree and move u up
        if (v->isOnLeft()) {
          parent->left = u;
        } else {
          parent->right = u;
        }
        delete v;
        u->parent = parent;
        if (uvBlack) {
          // u and v both black, fix double black at u
          fixDoubleBlack(u);
        } else {
          // u or v red, color u black
          u->color = black;
        }
      }
      return;
    }
    //v has 2 children, swap values with successor and recurse

    swap(u->data, v->data);
    deleteNode(u);
}

void RBTree::fixDoubleBlack(Node* x)
{
    if (x == root)
      // Reached root
      return;

    Node *sibling = x->sibling(), *parent = x->parent;
    if (sibling == NULL) {
      // No sibiling, double black pushed up
      fixDoubleBlack(parent);
    } else {
      if (sibling->color == red) {
        // Sibling red
        parent->color = red;
        sibling->color = black;
        if (sibling->isOnLeft()) {
          // left case
          rotateRight(parent);
        } else {
          // right case
          rotateLeft(parent);
        }
        fixDoubleBlack(x);
      } else {
        // Sibling black
        if (sibling->hasRedChild()) {
          // at least 1 red children
          if (sibling->left != NULL and sibling->left->color == red) {
            if (sibling->isOnLeft()) {
              // left left
              sibling->left->color = sibling->color;
              sibling->color = parent->color;
              rotateRight(parent);
            } else {
              // right left
              sibling->left->color = parent->color;
              rotateRight(sibling);
              rotateLeft(parent);
            }
          } else {
            if (sibling->isOnLeft()) {
              // left right
              sibling->right->color = parent->color;
              rotateLeft(sibling);
              rotateRight(parent);
            } else {
              // right right
              sibling->right->color = sibling->color;
              sibling->color = parent->color;
              rotateLeft(parent);
            }
          }
          parent->color = black;
        } else {
          // 2 black children
          sibling->color = red;
          if (parent->color == black)
            fixDoubleBlack(parent);
          else
            parent->color = black;
        }
      }
    }
}
Node* maxValueNode(Node *node) {

    Node *ptr = node;

    while (ptr->right != NULL)
        ptr = ptr->right;

    return ptr;
}


Node *search(Node* root, int n) {
    Node *temp = root;
    while (temp != NULL) {
      if (n < temp->data) {
        if (temp->left == NULL)
          break;
        else
          temp = temp->left;
      } else if (n == temp->data) {
        break;
      } else {
        if (temp->right == NULL)
          break;
        else
          temp = temp->right;
      }
    }

    return temp;
}

void RBTree::deleteValue(int n)
{
    if (root == NULL)
        return;
    Node *v = search(root, n), *u;

    if (v->data != n) {
      cout << "No node found to delete with value:" << n << endl;
      return;
    }

    deleteNode(v);
}

/* Successor*/
//Если нет правого сына, то поднимаешься до тех пор, пока я сам праый сын. Если правый сын есть, один раз вправо и до конца влево
Node* RBTree::inSuccessorNode(Node* n)
{
    Node *parent_pt = n->parent;

    if(n->right == NULL)
    {
        while(parent_pt != NULL && n == parent_pt->right)
        {
            n = parent_pt;
            parent_pt = n->parent;
        }
        return parent_pt;
    }
    else
    {
        return successor(n->right);
    }
}


void RBTree::inOrderSuccessor(int n)
{
    if(root == NULL)
        return;
    Node* v = search(root, n), *k;

    if (v->data != n) {
        cout << "\nNo node found to in-successor with value:" << n << endl;
        return;
    }

    k = inSuccessorNode(v);
    if(k != NULL)
        cout << "\nInsuccessorNode = " << k->data << endl;
}
Node* RBTree::PredecessorNode(Node* n)
{

    Node *parent_pt = n->parent;

    if(n->left == NULL)
    {
        while(parent_pt != NULL && n == parent_pt->left)
        {
            n = parent_pt;
            parent_pt = n->parent;
        }
        return parent_pt;
    }
    else
    {
        Node *temp = n->left;

        while (temp->right != NULL)
            temp = temp->right;

        return temp;
    }
}

void RBTree::Predecessor(int n)
{
    if(root == NULL)
        return;
    Node* v = search(root, n), *k;

    if (v->data != n) {
        cout << "\nNo node found to predecessor with value:" << n << endl;
        return;
    }

    k = PredecessorNode(v);
    if(k != NULL)
        cout << "\nPredecessor = " << k->data << endl;
}
//
void RBTree::findPreSuc(Node* root, Node*& pre, Node*& suc, int key)
{
    // Base case
    if (root == NULL)
    {
        suc = NULL; pre = NULL;
        return;
    }

    // If key is present at root
    if (root->data == key)
    {
        // the maximum value in left subtree is predecessor
        if (root->left != NULL)
        {
            Node* tmp = root->left;
            while (tmp->right)
                tmp = tmp->right;
            pre = tmp ;
        }

        // the minimum value in right subtree is successor
        if (root->right != NULL)
        {
            Node* tmp = root->right ;
            while (tmp->left)
                tmp = tmp->left ;
            suc = tmp ;
        }
        return ;
    }

    // If key is smaller than root's key, go to left subtree
    if (root->data > key)
    {
        suc = root ;
        findPreSuc(root->left, pre, suc, key) ;
    }
    else // go to right subtree
    {
        pre = root ;
        findPreSuc(root->right, pre, suc, key) ;
    }
}
void RBTree::printSucc(int key)
{
    Node *suc, *pre = NULL;

    Node* v = search(root, key);
    if (v->data != key) {
        cout << "\nNo node found to succ with value" << endl;
        return;
    }

    findPreSuc(root, pre, suc, key);
    if(pre != NULL)
        cout << "\nPreSucc = " << pre->data << endl;
    else
        cout << "\nNotFound";
    if(suc != NULL)
        cout << "InSucc = " << suc->data << endl;
    else
        cout << "\nNotFound";
}
//////////////////////////////////


// Function to do inorder and level order traversals
void RBTree::inorder()     {  inorderHelper(root);}
void RBTree::levelOrder()  {  levelOrderHelper(root); }


int main()
{
    RBTree tree;

    tree.insertValue(7);
    tree.insertValue(6);
    tree.insertValue(5);
    tree.insertValue(4);
    tree.insertValue(3);
    tree.insertValue(2);
    tree.insertValue(1);

    cout << "Inoder Traversal of Created Tree ";
    tree.inorder();
    cout << endl;

    cout << "Level Order Traversal of Created Tree ";
    tree.levelOrder();
    cout << endl;

/*
    cout<<endl<<"Deleting 3, 1, 4"<<endl;

    tree.deleteValue(3);
    tree.deleteValue(1);
    tree.deleteValue(4);
*/
    tree.inOrderSuccessor(4);
    tree.Predecessor(7);

    return 0;
}
