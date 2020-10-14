/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef THREADAPI_THREAD_THREAD_HPP
#define THREADAPI_THREAD_THREAD_HPP

#include <pthread.h>

#include "Sched.hpp"
#include "chrono/ClockTime.hpp"

namespace thread {

/*! \brief Thread Class
 * \details This class creates and manages new threads using POSIX calls.
 *
 * Before creating the thread, the attributes should be set using:
 *
 * - set_stacksize()
 * - set_detachstate()
 *
 * Once the thread is running the scheduling policy can be updated using:
 *
 * - set_priority()
 *
 * ```
 *
 * static void thread_execute(void * args);
 *
 * Thread t; //default 4096 stack size and detached = true
 *
 * t.set_stacksize(2048);
 * t.set_detachstate(Thread::JOINABLE);
 *
 * //calling get_policy() or get_priority() before thread is created will return
 * an error
 *
 * t.create(thread_execute);
 *
 * t.set_priority(5, Sched::RR); //priority 5 round robin scheduling
 *
 * //now that the thread is running, the stacksize and detachstate cannot be
 * changed
 *
 * printf("Thread policy is %d\n", t.get_policy());
 * printf("Thread priority is %d\n", t.get_priority());
 * printf("Thread stacksize is %d\n", t.get_stacksize());
 * printf("Thread is joinable? %d\n", t.is_joinable());
 *
 * ```
 *
 *
 */
class Thread : public api::Object {
public:
  enum class DetachState {
    joinable /*! Joinable thread */ = PTHREAD_CREATE_JOINABLE,
    detached /*! Detacthed thread */ = PTHREAD_CREATE_DETACHED
  };
  using Policy = Sched::Policy;

  typedef void *(*function_t)(void *);

  class Construct {
    API_ACCESS_FUNDAMENTAL(Construct, u32, stack_size, 4096);
    API_ACCESS_FUNDAMENTAL(
      Construct,
      DetachState,
      detach_state,
      DetachState::detached);

    API_ACCESS_FUNDAMENTAL(Construct, function_t, function, nullptr);
    API_ACCESS_FUNDAMENTAL(Construct, void *, argument, nullptr);
    API_ACCESS_FUNDAMENTAL(Construct, Policy, policy, Policy::other);
    API_ACCESS_FUNDAMENTAL(Construct, int, priority, 0);

  public:
    Construct() : m_detach_state(DetachState::detached), m_stack_size(4096) {}
  };

  Thread(const Construct &options = Construct());
  ~Thread();

  /*! \details Gets the ID of the thread. */
  pthread_t id() const { return m_id; }

  /*! \details Returns true if the thread has a valid id.
   *
   * If create() has not been called, this will return false.
   * If there was an error creating the thread, this will
   * also return false;
   *
   */
  bool is_valid() const;

  enum class CancelType {
    deferred = PTHREAD_CANCEL_DEFERRED,
    asynchronous = PTHREAD_CANCEL_ASYNCHRONOUS
  };

  Thread &set_cancel_type(CancelType cancel_type);
  enum class CancelState {
    enable = PTHREAD_CANCEL_ENABLE,
    disable = PTHREAD_CANCEL_DISABLE
  };

  Thread &set_cancel_state(CancelState cancel_state);

  Thread &cancel();

  /*! \details Checks if the thread is running.
   *
   * If the thread is no longer running, the thread id
   * value is reset to its initial state.
   *
   * This MUST be called on detached threads after
   * the thread has finished and before another
   * thread is created.
   *
   * This is also called by `wait()`. So that works too.
   *
   */
  bool is_running();

  /*! \details Waits for the thread to complete (joins thread if it is not
   * detached).
   *
   * If the thread is joinable, this method will join the thread which blocks
   * until the thread returns. If the thread is not-joinable, this method will
   * poll is_running() until the thread completes.
   *
   */
  Thread &
  wait(void **ret = nullptr, chrono::MicroTime interval = 100_milliseconds);

  /*! \details Returns the thread ID of the calling thread. */
  static pthread_t self() { return pthread_self(); }

  /*! \details Sends a signal to the thread.
   *
   * @param sig The signal to send
   * @return Zero on success
   *
   * This method sends a signal to the thread (equivalend to pthread_kill()).
   * It causes the signal handler to be executed in the context of the thread,
   * but the signal action will affect the whole process.
   *
   * ```
   *
   * Thread t;
   *
   * //create the thread
   *
   * t.kill(0); //returns zero if the thread is valid but doesn't send a signal
   * t.kill(Signal::KILL); //will cause the entire process to die
   * t.kill(Signal::STOP); //will stop the thread from running
   *
   *
   * ```
   *
   *
   */
  const Thread &kill(int signal_number) const {
    API_RETURN_VALUE_IF_ERROR(*this);
    API_SYSTEM_CALL("", pthread_kill(id(), signal_number));
    return *this;
  }

  /*! \details Joins the calling thread to this object's thread.
   *
   * @param value_ptr A pointer to where the return value of the thread function
   * will be stored (ignored if null)
   * @return 0 if joined, -1 if couldn't join (doesn't exist or is detached)
   *
   * This method will block the calling thread until the thread function
   * returns.
   */
  Thread &join(void **value = nullptr);

  bool is_joinable() const { return m_detach_state == DetachState::joinable; }

private:
  enum thread_flags {
    id_completed = static_cast<u32>(-3),
    id_error /*! ID is an error */ = static_cast<u32>(-2),
    id_pending /*! ID is ready to be created (not valid yet) */ = static_cast<
      u32>(-1),
  };

  DetachState m_detach_state = DetachState::detached;

#if defined __link
  u32 m_status;
  pthread_t m_id = {0};
#else
  pthread_t m_id = 0;
#endif

  void destroy();
  void create(const Construct &options);

  void set_id_pending() {
#if defined __link
    m_status = id_pending;
#else
    m_id = id_pending;
#endif
  }
  void set_id_error() {
#if defined __link
    m_status = id_error;
#else
    m_id = id_error;
#endif
  }

  bool is_id_pending() const {
#if defined __link
    return m_status == id_pending;
#else
    return m_id == id_pending;
#endif
  }
  bool is_id_error() const {
#if defined __link
    return m_status == id_error;
#else
    return m_id == id_error;
#endif
  }
};

} // namespace thread

#endif /* THREADAPI_THREAD_THREAD_HPP */
