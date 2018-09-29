
#include <cstdio>
#include "test/Test.hpp"
#include "sys.hpp"


using namespace test;

bool Test::m_is_initialized = false;
bool Test::m_all_test_result = true;
u32 Test::m_all_test_duration_microseconds = 0;

Test::Test(const var::ConstString & name, Test * parent){
    //start a JSON object


    if( !m_is_initialized ){
        initialize("unknown", "0.0");
    }

    m_parent = parent;
    if( m_parent ){
        m_indent_count = parent->indent();


    } else {
        m_indent_count = 1;

    }

    m_test_result = true;

    print("\"%s\": {\n", name.str());
    increment_indent();

    m_test_duration_microseconds = 0;
}

Test::~Test(){
    //close the JSON object

    if( m_parent ){
        m_parent->m_test_duration_microseconds += m_test_duration_microseconds;
    } else {
        m_all_test_duration_microseconds += m_test_duration_microseconds;
    }


    if( m_test_result == false ){
        print("\"result\": false,\n");
    } else {
        print("\"result\": true,\n");
    }

    print("\"microseconds\": %ld.0\n", m_test_duration_microseconds);
    decrement_indent();
    print("},\n");
}


void Test::open_case(const var::ConstString & case_name){
    print("\"%s\": {\n", case_name.str());
    increment_indent();
    m_case_message_number = 0;
    m_case_result = true;
    m_case_timer.restart();
}

void Test::close_case(){
    close_case(case_result());
}

void Test::close_case(bool result){
    m_case_timer.stop();
    m_test_duration_microseconds += m_case_timer.microseconds();
    if( result == false ){
        m_all_test_result = false;
        m_test_result = false;
        print("\"result\": false,\n");
    } else {
        print("\"result\": true,\n");
    }

    print("\"microseconds\": %ld.0\n", m_case_timer.microseconds());
    decrement_indent();
    print("},\n");
    m_case_result = true;
}

void Test::print_case_message(const char * fmt, ...){
    m_case_timer.stop();
    char key[16];
    key[15] = 0; //enforce null termination
	 snprintf(key, 15, "msg-" F32D, m_case_message_number);
    m_case_message_number++;
    va_list args;
    va_start (args, fmt);
    vprint_case_message(key, fmt, args);
    va_end(args);
    m_case_timer.resume();
}

void Test::print_case_failed(const char * fmt, ...){
    m_case_timer.stop();
    char key[16];
    key[15] = 0; //enforce null termination
	 snprintf(key, 15, "msg-" F32D, m_case_message_number);
    m_case_message_number++;
    va_list args;
    va_start (args, fmt);
    vprint_case_message(key, fmt, args);
    va_end(args);
    m_case_result = false;
    m_case_timer.resume();
}

void Test::print_case_message_with_key(const var::ConstString & key, const char * fmt, ...){
    m_case_timer.stop();
    va_list args;
    va_start (args, fmt);
    vprint_case_message(key.str(), fmt, args);
    va_end(args);
    m_case_timer.resume();
}

void Test::vprint_case_message(const var::ConstString & key, const char * fmt, va_list args){
    print("\"%s\": \"", key.str());
    vprintf(fmt, args);
    printf("\",\n");
}

