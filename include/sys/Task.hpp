#ifndef SYS_TASK_HPP
#define SYS_TASK_HPP

#include "../api/SysObject.hpp"
#include "../hal/Device.hpp"
#include "../var/ConstString.hpp"

namespace sys {


/*!
 * \brief The Task Attributes Class
 */
class TaskInfo : public api::SysInfoObject {
public:

	/*! \details Constructs a new object with the give task ID. */
	TaskInfo(int tid = 0){
		memset(&m_value, 0, sizeof(sys_taskattr_t));
		m_value.tid = tid;
	}

	TaskInfo(const sys_taskattr_t & attr){
		memcpy(&m_value, &attr, sizeof(m_value));
	}

	static TaskInfo invalid(){ return TaskInfo(-1); }

	bool is_valid() const {
		return m_value.tid != (u32)-1;
	}

	/*! \details Returns that process ID of the task.
	  *
	  *
	  */
	u32 pid() const { return m_value.pid; }

	/*! \details Returns the thread id. */
	u32 id() const { return m_value.tid; }

	/*! \details Returns the task id (same value as thread_id()). */
	u32 thread_id() const { return m_value.tid; }

	u64 timer() const { return m_value.timer; }

	/*! \details Returns true if the task is active (not sleeping or blocked). */
	bool is_active() const { return m_value.is_active > 0; }

	/*! \details Returns the task priority. */
	u8 priority() const { return m_value.prio; }

	/*! \details Returns the task priority ceiling (if it has a mutex locked). */
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

	/*! \details Returns the stack sizve of the current task. */
	u32 stack_size() const { return m_value.mem_loc + m_value.mem_size - m_value.stack_ptr; }

	/*! \details Prints the header info for the print() method. */
	static void print_header();

	/*! \details Prints information about the task to the standard output. */
	void print() const;


private:
	sys_taskattr_t m_value;
};

//this more closely matches the driver variable name
typedef TaskInfo TaskAttr;

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

#if defined __link
	Task(link_transport_mdriver_t * driver);
#else
	Task();
#endif
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
	void set_id(u32 value){ m_id = value; }

	/*! \details Returns the index of the current task as
	  * this object goes through all tasks using get_next().
	  *
	  * Use sys::Thread::self() to get the id
	  * of the currently executing thread.
	  *
	  *
	  * \endcode
	  *
	  *
	  *
	  */
	int id() const { return m_id; }

	/*! \details Gets the attributes for the next task.
	  *
	  * @param attr A reference for the destination information.
	  *
	  * @return
	  *  - Zero if there are no more tasks
	  *  - One if the task was successfully read
	  *  - less than zero for an error readin the task
	  *
	  */
	int get_next(TaskInfo & attr);

	/*! \details Gets the task attributes for the specifed id.
	  *
	  *
	  * The code below gets the task information for the
	  * currently executing thread.
	  *
	  * \code
	  *
	  * #include <sapi/sys.hpp>
	  *
	  * TaskInfo info;
	  * Task task;
	  *
	  * info = task(Thread::self());
	  *
	  */
	TaskInfo get_info(int id);
	TaskAttr get_attr(int id){ return get_info(id); }

	/*! \details Prints info for all enabled tasks. */
	void print(int pid = -1);

	int get_pid(const var::String & name);

	bool is_pid_running(int pid);

	void initialize();
	void finalize();


private:
	hal::Device m_sys_device;
	int m_id;

};


}

#endif // SYS_TASK_HPP
