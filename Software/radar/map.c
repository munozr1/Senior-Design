#include "map.h"
#include "read_com.h"
#include <pthread.h>
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
  if (gridX < 0 || gridX >= MAP_SIZE || gridY < 0 || gridY >= MAP_SIZE) {
    // Coordinates are out of bounds
    return;
  }

  // Add the point to the ring buffer. Add a timestamp so that the deletion
  // thread knows when to delete the point
  pthread_mutex_lock(&map_mutex);
#ifdef DEBUG
  printf("map_point() => map_mutex obtained, buffer_tail = %d\n", buffer_tail);
#endif
  map_ring_buffer[buffer_tail].x = gridX;
  map_ring_buffer[buffer_tail].y = gridY;
  time(&map_ring_buffer[buffer_tail].created);
// print the time stamp
#ifdef DEBUG
  printf("(%d, %d) %s\n", map_ring_buffer[buffer_tail].x,
         map_ring_buffer[buffer_tail].y,
         ctime(&map_ring_buffer[buffer_tail].created));
#endif
  buffer_tail = (buffer_tail + 1) % BUFFER_SIZE; // wrap around the ring buffer

  if (gridX >= 0 && gridX < MAP_SIZE && gridY >= 0 && gridY < MAP_SIZE) {
    map[gridX][gridY] = 1;
  } else {
    printf("Point map[%d][%d] (%lf, %lf) is out of bounds\n", gridX, gridY,
           distance, angle);
  }
#ifdef DEBUG
  printf("map_point() => map_mutex released\n");
#endif
  pthread_mutex_unlock(&map_mutex);
}

void *del_thread_buffer_del() {
  sleep(1);
  while (1) {
    pthread_mutex_lock(&map_mutex);
#ifdef DEBUG
    printf("del_thread_buffer_del() => map_mutex obtained, buffer_head = %d timestamp = %s\n", buffer_head, ctime(&map_ring_buffer[buffer_head].created));
#endif

    time_t currentTime;
    time(&currentTime);

    // Iterate while the head points to a point that's too old
    while (buffer_head != buffer_tail && difftime(currentTime, map_ring_buffer[buffer_head].created) > 1) {
      map[map_ring_buffer[buffer_head].x][map_ring_buffer[buffer_head].y] = 0;
#ifdef DEBUG
      printf("del_thread_buffer_del() => Deleting point (%d, %d) %s\n", map_ring_buffer[buffer_head].x,map_ring_buffer[buffer_head].y,ctime(&map_ring_buffer[buffer_head].created));
#endif
      buffer_head = (buffer_head + 1) % BUFFER_SIZE;
    }
    system("clear");
    print_map();
    sleep(1);

#ifdef DEBUG
    if(buffer_head == buffer_tail) {
        printf("BUFFER EMPTY!\n");
        sleep(1);
    }
    printf("del_thread_buffer_del() => map_mutex released\n");
#endif
    pthread_mutex_unlock(&map_mutex);
  }
  return 0;
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
