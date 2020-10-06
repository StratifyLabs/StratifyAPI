/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef SAPI_FS_FILE_HPP_
#define SAPI_FS_FILE_HPP_

#include <sos/link.h>

#include "Api.hpp"

#include "ChronoAPI/Time.hpp"
#include "FileInfo.hpp"

#include "VarAPI/Data.hpp"
#include "VarAPI/String.hpp"

#if !defined __link
#define SAPI_LINK_DRIVER_NULLPTR
#define SAPI_LINK_DRIVER_NULLPTR_LAST
#define SAPI_LINK_DRIVER
#define SAPI_LINK_DRIVER_LAST
#define SAPI_LINK_STAT stat
#define SAPI_LINK_DEFAULT_PAGE_SIZE 512
#define SAPI_LINK_DRIVER_ARGUMENT
#define SAPI_LINK_SET_DRIVER(x, y)
#else
#define SAPI_LINK_STAT link_stat
#define SAPI_LINK_DRIVER_NULLPTR                                               \
  fs::File::LinkDriver link_driver = fs::File::LinkDriver(nullptr)
#define SAPI_LINK_DRIVER_NULLPTR_LAST                                          \
  , fs::File::LinkDriver link_driver = fs::File::LinkDriver(nullptr)
#define SAPI_LINK_DRIVER fs::File::LinkDriver link_driver
#define SAPI_LINK_DRIVER_LAST , fs::File::LinkDriver link_driver
#define SAPI_LINK_DEFAULT_PAGE_SIZE 4096
#define SAPI_LINK_DRIVER_ARGUMENT link_driver.argument(),
#define SAPI_LINK_SET_DRIVER(x, y) x.set_driver(y)
#endif

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
class File : public virtual api::WorkObject {
public:

  enum class Overwrite { no, yes };

  enum class Whence {
    set /*! Set the location of the file descriptor */ = LINK_SEEK_SET,
    current /*! Set the location relative to the current location */
    = LINK_SEEK_CUR,
    end /*! Set the location relative to the end of the file or device */
    = LINK_SEEK_END
  };

#if defined __link
  using ExplicitLinkDriver
    = arg::Argument<link_transport_mdriver_t *, struct FileLinkDriverTag>;
  using SourceLinkDriver
    = arg::Argument<link_transport_mdriver_t *, struct FileSourceLinkDriverTag>;
  using DestinationLinkDriver = arg::
    Argument<link_transport_mdriver_t *, struct FileDestinationLinkDriverTag>;
  using LinkDriver = arg::ImplicitArgument<
    link_transport_mdriver_t *,
    struct FileImplicitLinkDriverTag,
    ExplicitLinkDriver>;
#endif

  File(SAPI_LINK_DRIVER_NULLPTR);

  ~File();

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
  virtual u32 size() const;

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

  /*! \details Opens a file.
   *
   * @param name The path to the file
   * @param flags The flags used to open the flag (e.g. File::READONLY)
   * @return Zero on success
   */
  File &open(
    const var::String &name,
    const OpenFlags &flags = OpenFlags::read_write());

  /*! \details Creates a new file (using the open() method).
   *
   * @param path The path to the file
   * @param is_overwrite Overwrite (truncate) the existing file (defaults to
   * true)
   * @param perms The permissions to assign to the newly created file
   */
  File &create(
    const var::String &path,
    Overwrite is_overwrite,
    const Permissions &perms = Permissions(0666));

  /*! \details Closes the file or device. */
  File &close();

  /*! \details Excute system fsync() on the file. */
  File &sync();

  /*! \details Reads the file.
   *
   * @return The number of bytes read or less than zero on an error
   *
   *
   */
  File &read(void *buf, size_t size);

  /*! \details Reads the file into a var::Data object.
   *
   * @param data The destination data object
   * @return The number of bytes read
   *
   * This method will read up to data.size() bytes.
   *
   */
  File &read(const var::Item &item) {
    int result = read(item.to_void(), reference.size());
    return result;
  }

