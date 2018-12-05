/*
 * test_sensor_stream.cpp
 *
 *  Created on: Oct 22, 2013
 *      Author: righetti
 */




/*!=============================================================================
  ==============================================================================

  \file    test_communication_loop.cpp

  \author  righetti
  \date    May 14, 2012

  ==============================================================================
  \brief This program runs a communication loop at a user defined frequency
  \brief and logs the number of received messages at each loop in a file

  ============================================================================*/



#include <native/task.h>
#include <native/pipe.h>
#include <native/timer.h>
#include <sys/mman.h>

#include <iostream>
#include <string>

#include <cstdlib>
#include <boost/thread.hpp>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include <AtiFTSensor.h>

RT_PIPE log_pipe;
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

void logTask()
{
  int fd;
  FILE *log_file = fopen("log_file.dat","w");

  ati_ft_sensor::AtiFTSensor::steaming_msg log;

  fd = open("/proc/xenomai/registry/native/pipes/ati_ft_stream", O_RDONLY);

  if (fd < 0) {
    printf("cannot open log pipe - ERROR %d\n",fd);
    return;
  }


  size_t size = 0;
  bool reading = true;

  if(!((size = read(fd,&log,sizeof(log))) == sizeof(log)))
    reading = false;


  while ( going )
  {
    if(reading)
    {
      fprintf(log_file, "%u \t", log.rdt_seq);
      fprintf(log_file, "%u \t", log.ft_seq);
      fprintf(log_file, "%u \t", log.status);
      fprintf(log_file, "%2.6f \t", log.time);
      fprintf(log_file, "%4.2f \t %4.2f \t %4.2f \t %4.2f \t %4.2f \t %4.2f \n",
              log.F[0],log.F[1],log.F[2],log.T[0],log.T[1],log.T[2]);
    }
    reading = true;
    if(!((size = read(fd,&log,sizeof(log))) == sizeof(log)))
      reading = false;


  }
  printf("stop logging\n");
  close(fd);
  fclose(log_file);

  return;
}



int main(int argc, char* argv[])
{
  mlockall(MCL_CURRENT | MCL_FUTURE);
  rt_task_shadow(NULL, "test_communication_loop", 50, 0);

  sensor.initialize();
  sensor.stream(true);
  rt_task_sleep(1e6);

  boost::thread log_thread(logTask);

  std::cout << "Press [Enter] to exit.\n";
  waitForEnter();
  std::cout << "exiting\n";

  going = false;
  log_thread.join();
  sensor.stream(false);
  sensor.stop();

  return 0;
}
