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
  


  //declare node structure
  //maybe good idea is to compact it! (ie no padding)
  typedef struct tree_node{
    double x_lim; //x division
    double y_lim; // y division
    double width; //width of the box
    double cm; //center of mass
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
    double parent_x_lim, parent_y_lim;

    //If node is null create it!
    if((*node) == NULL){
      //allocate mem for new node
      *node = (node_t*)malloc(sizeof(node_t));
      (*node)->x_lim =
    }

  }


  return 0;
}