  /*! \details Write the file.
   *
   * @return The number of bytes written or less than zero on an error
   */
  File &write(const void *buf, size_t size);

  /*! \details Writes the file using a var::Data object. */
  File &write(const var::Item &item) const {
    return write(item.to_const_void(), item.size());
  }

  class WriteOptions {
  public:
    API_ACCESS_FUNDAMENTAL(WriteOptions, u32, location, static_cast<u32>(-1));
    API_ACCESS_FUNDAMENTAL(
      WriteOptions,
      u32,
      page_size,
      SAPI_LINK_DEFAULT_PAGE_SIZE);
    API_ACCESS_FUNDAMENTAL(WriteOptions, size_t, size, static_cast<size_t>(-1));
    API_ACCESS_FUNDAMENTAL(
      WriteOptions,
      const sys::ProgressCallback *,
      progress_callback,
      nullptr);
  };

  File &write(const File &source_file, const WriteOptions &options);

  /*! \details Reads a line from a file.
   *
   * @param buf Destination buffer
   * @param nbyte Number of bytes available in buffer
   * @param timeout_msec Timeout in ms if line does not arrive
   * @param terminator Terminating character of the line (default is newline)
   * @return Number of bytes received
   */
  int readline(char *buf, int nbyte, int timeout_msec, char terminator = '\n')
    const;

  /*! \details Seeks to a location in the file or on the device. */
  File &seek(int location, Whence whence = Whence::set);

  /*! \details Reads a line in to the var::String until end-of-file or \a term
   * is reached. */
  var::String gets(char term = '\n') const;

#ifdef __link
  static void set_default_driver(LinkDriver driver) {
    m_default_driver = driver.argument();
  }

  void set_driver(link_transport_mdriver_t *driver) { m_driver = driver; }

  link_transport_mdriver_t *driver() const { return m_driver; }
  static link_transport_mdriver_t *default_driver() { return m_default_driver; }

#endif

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

protected:
#ifdef __link
  mutable link_transport_mdriver_t *m_driver;
  static link_transport_mdriver_t *m_default_driver;
  int check_driver() const {
    if (m_driver == 0) {
      m_driver = m_default_driver;
      if (m_driver == 0) {
        set_error_number(2);
        return -1;
      }
    }
    return 0;
  }

  static link_transport_mdriver_t *check_driver(LinkDriver driver) {
    if (driver.argument() == 0) {
      return m_default_driver;
    }
    return driver.argument();
  }

#endif

  mutable int m_fd;

private:
  bool m_is_keep_open;
  constexpr static size_t m_gets_buffer_size = 128;

  virtual int open(const char *path, int flags, int mode);
  virtual int lseek(int fd, int offset, int whence);
  virtual int read(int fd, void *buf, int nbyte);
  virtual int write(int fd, const void *buf, int nbyte);
  virtual int ioctl(int fd, int request, void *argument);
  virtual int close(int fd);
  virtual int fsync(int fd);
};

