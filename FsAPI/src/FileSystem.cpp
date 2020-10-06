#include "FsAPI/FileSystem.hpp"

using namespace fs;

FileSystem::FileSystem(SAPI_LINK_DRIVER) { SAPI_LINK_CONSTRUCT_DRIVER(); }

FileSystem &FileSystem::remove(const var::StringView &path) {
  if (link_unlink(link_driver.argument(), path.cstring()) < 0) {
    return api::error_code_fs_failed_to_unlink;
  }
  return 0;
}

FileSystem &copy(const CopyOptions &options) {}

FileSystem &copy(const PrivateCopyOptions &options) {
  MCU_UNUSED_ARGUMENT(source_path);
  struct SAPI_LINK_STAT st;

  if (
    API_ASSIGN_ERROR_CODE(
      api::ErrorCode::io_error,
      options.source()->fstat(&st))
    < 0) {
    return *this;
  }

  if ((st.st_mode & 0666) == 0) {
    st.st_mode = 0666;
  }

  if (
    options.destination()->create(
      dest_path.argument(),
      is_overwrite,
      Permissions(st.st_mode & 0777))
    < 0) {
    return api::error_code_fs_failed_to_create;
  }

  int result = dest.argument().write(
    source.argument(),
    PageSize(SAPI_LINK_DEFAULT_PAGE_SIZE),
    Size(size_t(-1)),
    progress_callback);
  if (result < 0) {
    return api::error_code_fs_failed_to_write;
  }
  return result;
}

int FileSystem::copy(
  SourcePath source_path,
  DestinationPath dest_path,
  const sys::ProgressCallback *progress_callback
#if defined __link
  ,
  SourceLinkDriver source_driver,
  DestinationLinkDriver destination_driver
#endif
) {
  File source;
  File dest;

  LINK_SET_DRIVER(source, source_driver.argument());
  LINK_SET_DRIVER(dest, destination_driver.argument());

  if (source.open(source_path.argument(), OpenFlags::read_only()) < 0) {
    return api::error_code_fs_failed_to_open;
  }

  return copy(
    Source(source),
    Destination(dest),
    source_path,
    dest_path,
    progress_callback);
}

int FileSystem::copy(const CopyOptions &options) {
  File source;
  File dest;

  LINK_SET_DRIVER(source, options.source_driver());
  LINK_SET_DRIVER(dest, options.destination_driver());

  if (source.open(options.source_path(), OpenFlags::read_only()) < 0) {
    return api::error_code_fs_failed_to_open;
  }

  return copy(
    Source(source),
    Destination(dest),
    SourcePath(options.source_path()),
    DestinationPath(options.destination_path()),
    options.progress_callback());
}

int FileSystem::copy(
  SourcePath source_path,
  DestinationPath dest_path,
  IsOverwrite is_overwrite,
  const sys::ProgressCallback *progress_callback
#if defined __link
  ,
  SourceLinkDriver source_driver,
  DestinationLinkDriver destination_driver
#endif
) {

  File source;
  File dest;

  LINK_SET_DRIVER(source, source_driver.argument());
  LINK_SET_DRIVER(dest, destination_driver.argument());

  if (source.open(source_path.argument(), OpenFlags::read_only()) < 0) {
    return api::error_code_fs_failed_to_open;
  }

  return copy(
    Source(source),
    Destination(dest),
    source_path,
    dest_path,
    is_overwrite,
    progress_callback);
}

FileSystem &FileSystem::save_copy(
  const var::String &file_path,
  Overwrite is_overwrite) const {
  File copy_file;
  if (
    set_error_number_if_error(copy_file.create(file_path, is_overwrite)) < 0) {
    return return_value();
  }
  return copy_file.write(*this);
}

FileSystem &FileSystem::touch(const var::String &path) {
  File touch_file;
  if (touch_file.open(path, OpenFlags::read_write()) < 0) {
    return api::error_code_fs_failed_to_open;
  } else {
    char c;
    if (touch_file.read(var::Reference(c)) != 1) {
      return api::error_code_fs_failed_to_read;
    }
    if (touch_file.write(FileSystem::Location(0), var::Reference(c)) != 1) {
      return api::error_code_fs_failed_to_write;
    }
  }
  return 0;
}

int FileSystem::rename(const RenameOptions &options) {
#if defined __link
  if (options.driver() == nullptr) {
    return ::rename(
      options.source().cstring(),
      options.destination().cstring());
  }
#endif
  return link_rename(
    options.driver(),
    options.source().cstring(),
    options.destination().cstring());
}

int FileSystem::copy(
  Source source,
  Destination dest,
  SourcePath source_path,
  DestinationPath dest_path,
  IsOverwrite is_overwrite,
  const sys::ProgressCallback *progress_callback) {

  MCU_UNUSED_ARGUMENT(source_path);
  struct SAPI_LINK_STAT st;

  if (source.argument().fstat(&st) < 0) {
    return api::error_code_fs_failed_to_stat;
  }

  if ((st.st_mode & 0666) == 0) {
    st.st_mode = 0666;
  }

  if (
    dest.argument().create(
      dest_path.argument(),
      is_overwrite,
      Permissions(st.st_mode & 0777))
    < 0) {
    return api::error_code_fs_failed_to_create;
  }

  int result = dest.argument().write(
    source.argument(),
    PageSize(SAPI_LINK_DEFAULT_PAGE_SIZE),
    Size(size_t(-1)),
    progress_callback);
  if (result < 0) {
    return api::error_code_fs_failed_to_write;
  }
  return result;
}

bool FileSystem::exists(const var::String &path SAPI_LINK_DRIVER_LAST) {

  File f;

  LINK_SET_DRIVER(f, link_driver.argument());

  if (f.open(path, fs::OpenFlags::read_only()) < 0) {
    return false;
  }
  f.close();

  return f.result().is_error() == false;
}

FileInfo FileSystem::get_info(const var::String &path SAPI_LINK_DRIVER_LAST) {
  struct SAPI_LINK_STAT stat;
  memset(&stat, 0, sizeof(stat));
  FileSystem::stat(
    path,
    stat
#if defined __link
    ,
    link_driver
#endif
  );
  return Stat(stat);
}

Stat FileSystem::get_info(Descriptor fd SAPI_LINK_DRIVER_LAST) {
  struct SAPI_LINK_STAT stat;
  memset(&stat, 0, sizeof(stat));
  link_fstat(link_driver.argument(), fd.argument(), &stat);
  return Stat(stat);
}
