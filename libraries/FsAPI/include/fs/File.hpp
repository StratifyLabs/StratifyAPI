/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef SAPI_FS_FILE_HPP_
#define SAPI_FS_FILE_HPP_

#include <sos/link.h>

#include "api/api.hpp"

#include "chrono/Time.hpp"
#include "FileInfo.hpp"
#include "Path.hpp"

#include "var/Data.hpp"
#include "var/String.hpp"

namespace fs {

class File;

/*! \brief File Class
 * \details This class is used to access files (and devices).  It uses the POSIX
 *functions open(), read(), write(), close(), etc.  You can always call these
 *functions directly or use the standard C library to access files (fopen(),
 *fread(), fwrite()-- these use more memory than this class or the POSIX
 *functions).
 *
 * Here is an example of using this class:
 *
 * ```
 * //md2code:include
 * #include <sapi/fs.hpp>
 * #include <sapi/var.hpp>
 * ```
 *
 * ```
 * //md2code:main
 * File f;
 * String str;
 *
 *	//create a new file and write a string to it
 *	f.create(
 *    File::DestinationPath("/home/myfile.txt"),
 *    File::IsOverwrite(true)
 *    );
 *	str = "Hello New File!\n";
 *	f.write(str);
 *	f.close();
 *
 *  //Now open the file we just closed
 *	f.open(
 *   arg::FilePath("/home/myfile.txt"),
 *	  OpenFlags::read_only()
 *	  );
 *	str = "";
 *	str = f.gets(); //reads a line from the file
 *	f.close();
 *
 * //This is what was read from the file
 *	printf("The String is %s\n", str.cstring());
 *
 *	File::remove(
 *   arg::SourceFilePath("/home/myfile.txt")
 *  ); //delete the file
 *
 *	int fd;
 *	if(1){
 *	  File file;
 *   file.open(
 *     arg::FilePath("/home/file.txt"),
 *     OpenFlags::read_only()
 *   );
 *   fd = file.fileno();
 *	  file.set_keep_open(); //will keep the file open after ~File()
 *	  //~File() is called here
 *	 }
 *
 *	char buffer[16];
 *	read(fd, buffer, 16); //OK because file.set_keep_open() was used
 *	 return 0;
 * ```
 *
 */
class File : public api::Object, public FileInfoFlags {
public:
  enum class IsOverwrite { no, yes };

  enum class Whence {
    set /*! Set the location of the file descriptor */ = LINK_SEEK_SET,
    current /*! Set the location relative to the current location */
    = LINK_SEEK_CUR,
    end /*! Set the location relative to the end of the file or device */
    = LINK_SEEK_END
  };

  File(FSAPI_LINK_DECLARE_DRIVER_NULLPTR);
  File(
    var::StringView name,
    OpenMode flags
    = OpenMode::read_write() FSAPI_LINK_DECLARE_DRIVER_NULLPTR_LAST);

  ~File();

  static File create(
    var::StringView path,
    IsOverwrite is_overwrite,
    Permissions perms
    = Permissions(0666) FSAPI_LINK_DECLARE_DRIVER_NULLPTR_LAST);

  FileInfo get_info() const;

  /*! \details Returns whether the file will
   * be closed upon object destruction.
   *
   * The default value on object creation is true.
   *
   * \sa set_keep_open()
   *
   */
  bool is_keep_open() const { return m_is_keep_open; }

  /*! \details Returns the file size. */
  size_t size() const;
  ssize_t size_signed() const { return static_cast<ssize_t>(size()); }

  /*! \details Returns the location of the cursor in the device or file. */
  int location() const;

  /*! \details Return the current flags for the file. */
  int flags() const;

  /*! \details Return the file number for accessing the file or device. */
  int fileno() const;

  /*! \details Sets the file to stay open even
   * after the destructor has been called.
   *
   * The default value on object creation is true.
   *
   * \code
   * #include <sapi/sys.hpp>
   *
   * int fd;
   * if(1){
   *   File f;
   *   f.open("/home/data.txt");
   *   fd = f.fileno();
   *   f.set_keep_open();
   *   //~File() will be called here
   * }
   *
   * //fd is still open because set_keep_open() was called
   * char buffer[16];
   * read(fd, buffer, 16);
   * \endcode
   *
   */
  File &set_keep_open(bool value = true) {
    m_is_keep_open = value;
    return *this;
  }

  /*! \details Excute system fsync() on the file. */
  File &sync();

  /*! \details Reads the file.
   *
   * @return The number of bytes read or less than zero on an error
   *
   *
   */
  File &read(void *buf, int size);

