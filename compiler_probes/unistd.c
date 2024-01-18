#include "unistd.h"

int main() {
  return (int)sysconf(_SC_NPROCESSORS_ONLN);
}