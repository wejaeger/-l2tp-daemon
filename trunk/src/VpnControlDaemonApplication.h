/*
 * $Id$
 *
 * $Id$
 *
 * File:   VpnControlDaemonApplication.h
 * Author: Werner Jaeger
 *
 * Created on August 25, 2010, 4:57 PM
 */

#ifndef VPNCONTROLDAEMONAPPLICATION_H
#define	VPNCONTROLDAEMONAPPLICATION_H

#include <QCoreApplication>
#include <QPointer>

#include "qtservice.h"

class VpnControlDaemon;

class VpnControlDaemonApplication : public QtService<QCoreApplication>
{
public:
   VpnControlDaemonApplication(int iArgc, char** ppArgv);
   virtual ~VpnControlDaemonApplication();

protected:
   void start();
   void stop();

private:
   VpnControlDaemonApplication(const VpnControlDaemonApplication& orig);
   VpnControlDaemonApplication& operator=(const VpnControlDaemonApplication& orig);

   QPointer<VpnControlDaemon> const m_Daemon;
};

#endif	/* VPNCONTROLDAEMONAPPLICATION_H */

