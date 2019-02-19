#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define POS_X  0
#define POS_Y 1
#define MASS 2

//declare node structure
//maybe good idea is to compact it! (ie no padding)
typedef struct body{
  double x_pos;
  double y_pos;
  double mass;
  double bright;
}body_t;

typedef struct tree_node{
  double x_lim; //x division (middle point in x)
  double y_lim; // y division (middle point in y)
  double width; //width of the box
  double cm_x;
  double cm_y; //center of mass of the quadrant
  double tot_mass; //total mass of the quadrant
  struct body *body_node; //body stored in the node
  struct tree_node *left_down; //Q3 child
  struct tree_node *left_up; //Q2 child
  struct tree_node *right_down; //Q4 child
  struct tree_node *right_up; //Q1 child
}node_t;

void print_qtree(node_t * node);
void insert(node_t **node, double x_pos, double y_pos, double mass,
   double x_lim_parent, double y_lim_parent, int depth, double* pow_2);

int main(int argc, char const *args[]){

  if (argc!=7){
    printf("Invalid number of arguments \n");
    printf("galsim expects: N filename nsteps delta_t theta_max graphics\n");
    return -1;
  }
  clock_t begin = clock();

  char *file_name = args[2];
  const int N = atoi(args[1]);
  const int n_steps = atoi(args[3]);
  /*not sure if this is the correct way of converting from
  character to double, maybe a single cast would suffice */
  const double delta_t = atof(args[4]);
  const double theta_max = atof(args[5]);

  //Read the file with initial conditions

  FILE *file;
  file = fopen(file_name , "rb");
  /*maybe in this case we could allocate memory for this
  matrix statically*/
  double **arr = (double **)malloc(N*sizeof(double*));
  for (int i = 0 ; i<N ; i++){
    arr[i] = (double*)malloc(6 * sizeof(double));
  }

  for (int i = 0 ; i<(N) ; i++){
    double x , y , vx , vy , mass , bright;
    fread(&x , sizeof(double) , 1 ,file);
    fread(&y , sizeof(double) , 1 ,file);
    fread(&mass , sizeof(double) , 1 ,file);
    fread(&vx , sizeof(double) , 1 ,file);
    fread(&vy , sizeof(double) , 1 ,file);
    fread(&bright, sizeof(double) , 1 ,file);
    arr[i][0] = x;
    arr[i][1] = y;
    arr[i][2] = mass;
    arr[i][3] = vx;
    arr[i][4] = vy;
    arr[i][5] = bright;
  }
  fclose(file);
  const float G = 100/(double)N;
  const float epsilon_0 = 0.001;
  //find powers of two so we only have to do it once
  const int K=200;
  double pow_2[K];
  pow_2[0]=1.0;
  for(int i=1; i< K; i++){
    pow_2[i]= pow_2[i-1]/2;
  }

  node_t * root = NULL;
  // insert particles in quad tree
  //we pass a pointer to the node since we want to
  //modify what node we are using (ie pointing to)
  printf("inserting nodes in the tree...\n");
  for(int i=0; i<N; i++){
    insert(&root ,arr[i][POS_X], arr[i][POS_Y], arr[i][MASS],
      0.5, 0.5, 1, pow_2);
  }
  clock_t end = clock();
  printf("Time: %lu \n", end - begin);
  printf("Particles: \n");
  for(int i=0; i<N; i++){
    printf("particle %d: m = %lf, (%lf,%lf) \n", i, arr[i][MASS],
    arr[i][POS_X], arr[i][POS_Y]);
  }
  printf("Printing tree .. \n");
  print_qtree(root);
  return 0;
}

//modify what node we are using (ie pointing to)
void insert(node_t **node, double x_pos, double y_pos, double mass,
  double x_lim, double y_lim, int depth, double* pow_2){
  //insert node in tree with given coordinates x and y.
  //If node is null create it!
  if((*node) == NULL){

    *node = (node_t*)malloc(sizeof(node_t));
    //if right is true, we go 2^(depth) to the right
    (*node)->x_lim = x_lim;
    (*node)->y_lim =  y_lim;
    (*node)-> width = pow_2[depth - 1];
    (*node)-> cm_x = x_pos;
    (*node)-> cm_y = y_pos;
    (*node)-> tot_mass = mass;
    (*node)->body_node = (body_t*)malloc(sizeof(body_t));
    (*node)->left_down = NULL;
    (*node)->left_up = NULL;
    (*node)->right_down = NULL;
    (*node)->right_up = NULL;

    (*node)->body_node->x_pos = x_pos;
    (*node)->body_node->y_pos = y_pos;
    (*node)->body_node->mass = mass;

  }else{
    //We go deeper in the tree
    //depth increase
    depth++;
    // NOTE: try to paralelize assigments with right and
    //left variables as u had it before
    //update x_lim, y_lim
    double x_lim_parent = (*node)->x_lim;
    double y_lim_parent = (*node)->y_lim;
    double cm_mass = (*node)->tot_mass;
    //update center of mass new_cm = old_mass*old_cm + pos_new_particle*mass_new_particle
    (*node)->tot_mass+=mass;
    //we can save operations in here by declaring new varaibles
    (*node)->cm_x = cm_mass/(*node)->tot_mass * (*node)->cm_x + x_pos*(mass/(*node)->tot_mass); // Note: This maybe we can do more efficently
    (*node)->cm_y = cm_mass/(*node)->tot_mass* (*node)->cm_y + y_pos*(mass/(*node)->tot_mass);
    if( x_pos > x_lim_parent){
      //x_pos is bigger than current x_lim so we go to the right
      x_lim = x_lim_parent + pow_2[depth];
      if(y_pos > y_lim_parent){
        //y_pos bigger we go up
        y_lim =  y_lim_parent + pow_2[depth];
        node = &(*node)->right_up;
      }else{
        y_lim =  y_lim_parent - pow_2[depth];
        node =&(*node)->right_down;
      }
    }else{
      x_lim = x_lim_parent - pow_2[depth];
      if(y_pos > y_lim_parent){
        //x_pos is smaller than current x_lim so we go to the right
        //y_pos bigger we go up
        y_lim =  y_lim_parent+ pow_2[depth];
        node = &(*node)->left_up;
      }else{
        y_lim = y_lim_parent - pow_2[depth];
        node= &(*node)->left_down;
      }
    }
    //call recursively insert with one deeper node
    insert(node, x_pos, y_pos, mass, x_lim, y_lim, depth, pow_2);
  }
  return;
}

void print_qtree(node_t * node){
  if(node == NULL){printf("Tree is empty \n"); return;}

  if(node != NULL){
    printf("total mass: %lf, cm: (%lf, %lf) limits: (%lf,%lf). body: (%lf, %lf) m- %lf) \t",
    node->tot_mass,
    node->cm_x, node->cm_y,
    node->x_lim, node->x_lim,
    node->body_node->x_pos, node->body_node->y_pos, node->body_node->mass);
  }
  if (node->right_up != NULL) printf("Q1: total mass %lf,", node->right_up->tot_mass);
  if (node->left_up != NULL) printf("Q2: total mass: %lf,", node->left_up->tot_mass);
  if (node->left_down != NULL) printf("Q3: total mass: %lf,", node->left_down->tot_mass);
  if (node->right_down != NULL) printf("Q4: total mass; %lf,", node->right_down->tot_mass);
  printf("\n");

  if (node->right_up != NULL) print_qtree(node->right_up);
  if (node->left_up != NULL) print_qtree(node->left_up);
  if (node->left_down != NULL) print_qtree(node->left_down);
  if (node->right_down != NULL) print_qtree(node->right_down);
  return;
}
