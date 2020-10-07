/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
// Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include <cstdio>
#include <cstring>
#include <fcntl.h>

#include "chrono/Timer.hpp"
#include "fs/File.hpp"

#include "LinkAPI/Link.hpp"

#include "local.h"

using namespace fs;

File::File(FSAPI_LINK_DECLARE_DRIVER) {
  m_fd = -1; // The file is not open
  set_keep_open(false);
}

File::File(
  var::StringView name,
  FileFlags flags FSAPI_LINK_DECLARE_DRIVER_DECLARE_LAST) {
  open(name, flags);
}

File::~File() {
  if (is_keep_open() == false) {
    if (fileno() >= 0) {
      close();
    }
  }
}

File File::create(
  var::StringView path,
  Overwrite is_overwrite,
  Permissions perms FSAPI_LINK_DECLARE_DRIVER_DECLARE_LAST) {
  return File(FSAPI_LINK_DECLARE_DRIVER).internal_create(path, is_overwrite, perms);
}

int File::interface_open(const char *path, int flags, int mode) const {
  return FSAPI_LINK_OPEN(driver(), path, flags, mode);
}

int File::interface_read(int fd, void *buf, int nbyte) const {
  return FSAPI_LINK_READ(driver(), fd, buf, nbyte);
}

int File::interface_write(int fd, const void *buf, int nbyte) const {
  return FSAPI_LINK_WRITE(driver(), fd, buf, nbyte);
}

int File::interface_ioctl(int fd, int request, void *argument) const {
  return FSAPI_LINK_IOCTL(driver(), fd, request, argument);
}

int File::interface_close(int fd) const {
  return FSAPI_LINK_CLOSE(driver(), fd);
}

int File::interface_fsync(int fd) const {
#if defined __link
  return 0;
#else
  return ::fsync(fd);
#endif
}

int File::interface_lseek(int fd, int offset, int whence) const {
  return FSAPI_LINK_LSEEK(driver, fd, offset, whence);
}

File &
File::open(var::StringView path, FileFlags flags, Permissions permissions) {
  if (m_fd != -1) {
    close(); // close first so the fileno can be changed
  }

  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    m_fd = interface_open(
      path.cstring(),
      flags.o_flags(),
      permissions.permissions()));
  return *this;
}

File &File::internal_create(
  var::StringView path,
  Overwrite is_overwrite,
  const Permissions &perms) {
  FileFlags flags = FileFlags::create();
  if (is_overwrite == Overwrite::yes) {
    flags.set_truncate();
  } else {
    flags.set_exclusive();
  }

  return open(path, flags, perms);
}

size_t File::size() const {
  // get current cursor
  int loc = location();
  lseek(m_fd, 0, static_cast<int>(Whence::end));
  size_t size = static_cast<size_t>(location());
  lseek(m_fd, loc, static_cast<int>(Whence::set)); // restore the cursor
  return size;
}

int File::fstat(struct FSAPI_LINK_STAT_STRUCT *st) {
  return FSAPI_LINK_FSTAT(driver(), m_fd, st);
}

File &File::readline(char *buf, int nbyte, int timeout, char term) {
  int t;
  int bytes_recv;
  char c;
  t = 0;
  bytes_recv = 0;
  do {
    if (read(&c, 1).status().value() == 1) {
      *buf = c;
      buf++;
      bytes_recv++;
      if (c == term) {
        return *this;
      }
    } else {
      t++;
#if !defined __link
      chrono::wait(chrono::Milliseconds(1));
#endif
    }
  } while ((bytes_recv < nbyte) && (t < timeout));

  return *this;
}

File &File::close() {
  if (m_fd >= 0) {
    API_ASSIGN_ERROR_CODE(api::ErrorCode::io_error, interface_close(m_fd));
    m_fd = -1;
  }
  return *this;
}

File &File::sync() {
#if defined __link
  if (driver()) {
    return *this;
  }
#endif
  if (m_fd >= 0) {
#if !defined __win32
    API_ASSIGN_ERROR_CODE(api::ErrorCode::io_error, interface_fsync(m_fd));
#else
    ret = 0;
#endif
  }
  return *this;
}

File &File::read(void *buf, int nbyte) {
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    interface_read(m_fd, buf, nbyte));
  return *this;
}

File &File::write(const void *buf, int nbyte) {
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    interface_write(m_fd, buf, nbyte));
  return *this;
}

File &File::seek(int location, Whence whence) {
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    lseek(m_fd, location, static_cast<int>(whence)));
  return *this;
}

int File::fileno() const { return m_fd; }

int File::location() const {
  return lseek(m_fd, 0, static_cast<int>(Whence::current));
}

int File::flags() const {
#if defined __link
  return set_error_number_if_error(api::error_code_fs_unsupported_operation);
#else
  if (fileno() < 0) {
    API_ASSIGN_ERROR_CODE(api::ErrorCode::bad_file_number, -1);
    return status().value();
  }
  return _global_impure_ptr->procmem_base->open_file[m_fd].flags;
#endif
}

var::String File::gets(char term) {
  char c;
  var::String result;
  do {
    if (read(var::View(c)).status().value() == 1) {
      result.append(c);
    } else {
      return result;
    }
  } while (c != term);

  return result;
}

File &File::ioctl(int request, void *argument) {
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    interface_ioctl(m_fd, request, argument));
  return *this;
}

