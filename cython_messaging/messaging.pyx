cimport c_messaging
from libcpp.string cimport string
from cython.operator cimport dereference as deref

cdef class PyRequest:
    def __cinit__(self):
        self.thisptr = new c_messaging.ClientRequest()


    def __dealloc__(self):
        del self.thisptr

    cpdef void read(self, const char *message):
        self.thisptr.read(message)

    @property
    def method(self):
        return deref(&self.thisptr.method)

    @property
    def seq(self):
        return deref(&self.thisptr.seq)