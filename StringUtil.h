#pragma once 

#include <vector>
#include <iostream>
#include <sstream>
#include "CheckCastNoThrow.h"

namespace gsl {

template<class T>
int StringSplit(const std::string& str, std::vector<T>& tokens, const char& sep)
{
  std::stringstream ss(str);        
  std::string tok;
  while (getline(ss, tok, sep)) {
    T value;
    gsl::check_cast(tok.c_str(), value);
    tokens.push_back(value);
  }
  return tokens.size();
}

} //end namespace gsl
