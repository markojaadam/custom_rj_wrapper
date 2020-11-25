//
// Created by adam on 2020. 11. 20..
//

#ifndef CPP_JSONTEST_BASICS_HPP
#define CPP_JSONTEST_BASICS_HPP

#include "rapidjson.hpp"
#include "conversions.hpp"


template <class Allocator>
static void setMember(rapidjson::Value &node, const std::string_view name,
                      rapidjson::Value value, Allocator &alloc) {
  const auto nameRef = rapidjson::StringRef(name.data(), name.size());
  if (auto member = node.FindMember(nameRef); member != node.MemberEnd())
    member->value = std::move(value);
  else
    node.AddMember(rapidjson::Value{std::string{name}, alloc}, std::move(value),
                   alloc);
}

class ServerResponse {
public:
  int method{};
  int seq{};
  rapidjson::Document document;
  explicit ServerResponse(int methodCode);
  void setParameter(std::string param, rapidjson::Value value);
  void setIntParameter(std::string param, int intValue);
  void init(int methodCode, int seqValue);
  std::string dump();

private:
  rapidjson::Value &paramsMember() noexcept;
  const rapidjson::Value &paramsMember() const noexcept;
};


class ClientRequest {
public:
  ClientRequest();
  ~ClientRequest();
  rapidjson::Document document;
  int method;
  int seq;
  const rapidjson::Value &getParameter(std::string_view name) const noexcept;
  int getIntParameter(std::string_view name) const noexcept;
  void read(const char *message);

private:
  rapidjson::Value &paramsMember() noexcept;
  const rapidjson::Value &paramsMember() const noexcept;
};

#endif // CPP_JSONTEST_BASICS_HPP