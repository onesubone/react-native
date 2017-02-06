//
// Created by hanyanan on 2017/1/4.
//
#include <algorithm>
#include <condition_variable>
#include <mutex>
#include <sstream>
#include <string>
#include <folly/json.h>
#include <android/log.h>
#include <folly/Exception.h>
#include <folly/Memory.h>
#include <folly/String.h>
#include <folly/Conv.h>
#include <fcntl.h>
#include <sys/time.h>

#include "SystraceSection.h"
#include "JSModulesUnbundle.h"
#include "ModuleRegistry.h"
#include "V8JSExecutor.h"
#include "V8NativeModules.h"
#include "V8Utils.h"
#include "include/v8.h"
#include "include/libplatform/libplatform.h"

namespace v8 {
#define THROW_RUNTIME_ERROR(INFO) std::throw_with_nested(std::runtime_error(INFO))
#define _ISOLATE_CONTEXT_ENTER Isolate *isolate = GetIsolate(); \
    Isolate::Scope isolate_scope(isolate); \
    HandleScope handle_scope(isolate); \
    Local<Context> context = Local<Context>::New(isolate, m_context); \
    Context::Scope context_scope(context); \

Isolate *V8JSExecutor::m_isolate = nullptr;

void printType(Local<Value> value, const char *desc) {
    if (!*value) {
        LOGI("V8JSExecutor  %s is null", desc);
        return;
    }
    if (value->IsFunction()) {
        LOGI("V8JSExecutor  %s is a IsFunction", desc);
    }
    if (value->IsObject()) {
        LOGI("V8JSExecutor  %s is a object", desc);
    }
    if (value->IsArgumentsObject()) {
        LOGI("V8JSExecutor  %s is a IsArgumentsObject", desc);
    }
    if (value->IsArray()) {
        LOGI("V8JSExecutor  %s is a IsArray", desc);
    }
    if (value->IsArrayBuffer()) {
        LOGI("V8JSExecutor  %s is a IsArrayBuffer", desc);
    }
    if (value->IsAsyncFunction()) {
        LOGI("V8JSExecutor  %s is a IsAsyncFunction", desc);
    }
    if (value->IsBoolean()) {
        LOGI("V8JSExecutor  %s is a IsBoolean", desc);
    }
    if (value->IsBooleanObject()) {
        LOGI("V8JSExecutor  %s is a IsBooleanObject", desc);
    }
    if (value->IsDataView()) {
        LOGI("V8JSExecutor  %s is a IsDataView", desc);
    }
    if (value->IsDate()) {
        LOGI("V8JSExecutor  %s is a IsDate", desc);
    }
    if (value->IsExternal()) {
        LOGI("V8JSExecutor  %s is a IsExternal", desc);
    }
    if (value->IsFloat32Array()) {
        LOGI("V8JSExecutor  %s is a IsFloat32Array", desc);
    }
    if (value->IsGeneratorFunction()) {
        LOGI("V8JSExecutor  %s is a IsGeneratorFunction", desc);
    }
    if (value->IsGeneratorObject()) {
        LOGI("V8JSExecutor  %s is a IsGeneratorObject", desc);
    }
    if (value->IsInt8Array()) {
        LOGI("V8JSExecutor  %s is a IsInt8Array", desc);
    }
    if (value->IsInt16Array()) {
        LOGI("V8JSExecutor  %s is a IsInt16Array", desc);
    }
    if (value->IsFloat64Array()) {
        LOGI("V8JSExecutor  %s is a IsFloat64Array", desc);
    }
    if (value->IsInt32()) {
        LOGI("V8JSExecutor  %s is a IsInt32", desc);
    }
    if (value->IsInt32Array()) {
        LOGI("V8JSExecutor  %s is a IsInt32Array", desc);
    }
    if (value->IsMap()) {
        LOGI("V8JSExecutor  %s is a IsMap", desc);
    }
    if (value->IsMapIterator()) {
        LOGI("V8JSExecutor  %s is a IsMapIterator", desc);
    }
    if (value->IsNumber()) {
        LOGI("V8JSExecutor  %s is a IsNumber", desc);
    }
    if (value->IsNativeError()) {
        LOGI("V8JSExecutor  %s is a IsNativeError", desc);
    }
    if (value->IsNumberObject()) {
        LOGI("V8JSExecutor  %s is a IsNumberObject", desc);
    }
    if (value->IsName()) {
        LOGI("V8JSExecutor  %s is a IsName", desc);
    }
    if (value->IsNull()) {
        LOGI("V8JSExecutor  %s is a IsNull", desc);
    }
    if (value->IsPromise()) {
        LOGI("V8JSExecutor  %s is a IsPromise", desc);
    }
    if (value->IsProxy()) {
        LOGI("V8JSExecutor  %s is a IsProxy", desc);
    }
    if (value->IsSet()) {
        LOGI("V8JSExecutor  %s is a IsSet", desc);
    }
    if (value->IsSetIterator()) {
        LOGI("V8JSExecutor  %s is a IsSetIterator", desc);
    }
    if (value->IsString()) {
        LOGI("V8JSExecutor  %s is a IsString", desc);
    }
    if (value->IsStringObject()) {
        LOGI("V8JSExecutor  %s is a IsStringObject", desc);
    }
    if (value->IsSymbol()) {
        LOGI("V8JSExecutor  %s is a IsSymbol", desc);
    }
    if (value->IsSymbolObject()) {
        LOGI("V8JSExecutor  %s is a IsSymbolObject", desc);
    }
    if (value->IsTypedArray()) {
        LOGI("V8JSExecutor  %s is a IsTypedArray", desc);
    }
    if (value->IsUint8Array()) {
        LOGI("V8JSExecutor  %s is a IsUint8Array", desc);
    }
    if (value->IsUint8ClampedArray()) {
        LOGI("V8JSExecutor  %s is a IsUint8ClampedArray", desc);
    }
    if (value->IsUint16Array()) {
        LOGI("V8JSExecutor  %s is a IsUint16Array", desc);
    }
    if (value->IsUint32()) {
        LOGI("V8JSExecutor  %s is a IsUint32", desc);
    }
    if (value->IsUint32Array()) {
        LOGI("V8JSExecutor  %s is a IsUint32Array", desc);
    }
    if (value->IsUndefined()) {
        LOGI("V8JSExecutor  %s is a IsUndefined", desc);
    }
    if (value->IsWeakMap()) {
        LOGI("V8JSExecutor  %s is a IsWeakMap", desc);
    }
    if (value->IsWeakSet()) {
        LOGI("V8JSExecutor  %s is a IsWeakSet", desc);
    }
    if (value->IsWebAssemblyCompiledModule()) {
        LOGI("V8JSExecutor  %s is a IsWebAssemblyCompiledModule", desc);
    }
}

const Local<Value> fromDynamic(Isolate *isolate, v8::Local<v8::Context> context,
                               const folly::dynamic &value) {
    const std::string &json = folly::toJson(std::move(value));
    Local<Value> result;
    if (v8::JSON::Parse(context, toLocalString(std::move(json))).ToLocal(&result)) {
        return result;
    }
    return Local<Value>();
}

void nativeLog(const FunctionCallbackInfo<Value> &args) {
    android_LogPriority logLevel = ANDROID_LOG_DEBUG;
    if (args.Length() > 1) {
        int32_t level = Int32::Cast(*(args[1]))->Value();
        // The lowest log level we get from JS is 0. We shift and cap it to be
        // in the range the Android logging method expects.
        logLevel = std::min(static_cast<android_LogPriority>(level + ANDROID_LOG_DEBUG),
                            ANDROID_LOG_FATAL);
    }
    if (args.Length() > 0) {
        Local<String> log = Local<String>::Cast(args[0]);
        const std::string &str = toStdString(log);
        __android_log_print(logLevel, "ReactNativeJS", str.c_str(), NULL);
    }
}

void nativePerformanceNow(const FunctionCallbackInfo<Value> &args) {
    static const int64_t NANOSECONDS_IN_SECOND = 1000000000LL;
    static const int64_t NANOSECONDS_IN_MILLISECOND = 1000000LL;

    // This is equivalent to android.os.SystemClock.elapsedRealtime() in native
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC_RAW, &now);
    int64_t nano = now.tv_sec * NANOSECONDS_IN_SECOND + now.tv_nsec;
    args.GetReturnValue().Set((nano / (double) NANOSECONDS_IN_MILLISECOND));
}

