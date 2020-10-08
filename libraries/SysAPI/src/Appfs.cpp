/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
// Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include <errno.h>
#include <sos/link.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fs/Dir.hpp"
#include "fs/File.hpp"
#include "fs/FileSystem.hpp"
#include "printer/Printer.hpp"
#include "sys/Appfs.hpp"
#include "var/String.hpp"

printer::Printer &
printer::operator<<(printer::Printer &printer, const appfs_file_t &a) {
  printer.key("name", var::StringView(a.hdr.name));
  printer.key("id", var::StringView(a.hdr.id));
  printer.key("mode", var::String::number(a.hdr.mode, "0%o"));
  printer.key(
    "version",
    var::String().format("%d.%d", a.hdr.version >> 8, a.hdr.version & 0xff));
  printer.key("startup", var::String::number(a.exec.startup, "%p"));
  printer.key("codeStart", var::String::number(a.exec.code_start, "%p"));
  printer.key("codeSize", var::String::number(a.exec.code_size));
  printer.key("ramStart", var::String::number(a.exec.ram_start, "%p"));
  printer.key("ramSize", var::String::number(a.exec.ram_size));
  printer.key("dataSize", var::String::number(a.exec.data_size));
  printer.key("oFlags", var::String::number(a.exec.o_flags, "0x%lX"));
  printer.key("signature", var::String::number(a.exec.signature, "0x%08lx"));
  return printer;
}

printer::Printer &printer::operator<<(
  printer::Printer &printer,
  const sys::AppfsFileAttributes &a) {
  printer.key("name", a.name());
  printer.key("id", a.id());
  printer.key(
    "version",
    var::String().format("%d.%d", a.version() >> 8, a.version() & 0xff));

  printer.key("flash", a.is_flash());
  printer.key("codeExternal", a.is_code_external());
  printer.key("dataExternal", a.is_data_external());
  printer.key("codeTightlyCoupled", a.is_code_tightly_coupled());
  printer.key("dataTightlyCoupled", a.is_data_tightly_coupled());
  printer.key("startup", a.is_startup());
  printer.key("unique", a.is_unique());
  printer.key("ramSize", var::String::number(a.ram_size()));
  return printer;
}

printer::Printer &
printer::operator<<(printer::Printer &printer, const sys::AppfsInfo &a) {
  printer.key("name", a.name());
  printer.key("mode", var::String::number(a.mode(), "0%o"));
  if (a.is_executable()) {
    printer.key("id", a.id());
    printer.key(
      "version",
      var::String().format("%d.%d", a.version() >> 8, a.version() & 0xff));

    printer.key("signature", var::String::number(a.signature(), F3208X));
    printer.key("ram", var::String::number(a.ram_size()));
    printer.key("orphan", a.is_orphan());
    printer.key("flash", a.is_flash());
    printer.key("startup", a.is_startup());
    printer.key("unique", a.is_unique());
  }
  return printer;
}

using namespace sys;

AppfsFileAttributes::AppfsFileAttributes(const appfs_file_t &appfs_file) {
  m_name = appfs_file.hdr.name;
  m_id = appfs_file.hdr.id;
  m_ram_size = appfs_file.exec.ram_size;
  m_o_flags = appfs_file.exec.o_flags;
  m_version = appfs_file.hdr.version;
}

void AppfsFileAttributes::apply(appfs_file_t *appfs_file) const {

  if (m_name.is_empty() == false) {
    memcpy(appfs_file->hdr.name, m_name.cstring(), LINK_NAME_MAX);
  }

  if (m_id.is_empty() == false) {
    memcpy(appfs_file->hdr.id, m_id.cstring(), LINK_NAME_MAX);
  }

  if (m_version != 0) {
    appfs_file->hdr.version = m_version;
  }

  if (m_access_mode != 0) {
    appfs_file->hdr.mode = m_access_mode;
  }

  if (m_ram_size >= 4096) {
    appfs_file->exec.ram_size = m_ram_size;
  }

  if (appfs_file->exec.ram_size < 4096) {
    appfs_file->exec.ram_size = 4096;
  }

  appfs_file->exec.o_flags = m_o_flags;
}

