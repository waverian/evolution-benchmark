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

#ifdef EB_USE_WINAPI
#include <windows.h>
#endif

#include "thread.h"

#ifdef EB_USE_POSIX_THREADS
#include <pthread.h>
#endif

#include <stdlib.h>

typedef struct thread_handler_t {
#ifdef EB_USE_POSIX_THREADS
  pthread_t pthread_handler;
#else
#ifdef EB_USE_WIN_THREADS
  HANDLE thread_handler;
  thread_routine routine;
  thread_args args;
#else
#endif
#endif
} thread_handler_t;

#ifdef EB_USE_WIN_THREADS
static DWORD WINAPI _routine_wrapper(LPVOID handler_as_void) {
  thread_handler_p handler = handler_as_void;
  handler->routine(handler->args);
  return 0;
}
#endif

thread_handler_p thread_init(thread_routine routine, thread_args args) {
#ifdef EB_USE_POSIX_THREADS
  thread_handler_p handler = calloc(1, sizeof(thread_handler_t));
  handler->pthread_handler = 0;
  pthread_create(&handler->pthread_handler, NULL, routine, args);
  return handler;
#else
#ifdef EB_USE_WIN_THREADS
  thread_handler_p handler = calloc(1, sizeof(thread_handler_t));
  handler->routine = routine;
  handler->args = args;

  handler->thread_handler = CreateThread(NULL, 0, _routine_wrapper, handler, 0, NULL);
  return handler;
#else
  routine(args);
  return NULL;
#endif
#endif
}

void thread_join(thread_handler_p handler) {
#ifdef EB_USE_POSIX_THREADS
  pthread_join(handler->pthread_handler, NULL);
  free(handler);
#else
#ifdef EB_USE_WIN_THREADS
  WaitForSingleObject(handler->thread_handler, INFINITE);
  CloseHandle(handler->thread_handler);
  free(handler);
#else
  (void)handler;
#endif
#endif
}