template <class Derived> class FileAccess : public File {
public:
  Derived &set_keep_open(bool value = true) {
    return static_cast<Derived &>(File::set_keep_open(value));
  }

  Derived &open(
    const var::StringView &path,
    const OpenFlags &flags = OpenFlags::read_write()) {
    return static_cast<Derived &>(File::open(path, flags));
  }

  Derived &create(
    const var::StringView &path,
    Overwrite is_overwrite,
    const Permissions &perms = Permissions(0666)) {
    return static_cast<Derived &>(File::create(path, is_overwrite, perms));
  }

  Derived &close() { return static_cast<Derived &>(File::close()); }
  Derived &sync() { return static_cast<Derived &>(File::sync()); }

  Derived &read(void *buf, size_t size) {
    return static_cast<Derived &>(File::read(buf, size));
  }

  Derived &read(const var::Item &item) {
    return static_cast<Derived &>(File::read(item));
  }

  Derived &write(const void *buf, size_t size) {
    return static_cast<Derived &>(File::write(buf, size));
  }

  Derived &write(const var::Item &item) const {
    return static_cast<Derived &>(File::write(item));
  }

  Derived &write(const File &source_file, const WriteOptions &options) {
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
  DataFile(const OpenFlags &flags = OpenFlags::read_write())
    : m_open_flags(flags) {
    m_fd = 0;
    m_location = 0;
  }
  virtual ~DataFile() { m_fd = -1; }

  explicit DataFile(fs::File::Path file_path);
  explicit DataFile(const fs::File &file_to_load);

  /*! \details Returns the size of the
   * file (size of the data).
   *
   */
  u32 size() const override { return data().size(); }

  using File::read;
  using File::seek;
  using File::write;

  OpenFlags &flags() { return m_open_flags; }
  const OpenFlags &flags() const { return m_open_flags; }

  /*! \details Accesses (read-only) the member data object. */
  const var::Data &data() const { return m_data; }
  /*! \details Accesses the member data object. */
  var::Data &data() { return m_data; }

private:
  mutable int m_location; // offset location for seeking/reading/writing
  OpenFlags m_open_flags;
  mutable var::Data m_data;

  int open(const char *path, int flags, int mode) override {
    MCU_UNUSED_ARGUMENT(path);
    this->flags() = flags;
    return 0;
  }
  int close(int fd) override { return 0; }
  int read(int fd, void *buf, size_t nbyte) override;
  int write(int fd, const void *buf, int nbyte) override;
  int lseek(int fd, int offset, int whence) override;
  int ioctl(int fd, int request, void *argument) override {
    MCU_UNUSED_ARGUMENT(request);
    MCU_UNUSED_ARGUMENT(argument);
    return 0;
  }
  int fsync(int fd) override { return 0; }
};

class ItemFile : public File {
public:
  /*! \details Constructs a data file. */
  ItemFile(const OpenFlags &flags = OpenFlags::read_write())
    : m_open_flags(flags) {
    m_location = 0;
  }

  virtual ~ItemFile() { m_fd = -1; }

  /*! \details Returns the size of the
   * file (size of the data).
   *
   */
  u32 size() const override { return reference().size(); }

  using File::read;
  using File::seek;
  using File::write;

  OpenFlags &flags() { return m_open_flags; }
  const OpenFlags &flags() const { return m_open_flags; }

  /*! \details Accesses (read-only) the member data object. */
  const var::Item &item() const { return m_reference; }
  /*! \details Accesses the member data object. */
  var::Item &item() { return m_reference; }

private:
  int m_location; // offset location for seeking/reading/writing
  OpenFlags m_open_flags;
  var::Item m_reference;

  int open(const char *path, int flags, int mode) override {
    MCU_UNUSED_ARGUMENT(path);
    this->flags() = flags;
    if (flags.is_append()) {
      return -1;
    }
    return 0;
  }

  int lseek(int fd, int offset, int whence) override { return 0; }
  int close(int fd) override { return 0; }
  int read(int fd, void *buf, int nbyte) override;
  int write(int fd, const void *buf, int nbyte) override;
  int ioctl(int fd, int request, void *argument) override;
  int fsync(int fd) override { return 0; }
};

class NullFile : public File {
public:
  /*! \details Constructs a null file. */
  NullFile() { m_fd = 0; }

  /*! \details Returns zero.
   *
   */
  u32 size() const override { return 0; }

  using File::read;
  using File::seek;
  using File::write;

  OpenFlags &flags() { return m_open_flags; }
  const OpenFlags &flags() const { return m_open_flags; }

private:
  OpenFlags m_open_flags;

  int open(const char *path, int flags, int mode) override;
  int close(int fd) override { return 0; }
  int read(int fd, void *buf, int nbyte) override;
  int write(int fd, const void *buf, int nbyte) override;
  int ioctl(int fd, int request, void *argument) override;
  int lseek(int fd, int location, int whence) override;
  int fsync(int fd) override { return 0; }
};

} // namespace fs

#endif /* SAPI_FS_FILE_HPP_ */
