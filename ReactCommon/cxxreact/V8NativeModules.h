//
// Created by hanyanan on 2017/1/18.
//
#ifndef DEMO_V8NATIVEMODULES_H
#define DEMO_V8NATIVEMODULES_H

#include <folly/Optional.h>
#include <memory>
#include <string>
#include "ModuleRegistry.h"
#include "include/v8.h"

using namespace facebook::react;
using namespace std;

namespace v8 {
class V8NativeModules {
public:
    explicit V8NativeModules(std::shared_ptr<ModuleRegistry> moduleRegistry);

    Global<Value> getModule(Isolate *isolate, Local<Context> context, const std::string &moduleName);

    void reset();

private:
    std::shared_ptr<ModuleRegistry> m_moduleRegistry;
    Global<Function> m_genNativeModuleJS;
    std::unordered_map<std::string, Global<Value>> m_objects;

    Local<Value> createModule(Isolate *isolate, Local<Context> context, const std::string &name);
};
}

#endif //DEMO_V8NATIVEMODULES_H
