/* Copyright 2016-2018 Tyler Gilbert ALl Rights Reserved */


/*! \file
 *
 */

#ifndef LINK_HPP
#define LINK_HPP

#include <mcu/types.h>
#include <sos/link.h>
#include "../var/String.hpp"
#include "../var/Vector.hpp"

namespace sys {

/*! \brief Class to access Stratify OS from a desktop C++ application
 * \details This class is used to access devices
 * running Stratify OS from a desktop C++ application.
 *
 * This class is not available on native Stratify OS applications.
 *
 *
 *
 */
class Link {
public:
    Link();
    ~Link();

    typedef bool (*update_callback_t)(void*, int, int);

    /*! \details Gets the error message if an operation fails.
     */
    const var::String & error_message() const { return m_error_message; }

    /*! \details Gets the current progress of an operation.  This allows
     * multi-threaded applications to update a progress bar while copying files.
     */
    int progress() const { return m_progress; }

    /*! \details Gets the maximum progress value of the current operation.
     *
     */
    int progress_max() const { return m_progress_max; }

    /*! \details This gets the status of the current operation in progress.
     *
     */
    const var::String & status_message() const { return m_status_message; }

    /*! \details This connects to the specified Stratify OS device.  After calling this,
     * other applications will not have access to the device.
     *
     */
    int init(const var::String & path /*! The path to the serial device */,
             const var::String & sn /*! The serial number or an empty string to ignore */,
             bool is_legacy = false);
    int reinit(){ return init(path(), serial_no()); }

    /*! \details This disconnects from the device.  After calling this,
     * other applications can access the device.
     */
    int exit();

    /*! \details This checks to see if the device is connected.
     * \return true if connected
     */
    bool is_connected() const;
    bool get_is_connected() const { return is_connected(); }

    void set_disconnected();

    //These are all the file transfer options over Stratify OS Link

    //Operations on the device
    /*! \details This creates a directory on the target device.
     *
     */
    int mkdir(const var::String & directory /*! The directory name */,
              link_mode_t mode /*! The access permissions */);

    /*! \details This removes a directory on the target device.
     *
     */
    int rmdir(const var::String & directory /*! The directory name (must be empty) */); //Directory must be empty

    /*! \details This deletes a file on the target device.
     *
     */
    int unlink(const var::String & filename /*! The filename to delete */);

    /*! \details This creates a symbolic link on the device.
     * \note Stratify OS does not currently support symbolic links.  This function will
     * always return an error.
     *
     * \return Zero on sucess.
     */
    int symlink(const var::String & oldPath /*! The existing path */,
                const var::String & newPath /*! The path to the new link */);

    /*! \details Load the entries of a directory. */
    var::Vector<var::String> get_dir_list(const var::String & directory);

    /*! \details This methods converts the permissions to a
     * string of the format:
     *
     * -rwxrwxrwx
     *
     * The first character indicates:
     * - - File
     * - d Directory
     * - c Character file
     * - b Block file
     *
     * The order is other, group, user.  If the permission
     * is not available, the character is replace by a "-".
     *
     */
    static var::String convert_permissions(link_mode_t mode);

    /*! \details This function opens a directory such that it's contents can be
     * read with readdir().
     */
    int opendir(const var::String & directory /*! The directory to open */);

    /*! \details This function reads an entry from an open directory.
     *
     * \return The name of the next entry in the directory.
     */
    int readdir_r(int dirp /*! The directory to read (returned from opendir()) */,
                  struct link_dirent * entry /*! A pointer to the destination memory */,
                  struct link_dirent ** result /*! Assigned to \a entry on success or NULL on failure */);

    /*! \details This closes an open directory.
     *
     */
    int closedir(int dirp /*! The directory to close */);

    /*! \details This copies a file either from the device to the
     * host or from the host to the device depending on the value of \a toDevice.
     *
     * \return Zero on success
     */
    int copy(const var::String & src /*! The path to the source file */,
             const var::String & dest /*! The path to the destination file */,
             link_mode_t mode /*! The access permissions if copying to the device */,
             bool to_device = true /*! When true, copy is from host to device */,
             bool (*update)(void *, int, int) = 0,
             void * context = 0);

    int copy_file_to_device(const var::String & src, const var::String & dest, link_mode_t mode, bool (*update)(void*,int,int) = 0, void * context = 0){
        return copy(src, dest, mode, true, update, context);
    }

    int copy_file_from_device(const var::String & src, const var::String & dest, link_mode_t mode, bool (*update)(void*,int,int) = 0, void * context = 0){
        return copy(src, dest, mode, false, update, context);
    }

    /*! \details This formats the filesystem on the device.
     *
     * \return Zero on success
     */
    int format(const var::String & path); //Format the drive

    /*! \details This runs an application on the device.
     *
     * \return The PID of the new process or less than zero for an error
     */
    int run_app(const var::String & path);


    /*! \details This opens a file (or device such as /dev/adc0) on the target device.
     *
     * \return The file descriptor on success or -1 on failure
     */
    int open(const var::String & file /*! The name of the file to open */,
             int flags /*! The access flags such as LINK_O_RDWR */,
             link_mode_t mode = 0 /*! The access permissions when creating a new file */);

    /*! \details This reads an open file descriptor.
     *
     * \return Number of bytes read or less than zero on failure
     */
    int read(int fd, void * buf, int nbyte);

    /*! \details This write an open file descriptor.
     *
     * \return Number of bytes written or less than zero on failure
     */
    int write(int fd, const void * buf, int nbyte);

    /*! \details This checks to see if the target is in bootloader mode.
     * \return Non zero if bootloader mode is active.
     */
    bool is_bootloader() const { return m_is_bootloader; }

