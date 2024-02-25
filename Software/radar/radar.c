#include "map.h"
#include "threads.h"
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

// create map and buffer
pthread_mutex_t map_mutex = PTHREAD_MUTEX_INITIALIZER; // The map mutex is used to ensure that only one thread can access the map at a time.
int map[MAP_SIZE][MAP_SIZE] = {0}; // The map is used to store the points that are mapped.
struct Coordinate map_ring_buffer[BUFFER_SIZE]; // The ring buffer is used to store and manage points on the map. It enables the efficient
                                                // addition of new points and deletion of old ones by treating the buffer as a circular array.
int buffer_head = 0; // The head of the buffer is used to add new points to the buffer.
int buffer_tail = 0; // The tail of the buffer is used to delete old points from the buffer.

pthread_mutex_t daq_fd_mutex = PTHREAD_MUTEX_INITIALIZER; // The daq_fd_mutex is used to ensure that only one thread can access the com port at a time.
int daq_fd; // File Descriptor: used to read the com port. In unix, everything is a file.
char daq_buffer[15]; // Data Aquisition Buffer: used to store the info sent from arduino via com port.

int main(void) {
  // make station linked list circular
  H.next = &A;
  // init threads;
  pthread_t daq_thread_id; // Thread responsible for Data Aquisition from sensors
  pthread_t del_thread_id; // Thread responsible for deleting old points in map
  // config to read com port
  const char *serial_port_path = "/dev/cu.usbmodem14201";
  struct termios tio; // Terminal I/O

  // Open port
  daq_fd = open(serial_port_path, O_RDWR | O_NOCTTY);
  if (daq_fd < 0) {
    perror("Error opening serial port");
    return -1;
  }

  // Configure port
  memset(&tio, 0, sizeof(tio));
  cfsetospeed(&tio, B9600);
  cfsetispeed(&tio, B9600);
  tio.c_cflag &= ~PARENB;
  tio.c_cflag &= ~CSTOPB;
  tio.c_cflag &= ~CSIZE;
  tio.c_cflag |= CS8;
  tio.c_cflag |= CREAD | CLOCAL;
  tcsetattr(daq_fd, TCSANOW, &tio);
  tcflush(daq_fd, TCIFLUSH);

  // init reading data from com port
  if (pthread_create(&daq_thread_id, NULL, daq_thread_read_com, NULL) != 0) {
    perror("Failed to create DAQ thread");
    close(daq_fd);
    return -1;
  } else {
    printf("daq thread created \n");
  }

  // init deleting old points in map
  if (pthread_create(&del_thread_id, NULL, del_thread_buffer_maintinance, NULL) != 0) {
    perror("Failed to create delete thread \n");
    close(daq_fd);
    return -1;
  } else {
    printf("delete thread created \n");
  }

  // Set starting position, this should be set via bluetooth or something. Hard coded for now.
  current_location.x = 45;
  current_location.y = 45;

// Main loop: print the map
#ifdef DEBUG
  while (1) {
    pthread_mutex_lock(&map_mutex);
    printf("main => map_mutex obtained\n");
    printf("main => map_mutex released\n");
    pthread_mutex_unlock(&map_mutex);
    sleep(1);
  }
#else
    // pthread_join(&del_thread_id,NULL);
    // pthread_join(&daq_thread_id,NULL);
  // int temp_map[MAP_SIZE][MAP_SIZE] = {0};
  while (1) {
  //   pthread_mutex_lock(&map_mutex);
  //   //copy map to temp_map
  //   for (int i = 0; i < MAP_SIZE; i++) {
  //     for (int j = 0; j < MAP_SIZE; j++) {
  //       temp_map[i][j] = map[i][j];
  //     }
  //   }
  //   pthread_mutex_unlock(&map_mutex);
  //   system("clear");
  //   // print temp_map
  //   for (int i = 0; i < MAP_SIZE; i++) {
  //     for (int j = 0; j < MAP_SIZE; j++) {
  //       if (temp_map[i][j] == 1) {
  //         printf("X");
  //       } else {
  //         printf(" ");
  //       }
  //     }
  //     printf("\n");
  //   }
  //   // sleep for .5 seconds
  //   usleep(250000);
  }

#endif
  return 0;
}
