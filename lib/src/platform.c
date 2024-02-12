/*
 * Copyright (c) 2022 waverian. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifdef EB_USE_WIN_CPUID
#include <windows.h>
#endif

#include <string.h>

#include "platform.h"
#ifdef EB_USE_UNISTD
#include <unistd.h>
#endif
#ifdef EB_USE_POSIX_CPUID
#include <cpuid.h>
#endif
#ifdef EB_USE_WIN_CPUID
#include <intrin.h>
#endif

int get_core_count() {
#ifdef EB_USE_WIN_CPUID
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  return (int)sysinfo.dwNumberOfProcessors;
#else
#ifdef EB_USE_UNISTD
  return (int)sysconf(_SC_NPROCESSORS_ONLN);
#endif
  return 1;
#endif
}

char *get_cpu_name() {
#ifdef EB_USE_WIN_CPUID
  int brand[12];
  __cpuid(&brand[0], 0x80000002);
  __cpuid(&brand[4], 0x80000003);
  __cpuid(&brand[8], 0x80000004);
  return strdup((char *)brand);
#else
#ifdef EB_USE_POSIX_CPUID
  unsigned int brand[12];
  if (!__get_cpuid_max(0x80000004, NULL)) {
    return strdup("No CPUID information");
  }

  __get_cpuid(0x80000002, brand + 0x0, brand + 0x1, brand + 0x2, brand + 0x3);
  __get_cpuid(0x80000003, brand + 0x4, brand + 0x5, brand + 0x6, brand + 0x7);
  __get_cpuid(0x80000004, brand + 0x8, brand + 0x9, brand + 0xa, brand + 0xb);

  // remove trailing whitespaces
  char *ret = (char *)brand;
  while (' ' == *ret) {
    ret++;
  }

  return strdup((char *)brand);
#else
  return strdup("No CPUID information");
#endif
#endif
}