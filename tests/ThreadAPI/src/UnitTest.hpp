
#include <cstdio>
#include <mcu/types.h>

#include "api/api.hpp"
#include "chrono.hpp"
#include "fs.hpp"
#include "sys.hpp"
#include "test/Test.hpp"
#include "thread.hpp"
#include "var.hpp"

using T = thread::Thread;
using M = thread::Mutex;
using S = thread::Signal;

class UnitTest : public test::Test {
public:
  UnitTest(var::StringView name) : test::Test(name) {}

  bool execute_class_api_case() { return true; }

  bool mutex_api_case() { return true; }
  bool sched_api_case() { return true; }
  bool sem_api_case() { return true; }
  bool signal_api_case() { return true; }

  bool thread_api_case() {

    {
      m_did_execute = false;
      T(T::Construct().set_detach_state(T::DetachState::joinable))
        .create(
          T::Create().set_argument(this).set_function([](void *args) -> void * {
            reinterpret_cast<UnitTest *>(args)->m_did_execute = true;
            return nullptr;
          }))
        .wait();

      TEST_ASSERT(is_success());
      TEST_ASSERT(m_did_execute);

      m_did_execute = false;
      T(T::Construct().set_detach_state(T::DetachState::detached))
        .create(
          T::Create().set_argument(this).set_function([](void *args) -> void * {
            reinterpret_cast<UnitTest *>(args)->m_did_execute = true;
            return nullptr;
          }))
        .wait();

      TEST_ASSERT(is_success());
      TEST_ASSERT(m_did_execute);
    }

    {

      m_did_execute = true;
      T t = T(T::Construct().set_detach_state(T::DetachState::detached))
              .create(T::Create().set_argument(this).set_function(
                [](void *args) -> void * {
                  UnitTest *self = reinterpret_cast<UnitTest *>(args);
                  while (1) {
                    wait(10_milliseconds);
                  }
                  self->m_did_execute = false;
                  return nullptr;
                }));

      TEST_ASSERT(t.set_cancel_state(T::CancelState::enable)
                    .set_cancel_type(T::CancelType::deferred)
                    .cancel()
                    .wait()
                    .is_success());

      TEST_ASSERT(m_did_execute);
    }

    {
      m_did_execute = false;
      T(T::Construct()
          .set_detach_state(T::DetachState::joinable)
          .set_stack_size(8192))
        .create(
          T::Create().set_argument(this).set_function([](void *args) -> void * {
            reinterpret_cast<UnitTest *>(args)->m_did_execute = true;
            return nullptr;
          }))
        .join();

      TEST_ASSERT(m_did_execute);

      TEST_ASSERT(
        T(T::Construct().set_stack_size(8192)).get_stacksize() == 8192);

      TEST_ASSERT(
        T(T::Construct().set_detach_state(T::DetachState::detached))
          .is_joinable()
        == false);

      TEST_ASSERT(
        T(T::Construct().set_detach_state(T::DetachState::joinable))
          .is_joinable()
        == false);
    }

    {
      TEST_ASSERT(m_mutex.lock().is_success());

      m_did_execute = false;
      T t = T(T::Construct().set_detach_state(T::DetachState::joinable))
              .create(T::Create().set_argument(this).set_function(
                [](void *args) -> void * {
                  UnitTest *self = reinterpret_cast<UnitTest *>(args);
                  MutexGuard mg(self->m_mutex);
                  MutexGuard t_mg(self->m_thread_mutex);
                  wait(250_milliseconds);
                  self->m_did_execute = true;
                  return nullptr;
                }));

      TEST_ASSERT(t.is_running());

      // unlock to allow thread to continue
      TEST_ASSERT(m_mutex.unlock().is_success());
      wait(10_milliseconds);
      TEST_ASSERT(m_mutex.try_lock() == false);

      while (m_thread_mutex.try_lock() == true) {
        m_thread_mutex.unlock();
        wait(10_milliseconds);
      }

      TEST_ASSERT(t.is_joinable());
      TEST_ASSERT(t.join().is_success());
      TEST_ASSERT(m_did_execute);
    }

    return true;
  }

private:
  volatile bool m_did_execute;
  M m_mutex;
  M m_thread_mutex;
};