int AppfsFileAttributes::apply(fs::File &file) const {
  appfs_file_t appfs_file;
  var::View appfs_file_reference(appfs_file);

  int result;

  int location = file.location();

  if (
    file.seek(0).read(appfs_file_reference).status().value()
    != (int)appfs_file_reference.size()) {
    return -1;
  }

  this->apply(appfs_file_reference.to<appfs_file_t>());

  if (
    file.seek(0).write(appfs_file_reference).status().value()
    != (int)appfs_file_reference.size()) {
    return -1;
  }

  file.seek(location, fs::File::Whence::set);

  return 0;
}

Appfs::Appfs(const Options &options FSAPI_LINK_DECLARE_DRIVER_LAST)
  : m_file(
    "/app/.install",
    fs::OpenMode::write_only() FSAPI_LINK_INHERIT_DRIVER_LAST) {
  create_asynchronous(options);
}

bool Appfs::is_flash_available() {
  return fs::Dir("app/flash" FSAPI_LINK_MEMBER_DRIVER_LAST)
    .status()
    .is_success();
}

bool Appfs::is_ram_available() {
  return fs::Dir("/app/ram" FSAPI_LINK_MEMBER_DRIVER_LAST)
    .status()
    .is_success();
}

Appfs &Appfs::create(const CreateOptions &options) {

  Appfs appfs(Options()
                .set_mount(options.mount())
                .set_name(options.name())
                .set_size(options.source()->size()));

  if (!appfs.is_valid()) {
    return *this;
  }

  u8 buffer[APPFS_PAGE_SIZE];
  var::View buffer_blob(buffer);

  while (appfs.is_ready()) {
    if (
      options.source()->read(buffer_blob).status().value()
      == buffer_blob.size()) {

      appfs.append(buffer_blob);
    }
  }

  return *this;
}

#if 0
int Appfs::create(
  Name name,
  const fs::File &source,
  MountPath mount_path,
  const ProgressCallback *progress_callback
    FSAPI_LINK_DECLARE_DRIVER_LAST) {
  fs::File file
#if defined __link
    (link_driver)
#endif
      ;
  char buffer[LINK_PATH_MAX];
  int tmp;
  appfs_createattr_t attr;
  int loc;
  unsigned int bw; // bytes written
  appfs_file_t f;
  strncpy(buffer, mount_path.argument().cstring(), LINK_PATH_MAX - 1);
  strncat(buffer, "/flash/", LINK_PATH_MAX - 1);
  strncat(buffer, name.argument().cstring(), LINK_PATH_MAX - 1);

  // delete the settings if they exist
  strncpy(f.hdr.name, name.argument().cstring(), LINK_NAME_MAX - 1);
  f.hdr.mode = 0666;
  f.exec.code_size = source.size() + sizeof(f); // total number of bytes in file
  f.exec.signature = APPFS_CREATE_SIGNATURE;

  fs::File::remove(
    buffer
#if defined __link
    ,
    link_driver
#endif
  );

  if (file.open("/app/.install", fs::OpenMode::write_only()) < 0) {
    return -1;
  }

  var::View::memory_copy(
    var::View::SourceBuffer(&f),
    var::View::DestinationBuffer(attr.buffer),
    var::View::Size(sizeof(f)));

  // now copy some bytes
  attr.nbyte = APPFS_PAGE_SIZE - sizeof(f);
  if (source.size() < (u32)attr.nbyte) {
    attr.nbyte = source.size();
  }

  source.read(attr.buffer + sizeof(f), fs::File::Size(attr.nbyte));

  attr.nbyte += sizeof(f);
  loc = 0;
  bw = 0;
  do {
    if (loc != 0) { // when loc is 0 -- header is copied in
      if ((f.exec.code_size - bw) > APPFS_PAGE_SIZE) {
        attr.nbyte = APPFS_PAGE_SIZE;
      } else {
        attr.nbyte = f.exec.code_size - bw;
      }
      source.read(attr.buffer, fs::File::Size(attr.nbyte));
    }

    // location gets modified by the driver so it needs to be fixed on each
    // loop
    attr.loc = loc;

    if (
      (tmp = file.ioctl(
         fs::File::IoRequest(I_APPFS_CREATE),
         fs::File::IoArgument(&attr)))
      < 0) {
      return tmp;
    }

    bw += attr.nbyte;
    loc += attr.nbyte;

    if (progress_callback) {
      progress_callback->update(bw, f.exec.code_size);
    }

  } while (bw < f.exec.code_size);
  if (progress_callback) {
    progress_callback->update(0, 0);
  }

  return f.exec.code_size;
}
#endif