    bool is_legacy() const { return m_is_legacy; }

    /*! \details This reads the flash memory of the device.
     * \return Number of bytes read.
     */
    int read_flash(int addr, void * buf, int nbyte);

    /*! \details This writes the flash memory of the device.
     * \return Number of bytes read.
     */
    int write_flash(int addr, const void * buf, int nbyte);

    /*! \details This performs IO ctl on an open file descriptor.  \a fd
     * must refer to a device rather than a regular file.  \a ctl must
     * be unionized with a link_bulk_t, for example:
     * \code
     * union {
     * 	adc_attr_t attr;
     * 	link_bulk_t buf;
     * } adc;
     * adc.attr.freq = 200000;
     * adc.attr.enabled_channels = 0x0F;
     * adc.attr.pin_assign = 0;
     * dev.ioctl(fd, I_SETATTR, &adc);
     * \endcode
     *
     * \return Number of bytes read or less than zero on failure
     */
    int ioctl(int fd, int request, void * ctl = NULL);

    /*! \details This function performs an lseek \a fd.
     *
     * \return Zero on success
     */
    int lseek(int fd, int offset, int whence);

    /*! \details This function reads the file statistics on
     * the specified target device file.
     *
     * \return Zero on success with \a st populated or less than zero on failure
     */
    int stat(const var::String & path /*! The path to the target device file */,
             struct link_stat * st /*! A pointer to the destination structure */);
    int close(int fd);

    /*! \details This function kills the process \a pid.
     *
     * \return Zero on success or less than zero on error
     */
    int kill_pid(int pid, int signo);

    /*! \details Get the PID of the specified application name
     *
     * @param name The name of the application to find the pid
     * @return The PID or -1 if the application is not currently running
     */
    int get_pid(const var::String & name){ return get_is_executing(name); }

    /*! \details This function resets the device (connection will be terminated)
     *
     * \return Zero on success or less than zero on error
     */
    int reset();

    /*! \details This function resets the device and invokes the DFU bootloader.
     * The connection to the device is terminated with this call.
     *
     * \return Zero on success or less than zero on error
     */
    int reset_bootloader();

    /*! \details This function reads the time from
     * the device.
     *
     * \return Zero on success
     *
     */
    int get_time(struct tm * gt);

    /*! \details This function sets the time on the device.
     * \return Zero on success
     */
    int set_time(struct tm * gt);


    /*! \details This gets the address of the security word on the device.
     * \return Zero on success.
     */
    int get_security_addr(uint32_t * addr);

    /*! \details This renames a file.
     * \return Zero on success
     */
    int rename(const var::String & old_path, const var::String & new_path);

    /*! \details This changes the ownership of a file.
     * \return Zero on success.
     */
    int chown(const var::String & path, int owner, int group);

    /*! \details This changes the mode of a file.
     * \return Zero on success.
     */
    int chmod(const var::String & path, int mode);

    /*! \details This method checks to see if a process called \a name is running.
     * \return The pid of the running process or -1 if no processes match the name
     */
    int get_is_executing(const var::String & name);

    int update_os(const var::String & path, bool verify, bool (*update)(void*,int,int) = 0, void * context = 0);

    link_transport_mdriver_t * driver() const { return m_driver; }
    void set_driver(link_transport_mdriver_t * driver){ m_driver = driver; }

    void set_progress(int p){ m_progress = p; }
    void set_progress_max(int p){ m_progress_max = p; }

    //creating files in Flash/Ram

    /*! details This method will update a binary app with the specified settings
     *
     * @param path Full path to the binary file
     * @param name The name that should be assigned to the app
     * @param startup True if app should run at startup
     * @param run_in_ram True if app should run in ram
     * @param ram_size Number of bytes the app needs for RAM (excluding code if run_in_ram is true)
     * @return Zero on success or -1 with error() set to an appropriate message
     */
    int update_binary_install_options(const var::String & path, const var::String & name, const var::String & id, int version, bool startup, bool run_in_ram, int ram_size);

    /*! \details This method will install a binary to the specified location
     *
     * @param source The source of the binary file
     * @param dest The location to install the file (directory)
     * @param update A callback to update the caller on the progress of the install
     * @param context First argument passed to \a update
     * @return Zero on success or -1 with error() set to an appropriate message
     */
    int install_app(const var::String & source, const var::String & dest, const var::String & name, bool (*update)(void*,int,int) = 0, void * context = 0);

    /*! \details Return the serial number of the last device
     * that was connected (including the currently connected device)
     * @return A string containing the serial number of the last connected (or currently connected) device
     */
    const var::String & serial_no() const { return m_serialno; }

    /*! \details The path of the currently connected (or last connected) device */
    const var::String & path() const { return m_path; }

    /*! \details The path of the currently connected (or last connected) notify device */
    const var::String & notify_path() const { return m_notify_path; }

    sys_info_t sys_info() const { return m_sys_info; }

    int get_bootloader_attr(bootloader_attr_t & attr);

private:

    int check_error(int err);
    int lock_device();
    int unlock_device();
    void reset_progress();

    var::String m_serialno;
    var::String m_path;
    var::String m_notify_path;
    var::String m_error_message;
    var::String m_status_message;
    int m_stdout_fd;
    int m_stdin_fd;
    volatile int m_progress;
    volatile int m_progress_max;
    volatile int m_lock;
    bool m_is_bootloader;
    bool m_is_legacy;

    sys_info_t m_sys_info;

    link_transport_mdriver_t m_default_driver;
    link_transport_mdriver_t * m_driver;
};

}

#endif // LINK_H
