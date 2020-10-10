#ifndef FSAPI_FS_FILESYSTEM_HPP
#define FSAPI_FS_FILESYSTEM_HPP

#include "Dir.hpp"

namespace fs {

class FileSystem : public api::Object {
public:
  using Overwrite = File::IsOverwrite;
  using Recursive = Dir::IsRecursive;

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

  class Copy {
    API_ACCESS_COMPOUND(Copy, var::StringView, source_path);
    API_ACCESS_COMPOUND(Copy, var::StringView, destination_path);
    API_ACCESS_FUNDAMENTAL(Copy, Overwrite, overwrite, Overwrite::no);
    API_ACCESS_FUNDAMENTAL(Copy, Recursive, recursive, Recursive::no);
    API_ACCESS_FUNDAMENTAL(
      Copy,
      const api::ProgressCallback *,
      progress_callback,
      nullptr);
#if defined __link
    API_ACCESS_FUNDAMENTAL(
      Copy,
      link_transport_mdriver_t *,
      source_driver,
      nullptr);
    API_ACCESS_FUNDAMENTAL(
      Copy,
      link_transport_mdriver_t *,
      destination_driver,
      nullptr);

#endif
  };

  FileSystem &copy(const Copy &options);

  FileSystem &copy_directory(const Copy &options);

  /*! \details Removes a file or directory.
   *
   * @return Zero on success or -1 for an error
   */
  FileSystem &remove_directory(var::StringView path);

  FileSystem &
  remove_directory(var::StringView path, Recursive recursive);

  bool directory_exists(var::StringView path);

  FileSystem &
  save_copy(var::StringView file_path, Overwrite is_overwrite) const;

  FileSystem &create_directory(
    var::StringView path,
    const Permissions &permissions = Permissions(0777));

  FileSystem &create_directory(
    var::StringView path,
    const Permissions &permissions,
    Recursive is_recursive);

  var::StringList read_directory(
    var::StringView path,
    Recursive is_recursive = Recursive::no);

  var::StringList read_directory(
    var::StringView path,
    var::StringView (*filter)(var::StringView),
    Recursive is_recursive = Recursive::no);

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

  DataFile load_data_file(var::StringView path);

#if !defined __link
  int access(var::StringView path, const Access &access);
#endif

private:
#ifdef __link
  API_AF(FileSystem, link_transport_mdriver_t *, driver, nullptr);
#endif

  FileSystem &copy(File &source, File &destination, const Copy &options);
};

} // namespace fs

#endif // FSAPI_FS_FILESYSTEM_HPP
