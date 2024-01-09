#include <intrin.h>
#include <windows.h>

int main() {
  __cpuid(NULL, 0x80000002);
  return 0;
}