// Native JS hooks
template<void (V8JSExecutor::*method)(const FunctionCallbackInfo<Value> &args)>
void V8JSExecutor::installNativeHook(const Handle<ObjectTemplate> &global, const char *name) {
    struct funcWrapper {
        static void call(const v8::FunctionCallbackInfo<v8::Value> &args) {
            Isolate *isolate = args.GetIsolate();
            HandleScope handle_scope(isolate);
            Local<Context> context = isolate->GetCurrentContext();
            auto ptr = context->GetAlignedPointerFromEmbedderData(1);
            V8JSExecutor *executor = static_cast<V8JSExecutor *>(ptr);
            if (!executor) {
                THROW_RUNTIME_ERROR("Get Empty Context in installNativeHook");
            }
            (executor->*method)(std::move(args));
        }
    };
    global->Set(toLocalString(name), FunctionTemplate::New(GetIsolate(), funcWrapper::call));
}

// Native Static JS hooks
template<void (*method)(const v8::FunctionCallbackInfo<Value> &args)>
inline void installGlobalNativeHook(const Handle<ObjectTemplate> &global, const char *name) {
    global->Set(toLocalString(name), FunctionTemplate::New(V8JSExecutor::GetIsolate(), method));
}

template<Global<Value> (V8JSExecutor::*method)(Local<String> property,
                                               const PropertyCallbackInfo<Value> &info)>
