/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_SYS_HPP_
#define SAPI_SYS_HPP_

/*! \brief System Access (POSIX wrappers)
 * \details The sys namespace includes classes for
 * accessing the system.
 *
 */
namespace sys {}

#include "sys/MicroTime.hpp"

#if !defined __link
#include "sys/Assets.hpp"
#include "sys/Mq.hpp"
#include "sys/Sem.hpp"
#include "sys/Signal.hpp"
#include "sys/Sched.hpp"
#include "sys/Timer.hpp"
#include "sys/Time.hpp"
#include "sys/Trace.hpp"
#else
#include "sys/Link.hpp"
#endif

#include "sys/Auth.hpp"
#include "sys/Mutex.hpp"
#include "sys/Thread.hpp"
#include "sys/TaskManager.hpp"
#include "sys/Cli.hpp"
#include "sys/Printer.hpp"
#include "sys/Sys.hpp"
#include "sys/Appfs.hpp"


using namespace sys;


#endif /* SAPI_SYS_HPP_ */
