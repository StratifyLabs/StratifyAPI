
#include <cstdio>
#include "test/Test.hpp"
#include "sys.hpp"


using namespace test;

bool Test::m_is_initialized = false;
bool Test::m_is_first_test = true;

Test::Test(const char * name){
    //start a JSON object
    m_is_first_case = true;

    if( !m_is_initialized ){
        initialize("unknown", "0.0");
    }

    if( m_is_first_test == false ){
        printf(",\n");
    } else {
        m_is_first_test = false;
    }

    m_test_result = true;

    printf("  \"%s\": {\n", name);

    m_test_timer.start();
}

Test::~Test(){
    //close the JSON object

    m_test_timer.stop();

    if( m_is_first_case == false ){
        printf(",\n");
    }

    if( m_test_result == false ){
        printf("    \"result\": false,\n");
    } else {
        printf("    \"result\": true,\n");
    }

    printf("    \"microseconds\": %ld.0\n", m_test_timer.value().microseconds());
    printf("  }");
}


void Test::open_case(const char * case_name){
    if( m_is_first_case  == false ){
        printf(",\n");
    } else {
        m_is_first_case = false;
    }
    printf("    \"%s\": {\n", case_name);
    m_case_message_number = 0;
    m_test_timer.resume();
    m_case_timer.restart();
}

void Test::close_case(bool result){
    m_case_timer.stop();
    m_test_timer.stop();
    if( result == false ){
        m_test_result = false;
        printf("      \"result\": false,\n");
    } else {
        printf("      \"result\": true,\n");
    }

    printf("      \"microseconds\": %ld.0\n", m_case_timer.value().microseconds());
    printf("    }");

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
    printf("      \"%s\": \"", key);
    vprintf(fmt, args);
    printf("\",\n");
}

void Test::initialize(const char * name, const char * version){
    m_is_initialized = true;
    m_is_first_test = false;
    printf("{\n");


    Sys sys;
    sys_info_t info;
    printf("  \"system\": {\n");
    if( (sys.open() < 0) || (sys.get_info(info) < 0) ){
        printf("\"sys_name\": \"unknown\"\n");
    } else {
        printf("    \"name\": \"%s\",\n", info.name);
        printf("    \"arch\": \"%s\",\n", info.arch);
        printf("    \"sys version\": \"%s\",\n", info.sys_version);
        printf("    \"kernel version\": \"%s\",\n", info.kernel_version);
        printf("    \"shared memory size\": \"%ld\",\n", info.sys_mem_size);
        printf("    \"id\": \"%s\",\n", info.id);
        printf("    \"serial\": \"%lX%lX%lX%lX\"\n", info.serial.sn[3], info.serial.sn[2], info.serial.sn[1], info.serial.sn[0]);
    }
    sys.close();
    printf("  },\n");
    printf("  \"test\": {\n");
    printf("    \"name\": \"%s\",\n", name);
    //need to add the amount of RAM the program has to output
    printf("    \"version\": \"%s\"\n", version);
    printf("  }");
}

void Test::finalize(){
    if( m_is_initialized ){
        m_is_initialized = false;
        printf("\n}\n");
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

