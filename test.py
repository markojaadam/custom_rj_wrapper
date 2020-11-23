# from messaging.messaging import new_req_from_msg, str_test
from messaging.messaging import PyRequest

testdata = """{
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

req = PyRequest()
req.read(testdata.encode())
print(req.method)
