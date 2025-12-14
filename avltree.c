#include <stdio.h>
#include <stdlib.h>
#define max(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
#define height(n) (n?n->height:0)
#define balance_factor(n) (height(n->left)-height(n->right))

typedef struct avl_tree_node {
    int data;
    size_t height;
    struct avl_tree_node *left;
    struct avl_tree_node *right;
    struct avl_tree_node *parent;  
} avl_tree_node;

typedef struct avl_tree {
    avl_tree_node *root;
    size_t size;
} avl_tree;

avl_tree *avl_tree_create() {
    avl_tree* tree = (avl_tree*)malloc(sizeof(avl_tree));
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

avl_tree_node *avl_tree_create_node(int data, avl_tree_node *left, avl_tree_node *right) {
    avl_tree_node* node = (avl_tree_node*)malloc(sizeof(avl_tree_node));
    node->data = data;
    node->left = left;
    node->right = right;
    return node;
}

avl_tree_node *avl_tree_rotate_right(avl_tree_node *y) {
    avl_tree_node *x = y->left;
    avl_tree_node *t2 = x->right;

    x->right = y;
    y->left = t2;

    x->parent = y->parent;
    y->parent = x;
    if (t2 != NULL) 
        t2->parent = y;

    if (x->parent) {
        if (x->parent->left == y)
            x->parent->left = x;
        else
            x->parent->right = x;
    }

    y->height = 1+max(height(y->left), height(y->right));
    x->height = 1+max(height(x->left), height(x->right));

    return x;
}

avl_tree_node *avl_tree_rotate_left(avl_tree_node *x) {
    avl_tree_node *y = x->right;
    avl_tree_node *t2 = y->left;

    y->left = x;
    x->right = t2;

    y->parent = x->parent;
    x->parent = y;
    if (t2 != NULL) 
        t2->parent = x;

    if (y->parent) {
        if (y->parent->left == x)
            y->parent->left = y;
        else
            y->parent->right = y;
    }

    x->height = 1+max(height(x->left), height(x->right));
    y->height = 1+max(height(y->left), height(y->right));

    return y;
}

void avl_tree_insert(avl_tree *tree, int data) {
    avl_tree_node *node = avl_tree_create_node(data, NULL, NULL);
    if(tree->root == NULL) {
        tree->root = node;
        tree->size = 1;
        return;
    }

    avl_tree_node *cur = tree->root;
    while(1) {
        if(data < cur->data) {
            if(cur->left != NULL)
                cur = cur->left;
            else {
                cur->left = node;
                node->parent = cur;
                break;
            }
        } else {
            if(cur->right != NULL)
                cur = cur->right;
            else {
                cur->right = node;
                node->parent = cur;
                break;
            }
        }
    }
    ++tree->size;

    cur = node->parent;
    while (cur) {
        cur->height = 1+max(height(cur->left), height(cur->right));
        int balance = balance_factor(cur);

        if(balance > 1) {
            if(data < cur->left->data) {
                cur = avl_tree_rotate_right(cur);
            } else {
                cur->left = avl_tree_rotate_left(cur->left);
                cur = avl_tree_rotate_right(cur);
            }
        } else if(balance < -1) {
            if(data > cur->right->data) {
                cur = avl_tree_rotate_left(cur);
            } else {
                cur->right = avl_tree_rotate_right(cur->right);
                cur = avl_tree_rotate_left(cur);
            }
        }
        if(!cur->parent)
            tree->root = cur;
        cur = cur->parent;
    }
}

void avl_tree_delete(avl_tree *tree, int data) {
    avl_tree_node *parent = NULL;
    avl_tree_node *cur = tree->root;
    while(cur != NULL && cur->data != data) {
        parent = cur;
        if(data < cur->data)
            cur = cur->left;
        else
            cur = cur->right;
    }

    if(cur == NULL)
        return;

    if(cur->left != NULL && cur->right != NULL) {
        avl_tree_node *succ_parent = cur;
        avl_tree_node *succ = cur->right;
        while(succ->left) {
            succ_parent = succ;
            succ = succ->left;
        }
        cur->data = succ->data;
        parent = succ_parent;
        cur = succ;
    }

    avl_tree_node *child = cur->left?cur->left:cur->right;

    if(parent == NULL)
        tree->root = child;
    else if(parent->left == cur)
        parent->left = child;
    else
        parent->right = child;
    free(cur);
    --tree->size;
}

avl_tree_node *avl_tree_search(avl_tree *tree, int data) {
    if(tree->root == NULL)
        return NULL;
    avl_tree_node *cur = tree->root;
    while(cur != NULL) {
        if(cur->data == data)
            return cur;
        else if(cur->data > data)
            cur = cur->left;
        else if(cur->data < data)
            cur = cur->right;
    }
}

static void print_tree_node(avl_tree_node *node, const char *prefix, int isTail, const char *relation) {
    if (!node) 
        return;

    printf("%s%s%s[%d] %d\n", prefix, isTail ? " └── " : " ├── ", relation, node->height, node->data);

    const char *newPrefix = isTail ? "     " : " │   ";

    avl_tree_node *children[2];
    int count = 0;

    if (node->left) children[count++] = node->left;
    if (node->right) children[count++] = node->right;

    for (int i = 0; i < count - 1; i++) {
        const char *rel = (children[i] == node->left) ? "(L) " : "(R) ";
        char updatedPrefix[100];
        sprintf(updatedPrefix, "%s%s", prefix, newPrefix);
        print_tree_node(children[i], updatedPrefix, 0, rel);
    }

    if (count > 0) {
        avl_tree_node *last = children[count - 1];
        const char *rel = (last == node->left) ? "(L) " : "(R) ";
        char updatedPrefix[100];
        sprintf(updatedPrefix, "%s%s", prefix, newPrefix);
        print_tree_node(last, updatedPrefix, 1, rel);
    }
}

int main() {
    avl_tree *tree = avl_tree_create();
    avl_tree_insert(tree, 1);
    avl_tree_insert(tree, 2);
    avl_tree_insert(tree, 3);
    avl_tree_insert(tree, 4);
    avl_tree_insert(tree, 5);
    avl_tree_insert(tree, 9);
    avl_tree_insert(tree, 12);
    avl_tree_insert(tree, 7);
    print_tree_node(tree->root, "", 1, "(root) ");
    avl_tree_insert(tree, 16);
    avl_tree_delete(tree, 5);
    print_tree_node(tree->root, "", 1, "(root) ");
    print_tree_node(avl_tree_search(tree, 7), "", 1, "(7) ");
}