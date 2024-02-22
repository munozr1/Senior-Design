#include "map.h"
#include <stdio.h>

int main(void) {

  // make station linked list circular
  H.next = &A;
  // set starting position, this should be set via bluetooth or something. Hard
  // coded for now
  current_location.x = 45;
  current_location.y = 45;
  int dir = 0;

  // create map
  int map[MAP_SIZE][MAP_SIZE] = {0};

  while (1) {
    if (dir == 0) {
      current_location.x--;
      current_location.y--;
      if (current_location.x < 45) {
        dir = 1;
      }
    } else {
      current_location.x++;
      current_location.y++;
      if (current_location.x > 45) {
        dir = 0;
      }
    }
    clearMap(map);
    // open file
    FILE *file = fopen("lidar", "r");

    // read file
    char line[100];
    while (fgets(line, sizeof(line), file)) {
      mapPoint(line, map);
    }
    fclose(file);
    // print map
    for (int i = 0; i < MAP_SIZE; i++) {
      for (int j = 0; j < MAP_SIZE; j++) {
        printf("%d", map[i][j]);
      }
      printf("\n");
    }
  }

  return 0;
}
