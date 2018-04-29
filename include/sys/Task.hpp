#ifndef SYS_TASK_HPP
#define SYS_TASK_HPP

#include "../api/SysObject.hpp"
#include "../hal/Device.hpp"

namespace sys {


/*!
 * \brief The Task Attributes Class
 */
class TaskAttr : public api::SysInfoObject {
public:

    /*! \details Constructs a new object with the give task ID. */
    TaskAttr(int tid = 0){
        memset(&m_value, 0, sizeof(sys_taskattr_t));
        m_value.tid = tid;
    }

    TaskAttr(const sys_taskattr_t & attr){
        memcpy(&m_value, &attr, sizeof(m_value));
    }

    static TaskAttr invalid(){ return TaskAttr(-1); }

    bool is_valid() const {
        return m_value.tid != (u32)-1;
    }

    /*! \details Returns that process ID of the task.
     *
     *
     */
    u32 pid() const { return m_value.pid; }

    /*! \details Returns the thread id. */
    u32 thread_id() const { return m_value.tid; }

    /*! \details Returns the task id (same value as thread_id()). */
    u32 thread_id() const { return m_value.tid; }

    u64 timer() const { return m_value.timer; }

    /*! \details Returns true if the task is active (not sleeping or blocked). */
    bool is_active() const { return m_value.is_active > 0; }
    u8 priority() const { return m_value.prio; }
    u8 priority_ceiling() const { return m_value.prio_ceiling; }

    /*! \details Returns true if the task is a thread.
     *
     * This returns true for a task that was created as a new
     * thread within an application. It returns false for tasks
     * that are creating using Sys::launch().
     *
     */
    bool is_thread() const { return m_value.is_thread != 0; }

    /*! \details Returns true if the task slot has been assigned.
     *
     * The system has a fixed number of tasks that it can run.
     * If this returns false, the slot is free to be used
     * by a new task (thread or process).
     *
     */
    bool is_enabled() const { return m_value.is_enabled != 0; }

    /*! \details Returns the name of the task. */
    const char * name() const { return m_value.name; }

    /*! \details Returns the number of bytes of memory available to the task. */
    u32 memory_size() const{ return m_value.mem_size; }

    /*! \details Returns the heap size available to the task. */
    u32 heap_size() const { return m_value.malloc_loc - m_value.mem_loc; }

    u32 stack_size() const { return m_value.mem_loc + m_value.mem_size - m_value.stack_ptr; }



private:
    sys_taskattr_t m_value;
};


/*! \brief Task Class
 * \details The Task Class is used to query
 * the system resources for task attribute information.
 *
 * It is not used for creating tasks. Use sys::Thread or sys::Sys::launch()
 * to create threads or processes respectively.
 *
 * \code
 *
 * Task task;
 * TaskAttr attr;
 * do {
 *   task.get_next(attr);
 *   if( attr.is_valid() ){
 *     if( attr.is_enabled() ){
 *       printf("Task Name is %s\n", attr.name());
 *     }
 *   }
 * } while( attr != TaskAttr::invalid() );
 * \endcode
 *
 *
 */
class Task : public api::SysWorkObject {
public:

    Task();
    ~Task();

    /*! \details Returns the total number of tasks that can
     * run on the system.
     *
     */
    int count_total();

    /*! \details Returns the total number of tasks that are
     * currently used.
     *
     */
    int count_free();

    /*! \details Sets the task ID value for the get_next() method.
     *
     * Valid values from from 0 to count_total() - 1.
     *
     *
     */
    void set_id(int value){ m_id = value; }

    /*! \details Returns the index of the current task. */
    int id() const { return m_id; }

    /*! \details Gets the attributes for the next task.
     *
     * @param attr A reference for the destination information.
     *
     */
    int get_next(TaskAttr & attr);

    /*! \details Gets the task attributes for the specifed id.
     *
     */
    TaskAttr get_attr(int id);


private:
    void initialize();
    hal::Device m_sys_device;
    int m_id;

};


}

#endif // SYS_TASK_HPP
