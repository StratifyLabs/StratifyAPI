/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef SAPI_FS_FILE_HPP_
#define SAPI_FS_FILE_HPP_

#include <sos/link.h>

#include "../api/SysObject.hpp"
#include "../chrono/MicroTime.hpp"
#include "Stat.hpp"
#include "../arg/Argument.hpp"

#include "../var/Data.hpp"
#include "../var/String.hpp"
#include "../sys/ProgressCallback.hpp"

#if !defined __link
#define SAPI_LINK_DRIVER_NULLPTR
#define SAPI_LINK_DRIVER_NULLPTR_LAST
#define SAPI_LINK_DRIVER
#define SAPI_LINK_DRIVER_LAST
#define SAPI_LINK_STAT stat
#define SAPI_LINK_DEFAULT_PAGE_SIZE 512
#else
#define SAPI_LINK_STAT link_stat
#define SAPI_LINK_DRIVER_NULLPTR fs::File::LinkDriver link_driver = fs::File::LinkDriver(nullptr)
#define SAPI_LINK_DRIVER_NULLPTR_LAST , fs::File::LinkDriver link_driver = fs::File::LinkDriver(nullptr)
#define SAPI_LINK_DRIVER fs::File::LinkDriver link_driver
#define SAPI_LINK_DRIVER_LAST , fs::File::LinkDriver link_driver
#define SAPI_LINK_DEFAULT_PAGE_SIZE 4096
#endif


namespace fs {


/*! \brief File Class
 * \details This class is used to access files (and devices).  It uses the POSIX functions open(), read(), write(), close(), etc.  You
 * can always call these functions directly or use the standard C library to access files (fopen(), fread(), fwrite()--
 * these use more memory than this class or the POSIX functions).
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

	using Source = arg::Argument<const File &, struct FileSourceTag>;
	using Destination = arg::Argument<File &, struct FileDestinationTag>;

	using Path = arg::Argument<const var::String &, struct FilePathTag>;
	using SourcePath = arg::Argument<const var::String &, struct FileSourcePathTag>;
	using DestinationPath = arg::Argument<const var::String &, struct FileDestinationPathTag>;
	using IsOverwrite = var::Reference::IsOverwrite;

	using SourceReference = var::Reference::Source;
	using DestinationReference = var::Reference::Destination;

	using SourceBuffer = var::Reference::SourceBuffer;
	using DestinationBuffer = var::Reference::DestinationBuffer;

	using Size = var::Reference::Size;
	using PageSize = var::Reference::PageSize;

	using Descriptor = arg::Argument<int, struct FileDescriptorTag>;
	using ImplicitDescriptor = arg::ImplicitArgument<int, struct FileImplicitDescriptorTag, Descriptor>;

	using IoRequest = arg::Argument<int, struct FileIoRequestTag>;
	using IoArgument = arg::Argument<void*, struct FileIoArgumentTag>;
	using IoConstArgument = arg::Argument<const void*, struct FileIoConstArgumentTag>;
	using IoIntArgument = arg::Argument<int, struct FileIoIntArgumentTag>;

	using Location = arg::Argument<int, struct FileLocationTag>;
	using OwnerId = arg::Argument<int, struct FileOwnerIdTag>;
	using GroupId = arg::Argument<int, struct FileGroupIdTag>;

	enum whence {
		SET /*! Set the location of the file descriptor */ = LINK_SEEK_SET,
		CURRENT /*! Set the location relative to the current location */ = LINK_SEEK_CUR,
		END /*! Set the location relative to the end of the file or device */ = LINK_SEEK_END,
		whence_set = LINK_SEEK_SET,
		whence_current = LINK_SEEK_CUR,
		whence_end = LINK_SEEK_END
	};

