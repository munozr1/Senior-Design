#pragma once
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

//Circular buffer size is 180 because the LiDAR sensor has a 180 degree field of view
#define BUFFER_SIZE 180
#define MAP_SIZE 150
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
  time_t created;
};

extern pthread_mutex_t map_mutex;
extern int map[MAP_SIZE][MAP_SIZE];
extern struct Coordinate map_ring_buffer[180];
extern int buffer_head;
extern int buffer_tail;


extern struct ChargingStation A, B, C, D, E, F, G, H;
extern struct Coordinate current_location;

double deg2rad(double deg);
double mm2in(double mm);
void map_point(char *polarCoord);
void clear_map();
void * del_thread_buffer_del();
void print_map();
