from libcpp.string cimport string

cdef extern from "messaging.hpp":
    cdef cppclass ClientRequest:
        ClientRequest() nogil
        int method
        int seq
        void read(const char *message) nogil
        Value getParameter(string name) nogil
        int getIntParameter(string name) nogil

    cdef cppclass ServerResponse:
        ServerResponse(int) nogil
        int method
        int seq
        string dump() nogil
        int setIntParameter(string name, int value) nogil

cdef extern from "rapidjson.hpp" namespace "rapidjson":
    cdef cppclass Value:
        Value() nogil

    cdef cppclass GenericValue:
        GenericValue() nogil

cdef extern from "conversions.hpp" namespace "core":
    cdef ToType to[ToType](Value value) nogil
