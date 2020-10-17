/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef SAPI_FS_FILE_HPP_
#define SAPI_FS_FILE_HPP_

#include <sos/link.h>

#include "api/api.hpp"

#include "FileInfo.hpp"
#include "Path.hpp"
#include "chrono/MicroTime.hpp"
#include "var/Base64.hpp"

#include "var/Data.hpp"
#include "var/String.hpp"

namespace fs {

class File;

class File : public api::ExecutionContext, public FileInfoFlags {
public:
  enum class IsOverwrite { no, yes };

  enum class Whence {
    set /*! Set the location of the file descriptor */ = LINK_SEEK_SET,
    current /*! Set the location relative to the current location */
    = LINK_SEEK_CUR,
    end /*! Set the location relative to the end of the file or device */
    = LINK_SEEK_END
  };

  explicit File(
    var::StringView name,
    OpenMode flags
    = OpenMode::read_write() FSAPI_LINK_DECLARE_DRIVER_NULLPTR_LAST);

  File(
    IsOverwrite is_overwrite,
    var::StringView path,
    OpenMode flags = OpenMode::read_write(),
    Permissions perms
    = Permissions(0666) FSAPI_LINK_DECLARE_DRIVER_NULLPTR_LAST);

  File(const File &file) = delete;
  File &operator=(const File &file) = delete;

  File(File &&a) { std::swap(m_fd, a.m_fd); }
  File &operator=(File &&a) {
    std::swap(m_fd, a.m_fd);
    return *this;
  }

  virtual ~File();

  /*! \details Returns the file size. */
  size_t size() const;
  ssize_t size_signed() const { return static_cast<ssize_t>(size()); }

  /*! \details Returns the location of the cursor in the device or file. */
  int location() const;

  /*! \details Return the current flags for the file. */
  int flags() const;

  /*! \details Return the file number for accessing the file or device. */
  int fileno() const;

  File &set_keep_open(bool value = true) {
    m_is_keep_open = value;
    return *this;
  }

  /*! \details Excute system fsync() on the file. */
  const File &sync() const;

  /*! \details Reads the file.
   *
   * @return The number of bytes read or less than zero on an error
   *
   *
   */
  const File &read(void *buf, int size) const;
  File &read(void *buf, int size) {
    return API_CONST_CAST_SELF(File, read, buf, size);
  }

  /*! \details Reads the file into a var::Data object.
   *
   * @param data The destination data object
   * @return The number of bytes read
   *
   * This method will read up to data.size() bytes.
   *
   */
  const File &read(var::View view) const {
    return read(view.to_void(), view.size());
  }

  File &read(var::View view) { return read(view.to_void(), view.size()); }

  /*! \details Write the file.
   *
   * @return The number of bytes written or less than zero on an error
   */
  const File &write(const void *buf, int size) const;
  File &write(const void *buf, int size) {
    return API_CONST_CAST_SELF(File, write, buf, size);
  }

  /*! \details Writes the file using a var::Data object. */
  const File &write(var::View view) const {
    return write(view.to_const_void(), view.size());
  }

  File &write(var::View view) {
    return write(view.to_const_void(), view.size());
  }

  class Write {
  public:
    Write()
      : m_location(-1), m_page_size(FSAPI_LINK_DEFAULT_PAGE_SIZE),
        m_size(static_cast<size_t>(-1)) {}
    API_ACCESS_FUNDAMENTAL(Write, int, location, -1);
    API_ACCESS_FUNDAMENTAL(Write, u32, page_size, FSAPI_LINK_DEFAULT_PAGE_SIZE);
    API_ACCESS_FUNDAMENTAL(Write, size_t, size, static_cast<size_t>(-1));
    API_ACCESS_FUNDAMENTAL(Write, char, terminator, 0);
    API_ACCESS_FUNDAMENTAL(Write, chrono::MicroTime, timeout, 0_microseconds);
    API_ACCESS_FUNDAMENTAL(
      Write,
      chrono::MicroTime,
      retry_delay,
      10_milliseconds);
    API_ACCESS_FUNDAMENTAL(
      Write,
      const var::Transformer *,
      transformer,
      nullptr);
    API_ACCESS_FUNDAMENTAL(
      Write,
      const api::ProgressCallback *,
      progress_callback,
      nullptr);
  };

  const File &
  write(const File &source_file, const Write &options = Write()) const;

  File &write(const File &source_file, const Write &options = Write()) {
    return API_CONST_CAST_SELF(File, write, source_file, options);
  }

  const File &write(
    const File &source_file,
    const var::Transformer &transformer,
    const Write &options = Write()) const {
    return write(source_file, Write(options).set_transformer(&transformer));
  }

  File &write(
    const File &source_file,
    const var::Transformer &transformer,
    const Write &options = Write()) {
    return API_CONST_CAST_SELF(File, write, source_file, transformer, options);
  }

