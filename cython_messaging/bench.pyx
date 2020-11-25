from libcpp.string cimport string
from libc.stdio cimport printf
cimport cython
from cython.parallel import parallel, prange
cimport c_messaging
import time
import threading
from concurrent.futures import ThreadPoolExecutor, as_completed

cdef string TEST_RAW = b"""{
        "method": 10000,
            "seq": 3,
            "params": {
          "test_param_1": false,
              "test_param_2": null,
              "test_param_3": "what",
              "test_param_4": 4,
              "test_param_5": [
          1111,
              2222,
              3333
          ],
          "test_param_6": [
          false,
              true,
              false
          ],
          "test_param_7": [
          "aaa",
              "bbb",
              "ccc"
          ],
          "test_param_8": {
            "sub_param_1": 1,
                "sub_param_2": "blah",
                "nested_sub_param": {
              "nested1": "nested",
                  "nested2": 1,
                  "nestedList": [1, 2, 3]
            }
          }
        }
      }"""

cdef const char *TEST_DATA = TEST_RAW.c_str()

cdef string TEST_PARAM = b"test_param_4"

cdef void _conversion_test() nogil:
    req = new c_messaging.ClientRequest()
    req.read(TEST_DATA)
    cdef int ret1 = c_messaging.to[int](req.getParameter(TEST_PARAM))
    with gil:
        ret2 = c_messaging.to[string](req.getParameter(b"test_param_3"))
        print(ret1)
        print(ret2)
    del req

def conversion_test():
    with nogil:
        _conversion_test()

cdef void _parse_test() nogil:
    req = new c_messaging.ClientRequest()
    req.read(TEST_DATA)
    with gil:
        result = req.getIntParameter(TEST_PARAM)
        print(result)
    del req


cdef void _construct_test() nogil:
    resp = new c_messaging.ServerResponse(10000)
    resp.setIntParameter(TEST_PARAM, 10)
    with gil:
        print(resp.dump())
    del resp

def parse_test():
    with nogil:
        _parse_test()

def construct_test():
    with nogil:
        _construct_test()


cdef void parse_bench_unit() nogil:
    req = new c_messaging.ClientRequest()
    req.read(TEST_DATA)
    req.getIntParameter(TEST_PARAM)
    del req


cdef void construct_bench_unit() nogil:
    resp = new c_messaging.ServerResponse(10000)
    resp.setIntParameter(TEST_PARAM, 10)
    resp.dump()
    del resp

def parse_py_worker(int cycles):
    with nogil:
        for i in range(cycles):
            parse_bench_unit()

cdef void parse_omp_worker(int cycles) nogil:
    for i in range(cycles):
        parse_bench_unit()

def construct_py_worker(int cycles):
    with nogil:
        for i in range(cycles):
            construct_bench_unit()

cdef void construct_omp_worker(int cycles) nogil:
    for i in range(cycles):
        construct_bench_unit()

def test_parse_pythread(int n_cycles, int n_workers):
    start_time = time.time()
    threads = []
    for i in range(n_workers):
        t = threading.Thread(target=parse_py_worker, args=[int(n_cycles / n_workers)], daemon=True)
        threads.append(t)
    for t in threads:
        t.start()
    for t in threads:
        t.join()
    elapsed = time.time() - start_time
    return elapsed


def test_parse_tpe(int n_cycles, int n_workers):
    start_time = time.time()
    cdef int worker_run = int(n_cycles / n_workers)
    with ThreadPoolExecutor(max_workers=n_workers) as executor:
        futures = []
        for i in range(n_workers):
            futures.append(executor.submit(parse_py_worker, int(n_cycles / n_workers)))
        as_completed(futures)
    elapsed = time.time() - start_time
    return elapsed

def test_parse_omp(int n_cycles, int n_workers):
    cdef int worker_run = int(n_cycles / n_workers)
    start_time = time.time()
    with nogil, parallel(num_threads=n_workers):
        parse_omp_worker(worker_run)
    elapsed = time.time() - start_time
    return elapsed

def test_construct_pythread(int n_cycles, int n_workers):
    start_time = time.time()
    threads = []
    for i in range(n_workers):
        t = threading.Thread(target=construct_py_worker, args=[int(n_cycles / n_workers)], daemon=True)
        threads.append(t)

    for t in threads:
        t.start()
    for t in threads:
        t.join()

    elapsed = time.time() - start_time
    return elapsed

def test_construct_tpe(int n_cycles, int n_workers):
    start_time = time.time()
    cdef int worker_run = int(n_cycles / n_workers)
    with ThreadPoolExecutor(max_workers=n_workers) as executor:
        futures = []
        for i in range(n_workers):
            futures.append(executor.submit(construct_py_worker, int(n_cycles / n_workers)))
        as_completed(futures)
    elapsed = time.time() - start_time
    return elapsed


def test_construct_omp(int n_cycles, int n_workers):
    cdef int worker_run = int(n_cycles / n_workers)
    start_time = time.time()
    with nogil, parallel(num_threads=n_workers):
        construct_omp_worker(worker_run)
    elapsed = time.time() - start_time
    return elapsed