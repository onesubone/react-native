//
// Created by hanyanan on 2017/1/22.
//

#include "V8Utils.h"
#include "V8JSExecutor.h"

namespace v8 {

inline Isolate *GetIsolate() {
    return v8::V8JSExecutor::GetIsolate();
}

Local<String> toLocalString(const char *string) {
    return String::NewFromUtf8(GetIsolate(), std::move(string));
}

Local<String> toLocalString(const std::string &string) {
    return String::NewFromUtf8(GetIsolate(), string.c_str(), NewStringType::kNormal,
                        static_cast<int>(string.length())).ToLocalChecked();
}

Local<String> toLocalString(const facebook::react::JSBigString &bigString) {
    return String::NewFromUtf8(GetIsolate(), std::move(bigString.c_str()));
}

std::string toStdString(const Local<String> &string) {
    int length = string->Length();
    char buff[length + 1];
    buff[length] = '\0';
    string->WriteUtf8(buff, length);
    return std::string(buff);
}

std::string toJsonStdString(Local<Context> context, const Local<Object> &object) {
    Local<String> jsonStr = JSON::Stringify(context, std::move(object)).ToLocalChecked();
    return toStdString(std::move(jsonStr));
}

Local<String> toJsonString(Local<Context> context, const Local<Object> &object) {
    return JSON::Stringify(context, std::move(object)).ToLocalChecked();
}

Local<Value> fromJsonString(Local<Context> context, const std::string &jsonStr) {
    Local<String> res = String::NewFromUtf8(GetIsolate(), std::move(jsonStr.c_str()));
    return JSON::Parse(context, std::move(res)).ToLocalChecked();
}

Local<Value> fromJsonString(Local<Context> context, const char *jsonStr) {
    Local<String> res = String::NewFromUtf8(GetIsolate(), std::move(jsonStr));
    return JSON::Parse(context, std::move(res)).ToLocalChecked();
}

Local<Value> fromJsonString(Local<Context> context, const Local<String> &jsonStr) {
    return JSON::Parse(context, std::move(jsonStr)).ToLocalChecked();
}
}