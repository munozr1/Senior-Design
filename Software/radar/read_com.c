#include "read_com.h"
#include "map.h"
#include <pthread.h>

void * daq_thread_read_com() {
    pthread_mutex_lock(&daq_fd_mutex);
    // Read and print data
    while (1) {
        int bytes_read = read(daq_fd, daq_buffer, sizeof(daq_buffer));
        if (bytes_read < 0) {
            perror("daq_thread_read_com => Error reading from serial port");
        } else {
            daq_buffer[bytes_read] = '\0'; // Null terminate the string
            map_point(daq_buffer);
            // printf("%s", daq_buffer);
        }
    }
    pthread_mutex_unlock(&daq_fd_mutex);

}
