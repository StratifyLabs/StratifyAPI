#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include "File.hpp"

namespace fs {

class FileSystem : public api::Object {
public:
  enum class Overwrite { no, yes };

  FileSystem(SAPI_LINK_DRIVER_NULLPTR);

#if !defined __link
  /*! \details Returns an error if the access is not allowed.
   *
   */
  static int access(const var::String &path, const Access &o_access);
#endif

  bool exists(const var::StringView &path);

  /*! \details Deletes a file.
   *
   * @return Zero on success
   *
   */
  FileSystem &remove(const var::StringView &path);

  /*! \details Gets the size of the file.
   *
   * @param path The path to the file
   * @return The number of bytes in the file or less than zero for an error
   *
   */
  u32 size(const var::String &path SAPI_LINK_DRIVER_NULLPTR_LAST);

  class CopyOptions {
    API_ACCESS_COMPOUND(CopyOptions, var::StringView, source_path);
    API_ACCESS_COMPOUND(CopyOptions, var::StringView, destination_path);
    API_ACCESS_FUNDAMENTAL(CopyOptions, Overwrite, overwrite, Overwrite::no);
    API_ACCESS_FUNDAMENTAL(
      CopyOptions,
      const api::ProgressCallback *,
      progress_callback,
      nullptr);
#if defined __link
    API_ACCESS_FUNDAMENTAL(
      CopyOptions,
      link_transport_mdriver_t *,
      source_driver,
      nullptr);
    API_ACCESS_FUNDAMENTAL(
      CopyOptions,
      link_transport_mdriver_t *,
      destination_driver,
      nullptr);

#endif
  };

  FileSystem &copy(const CopyOptions &options);

  /*! \details Copies a file from the source to the destination.
   *
   * @return Zero on success
   *
   *
   */
  int copy(
    SourcePath source_path,
    DestinationPath dest_path,
    IsOverwrite overwrite,
    const sys::ProgressCallback *progress_callback = nullptr
#if defined __link
    ,
    SourceLinkDriver source_driver = SourceLinkDriver(nullptr),
    DestinationLinkDriver destination_driver = DestinationLinkDriver(nullptr)
#endif
  );

  FileSystem &
  save_copy(const var::String &file_path, Overwrite is_overwrite) const;

  class RenameOptions {
    API_AC(RenameOptions, var::String, source);
    API_AC(RenameOptions, var::String, destination);
  };

  /*! \details Renames a file.
   *
   * \param old_path Current path to the file (will be old path after rename)
   * \param new_path New path to the file
   * \return Zero on success
   *
   */
  FileSystem &rename(const RenameOptions &options);

  FileSystem &touch(const var::String &path);

  static FileInfo get_info(const var::String &path);
  static FileInfo get_info(int fd);

private:
#ifdef __link
  link_transport_mdriver_t *m_driver = nullptr;
#endif

  class PrivateCopyOptions {
    API_AF(PrivateCopyOptions, File *, source, nullptr);
    API_AF(PrivateCopyOptions, File *, destination, nullptr);
    API_AF(PrivateCopyOptions, var::StringView, source_path, nullptr);
    API_AF(PrivateCopyOptions, var::StringView, destination_path, nullptr);
    API_ACCESS_FUNDAMENTAL(
      CopyOptions,
      const api::ProgressCallback *,
      progress_callback,
      nullptr);
#if defined __link
    API_ACCESS_FUNDAMENTAL(
      CopyOptions,
      link_transport_mdriver_t *,
      source_driver,
      nullptr);
    API_ACCESS_FUNDAMENTAL(
      CopyOptions,
      link_transport_mdriver_t *,
      destination_driver,
      nullptr);
#endif
  };

  FileSystem &copy(const PrivateCopyOptions);

  int copy(
    SourcePath source_path,
    DestinationPath dest_path,
    const sys::ProgressCallback *progress_callback = nullptr
#if defined __link
    ,
    SourceLinkDriver source_driver = SourceLinkDriver(nullptr),
    DestinationLinkDriver destination_driver = DestinationLinkDriver(nullptr)
#endif
  );

  int copy(
    Source source,
    Destination dest,
    SourcePath source_path,
    DestinationPath dest_path,
    const sys::ProgressCallback *progress_callback);

  int copy(
    Source source,
    Destination dest,
    SourcePath source_path,
    DestinationPath dest_path,
    IsOverwrite is_overwrite,
    const sys::ProgressCallback *progress_callback);
};

} // namespace fs

#endif // FILESYSTEM_HPP