#if defined __link
	using LinkDriver = arg::Argument<link_transport_mdriver_t*, struct FileLinkDriverTag >;
	using SourceLinkDriver = arg::Argument<link_transport_mdriver_t*, struct FileSourceLinkDriverTag >;
	using DestinationLinkDriver = arg::Argument<link_transport_mdriver_t*, struct FileDestinationLinkDriverTag >;
	using ImplicitLinkDriver = arg::ImplicitArgument<link_transport_mdriver_t*, struct FileImplicitLinkDriverTag, LinkDriver >;
#endif

	File(
			SAPI_LINK_DRIVER_NULLPTR
			);
	static bool exists(
			const var::String & path
			SAPI_LINK_DRIVER_NULLPTR_LAST
			);
	static Stat get_info(
			const var::String & path
			SAPI_LINK_DRIVER_NULLPTR_LAST
			);
	static Stat get_info(
			Descriptor fd
			SAPI_LINK_DRIVER_NULLPTR_LAST
			);


	~File();


	Stat get_info() const {
		return get_info(Descriptor(fileno()));
	}

	/*! \details Gets the name of the file from a given path.
	 *
	 * \code
	 * const char * path = "/app/flash/HelloWorld";
	 * printf("Name is %s", File::name(path));
	 * \endcode
	 *
	 * The above code will output:
	 * \code
	 * Name is HelloWorld
	 * \endcode
	 *
	 */
	static const var::String name(
			const var::String & path
			);

	static var::String parent_directory(
			const var::String & path
			);

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
	File & set_keep_open(bool value = true){
		m_is_keep_open = value;
		return *this;
	}

	/*! \details Returns whether the file will
	 * be closed upon object destruction.
	 *
	 * The default value on object creation is true.
	 *
	 * \sa set_keep_open()
	 *
	 */
	bool is_keep_open() const {
		return m_is_keep_open;
	}


#if !defined __link
	/*! \details Returns an error if the access is not allowed.
		*
		*/
	static int access(
			const var::String & path,
			const Access & o_access
			);
