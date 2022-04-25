#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap*) malloc (sizeof(TreeMap));
    new->current = NULL;
    new->root = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {

    Pair * aux = searchTreeMap(tree, key);

    if(aux != NULL) return;

    TreeNode * newNode = createTreeNode(key, value);

    newNode->parent = tree->current;
    newNode->left = NULL;
    newNode->right = NULL;
    if(tree->lower_than(key, tree->current->pair->key) == 1)
    {
        tree->current->left = newNode;
    }
    else
    {
        tree->current->right = newNode;
    }
    tree->current = newNode;
}

TreeNode * minimum(TreeNode * x){

    while(x->left != NULL)
    {
        x = x->left;
    }

    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {

    TreeNode * son;
        if(tree->lower_than(node->pair->key, node->parent->pair->key) == 1)
        {
            son = node->parent->left;
        }
        else
        {
            son = node->parent->right;
        }
        printf("%p\n", son->pair->key);
        // Solo tiene hijo izquierdo
        if(node->left != NULL && node->right == NULL)
        {
            son = node->left;
            node->left->parent = node->parent;
        }
        // Solo tiene hijo derecho
        else if(node->left == NULL && node->right != NULL )
        {
            son = node->right;
            node->right->parent = node->parent;
        }
        // Tiene ambos hijos
        else if(node->left != NULL && node->right != NULL)
        {
            TreeNode * aux = minimum(node->right);
            node->pair->key = aux->pair->key;
            node->pair->value = aux->pair->value;
            removeNode(tree, aux);
        }
        // No tiene hijos
        else
        {
            son = NULL;
        }

    free(node);
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);
}

Pair * searchTreeMap(TreeMap * tree, void* key) {

    TreeNode * aux = tree->root;

    while(aux != NULL)
    {
        tree->current = aux;
        if(is_equal(tree, key, aux->pair->key) == 1)
        {
            return aux->pair;
        }
        else if(tree->lower_than(key, aux->pair->key) == 1)
        {
            aux = aux->left;
        }
        else if(tree->lower_than(key, aux->pair->key) == 0)
        {
            aux = aux->right;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    
    tree->current = tree->root;
    Pair * aux = tree->current->pair;
    if(tree->current->left != NULL || tree->current->right != NULL)
    {
        aux = searchTreeMap(tree, key);
        aux = tree->current->pair;
        return aux;
    }
    else
    {
        return NULL;
    }
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode * aux = minimum(tree->root);
    tree->current = aux;
    return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if(tree->current->right != NULL)
    {
        tree->current = minimum(tree->current->right);
    }
    else
    {
        if(tree->current->right != NULL)
    {
        tree->current = minimum(tree->current->right);
    }
    else
    {
        if(tree->current->parent != NULL)
        {
            TreeNode * aux = tree->current->parent;

            while(1)
            {
                if(aux != NULL)
                {
                    if(tree->lower_than(tree->current->pair->key, aux->pair->key) == 1)
                    {
                        break;
                    }
                    aux = aux->parent;
                }
                else
                {
                    return NULL;
                }
            }
            tree->current = aux;
            }
        else
        {
            return NULL;
        }
    }
    return tree->current->pair;
}
