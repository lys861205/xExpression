#pragma once

#include <string>

namespace route {

  typedef enum __DataType {
    Invalid = 0,
    Bool,
    Char,
    Int,
    UInt,
    Long,
    ULong,
    LongLong,
    ULongLong,
    Float,
    Double,
    MaxSimpleType = Double,
    String,
  } DataType;

  class Variant {

    public:
      Variant();
      Variant(bool b);
      Variant(int i);
      Variant(unsigned int u);
      Variant(long l);
      Variant(unsigned long ul);
      Variant(long long ll);
      Variant(unsigned long long ll);
      Variant(float f);
      Variant(double d);
      Variant(char c);
      Variant(const char *str);
      Variant(char *str);
      Variant(const std::string &val);
      ~Variant();

    public:
      Variant(const Variant &other);
      Variant& operator=(const Variant &other);
      bool operator==(const Variant &other) const;
      bool operator!=(const Variant &other) const;

    public:
      inline bool isEmpty() const {
        return _type == Invalid;
      }

      inline bool isBool() const {
        return _type == Bool;
      }

      inline bool isInt() const {
        return _type == Int;
      }

      inline bool isUInt() const {
        return _type == UInt;
      }

      inline bool isLong() const {
        return _type == Long;
      }

      inline bool isULong() const {
        return _type == ULong;
      }

      inline bool isLongLong() const {
        return _type == LongLong;
      }

      inline bool isULongLong() const {
        return _type == ULongLong;
      }

      inline bool isFloat() const {
        return _type == Float;
      }

      inline bool isDouble() const {
        return _type == Double;
      }

      inline bool isChar() const {
        return _type == Char;
      }

      inline bool isString() const {
        return _type == String;
      }

    public:
      inline bool asConstBool() const {
        return !(_numValue.intValue == 0);
      }

      inline int asConstInt() const {
        return _numValue.intValue;
      }

      inline unsigned int asConstUInt() const {
        return _numValue.intValue;
      }

      inline long asConstLong() const {
        return _numValue.intValue;
      }

      inline unsigned long asConstULong() const {
        return _numValue.intValue;
      }

      inline long long asConstLongLong() const {
        return _numValue.intValue;
      }

      inline unsigned long long asConstULongLong() const {
        return _numValue.intValue;
      }

      inline float asConstFloat() const {
        return _numValue.doubleValue;
      }

      inline double asConstDouble() const {
        return _numValue.doubleValue;
      }

      inline const std::string &asConstString() const {
        if (_type != String || !_stringValue) {
          static std::string empty;
          return empty;
        }
        return *_stringValue;
      }

      inline char asConstChar() const {
        return _numValue.intValue;
      }
    private:
      int16_t _type;
      union Value {
        int64_t intValue;
        double  doubleValue;
        Value():intValue(0) {;}
        Value(bool b):intValue(b) {;}
        Value(int i):intValue(i) {;}
        Value(unsigned int u):intValue(u) {;}
        Value(long l):intValue(l) {;}
        Value(unsigned long ul):intValue(ul) {;}
        Value(long long ll):intValue(ll) {;}
        Value(unsigned long long ll):intValue(ll) {;}
        Value(float f):doubleValue(f) {;}
        Value(double d):doubleValue(d) {;}
        Value(char c):intValue(c) {;}
      };
      Value   _numValue;
      std::string *_stringValue;
  };

} // end namespace route
