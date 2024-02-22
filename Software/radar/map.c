#include "map.h"
#include <unistd.h>

struct ChargingStation H = {0, 0, 'H', 0, NULL};
struct ChargingStation G = {0, 0, 'G', 0, &H};
struct ChargingStation F = {0, 0, 'F', 0, &G};
struct ChargingStation E = {0, 0, 'E', 0, &F};
struct ChargingStation D = {0, 0, 'D', 0, &E};
struct ChargingStation C = {0, 0, 'C', 0, &D};
struct ChargingStation B = {0, 0, 'B', 0, &C};
struct ChargingStation A = {0, 0, 'A', 0, &B};
struct Coordinate current_location = {0, 0};

double mm2inches(double mm) { return mm / 25.4; }

double deg2rad(double deg) { return deg * (PI / 180); }

void mapPoint(char *polarCoord, int map[MAP_SIZE][MAP_SIZE]) {
  double distance, angle;

  sscanf(polarCoord, "%lf@%lf", &distance, &angle);

  angle = deg2rad(angle);

  int x = (int)(distance * cos(angle));
  int y = (int)(distance * sin(angle));

  int gridX = x + current_location.x;
  int gridY = y + current_location.y;

  if (gridX >= 0 && gridX < MAP_SIZE && gridY >= 0 && gridY < MAP_SIZE) {
    map[gridX][gridY] = 1;
  } else {
    printf("Point map[%d][%d] (%lf, %lf) is out of bounds\n", gridX, gridY,
           distance, angle);
  }
}

void clearMap(int map[MAP_SIZE][MAP_SIZE]) {
  system("clear");
  for (int i = 0; i < MAP_SIZE; i++) {
    for (int j = 0; j < MAP_SIZE; j++) {
      map[i][j] = 0;
    }
  }
}