#endif

	/*! \details Returns a pointer to the file suffix.
	 *
	 * @return A pointer to the suffix
	 *
	 * For example:
	 *
	 * \code
		* const var::String path = "/home/data.txt";
		* printf("Suffix is %s", File::suffix(path).cstring());
	 * \endcode
	 *
	 * The above code will output:
	 * \code
	 * Suffix is txt
	 * \endcode
	 *
	 */
	static const var::String suffix(
			const var::String & path
			);

	/*! \details Deletes a file.
	 *
	 * @return Zero on success
	 *
	 */
	static int remove(
			const var::String & path
			SAPI_LINK_DRIVER_NULLPTR_LAST
			);

	/*! \details Gets file stat data.
	 *
	 * @return Zero on success
	 *
	 */
	static int stat(
			const var::String & path,
			struct SAPI_LINK_STAT & st
			SAPI_LINK_DRIVER_NULLPTR_LAST
			);


	int fstat(
			struct SAPI_LINK_STAT * st
			) const;

	/*! \details Gets the size of the file.
	 *
	 * @param path The path to the file
	 * @return The number of bytes in the file or less than zero for an error
	 *
	 */
	static u32 size(
			const var::String & path
			SAPI_LINK_DRIVER_NULLPTR_LAST
			);


	/*! \details Opens a file.
	 *
	 * @param name The path to the file
	 * @param flags The flags used to open the flag (e.g. File::READONLY)
	 * @return Zero on success
	 */
	virtual int open(
			const var::String & name,
			const OpenFlags & flags = OpenFlags::read_write()
			);

	/*! \details Opens a file.
	 *
	 * @param name The filename to open
	 * @param flags The access ie RDWR
	 * @param permissions Permission settings
	 * @return Zero on success
	 *
	 * If the object already has a file open, the
	 * file will be closed.
	 *
	 */
	int open(
			const var::String & path,
			const OpenFlags & flags,
			const Permissions & permissions
			);

	/*! \details Creates a new file (using the open() method).
	 *
	 * @param path The path to the file
	 * @param is_overwrite Overwrite (truncate) the existing file (defaults to true)
	 * @param perms The permissions to assign to the newly created file
	 */
	int create(
			const var::String & path,
			IsOverwrite is_overwrite,
			const Permissions & perms = Permissions(0666)
			);

	/*! \details Returns the file size. */
	virtual u32 size() const;

	/*! \details Returns the location of the cursor in the device or file. */
	int location() const;

	/*! \details Return the current flags for the file. */
	int flags() const;

	/*! \details Return the file number for accessing the file or device. */
	int fileno() const;

	/*! \details Closes the file or device. */
	virtual int close();

	/*! \details Reads the file.
	 *
	 * @return The number of bytes read or less than zero on an error
	 *
	 *
	 * ```
	 * //md2code:main
	 * File f;
	 * char buffer[64];
	 * f.read(
	 *   arg::DestinationBuffer(buffer),
	 *   arg::Size(64)
	 *	);
	 *
	 * //the above works but this is better
	 * f.read(
	 *   arg::DestinationData( DataReference(buffer) )
	 *	);
	 * ```
	 *
	 */
	virtual int read(
			void * buf,
			Size size
			) const;

	/*! \details Reads the file into a var::Data object.
		*
		* @param data The destination data object
		* @return The number of bytes read
		*
		* This method will read up to data.size() bytes.
		*
		*/
	int read(const var::Reference & reference) const {
		int result = read(
					reference.to_void(),
					Size(reference.size())
					);
		return result;
	}

	/*! \details Write the file.
	 *
	 * @return The number of bytes written or less than zero on an error
	 */
	virtual int write(
			const void * buf,
			Size size
			) const;

	/*! \details Writes the file using a var::Data object. */
	int write(const var::Reference & reference) const {
		return write(
					reference.to_const_void(),
					Size(reference.size())
					);
	}

	/*! \details Reads the file.
	 *
	 * @return The number of bytes read or less than zero on an error
	 */
	int read(
			Location location,
			void * buf,
			Size size
			) const;

	/*! \details Reads the file using a var::Data object. */
	int read(
			Location location,
			const var::Reference & reference
			) const {
		int result = read(
					location,
					reference.to_void(),
					Size(reference.size())
					);
		return result;
	}

	/*! \details Writes the file at the location specified.
	 *
	 * @param loc Location to write (not application to character devices)
	 * @param buf Pointer to the source data
	 * @param nbyte Number of bytes to write
		* @return Number of bytes successfully written or less than zero with errno set
	 */
	int write(
			Location location,
			const void * buf,
			Size size
			) const;

	/*! \details Writes the file using a var::Data object at the location specified. */
	int write(
			Location location,
			const var::Reference & reference
			) const {
		return write(
					location,
					reference.to_const_void(),
					Size(reference.size())
					);
	}

	int write(
			const File & source_file,
			PageSize page_size = PageSize(SAPI_LINK_DEFAULT_PAGE_SIZE),
			Size size = Size(size_t(-1))
			) const;

	int write(
			Location location,
			const File & source_file,
			PageSize page_size = PageSize(SAPI_LINK_DEFAULT_PAGE_SIZE),
			Size size = Size(size_t(-1))
			) const {
		seek(location, SET);
		return write(source_file, page_size, size);
	}

	int write(
			const File & source_file,
			PageSize page_size,
			Size size,
			const sys::ProgressCallback * progress_callback
			) const;

	int write(
			Location location,
			const File & source_file,
			PageSize page_size,
			Size size,
			const sys::ProgressCallback * progress_callback
			) const {
		seek(location, SET);
		return write(source_file, page_size, size, progress_callback);
	}

	/*! \details Reads a line from a file.
	 *
	 * @param buf Destination buffer
	 * @param nbyte Number of bytes available in buffer
	 * @param timeout_msec Timeout in ms if line does not arrive
	 * @param terminator Terminating character of the line (default is newline)
	 * @return Number of bytes received
	 */
	int readline(char * buf, int nbyte, int timeout_msec, char terminator = '\n') const;

	const File& operator<<(const var::Reference & a) const {
		write(a); return *this;
	}

	const File& operator>>(var::Reference & a) const {
		read(a);
		return *this;
	}

	/*! \details Seeks to a location in the file or on the device. */
	virtual int seek(
			int location,
			enum whence whence = SET
			) const;

	int seek(
			Location location,
			enum whence whence
			) const {
		return seek(location.argument(), whence);
	}


	/*! \details Reads a line in to the var::String until end-of-file or \a term is reached. */
	var::String gets(char term = '\n') const;

	const char * gets(var::String & s, char term = '\n') const;


	API_DEPRECATED("Use gets(var::String & s) instead")
	char * gets(char * s, int n, char term = '\n') const;

