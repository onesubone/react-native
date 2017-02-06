//
// Created by hanyanan on 2017/1/18.
//
#include <folly/Optional.h>
#include <folly/Exception.h>
#include <folly/Memory.h>
#include <folly/String.h>
#include <folly/Conv.h>
#include <folly/json.h>
#include <memory>
#include <string>
#include "include/v8.h"
#include "V8NativeModules.h"
#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO  , "V8Application", __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG  , "V8Application", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN  , "V8Application", __VA_ARGS__)

using namespace facebook::react;
using namespace std;
using namespace folly;

namespace v8 {
auto newFromChar = [&](Isolate *isolate, const char *body) {
    return String::NewFromUtf8(isolate, body);
};
void printType1(Local<Value> value, const char *desc) {
    if (!*value) {
        LOGI("V8JSExecutor::bindBridge %s is null", desc);
        return;
    }
    if (value->IsFunction()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsFunction", desc);
    }
    if (value->IsObject()) {
        LOGI("V8JSExecutor::bindBridge %s is a object", desc);
    }
    if (value->IsArgumentsObject()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsArgumentsObject", desc);
    }
    if (value->IsArray()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsArray", desc);
    }
    if (value->IsArrayBuffer()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsArrayBuffer", desc);
    }
    if (value->IsAsyncFunction()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsAsyncFunction", desc);
    }
    if (value->IsBoolean()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsBoolean", desc);
    }
    if (value->IsBooleanObject()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsBooleanObject", desc);
    }
    if (value->IsDataView()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsDataView", desc);
    }
    if (value->IsDate()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsDate", desc);
    }
    if (value->IsExternal()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsExternal", desc);
    }
    if (value->IsFloat32Array()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsFloat32Array", desc);
    }
    if (value->IsGeneratorFunction()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsGeneratorFunction", desc);
    }
    if (value->IsGeneratorObject()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsGeneratorObject", desc);
    }
    if (value->IsInt8Array()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsInt8Array", desc);
    }
    if (value->IsInt16Array()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsInt16Array", desc);
    }
    if (value->IsFloat64Array()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsFloat64Array", desc);
    }
    if (value->IsInt32()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsInt32", desc);
    }
    if (value->IsInt32Array()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsInt32Array", desc);
    }
    if (value->IsMap()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsMap", desc);
    }
    if (value->IsMapIterator()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsMapIterator", desc);
    }
    if (value->IsNumber()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsNumber", desc);
    }
    if (value->IsNativeError()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsNativeError", desc);
    }
    if (value->IsNumberObject()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsNumberObject", desc);
    }
    if (value->IsName()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsName", desc);
    }
    if (value->IsNull()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsNull", desc);
    }
    if (value->IsPromise()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsPromise", desc);
    }
    if (value->IsProxy()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsProxy", desc);
    }
    if (value->IsSet()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsSet", desc);
    }
    if (value->IsSetIterator()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsSetIterator", desc);
    }
    if (value->IsString()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsString", desc);
    }
    if (value->IsStringObject()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsStringObject", desc);
    }
    if (value->IsSymbol()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsSymbol", desc);
    }
    if (value->IsSymbolObject()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsSymbolObject", desc);
    }
    if (value->IsTypedArray()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsTypedArray", desc);
    }
    if (value->IsUint8Array()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsUint8Array", desc);
    }
    if (value->IsUint8ClampedArray()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsUint8ClampedArray", desc);
    }
    if (value->IsUint16Array()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsUint16Array", desc);
    }
    if (value->IsUint32()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsUint32", desc);
    }
    if (value->IsUint32Array()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsUint32Array", desc);
    }
    if (value->IsUndefined()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsUndefined", desc);
    }
    if (value->IsWeakMap()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsWeakMap", desc);
    }
    if (value->IsWeakSet()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsWeakSet", desc);
    }
    if (value->IsWebAssemblyCompiledModule()) {
        LOGI("V8JSExecutor::bindBridge %s is a IsWebAssemblyCompiledModule", desc);
    }
}

inline std::string toStdString1(Local<String> data) {
    int length = data->Length();
    char buff[length + 1];
    buff[length] = '\0';
    data->WriteUtf8(buff, length);
    return std::string(buff);
}

V8NativeModules::V8NativeModules(std::shared_ptr<ModuleRegistry> moduleRegistry) :
        m_moduleRegistry(std::move(moduleRegistry)) { }

Global<Value> V8NativeModules::getModule(Isolate *isolate, Local<Context> context, const std::string &moduleName) {
    LOGI("V8NativeModules::getModule moduleName %s", moduleName.c_str());
    std::unordered_map<std::string, Global<Value>>::const_iterator it = m_objects.find(moduleName);
//    if (it != m_objects.end()) {
//        const Global<Value> result = it->second;
//        return result.Get(isolate);
//    }

    Local<Value> module = createModule(isolate, context, moduleName);
    if (module.IsEmpty()) {
        return Global<Value>();
    }
    Global<Value> globalValue;
    globalValue.Reset(isolate, module);
//    m_objects.emplace(moduleName, globalValue);
    return globalValue;
}

Local<Value> V8NativeModules::createModule(Isolate *isolate, Local<Context> context, const std::string &name) {
    LOGI("V8NativeModules::createModule moduleName %s", name.c_str());
    if (m_genNativeModuleJS.IsEmpty()) {
        Local<Object> globalObj = context->Global();
        Local<Value> fbGenNativeModuleValue; // batchedBridgeValue;
        if (!globalObj->Get(context, String::NewFromUtf8(isolate, "__fbGenNativeModule")).ToLocal(
                &fbGenNativeModuleValue)) {
            // TODO
        }
        LOGI("V8NativeModules::createModule fbGenNativeModuleValue %d", (int)*fbGenNativeModuleValue);
        Local<Function> fbGenNativeModule = Local<Function>::Cast(fbGenNativeModuleValue);
        LOGI("V8NativeModules::createModule fbGenNativeModule %d", (int)*fbGenNativeModule);
        m_genNativeModuleJS.Reset(isolate, fbGenNativeModule);
        // Initialize the module name list, otherwise getModuleConfig won't work
        // TODO (pieterdb): fix this in ModuleRegistry
        m_moduleRegistry->moduleNames();
    }
    LOGI("V8NativeModules::createModule m_moduleRegistry->getConfig");
    auto result = m_moduleRegistry->getConfig(name);
    if (!result.hasValue()) {
        return Local<Value>();
    }

    Local<Function> genNativeModuleJS = Local<Function>::New(isolate, m_genNativeModuleJS);

    Local<Integer> moduleId = Integer::NewFromUnsigned(isolate, result->index);
    const std::string &json = folly::toJson(result->config);
    Local<String> config = Local<String>::Cast(v8::JSON::Parse(isolate, String::NewFromUtf8(isolate, json.c_str())).ToLocalChecked());
    Local<Value> argv[2] = {config, moduleId};
    LOGI("V8NativeModules::createModule moduleId %d, config: %s", (int)result->index, json.c_str());
    Local<Value> res = genNativeModuleJS->Call(context, context->Global(), 2, argv).ToLocalChecked();
    LOGI("V8NativeModules::createModule after genNativeModuleJS->Call, res %d", (int)*res);
    printType1(res, "genNativeModuleJS->Call");
    CHECK(!res.IsEmpty()) << "Module returned from genNativeModule is null";
    Local<Object> obj = Local<Object>::Cast(res);
    LOGI("V8NativeModules::createModule obj %d", (int)*obj);

    Local<Array> localArray = obj->GetPropertyNames(context).ToLocalChecked();
    uint32_t length = localArray->Length();
    LOGI("V8NativeModules::createModule localArray count %d", length);
    for (uint32_t i = 0; i < length; ++i) {
        Local<Value> v = localArray->Get(i);
        Local<String> st = Local<String>::Cast(v);
        const std::string &log = toStdString1(st);
        LOGI("V8NativeModules::createModule index %d, PropertyNames %s ", (int) i, log.c_str());
    }
    Local<Value> ressss = Local<Object>::Cast(res)->Get(context, newFromChar(isolate, "module")).ToLocalChecked();
    printType1(ressss, "genNativeModuleJS ressss");
    return Local<Object>::Cast(ressss);
}

void V8NativeModules::reset() {
    // TODO
    //    m_genNativeModuleJS = nullptr;
    //    m_objects.clear();
}
}