#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

// Интерфейс
class Container
{
public:
    // Виртуальные методы, будут реализованы далее
    virtual void insert(int value) = 0;
    virtual bool exists(int value) = 0;
    virtual void remove(int value) = 0;
    // Это потом заменим на перегруженный оператор <<
    virtual void print() = 0;

    // Виртуальный деструктор
    virtual ~Container();
};


Container::~Container() { }

// Реализация интерфейса
typedef struct tree_element {
	int value;			// Element value
	struct tree_element* left;	// Pointer to left subtree
	struct tree_element* right;
	struct tree_element* parent;
	int h;// Pointer to right subtree
} tree_element;
// TODO

class SimpleTree: public Container
{
public:
    SimpleTree();
    ~SimpleTree();
    void insert(int value);
    bool exists(int value);
    void remove(int value);
    void print();

protected:
    tree_element* create(int i);
    tree_element* find(int i);
    tree_element* root;
};

void delete_tree(tree_element* root)
{
    if (root->left != NULL)
        delete_tree(root->left);
    if (root->right != NULL)
        delete_tree (root->right);
    free(root);
}

SimpleTree::SimpleTree()
{
    this->root = NULL;
};

SimpleTree::~SimpleTree()
{
    delete_tree(this->root);
}

tree_element* SimpleTree::create(int value)
{
    tree_element* a = (tree_element*) malloc( sizeof(tree_element));
    a->value = value;
    a->left = NULL;
    a->right = NULL;
    a->parent = NULL;
    a->h = 1;
    return a;
}

tree_element* insert_into_tree(tree_element* root, tree_element* elem)
{
    int k = 0;

    if ((elem->value) < (root->value))
        {
        if ((root->left) != NULL)
        {
            root->left = insert_into_tree(root -> left, elem);
            if (k == 1)
                root->h += 1;
        }
        else
        {
           if (root->right == NULL)
                k = 1;
            root -> left = elem;
            elem -> parent = root;
            return root;
        }
        }
    else
        {
        if ((root->right) != NULL)
        {
            root->right = insert_into_tree(root->right, elem);
            if (k == 1)
                root->h += 1;
        }
        else
        {
            if (root->left == NULL)
                k = 1;
            root -> right = elem;
            elem -> parent = root;
            return root;
        }
        }
}

tree_element* balance (tree_element* root)
{
    int i;
    cout << "Start" << endl;
    if ((root->left == NULL)&&(root->right == NULL))
        return root;
    else if ((root->right != NULL)&&(root->left == NULL))
    {
        cout<< "First check" << endl;
        if((root->right->right != NULL)||(root->right->left != NULL))
        {
            cout << "First megacheck" << endl;
            tree_element* tree0 = root;
            root = root->right;
            tree0->h = 0;
            tree0->left = NULL;
            tree0->right = NULL;
            tree0->parent = NULL;
            root = insert_into_tree(root, tree0);
            return root;
        }
        else
            return root;
    }
    else if ((root->left != NULL)&&(root->right == NULL))
    {
        cout << "Second check" << endl;
        if((root->left->right != NULL)||(root->left->left != NULL))
        {
            tree_element* tree0 = root;
            root = root->left;
            tree0->h = 0;
            tree0->left = NULL;
            tree0->right = NULL;
            tree0->parent = NULL;
            insert_into_tree(root, tree0);
            return root;
        }
        else
            return root;
    }
    else
    {
    cout << "Third check" << endl;
    int rootRR;
    if (root->right->right != NULL)
        rootRR = root->right->right->h;
    else
        rootRR = 0;

    int rootRL;
    if (root->right->left != NULL)
        rootRL = root->right->left->h;
    else
        rootRL = 0;

    int rootLR;
    if (root->left->right != NULL)
        rootLR = root->left->right->h;
    else
        rootLR = 0;

    int rootLL;
    if (root->left->left != NULL)
        rootLL = root->left->left->h;
    else
        rootLL = 0;



    if ((abs(root->right->h - root->left->h) > 1)&&(abs(rootRR - rootRL) > 1))
    {
        cout << "right" << endl;
        root = balance(root->right);
    }
    else if ((abs(root->right->h - root->left->h) > 1)&&(abs(rootLR - rootLL) > 1))
    {
        root = balance(root->left);
        cout << "left" << endl;
    }
    else
    {
        i += 1;
        cout << "Balance_" << i<< endl;\
        tree_element* tree0 = root;
        tree_element* treeleft = tree0->left;
        tree_element* treeright = tree0->right;\

        if (root->right->h - root->left->h > 1)
        {
            treeright->parent = tree0->parent;
            tree0->parent = treeright;
            tree0->right = treeright -> left;
            treeright->left = tree0;
            root = treeright;
            return root;
        }
        else if (root->right->h-root->left->h < -1)
        {
            treeleft->parent = NULL;
            tree0->parent = treeleft;
            tree0->left = treeleft -> right;
            treeleft->right = tree0;
            root = treeleft;
            return root;
        }
        else
            return root;
    }
    }
}

