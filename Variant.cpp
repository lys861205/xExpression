#include "Variant.h"

namespace route {

Variant::Variant():_type(Invalid),_numValue(0),_stringValue(NULL) {
}

Variant::Variant(bool b):_type(Bool),_numValue(b),_stringValue(NULL) {
}

Variant::Variant(int i):_type(Int),_numValue(i),_stringValue(NULL) {
}

Variant::Variant(unsigned int u):_type(UInt),_numValue(u),_stringValue(NULL) {
}

Variant::Variant(long l):_type(Long),_numValue(l),_stringValue(NULL) {
}

Variant::Variant(unsigned long ul):_type(ULong),_numValue(ul),_stringValue(NULL) {
}

Variant::Variant(long long ll):_type(LongLong),_numValue(ll),_stringValue(NULL) {
}

Variant::Variant(unsigned long long ll):_type(ULongLong),_numValue(ll),_stringValue(NULL) {
}

Variant::Variant(float f):_type(Float),_numValue(f),_stringValue(NULL){
}

Variant::Variant(double d):_type(Double),_numValue(d),_stringValue(NULL) {
}

Variant::Variant(char c):_type(Char),_numValue(c),_stringValue(NULL) {
}

Variant::Variant(const char *str):_type(String),_numValue(0),_stringValue(new std::string(str)) {
}

Variant::Variant(char *str):_type(String),_numValue(0),_stringValue(new std::string(str)) {
}

Variant::Variant(const std::string &val):_type(String),_numValue(0),_stringValue(new std::string(val)) {
}

Variant::~Variant() {
  delete _stringValue;
  _stringValue = NULL;
}

Variant::Variant(const Variant &other) {
  _type = other._type;
  _numValue = other._numValue;
  _stringValue = NULL;
  if (other._stringValue) {
    _stringValue = new std::string(*other._stringValue);
  }
}

Variant& Variant::operator=(const Variant &other) {
  _type = other._type;
  _numValue = other._numValue;
  if (_stringValue) {
    _stringValue->clear();
  }
  if (other._stringValue) {
    if (!_stringValue) {
      _stringValue = new std::string(*other._stringValue);
    } else {
      _stringValue->append(*other._stringValue);
    }
  }
  return *this;
}

bool Variant::operator==(const Variant &other) const {
  if (_type != other._type) {
    return false;
  }
  if (other._type == String) {
    return _stringValue == other._stringValue;
  }
  if (other._type == Float || other._type == Double) {
    return _numValue.doubleValue - other._numValue.doubleValue < 0.000001 && other._numValue.doubleValue - _numValue.doubleValue < 0.000001;
  }
  return _numValue.intValue == other._numValue.intValue;
}

bool Variant::operator!=(const Variant &other) const {
  return !(*this == other);
}

}
