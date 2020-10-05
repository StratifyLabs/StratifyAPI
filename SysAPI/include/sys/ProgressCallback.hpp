/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.
#ifndef SAPI_SYS_PROGRESSCALLBACK_HPP_
#define SAPI_SYS_PROGRESSCALLBACK_HPP_

#include "../api/SysObject.hpp"


namespace sys {

/*! \brief ProgressCallback Class
 * \details The ProgressCallback class is used
 * by various methods to allow the application
 * to update the progress of an operation.
 *
 * ```
 * #include <sapi/sys.hpp>
 * #include <sapi/inet.hpp>
 * Printer p;
 *
 * p.set_progress_key("downloading");
 * HttpClient http_client;
 * DataFile data_file(File::APPEND);
 *
 * //download file to data_file and print the progress using #'s
 * http_client.get("http://some.url/file", data_file, p.progress_callback());
 * ```
 *
 */
class ProgressCallback : public api::InfoObject {
public:

	/*! \details Defines the callback function prototype.
	 *
	 * The function has three arguments.  The first
	 * is the context() member of this object. The first
	 * integer is the current progress value and the second
	 * is the total progress value.
	 *
	 * If the total (third argument) is zero, the operation is either complete
	 * or aborted. If total is set to -1, the progress should be
	 * shown as indeterminate.
	 *
	 */
	typedef bool (*callback_t)(void*, int, int);

	/*! \details Constructs an empty object. */
	ProgressCallback();

	static int indeterminate_progress_total(){
		return -1;
	}

	/*! \details Executes the callback if it is valid.
	 *
	 * This method is called within operations in order
	 * to execute the provided callback.
	 *
	 * @param value The value of the progress of the operation
	 * @param total The total possible progress for the operation
	 * @return true to abort the operation or false to continue as normal
	 *
	 */
	bool update(int value, int total) const;

	static int update_function(const void * context, int value, int total);

private:
	API_AF(ProgressCallback,callback_t,callback,nullptr);
	API_AF(ProgressCallback,void*,context,nullptr);

};


}

#endif // SAPI_SYS_PROGRESSCALLBACK_HPP_
