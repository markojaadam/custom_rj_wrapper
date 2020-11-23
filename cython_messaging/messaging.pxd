cimport c_messaging
import cython
from libcpp.string cimport string

@cython.final
cdef class PyRequest():
    cdef c_messaging.ClientRequest *thisptr
    # cpdef c_messaging.Document document
    cpdef int method
    cpdef int seq
    cpdef void read(self, const char *message) nogil
    cpdef int getIntParameter(self, string name) nogil