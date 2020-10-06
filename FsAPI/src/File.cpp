/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
// Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include <cstdio>
#include <cstring>
#include <fcntl.h>

#include "ChronoAPI/Timer.hpp"
#include "FsAPI/File.hpp"

#include "LinkAPI/Link.hpp"

using namespace fs;
using namespace arg;

File::File(SAPI_LINK_DRIVER) {
  m_fd = -1; // The file is not open
  set_keep_open(false);
#if defined __link
  m_driver = link_driver.argument();
#endif
}

File::~File() {
  if (is_keep_open() == false) {
    if (fileno() >= 0) {
      close();
    }
  }
}

int open(const char *path, int flags, int mode) {
  return link_open(driver(), path, flags, mode);
}

int read(int fd, void *buf, int nbyte) const {
  return link_read(driver(), fd, buf, nbyte);
}

int write(int fd, const void *buf, int nbyte) {
  return link_write(driver(), fd, buf, nbyte);
}

int ioctl(int fd, int request, void *argument) {
  return link_ioctl(driver(), fd, request, argument);
}

int close(int fd) { return link_close(driver(), fd); }

int fsync(int fd) {
#if defined __link
  return 0;
#else
  return ::fsync(fd);
#endif
}

int lseek(int fd, int offset, int whence) {
  return link_lseek(driver, fd, offset, whence);
}

int File::open(const var::String &path, const fs::OpenFlags &flags) {
  return open(path, flags, Permissions());
}

File &File::open(
  const var::String &path,
  const fs::OpenFlags &flags,
  const Permissions &permissions) {
  if (m_fd != -1) {
    close(); // close first so the fileno can be changed
  }

  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    m_fd = open(path.cstring(), flags.o_flags(), permissions.permissions()));

  return *this;
}

File &File::create(
  const var::StringView &path,
  Overwrite is_overwrite,
  const Permissions &perms) {
  fs::OpenFlags flags = fs::OpenFlags::create();
  if (is_overwrite == Overwrite::yes) {
    flags.set_truncate();
  } else {
    flags.set_exclusive();
  }

  return open(path, flags, perms);
}

u32 File::size() const {
  int loc = seek(Location(0), whence_current); // get current cursor
  u32 size = static_cast<u32>(seek(Location(0), whence_end)); // seek to the end
  seek(Location(loc), whence_set); // restore the cursor
  return size;
}

int File::stat(
  const var::String &name,
  struct SAPI_LINK_STAT &st SAPI_LINK_DRIVER_LAST) {
  return link_stat(link_driver.argument(), name.cstring(), &st);
}

File &File::fstat(struct SAPI_LINK_STAT *st) {
  return link_fstat(driver(), m_fd, st);
}

#if !defined __link
u32 File::size(const var::String &name) {
  struct stat st;
  if (stat(name, st) < 0) {
    return (s32)-1;
  }
  return st.st_size;
}
#else
u32 File::size(const var::String &name SAPI_LINK_DRIVER_LAST) {
  struct SAPI_LINK_STAT st;
  if (
    stat(
      name,
      st
#if defined __link
      ,
      link_driver
#endif
      )
    < 0) {
    return static_cast<u32>(-1);
  }
  return st.st_size;
}

#endif

int File::read(Location location, void *buf, Size size) const {
  int result = seek(location, whence_set);
  if (result < 0) {
    return result;
  }
  return read(buf, size);
}

int File::write(Location location, const void *buf, Size size) const {
  int result = seek(location, whence_set);
  if (result < 0) {
    return result;
  }
  return write(buf, size);
}

int File::readline(char *buf, int nbyte, int timeout, char term) const {
  int t;
  int bytes_recv;
  char c;
  t = 0;
  bytes_recv = 0;
  do {
    if (read(&c, Size(1)) == 1) {
      *buf = c;
      buf++;
      bytes_recv++;
      if (c == term) {
        return bytes_recv;
      }
    } else {
      t++;
#if !defined __link
      chrono::wait(chrono::Milliseconds(1));
#endif
    }
  } while ((bytes_recv < nbyte) && (t < timeout));

  return bytes_recv;
}

int File::close() {
  int ret = 0;
  if (m_fd >= 0) {
    ret = set_error_number_if_error(link_close(driver(), m_fd));
    m_fd = -1;
  }
  return ret;
}

File &File::fsync() {
  int ret = 0;

#if defined __link
  if (driver()) {
    return 0;
  }
#endif
  if (m_fd >= 0) {
#if !defined __win32
    ret = set_error_number_if_error(::fsync(m_fd));
#else
    ret = 0;
#endif
  }
  return ret;
}

int File::read(void *buf, Size size) const {
  return set_error_number_if_error(
    link_read(driver(), m_fd, buf, static_cast<int>(size.argument())));
}

int File::write(const void *buf, Size size) const {
  return set_error_number_if_error(
    link_write(driver(), m_fd, buf, static_cast<int>(size.argument())));
}

int File::seek(int location, enum whence whence) const {
  return set_error_number_if_error(
    link_lseek(driver(), m_fd, location, whence));
}

int File::fileno() const { return m_fd; }

