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

__version__ = 'v1.0.0-Beta.4'

cdef extern from "waverian/benchmark/details/constants.h":
    cdef int WB_KERNEL_COUNT
    cdef char *WB_KERNEL_NAMES[]
    cdef char *WB_OPTIMIZATION_NAMES[]
    cdef char *WB_RUN_TYPE_NAMES[]
    cdef char *WB_EXECUTION_MODE_NAMES[]

cdef extern from "waverian/benchmark/details/error_code.h":
    ctypedef enum BENCHMARK_ERROR_CODE_E:
        BENCHMARK_ERROR_CODE_OK,
        BENCHMARK_ERROR_CODE_NULL_POINTER,
        BENCHMARK_ERROR_CODE_INVALID_ARGUMENT,
        BENCHMARK_ERROR_CODE_FILE_WRITE_ERROR

cdef extern from "waverian/benchmark/details/types.h":
    ctypedef void *benchmark_handler_t

    ctypedef enum WB_RUN_TYPE_E:
        WB_RUN_TYPE_MANUAL,
        WB_RUN_TYPE_SINGLE_CORE,
        WB_RUN_TYPE_MULTI_CORE,
        WB_RUN_TYPE_QUAD_CORE,
        WB_RUN_TYPE_WORKSTATION,
        WB_RUN_TYPE_SIZE

    ctypedef enum WB_EXECUTION_MODE_E:
        WB_EXECUTION_MODE_MANUAL,
        WB_EXECUTION_MODE_AUTO,
        WB_EXECUTION_MODE_FULL_AUTO,
        WB_EXECUTION_MODE_SIZE

    ctypedef enum WB_OPTIMIZATION_E:
        WB_OPTIMIZATION_DISABLED,
        WB_OPTIMIZATION_ENABLED,
        WB_OPTIMIZATION_SIZE

    ctypedef struct system_info_t:
        const char *compiler_info
        const char *version_info
        const char *cpu_name
        int cpu_core_count

    ctypedef struct run_result_t:
        int valid

        double score
        double ratio

        double maximum
        double average
        double geometric
        double harmonic
        double minimum

        double kernels[24]

    ctypedef struct full_result_t:
        double score
        run_result_t detailed[5]

    ctypedef struct benchmark_result_t:
        system_info_t system_info
        char timestamp[32]
        char comment[128]
        full_result_t full_result[2]

cdef extern from "waverian/benchmark/benchmark.h":
    ctypedef int (*benchmark_progress_callback_t)(void *data, int progress, const char *message)

    ctypedef struct benchmark_progress_callback_handler_t:
        void *data
        benchmark_progress_callback_t callback

    benchmark_handler_t benchmark_init()
    BENCHMARK_ERROR_CODE_E benchmark_cleanup(benchmark_handler_t handler)
    BENCHMARK_ERROR_CODE_E benchmark_set_progress_callback(benchmark_handler_t handler,
                                                           benchmark_progress_callback_handler_t callback)

    BENCHMARK_ERROR_CODE_E benchmark_set_execution_time(benchmark_handler_t handler, double execution_time)
    BENCHMARK_ERROR_CODE_E benchmark_set_core_count_limit(benchmark_handler_t handler, int core_count)
    BENCHMARK_ERROR_CODE_E benchmark_set_comment(benchmark_handler_t handler, const char *comment)
    BENCHMARK_ERROR_CODE_E benchmark_set_execution_mode(benchmark_handler_t handler, WB_EXECUTION_MODE_E execution_mode)
    BENCHMARK_ERROR_CODE_E benchmark_run(benchmark_handler_t handler, benchmark_result_t* result) nogil

    BENCHMARK_ERROR_CODE_E benchmark_get_system_info(benchmark_handler_t handler, system_info_t* system_info)

cdef extern from "waverian/benchmark/printers.h":
    BENCHMARK_ERROR_CODE_E benchmark_print_results_html(benchmark_result_t result, const char *path)
    BENCHMARK_ERROR_CODE_E benchmark_print_results_text(benchmark_result_t result, const char *path)


cdef class Result():
    cdef benchmark_result_t data
    cdef public dict result

    cdef init(self, benchmark_result_t result):
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
                    print(f'deleting {opt} {run}')
                    del self.result['full_result'][opt]['detailed'][run]

        def fix_string(result):
            for key, value in result.items():
                if isinstance(value, dict):
                    fix_string(value)
                elif isinstance(value, bytes):
                    result[key] = value.decode('utf-8')
        fix_string(self.result)

        print(self.result)

    def print_results_html(self, path: str):
        benchmark_print_results_html(self.data, bytes(path, 'utf-8'))

    def print_results_text(self, path: str):
        benchmark_print_results_text(self.data, bytes(path, 'utf-8'))


cdef class Benchmark:
    '''Wrapper around the lfkbenchmark module.
    '''

    cdef benchmark_handler_t handler
    cdef system_info_t parameters

    @staticmethod
    cdef int progress_callback_internal(void* data, int progress,const char *message) nogil:
        with gil:
            return (<Benchmark>data).__call_callback(progress, message.decode('utf-8'))

    cdef benchmark_progress_callback_handler_t callback_handler

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
        self.handler = benchmark_init()
        #self.parameters = benchmark_get_system_info(self.handler)

        self.external_callback = None

        self.callback_handler.data = <void*>self
        self.callback_handler.callback = self.progress_callback_internal

        benchmark_set_progress_callback(self.handler, self.callback_handler)
        benchmark_set_execution_time(self.handler, 0.000001)

    def __del__(self):
        benchmark_cleanup(self.handler)

    def run_benchmark(self, comment: str = None, callback: Callable[[int, str], int] = None):
        '''Run benchmark from UI.
        Warning::

            This is a blocking operation. You should be running this through a thread/async task.

        Arguments::
        
            `callback` pass a function here. Defaults to `None`.
        '''
        if not self.handler:
            raise RuntimeError("Use Benchmark within a 'with' statement.")
        if comment:
            benchmark_set_comment(self.handler, bytes(comment, 'utf-8'))

        Logger.debug('LFKBenchmark:  Setting up benchmark.')

        self.external_callback = callback

        cdef system_info_t c_system_info
        cdef benchmark_result_t c_result

        benchmark_get_system_info(self.handler, &c_system_info)

        Logger.debug('LFKBenchmark: Runing benchmark.')
        with nogil:
            benchmark_run(self.handler, &c_result)


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
        cdef system_info_t system_info
        benchmark_get_system_info(self.handler, &system_info)
        return system_info
