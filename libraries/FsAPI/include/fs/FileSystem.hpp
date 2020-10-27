#ifndef FSAPI_FS_FILESYSTEM_HPP
#define FSAPI_FS_FILESYSTEM_HPP

#include "Dir.hpp"

namespace fs {

using PathList = var::Vector<var::PathString>;

class FileSystem : public api::ExecutionContext {
public:
  using IsOverwrite = File::IsOverwrite;
  using IsRecursive = Dir::IsRecursive;

  FileSystem(FSAPI_LINK_DECLARE_DRIVER_NULLPTR);

#if !defined __link
  Access access(const var::StringView path);
#endif

  bool exists(var::StringView path) const;

  const FileSystem &remove(var::StringView path) const;
  const FileSystem &remove_directory(var::StringView path) const;

  const FileSystem &
  remove_directory(var::StringView path, IsRecursive recursive) const;

  bool directory_exists(var::StringView path) const;

  const FileSystem &create_directory(
    var::StringView path,
    const Permissions &permissions = Permissions(0)) const;

  const FileSystem &create_directory(
    var::StringView path,
    IsRecursive is_recursive,
    const Permissions &permissions = Permissions(0)) const;


  PathList read_directory(
    const fs::Dir &directory,
    IsRecursive is_recursive = IsRecursive::no,
    bool (*exclude)(var::StringView) = nullptr) const;

  class Rename {
    API_AC(Rename, var::StringView, source);
    API_AC(Rename, var::StringView, destination);
  };

  /*! \details Renames a file.
   *
   * \param old_path Current path to the file (will be old path after rename)
   * \param new_path New path to the file
   * \return Zero on success
   *
   */
  const FileSystem &rename(const Rename &options) const;
  inline const FileSystem &operator()(const Rename &options) const {
    return rename(options);
  }

  const FileSystem &touch(var::StringView path) const;

  FileInfo get_info(var::StringView path) const;
  FileInfo get_info(const File &file) const;

#if !defined __link
  int access(var::StringView path, const Access &access);
#endif

protected:
  Permissions get_permissions(var::StringView path) const;

  int interface_mkdir(const char *path, int mode) const;
  int interface_rmdir(const char *path) const;
  int interface_unlink(const char *path) const;
  int interface_stat(const char *path, struct stat *stat) const;
  int interface_fstat(int fd, struct stat *stat) const;
  int interface_rename(const char *old_name, const char *new_name) const;

private:
#ifdef __link
  API_AF(FileSystem, link_transport_mdriver_t *, driver, nullptr);
#endif
};

class TemporaryDirectory : public api::ExecutionContext {
public:
  explicit TemporaryDirectory(var::StringView parent = "");
  ~TemporaryDirectory();

private:
  API_AC(TemporaryDirectory, var::PathString, path);
};

} // namespace fs

namespace printer {
class Printer;
Printer &operator<<(Printer &printer, const fs::PathList &a);
} // namespace printer

#endif // FSAPI_FS_FILESYSTEM_HPP