int File::location() const { return seek(Location(0), whence_current); }

int File::flags() const {
#if defined __link
  return set_error_number_if_error(api::error_code_fs_unsupported_operation);
#else
  if (fileno() < 0) {
    return set_error_number_if_error(api::error_code_fs_bad_descriptor);
  }
  return _global_impure_ptr->procmem_base->open_file[m_fd].flags;
#endif
}

char *File::gets(char *s, int n, char term) const {
  int t;
  int ret;
  int i;
  char buffer[m_gets_buffer_size];

  if (n < 1) {
    return nullptr;
  }

  s[0] = 0;
  t = 0;
  do {
    ret = read(buffer, m_gets_buffer_size);
    for (i = 0; i < ret; i++) {
      s[t] = buffer[i];
      s[t + 1] = 0;
      if (s[t] == '\n') {
        break;
      }
      if (t == (n - 1)) {
        break;
      }
      t++;
    }
  } while ((t < (n - 1)) && (s[t] != term) && (ret > 0));

  if (t == 0) {
    return nullptr;
  }

  if (ret > 0) {
    seek(i - ret + 1, Whence::current);
  }

  return s;
}

var::String File::gets(char term) const {
  var::String result;

  var::String result;
  gets(ret, term);
  return ret;
}

const char *File::gets(var::String &s, char term) const {
  int ret;
  char c;
  s.clear();
  do {
    ret = read(&c, Size(1));
    if (ret > 0) {
      s.append(c);
    } else {
      return nullptr;
    }
  } while (c != term);

  return s.cstring();
}

const var::String File::name(const var::String &path) {
  size_t pos = path.rfind('/');

  if (pos == var::String::npos) {
    return path;
  }

  return path.create_sub_string(var::String::Position(pos + 1));
}

#if !defined __link
int File::access(const var::String &path, const Access &access) {
  return ::access(path.cstring(), access.o_access());
}
#endif

const var::String File::suffix(const var::String &path) {
  size_t pos = path.rfind('.');
  if (pos != var::String::npos) {
    return path.cstring() + pos + 1;
  }
  return nullptr;
}