void Test::initialize(const var::ConstString & name, const var::ConstString & version, const var::ConstString & git_hash){
    m_is_initialized = true;
    m_all_test_duration_microseconds = 0;

    print_indent(0, "{\n");

#if !defined __link
    Sys sys;
    sys_info_t info;
    print_indent(1, "\"system\": {\n");
    if( (sys.open() < 0) || (sys.get_info(info) < 0) ){
        print_indent(2, "\"name\": \"unknown\"\n");
    } else {
        print_indent(2, "\"name\": \"%s\",\n", info.name);
        print_indent(2, "\"arch\": \"%s\",\n", info.arch);
        print_indent(2, "\"sys version\": \"%s\",\n", info.sys_version);
        print_indent(2, "\"kernel version\": \"%s\",\n", info.kernel_version);
        print_indent(2, "\"shared memory size\": \"%ld\",\n", info.sys_mem_size);
        print_indent(2, "\"id\": \"%s\",\n", info.id);
        print_indent(2, "\"serial\": \"%08lX%08lX%08lX%08lX\",\n", info.serial.sn[3], info.serial.sn[2], info.serial.sn[1], info.serial.sn[0]);
        print_indent(2, "\"frequency\": \"%ld\"\n", info.cpu_freq);
        print_indent(2, "\"bsp git hash\": \"%s\"\n", info.bsp_git_hash);
        print_indent(2, "\"sos git hash\": \"%s\"\n", info.sos_git_hash);
        print_indent(2, "\"mcu git hash\": \"%s\"\n", info.mcu_git_hash);

    }
    sys.close();
    print_indent(1, "},\n");
#endif

    print_indent(1, "\"test\": {\n");
    print_indent(2, "\"name\": \"%s\",\n", name.str());
    //need to add the amount of RAM the program has to output
    print_indent(2, "\"version\": \"%s\"\n", version.str());
    print_indent(2, "\"git hash\": \"%s\"\n", git_hash.str());
    print_indent(2, "\"api version\": \"%s\"\n", api_version());
    print_indent(2, "\"api git hash\": \"%s\"\n", api_git_hash());
    print_indent(1, "},\n");
}

void Test::print(const char * fmt, ...){
    va_list args;
    va_start (args, fmt);
    vprint_indent(indent(), fmt, args);
    va_end(args);
}

void Test::print_indent(int indent, const char * fmt, ...){
    va_list args;
    va_start (args, fmt);
    vprint_indent(indent, fmt, args);
    va_end(args);
}


void Test::vprint_indent(int indent, const char * fmt, va_list args){
    int i;
    for(i=0; i < indent; i++){
        printf("  ");
    }
    vprintf(fmt, args);
}

u32 Test::parse_options(const sys::Cli & cli){
    u32 o_execute_flags = 0;

    if( cli.is_option("-execute_all") ){ return Test::EXECUTE_ALL; }
    if( cli.is_option("-api") ){ o_execute_flags |= Test::EXECUTE_API; }
    if( cli.is_option("-performance") ){ o_execute_flags |= Test::EXECUTE_PERFORMANCE; }
    if( cli.is_option("-stress") ){ o_execute_flags |= Test::EXECUTE_STRESS; }
    if( cli.is_option("-additional") ){ o_execute_flags |= Test::EXECUTE_ADDITIONAL; }

    return o_execute_flags;
}


void Test::finalize(){
    if( m_is_initialized ){
        print_indent(1, "\"result of all tests\": {\n");
        if( m_all_test_result ){
            print_indent(2, "\"result\": true,\n");
        } else {
            print_indent(2, "\"result\": false,\n");
        }
        print_indent(2, "\"microseconds\": %ld.0\n", m_all_test_duration_microseconds);

        //need to add the amount of RAM the program has to output
        print_indent(1,"}");
        m_is_initialized = false;
        print_indent(0, "\n}\n");
    }
}

void Test::execute_api_case(){
    open_case("api");
    close_case( execute_class_api_case() );
}

void Test::execute_performance_case(){
    open_case("performance");
    close_case( execute_class_performance_case() );
}

void Test::execute_stress_case(){
    open_case("stress");
    close_case( execute_class_stress_case() );
}

void Test::execute_additional_cases(){
    open_case("additional");
    print_case_message("no additional cases");
    close_case(true);
}

bool Test::execute_class_api_case(){
    print_case_message("no api case");
    return true;
}

bool Test::execute_class_performance_case(){
    print_case_message("no performance case");
    return true;
}

bool Test::execute_class_stress_case(){
    print_case_message("no stress case");
    return true;
}