void V8JSExecutor::installNativeHandlerHook(Handle<ObjectTemplate> &global, const char *name) {
    struct funcWrapper {
        static void call(Local<Name> property, const PropertyCallbackInfo<Value> &info) {
            Isolate *isolate = info.GetIsolate();
            HandleScope handle_scope(isolate);
            Local<Context> context = isolate->GetCurrentContext();
            auto ptr = context->GetAlignedPointerFromEmbedderData(1);
            V8JSExecutor *executor = static_cast<V8JSExecutor *>(ptr);
            if (!executor) {
                THROW_RUNTIME_ERROR("Get Empty Context in installNativeHandlerHook");
            }
            Global<Value> res = (executor->*method)(Local<String>::Cast(property), std::move(info));
            info.GetReturnValue().Set(std::move(res));
        }
    };
    Local<ObjectTemplate> nativeModuleProxyTemplate = ObjectTemplate::New(GetIsolate());
    NamedPropertyHandlerConfiguration configuration(funcWrapper::call);
    nativeModuleProxyTemplate->SetHandler(configuration);
    global->Set(toLocalString(name), nativeModuleProxyTemplate);
}

Isolate *V8JSExecutor::GetIsolate() {
    if (m_isolate) {
        return m_isolate;
    }
    LOGI("Create Isolate Step1");

    Platform *platform = platform::CreateDefaultPlatform();
    V8::InitializePlatform(platform);
    V8::Initialize();

    // Create a new Isolate and make it the current one.
    Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    Isolate *isolate = Isolate::New(create_params);
    m_isolate = isolate;
    LOGI("Create Isolate Step2");
    return isolate;
}

void *V8JSExecutor::getJavaScriptContext() {
    return this;
}

bool V8JSExecutor::supportsProfiling() {
    LOGI("V8JSExecutor::supportsProfiling");
#ifdef WITH_FBSYSTRACE
    return true;
#else
    return false;
#endif
}

V8JSExecutor::V8JSExecutor(std::shared_ptr<ExecutorDelegate> delegate,
                           std::shared_ptr<MessageQueueThread> messageQueueThread,
                           const std::string &cacheDir,
                           const folly::dynamic &jscConfig) throw(JSException) :
        m_delegate(delegate),
        m_deviceCacheDir(cacheDir),
        m_messageQueueThread(messageQueueThread),
        m_nativeModules(delegate ? delegate->getModuleRegistry() : nullptr),
        m_jscConfig(jscConfig) {
    initOnJSVMThread();
}

V8JSExecutor::~V8JSExecutor() {
    CHECK(*m_isDestroyed) << "V8JSExecutor::destroy() must be called before its destructor!";
}

void V8JSExecutor::destroy() {
    *m_isDestroyed = true;
    if (m_messageQueueThread.get()) {
        m_messageQueueThread->runOnQueueSync([this]() {
            terminateOnJSVMThread();
        });
    } else {
        terminateOnJSVMThread();
    }
}

