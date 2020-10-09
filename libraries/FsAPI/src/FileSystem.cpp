
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
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    FSAPI_LINK_UNLINK(driver(), path.cstring()));
  return *this;
}

FileSystem &FileSystem::copy(const Copy &options) {
  File source(
    options.source_path(),
    OpenMode::read_only()
#if defined __link
      ,
    options.source_driver()
#endif
  );

  File dest(
    options.destination_path(),
    OpenMode::read_only()
#if defined __link
      ,
    options.destination_driver()
#endif
  );

  if (
    API_ASSIGN_ERROR_CODE(api::ErrorCode::io_error, source.status().value())
    < 0) {
    return *this;
  }

  if (
    API_ASSIGN_ERROR_CODE(api::ErrorCode::io_error, dest.status().value())
    < 0) {
    return *this;
  }

  return copy(source, dest, options);
}

FileSystem &
FileSystem::copy(File &source, File &destination, const Copy &options) {
  u32 mode = 0;

  FileInfo source_info = source.get_info();
  if (API_ASSIGN_ERROR_CODE(
        api::ErrorCode::io_error,
        source.status().value())) {
    return *this;
  }

  mode = source_info.permissions().permissions();

  if (mode == 0) {
    mode = 0666;
  }

  if (
    API_ASSIGN_ERROR_CODE(
      api::ErrorCode::io_error,
      destination
        .create(
          options.destination_path(),
          options.overwrite(),
          Permissions(mode & 0777))
        .status()
        .value())
    < 0) {
    return *this;
  }

  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    destination
      .write(
        source,
        File::Write().set_progress_callback(options.progress_callback()))
      .status()
      .value());

  return *this;
}

FileSystem &FileSystem::touch(var::StringView path) {
  char c;
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::no_entity,
    File(path, OpenMode::read_write() FSAPI_LINK_MEMBER_DRIVER_LAST)
      .read(var::View(c))
      .seek(0)
      .write(var::View(c))
      .status()
      .value());
  return *this;
}

FileSystem &FileSystem::rename(const Rename &options) {
#if defined __link
  if (driver() == nullptr) {
    API_ASSIGN_ERROR_CODE(
      api::ErrorCode::io_error,
      ::rename(options.source().cstring(), options.destination().cstring()));
    return *this;
  }
#endif
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    FSAPI_LINK_RENAME(
      driver(),
      options.source().cstring(),
      options.destination().cstring()));
  return *this;
}

bool FileSystem::exists(var::StringView path) {
  return File(path, OpenMode::read_only() FSAPI_LINK_MEMBER_DRIVER_LAST)
    .status()
    .is_success();
}

FileInfo FileSystem::get_info(var::StringView path) {
  struct FSAPI_LINK_STAT_STRUCT stat = {0};
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::no_entity,
    FSAPI_LINK_STAT(driver(), path.cstring(), &stat));
  return FileInfo(stat);
}

FileInfo FileSystem::get_info(int fd) {
  struct FSAPI_LINK_STAT_STRUCT stat = {0};
  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::bad_file_number,
    FSAPI_LINK_FSTAT(driver(), fd, &stat));
  return FileInfo(stat);
}

FileSystem &FileSystem::copy_directory(const Copy &options) {

  var::Vector<var::String> source_contents
    = read_directory(options.source_path(), Recursive::no);

  for (auto entry : source_contents.vector()) {
    var::String entry_path = options.source_path() + "/" + entry;

    var::String destination_entry_path
      = options.destination_path() + "/" + entry;

    FileInfo info = FileSystem(
#if __link
                      options.source_driver()
#endif
                        )
                      .get_info(entry_path);

    if (info.is_directory()) {
      // copy recursively

      if (FileSystem(
#if __link
            options.destination_driver()
#endif
              )
            .create_directory(
              destination_entry_path,
              Permissions::all_access(),
              Recursive::yes)
            .status()
            .is_error()) {
        // error here
        return *this;
      }

      copy_directory(Copy()
                       .set_source_path(entry_path)
                       .set_destination_path(destination_entry_path)
                       .set_progress_callback(options.progress_callback())
#if defined __link
                       .set_source_driver(options.source_driver())
                       .set_destination_driver(options.destination_driver())
#endif
      );

      // check result for errors

    } else if (info.is_file()) {
      copy(Copy()
             .set_source_path(entry_path)
             .set_destination_path(destination_entry_path)
             .set_overwrite(options.overwrite())
             .set_progress_callback(options.progress_callback())
#if defined __link
             .set_source_driver(options.source_driver())
             .set_destination_driver(options.destination_driver())
#endif
      );
      if (status().is_error()) {
        return *this;
      }
    }
  }
  return *this;
}

