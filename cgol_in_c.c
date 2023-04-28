#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#define MAX_ROUNDS 100

const size_t MAX_COL = 20;
const size_t MAX_ROW = 20;

typedef struct cell {
  bool alive;
  unsigned int neighbours;
}cell;

void print_cell_matrix(cell in[MAX_COL][MAX_ROW]) {
  for (int row = 0; row < MAX_ROW; row++) {
    for (int col = 0; col < MAX_COL; col++) {
      printf(in[col][row].alive? "X " : "  ");
    }
    printf("\n");
  }
}


void print_neighbours_amount(cell in[MAX_COL][MAX_ROW]) {
  for (int row = 0; row < MAX_ROW; row++) {
    for (int col = 0; col < MAX_COL; col++) {
      printf("%i\t", in[col][row].neighbours);
    }
    printf("\n");
  }
}

void fill_cell_matrix(cell in[MAX_COL][MAX_ROW]) {
  for (int col = 0; col < MAX_COL; col++) {
    for (int row = 0; row < MAX_ROW; row++) {
      in[col][row].alive = false;
      in[col][row].neighbours = 0;      
    }
  }
}

int get_neighbour_of_cell_in_matrix(cell in[MAX_COL][MAX_ROW], int selected_col, int selected_row) {
  int amount = 0;
  for (int col = selected_col - 1; col <= selected_col + 1; col++) {
    for (int row = selected_row - 1; row <= selected_row + 1; row++) {
      if (in[col % (MAX_COL)][row % (MAX_ROW)].alive) {
	amount++;
      }
    }
  }
  if (in[selected_col][selected_row].alive) {
    amount--;
  }
  assert(amount <= 8);
  assert(amount >= 0);
  return amount;
}

void copy_cell_matrix(cell in[MAX_COL][MAX_ROW], cell output[MAX_COL][MAX_ROW]) {
  for (int col = 0; col < MAX_COL; col++) {
    for (int row = 0; row < MAX_ROW; row++) {
      output[col][row].alive = in[col][row].alive;
      output[col][row].neighbours = in[col][row].neighbours;
    }
  }
}

void update_neighbours(cell in[MAX_COL][MAX_ROW]) {
  cell output[MAX_COL][MAX_ROW];
  copy_cell_matrix(in, output); 
  for (int col = 0; col < MAX_COL; col++) {
    for (int row = 0; row < MAX_ROW; row++) {
      output[col][row].neighbours = get_neighbour_of_cell_in_matrix(in, col, row);
    }
  }
  copy_cell_matrix(output, in);
}

void update_alive_state(cell in[MAX_COL][MAX_ROW]) {
  for (int col = 0; col < MAX_COL; col++) {
    for (int row = 0; row < MAX_ROW; row++) {
      if(in[col][row].neighbours == 3 || (in[col][row].neighbours == 2 || in[col][row].neighbours == 3) && in[col][row].alive) {
	in[col][row].alive = true;  
      } else {
	in[col][row].alive = false;
      }
    }
  }
}

int main() {
  cell play_field[MAX_COL][MAX_ROW];

  fill_cell_matrix(play_field);

  play_field[2][1].alive = true;
  play_field[3][2].alive = true;
  play_field[1][3].alive = true;
  play_field[2][3].alive = true; 
  play_field[3][3].alive = true; 

  for(int i=0; i <= MAX_ROUNDS; i++) {

    update_neighbours(play_field);

    /* print_neighbours_amount(play_field); */
    
    print_cell_matrix(play_field);

    update_alive_state(play_field);
  
  }

  // TODO: Fix the Modulo for the Board edges.
  
  return 0;
}
