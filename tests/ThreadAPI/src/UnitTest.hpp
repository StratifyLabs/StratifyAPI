
#include <cstdio>
#include <mcu/types.h>
#include <signal.h>

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

void signal_handler(int a) {
  printf("signal %d\n", a);
  API_ASSERT(false);
  exit(1);
}

class UnitTest : public test::Test {
public:
  UnitTest(var::StringView name) : test::Test(name) {}

  bool execute_class_api_case() {

    signal(11, signal_handler);
    if (!thread_api_case()) {
      return false;
    }

    if (!mutex_api_case()) {
      return false;
    }

    return true;
  }

  bool mutex_api_case() {

    {
      m_did_execute = false;
      Mutex m(Mutex::Attributes().set_type(Mutex::Type::recursive));
      m_mutex_pointer = &m;

      TEST_ASSERT(m.lock().lock().lock().is_success());

      T t = T(
        T::Construct().set_argument(this).set_function(
          [](void *args) -> void * {
            UnitTest *self = reinterpret_cast<UnitTest *>(args);
            self->m_mutex_pointer->lock().lock();
            self->m_did_execute = true;
            self->m_mutex_pointer->unlock().unlock();
            return nullptr;
          }),
        T::Attributes().set_detach_state(T::DetachState::joinable));

      TEST_ASSERT(m.unlock().unlock().unlock().is_success());
      TEST_ASSERT(t.wait().is_success());

      m.unlock();
      if (is_error()) {
      }
      reset_error();
      TEST_ASSERT(m_did_execute);
    }

    return true;
  }
  bool sched_api_case() { return true; }
  bool sem_api_case() { return true; }
  bool signal_api_case() { return true; }

  bool thread_api_case() {

    {

      TEST_ASSERT(
        T::Attributes().set_stack_size(4096).get_stack_size() >= 4096);

      bool (*test_policy)(UnitTest * self, const Sched::Policy policy)
        = [](UnitTest *self, const Sched::Policy policy) -> bool {
        const int min_priority = Sched::get_priority_min(policy);
        const int max_priority = Sched::get_priority_max(policy);
        TEST_SELF_ASSERT(min_priority <= max_priority);
        TEST_SELF_ASSERT(is_success());

        TEST_SELF_ASSERT(
          T::Attributes().set_sched_policy(policy).get_sched_policy()
          == policy);

        for (int i = min_priority; i <= max_priority; i++) {
          TEST_SELF_ASSERT(
            T::Attributes()
              .set_sched_policy(policy)
              .set_sched_priority(i)
              .get_sched_priority()
            == i);
        }
        return true;
      };

      TEST_ASSERT(test_policy(this, Sched::Policy::round_robin));
      TEST_ASSERT(test_policy(this, Sched::Policy::fifo));
      TEST_ASSERT(test_policy(this, Sched::Policy::other));
    }

    {
      T::Attributes attributes
        = T::Attributes().set_scope(T::ContentionScope::process);

      TEST_ASSERT(is_success());
      printer().key(
        "scopeProcess",
        attributes.get_scope() == T::ContentionScope::process);
      TEST_ASSERT(is_success());

      attributes.set_scope(T::ContentionScope::system);
      TEST_ASSERT(is_success());

      printer().key(
        "scopeSystem",
        attributes.get_scope() == T::ContentionScope::system);
      TEST_ASSERT(is_success());
    }

    {

      T::Attributes attributes
        = T::Attributes().set_inherit_sched(T::IsInherit::yes);

      TEST_ASSERT(is_success());
      printer().key(
        "canInherit",
        attributes.get_inherit_sched() == T::IsInherit::yes);

      TEST_ASSERT(
        attributes.get_inherit_sched() == T::IsInherit::yes
        || attributes.get_inherit_sched() == T::IsInherit::no);

      TEST_ASSERT(is_success());

      attributes.set_inherit_sched(T::IsInherit::no);

      printer().key(
        "explicit",
        attributes.get_inherit_sched() == T::IsInherit::no);

      TEST_ASSERT(
        attributes.get_inherit_sched() == T::IsInherit::yes
        || attributes.get_inherit_sched() == T::IsInherit::no);

      TEST_ASSERT(is_success());
    }
    {
      m_did_execute = false;
      T t(
        T::Construct().set_argument(this).set_function(
          [](void *args) -> void * {
            UnitTest *self = reinterpret_cast<UnitTest *>(args);
            self->m_did_execute = true;
            return nullptr;
          }),
        T::Attributes().set_detach_state(T::DetachState::joinable));

      const int priority = Sched::get_priority_min(Sched::Policy::fifo);

      TEST_ASSERT(
        t.set_sched_parameters(Sched::Policy::fifo, priority)
          .get_sched_priority()
        == priority);

      TEST_ASSERT(t.get_sched_policy() == Sched::Policy::fifo);

      TEST_ASSERT(t.join().is_success());
    }

    {
      m_did_execute = false;
      T(T::Construct().set_argument(this).set_function(
          [](void *args) -> void * {
            UnitTest *self = reinterpret_cast<UnitTest *>(args);
            self->m_did_execute = true;
            return nullptr;
          }),
        T::Attributes().set_detach_state(T::DetachState::joinable))
        .wait();

      TEST_ASSERT(is_success());
      TEST_ASSERT(m_did_execute);

      m_did_execute = false;
      T(T::Construct().set_argument(this).set_function(
          [](void *args) -> void * {
            UnitTest *self = reinterpret_cast<UnitTest *>(args);
            self->m_did_execute = true;
            return nullptr;
          }),
        T::Attributes().set_detach_state(T::DetachState::detached))
        .wait();

      TEST_ASSERT(is_success());
      TEST_ASSERT(m_did_execute);
    }

    {

      m_did_execute = true;
      T t = T(
        T::Construct().set_argument(this).set_function(
          [](void *args) -> void * {
            UnitTest *self = reinterpret_cast<UnitTest *>(args);
            while (1) {
              wait(10_milliseconds);
            }
            self->m_did_execute = false;
            return nullptr;
          }),
        T::Attributes().set_detach_state(T::DetachState::detached));

      TEST_ASSERT(t.set_cancel_state(T::CancelState::enable)
                    .set_cancel_type(T::CancelType::deferred)
                    .cancel()
                    .wait()
                    .is_success());

      TEST_ASSERT(m_did_execute);
    }

    {
      m_did_execute = false;
      T(T::Construct().set_argument(this).set_function(
          [](void *args) -> void * {
            reinterpret_cast<UnitTest *>(args)->m_did_execute = true;
            return nullptr;
          }),
        T::Attributes()
          .set_detach_state(T::DetachState::joinable)
          .set_stack_size(8192))
        .join();

      TEST_ASSERT(m_did_execute);
    }

    {
      TEST_ASSERT(m_mutex.lock().is_success());
      Thread::self();

      m_did_execute = false;
      T t = T(
        T::Construct().set_argument(this).set_function(
          [](void *args) -> void * {
            UnitTest *self = reinterpret_cast<UnitTest *>(args);
            MutexGuard mg(self->m_mutex);
            MutexGuard t_mg(self->m_thread_mutex);
            self->printer().info("wait 250ms");
            Thread::self();
            wait(250_milliseconds);
            self->m_did_execute = true;
            return nullptr;
          }),
        T::Attributes().set_detach_state(T::DetachState::joinable));

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
  M *m_mutex_pointer;
  M m_mutex;
  M m_thread_mutex;
};