  /*! \details Seeks to a location in the file or on the device. */
  const File &seek(int location, Whence whence = Whence::set) const;
  File &seek(int location, Whence whence = Whence::set) {
    return API_CONST_CAST_SELF(File, seek, location, whence);
  }

  /*! \details Reads a line in to the var::String until end-of-file or \a term
   * is reached. */
  var::String gets(char term = '\n') const;

  class Ioctl {
    API_ACCESS_FUNDAMENTAL(Ioctl, int, request, 0);
    API_ACCESS_FUNDAMENTAL(Ioctl, void *, argument, nullptr);
  };

  /*! \details Executes an IO control request.
   *
   *
   * This method is typically only implemented for devices
   * and other special file systems.
   *
   */
  const File &ioctl(int request, void *arg) const;
  File &ioctl(int request, void *arg) {
    return API_CONST_CAST_SELF(File, ioctl, request, arg);
  }

  const File &ioctl(int request) const { return ioctl(request, nullptr); }
  File &ioctl(int request) { return ioctl(request, nullptr); }

  const File &ioctl(const Ioctl &options) const {
    return ioctl(options.request(), options.argument());
  }

  File &ioctl(const Ioctl &options) {
    return ioctl(options.request(), options.argument());
  }

  /*! \details Sets the file descriptor for this object. */
  File &set_fileno(int fd) {
    m_fd = fd;
    return *this;
  }

  /*! \details Sets the file descriptor of this object to the file descriptor of
   * file.
   *
   *
   * The reference \a file must already be opened and have a valid file
   * number. This won't bind the file number to file just assign
   * it based on the state of file.fileno() when this method is called.
   *
   *
   */
  File &set_fileno(const File &file) {
    m_fd = file.fileno();
    return *this;
  }

protected:
  File() = default;

  bool is_keep_open() const { return m_is_keep_open; }

  virtual int interface_lseek(int offset, int whence) const;
  virtual int interface_read(void *buf, int nbyte) const;
  virtual int interface_write(const void *buf, int nbyte) const;
  virtual int interface_ioctl(int request, void *argument) const;

  int interface_fsync(int fd) const;

  static void
  fake_seek(int &location, const size_t size, int offset, int whence);

  static int fake_ioctl(int request, void *argument) {
    MCU_UNUSED_ARGUMENT(request);
    MCU_UNUSED_ARGUMENT(argument);
    errno = ENOTSUP;
    return -1;
  }

private:
#ifdef __link
  API_AF(File, link_transport_mdriver_t *, driver, nullptr);
#endif
  constexpr static size_t m_gets_buffer_size = 128;
  bool m_is_keep_open = false;
  int m_fd = -1;

  int fstat(struct stat *st);

  void internal_create(
    IsOverwrite is_overwrite,
    var::StringView path,
    OpenMode open_mode,
    Permissions perms);

  void open(
    var::StringView name,
    OpenMode flags = OpenMode::read_write(),
    Permissions perms = Permissions(0666));

  // open/close are part of construction/deconstruction and can't be virtual
  void close();
  int internal_close(int fd) const;
  int internal_open(const char *path, int flags, int mode) const;
};

template <class Derived> class FileAccess : public File {
public:

  Derived &set_keep_open(bool value = true) {
    return static_cast<Derived &>(File::set_keep_open(value));
  }

  const Derived &sync() const {
    return static_cast<const Derived &>(File::sync());
  }

  const Derived &read(void *buf, size_t size) const {
    return static_cast<const Derived &>(File::read(buf, size));
  }

  Derived &read(void *buf, size_t size) {
    return static_cast<Derived &>(File::read(buf, size));
  }

  const Derived &read(var::View view) const {
    return static_cast<const Derived &>(File::read(view));
  }

  Derived &read(var::View view) {
    return static_cast<Derived &>(File::read(view));
  }

  const Derived &write(const void *buf, size_t size) const {
    return static_cast<const Derived &>(File::write(buf, size));
  }

  Derived &write(const void *buf, size_t size) {
    return static_cast<Derived &>(File::write(buf, size));
  }

  const Derived &write(var::View view) const {
    return static_cast<const Derived &>(File::write(view));
  }

  Derived &write(var::View view) {
    return static_cast<Derived &>(File::write(view));
  }

  const Derived &
  write(const File &source_file, const Write &options = Write()) const {
    return static_cast<const Derived &>(File::write(source_file, options));
  }

  Derived &write(const File &source_file, const Write &options = Write()) {
    return static_cast<Derived &>(File::write(source_file, options));
  }

  const Derived &write(
    const File &source_file,
    const var::Transformer &transformer,
    const Write &options = Write()) const {
    return static_cast<const Derived &>(
      File::write(source_file, transformer, options));
  }

  Derived &write(
    const File &source_file,
    const var::Transformer &transformer,
    const Write &options = Write()) {
    return static_cast<Derived &>(
      File::write(source_file, transformer, options));
  }

  const Derived &seek(int location, Whence whence = Whence::set) const {
    return static_cast<const Derived &>(File::seek(location, whence));
  }

  Derived &seek(int location, Whence whence = Whence::set) {
    return static_cast<Derived &>(File::seek(location, whence));
  }

  const Derived &ioctl(int request, void *arg) const {
    return static_cast<const Derived &>(File::ioctl(request, arg));
  }

  Derived &ioctl(int request, void *arg) {
    return static_cast<Derived &>(File::ioctl(request, arg));
  }

  const Derived &ioctl(const Ioctl &options) const {
    return static_cast<const Derived &>(File::ioctl(options));
  }

  Derived &ioctl(const Ioctl &options) {
    return static_cast<Derived &>(File::ioctl(options));
  }

  Derived &set_fileno(int fd) {
    return static_cast<Derived &>(File::set_fileno(fd));
  }

  Derived &set_fileno(const File &file) {
    return static_cast<Derived &>(File::set_fileno(file));
  }

protected:
  FileAccess<Derived>() = default;

private:
};

/*! \brief DataFile Class
 * \details The DataFile class is a class
 * that uses a var::Data object to allow
 * fs::File operations. This allows for an
 * easy way to have a valid fs::File object
 * that can be passed to methods that read/write
 * data from the file.
 *
 *
 */
class DataFile : public FileAccess<DataFile> {
public:
  /*! \details Constructs a data file. */