void V8JSExecutor::initOnJSVMThread() throw(JSException) {
    SystraceSection s("V8JSExecutor.initOnJSVMThread");
    LOGI("V8JSExecutor.initOnJSVMThread");
    Isolate *isolate = GetIsolate();
    Isolate::Scope isolate_scope(isolate);
    HandleScope handle_scope(isolate);
    Local<ObjectTemplate> global = ObjectTemplate::New(isolate);
    // Bind the global 'print' function to the C++ Print callback.
    installNativeHook<&V8JSExecutor::nativeFlushQueueImmediate>(global,
                                                                "nativeFlushQueueImmediate");
    installNativeHook<&V8JSExecutor::nativeCallSyncHook>(global, "nativeCallSyncHook");
    // Websorker support
    installNativeHook<&V8JSExecutor::nativeStartWorker>(global, "nativeStartWorker");
    installNativeHook<&V8JSExecutor::nativePostMessageToWorker>(global,
                                                                "nativePostMessageToWorker");
    installNativeHook<&V8JSExecutor::nativeTerminateWorker>(global, "nativeTerminateWorker");
    // native require
    installNativeHook<&V8JSExecutor::nativeRequire>(global, "nativeRequire");
    installNativeHandlerHook<&V8JSExecutor::getNativeModule>(global, "nativeModuleProxy");
    installGlobalNativeHook<&nativeLog>(global, "nativeLoggingHook");
    installGlobalNativeHook<&nativePerformanceNow>(global, "nativePerformanceNow");

    Local<Context> context = Context::New(isolate, NULL, global);
    context->SetAlignedPointerInEmbedderData(1, this);
    m_context.Reset(GetIsolate(), context);
    LOGI("V8JSExecutor.initOnJSVMThread Finished!");
}

void V8JSExecutor::terminateOnJSVMThread() {
    m_nativeModules.reset();
    m_context.Reset();
    m_invokeCallbackAndReturnFlushedQueueJS.Reset();
    m_callFunctionReturnFlushedQueueJS.Reset();
    m_flushedQueueJS.Reset();
    m_callFunctionReturnResultAndFlushedQueueJS.Reset();
}

void V8JSExecutor::executeScript(Local<Context> context,
                                 const Local<String> &script) throw(JSException) {
    Isolate *isolate = GetIsolate();
    TryCatch try_catch(isolate);
    Local<Script> compiled_script;
    if (!Script::Compile(context, std::move(script)).ToLocal(&compiled_script)) {
        String::Utf8Value error(try_catch.Exception());
        // The script failed to compile; bail out.
        THROW_RUNTIME_ERROR("Error ExecuteScript while compile script!");
    }

    // Run the script!
    Local<Value> result;
    if (!compiled_script->Run(context).ToLocal(&result)) {
        // The TryCatch above is still in effect and will have caught the error.
        String::Utf8Value error(try_catch.Exception());
        LOGW("compiled_script->Run error: %s", *error);
        THROW_RUNTIME_ERROR("Error ExecuteScript while running script!");
    }
}

void V8JSExecutor::loadApplicationScript(std::unique_ptr<const JSBigString> script,
                                         std::string sourceURL) throw(JSException) {
    SystraceSection s("V8JSExecutor::loadApplicationScript", "sourceURL", sourceURL);
    _ISOLATE_CONTEXT_ENTER;
    executeScript(context, std::move(toLocalString(std::move(*script))));
    if (m_delegate) {
        bindBridge();
        flush();
    }
}

void V8JSExecutor::loadApplicationScript(std::string bundlePath, std::string source,
                                         int flags) throw(JSException) {
    SystraceSection s("V8JSExecutor::loadApplicationScript", "bundlePath", bundlePath,
                      ", source", source);
    THROW_RUNTIME_ERROR("Error ExecuteScript while running script! Not Support!!!");
}

void V8JSExecutor::bindBridge() throw(JSException) {
    SystraceSection s("V8JSExecutor::bindBridge");
    _ISOLATE_CONTEXT_ENTER;
    Local<Object> globalObj = context->Global();
    Local<Value> batchedBridgeValue; // batchedBridgeValue;
    if (!globalObj->Get(context, toLocalString("__fbBatchedBridge")).ToLocal(&batchedBridgeValue)) {
        LOGI("V8JSExecutor::bindBridge obj __fbBatchedBridge is empty");
        THROW_RUNTIME_ERROR("Not Found __fbBatchedBridge Global Data!!!");
    }
    Local<Object> batchedBridge = Local<Object>::Cast(batchedBridgeValue);
    auto funcSet = [&](const char *name, Global<Function> &globalFunc) mutable {
        Local<Function> localFunc = Local<Function>::Cast(batchedBridge->Get(toLocalString(name)));
        globalFunc.Reset(isolate, localFunc);
    };
    funcSet("callFunctionReturnFlushedQueue", m_callFunctionReturnFlushedQueueJS);
    funcSet("invokeCallbackAndReturnFlushedQueue", m_invokeCallbackAndReturnFlushedQueueJS);
    funcSet("flushedQueue", m_flushedQueueJS);
    funcSet("callFunctionReturnResultAndFlushedQueue", m_callFunctionReturnResultAndFlushedQueueJS);
}

