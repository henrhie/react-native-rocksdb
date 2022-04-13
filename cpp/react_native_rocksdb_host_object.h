#include <jsi/jsi.h>
#include <vector>

#include "rocksdb/db"

using namespace facebook;

#ifndef ROCKSDBHOSTOBJECT_H
#define ROCKSDBHOSTOBJECT_H

class JSI_EXPORT reactNativeRocksdbHostObject: jsi::HostObject {
  public: 
    jsi::Value get(jsi::Runtime& rt, const jsi::PropNameID& name) override;
    std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime& rt) override;

  private:
    rocksdb:DB* dbInstance;
}

#endif //ROCKSDBHOSTOBJECT_H