  DataFile(DataFile &&file) = default;
  DataFile &operator=(DataFile &&file) = default;

  DataFile(const OpenMode &flags = OpenMode::append_read_write())
    : m_open_flags(flags) {
    m_location = 0;
  }

  explicit DataFile(File &file_to_load);

  DataFile &reserve(size_t size) {
    m_data.reserve(size);
    return *this;
  }

  DataFile &resize(size_t size) {
    m_data.resize(size);
    return *this;
  }

  DataFile &copy(var::View view) {
    m_data.copy(view);
    return *this;
  }

  DataFile &set_flags(OpenMode open_flags) {
    m_open_flags = open_flags;
    return *this;
  }
  const OpenMode &flags() const { return m_open_flags; }

  /*! \details Accesses (read-only) the member data object. */
  const var::Data &data() const { return m_data; }
  /*! \details Accesses the member data object. */
  var::Data &data() { return m_data; }

private:
  mutable int m_location = 0; // offset location for seeking/reading/writing
  mutable OpenMode m_open_flags;
  mutable var::Data m_data;

  int interface_read(void *buf, int nbyte) const override;
  int interface_write(const void *buf, int nbyte) const override;
  int interface_lseek(int offset, int whence) const override;
  int interface_ioctl(int request, void *argument) const override {
    return fake_ioctl(request, argument);
  }
};

class ViewFile : public FileAccess<ViewFile> {
public:
  /*! \details Constructs a data file. */

  ViewFile(ViewFile &&view_file) = default;
  ViewFile &operator=(ViewFile &&view_file) = default;

  ViewFile(var::View view)
    : m_open_flags(
      view.is_read_only() ? OpenMode::read_only() : OpenMode::read_write()) {
    m_view = view;
  }

  ViewFile &set_flags(const OpenMode &open_flags) {
    m_open_flags = open_flags;
    return *this;
  }

  const OpenMode &flags() const { return m_open_flags; }

  /*! \details Accesses (read-only) the member data object. */
  const var::View &item() const { return m_view; }
  /*! \details Accesses the member data object. */
  var::View &item() { return m_view; }

private:
  mutable int m_location = 0; // offset location for seeking/reading/writing
  mutable OpenMode m_open_flags;
  var::View m_view;

  int interface_ioctl(int request, void *argument) const override {
    return fake_ioctl(request, argument);
  }
  int interface_lseek(int offset, int whence) const override;
  int interface_read(void *buf, int nbyte) const override;
  int interface_write(const void *buf, int nbyte) const override;
};

class NullFile : public FileAccess<NullFile> {
public:
  NullFile(size_t size = 0) : m_size(size) {}

  NullFile(const NullFile &) = delete;
  NullFile &operator=(const NullFile &) = delete;
  NullFile(NullFile &&) = default;
  NullFile &operator=(NullFile &&) = default;

private:
  mutable int m_location = 0;
  mutable size_t m_size;

  int interface_read(void *buf, int nbyte) const override;
  int interface_write(const void *buf, int nbyte) const override;
  int interface_lseek(int offset, int whence) const override;

  int interface_ioctl(int request, void *argument) const override {
    return fake_ioctl(request, argument);
  }
};

} // namespace fs

#endif /* SAPI_FS_FILE_HPP_ */
