/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef SAPI_FS_DIR_HPP_
#define SAPI_FS_DIR_HPP_

#ifdef __link
#if defined __win32
#include <windows.h>
#include <winsock2.h>
#if !defined FALSE
#define FALSE 0
#endif
#if !defined TRUE
#define TRUE 1
#endif
#include "dirent_windows.h"
#undef ERROR
#else
#include <dirent.h>
#endif

#include <sos/link.h>

#else
#include <dirent.h>
#endif
#include <functional>

#include "var/String.hpp"
#include "var/Vector.hpp"

#include "File.hpp"
#include "FileInfo.hpp"

namespace fs {

/*! \brief Dir class
 *
 * \todo Add some examples
 *
 *
 */
class Dir : public api::Object {
public:
#if defined __link
  using LinkDriver = File::LinkDriver;
  using SourceLinkDriver = File::SourceLinkDriver;
  using DestinationLinkDriver = File::DestinationLinkDriver;
#endif

  enum class Recursive { no, yes };

  /*! \details Constructs a Dir object. */
  Dir(var::StringView path FSAPI_LINK_DECLARE_DRIVER_NULLPTR_LAST);

  static const var::String filter_hidden(const var::String &entry) {
    if (!entry.is_empty() && entry.front() == '.') {
      return var::String();
    }
    return entry;
  }

  class CopyOptions {
    API_AC(CopyOptions, var::StringView , source_path);
    API_AC(CopyOptions, var::StringView , destination_path);
#if defined __link
    API_AF(CopyOptions, , destination_path);

#endif
  };

  /*! \details Destructs the object.
   *
   * If the object has a directory that is
   * currently open, the directory will
   * be closed upon destruction.
   *
   */
  ~Dir();

  /*! \details Opens a directory. */
  Dir &open(var::StringView path);

  /*! \details Closes the directory.
   *
   * If this method is not called explicitly before
   * the object is destroyed, it will be called
   * during destruction. See ~Dir().
   *
   */
  Dir &close();

  /*! \details Returns true if the directory is open. */
  bool is_open() const { return m_dirp != 0; }

  /*! \details Returns a pointer to the next entry or 0 if no more entries
   * exist.
   */
  const char *read();

  /*! \details Gets the next entry and writes the full path of the entry to the
   * given string.
   *
   * @param path_dest The var::String that will hold the full path of the next
   * entry.
   * @return True if an entry was read or false for an error or no more entries
   */
  var::String get_entry();

  /*! \details Returns a list of all
   * the entries in the directory.
   *
   *
   * ```
   * #include <sapi/fs.hpp>
   * #include <sapi/var.hpp>
   *
   *
   * Dir d;
   *
   * d.open(arg::SourceDirectoryPath("/home");
   * Vector<String> list = d.read_list();
   * d.close();
   *
   * for(u32 i=0; i < list.count(); i++){
   *   printf("Entry is %s\n", list.at(i).cstring());
   * }
   *
   * ```
   *
   *
   *
   */
  var::StringList read_list(Recursive is_recursive = Recursive::no);

  var::StringList read_list(
    std::function<const var::String(const var::String &entry)> filter,
    Recursive is_recursive = Recursive::no);

  /*! \details Returns a pointer (const) to the name of the most recently read
   * entry. */
  const char *entry_name() { return m_entry.d_name; }

  /*! \details Returns the serial number of the most recently read entry. */
  int ino() { return m_entry.d_ino; }

#ifndef __link
  /*! \details Returns the directory handle pointer. */
  DIR *dirp() { return m_dirp; }
  /*! \details Counts the total number of entries in the directory. */
  int count();
  /*! \details Rewinds the directory pointer. */
  Dir &rewind() {
    if (m_dirp) {
      rewinddir(m_dirp);
    }
    return *this;
  }
  /*! \details Seeks to a location in the directory.
   *
   * Each entry in the directory occupies 1 location
   * space. The first entry is at location 0.
   *
   *
   */
  Dir &seek(size_t location) {
    if (m_dirp) {
      seekdir(m_dirp, location);
    }
    return *this;
  }

  /*! \details Returns the current location in the directory.
   *
   *
   */
  inline long tell() {
    if (m_dirp) {
      return telldir(m_dirp);
    }
    return 0;
  }
#else
  Dir &set_driver(LinkDriver link_driver) {
    m_driver = link_driver.argument();
    return *this;
  }
#endif

private:
  var::String m_path;

#ifdef __link
  int m_dirp = 0;
  struct link_dirent m_entry = {0};
  link_transport_mdriver_t *m_driver = nullptr;
  DIR *m_dirp_local = nullptr;
  struct dirent m_entry_local = {0};

  link_transport_mdriver_t *driver() { return m_driver; }
#else
  DIR *m_dirp = nullptr;
  struct dirent m_entry = {0};
#endif
};

} // namespace fs

#endif /* SAPI_FS_DIR_HPP_ */
