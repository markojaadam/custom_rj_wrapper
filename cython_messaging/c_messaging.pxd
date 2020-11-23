cdef extern from "messaging.hpp":
    cdef cppclass ClientRequest:
        ClientRequest()
        int method
        int seq
        void read(const char *message)
