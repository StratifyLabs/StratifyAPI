/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef SYS_HPP_
#define SYS_HPP_

/*! \brief System and Kernel Access (including POSIX abstraction)
 */
namespace sys {};

#if !defined __link
#include "sys/Aio.hpp"
#include "sys/Mq.hpp"
#include "sys/Mutex.hpp"
#include "sys/Sem.hpp"
#include "sys/Signal.hpp"
#include "sys/Thread.hpp"
#include "sys/Sched.hpp"
#include "sys/Timer.hpp"
#include "sys/Time.hpp"
#include "sys/Dbug.hpp"
#include "sys/DbugStdout.hpp"
#include "sys/DbugUart.hpp"
#include "sys/Cli.hpp"
#endif

#include "sys/Trace.hpp"
#include "sys/Kernel.hpp"
#include "sys/Appfs.hpp"
#include "sys/Dir.hpp"
#include "sys/Fifo.hpp"
#include "sys/File.hpp"
#include "sys/Hio.hpp"

using namespace sys;


#endif /* SYS_HPP_ */
