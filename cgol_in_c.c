#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_ROUNDS 10

size_t MAX_COL = 50;
size_t MAX_ROW = 50;

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

int get_modified_index_for_matrix(int index, bool is_col) {
  if (is_col && index < 0) {
    return (MAX_COL + index) % MAX_COL;
  } else if (is_col) {
    return index % MAX_COL;
  } else if (!is_col && index < 0) {
    return (MAX_ROW + index) % MAX_ROW;
  } else if (!is_col) {
    return index % MAX_ROW;
  } else {
    assert(false);
  }
}

int get_neighbour_of_cell_in_matrix(cell in[MAX_COL][MAX_ROW], int selected_col, int selected_row) {
  int amount = 0;
  for (int col = selected_col - 1; col <= selected_col + 1; col++) {
    for (int row = selected_row - 1; row <= selected_row + 1; row++) {
      if (in[get_modified_index_for_matrix(col, true)][get_modified_index_for_matrix(row, false)].alive) {
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

void remove_all_text() {
  printf("\e[1;1H\e[2J");
}

void test_get_modified_index_for_matrix() {
  assert(get_modified_index_for_matrix(-1, true) == MAX_COL-1);
  assert(get_modified_index_for_matrix(MAX_COL+1, true) == 1);
  assert(get_modified_index_for_matrix(MAX_COL, true) == 0);
}

void parse_file_into_playfield(char *input, cell playfield[MAX_COL][MAX_ROW]) {
  {
    int cc = 0;
    for (int i=0; i<MAX_COL; i++) {
      for (int j=0; j<MAX_ROW; j++) {
	if(input[cc] != '0' && input[cc] != '1'){
	  cc++;
	}
	playfield[j][i].alive = input[cc] == '1';
	cc++;
      }
    }
  }
}

int main(int argc, char** argv) {

  test_get_modified_index_for_matrix();
  
  cell play_field[MAX_COL][MAX_ROW];

  fill_cell_matrix(play_field);

  if (argc == 1) {
    play_field[2][1].alive = true;
    play_field[3][2].alive = true;
    play_field[1][3].alive = true;
    play_field[2][3].alive = true;
    play_field[3][3].alive = true;    
  } else if (argc == 2) {
    
    FILE *map_file = fopen(argv[1], "r");

    if(!map_file) {
      perror("Cannot get file!");
      return 74;
    }

    fseek(map_file, 0, SEEK_END); // seek to end of file
    int map_file_size = ftell(map_file); // get current file pointer
    fseek(map_file, 0, SEEK_SET);
    char *file_contents = malloc(map_file_size);
    fread(file_contents, sizeof(char), map_file_size, map_file);
    printf("%s", file_contents);
    MAX_COL = strtol(file_contents, NULL, 10);
    MAX_ROW = strtol(file_contents+sizeof(char)*4, NULL, 10);
    printf("MAX_ROW = %lu\n", MAX_ROW);
    parse_file_into_playfield(file_contents+sizeof(char)*8, play_field);
    print_cell_matrix(play_field);
    
  }
  
  for(int i=0; i <= MAX_ROUNDS; i++) {

    update_neighbours(play_field);

    remove_all_text();

    usleep(100000);

    /* sleep(1); */
    
    print_cell_matrix(play_field);

    update_alive_state(play_field);

  }
  
  return 0;
}
