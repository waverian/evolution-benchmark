# Copyright (c) 2022 waverian. All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


'''This is the python module for interfacing with lfk-mp-benchmark.

 Usage::

     import lfkbenchmark
     benchmark = lfkbenchmark.lfk_benchmark()
     benchmark.console_run_benchmark()


 Repository: https://github.com/waverian/py-lfk-mp-benchmark

 For details of the C module look at https://github.com/waverian/lfk-mp-benchmark
 '''
 
cython: language_level=3

from typing import Callable

import os
try:
    from kivy.logger import Logger
except ImportError: 
    import logging

    log = logging.getLogger()
    log.setLevel(logging.NOTSET)
    hd = logging.StreamHandler()
    hd.setLevel(logging.NOTSET)
    log.addHandler(hd)
    Logger = log


CPU_COUNT = 0 
'''
0:  is auto # os.cpu_count()
'''

cdef extern from "waverian/evolution_benchmark/constants.h":
    cdef int EB_KERNEL_COUNT
    cdef char *EB_KERNEL_NAMES[]
    cdef char *EB_OPTIMIZATION_NAMES[]
    cdef char *EB_RUN_TYPE_NAMES[]
    cdef char *EB_EXECUTION_MODE_NAMES[]

cdef extern from "waverian/evolution_benchmark/error_code.h":
    ctypedef enum EB_ERROR_CODE_E:
        EB_ERROR_CODE_OK,
        EB_ERROR_CODE_NULL_POINTER,
        EB_ERROR_CODE_INVALID_ARGUMENT,
        EB_ERROR_CODE_FILE_WRITE_ERROR

cdef extern from "waverian/evolution_benchmark/types.h":
    ctypedef void *eb_handler_t

    ctypedef enum EB_RUN_TYPE_E:
        EB_RUN_TYPE_MANUAL,
        EB_RUN_TYPE_SINGLE_CORE,
        EB_RUN_TYPE_MULTI_CORE,
        EB_RUN_TYPE_QUAD_CORE,
        EB_RUN_TYPE_WORKSTATION,
        EB_RUN_TYPE_SIZE

    ctypedef enum EB_EXECUTION_MODE_E:
        EB_EXECUTION_MODE_MANUAL,
        EB_EXECUTION_MODE_AUTO,
        EB_EXECUTION_MODE_FULL_AUTO,
        EB_EXECUTION_MODE_SIZE

    ctypedef enum EB_OPTIMIZATION_E:
        EB_OPTIMIZATION_DISABLED,
        EB_OPTIMIZATION_ENABLED,
        EB_OPTIMIZATION_SIZE

    ctypedef struct eb_system_info_t:
        const char *compiler_info
        const char *version_info
        const char *cpu_name
        int cpu_core_count

    ctypedef struct eb_run_result_t:
        int valid

        double score
        double ratio

        double maximum
        double average
        double geometric
        double harmonic
        double minimum

        double kernels[24]

    ctypedef struct eb_full_result_t:
        double score
        eb_run_result_t detailed[5]

    ctypedef struct eb_result_t:
        eb_system_info_t system_info
        char timestamp[32]
        char comment[128]
        eb_full_result_t full_result[2]

cdef extern from "waverian/evolution_benchmark.h":
    ctypedef int (*eb_progress_callback_t)(void *data, int progress, const char *message)

    ctypedef struct eb_progress_callback_handler_t:
        void *data
        eb_progress_callback_t callback

    eb_handler_t eb_init()
    EB_ERROR_CODE_E eb_cleanup(eb_handler_t handler)
    EB_ERROR_CODE_E eb_set_progress_callback(eb_handler_t handler,
                                                           eb_progress_callback_handler_t callback)

    EB_ERROR_CODE_E eb_set_execution_time(eb_handler_t handler, double execution_time)
    EB_ERROR_CODE_E eb_set_core_count_limit(eb_handler_t handler, int core_count)
    EB_ERROR_CODE_E eb_set_comment(eb_handler_t handler, const char *comment)
    EB_ERROR_CODE_E eb_set_execution_mode(eb_handler_t handler, EB_EXECUTION_MODE_E execution_mode)
    EB_ERROR_CODE_E eb_run_benchmark(eb_handler_t handler, eb_result_t* result) nogil

    EB_ERROR_CODE_E eb_get_system_info(eb_handler_t handler, eb_system_info_t* system_info)

