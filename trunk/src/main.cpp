/*
 * $Id$
 *
 * File:   main.cpp
 * Author: Werner Jaeger
 *
 * Created on August 25, 2010, 4:52 PM
 */
#include "VpnControlDaemonApplication.h"

int main(int iArgc, char* pcaArgv[])
{
   VpnControlDaemonApplication service(iArgc, pcaArgv);
   return(service.daemonize());
}
