#ifndef FSAPI_FS_FILESYSTEM_HPP
#define FSAPI_FS_FILESYSTEM_HPP

#include "Dir.hpp"

namespace fs {

class FileSystem : public api::Object {
public:
  using IsOverwrite = File::IsOverwrite;
  using IsRecursive = Dir::IsRecursive;

  FileSystem(FSAPI_LINK_DECLARE_DRIVER_NULLPTR);

#if !defined __link
  /*! \details Returns an error if the access is not allowed.
   *
   */
  static int access(const var::String &path, const Access &o_access);
#endif

  bool exists(var::StringView path);

  /*! \details Deletes a file.
   *
   * @return Zero on success
   *
   */
  FileSystem &remove(var::StringView path);

  /*! \details Gets the size of the file.
   *
   * @param path The path to the file
   * @return The number of bytes in the file or less than zero for an error
   *
   */
  u32 size(var::StringView path);

  /*! \details Removes a file or directory.
   *
   * @return Zero on success or -1 for an error
   */
  FileSystem &remove_directory(var::StringView path);

  FileSystem &remove_directory(var::StringView path, IsRecursive recursive);

  bool directory_exists(var::StringView path);

  FileSystem &
  save_copy(var::StringView file_path, IsOverwrite is_overwrite) const;

  FileSystem &create_directory(
    var::StringView path,
    const Permissions &permissions = Permissions(0777));

  FileSystem &create_directory(
    var::StringView path,
    const Permissions &permissions,
    IsRecursive is_recursive);

  var::StringList read_directory(
    const fs::Dir &directory,
    IsRecursive is_recursive = IsRecursive::no);

  var::StringList read_directory(
    const fs::Dir &directory,
    var::StringView (*filter)(var::StringView),
    IsRecursive is_recursive = IsRecursive::no);

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
  FileSystem &rename(const Rename &options);

  FileSystem &touch(var::StringView path);

  FileInfo get_info(var::StringView path);
  FileInfo get_info(int fd);


#if !defined __link
  int access(var::StringView path, const Access &access);
#endif

protected:
  virtual int interface_mkdir(const char *path, int mode) const;
  virtual int interface_rmdir(const char *path) const;
  virtual int interface_unlink(const char *path) const;
  virtual int interface_stat(const char *path, struct stat *stat) const;
  virtual int interface_fstat(int fd, struct stat *stat) const;
  virtual int
  interface_rename(const char *old_name, const char *new_name) const;

private:
#ifdef __link
  API_AF(FileSystem, link_transport_mdriver_t *, driver, nullptr);
#endif
};

} // namespace fs

#endif // FSAPI_FS_FILESYSTEM_HPP
