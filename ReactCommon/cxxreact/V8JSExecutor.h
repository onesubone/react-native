//
// Created by hanyanan on 2017/1/4.
//
#ifndef DEMO_V8JSEXECUTOR_H
#define DEMO_V8JSEXECUTOR_H

#define RN_V8JS_EXECUTOR_EXPORT __attribute__((visibility("default")))

//#include <unordered_map>
//
//#include <folly/json.h>
////#include <folly/Optional.h>
//
#include <string>

#include "Executor.h"
#include "jschelpers/Value.h"
#include "MessageQueueThread.h"
#include "V8NativeModules.h"
#include "include/v8.h"

using namespace std;
using namespace facebook::react;

namespace v8 {
class RN_V8JS_EXECUTOR_EXPORT V8JSExecutor : public facebook::react::JSExecutor {
public:
    /**
     * Must be invoked from thread this Executor will run on.
     */
    explicit V8JSExecutor(std::shared_ptr<ExecutorDelegate> delegate,
                          std::shared_ptr<MessageQueueThread> messageQueueThread,
                          const std::string &cacheDir,
                          const folly::dynamic &jscConfig) throw(JSException);

    virtual ~V8JSExecutor() override;

    /**
     * Execute an application script bundle in the JS context.
     */
    virtual void loadApplicationScript(std::unique_ptr<const JSBigString> script,
                                       std::string sourceURL) throw(JSException);

    /**
     * Execute an application script optimized bundle in the JS context.
     */
    virtual void loadApplicationScript(std::string bundlePath, std::string source,
                                       int flags) throw(JSException);

    /**
     * Add an application "unbundle" file
     */
    virtual void setJSModulesUnbundle(std::unique_ptr<JSModulesUnbundle> bundle);

    /**
     * Executes BatchedBridge.callFunctionReturnFlushedQueue with the module ID,
     * method ID and optional additional arguments in JS. The executor is responsible
     * for using Bridge->callNativeModules to invoke any necessary native modules methods.
     */
    virtual void callFunction(const std::string &moduleId, const std::string &methodId,
                              const folly::dynamic &arguments);

    /**
     * Executes BatchedBridge.invokeCallbackAndReturnFlushedQueue with the cbID,
     * and optional additional arguments in JS and returns the next queue. The executor
     * is responsible for using Bridge->callNativeModules to invoke any necessary
     * native modules methods.
     */
    virtual void invokeCallback(const double callbackId, const folly::dynamic &arguments);

    virtual void setGlobalVariable(std::string propName,
                                   std::unique_ptr<const JSBigString> jsonValue);

    virtual void *getJavaScriptContext();

    virtual bool supportsProfiling();

    virtual void startProfiler(const std::string &titleString);

    virtual void stopProfiler(const std::string &titleString, const std::string &filename);

    virtual void handleMemoryPressureUiHidden();

    virtual void handleMemoryPressureModerate();

    virtual void handleMemoryPressureCritical();

    virtual void destroy();

    /**
     * global data
     */
    static Isolate *GetIsolate();

private:
    std::shared_ptr<ExecutorDelegate> m_delegate;
    std::shared_ptr<bool> m_isDestroyed = std::shared_ptr<bool>(new bool(false));
    std::string m_deviceCacheDir;
    Global<v8::Context> m_context;
    std::shared_ptr<MessageQueueThread> m_messageQueueThread;
    std::unique_ptr<JSModulesUnbundle> m_unbundle;
    V8NativeModules m_nativeModules;
    folly::dynamic m_jscConfig;

    template<void (V8JSExecutor::*method)(const v8::FunctionCallbackInfo<v8::Value> &args)>
    void installNativeHook(const Handle<ObjectTemplate> &, const char *name);

    template<Global<Value> (V8JSExecutor::*method)(Local<String> property,
                                                   const PropertyCallbackInfo<Value> &info)>
    void installNativeHandlerHook(Handle<v8::ObjectTemplate> &global, const char *name);

    void initOnJSVMThread() throw(JSException);

    void terminateOnJSVMThread();

    void bindBridge() throw(JSException);

    Global<Function> m_invokeCallbackAndReturnFlushedQueueJS;
    Global<Function> m_callFunctionReturnFlushedQueueJS;
    Global<Function> m_flushedQueueJS;
    Global<Function> m_callFunctionReturnResultAndFlushedQueueJS;

    void nativeStartWorker(const FunctionCallbackInfo<Value> &args);

    void nativePostMessageToWorker(const FunctionCallbackInfo<Value> &args);

    void nativeTerminateWorker(const FunctionCallbackInfo<Value> &args);

    void nativePostMessage(const FunctionCallbackInfo<Value> &args);

    void nativeRequire(const FunctionCallbackInfo<Value> &args);

    void nativeFlushQueueImmediate(const FunctionCallbackInfo<Value> &args);

    void nativeCallSyncHook(const FunctionCallbackInfo<Value> &args);

    Global<Value> getNativeModule(Local<String> property, const PropertyCallbackInfo<Value> &info);

    void flush();

    void callNativeModules(Local<Context> context, Local<Value> value);

    void executeScript(Local<Context> context, const Local<String> &script) throw(JSException);

    static Isolate *m_isolate;
};
}

#endif //DEMO_V8JSEXECUTOR_H