#ifdef __link
	static void set_default_driver(
			LinkDriver driver
			){
		m_default_driver = driver.argument();
	}

	void set_driver(
			link_transport_mdriver_t * driver
			){
		m_driver = driver;
	}

	link_transport_mdriver_t * driver() const { return m_driver; }
	static link_transport_mdriver_t * default_driver(){ return m_default_driver; }

#endif

	/*! \details Executes an IO control request.
	 *
	 *
	 * This method is typically only implemented for devices
	 * and other special file systems.
	 *
	 */
	virtual int ioctl(
			IoRequest request,
			IoArgument arg
			) const;

	/*! \details Executes an ioctl() with request and const arg pointer.
	 *
	 * @return Depends on request
	 *
	 */
	int ioctl(
			IoRequest request,
			IoConstArgument argument
			) const {
		return ioctl(
					request,
					IoArgument((void*)argument.argument())
					);
	}
	/*! \details Executes an ioctl() with just a request.
	 *
	 * @return Depends on request
	 *
	 * The arg value for the ioctl is set to NULL.
	 *
	 */
	int ioctl(
			IoRequest request
			) const {
		return ioctl(request, IoArgument(nullptr));
	}
	/*! \details Executes an ioctl() with request and integer arg.
	 *
	 * @return Depends on request
	 */
	int ioctl(
			IoRequest request,
			IoIntArgument argument
			) const {
		return ioctl(
					request,
					IoArgument(MCU_INT_CAST(argument.argument()))
					);
	}

	/*! \details Sets the file descriptor for this object. */
	void set_fileno(int fd) const { m_fd = fd; }

	/*! \details Sets the file descriptor of this object to the file descriptor of file.
		*
		*
		* The reference \a file must already be opened and have a valid file
		* number. This won't bind the file number to file just assign
		* it based on the state of file.fileno() when this method is called.
		*
		*
		*/
	void set_fileno(const File & file) const { m_fd = file.fileno(); }

	/*! \details Copies a file from the source to the destination.
		*
		* @return Zero on success
		*
		*
		*/
	static int copy(
			SourcePath source_path,
			DestinationPath dest_path,
			const sys::ProgressCallback * progress_callback = nullptr
		#if defined __link
			, SourceLinkDriver source_driver = SourceLinkDriver(nullptr),
			DestinationLinkDriver destination_driver = DestinationLinkDriver(nullptr)
		#endif
			);

	/*! \details Copies a file from the source to the destination.
		*
		* @return Zero on success
		*
		*
		*/
	static int copy(
			SourcePath source_path,
			DestinationPath dest_path,
			IsOverwrite overwrite,
			const sys::ProgressCallback * progress_callback = nullptr
		#if defined __link
			, SourceLinkDriver source_driver = SourceLinkDriver(nullptr),
			DestinationLinkDriver destination_driver = DestinationLinkDriver(nullptr)
		#endif
			);

	int save_copy(
			const var::String& file_path,
			IsOverwrite is_overwrite
			);

	/*! \details Renames a file.
		*
		* \param old_path Current path to the file (will be old path after rename)
		* \param new_path New path to the file
		* \return Zero on success
		*
		*/
	static int rename(
			SourcePath old_path,
			DestinationPath new_path
			SAPI_LINK_DRIVER_NULLPTR_LAST
			);

	static int touch(const var::String& path);


