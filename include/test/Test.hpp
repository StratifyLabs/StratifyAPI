#ifndef TEST_TEST_HPP
#define TEST_TEST_HPP

#include "../api/SObject.hpp"
#include "../sys/MicroTime.hpp"
#include "../sys/Timer.hpp"

namespace test {

class Test : public api::SObject {
public:
    Test(const char * name);
    ~Test();

    static void initialize(const char * name, const char * version);
    static void finalize();

    sys::Timer & case_timer(){ return m_case_timer; }

    u32 execution_time() const { return m_execution_time; }

    void open_case(const char * case_name);
    void close_case(bool result);

    void set_execution_time(const sys::MicroTime & time){
        m_execution_time = time;
    }

    void set_case_message(const char * key, const char * message);
    void set_case_value(const char * key, u32 value);
    void set_case_value(const char * key, float value);
    void set_case_value(const char * key, s32 value);
    void set_case_value(const char * key, int value);

    enum {
        EXECUTE_API = (1<<0),
        EXECUTE_STRESS = (1<<1),
        EXECUTE_PERFORMANCE = (1<<2),
        EXECUTE_ADDITIONAL = (1<<3),
        EXECUTE_ALL = (int)-1
    };

    void execute(int o_flags = EXECUTE_ALL){
        if( o_flags & EXECUTE_API ){ execute_api_case(); }
        if( o_flags & EXECUTE_STRESS ){ execute_stress_case(); }
        if( o_flags & EXECUTE_PERFORMANCE ){ execute_performance_case(); }
        if( o_flags & EXECUTE_ADDITIONAL ){ execute_additional_cases(); }
    }

    void execute_api_case();
    void execute_performance_case();
    void execute_stress_case();
    virtual void execute_additional_cases();

protected:

    virtual bool execute_class_api_case();
    virtual bool execute_class_performance_case();
    virtual bool execute_class_stress_case();



private:

    sys::MicroTime m_execution_time;
    bool m_test_result;
    sys::Timer m_case_timer;
    sys::Timer m_test_timer;
    bool m_is_first_case;
    static bool m_is_initialized;
    static bool m_is_first_test;

};

}

#endif // TEST_TEST_HPP
