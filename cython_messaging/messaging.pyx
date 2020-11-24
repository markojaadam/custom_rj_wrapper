cimport c_messaging
from libcpp.string cimport string
from cython.operator cimport dereference as deref
import cython

@cython.final
cdef class PyRequest:
    def __cinit__(self):
        with nogil:
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


@cython.final
cdef class PyResponse:
    def __cinit__(self, int method):
        with nogil:
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
