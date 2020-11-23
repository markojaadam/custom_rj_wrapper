//
// Created by adam on 2020. 11. 20..
//

#include "messaging.hpp"
#include "rapidjson.hpp"

rapidjson::Value &ServerResponse::paramsMember() noexcept {
  return const_cast<rapidjson::Value &>(
      static_cast<const ServerResponse *>(this)->paramsMember());
}

const rapidjson::Value &ServerResponse::paramsMember() const noexcept {
  return document.FindMember("params")->value;
}

rapidjson::Value &ClientRequest::paramsMember() noexcept {
  return const_cast<rapidjson::Value &>(
      static_cast<const ClientRequest *>(this)->paramsMember());
}

const rapidjson::Value &ClientRequest::paramsMember() const noexcept {
  return document.FindMember("params")->value;
}

void ServerResponse::setParameter(std::string param, rapidjson::Value value) {
  setMember(paramsMember(), param, std::move(value), document.GetAllocator());
}

void ServerResponse::setIntParameter(std::string param, int32_t intValue){
  setMember(paramsMember(), param, std::move(rapidjson::Value{intValue}), document.GetAllocator());
}

const rapidjson::Value& ClientRequest::getParameter(const std::string_view name) const noexcept {
  const auto nameRef = rapidjson::StringRef(name.data(), name.size());
  const auto i = paramsMember().FindMember(nameRef);
  const auto e = paramsMember().MemberEnd();
  if (i == e) {
    return core::NullJsonValue;
  } else
    return i->value;
}

int32_t ClientRequest::getIntParameter(const std::string_view name) const noexcept {
  const auto nameRef = rapidjson::StringRef(name.data(), name.size());
  const auto i = paramsMember().FindMember(nameRef);
  const auto e = paramsMember().MemberEnd();
  if (i == e) {
    return 0;
  } else
    return i->value.GetInt();
}

void ClientRequest::read(const char *message) {
  document.Parse(message);
  auto& alloc = document.GetAllocator();
  const auto i1 = document.FindMember("seq");
  const auto e1 = paramsMember().MemberEnd();
  if (i1 == e1) {
    return;
  } else {
    seq = i1->value.GetInt();
  }
  const auto i2 = document.FindMember("method");
  const auto e2 = paramsMember().MemberEnd();
  if (i1 == e1) {
    return;
  } else {
    method = i2->value.GetInt();
  }
}
ClientRequest::ClientRequest(){}
ClientRequest::~ClientRequest(){}


void ServerResponse::init(const int methodCode, const int seqValue) {
  document = rapidjson::Document{rapidjson::Type::kObjectType};
  method = methodCode;
  seq = seqValue;
  auto& alloc = document.GetAllocator();
  rapidjson::Value params = rapidjson::Value{rapidjson::kObjectType};
  document.AddMember("seq", rapidjson::Value{1}, alloc);
  document.AddMember("method", rapidjson::Value{methodCode}, alloc);
  document.AddMember("params", std::move(params), alloc);

}
ServerResponse::ServerResponse(const int32_t methodCode){
  ServerResponse::init(methodCode, 1);
}