  /*! \details Reads the file into a var::Data object.
   *
   * @param data The destination data object
   * @return The number of bytes read
   *
   * This method will read up to data.size() bytes.
   *
   */
  File &read(const var::View &item) {
    return read(item.to_void(), item.size());
  }

  /*! \details Write the file.
   *
   * @return The number of bytes written or less than zero on an error
   */
  File &write(const void *buf, int size);

  /*! \details Writes the file using a var::Data object. */
  File &write(const var::View &item) {
    return write(item.to_const_void(), item.size());
  }

  class WriteOptions {
  public:
    API_ACCESS_FUNDAMENTAL(WriteOptions, u32, location, static_cast<u32>(-1));
    API_ACCESS_FUNDAMENTAL(
      WriteOptions,
      u32,
      page_size,
      FSAPI_LINK_DEFAULT_PAGE_SIZE);
    API_ACCESS_FUNDAMENTAL(WriteOptions, size_t, size, static_cast<size_t>(-1));
    API_ACCESS_FUNDAMENTAL(
      WriteOptions,
      const api::ProgressCallback *,
      progress_callback,
      nullptr);
  };

  File &write(File &source_file, const WriteOptions &options);

  /*! \details Reads a line from a file.
   *
   * @param buf Destination buffer
   * @param nbyte Number of bytes available in buffer
   * @param timeout_msec Timeout in ms if line does not arrive
   * @param terminator Terminating character of the line (default is newline)
   * @return Number of bytes received
   */
  File &
  readline(char *buf, int nbyte, int timeout_msec, char terminator = '\n');

  /*! \details Seeks to a location in the file or on the device. */
  File &seek(int location, Whence whence = Whence::set);

  /*! \details Reads a line in to the var::String until end-of-file or \a term
   * is reached. */
  var::String gets(char term = '\n');

  class IoctlOptions {
    API_ACCESS_FUNDAMENTAL(IoctlOptions, int, request, 0);
    API_ACCESS_FUNDAMENTAL(IoctlOptions, void *, argument, nullptr);
  };

  /*! \details Executes an IO control request.
   *
   *
   * This method is typically only implemented for devices
   * and other special file systems.
   *
   */
  File &ioctl(int request, void *arg);
  File &ioctl(int request) { return ioctl(request, nullptr); }

