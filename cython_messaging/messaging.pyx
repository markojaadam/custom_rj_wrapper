cimport c_messaging
from libcpp.string cimport string
from libc.stdio cimport printf, sprintf, getchar
from cython.operator cimport dereference as deref
from cython.parallel import parallel, prange, threadid
import cython
from cython.parallel cimport parallel
cimport openmp

@cython.final
cdef class PyRequest:
    def __cinit__(self):
        self.thisptr = new c_messaging.ClientRequest()

    def __dealloc__(self):
        del self.thisptr

    cpdef void read(self, const char *message) nogil:
        self.thisptr.read(message)

    cpdef int getIntParameter(self, string name) nogil:
        return self.thisptr.getIntParameter(name)

    @property
    def method(self):
        return deref(&self.thisptr.method)

    @property
    def seq(self):
        return deref(&self.thisptr.seq)

    @cython.boundscheck(False)
    @cython.wraparound(False)
    def test(self, const char *message, string test_param, int n_cycles):
        cdef int i
        for i in prange(n_cycles, nogil=True):
            self.read(message)
            self.getIntParameter(test_param)

@cython.final
cdef class PyResponse:
    def __cinit__(self, int method):
        self.thisptr = new c_messaging.ServerResponse(method)

    def __dealloc__(self):
        del self.thisptr

    cpdef string dump(self) nogil:
        return self.thisptr.dump()

    cpdef void setIntParameter(self, string name, int value) nogil:
        self.thisptr.setIntParameter(name, value)

    @property
    def method(self):
        return deref(&self.thisptr.method)

    @property
    def seq(self):
        return deref(&self.thisptr.seq)

#############################################
################ TEST MODULE ################
#############################################

import time
import threading

cdef void omp_worker(int cycles, string testdata, string test_param) nogil:
    with gil:
        req = PyRequest()
    for i in range(cycles):
        req.read(testdata.c_str())
        req.getIntParameter(test_param)

def py_worker(int cycles):
    cdef string testdata = b"""{
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
    cdef string test_param = b"test_param_4"
    cdef PyRequest req = PyRequest()
    with nogil:
        for i in range(cycles):
            req.read(testdata.c_str())
            req.getIntParameter(test_param)

def test_parse_pythread(int n_cycles, int n_workers):
    start_time = time.time()
    threads = []
    for i in range(n_workers):
        t = threading.Thread(target=py_worker, args=[int(n_cycles / n_workers)], daemon=True)
        threads.append(t)

    for t in threads:
        t.start()
    for t in threads:
        t.join()

    elapsed = time.time() - start_time
    return elapsed

from concurrent.futures import ThreadPoolExecutor, as_completed
def test_parse_tpe(int n_cycles, int n_workers):
    start_time = time.time()
    cdef int worker_run = int(n_cycles / n_workers)
    with ThreadPoolExecutor(max_workers=n_workers) as executor:
        futures = []
        for i in range(n_workers):
            futures.append(executor.submit(py_worker, int(n_cycles / n_workers)))
        as_completed(futures)
        # print(future.result())
    elapsed = time.time() - start_time
    return elapsed

def test_parse_omp(int n_cycles, int n_workers):
    cdef string testdata = b"""{
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
    cdef string test_param = b"test_param_4"
    cdef int num_threads
    cdef int i
    cdef int worker_run = int(n_cycles / n_workers)
    start_time = time.time()
    with nogil, parallel(num_threads=n_workers):
        omp_worker(worker_run, testdata, test_param)
    elapsed = time.time() - start_time
    return elapsed


def response_test():
    cdef int metcode = 10000
    resp = PyResponse(metcode)
    resp.setIntParameter(b"test_param_1", 10)
    print(resp.dump())