void V8JSExecutor::callNativeModules(Local<Context> context, Local<Value> value) {
    SystraceSection s("V8JSExecutor::callNativeModules");
    try {
        if (value->IsObject()) {
            Local<Object> obj = Local<Object>::Cast(value);
            const std::string &arg = toJsonStdString(context, std::move(obj));
            m_delegate->callNativeModules(*this, folly::parseJson(std::move(arg)), true);
        } else {
            LOGI("V8JSExecutor::callNativeModules arg NULL");
//            m_delegate->callNativeModules(*this, folly::parseJson(std::string("")), true);
        }
    } catch (...) {
        std::string message = "Error in callNativeModules()";
//        try {
//            message += ":" + value.toString().str();
//        } catch (...) {
//            // ignored
//        }
        std::throw_with_nested(std::runtime_error(message));
    }
}

void V8JSExecutor::flush() {
    SystraceSection s("V8JSExecutor::flush");
    _ISOLATE_CONTEXT_ENTER;
    Local<Function> flushedQueueJS = Local<Function>::New(isolate, m_flushedQueueJS);
    callNativeModules(context,
                      flushedQueueJS->Call(context, flushedQueueJS, 0, {}).ToLocalChecked());
}

void V8JSExecutor::setJSModulesUnbundle(std::unique_ptr<JSModulesUnbundle> unbundle) {
    m_unbundle = std::move(unbundle);
}

void V8JSExecutor::callFunction(const std::string &moduleId, const std::string &methodId,
                                const folly::dynamic &arguments) {
    SystraceSection s("V8JSExecutor::callFunction");
    LOGD("V8JSExecutor::callFunction moduleId: %s, methodId:%s", moduleId.c_str(),
         methodId.c_str());
    _ISOLATE_CONTEXT_ENTER;
    Local<Function> localFunc = Local<Function>::New(isolate, m_callFunctionReturnFlushedQueueJS);
    Local<String> localModuleName = toLocalString(moduleId);
    Local<String> localMethodId = toLocalString(methodId);
    const std::string &json = folly::toJson(arguments);
    Local<String> json_str = toLocalString(std::move(json));
    Local<Value> localArguments = JSON::Parse(context, json_str).ToLocalChecked();
    Local<Value> argv[3] = {localModuleName, localMethodId, localArguments};
    Local<Value> result = localFunc->Call(context, localFunc, 3, argv).ToLocalChecked();
    callNativeModules(context, result);
}

void V8JSExecutor::invokeCallback(const double callbackId, const folly::dynamic &arguments) {
    SystraceSection s("V8JSExecutor::invokeCallback");
    _ISOLATE_CONTEXT_ENTER;
    Local<Function> invokeFunc = Local<Function>::New(isolate,
                                                      m_invokeCallbackAndReturnFlushedQueueJS);
    Local<Number> localCallbackId = Number::New(isolate, callbackId);
    Local<Value> localArguments = fromDynamic(isolate, context, arguments);
    Local<Value> argv[2] = {localCallbackId, localArguments};
    Local<Value> result = invokeFunc->Call(context, context->Global(), 2, argv).ToLocalChecked();
    callNativeModules(context, result);
}

void V8JSExecutor::setGlobalVariable(std::string propName,
                                     std::unique_ptr<const JSBigString> jsonValue) {
    try {
        SystraceSection s("V8JSExecutor.setGlobalVariable", "propName", propName);
        _ISOLATE_CONTEXT_ENTER;
        Local<String> propNameString = toLocalString(propName);
        Local<Value> attribute = fromJsonString(context, toLocalString(*jsonValue));
        context->Global()->Set(context, propNameString, attribute);
    } catch (...) {
        std::throw_with_nested(std::runtime_error("Error setting global variable: " + propName));
    }
}

