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

    if(node->parent != NULL)
    {
        if(tree->lower_than(node->pair->key, node->parent->pair->key) == 1)
        {
            // Solo tiene hijo izquierdo
            if(node->left != NULL && node->right == NULL)
            {
                node->parent->left = node->left;
                node->left->parent = node->parent;
            }
            // Solo tiene hijo derecho
            else if(node->left == NULL && node->right != NULL )
            {
                node->parent->left = node->right;
                node->right->parent = node->parent;
            }
            // Tiene ambos hijos
            else if(node->left != NULL && node->right != NULL)
            {
                TreeNode * aux = node->left;

                /*
                while(aux->right != NULL)
                {                
                    aux = aux->right;
                }
                node->pair->key = aux->pair->key;
                node->pair->value = aux->pair->value;

                if(aux->left == NULL)
                {
                    aux->parent->right = NULL;
                }
                else
                {
                    aux->parent->right = aux->left;
                    aux->left->parent = aux->parent;
                }
                free(aux);
                return;
                */

                aux = minimum(node->right);
                node->pair->key = aux->pair->key;
                node->pair->value = aux->pair->value;
                removeNode(tree, aux);
            }
            // No tiene hijos
            else
            {
                node->parent->left = NULL;
            }
        }
        else
        {
            if(node->left != NULL && node->right == NULL)
            {
                node->parent->right = node->left;
                node->left->parent = node->parent;
            }
            else if(node->left == NULL && node->right != NULL )
            {
                node->parent->right = node->right;
                node->right->parent = node->parent;
            }
            /*else if(node->left != NULL && node->right != NULL)
            {
                TreeNode * aux = node->left;
                int i = 0;

                while(aux->right != NULL)
                {
                    i++;
                    aux = aux->right;
                }
                node->pair->key = aux->pair->key;
                node->pair->value = aux->pair->value;

                if(i == 0)
                {
                    if(aux->left == NULL)
                    {
                        aux->parent->left = NULL;
                    }
                    else
                    {
                        aux->parent->left = aux->left;
                        aux->left->parent = aux->parent;
                    }
                    free(aux);
                    return;
                }
                else
                {
                    if(aux->left == NULL)
                    {
                        aux->parent->right = NULL;
                    }
                    else
                    {
                        aux->parent->right = aux->left;
                        aux->left->parent = aux->parent;
                    }
                    free(aux);
                    return;
                }
            }*/
            else if(node->left != NULL && node->right != NULL)
            {
                TreeNode * aux = node->left;

                aux = minimum(node->right);
                node->pair->key = aux->pair->key;
                node->pair->value = aux->pair->value;
                removeNode(tree, aux);
            }
            else
            {
                node->parent->right = NULL;
            }
        }
    }
    else if(node->parent == NULL)
    {
        if(node->left != NULL)
        {
            node->left->parent = NULL;
            node->left = NULL;
        }
        if(node->right != NULL)
        {
            node->right->parent = NULL;
            node->right = NULL;
        }
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
    return NULL;
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
        TreeNode * aux = tree->current->parent;
        while(0)
        {
            printf("O");
            if(tree->current->parent != NULL)
            {
                if(tree->lower_than(aux->pair->key, tree->current->pair->key) == 0)
                {
                    break;
                }
                aux = aux->parent;
            }    
        }
        tree->current = aux;
    }
    return tree->current->pair;
}
