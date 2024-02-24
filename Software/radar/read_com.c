#include "read_com.h"
#include "map.h"
#include <pthread.h>

void *daq_thread_read_com() {
  // Read and print data
  while (1) {
    if (((buffer_head + 1) % BUFFER_SIZE) == buffer_tail)
        continue;

   int bytes_read = read(daq_fd, daq_buffer, sizeof(daq_buffer));
    if (bytes_read < 0) {
      perror("daq_thread_read_com => Error reading from serial port");
    } else {
      daq_buffer[bytes_read] = '\0'; // Null terminate the string
      pthread_mutex_lock(&map_mutex);
      printf("daq_thread_read_com => map_mutex obtained\n");
      map_point(daq_buffer);
      buffer_head = (buffer_head + 1) % BUFFER_SIZE; // wrap around the ring buffer
      pthread_mutex_unlock(&map_mutex);
      printf("daq_thread_read_com => map_mutex released\n");
    }
  }
}