void V8JSExecutor::startProfiler(const std::string &titleString) {
    LOGI("V8JSExecutor.startProfiler titleString:%s", titleString.c_str());
}

void V8JSExecutor::stopProfiler(const std::string &titleString, const std::string &filename) {
    LOGI("V8JSExecutor.stopProfiler titleString:%s, filename:%s", titleString.c_str(),
         filename.c_str());
}

void V8JSExecutor::handleMemoryPressureUiHidden() {
    LOGI("V8JSExecutor.handleMemoryPressureUiHidden");
    #ifdef WITH_JSC_MEMORY_PRESSURE
    // JSHandleMemoryPressure(this, m_context, JSMemoryPressure::UI_HIDDEN);
    #endif
}

void V8JSExecutor::handleMemoryPressureModerate() {
    LOGI("V8JSExecutor.handleMemoryPressureModerate");
}

void V8JSExecutor::handleMemoryPressureCritical() {
    LOGI("V8JSExecutor.handleMemoryPressureCritical");
}

void V8JSExecutor::nativeStartWorker(const v8::FunctionCallbackInfo<v8::Value> &args) {
    LOGI("V8JSExecutor.nativeStartWorker");
}

void V8JSExecutor::nativePostMessageToWorker(const v8::FunctionCallbackInfo<v8::Value> &args) {
    LOGI("V8JSExecutor.nativePostMessageToWorker");
}

void V8JSExecutor::nativeTerminateWorker(const v8::FunctionCallbackInfo<v8::Value> &args) {
    LOGI("V8JSExecutor.nativeTerminateWorker");
}

void V8JSExecutor::nativePostMessage(const v8::FunctionCallbackInfo<v8::Value> &args) {
    LOGI("V8JSExecutor.nativePostMessage");
}

void V8JSExecutor::nativeRequire(const v8::FunctionCallbackInfo<v8::Value> &args) {
    if (args.Length() != 1) {
        throw std::invalid_argument("Got wrong number of args");
    }
    _ISOLATE_CONTEXT_ENTER;
    Local<Uint32> id = Local<Uint32>::Cast(args[0]);
    uint32_t moduleId = id->Value();
    LOGD("V8JSExecutor.nativeRequire moduleId %u", moduleId);
    auto module = m_unbundle->getModule(moduleId);
    const std::string &source = module.code;
    executeScript(context, std::move(toLocalString(std::move(source))));
}

void V8JSExecutor::nativeFlushQueueImmediate(const v8::FunctionCallbackInfo<v8::Value> &args) {
    if (args.Length() != 1) {
        throw std::invalid_argument("Got wrong number of args");
    }
    _ISOLATE_CONTEXT_ENTER;
    const std::string &queueStr = toJsonStdString(context, Local<Object>::Cast(args[0]));
    m_delegate->callNativeModules(*this, folly::parseJson(std::move(queueStr)), false);
}

void V8JSExecutor::nativeCallSyncHook(const v8::FunctionCallbackInfo<v8::Value> &args) {
    LOGI("V8JSExecutor.nativeCallSyncHook");
    if (args.Length() != 3) {
        throw std::invalid_argument("Got wrong number of args");
    }
    Local<Context> context = Local<Context>::New(GetIsolate(), m_context);
    uint32_t moduleId = Uint32::Cast(*(args[0]))->Value();
    uint32_t methodId = Uint32::Cast(*(args[1]))->Value();
    const std::string &argsJson = toJsonStdString(context, Local<Object>::Cast(args[2]));
    MethodCallResult result = m_delegate->callSerializableNativeHook(*this, moduleId, methodId,
                                                                     std::move(argsJson));
    if (result.isUndefined) {
        return;
    }

    args.GetReturnValue().Set(fromDynamic(GetIsolate(), context, result.result));
}

Global<Value> V8JSExecutor::getNativeModule(Local<String> property,
                                            const PropertyCallbackInfo<Value> &info) {
    SystraceSection s("V8JSExecutor.getNativeModule");
    Isolate *isolate = GetIsolate();
    HandleScope handle_scope(isolate);
    v8::Local<v8::Context> context = v8::Local<v8::Context>::New(GetIsolate(), m_context);
    const std::string &pro = toStdString(property);
    if ("name" == pro) {
        return Global<Value>(isolate, toLocalString("NativeModules"));
    }
    return m_nativeModules.getModule(isolate, context, pro);
}
}