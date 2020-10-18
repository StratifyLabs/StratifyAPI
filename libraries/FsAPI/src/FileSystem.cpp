
#include <sys/stat.h>

#include "chrono/ClockTime.hpp"
#include "fs/Dir.hpp"
#include "sys/System.hpp"
#include "var/Tokenizer.hpp"

#include "fs/FileSystem.hpp"
#include "local.h"

using namespace fs;

FileSystem::FileSystem(FSAPI_LINK_DECLARE_DRIVER) {
  LINK_SET_DRIVER((*this), link_driver);
}

const FileSystem &FileSystem::remove(var::StringView path) const {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL("", interface_unlink(Path(path).cstring()));
  return *this;
}

const FileSystem &FileSystem::touch(var::StringView path) const {
  API_RETURN_VALUE_IF_ERROR(*this);
  char c;
  API_SYSTEM_CALL(
    "",
    File(path, OpenMode::read_write())
      .read(var::View(c))
      .seek(0)
      .write(var::View(c))
      .return_value());
  return *this;
}

const FileSystem &FileSystem::rename(const Rename &options) const {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL(
    Path(options.source()).cstring(),
    interface_rename(
      Path(options.source()).cstring(),
      Path(options.destination()).cstring()));
  return *this;
}

bool FileSystem::exists(var::StringView path) const {
  API_RETURN_VALUE_IF_ERROR(false);
  bool result = File(path, OpenMode::read_only()).is_success();
  reset_error();
  return result;
}

FileInfo FileSystem::get_info(var::StringView path) const {
  API_RETURN_VALUE_IF_ERROR(FileInfo());
  struct stat stat = {0};
  API_SYSTEM_CALL(
    Path(path).cstring(),
    interface_stat(Path(path).cstring(), &stat));
  return FileInfo(stat);
}

FileInfo FileSystem::get_info(const File &file) const {
  API_RETURN_VALUE_IF_ERROR(FileInfo());
  struct stat stat = {0};
  API_SYSTEM_CALL("", interface_fstat(file.fileno(), &stat));
  return FileInfo(stat);
}

const FileSystem &FileSystem::remove_directory(
  var::StringView path,
  IsRecursive recursive) const {

  if (recursive == IsRecursive::yes) {
    Dir d(path FSAPI_LINK_MEMBER_DRIVER_LAST);

    var::String entry;
    while ((entry = d.read()).is_empty() == false) {
      var::String entry_path = path + "/" + entry;
      FileInfo info = get_info(entry_path);
      if (info.is_directory()) {
        if (entry != "." && entry != "..") {
          if (remove_directory(entry_path, recursive).is_error()) {
            return *this;
          }
        }

      } else {
        if (remove(entry_path).is_error()) {
          return *this;
        }
      }
    }
  }

  remove_directory(path);
  return *this;
}

const FileSystem &FileSystem::remove_directory(var::StringView path) const {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL(Path(path).cstring(), interface_rmdir(Path(path).cstring()));
  return *this;
}

FileSystem::PathList FileSystem::read_directory(
  const fs::Dir &directory,
  IsRecursive is_recursive,
  bool (*exclude)(var::StringView entry)) const {
  PathList result;
  bool is_the_end = false;

  do {
    const Path entry = Path(var::StringView(directory.read()));
    if (entry.path().is_empty()) {
      is_the_end = true;
    }

    if (
      (exclude == nullptr || !exclude(entry.path())) && !entry.path().is_empty()
      && (entry.path() != ".") && (entry.path() != "..")) {

      if (is_recursive == IsRecursive::yes) {

        const Path entry_path
          = Path(directory.path()).append("/").append(entry.path());
        FileInfo info = get_info(entry_path.cstring());

        if (info.is_directory()) {
          PathList intermediate_result = read_directory(
            Dir(entry_path.cstring() FSAPI_LINK_MEMBER_DRIVER_LAST),
            is_recursive,
            exclude);

          for (const auto &intermediate_entry : intermediate_result) {
            result.push_back(Path(entry_path)
                               .append("/")
                               .append(intermediate_entry.cstring()));
          }
        } else {
          result.push_back(entry_path);
        }
      } else {
        result.push_back(entry);
      }
    }
  } while (!is_the_end);

  return std::move(result);
}

u32 FileSystem::size(var::StringView name) const {
  return get_info(name).size();
}

bool FileSystem::directory_exists(var::StringView path) const {
  API_RETURN_VALUE_IF_ERROR(false);
  bool result = Dir(path).is_success();
  reset_error();
  return result;
}

Permissions FileSystem::get_permissions(var::StringView path) const {
  const var::StringView parent = Path(path).parent_directory();
  if (parent.is_empty()) {
    return get_info(".").permissions();
  }

  return get_info(parent).permissions();
}

const FileSystem &FileSystem::create_directory(
  var::StringView path,
  const Permissions &permissions) const {

  const Permissions use_permissions
    = permissions.permissions() == 0 ? get_permissions(path) : permissions;

  API_SYSTEM_CALL(
    Path(path).cstring(),
    interface_mkdir(Path(path).cstring(), use_permissions.permissions()));
  return *this;
}

const FileSystem &FileSystem::create_directory(
  var::StringView path,
  IsRecursive is_recursive,
  const Permissions &permissions) const {

  if (is_recursive == IsRecursive::no) {
    return create_directory(path, permissions);
  }

  var::Tokenizer path_tokens
    = var::Tokenizer(path, var::Tokenizer::Construct().set_delimeters("/"));
  var::String base_path;

  // tokenizer will strip the first / and create an empty token
  if (path.length() && path.front() == '/') {
    base_path += "/";
  }

  for (u32 i = 0; i < path_tokens.count(); i++) {
    if (path_tokens.at(i).is_empty() == false) {
      base_path += path_tokens.at(i);
      if (create_directory(base_path.cstring(), permissions)
            .is_error()) {
        return *this;
      }
      base_path += "/";
    }
  }

  return *this;
}

#if !defined __link
int FileSystem::access(var::StringView path, const Access &access) {
  return ::access(path.cstring(), static_cast<int>(access.o_access()));
}
#endif

int FileSystem::interface_mkdir(const char *path, int mode) const {
  return FSAPI_LINK_MKDIR(driver(), path, mode);
}

int FileSystem::interface_rmdir(const char *path) const {
  return FSAPI_LINK_RMDIR(driver(), path);
}

int FileSystem::interface_unlink(const char *path) const {
  return FSAPI_LINK_UNLINK(driver(), path);
}

int FileSystem::interface_stat(const char *path, struct stat *stat) const {
  return ::stat(path, stat);
}
int FileSystem::interface_fstat(int fd, struct stat *stat) const {
  return ::fstat(fd, stat);
}
int FileSystem::interface_rename(const char *old_name, const char *new_name)
  const {
  return FSAPI_LINK_RENAME(driver(), old_name, new_name);
}

TemporaryDirectory::TemporaryDirectory(var::StringView parent) {
  m_path
    = (parent.is_empty() ? (var::String(sys::System::user_data_path()) + "/")
                         : var::String(""))
      + chrono::ClockTime::get_unique_string();
  FileSystem().create_directory(m_path);
  if (is_error()) {
    m_path.clear();
  }
}

TemporaryDirectory::~TemporaryDirectory() {
  if (m_path.is_empty() == false) {
    FileSystem().remove_directory(m_path, FileSystem::IsRecursive::yes);
  }
}
