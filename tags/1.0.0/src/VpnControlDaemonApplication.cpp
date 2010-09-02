/*
 * $Id$
 *
 * $Id$
 *
 * File:   VpnControlDaemonApplication.cpp
 * Author: Werner Jaeger
 *
 * Created on August 25, 2010, 4:57 PM
 */

#include <QTextCodec>

#include "VpnControlDaemon.h"
#include "VpnControlDaemonApplication.h"

static const char* const KEY = "L2tpIPsecVpnControlDaemon";

VpnControlDaemonApplication::VpnControlDaemonApplication(int iArgc, char** ppArgv) : QtService<QCoreApplication>(iArgc, ppArgv, "L2tp over IPsec Control Daemon"),
   m_Daemon(new VpnControlDaemon(KEY, application()))
{
   QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
   QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

   setServiceDescription("Executes various ipsec and xl2tp related start and stop commands on behalf of the L2tpIPsecVpn client");
   setStartupType(QtServiceController::AutoStartup);
}

VpnControlDaemonApplication::~VpnControlDaemonApplication()
{
}

void VpnControlDaemonApplication::start()
{
   if (!m_Daemon->start())
   {
      logMessage("Failed to start VPN Control Daemon", QtServiceBase::Error);
      QCoreApplication::exit(3);
   }
}

void VpnControlDaemonApplication::stop()
{
   QCoreApplication::quit();
}
