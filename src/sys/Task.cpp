#include "sys/Task.hpp"


using namespace sys;


Task::Task(){
    m_id = 0;
}

Task::~Task(){
    m_sys_device.close();
}

void Task::initialize(){
    if( m_sys_device.fileno() < 0 ){
        m_sys_device.open("/dev/sys");
    }
}

int Task::count_total(){
    int idx = m_id;
    int count = 0;
    set_id(0);
    TaskAttr attr;
    while( get_next(attr) >= 0 ){
        count++;
    }
    set_id(idx);
    return count;
}


int Task::count_free(){
    int idx = m_id;
    int count = 0;
    set_id(0);
    TaskAttr attr;
    while( get_next(attr) >= 0) {
        printf("Task name is %s\n", attr.name());
        if( !attr.is_enabled() ){
            count++;
        }
    }
    set_id(idx);
    return count;
}


int Task::get_next(TaskAttr & attr){
    sys_taskattr_t task_attr;
    int ret;

    task_attr.tid = m_id;
    m_id++;

    initialize();

    ret = set_error_number_if_error( m_sys_device.ioctl(I_SYS_GETTASK, &task_attr) );
    if( ret < 0 ){
        attr = TaskAttr::invalid();
        return ret;
    }

    attr = task_attr;
    return ret;
}

TaskAttr Task::get_attr(int idx){
    sys_taskattr_t attr;
    attr.tid = idx;
    initialize();
    if( set_error_number_if_error( m_sys_device.ioctl(I_SYS_GETTASK, &attr) ) < 0 ){
        return TaskAttr::invalid();
    }

    return TaskAttr(attr);
}