int Appfs::create_asynchronous(const Options &options) {
  appfs_file_t *f
    = reinterpret_cast<appfs_file_t *>(m_create_attributes.buffer);
  var::String path = options.mount() + "/flash/" + options.name();

  // delete the settings if they exist
  strncpy(f->hdr.name, options.name().cstring(), LINK_NAME_MAX - 1);
  f->hdr.mode = 0666;
  f->exec.code_size
    = options.size() + overhead(); // total number of bytes in file
  f->exec.signature = APPFS_CREATE_SIGNATURE;

  fs::FileSystem(FSAPI_LINK_MEMBER_DRIVER).remove(path);

  // f holds bytes in the buffer
  m_bytes_written = overhead();
  m_data_size = f->exec.code_size;

  return 0;
}

Appfs &Appfs::append(var::View blob) {

  u32 bytes_written = 0;
  if (m_file.fileno() < 0) {
    // already full -- can't write more
    return *this;
  }

  while (m_bytes_written < m_data_size && bytes_written < blob.size()) {

    const u32 page_offset = m_bytes_written % APPFS_PAGE_SIZE;
    const u32 page_size_available = APPFS_PAGE_SIZE - page_offset;
    u32 page_size = blob.size() - bytes_written;
    if (page_size > page_size_available) {
      page_size = page_size_available;
    }

    memcpy(
      m_create_attributes.buffer + page_offset,
      blob.to_const_u8() + bytes_written,
      page_size);

    m_bytes_written += page_size;
    bytes_written += page_size;

    if (
      ((m_bytes_written % APPFS_PAGE_SIZE) == 0) // at page boundary
      || (m_bytes_written == m_data_size)) {     // or to the end

      page_size = m_bytes_written % APPFS_PAGE_SIZE;
      if (page_size == 0) {
        m_create_attributes.nbyte = APPFS_PAGE_SIZE;
      } else {
        m_create_attributes.nbyte = page_size;
      }
      if (
        m_file.ioctl(I_APPFS_CREATE, &m_create_attributes).status().value()
        < 0) {
        return *this;
      }

      m_create_attributes.loc += m_create_attributes.nbyte;
    }
  }

  if (m_bytes_written == m_data_size) {
    // all done
    m_file.close();
  }

  return *this;
}

#if 0
//now copy some bytes
m_create_attributes.nbyte = APPFS_PAGE_SIZE - sizeof(f);
if( source.size() < (u32)attr.nbyte ){
	attr.nbyte = source.size();
}

source.read(
		attr.buffer + sizeof(f),
		fs::File::Size(attr.nbyte)
		);

attr.nbyte += sizeof(f);
loc = 0;
bw = 0;
do {
if( loc != 0 ){ //when loc is 0 -- header is copied in
	if( (f.exec.code_size - bw) > APPFS_PAGE_SIZE ){
		attr.nbyte = APPFS_PAGE_SIZE;
	} else {
		attr.nbyte = f.exec.code_size - bw;
	}
	source.read(
				attr.buffer,
				fs::File::Size(attr.nbyte)
				);
}

//location gets modified by the driver so it needs to be fixed on each loop
attr.loc = loc;

if( (tmp = file.ioctl(
			fs::File::IoRequest(I_APPFS_CREATE),
			fs::File::IoArgument(&attr)
			)) < 0 ){
	return tmp;
}

bw += attr.nbyte;
loc += attr.nbyte;

if( progress_callback ){
	progress_callback->update(bw, f.exec.code_size);
}

} while( bw < f.exec.code_size);
if( progress_callback ){ progress_callback->update(0,0); }

