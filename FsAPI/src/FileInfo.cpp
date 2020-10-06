/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#if defined __link
#include <sys/stat.h>
#endif

#include "FsAPI/FileInfo.hpp"
#include "PrinterAPI/Printer.hpp"

printer::Printer &operator<<(sys::Printer &printer, const fs::FileInfo &a) {
  var::String type;
  if (a.is_directory()) {
    type = "directory";
  }
  if (a.is_file()) {
    type = "file";
  }
  if (a.is_device()) {
    type = "device";
  }
  if (a.is_block_device()) {
    type = "block device";
  }
  if (a.is_character_device()) {
    type = "character device";
  }
  if (a.is_socket()) {
    type = "socket";
  }
  printer.key("type", type);
  if (a.is_file()) {
    printer.key("size", "%ld", a.size());
  }
  printer.key("mode", "0%o", a.permissions().permissions() & 0777);

  return printer;
}

using namespace fs;

#if defined __link
FileInfo::Stat(bool is_local) {
  memset(&m_stat, 0, sizeof(m_stat));
  m_is_local = is_local;
}
#else

FileInfo::FileInfo() { memset(&m_stat, 0, sizeof(m_stat)); }
#endif

bool FileInfo::is_directory() const {
#if defined __link
  if (m_is_local) {
    return (m_stat.st_mode & S_IFMT) == S_IFDIR;
  }
#endif
  return (m_stat.st_mode & FileInfo::mode_format) == FileInfo::mode_directory;
}

bool FileInfo::is_file() const {
#if defined __link
  if (m_is_local) {
    return (m_stat.st_mode & S_IFMT) == S_IFREG;
  }
#endif
  return (m_stat.st_mode & FileInfo::mode_format) == FileInfo::mode_regular;
}

bool FileInfo::is_device() const {
  return is_block_device() || is_character_device();
}

bool FileInfo::is_block_device() const {
#if defined __link
  if (m_is_local) {
    return (m_stat.st_mode & S_IFMT) == S_IFBLK;
  }
#endif
  return (m_stat.st_mode & (FileInfo::mode_format)) == FileInfo::mode_block;
}

bool FileInfo::is_character_device() const {
#if defined __link
  if (m_is_local) {
    return (m_stat.st_mode & S_IFMT) == S_IFCHR;
  }
#endif
  return (m_stat.st_mode & (FileInfo::mode_format)) == FileInfo::mode_character;
}

bool FileInfo::is_socket() const {
#if defined __link
  if (m_is_local) {
#if !defined __win32
    return (m_stat.st_mode & S_IFMT) == S_IFSOCK;
#else
    return false;
#endif
  }
#endif
  return (m_stat.st_mode & FileInfo::mode_format) == FileInfo::mode_file_socket;
}

u32 FileInfo::size() const { return m_stat.st_size; }

bool FileInfo::is_executable() const { return false; }

const var::String FileInfo::suffix(const var::String &path) {
  size_t pos = path.rfind('.');

  if (pos == var::String::npos) {
    return var::String();
  }

  return path.create_sub_string(var::String::Position(pos + 1));
}

const var::String FileInfo::name(const var::String &path) {
  size_t pos = path.rfind('/');

  if (pos == var::String::npos) {
    return path;
  }

  return path.create_sub_string(var::String::Position(pos + 1));
}

const var::String FileInfo::parent_directory(const var::String &path) {
  size_t pos = path.rfind('/');

  if (pos == var::String::npos) {
    return var::String();
  }

  return path.create_sub_string(
    var::String::Position(0),
    var::String::Length(pos));
}

const var::String FileInfo::base_name(const var::String &path) {
  var::String result = name(path);
  size_t pos = result.rfind('.');
  if (pos == var::String::npos) {
    return result;
  }

  return result.create_sub_string(
    var::String::Position(0),
    var::String::Length(pos));
}

const var::String FileInfo::no_suffix(const var::String &path) {
  size_t pos = path.rfind('.');

  if (pos == var::String::npos) {
    return var::String();
  }

  return path.create_sub_string(
    var::String::Position(0),
    var::String::Length(pos));
}

bool FileInfo::is_hidden(const var::String &path) {
  if (name(path).find(".") == 0) {
    return true;
  }

  var::String parent = parent_directory(path);
  if (parent != path) {
    return is_hidden(parent_directory(path));
  }

  return false;
}
