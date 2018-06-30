
#include <cstdio>
#include "test/Test.hpp"
#include "sys.hpp"


using namespace test;

bool Test::m_is_initialized = false;
bool Test::m_all_test_result = true;
u32 Test::m_all_test_duration_microseconds = 0;

Test::Test(const char * name, Test * parent){
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

    print("\"%s\": {\n", name);
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


void Test::open_case(const char * case_name){
    print("\"%s\": {\n", case_name);
    increment_indent();
    m_case_message_number = 0;
    m_case_timer.restart();
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
}

void Test::print_case_message(const char * fmt, ...){
    m_case_timer.stop();
    char key[16];
    key[15] = 0; //enforce null termination
    snprintf(key, 15, "msg-%ld", m_case_message_number);
    m_case_message_number++;
    va_list args;
    va_start (args, fmt);
    vprint_case_message(key, fmt, args);
    va_end(args);
    m_case_timer.resume();
}

void Test::print_case_message_with_key(const char * key, const char * fmt, ...){
    m_case_timer.stop();
    va_list args;
    va_start (args, fmt);
    vprint_case_message(key, fmt, args);
    va_end(args);
    m_case_timer.resume();
}

void Test::vprint_case_message(const char * key, const char * fmt, va_list args){
    print("\"%s\": \"", key);
    vprintf(fmt, args);
    printf("\",\n");
}

void Test::initialize(const char * name, const char * version){
    m_is_initialized = true;
    m_all_test_duration_microseconds = 0;

    print_indent(0, "{\n");

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
        print_indent(2, "\"serial\": \"%lX%lX%lX%lX\",\n", info.serial.sn[3], info.serial.sn[2], info.serial.sn[1], info.serial.sn[0]);
        print_indent(2, "\"frequency\": \"%ld\"\n", info.cpu_freq);

    }
    sys.close();
    print_indent(1, "},\n");
    print_indent(1, "\"test\": {\n");
    print_indent(2, "\"name\": \"%s\",\n", name);
    //need to add the amount of RAM the program has to output
    print_indent(2, "\"version\": \"%s\"\n", version);
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

