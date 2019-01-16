/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_VAR_HPP_
#define SAPI_VAR_HPP_


/*! \brief Data Management
 * \details The var namespace is a collection of data storage mechanisms and routines
 * such as buffers and strings.
 *
 * Here is a brief summary of the most commonly used classes:
 *
 * - Data: manages static and dynamic allocation of memory ensuring no memory leaks
 * - String: An embedded friendly String object that implements many methods from std::string
 * - Queue: similar to std::queue (inherits LinkedList)
 * - Vector: similar to std::vector (inherits Data)
 * - Ring: template class for creating first-in first-out buffers of fixed size (inherits Data)
 * - Token: Breaks strings into tokens (inherits String)
 * - Array: similar to std::array
 *
 *
 */
namespace var {}

#include "var/Data.hpp"
#include "var/Flags.hpp"
#include "var/Item.hpp"
#include "var/Ring.hpp"
#include "var/LinkedList.hpp"
#include "var/Queue.hpp"
#include "var/Json.hpp"
#include "var/ConstString.hpp"
#include "var/VersionString.hpp"
#include "var/String.hpp"
#include "var/Tokenizer.hpp"
#include "var/Vector.hpp"
#include "var/Array.hpp"

using namespace var;


#endif /* SAPI_VAR_HPP_ */
