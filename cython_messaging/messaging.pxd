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

@cython.final
cdef class PyResponse():
    cdef c_messaging.ServerResponse *thisptr
    cpdef int method
    cpdef int seq
    cpdef string dump(self) nogil
    cpdef void setIntParameter(self, string name, int value) nogil