protected:

	enum {
		GETS_BUFFER_SIZE = 128
	};

#ifdef __link
	mutable link_transport_mdriver_t * m_driver;
	static link_transport_mdriver_t * m_default_driver;
	int check_driver() const {
		if( m_driver == 0 ){
			m_driver = m_default_driver;
			if( m_driver == 0 ){
				set_error_number(2);
				return -1;
			}
		}
		return 0;
	}

	static link_transport_mdriver_t * check_driver(
			LinkDriver driver
			){
		if( driver.argument() == 0 ){
			return m_default_driver;
		}
		return driver.argument();
	}

#endif

	mutable int m_fd;


private:
	static int copy(
			Source source,
			Destination dest,
			SourcePath source_path,
			DestinationPath dest_path,
			const sys::ProgressCallback * progress_callback
			);
	static int copy(
			Source source,
			Destination dest,
			SourcePath source_path,
			DestinationPath dest_path,
			IsOverwrite is_overwrite,
			const sys::ProgressCallback * progress_callback
			);
	bool m_is_keep_open;

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
class DataFile : public File {
public:

	/*! \details Constructs a data file. */
	DataFile(
			const OpenFlags & flags = OpenFlags::read_write()
			){
		m_fd = 0;
		m_location = 0;
		m_open_flags = flags;
	}
	virtual ~DataFile(){
		m_fd = -1;
	}

	DataFile(
			fs::File::Path file_path
			);

	DataFile(
			const fs::File & file_to_load
			);

	/*! \details Reimplements fs::File::open() to have no
	 * functionality.
	 *
	 */
	int open(
			const var::String & path,
			const OpenFlags & flags
			){
		MCU_UNUSED_ARGUMENT(path);
		this->flags() = flags;
		return 0;
	}

	/*! \details Reimplements fs::File::close() to have no
	 * functionality.
	 *
	 */
	int close(){
		printf("close data file with fileno %d\n", fileno());
		return 0;
	}

	/*! \details Reimplements fs::File::read() to simply
	 * read from the var::Data object contained herein
	 * rather than from the filesystem.
	 */
	int read(
			void * buf,
			Size nbyte
			) const;

	/*! \details Reimplements fs::File::write() to simply
	 * write to the var::Data object contained herein
	 * rather than to the filesystem.
	 *
	 * @return The number of bytes successfully written
	 */
	int write(
			const void * buf,
			Size nbyte
			) const;

	int seek(
			int location,
			enum whence whence = SET
			) const;

	/*! \details Reimplements fs::File::ioctl() to have
	 * no functionality.
	 *
	 */
	int ioctl(
			IoRequest request,
			IoArgument argument
			) const {
		MCU_UNUSED_ARGUMENT(request);
		MCU_UNUSED_ARGUMENT(argument);
		return 0;
	}

	/*! \details Returns the size of the
	 * file (size of the data).
	 *
	 */
	u32 size() const { return data().size(); }

	using File::read;
	using File::write;
	using File::seek;

	OpenFlags & flags(){ return m_open_flags; }
	const OpenFlags & flags() const { return m_open_flags; }

	/*! \details Accesses (read-only) the member data object. */
	const var::Data & data() const { return m_data; }
	/*! \details Accesses the member data object. */
	var::Data & data(){ return m_data; }

private:
	mutable int m_location; //offset location for seeking/reading/writing
	OpenFlags m_open_flags;
	mutable var::Data m_data;
};

class ReferenceFile : public File {
public:

	/*! \details Constructs a data file. */
	ReferenceFile(
			const OpenFlags & flags = OpenFlags::read_write()
			){
		m_location = 0;
		m_open_flags = flags;
	}
	virtual ~ReferenceFile(){
		m_fd = -1;
	}