int File::ioctl(IoRequest request, IoArgument argument) const {
  return set_error_number_if_error(
#if defined __link
    link_ioctl(
      driver(),
#else
    ::ioctl(
#endif
      m_fd,
      request.argument(),
      argument.argument()));
}

int File::write(const File &source_file, PageSize page_size, Size size) const {
  return write(source_file, page_size, size, nullptr);
}

File &File::write(const File &source_file, const WriteOptions &options) {

  if (options.location() != static_cast<u32>(-1)) {
    seek(options.location(), whence_set);
  }
  return write(
    source_file,
    PageSize(options.page_size()),
    Size(options.size()),
    options.progress_callback());
}

int File::write(
  const File &source_file,
  PageSize page_size,
  Size size,
  const sys::ProgressCallback *progress_callback) const {
  u32 size_processed = 0;
  u32 file_size = size.argument();
  if (file_size == static_cast<u32>(-1)) {
    file_size = source_file.size();
  }

  u32 page_size_value = page_size.argument();
  if (page_size_value == 0) {
    page_size_value = SAPI_LINK_DEFAULT_PAGE_SIZE;
  }

  var::Data buffer(page_size_value);

  if (buffer.size() == 0) {
    return set_error_number_if_error(-1);
  }

  if (file_size == 0) {
    if (progress_callback) {
      progress_callback->update(0, 100);
      progress_callback->update(100, 100);
      progress_callback->update(0, 0);
    }
    return 0;
  }

  int result;
  do {
    if (file_size - size_processed < page_size_value) {
      page_size_value = file_size - size_processed;
    }

    buffer.resize(page_size_value);
    if (buffer.size() != page_size_value) {
      return set_error_number_if_error(-1);
    }

    result = source_file.read(buffer);
    if (result > 0) {
      result = write(buffer.to_const_void(), Size(static_cast<size_t>(result)));
      if (result > 0) {
        size_processed += static_cast<u32>(result);
      } else if (result < 0) {
        return set_error_number_if_error(result);
      }
    }

    if (progress_callback) {
      // abort the transaction
      if (
        progress_callback->update(
          static_cast<int>(size_processed),
          static_cast<int>(file_size))
        == true) {
        progress_callback->update(0, 0);
        return static_cast<int>(size_processed);
      }
    }

  } while ((result > 0) && (file_size > size_processed));

  // this will terminate the progress operation
  if (progress_callback) {
    progress_callback->update(0, 0);
  }
  if ((result < 0) && (size_processed == 0)) {
    return set_error_number_if_error(result);
  }
  return set_error_number_if_error(static_cast<int>(size_processed));
}

DataFile::DataFile(const Path &file_path) {
  // read the contents of file_path into this object
  FileInfo info = File::get_info(file_path.path());
  m_open_flags = OpenFlags::append_write_only();
  m_fd = 0;
  m_location = 0;
  if (info.is_valid() && info.is_file()) {
    File f;
    if (f.open(file_path.argument(), OpenFlags::read_only()) >= 0) {
      write(f);
      f.close();
    }
  }
  m_location = 0;
  m_open_flags = OpenFlags::read_write();
}

DataFile::DataFile(fs::File &file_to_load) {
  m_fd = 0;
  m_location = 0;
  m_open_flags = OpenFlags::append_read_write();
  write(file_to_load);
  seek(0);
  m_open_flags = OpenFlags::read_write();
}

int DataFile::read(void *buf, size_t nbyte) const {

  if (flags().is_write_only()) {
    return set_error_number_if_error(api::error_code_fs_cant_read);
  }

  int size_ready = static_cast<int>(m_data.size()) - m_location;
  if (size_ready > static_cast<int>(nbyte.argument())) {
    size_ready = static_cast<int>(nbyte.argument());
  }

  if (size_ready < 0) {
    // EOF
    return set_error_number_if_error(-1);
  }

  var::Reference::memory_copy(
    SourceBuffer(m_data.to_const_u8() + m_location),
    DestinationBuffer(buf),
    Size(static_cast<size_t>(size_ready)));

  m_location += size_ready;
  return set_error_number_if_error(size_ready);
}

int DataFile::write(const void *buf, Size nbyte) const {

  if (flags().is_read_only()) {
    return set_error_number_if_error(api::error_code_fs_cant_write);
  }

  u32 size_ready = 0;
  if (flags().is_append()) {
    // make room in the m_data object for more bytes
    m_location = static_cast<int>(m_data.size());
    if (m_data.resize(m_data.size() + nbyte.argument()) < 0) {
      set_error_number_to_errno();
      return set_error_number_if_error(-1);
    }
    size_ready = nbyte.argument();
  } else {
    // limit writes to the current size of the data
    if (static_cast<int>(m_data.size()) > m_location) {
      size_ready = m_data.size() - static_cast<u32>(m_location);
      if (size_ready > nbyte.argument()) {
        size_ready = nbyte.argument();
      }
    } else {
      return set_error_number_if_error(-1);
    }
  }

  memcpy(buf, data().data_u8() + m_location, size_ready);

  var::Item::memory_copy(
    SourceBuffer(buf),
    DestinationBuffer(data().to_u8() + m_location),
    Size(size_ready));

  m_location += size_ready;
  return set_error_number_if_error(static_cast<int>(size_ready));
}

int DataFile::seek(int fd, int offset, int whence) {
  MCU_UNUSED_ARGUMENT(fd);
  switch (whence) {
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

int ReferenceFile::read(void *buf, Size nbyte) const {

  if (flags().is_write_only()) {
    return set_error_number_if_error(api::error_code_fs_cant_read);
  }

  int size_ready = reference().size() - m_location;
  if (size_ready > (int)nbyte.argument()) {
    size_ready = nbyte.argument();
  }

  if (size_ready < 0) {
    return set_error_number_if_error(-1);
  }

  var::Reference::memory_copy(
    SourceBuffer(reference().to_const_u8() + m_location),
    DestinationBuffer(buf),
    Size(size_ready));

  m_location += size_ready;
  return set_error_number_if_error(size_ready);
}

int ReferenceFile::write(const void *buf, Size nbyte) const {

  if (flags().is_read_only()) {
    return set_error_number_if_error(api::error_code_fs_cant_write);
  }

  if (reference().is_read_only()) {
    return set_error_number_if_error(api::error_code_fs_cant_write_read_only);
  }

  int size_ready = 0;
  if (flags().is_append()) {
    return set_error_number_if_error(api::error_code_fs_cant_write_append_only);
  } else {
    // limit writes to the current size of the data
    size_ready = reference().size() - m_location;
    if (size_ready > (int)nbyte.argument()) {
      size_ready = nbyte.argument();
    }
  }

  if (size_ready < 0) {
    return set_error_number_if_error(-1);
  }

  var::Reference::memory_copy(
    SourceBuffer(buf),
    DestinationBuffer(reference().to_u8() + m_location),
    Size(size_ready));
  m_location += size_ready;
  return set_error_number_if_error(size_ready);
}

int ReferenceFile::seek(int location, enum whence whence) const {
  switch (whence) {
  case whence_current:
    m_location += location;
    break;
  case whence_end:
    m_location = reference().size();
    break;
  case whence_set:
    m_location = location;
    break;
  }

  if (m_location > (int)size()) {
    m_location = reference().size();
  } else if (m_location < 0) {
    m_location = 0;
  }

  return m_location;
}

int NullFile::open(const var::String &name, const OpenFlags &flags) {
  MCU_UNUSED_ARGUMENT(name);
  MCU_UNUSED_ARGUMENT(flags);
  return 0;
}

int NullFile::read(void *buf, Size nbyte) const {
  MCU_UNUSED_ARGUMENT(buf);
  MCU_UNUSED_ARGUMENT(nbyte);
  return -1; // end of file
}

int NullFile::write(const void *buf, Size nbyte) const {
  MCU_UNUSED_ARGUMENT(buf);
  return nbyte.argument();
}

int NullFile::seek(int location, enum whence whence) const {
  MCU_UNUSED_ARGUMENT(location);
  MCU_UNUSED_ARGUMENT(whence);
  return -1;
}