return f.exec.code_size;

return 0;
}
#endif

AppfsInfo Appfs::get_info(var::StringView path) {

  appfs_file_t appfs_file_header;
  appfs_info_t info;
  int result;

  API_ASSIGN_ERROR_CODE(
    api::ErrorCode::io_error,
    fs::File(path, fs::OpenMode::read_only() FSAPI_LINK_MEMBER_DRIVER_LAST)
      .read(var::View(appfs_file_header))
      .status()
      .value());

  if (status().is_error()) {
    return AppfsInfo();
  }

  if (result == sizeof(appfs_file_header)) {
    // first check to see if the name matches -- otherwise it isn't an app
    // file
    const var::StringView path_name = fs::Path(path).name();

    if (path_name.find(".sys") == 0) {
      errno = EINVAL;
      return AppfsInfo();
    }

    if (path_name.find(".free") == 0) {
      errno = EINVAL;
      return AppfsInfo();
    }

    if ((appfs_file_header.hdr.mode & 0111) == 0) {
      // return AppfsInfo();
    }

    const var::StringView app_name = appfs_file_header.hdr.name;

    memset(&info, 0, sizeof(info));
    if (
      path_name == app_name
#if defined __link
      || (path_name.find(app_name) == 0)
#endif

    ) {
      memcpy(info.name, appfs_file_header.hdr.name, LINK_NAME_MAX);
      info.mode = appfs_file_header.hdr.mode;
      info.version = appfs_file_header.hdr.version;
      memcpy(info.id, appfs_file_header.hdr.id, LINK_NAME_MAX);
      info.ram_size = appfs_file_header.exec.ram_size;
      info.o_flags = appfs_file_header.exec.o_flags;
      info.signature = appfs_file_header.exec.signature;
    } else {
      errno = ENOEXEC;
      return AppfsInfo();
    }
  } else {
    errno = ENOEXEC;
    return AppfsInfo();
  }
  return AppfsInfo(info);
}

u16 Appfs::get_version(const var::String &path) {
  AppfsInfo info;
  info = get_info(path);
  return info.version();
}

var::String Appfs::get_id(const var::String &path) {
  AppfsInfo info;
  info = get_info(path);

  if (info.is_valid() == false) {
    return var::String();
  }

  return info.id();
}

#ifndef __link

Appfs &Appfs::cleanup(CleanData clean_data) {
  struct stat st;
  char buffer[LINK_PATH_MAX];
  const char *name;

  fs::Dir dir("/app/ram");

  if (dir.status().is_error()) {
    API_COPY_ERROR_CODE(dir.status());
    return *this;
  }

  while ((name = dir.read()) != 0) {
    strcpy(buffer, "/app/ram/");
    strcat(buffer, name);
    if (stat(buffer, &st) < 0) {
      API_ASSIGN_ERROR_CODE(api::ErrorCode::io_error, -1);
      return *this;
    }

    if (
      ((st.st_mode & (LINK_S_IXUSR | LINK_S_IXGRP | LINK_S_IXOTH))
       || (clean_data == CleanData::yes))
      && (name[0] != '.')) {
      if (unlink(buffer) < 0) {
        API_ASSIGN_ERROR_CODE(api::ErrorCode::io_error, -1);
        return *this;
      }
    }
  }

  return *this;
}

Appfs &Appfs::free_ram(var::StringView path) {
  API_COPY_ERROR_CODE(fs::File(path, fs::OpenMode::read_only())
                        .ioctl(I_APPFS_FREE_RAM, nullptr)
                        .status());
  return *this;
}

Appfs &Appfs::reclaim_ram(var::StringView path) {
  API_COPY_ERROR_CODE(fs::File(path, fs::OpenMode::read_only())
                        .ioctl(I_APPFS_RECLAIM_RAM, nullptr)
                        .status());
  return *this;
}

#endif
