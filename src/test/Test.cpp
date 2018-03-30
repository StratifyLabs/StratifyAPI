
#include <cstdio>
#include "test/Test.hpp"


using namespace test;

bool Test::m_is_first_test = true;

Test::Test(const char * name){
    //start a JSON object
    m_is_first_case = true;

    if( m_is_first_test == false ){
        printf(",\n");
    } else {
        m_is_first_test = false;
        printf("{\n");
    }

    m_test_result = true;

    printf("\"%s\": {", name);

    m_test_timer.start();
}

Test::~Test(){
    //close the JSON object

    m_test_timer.stop();

    if( m_is_first_case == false ){
        printf(",");
    }

    if( m_test_result == false ){
        printf("\"result\": false,\n");
    } else {
        printf("\"result\": true,\n");
    }

    printf("\"microseconds\": %ld.0", m_test_timer.value().microseconds());
    printf("\n}\n");
    printf("}\n");
}


void Test::open_case(const char * case_name){

    if( m_is_first_case  == false ){
        printf(",\n");
    } else {
        m_is_first_case = false;
    }

    printf("\"%s\": {", case_name);
}

void Test::close_case(){

    if( m_case_result == false ){
        m_test_result = false;
        printf("\"result\": false,\n");
    } else {
        printf("\"result\": true,\n");
    }

    printf("\"microseconds\": %ld.0\n", case_timer().value().microseconds());
    printf("}");
}

void Test::set_case_message(const char * key, const char * message){
    printf("\"%s\": \"%s\",\n", key, message);
}

void Test::set_case_value(const char * key, u32 value){
    printf("\"%s\": %ld,\n", key, value);
}

void Test::set_case_value(const char * key, float value){
    printf("\"%s\": %f,\n", key, value);

}

void Test::set_case_value(const char * key, s32 value){
    printf("\"%s\": %ld,\n", key, value);
}

void Test::set_case_value(const char * key, int value){
    printf("\"%s\": %d,\n", key, value);
}


