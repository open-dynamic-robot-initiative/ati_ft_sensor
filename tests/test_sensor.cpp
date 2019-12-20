/*
 * test_sensor.cpp
 *
 *  Created on: 1 Dec, 2018
 *      Author: jviereck
 */

#ifdef XENOMAI
#include <native/task.h>
#include <native/pipe.h>
#include <native/timer.h>
#include <sys/mman.h>
#endif

#include <iostream>
#include <string>

#include <cstdlib>
#include <boost/thread.hpp>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include <chrono>
#include <thread>

#include <real_time_tools/thread.hpp>
#include <AtiFTSensor.h>

ati_ft_sensor::AtiFTSensor sensor;
bool going = true;

void waitForEnter()
{
  std::string line;
  std::getline(std::cin, line);
  std::cout << line << std::endl;
}

void warnOnSwitchToSecondaryMode(int)
{
  std::cerr << "WARNING: Switched out of RealTime. Stack-trace in syslog.\n";
}

THREAD_FUNCTION_RETURN_TYPE logTask(void*)
{
  sensor.initialize();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  sensor.setBias();

  double F[3];
  double T[3];
  uint32_t rdt_seq, ft_seq, status;


  while(true) {
    sensor.getStatus(rdt_seq, ft_seq, status);
    sensor.getFT(&F[0], &T[0]);

    printf("rdt_seq=%d, ft_seq=%d, status=%d\n", rdt_seq, ft_seq, status);
    printf("%4.2f \t %4.2f \t %4.2f \t %4.2f \t %4.2f \t %4.2f \n",
              F[0], F[1], F[2], T[0], T[1], T[2]);

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}



int main(int, char*)
{
  real_time_tools::RealTimeThread thread_;
  thread_.create_realtime_thread(&logTask, nullptr);
  thread_.join();

  return 0;
}
