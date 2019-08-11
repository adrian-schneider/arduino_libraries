#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Integer or Fixed Point TO Ascii
// Convert an integer 'i' to an integer or fixed point ASCII representation of
// 'width' with 'dec' places after the decimal point. Set 'dec' to -1 to get
// a plain integer representation. Set 'dec' to -16 to get a hexadecimal
// integer representation.
// A sufficently sized buffer 'buf' has to be provided. The buffer is returned.
const char *ifptoa(int i, int width, int dec, char *buf) {
  buf[--width] = 0;
  memset(buf, ' ', --width);

  int m = (dec == -16) ? 16 : 10; // -16 also suppresses the decimal point.

  int sign = false;
  if (i < 0) {
    sign = true;
    i = -i;
  }

  int d = 0;
  do {
    if (dec == d++) {
      buf[width--] = '.';
    } else {
      int j = i % m;
      buf[width--] = (j < 10 ? '0' : '\067') + j; // '\067' == 'A' - 10
      i /= m;
    }
  } while (i || (d - 2) < dec);

  if (sign) buf[width] = '-';

  return buf;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    puts("int argument required.\n");
    exit(1);
  }

  int i = atoi(argv[1]);
  char buf[32];
  ifptoa(i, 8, 4, buf);

  printf("%d :: '%s'\n", i, buf);
  return 0;
}
