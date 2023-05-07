#pragma once 

#include <string>
#include <cstdint>
#include <vector>
#include <algorithm>
#include "CheckCastNoThrow.h"
#include <iostream>

namespace route {

class IChecker {
public:
    IChecker() = default;
    virtual ~IChecker() = default;
    
public:
    virtual int Parser(const std::string& pattern) = 0;

    virtual bool IsValid(const int8_t& value)
    {
        return false;
    }

    virtual bool IsValid(const uint8_t& value)
    {
        return false;
    }

    virtual bool IsValid(const int32_t& value)
    {
        return false;
    }

    virtual bool IsValid(const uint32_t& value)
    {
        return false;
    }

    virtual bool IsValid(const int64_t& value)
    {
        return false;
    }

    virtual bool IsValid(const uint64_t& value)
    {
        return false;
    }

    virtual bool IsValid(const float& value)
    {
        return false;
    }

    virtual bool IsValid(const double& value)
    {
        return false;
    }

    virtual bool IsValid(const std::string& value)
    {
        return false;
    }
};


template<typename T, typename Judge>
class TChecker : public IChecker {
    enum State {
        E_START,
        E_VALUE,
        E_END,
    };
public:
    TChecker() = default;
    ~TChecker() = default;

    static IChecker* Create() 
    {
        return new TChecker<T, Judge>();    
    }
    /**
    * @brief 
    *
    * @param pattern
    *
    * @return 0 success, 1 error for format 
    */
    int Parser(const std::string& pattern) override
    {
        State s = E_START;
        std::string value;
        int dot = 0;
        for (size_t i=0; i < pattern.size(); ++i) {
            const char& ch = pattern[i];
            if (std::isspace(ch)) {
                continue;
            }
            switch (s) {
                case E_START:
                    if (ch == '(' || ch == '[' || ch == '{') {
                        l_ch_ = ch;
                    }
                    else {
                        return 1;
                    }
                    s = E_VALUE;
                break;
                case E_VALUE:
                    {
                        bool finish = false;
                        if (ch == '.') {
                            ++dot;
                        }
                        if (judge_(ch)) {
                            value.push_back(ch);
                        }
                        else if (ch == ',') {
                            finish =  true;
                        }
                        else if (ch == ')' || ch == ']' || ch == '}') {
                            finish =  true;
                            r_ch_ = ch;
                            s = E_END;
                        }
                        else {
                            return 1; 
                        }
                        if (dot > 1) {
                            return 1;
                        }
                        if (finish) {
                            dot = 0;
                            T num;
                            gsl::check_cast<T, const char*>(value.c_str(), num);
                            candidate_values_.push_back(num);
                            value.clear();
                        }
                    }
                break;
                case E_END:
                break;
            }
            
        }
        return s == E_END && IsMatch() ? 0 : 1;
    }

    bool IsValid(const T& value) override
    {
        if (l_ch_ == '(' && r_ch_ == ')') {
           return candidate_values_[0] < value && value < candidate_values_[1];
        } else if (l_ch_ == '(' && r_ch_ == ']') {
           return candidate_values_[0] < value && value <= candidate_values_[1];
        } else if (l_ch_ == '[' && r_ch_ == ']') {
           return candidate_values_[0] <= value && value <= candidate_values_[1];
        } else if (l_ch_ == '[' && r_ch_ == ')') {
           return candidate_values_[0] <= value && value < candidate_values_[1];
        } else if (l_ch_ == '{' && r_ch_ == '}') {
            for (auto& it : candidate_values_) {
                if (it == value) {
                    return true;
                }
            }
        }
        return false;
    }

private:
    inline bool IsMatch() 
    {
        bool match = false;
        if ((l_ch_ == '(' || l_ch_ == '[') && 
            (r_ch_ == ')' || r_ch_ == ']') && 
            candidate_values_.size() == 2) {
           match = true; 
        } else if (l_ch_ == '{' && r_ch_ == '}') {
           match = true; 
        }
        return match;
    }

protected:
    char l_ch_;
    char r_ch_;
    std::vector<T> candidate_values_;
    Judge judge_;
};

struct NumberCheck {
    bool operator()(const char& c) {
        return c >= '0' && c <= '9'; 
    }
};

struct FloatCheck {
    bool operator()(const char& c) {
        return c >= '0' && c <= '9' || c == '.';
    }
};

struct StringCheck {
    bool operator()(const char& c) {
        return std::iscntrl(c) == 0 && !(c ==',' || c == '{' || c == '}' || c == ']' || c == '[' || c == '(' || c == ')');
    }
};

struct WildcardCheck {
    bool operator()(const char& c) {
        return c == '*';
    }
};

typedef TChecker<int32_t, NumberCheck> IntChecker;
typedef TChecker<uint32_t, NumberCheck> UIntChecker;
typedef TChecker<int64_t, NumberCheck> LongChecker;
typedef TChecker<uint64_t, NumberCheck> ULongChecker;
typedef TChecker<float, FloatCheck> FloatChecker;
typedef TChecker<double, FloatCheck> DoubleChecker;
typedef TChecker<std::string, StringCheck> StringChecker;

} //end namespace route
