#include "rapidjson.hpp"
#include "wrapper.cpp"
#include <chrono>
#include <iostream>

int main() {

  using namespace std::chrono;
  auto start = high_resolution_clock::now();
  int n_cycles = 1000000;
  for (int i = 1; i < n_cycles; i++) {
    rapidjson::Document doc{rapidjson::Type::kObjectType};
    auto response = ServerResponse{};
    response.init(10000, 1);
    response.setParameter("blah", rapidjson::Value("a"));
    response.setParameter("blah2", rapidjson::Value(false));
    response.setParameter("blah3", rapidjson::Value(true));
    toJsonString(response.document);
  }
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(stop - start);
  auto duration_ns = duration_cast<nanoseconds>(stop - start);
  std::cout << "Elapsed: " << duration.count()/1000 << " sec\t";
  std::cout << "\t" << duration_ns.count()/n_cycles << "ns/op" << std::endl;

}