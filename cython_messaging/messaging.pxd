cimport c_messaging
# from libcpp.string cimport string
#
cdef class PyRequest():
    cdef c_messaging.ClientRequest *thisptr
    # cpdef c_messaging.Document document
    cpdef int method
    cpdef int seq
    cpdef void read(self, const char *message)