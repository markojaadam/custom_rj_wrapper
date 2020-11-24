#include "rapidjson.hpp"
#include "messaging.cpp"
#include <chrono>
#include <iostream>

int main() {

  ServerResponse response{10000};
  response.setIntParameter("blahInt", 1);
  response.setParameter("blah", rapidjson::Value("a"));
  response.setParameter("blah2", rapidjson::Value(false));
  response.setParameter("blah3", rapidjson::Value(true));
  std::cout << response.dump() << std::endl;
}