#include "map.h"
#include "read_com.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
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

void map_point(char *polarCoord) {
  double distance, angle;

  // parse the polar coordinate input string
  sscanf(polarCoord, "%lf@%lf", &angle, &distance);
  // if we read a distance bigger than we can accomidate just ignore it. This is
  // more used for testing purposes. Its a good saftey though.
  if (distance > MAP_SIZE)
    return;

  angle = deg2rad(angle);
  int x = (int)(distance * cos(angle));
  int y = (int)(distance * sin(angle));

  int gridX = x + current_location.x;
  int gridY = y + current_location.y;
  if (gridX < 0 || gridX >= MAP_SIZE || gridY < 0 || gridY >= MAP_SIZE)
    return;


  if (gridX >= 0 && gridX < MAP_SIZE && gridY >= 0 && gridY < MAP_SIZE) {
    map_ring_buffer[buffer_head].x = gridX; // Add the point to the ring buffer
    map_ring_buffer[buffer_head].y = gridY;
    time(&map_ring_buffer[buffer_head].created); // Add a timestamp
    map[gridX][gridY] = 1;// Add the point to the map
  } else {
    printf("Point map[%d][%d] (%lf, %lf) is out of bounds\n", gridX, gridY,
           distance, angle);
  }
}

void *del_thread_buffer_del() {
  sleep(1); // Sleep for 1 second to let the map_thread get a head start
  while (1) {
    if (buffer_tail == buffer_head) // if buffer is empty
      continue;
    pthread_mutex_lock(&map_mutex);
    #ifdef DEBUG
    printf("del_thread_buffer_del => map_mutex obtained\n");
    #endif
    time_t currentTime; // Get the current time
    time(&currentTime);

    // Iterate while the head points to a point that's too old
    while (buffer_tail != buffer_head && difftime(currentTime, map_ring_buffer[buffer_tail].created) > 1) {
      map[map_ring_buffer[buffer_tail].x][map_ring_buffer[buffer_tail].y] = 0; // Remove the point from the map
      buffer_tail = (buffer_tail + 1) % BUFFER_SIZE; // Move the tail to the next point
    }
    pthread_mutex_unlock(&map_mutex);
    #ifdef DEBUG
    printf("del_thread_buffer_del => map_mutex released\n");
    #endif
  }
}

  void print_map() {
    for (int i = 0; i < MAP_SIZE; i++) {
      for (int j = 0; j < MAP_SIZE; j++) {
        if (map[i][j] == 1) {
          printf("X");
        } else {
          printf(" ");
        }
      }
      printf("\n");
    }
  }
