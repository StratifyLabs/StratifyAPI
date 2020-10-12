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
  LINK_SET_DRIVER((*this), link_driver);
  m_dirp = nullptr;
  open(path);
}

Dir::~Dir() { close(); }

Dir &Dir::open(var::StringView path) {
  API_RETURN_VALUE_IF_ERROR(*this);
  m_dirp = interface_opendir(path.cstring());

  if (m_dirp == 0) {
    API_RETURN_VALUE_ASSIGN_ERROR(*this, "no entity", ENOENT);
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
    API_RETURN_VALUE_ASSIGN_ERROR(-1, "", EBADF);
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

const char *Dir::read() const {
  API_RETURN_VALUE_IF_ERROR(nullptr);
  struct dirent *result;
  if (interface_readdir_r(m_dirp, &m_entry, &result) < 0) {
    return nullptr;
  }
  return m_entry.d_name;
}

var::String Dir::get_entry() const {
  const char *entry = read();

  if (entry == nullptr) {
    return var::String();
  }

  return m_path
         + ((m_path.is_empty() == false) ? var::String("/") : var::String())
         + var::StringView (entry);
}

Dir &Dir::close() {
  API_RETURN_VALUE_IF_ERROR(*this);
  if (m_dirp) {
    API_SYSTEM_CALL(m_path.cstring(), interface_closedir(m_dirp));
    m_dirp = nullptr;
  }

  m_path.clear();
  return *this;
}

DIR *Dir::interface_opendir(const char *path) const {
  return reinterpret_cast<DIR *>(FSAPI_LINK_OPENDIR(driver(), path));
}

int Dir::interface_readdir_r(
  DIR *dirp,
  struct dirent *result,
  struct dirent **resultp) const {
  return FSAPI_LINK_READDIR_R(driver(), dirp, result, resultp);
}

int Dir::interface_closedir(DIR *dirp) const {
  return FSAPI_LINK_CLOSEDIR(driver(), dirp);
}

int Dir::interface_telldir(DIR *dirp) const {
  return FSAPI_LINK_TELLDIR(driver(), dirp);
}

void Dir::interface_seekdir(DIR *dirp, size_t location) const {
  FSAPI_LINK_SEEKDIR(driver(), dirp, location);
}

void Dir::interface_rewinddir(DIR *dirp) const {
  FSAPI_LINK_REWINDDIR(driver(), dirp);
}
