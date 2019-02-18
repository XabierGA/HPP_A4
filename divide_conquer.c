#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int int main(int argc, char const *args[]) {

  if (argc!=7){
    printf("Invalid number of arguments \n");
    printf("galsim expects: N filename nsteps delta_t theta_max graphics\n", );
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

  //look up table for powers of two
  const K=200;
  const double pow_2[K];
  pow[0]=1.0;
  for(int i=1; i< K; i++)
    pow_2[i]= pow_2[i-1]/2;

  //declare node structure
  //maybe good idea is to compact it! (ie no padding)
  typedef struct tree_node{
    double x_lim; //x division
    double y_lim; // y division
    double width; //width of the box
    double cm_x;
    double cm_y; //center of mass
    double tot_mass; //total mass
    struct tree_node *left;
    struct tree_node *right;
    struct tree_node *down;
    struct tree_node *up;
  }node_t

  // insert particles in quad tree
  //we pass a pointer to the node since we want to
  //modify what node we are using (ie pointing to)
  void insert(node_t **node, double x_pos, double y_pos, double mass){
    //insert node in tree with given coordinates x and y.
    //edits center of mass at each level.
    int up, right, depth;
    double x_lim_parent, y_lim_parent;

    //If node is null create it!
    if((*node) == NULL){
      //allocate mem for new node
      *node = (node_t*)malloc(sizeof(node_t));
      //if right is true, we go 2^(depth) to the right
      (*node)->x_lim = x_lim_parent + right*pow[depth-1] - pow[depth];
      (*node)->y_lim =  y_lim_parent + up*pow_2[depth-1] - pow[depth];
      (*node)-> width = pow_2[depth];
      (*node)-> cm_x = x_pos;
      (*node)-> cm_y = y_pos;
      (*node)-> total_mass = mass;
      (*node)->left = NULL;
      (*node)->right = NULL;
      (*node)->down = NULL;
      (*node)->up = NULL;
    }else{
      //update center of mass new_cm = old_mass*old_cm + pos_new_particle*mass_new_particle
      (*node)->cm_x += pos_x*(mass/total_mass); // Note: This maybe we can do more efficently
      (*node)->cm_y += pos_y*(mass/total_mass);
      (*node)->total_mass+=mass;
      if( x_pos > (*node)->x_lim){
        //go to the right
        
      }
    }

  }


  return 0;
}
