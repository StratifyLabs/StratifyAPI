
#include <cstdio>
#include <mcu/types.h>

#include "api/api.hpp"
#include "chrono.hpp"
#include "sys.hpp"
#include "test/Test.hpp"
#include "thread.hpp"
#include "var.hpp"

class UnitTest : public test::Test {
public:
  UnitTest(var::StringView name) : test::Test(name) {}

  bool execute_class_api_case() {

    TEST_ASSERT(
      status().error_context().signature()
      == static_cast<const void *>(&(errno)));

    errno = EINVAL;
    const int line_error = __LINE__ + 1;
    API_SYSTEM_CALL("message", -1);

    TEST_ASSERT(error_context().error_number() == EINVAL);
    TEST_ASSERT(error_context().line_number() == line_error);
    TEST_ASSERT(StringView(status().error_context().message()) == "message");
    TEST_ASSERT(status().value() < 0);
    API_RESET_ERROR();
    TEST_ASSERT(status().value() == 0);

    TEST_ASSERT(status().error_context_count() == 1);

    void *thread_error_signature;
    // create a thread and make sure it's error context is independent
    Thread t(
      Thread::Construct().set_detach_state(Thread::DetachState::joinable));

    t
      .create(Thread::Create().set_argument(this).set_function(
        [](void *args) -> void * {
          Test *self = reinterpret_cast<Test *>(args);

          const int line_error = __LINE__ + 1;
          API_SYSTEM_CALL("threadmessage", -1);

          self->expect(
            __PRETTY_FUNCTION__,
            __LINE__,
            error_context().line_number() == line_error);
          self->expect(
            __PRETTY_FUNCTION__,
            __LINE__,
            StringView(status().error_context().message()) == "threadmessage");

          return error_context().signature();
        }))
      .join(&thread_error_signature);

    TEST_ASSERT(error_context().signature() != thread_error_signature);
    TEST_ASSERT(StringView(status().error_context().message()) == "message");
    TEST_ASSERT(status().error_context_count() == 2);

    return true;
  }

private:
  const void *m_original_context = nullptr;
};
