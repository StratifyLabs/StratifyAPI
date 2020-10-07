/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#if defined __link
#include <sys/stat.h>
#endif

#include "fs/FileInfo.hpp"
#include "printer/Printer.hpp"
#include "var/String.hpp"

printer::Printer &operator<<(printer::Printer &printer, const fs::FileInfo &a) {
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
    printer.key("size", var::String::number(a.size()));
  }
  printer.key(
    "mode",
    var::String::number(a.permissions().permissions() & 0777, "0%o"));

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
  TypeFlags masked = static_cast<TypeFlags>(m_stat.st_mode);
  masked &= TypeFlags::mask;
  return masked == TypeFlags::directory;
}

bool FileInfo::is_file() const {
#if defined __link
  if (m_is_local) {
    return (m_stat.st_mode & S_IFMT) == S_IFREG;
  }
#endif
  TypeFlags masked = static_cast<TypeFlags>(m_stat.st_mode);
  masked &= TypeFlags::mask;
  return masked == TypeFlags::regular;
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
  TypeFlags masked = static_cast<TypeFlags>(m_stat.st_mode);
  masked &= TypeFlags::mask;
  return masked == TypeFlags::block;
}

bool FileInfo::is_character_device() const {
#if defined __link
  if (m_is_local) {
    return (m_stat.st_mode & S_IFMT) == S_IFCHR;
  }
#endif

  TypeFlags masked = static_cast<TypeFlags>(m_stat.st_mode);
  masked &= TypeFlags::mask;
  return masked == TypeFlags::character;
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
  TypeFlags masked = static_cast<TypeFlags>(m_stat.st_mode);
  masked &= TypeFlags::mask;
  return masked == TypeFlags::file_socket;
}

u32 FileInfo::size() const { return m_stat.st_size; }

bool FileInfo::is_executable() const { return false; }