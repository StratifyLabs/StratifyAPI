
#include <sys/stat.h>

#include "fs/Dir.hpp"
#include "var/Tokenizer.hpp"

#include "fs/FileSystem.hpp"
#include "local.h"

using namespace fs;

FileSystem::FileSystem(FSAPI_LINK_DECLARE_DRIVER) {
  LINK_SET_DRIVER((*this), link_driver);
}

FileSystem &FileSystem::remove(var::StringView path) {
  API_SYSTEM_CALL("", interface_unlink(path.cstring()));
  return *this;
}

FileSystem &FileSystem::touch(var::StringView path) {
  char c;
  API_SYSTEM_CALL(
    "",
    File(path, OpenMode::read_write())
      .read(var::View(c))
      .seek(0)
      .write(var::View(c))
      .status()
      .value());
  return *this;
}

FileSystem &FileSystem::rename(const Rename &options) {
  API_SYSTEM_CALL(
    "",
    interface_rename(
      options.source().cstring(),
      options.destination().cstring()));
  return *this;
}

bool FileSystem::exists(var::StringView path) {
  API_RETURN_VALUE_IF_ERROR(false);
  bool result = File(path, OpenMode::read_only()).status().is_success();
  reset_error_context();
  return result;
}

FileInfo FileSystem::get_info(var::StringView path) {
  API_RETURN_VALUE_IF_ERROR(FileInfo());
  struct stat stat = {0};
  API_SYSTEM_CALL("get stat", interface_stat(path.cstring(), &stat));
  return FileInfo(stat);
}

FileInfo FileSystem::get_info(const File &file) {
  API_RETURN_VALUE_IF_ERROR(FileInfo());
  struct stat stat = {0};
  API_SYSTEM_CALL("get info fstat", interface_fstat(file.fileno(), &stat));
  return FileInfo(stat);
}

FileSystem &
FileSystem::remove_directory(var::StringView path, IsRecursive recursive) {
  int ret = 0;
  if (recursive == IsRecursive::yes) {
    Dir d(path);
    if (d.status().is_success()) {
      var::String entry;
      while ((entry = d.read()).is_empty() == false) {
        var::String entry_path = path + "/" + entry;
        FileInfo info = get_info(entry_path.cstring());
        if (info.is_directory()) {
          if (entry != "." && entry != "..") {
            if (remove_directory(entry_path.cstring(), recursive)
                  .status()
                  .is_error()) {
              return *this;
            }
          }
        } else {
          if (remove(entry_path.cstring()).status().is_error()) {
            return *this;
          }
        }
      }
    }
  }

  remove(path);

  return *this;
}

FileSystem &remove_directory(var::StringView path) {
  API_RETURN_VALUE_IF_ERROR(*this);
  API_SYSTEM_CALL(path.cstring(), interface_rmdir(path));
}

var::StringList FileSystem::read_directory(
  const fs::Dir &directory,
  var::StringView (*filter)(var::StringView entry),
  IsRecursive is_recursive) {
  var::Vector<var::String> result;
  var::String entry;
  bool is_the_end = false;

  do {
    entry.clear();
    entry = directory.read();
    if (entry.is_empty()) {
      is_the_end = true;
    }
    if (filter != nullptr) {
      entry = var::String(filter(entry.cstring()));
    }
    if (!entry.is_empty() && (entry != ".") && (entry != "..")) {

      if (is_recursive == IsRecursive::yes) {
        const var::String entry_path = directory.path() + "/" + entry;
        FileInfo info = get_info(entry_path);

        if (info.is_directory()) {
          var::StringList intermediate_result = read_directory(
            Dir(entry_path FSAPI_LINK_MEMBER_DRIVER_LAST),
            filter,
            is_recursive);

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

  return std::move(result);
}

var::StringList
FileSystem::read_directory(const fs::Dir &directory, IsRecursive is_recursive) {
  return read_directory(directory, nullptr, is_recursive);
}

u32 FileSystem::size(var::StringView name) { return get_info(name).size(); }

bool FileSystem::directory_exists(var::StringView path) {
  API_RETURN_VALUE_IF_ERROR(false);
  bool result = Dir(path).status().is_success();
  reset_error_context();
  return result;
}

FileSystem &FileSystem::create_directory(
  var::StringView path,
  const Permissions &permissions) {
  API_SYSTEM_CALL(
    "",
    interface_mkdir(path.cstring(), permissions.permissions()));
  return *this;
}

FileSystem &FileSystem::create_directory(
  var::StringView path,
  const Permissions &permissions,
  IsRecursive is_recursive) {

  if (is_recursive == IsRecursive::no) {
    return create_directory(path, permissions);
  }
  var::Tokenizer path_tokens
    = var::Tokenizer(path, var::Tokenizer::Construct().set_delimeters("/"));
  var::String base_path;

  if (path.find("/") == 0) {
    base_path += "/";
  }

  for (u32 i = 0; i < path_tokens.count(); i++) {
    if (path_tokens.at(i).is_empty() == false) {
      base_path += path_tokens.at(i);
      if (create_directory(base_path.cstring(), permissions)
            .status()
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
  return ::mkdir(path, mode);
}

int FileSystem::interface_rmdir(const char *path) const {
  return ::rmdir(path);
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
