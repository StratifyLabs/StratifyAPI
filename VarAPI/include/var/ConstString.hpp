/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_VAR_CONSTSTRING_HPP
#define SAPI_VAR_CONSTSTRING_HPP

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include "../api/VarObject.hpp"
#include "../arg/Argument.hpp"
#include "String.hpp"
#include "Reference.hpp"

namespace var {

class ConstString : public virtual api::WorkObject
{
public:
   ConstString(){
      m_string = nullptr;
   }

	 explicit ConstString(const char * s){
      m_string = s;
   }

   operator const char * () const {
      return m_string;
   }

   const char * cstring() const {
      return m_string;
   }

private:
   const char * m_string;
};

}

#endif // SAPI_VAR_CONSTSTRING_HPP
