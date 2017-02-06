//
// Created by hanyanan on 2017/1/22.
//

#ifndef V8_UTILS_H
#define V8_UTILS_H

#include "include/v8.h"
#include <string>
#include <android/log.h>
#include <folly/json.h>
#include <android/log.h>
#include <folly/Exception.h>
#include <folly/Memory.h>
#include <folly/String.h>
#include <folly/Conv.h>
#include <fcntl.h>
#include "jschelpers/Value.h"
#include "Executor.h"


#define _RN_V8_DEBUG_ 1
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO  , "V8Application", __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG  , "V8Application", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN  , "V8Application", __VA_ARGS__)

using namespace std;
using namespace facebook::react;

namespace v8 {

 Local<String> toLocalString(const char *string);

 Local<String> toLocalString(const std::string &string);

 Local<String> toLocalString(const facebook::react::JSBigString &bigString);

 std::string toStdString(const Local<String> &string);

 std::string toJsonStdString(Local<Context> context, const Local<Object> &object);

 Local<String> toJsonString(Local<Context> context, const Local<Object> &object);

 Local<Value> fromJsonString(Local<Context> context, const std::string &jsonStr);

 Local<Value> fromJsonString(Local<Context> context, const char *jsonStr);

 Local<Value> fromJsonString(Local<Context> context, const Local<String> &jsonStr);

}

#endif //V8_UTILS_H