	/*! \details Reimplements fs::File::open() to have no
	 * functionality.
	 *
	 */
	int open(
			const var::String & path,
			const OpenFlags & flags
			){
		MCU_UNUSED_ARGUMENT(path);
		this->flags() = flags;
		if( flags.is_append() ){ return -1; }
		return 0;
	}

	/*! \details Reimplements fs::File::close() to have no
	 * functionality.
	 *
	 */
	int close(){ return 0; }

	/*! \details Reimplements fs::File::read() to simply
	 * read from the var::Data object contained herein
	 * rather than from the filesystem.
	 */
	int read(
			void * buf,
			Size nbyte
			) const;

	/*! \details Reimplements fs::File::write() to simply
	 * write to the var::Data object contained herein
	 * rather than to the filesystem.
	 *
	 * @return The number of bytes successfully written
	 */
	int write(
			const void * buf,
			Size nbyte
			) const;

	/*! \details Seeks to the specified location in the file.
	 *
	 * @return Zero on success
	 *
	 */
	int seek(
			int location,
			enum whence whence = SET
			) const;

	/*! \details Reimplements fs::File::ioctl() to have
	 * no functionality.
	 *
	 */
	int ioctl(
			IoRequest request,
			IoArgument argument
			) const {
		MCU_UNUSED_ARGUMENT(request);
		MCU_UNUSED_ARGUMENT(argument);
		return 0;
	}

	/*! \details Returns the size of the
	 * file (size of the data).
	 *
	 */
	u32 size() const { return reference().size(); }

	using File::read;
	using File::write;
	using File::seek;

	OpenFlags & flags(){ return m_open_flags; }
	const OpenFlags & flags() const { return m_open_flags; }

	/*! \details Accesses (read-only) the member data object. */
	const var::Reference & reference() const { return m_reference; }
	/*! \details Accesses the member data object. */
	var::Reference & reference(){ return m_reference; }

private:
	mutable int m_location; //offset location for seeking/reading/writing
	OpenFlags m_open_flags;
	mutable var::Reference m_reference;
};

class NullFile : public File {
public:

	/*! \details Constructs a null file. */
	NullFile(){
		m_fd = 0;
	}

	/*! \details Reimplements fs::File::open() to have no
	 * functionality.
	 *
	 */
	int open(
			const var::String & name,
			const OpenFlags & flags
			);

	/*! \details Reimplements fs::File::close() to have no
	 * functionality.
	 *
	 */
	int close(){ return 0; }

	/*! \details Reimplements fs::File::read() to simply
	 * return -1 if a read is attempted.
	 *
	 * @param buf dest data pointer
	 * @param nbyte number of bytes to read
	 * @return -1 to indicate reads are not valid
	 */
	int read(
			void * buf,
			Size nbyte
			) const;

	/*! \details Reimplements fs::File::write() to simply
	 * to accept the data but it is not stored anywhere.
	 *
	 * @param buf source data pointer
	 * @param nbyte number of bytes to write
	 * @return The number of bytes successfully written
	 */
	int write(
			const void * buf,
			Size nbyte
			) const;

	/*! \details Returns an error.
	 *
	 * @param loc The location to seek to
	 * @param whence The location to seek from (e.g. fs::File::SET)
	 * @return -1 because seeking is not valid
	 *
	 */
	int seek(
			int location,
			enum whence whence = SET
			) const;


	/*! \details Reimplements fs::File::ioctl() to have
	 * no functionality.
	 *
	 */
	int ioctl(
			IoRequest request,
			IoArgument argument
			) const {
		MCU_UNUSED_ARGUMENT(request);
		MCU_UNUSED_ARGUMENT(argument);
		return 0;
	}

	/*! \details Returns zero.
	 *
	 */
	u32 size() const { return 0; }

	using File::read;
	using File::write;
	using File::seek;

	OpenFlags & flags(){ return m_open_flags; }
	const OpenFlags & flags() const { return m_open_flags; }


private:
	OpenFlags m_open_flags;

};


}

#endif /* SAPI_FS_FILE_HPP_ */
