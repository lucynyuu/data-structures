#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum { RED, BLACK } rb_colour;

typedef struct redblack_tree_node {
    int data;
    rb_colour colour;
    struct redblack_tree_node *left;
    struct redblack_tree_node *right;
    struct redblack_tree_node *parent;  
} redblack_tree_node;

typedef struct redblack_tree {
    redblack_tree_node *root;
    size_t size;
} redblack_tree;

redblack_tree *redblack_tree_create() {
    redblack_tree* tree = (redblack_tree*)malloc(sizeof(redblack_tree));
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

redblack_tree_node *redblack_tree_create_node(int data, rb_colour colour, redblack_tree_node *left, redblack_tree_node *right, redblack_tree_node *parent) {
    redblack_tree_node* node = (redblack_tree_node*)malloc(sizeof(redblack_tree_node));
    node->data = data;
    node->colour = colour;
    node->left = left;
    node->right = right;
    node->parent = parent;
    return node;
}

void redblack_tree_rotate_right(redblack_tree *tree, redblack_tree_node *y) {
    redblack_tree_node *x = y->left;
    redblack_tree_node *t2 = x->right;

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
    } else {
        tree->root = x;
    }
}

void redblack_tree_rotate_left(redblack_tree *tree, redblack_tree_node *x) {
    redblack_tree_node *y = x->right;
    redblack_tree_node *t2 = y->left;

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
    } else {
        tree->root = y;
    }
}

void redblack_tree_insert_fixup(redblack_tree *tree, redblack_tree_node *node) {
    while (node != tree->root && node->parent->colour == RED) {
        redblack_tree_node *parent = node->parent;
        redblack_tree_node *grandparent = parent->parent;
        if (parent == grandparent->left) {
            redblack_tree_node *uncle = grandparent->right;
            if (uncle && uncle->colour == RED) {
                parent->colour = BLACK;
                uncle->colour = BLACK;
                grandparent->colour = RED;
                node = grandparent;
            } else {
                if (node == parent->right) {
                    node = parent;
                    redblack_tree_rotate_left(tree, node);
                }
                parent->colour = BLACK;
                grandparent->colour = RED;
                redblack_tree_rotate_right(tree, grandparent);
            }
        } else {
            redblack_tree_node *uncle = grandparent->left;
            if (uncle && uncle->colour == RED) {
                parent->colour = BLACK;
                uncle->colour = BLACK;
                grandparent->colour = RED;
                node = grandparent;
            } else {
                if (node == parent->left) {
                    node = parent;
                    redblack_tree_rotate_right(tree, node);
                }
                parent->colour = BLACK;
                grandparent->colour = RED;
                redblack_tree_rotate_left(tree, grandparent);
            }
        }
    }
    tree->root->colour = BLACK;
}

void redblack_tree_insert(redblack_tree *tree, int data) {
    redblack_tree_node *node = redblack_tree_create_node(data, RED, NULL, NULL, NULL);
    if(tree->root == NULL) {
        tree->root = node;
        node->colour = BLACK;
        tree->size = 1;
        return;
    }

    redblack_tree_node *cur = tree->root;
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
    redblack_tree_insert_fixup(tree, node);
}

void redblack_tree_delete(redblack_tree *tree, int data) {
    redblack_tree_node *parent = NULL;
    redblack_tree_node *cur = tree->root;
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
        redblack_tree_node *succ_parent = cur;
        redblack_tree_node *succ = cur->right;
        while(succ->left) {
            succ_parent = succ;
            succ = succ->left;
        }
        cur->data = succ->data;
        parent = succ_parent;
        cur = succ;
    }

    redblack_tree_node *child = cur->left?cur->left:cur->right;

    if(parent == NULL)
        tree->root = child;
    else if(parent->left == cur)
        parent->left = child;
    else
        parent->right = child;
    free(cur);
    --tree->size;
}

redblack_tree_node *redblack_tree_search(redblack_tree *tree, int data) {
    if(tree->root == NULL)
        return NULL;
    redblack_tree_node *cur = tree->root;
    while(cur != NULL) {
        if(cur->data == data)
            return cur;
        else if(cur->data > data)
            cur = cur->left;
        else if(cur->data < data)
            cur = cur->right;
    }
}

static void print_tree_node(redblack_tree_node *node, const char *prefix, int isTail, const char *relation) {
    if (!node) 
        return;

    printf("%s%s%s[%s] %d\n", prefix, isTail ? " └── " : " ├── ", relation, node->colour==RED?"red":"black", node->data);

    const char *newPrefix = isTail ? "     " : " │   ";

    redblack_tree_node *children[2];
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
        redblack_tree_node *last = children[count - 1];
        const char *rel = (last == node->left) ? "(L) " : "(R) ";
        char updatedPrefix[100];
        sprintf(updatedPrefix, "%s%s", prefix, newPrefix);
        print_tree_node(last, updatedPrefix, 1, rel);
    }
}

int main() {
    redblack_tree *tree = redblack_tree_create();
    redblack_tree_insert(tree, 1);
    redblack_tree_insert(tree, 2);
    redblack_tree_insert(tree, 3);
    redblack_tree_insert(tree, 4);
    redblack_tree_insert(tree, 5);
    redblack_tree_insert(tree, 9);
    redblack_tree_insert(tree, 12);
    redblack_tree_insert(tree, 7);
    print_tree_node(tree->root, "", 1, "(root) ");
    redblack_tree_insert(tree, 16);
    // redblack_tree_delete(tree, 5);
    print_tree_node(tree->root, "", 1, "(root) ");
    print_tree_node(redblack_tree_search(tree, 9), "", 1, "(7) ");
}