cdef extern from "waverian/evolution_benchmark/printers.h":
    EB_ERROR_CODE_E eb_result_print_html(eb_result_t result, const char *path)
    EB_ERROR_CODE_E eb_result_print_text(eb_result_t result, const char *path)


cdef class Result():
    cdef eb_result_t data
    cdef public dict result

    cdef init(self, eb_result_t result):
        self.data = result
        self.result = dict(result)
        opts = ['non_optimized','optimized']
        runs = ['manual', 'single_core', 'multi_core', 'quad_core', 'workstation']
        self.result['full_result'] = dict(zip(opts, self.result['full_result']))
        for opt in opts:
            self.result['full_result'][opt]['detailed'] = dict(zip(runs, self.result['full_result'][opt]['detailed']))

        for opt in opts:
            for run in runs:
                if self.result['full_result'][opt]['detailed'][run]['valid'] == 0:
                    self.result['full_result'][opt]['detailed'][run] = None
                else:
                    del self.result['full_result'][opt]['detailed'][run]['valid']

        def fix_string(result):
            for key, value in result.items():
                if isinstance(value, dict):
                    fix_string(value)
                elif isinstance(value, bytes):
                    result[key] = value.decode('utf-8')
        fix_string(self.result)


    def print_results_html(self, path: str):
        eb_result_print_html(self.data, bytes(path, 'utf-8'))

    def print_results_text(self, path: str):
        eb_result_print_text(self.data, bytes(path, 'utf-8'))


cdef class Benchmark:
    '''Wrapper around the lfkbenchmark module.
    '''

    cdef eb_handler_t handler
    cdef eb_system_info_t parameters

    @staticmethod
    cdef int progress_callback_internal(void* data, int progress,const char *message) nogil:
        with gil:
            return (<Benchmark>data).__call_callback(progress, message.decode('utf-8'))

    cdef eb_progress_callback_handler_t callback_handler

    cdef object external_callback

    def __call_callback(self, progress: int, message: str):
        if self.external_callback:
            ret = self.external_callback(progress, message)
            if type(ret) is not int:
                return 0
            return ret
        #default callback
        Logger.debug(f'LFKBenchmark: {progress}, {message}')
        return 0

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.handler = eb_init()
        #self.parameters = eb_get_system_info(self.handler)

        self.external_callback = None

        self.callback_handler.data = <void*>self
        self.callback_handler.callback = self.progress_callback_internal

        eb_set_progress_callback(self.handler, self.callback_handler)
        eb_set_execution_time(self.handler, 1.00000 if 'EVOBENCHTESTMODE' not in os.environ else .0000001)

    def __del__(self):
        eb_cleanup(self.handler)

    def run_benchmark(self, comment: str = None, workstation: bool = False, callback: Callable[[int, str], int] = None):
        '''Run benchmark from UI.
        Warning::

            This is a blocking operation. You should be running this through a thread/async task.

        Arguments::
        
            `callback` pass a function here. Defaults to `None`.
        '''
        if comment:
            eb_set_comment(self.handler, bytes(comment, 'utf-8'))
        if workstation:
            eb_set_execution_mode(self.handler, EB_EXECUTION_MODE_FULL_AUTO)

        Logger.debug('LFKBenchmark:  Setting up benchmark.')

        self.external_callback = callback

        cdef eb_system_info_t c_system_info
        cdef eb_result_t c_result

        eb_get_system_info(self.handler, &c_system_info)

        Logger.debug('LFKBenchmark: Runing benchmark.')
        with nogil:
            eb_run_benchmark(self.handler, &c_result)


        Logger.debug('LFKBenchmark: Returning results.')
        result = Result()
        result.init(c_result)
        return result

    def run_print_benchmark(self):
        print(self.run_benchmark())

    def console_run_benchmark(self):
        '''Run the benchmark from console without blocking the terminal.
        '''
        import threading
        thread = threading.Thread(target=self.run_print_benchmark)
        thread.start()

    def get_parameters(self):
        cdef eb_system_info_t system_info
        eb_get_system_info(self.handler, &system_info)
        return system_info
