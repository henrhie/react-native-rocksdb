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

rocksdb::DB* valueToDb(const jsi::Value& value, std::string* err) {
  if (!value.isNumber()) {
    *err = "valueToDb/param-not-a-number";
    return nullptr;
  }
  int idx = (int)value.getNumber();
  if (idx < 0 || idx >= dbs.size()) {
    *err = "valueToDb/idx-out-of-range";
    return nullptr;
  }
  if (!dbs[idx].get()) {
    *err = "valueToDb/db-closed";
    return nullptr;
  }
  return dbs[idx].get();
}

rocksdb::Iterator* valueToIterator(const jsi::Value& value) {
  if (!value.isNumber()) {
    return nullptr;
  }
  int idx = (int)value.getNumber();
  if (idx < 0 || idx >= iterators.size()) {
    return nullptr;
  }

  return iterators[idx].get();
}

std::vector<jsi::PropNameID> Sha1HostObject::getPropertyNames(jsi::Runtime& rt) {
    std::vector<jsi::PropNameID> props;
    props.push_back(jsi::PropNameID::forUtf8(rt, std::string("open")));
    props.push_back(jsi::PropNameID::forUtf8(rt, std::string("destroy")));
    props.push_back(jsi::PropNameID::forUtf8(rt, std::string("close")));
    props.push_back(jsi::PropNameID::forUtf8(rt, std::string("put")));
    props.push_back(jsi::PropNameID::forUtf8(rt, std::string("delete")));
    props.push_back(jsi::PropNameID::forUtf8(rt, std::string("get")));
    props.push_back(jsi::PropNameID::forUtf8(rt, std::string("newIterator")));
    props.push_back(jsi::PropNameID::forUtf8(rt, std::string("iteratorSeekToFirst")));
    props.push_back(jsi::PropNameID::forUtf8(rt, std::string("iteratorSeekToLast")));
    props.push_back(jsi::PropNameID::forUtf8(rt, std::string("iteratorSeekToIdx")));
    props.push_back(jsi::PropNameID::forUtf8(rt, std::string("iteratorValid")));
    props.push_back(jsi::PropNameID::forUtf8(rt, std::string("iteratorPrevious")));
    props.push_back(jsi::PropNameID::forUtf8(rt, std::string("iteratorNext")));
    props.push_back(jsi::PropNameID::forUtf8(rt, std::string("iteratorDelete")));
    props.push_back(jsi::PropNameID::forUtf8(rt, std::string("iteratorKeyAsStr")));
    props.push_back(jsi::PropNameID::forUtf8(rt, std::string("iteratorValueAsStr")));
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