File &File::write(File &source_file, const WriteOptions &options) {

  if (options.location() != static_cast<u32>(-1)) {
    seek(options.location(), Whence::set);
  }

  u32 size_processed = 0;
  u32 file_size = options.size();
  if (file_size == static_cast<u32>(-1)) {
    file_size = source_file.size();
  }

  u32 page_size_value = options.page_size();
  if (page_size_value == 0) {
    page_size_value = FSAPI_LINK_DEFAULT_PAGE_SIZE;
  }

  var::Data buffer(page_size_value);

  if (buffer.size() == 0) {
    API_ASSIGN_ERROR_CODE(api::ErrorCode::no_memory, -1);
    return *this;
  }

  if (file_size == 0) {
    if (options.progress_callback()) {
      options.progress_callback()->update(0, 100);
      options.progress_callback()->update(100, 100);
      options.progress_callback()->update(0, 0);
    }
    return *this;
  }

  int bytes_read;
  do {
    if (file_size - size_processed < page_size_value) {
      page_size_value = file_size - size_processed;
    }

    buffer.resize(page_size_value);
    if (buffer.size() != page_size_value) {
      API_ASSIGN_ERROR_CODE(api::ErrorCode::no_memory, -1);
      return *this;
    }

    bytes_read = source_file.read(buffer).status().value();
    if (bytes_read > 0) {
      write(buffer.data(), bytes_read);
      if (status().is_error()) {
        return *this;
      }

      if (bytes_read > 0) {
        size_processed += static_cast<u32>(bytes_read);
      } else if (bytes_read < 0) {
        API_ASSIGN_ERROR_CODE(api::ErrorCode::io_error, -1);
        return *this;
      }
    }

    if (options.progress_callback()) {
      // abort the transaction
      if (
        options.progress_callback()->update(
          static_cast<int>(size_processed),
          static_cast<int>(file_size))
        == true) {
        options.progress_callback()->update(0, 0);
        API_ASSIGN_ERROR_CODE(api::ErrorCode::aborted, -1);
        return *this;
      }
    }

  } while ((bytes_read > 0) && (file_size > size_processed));

  // this will terminate the progress operation
  if (options.progress_callback()) {
    options.progress_callback()->update(0, 0);
  }
  if ((bytes_read < 0) && (size_processed == 0)) {
    API_ASSIGN_ERROR_CODE(api::ErrorCode::io_error, -1);
  }
  return *this;
}

DataFile::DataFile(fs::File &file_to_load) : FileAccess("") {
  m_location = 0;
  m_open_flags = FileFlags::append_read_write();
  write(file_to_load, WriteOptions());
  seek(0);
  m_open_flags = FileFlags::read_write();
}

int DataFile::interface_read(int fd, void *buf, int nbyte) const {

  if (flags().is_write_only()) {
    return -1;
  }

  int size_ready = m_data.size_signed() - m_location;
  if (size_ready > nbyte) {
    size_ready = nbyte;
  }

  if (size_ready < 0) {
    // EOF
    return -1;
  }

  memcpy(buf, m_data.data_u8() + m_location, size_ready);

  m_location += size_ready;
  return size_ready;
}

int DataFile::interface_write(int fd, const void *buf, int nbyte) const {

  if (flags().is_read_only()) {
    return -1;
  }

  int size_ready = 0;
  if (flags().is_append()) {
    // make room in the m_data object for more bytes
    m_location = static_cast<int>(m_data.size());
    if (m_data.resize(m_data.size() + nbyte).status().is_error()) {
      return -1;
    }
    size_ready = nbyte;
  } else {
    // limit writes to the current size of the data
    if (static_cast<int>(m_data.size()) > m_location) {
      size_ready = m_data.size_signed() - m_location;
      if (size_ready > nbyte) {
        size_ready = nbyte;
      }
    } else {
      return -1;
    }
  }

  memcpy(m_data.data_u8() + m_location, buf, size_ready);

  m_location += size_ready;
  return size_ready;
}

int DataFile::interface_lseek(int fd, int offset, int whence) const {
  MCU_UNUSED_ARGUMENT(fd);
  switch (static_cast<Whence>(whence)) {
  case Whence::current:
    m_location += offset;
    break;
  case Whence::end:
    m_location = static_cast<int>(m_data.size());
    break;
  case Whence::set:
    m_location = offset;
    break;
  }

  if (m_location > static_cast<int>(size())) {
    m_location = static_cast<int>(m_data.size());
  } else if (m_location < 0) {
    m_location = 0;
  }

  return m_location;
}

int ItemFile::interface_read(int fd, void *buf, int nbyte) const {

  if (flags().is_write_only()) {
    return -1;
  }

  int size_ready = item().size() - m_location;
  if (size_ready > nbyte) {
    size_ready = nbyte;
  }

  if (size_ready < 0) {
    return -1;
  }

  memcpy(buf, item().to_const_u8() + m_location, size_ready);

  m_location += size_ready;
  return size_ready;
}

int ItemFile::interface_write(int fd, const void *buf, int nbyte) const {
  MCU_UNUSED_ARGUMENT(fd);

  if (flags().is_read_only()) {
    return -1;
  }

  if (item().is_read_only()) {
    return -1;
  }

  int size_ready = 0;
  if (flags().is_append()) {
    return -1;
  } else {
    // limit writes to the current size of the data
    size_ready = item().size() - m_location;
    if (size_ready > nbyte) {
      size_ready = nbyte;
    }
  }

  if (size_ready < 0) {
    return -1;
  }

  memcpy(item().to_u8() + m_location, buf, size_ready);

  m_location += size_ready;
  return size_ready;
}

int ItemFile::interface_lseek(int fd, int location, int whence) const {
  switch (static_cast<Whence>(whence)) {
  case Whence::current:
    m_location += location;
    break;
  case Whence::end:
    m_location = item().size_signed();
    break;
  case Whence::set:
    m_location = location;
    break;
  }

  if (m_location > item().size_signed()) {
    m_location = item().size_signed();
  } else if (m_location < 0) {
    m_location = 0;
  }

  return m_location;
}
