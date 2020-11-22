from libcpp.string cimport string

cdef extern from "wrapper.hpp":
    cdef cppclass ClientRequest:
        int method
        int seq
        void read(const char *message)

def str_test(string message):
    cdef const char *msg_char = message.c_str()
    return msg_char

def new_req_from_msg(string message):
    cdef ClientRequest client_request
    cdef constchar *msg_char = message.c_str()
    client_request.read(msg_char)
    return client_request.method, client_request.seq