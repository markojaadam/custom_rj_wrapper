#include "rapidjson.hpp"
#include "messaging.cpp"
#include <chrono>
#include <iostream>

int main() {
  constexpr const char *msg = R"({
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
  }
)";
  using namespace std::chrono;
  auto start = high_resolution_clock::now();
  int n_cycles = 1000000;
  for (int i = 1; i < n_cycles; i++) {
    ClientRequest request;
    request.read(msg);
    request.getParameter("test_param_1");
    request.getParameter("test_param_2");
    request.getParameter("test_param_4");
  }

  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(stop - start);
  auto duration_ns = duration_cast<nanoseconds>(stop - start);
  std::cout << "Elapsed: " << duration.count()/1000 << " sec\t";
  std::cout << "\t" << duration_ns.count()/n_cycles << "ns/op" << std::endl;

}