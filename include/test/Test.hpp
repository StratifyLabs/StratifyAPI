#ifndef TEST_TEST_HPP
#define TEST_TEST_HPP

#include "../sys/MicroTime.hpp"
#include "../sys/Timer.hpp"

namespace test {

class Test {
public:
    Test(const char * name);
    ~Test();

    sys::Timer & case_timer(){ return m_case_timer; }

    u32 execution_time() const { return m_execution_time; }

    void open_case(const char * case_name);
    void close_case();

    void set_execution_time(const sys::MicroTime & time){
        m_execution_time = time;
    }

    void set_case_result(bool value){
        m_case_result = value;
    }

    void set_case_message(const char * key, const char * message);
    void set_case_value(const char * key, u32 value);
    void set_case_value(const char * key, float value);
    void set_case_value(const char * key, s32 value);
    void set_case_value(const char * key, int value);

private:

    static bool m_is_first_test;
    sys::MicroTime m_execution_time;
    bool m_case_result;
    bool m_test_result;
    sys::Timer m_case_timer;
    sys::Timer m_test_timer;
    bool m_is_first_case;

};

}

#endif // TEST_TEST_HPP