int check (tree_element* root)
{
    if (root == NULL)
        return 1;
    else if ((root->left == NULL) && (root->right == NULL))
        return 1;
    else if ((root->left == NULL) && (root->right->h == 1))
        return 1;
    else if ((root->right == NULL) && (root->left->h == 1))
        return 1;
    else if ((root->left == NULL) | (root->right == NULL))
        return 0;
    else if ((root->left->h - root->right->h) * (root->left->h - root->right->h) > 1)
        return 0;
    else
    {

        return check(root->right) * check(root->left);
    }

}

void SimpleTree::insert(int elem)
{
    if (this->root == NULL)
        this->root = create(elem);
    else
    {
    tree_element* element = create(elem);
    this->root = insert_into_tree(this->root, element);
    int i = 0;
    while (check(this->root) == 0)
        this->root = balance(root);
    }
}

bool SimpleTree::exists(int value)
{
    tree_element* cur_element = this->find(value);
    return cur_element != NULL;
}

tree_element* SimpleTree::find(int value)
{
    tree_element* cur_element = this->root;
    while(cur_element != NULL) {
        if(cur_element->value == value)
            return cur_element;
        else if(cur_element->value < value)
            cur_element = cur_element->right;
        else
            cur_element = cur_element->left;
    }
    return NULL;
}

void SimpleTree::remove(int value)
{
    tree_element* elem = this->find(value);

    if(elem == NULL)
    {
        return;
    }
    else if(this->root == elem)
    {
        tree_element* new_root = root->right;
        tree_element* lefttree = root->left;
        free(root);
        this->root = new_root;
        tree_element* cur_elem = this -> root;
        while(cur_elem->left != NULL)
            cur_elem = cur_elem->left;
        cur_elem->left = lefttree;
    }
    else
    {
        tree_element* lefttree0 = elem->left;
        tree_element* cur_elem = this->root;

        while (cur_elem != elem)
        {
            if (cur_elem->value < elem->value )
                cur_elem = cur_elem->right;
            else
                cur_elem = cur_elem->left;
        }
        if (cur_elem->right->value > cur_elem->parent->value)
        {
            cur_elem->parent->right = cur_elem ->right;
            free(cur_elem);
        }
        else
        {
            cur_elem->parent->left = cur_elem ->right;
            free(cur_elem);
        }
        tree_element* element = this -> root;
        while(element->left != NULL)
            element = element->left;
        element->left = lefttree0;
    }
    while (check(this->root) == 0)
    {
        this->root = balance(root);
    }
}

void print_tree(tree_element* cur_element)
{
    if (cur_element->left != NULL)
        print_tree(cur_element->left);
    cout << cur_element->value << endl;
    if (cur_element->right != NULL)
        print_tree(cur_element->right);
}

void SimpleTree::print()
{
    print_tree(this->root);
}


int main()
{
    Container* c = new SimpleTree();
    for(int i = 1; i < 10; i++)
        c->insert(i*i);
    printf("List after creation:\n");
    c->print();

    if(c->exists(25))
        cout << "Search for value 25: found" << endl;

    if(!c->exists(111))
        cout << "Search for value 111: not found" << endl;
    c->remove(25);
    cout << "List after deletion of the element:" << endl;
    c->print();

    delete c;
    return 0;
}
