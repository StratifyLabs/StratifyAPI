/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

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

#include "fs/Dir.hpp"
#include "fs/File.hpp"
#include "fs/FileInfo.hpp"

using namespace fs;


#endif /* SAPI_FILE_HPP_ */
