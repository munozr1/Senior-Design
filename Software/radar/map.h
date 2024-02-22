#pragma once
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAP_SIZE 96
#define PI 3.14159265358979323846

struct ChargingStation {
  char name;
  int x;
  int y;
  int occupied;
  struct ChargingStation *next;
};
struct Coordinate {
  int x;
  int y;
};
extern struct ChargingStation A, B, C, D, E, F, G, H;
extern struct Coordinate current_location;

double deg2rad(double deg);
double mm2in(double mm);
void mapPoint(char *polarCoord, int map[MAP_SIZE][MAP_SIZE]);
void clearMap(int map[MAP_SIZE][MAP_SIZE]);