  File &ioctl(const IoctlOptions &options) {
    ioctl(options.request(), options.argument());
    return *this;
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

  /*! \details Closes the file or device. */
  File &close();

protected:
  /*! \details Opens a file.
   *
   * @param name The path to the file
   * @param flags The flags used to open the flag (e.g. File::READONLY)
   * @return Zero on success
   */
  File &open(
    var::StringView name,
    OpenMode flags = OpenMode::read_write(),
    Permissions perms = Permissions(0666));

#ifdef __link
  API_AF(File, link_transport_mdriver_t *, driver, nullptr);
#endif

  virtual int interface_open(const char *path, int flags, int mode) const;
  virtual int interface_lseek(int fd, int offset, int whence) const;
  virtual int interface_read(int fd, void *buf, int nbyte) const;
  virtual int interface_write(int fd, const void *buf, int nbyte) const;
  virtual int interface_ioctl(int fd, int request, void *argument) const;
  virtual int interface_close(int fd) const;
  virtual int interface_fsync(int fd) const;

private:
  constexpr static size_t m_gets_buffer_size = 128;
  bool m_is_keep_open;
  int m_fd;

  int fstat(struct FSAPI_LINK_STAT_STRUCT *st);

  File &internal_create(
    var::StringView path,
    IsOverwrite is_overwrite,
    const Permissions &perms = Permissions(0666));
};

template <class Derived> class FileAccess : public File {
public:
  FileAccess<Derived>(
    var::StringView name,
    const OpenMode &flags
    = OpenMode::read_write() FSAPI_LINK_DECLARE_DRIVER_NULLPTR_LAST)
    : File(name, flags FSAPI_LINK_INHERIT_DRIVER_LAST) {}

  Derived &set_keep_open(bool value = true) {
    return static_cast<Derived &>(File::set_keep_open(value));
  }

  Derived &
  open(var::StringView path, const OpenMode &flags = OpenMode::read_write()) {
    return static_cast<Derived &>(File::open(path, flags));
  }

  Derived &create(
    var::StringView path,
    IsOverwrite is_overwrite,
    const Permissions &perms = Permissions(0666)) {
    return static_cast<Derived &>(File::create(path, is_overwrite, perms));
  }

  Derived &close() { return static_cast<Derived &>(File::close()); }
  Derived &sync() { return static_cast<Derived &>(File::sync()); }

  Derived &read(void *buf, size_t size) {
    return static_cast<Derived &>(File::read(buf, size));
  }

  Derived &read(const var::View &item) {
    return static_cast<Derived &>(File::read(item));
  }

  Derived &write(const void *buf, size_t size) {
    return static_cast<Derived &>(File::write(buf, size));
  }

  Derived &write(const var::View &item) {
    return static_cast<Derived &>(File::write(item));
  }

  Derived &write(File &source_file, const WriteOptions &options) {
    return static_cast<Derived &>(File::write(source_file, options));
  }

  Derived &seek(int location, Whence whence = Whence::set) {
    return static_cast<Derived &>(File::seek(location, whence));
  }

  Derived &ioctl(int request, void *arg) {
    return static_cast<Derived &>(File::ioctl(request, arg));
  }
  Derived &ioctl(const IoctlOptions &options) {
    return static_cast<Derived &>(File::ioctl(options));
  }

  Derived &set_fileno(int fd) {
    return static_cast<Derived &>(File::set_fileno(fd));
  }

  Derived &set_fileno(const File &file) {
    return static_cast<Derived &>(File::set_fileno(file));
  }

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
  DataFile(const OpenMode &flags = OpenMode::read_write())
    : FileAccess(""), m_open_flags(flags) {
    m_location = 0;
  }

  explicit DataFile(File &file_to_load);

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
  mutable int m_location; // offset location for seeking/reading/writing
  mutable OpenMode m_open_flags;
  mutable var::Data m_data;

  int interface_open(const char *path, int flags, int mode) const override {
    MCU_UNUSED_ARGUMENT(path);
    MCU_UNUSED_ARGUMENT(mode);
    m_open_flags = OpenMode(flags);
    return 0;
  }

  int interface_close(int fd) const override { return 0; }
  int interface_read(int fd, void *buf, int nbyte) const override;
  int interface_write(int fd, const void *buf, int nbyte) const override;
  int interface_lseek(int fd, int offset, int whence) const override;
  int interface_ioctl(int fd, int request, void *argument) const override {
    MCU_UNUSED_ARGUMENT(request);
    MCU_UNUSED_ARGUMENT(argument);
    return 0;
  }
  int interface_fsync(int fd) const override { return 0; }
};

class ItemFile : public FileAccess<ItemFile> {
public:
  /*! \details Constructs a data file. */
  ItemFile(const OpenMode &flags = OpenMode::read_write())
    : FileAccess(""), m_open_flags(flags) {
    m_location = 0;
  }

  ItemFile &set_flags(const OpenMode &open_flags) {
    m_open_flags = open_flags;
    return *this;
  }

  const OpenMode &flags() const { return m_open_flags; }

  /*! \details Accesses (read-only) the member data object. */
  const var::View &item() const { return m_reference; }
  /*! \details Accesses the member data object. */
  var::View &item() { return m_reference; }

private:
  mutable int m_location; // offset location for seeking/reading/writing
  mutable OpenMode m_open_flags;
  var::View m_reference;

  int interface_open(const char *path, int flags, int mode) const override {
    MCU_UNUSED_ARGUMENT(path);
    MCU_UNUSED_ARGUMENT(mode);
    m_open_flags = OpenMode(flags);
    if (this->flags().is_append()) {
      return -1;
    }
    return 0;
  }
  int interface_close(int fd) const override { return 0; }
  int interface_fsync(int fd) const override { return 0; }
  int interface_ioctl(int fd, int request, void *argument) const override {
    return -1;
  }

  int interface_lseek(int fd, int offset, int whence) const override;
  int interface_read(int fd, void *buf, int nbyte) const override;
  int interface_write(int fd, const void *buf, int nbyte) const override;
};

class NullFile : public FileAccess<NullFile> {
public:
  NullFile() : FileAccess("") {}

private:

  int interface_open(const char *path, int flags, int mode) const override {
    return 0;
  }
  int interface_close(int fd) const override { return 0; }
  int interface_read(int fd, void *buf, int nbyte) const override;
  int interface_write(int fd, const void *buf, int nbyte) const override;
  int interface_ioctl(int fd, int request, void *argument) const override;
  int interface_lseek(int fd, int location, int whence) const override;
  int interface_fsync(int fd) const override { return 0; }
};

} // namespace fs

#endif /* SAPI_FS_FILE_HPP_ */
