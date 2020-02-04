/*! \file */ //Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#ifndef SAPI_API_ARGUMENT_OBJECT_HPP_
#define SAPI_API_ARGUMENT_OBJECT_HPP_

#include "WorkObject.hpp"
#include "InfoObject.hpp"

namespace api {

class ArgumentInfoObject : public virtual InfoObject {};

class ArgumentWorkObject : public virtual WorkObject {};

}


#endif // SAPI_API_ARGUMENT_OBJECT_HPP_