FileSystem &
FileSystem::remove_directory(var::StringView path, Recursive recursive) {
  int ret = 0;
  if (recursive == Recursive::yes) {
    Dir d = Dir(path FSAPI_LINK_MEMBER_DRIVER_LAST);
    if (d.status().is_success()) {
      var::String entry;
      while ((entry = d.read()).is_empty() == false) {
        var::String entry_path = path + "/" + entry;
        FileInfo info = get_info(entry_path);
        if (info.is_directory()) {
          if (entry != "." && entry != "..") {
            if (remove_directory(entry_path, recursive).status().is_error()) {
              return *this;
            }
          }
        } else {
          if (remove(entry_path).status().is_error()) {
            return *this;
          }
        }
      }
    }
  }

  if (status().is_success()) {
    // this will remove an empty directory or a file
#if defined __link
    if (driver()) {
      remove(path);
    } else {
      API_ASSIGN_ERROR_CODE(api::ErrorCode::no_entity, ::rmdir(path.cstring()));
    }
#else
    remove(path);
#endif
  }

  return *this;
}

var::StringList FileSystem::read_directory(
  var::StringView path,
  std::function<const var::String(const var::String &entry)> filter,
  Recursive is_recursive) {
  return Dir(path FSAPI_LINK_MEMBER_DRIVER_LAST)
    .read_list(filter, is_recursive);
}

var::StringList FileSystem::read_directory(
  var::StringView path,
  Recursive is_recursive) {
  return read_directory(path, nullptr, is_recursive);
}

u32 FileSystem::size(var::StringView name) {
  return get_info(name).size();
}

bool FileSystem::directory_exists(var::StringView path) {
  return Dir(path FSAPI_LINK_MEMBER_DRIVER_LAST).status().is_success();
}

FileSystem &FileSystem::create_directory(
  var::StringView path,
  const Permissions &permissions) {
  int result;

#if defined __link
  if (driver()) {
    result = link_mkdir(driver(), path.cstring(), permissions.permissions());
  } else {
    // open a directory on the local system (not over link)
#if defined __win32
    result = mkdir(path.cstring());
#else
    result = mkdir(path.cstring(), permissions.permissions());
#endif
  }
#else
  result = mkdir(path.cstring(), permissions.permissions());
#endif
  API_ASSIGN_ERROR_CODE(api::ErrorCode::io_error, result);
  return *this;
}

FileSystem &FileSystem::create_directory(
  var::StringView path,
  const Permissions &permissions,
  Recursive is_recursive) {
  if (is_recursive == Recursive::no) {
    return create_directory(path, permissions);
  }
  var::Tokenizer path_tokens = var::Tokenizer(
    path,
    var::Tokenizer::Construct().set_delimeters("/"));
  var::String base_path;

  if (path.find("/") == 0) {
    base_path += "/";
  }

  for (u32 i = 0; i < path_tokens.count(); i++) {
    if (path_tokens.at(i).is_empty() == false) {
      base_path << path_tokens.at(i);
      if (create_directory(base_path, permissions).status().is_error()) {
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

DataFile FileSystem::load_data_file(var::StringView file_path) {
  // read the contents of file_path into this object
  FileInfo info = get_info(file_path);

  DataFile result(OpenMode::append_write_only());

  if (result.data().resize(info.size()).status().is_error()) {
    return DataFile();
  }

  File source_file(
    file_path,
    OpenMode::read_only() FSAPI_LINK_MEMBER_DRIVER_LAST);

  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    result.write(source_file, File::Write())
      .seek(0)
      .set_flags(OpenMode::read_write())
      .status()
      .value());

  return result;
}
