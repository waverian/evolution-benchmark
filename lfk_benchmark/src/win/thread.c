/*
 * Copyright (c) 2022 waverian
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

#include "thread.h"

#include <stdlib.h>
#include <windows.h>

typedef struct lfk_thread_handler_t {
  HANDLE thread_handler;
  lfk_thread_routine routine;
  lfk_thread_args args;
} thread_handler_t;

static DWORD WINAPI _routine_wrapper(LPVOID handler_as_void) {
  lfk_thread_handler_p handler = handler_as_void;
  handler->routine(handler->args);
  return 0;
}

lfk_thread_handler_p lfk_thread_init(lfk_thread_routine routine,
                                     lfk_thread_args args) {
  lfk_thread_handler_p handler = calloc(1, sizeof(thread_handler_t));
  handler->routine = routine;
  handler->args = args;

  handler->thread_handler =
      CreateThread(NULL, 0, _routine_wrapper, handler, 0, NULL);
  return handler;
}

void lfk_thread_join(lfk_thread_handler_p handler) {
  WaitForSingleObject(handler->thread_handler, INFINITE);
  CloseHandle(handler->thread_handler);
  free(handler);
}