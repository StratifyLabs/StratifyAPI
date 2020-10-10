/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
// Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include <sys/stat.h>
#include <unistd.h>

#include "fs/Dir.hpp"
#include "fs/File.hpp"
#include "var/String.hpp"
#include "var/Tokenizer.hpp"

#include "fs/FileSystem.hpp"
#include "local.h"

using namespace fs;

Dir::Dir(var::StringView path FSAPI_LINK_DECLARE_DRIVER_LAST) {
  API_RETURN_IF_ERROR();
  m_dirp = 0;
#if defined __link
  m_dirp_local = 0;
  m_driver = link_driver;
#endif
  open(path);
}

Dir::~Dir() { close(); }

Dir &Dir::open(var::StringView path) {
  API_RETURN_VALUE_IF_ERROR(*this);
#if defined __link
  if (driver()) {
    m_dirp = link_opendir(driver(), path.cstring());
  } else {
    // open a directory on the local system (not over link)

    m_dirp_local = opendir(path.cstring());
    if (m_dirp_local == nullptr) {
      API_ASSIGN_ERROR_CODE(api::ErrorCode::no_entity, -1);
      return *this;
    }
    m_path = var::String(path);
    return *this;
  }
#else
  m_dirp = opendir(path.cstring());
#endif

  if (m_dirp == 0) {
    API_ASSIGN_ERROR_CODE(api::ErrorCode::no_entity, -1);
  } else {
    m_path = var::String(path);
  }
  return *this;
}

#if !defined __link
int Dir::count() {
  API_RETURN_VALUE_IF_ERROR(-1);
  long loc;
  int count;

  if (!is_open()) {
    return API_ASSIGN_ERROR_CODE(api::ErrorCode::no_entity, -1);
  }

#if defined __link

#else
  loc = tell();
#endif

#if defined __link

#else
  rewind();
#endif

  count = 0;
  while (read() != 0) {
    count++;
  }

  seek(loc);

  return count;
}

#endif

var::StringList Dir::read_list(
  var::StringView (*filter)(var::StringView),
  IsRecursive is_recursive) {
  API_RETURN_VALUE_IF_ERROR(var::StringList());
  var::Vector<var::String> result;
  var::String entry;
  bool is_the_end = false;

  do {
    entry.clear();
    entry = read();
    if (entry.is_empty()) {
      is_the_end = true;
    }
    if (filter != nullptr) {
      entry = var::String(filter(entry));
    }
    if (!entry.is_empty() && (entry != ".") && (entry != "..")) {

      if (is_recursive == IsRecursive::yes) {
        var::String entry_path;
        entry_path = m_path + "/" + entry;
        FileInfo info = FileSystem(LINK_DRIVER_ONLY)
                          .get_info(
                            entry_path
                          );

        if (info.is_directory()) {
          var::Vector<var::String> intermediate_result
            = Dir(entry_path FSAPI_LINK_MEMBER_DRIVER_LAST)
                .read_list(filter, is_recursive);

          for (const auto &intermediate_entry : intermediate_result) {
            result.push_back(entry + "/" + intermediate_entry);
          }
        } else {
          result.push_back(entry);
        }
      } else {
        result.push_back(entry);
      }
    }

  } while (!is_the_end);

  return result;
}

var::Vector<var::String> Dir::read_list(IsRecursive is_recursive) {
  return read_list(nullptr, is_recursive);
}

const char *Dir::read() {

#if defined __link
  if (driver()) {
    struct link_dirent *result;
    memset(&m_entry, 0, sizeof(m_entry));
    if (link_readdir_r(driver(), m_dirp, &m_entry, &result) < 0) {
      return nullptr;
    }
  } else {
    struct dirent *result_local;
    if (
      (readdir_r(m_dirp_local, &m_entry_local, &result_local) < 0)
      || (result_local == 0)) {
      return nullptr;
    }
    return m_entry_local.d_name;
  }
#else
  struct dirent *result;
  if (
    API_ASSIGN_ERROR_CODE(
      api::ErrorCode::io_error,
      readdir_r(m_dirp, &m_entry, &result))
    < 0) {
    return nullptr;
  }
#endif
  return m_entry.d_name;
}

var::String Dir::get_entry() {
  const char *entry = read();

  if (entry == nullptr) {
    return var::String();
  }

  return m_path
         + ((m_path.is_empty() == false) ? var::String("/") : var::String())
         + var::StringView (entry);
}

Dir &Dir::close() {
  m_path.clear();
  if (m_dirp) {
#if defined __link
    if (driver()) {
      API_ASSIGN_ERROR_CODE(
        api::ErrorCode::io_error,
        link_closedir(driver(), m_dirp));
    }
    m_dirp = 0;
#else //__link
    API_ASSIGN_ERROR_CODE(api::ErrorCode::io_error, closedir(m_dirp));
    m_dirp = nullptr;
#endif
  }

#if defined __link
  if (m_dirp_local) {
    DIR *dirp_copy = m_dirp_local;
    m_dirp_local = 0;
    API_ASSIGN_ERROR_CODE(api::ErrorCode::io_error, closedir(dirp_copy));
  }
#endif
  return *this;
}
