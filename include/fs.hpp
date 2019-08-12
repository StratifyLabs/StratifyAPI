/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_FILE_HPP_
#define SAPI_FILE_HPP_

/*! \brief File (POSIX wrappers)
 * \details The file namespace includes
 * classes for accessing files.
 *
 */
namespace fs {}

#if !defined __link
#include "fs/Aio.hpp"
#endif

#include "fs/Stat.hpp"
#include "fs/Dir.hpp"
#include "fs/File.hpp"

using namespace fs;


#endif /* SAPI_FILE_HPP_ */