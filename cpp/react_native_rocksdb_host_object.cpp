#include "react-native-rocksdb-hostobject.h"

bool valueToString(jsi::Runtime& runtime, const jsi::Value& value, std::string* str) {
  if (value.isString()) {
    *str = value.asString(runtime).utf8(runtime);
    return true;
  }

  if (value.isObject()) {
    auto obj = value.asObject(runtime);
    if (!obj.isArrayBuffer(runtime)) {
      return false;
    }
    auto buf = obj.getArrayBuffer(runtime);
    *str = std::string((char*)buf.data(runtime), buf.size(runtime));
    return true;
  }
  return false;
}



std::vector<jsi::PropNameID> Sha1HostObject::getPropertyNames(jsi::Runtime& rt) {
    std::vector<jsi::PropNameID> props;
    props.push_back(jsi::PropNameID::forUtf8(rt, std::string("open")));
    props.push_back(jsi::PropNameID::forUtf8(rt, std::string("destroy")));
    props.push_back(jsi::PropNameID::forUtf8(rt, std::string("close")));
    props.push_back(jsi::PropNameID::forUtf8(rt, std::string("setItem")));
    props.push_back(jsi::PropNameID::forUtf8(rt, std::string("removeItem")));
    props.push_back(jsi::PropNameID::forUtf8(rt, std::string("getItem")));
    return props;
}

jsi::Value Sha1HostObject::get(jsi::Runtime& rt, const jsi::PropNameID& name) {
    auto _propName = name.utf8(rt);
    auto funcName = "SHA1."+_propName;

    if(_propName == "computeHash") {
        return jsi::Function::createFromHostFunction(
                rt,
                jsi::PropNameID::forAscii(rt, funcName),
                1, //input string
                [this](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count) -> jsi::Value {
                    std::string text;
                    if(!valueToString(runtime, args[0], &text)) {
                        throw jsi::JSError(runtime, "sha1 / invalid params");
                    }

                    std::string hashString = sha1(text);
                    return jsi::Value(jsi::String::createFromUtf8(runtime, hashString));
                }
        );
    }

    return jsi::Value::undefined();
}