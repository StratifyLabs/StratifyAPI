#ifndef API_WORK_OBJECT_HPP
#define API_WORK_OBJECT_HPP

#include "ApiObject.hpp"


/*! \brief Application Programming Interface (framework-wide namespace)
 */
namespace api {
/*! \brief WorkObject Class
 * \details The WorkObject is the base
 * object for all classes in the Stratify API.
 *
 * It is a simple object that allows the
 * inheriting class to set an error or halt
 * the program if it encouters a fatal problem.
 *
 */
class WorkObject : public virtual ApiObject {

public:
    WorkObject();


    enum {
      ERROR_NONE /*! No Errors */
    };

    /*! \details Returns the error number.
     *
     * If the error number is zero. There is
     * no error.
     *
     * If the error number is greater than zero. It indicates
     * an error indicator from the standard C library (such as
     * ENOENT).
     *
     * If the error number is less than zero, it
     * refers to a StratifyAPI defined error like
     * ERROR_NONE.
     *
     */
    int error_number() const { return m_error_number; }

    //assert?

    //some way to fatal
    static void exit_fatal(const char * message);

    /*! \details Clears the current error.
     */
    void clear_error_number() const { m_error_number = ERROR_NONE; }

protected:

    //These methods are used internally to assign the error_number() value
    int set_error_number_if_error(int ret) const;
    void * set_error_number_if_null(void * ret) const;
    void set_error_number_to_errno() const;
    void set_error_number(int value) const { m_error_number = value; }

private:
    mutable int m_error_number;

};


}
#endif // API_WORK_OBJECT_HPP
