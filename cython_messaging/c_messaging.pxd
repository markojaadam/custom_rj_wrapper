from libcpp.string cimport string

cdef extern from "messaging.hpp":
    cdef cppclass ClientRequest:
        ClientRequest()
        int method
        int seq
        void read(const char *message) nogil
        int getIntParameter(string name) nogil

    cdef cppclass ServerResponse:
        ServerResponse(int)
        int method
        int seq
        string dump() nogil
        int setIntParameter(string name, int value) nogil
