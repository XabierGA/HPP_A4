/**
 * Example of a main() function using a binary search tree (BST),
 * reading information about plants to add one by one from the
 * standard input.
 *
 * Course: High Performance Programming, Uppsala University
 *
 * Authors: Anastasia Kruchinina <anastasia.kruchinina@it.uu.se>
 *          Elias Rudberg <elias.rudberg@it.uu.se>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct tree_node
{
   int              ID;
   char             *name;
   struct tree_node *left;
   struct tree_node *right;
} node_t;


void print_bst(node_t *node)
{
   if(node == NULL) {printf("Tree is empty!\n"); return;}

   if (node != NULL) printf("%d %s: \t", node->ID, node->name);
   if (node->left != NULL) printf("L%d,", node->left->ID);
   if (node->right != NULL) printf("R%d", node->right->ID);
   printf("\n");

   if (node->left != NULL)
      print_bst(node->left);
   if (node->right != NULL)
      print_bst(node->right);
}

void delete_tree(node_t **node)
{
  //deletes node if it has no childs
  if ((*node)->left != NULL) {
    delete_tree(&(*node)->left);
  } else if ((*node)->right != NULL) {
    delete_tree(&(*node)->right);
  } else {
    /* both are null so we can free this node*/
    free((*node)->name);
    free((*node)->right);
    free((*node)->left);
    free((*node));
    (*node)=NULL;
  }
  if ((*node) != NULL)
    delete_tree(&(*node));
}
void insert(node_t **node, int ID, char *name)
{
//If passed node doesnt exists, ie is NULL then create the root
  if((*node) == NULL){
    *node=(node_t*)malloc(sizeof(node_t));
    (*node)->ID = ID;
    (*node)->name = strdup(name);
    (*node)->right = NULL;
    (*node)->left = NULL;
  }else{
    if((*node)->ID > ID){
      //run insert with left node
      node = &(*node)->left;
      insert(node, ID, name);
    }else if((*node)->ID < ID){
      //run insert with right node
      node = &(*node)->right;
      insert(node, ID, name);
    }else{
      printf("Node with ID %d already exists\n", ID);
    }
  }
}


void search(node_t *node, int ID)
{
  //checks ID within the node, if not goes to the childs, left if id smaller than ID
  //right if id bigger.
  int id=0;

  while(id != ID && node != NULL){
    id =node->ID;
    printf("Current node id %d\n", id);
    printf("Target %d\n", ID);
    if (id < ID){
      node = node->right;//go one deeper on the left
    } else if (id > ID) {
      node = node->left; //deeper on the right
    } else {
        id = ID;
    }
  }
  if( node != NULL){
    printf("Plat with ID %d, has name %s\n", id, node->name);
  }else{
    printf("Plant with ID Plat with ID %d does not exist", id);
  }
}


int main(int argc, char const *argv[])
{
   node_t *root = NULL;

   while(1) {
     printf("================================================\n");
     printf("Please enter ID of plant to add (zero to stop): ");
     int id;
     int r = scanf("%d", &id);
     if(r != 1 || id == 0) {
       printf("Breaking loop now.\n");
       break;
     }
     printf("OK, you entered id %d\n", id);
     printf("Please enter name of plant to add: ");
     char nameString[100]; /* Note: this will only work if the name entered is less than 100 characters long! */
     r = scanf("%s", nameString);
     if(r != 1) {
       printf("Failed to read name, breaking loop now.\n");
       break;
     }
     printf("OK, you have entered the following:  id = %d  nameString = %s\n", id, nameString);
     printf("Now insterting that into the tree...\n");
     insert(&root, id, nameString);
     printf("Now printing the tree:\n");
     print_bst(root);
   }
   int id;
   printf("After loop.\n");
   printf("What node do you want to look for? \n");
   scanf("%d", &id);
   printf("\n");
   search(root, id);
   delete_tree(&root);
   printf("Deleting the tree.\n");
   print_bst(root);

   printf("================================================\n");
   return 0;
}
