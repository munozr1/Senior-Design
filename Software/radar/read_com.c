
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

int main() {
  const char *device =
      "/dev/cu.usbmodem14301"; // Replace with your actual COM port device
  int fd = open(device, O_RDWR | O_NOCTTY);
  if (fd < 0) {
    perror("Unable to open device");
    return 1;
  }

  struct termios tty;
  memset(&tty, 0, sizeof tty);

  // Get the current configuration of the serial interface
  if (tcgetattr(fd, &tty) != 0) {
    perror("Error from tcgetattr");
    return 1;
  }

  // Set baud rates to 9600
  cfsetospeed(&tty, B9600);
  cfsetispeed(&tty, B9600);

  // Setting other Port Stuff
  tty.c_cflag &= ~PARENB; // Make 8n1
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CSIZE;
  tty.c_cflag |= CS8;

  tty.c_cflag &= ~CRTSCTS; // no flow control
  tty.c_cc[VMIN] = 0;      // non-blocking mode
  tty.c_cc[VTIME] = 5;     // 0.5 seconds read timeout (5 tenths of a second)
  tty.c_cflag |= CREAD | CLOCAL; // turn on READ & ignore ctrl lines

  // Make raw
  cfmakeraw(&tty);

  // Flush Port, then applies attributes
  tcflush(fd, TCIFLUSH);
  if (tcsetattr(fd, TCSANOW, &tty) != 0) {
    perror("Error from tcsetattr");
    return 1;

    // Read data continuously
    char buf[256]; // Increased buffer size to handle larger strings
    int n;
    printf("Starting to read from the device:\n");
    while (1) {
      memset(buf, 0, sizeof(buf)); // Clear buffer
      n = read(fd, &buf,
               sizeof(buf) -
                   1); // Read a block of data (leave space for null terminator)
      if (n > 0) {
        printf("%s", buf); // Print the string received from Arduino
      } else if (n < 0) {
        printf("Error reading: %s\n", strerror(errno));
        break; // Exit if there is a read error
      }
      // Optional: Implement a way to exit the loop gracefully
    }

    close(fd); // Close the port
    return 0;
  }
}
