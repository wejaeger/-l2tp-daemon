/*
 * $Id$
 *
 * File:   VpnControlDaemonApplication.h
 * Author: Werner Jaeger
 *
 * Created on August 25, 2010, 4:57 PM
 */

#include <QCoreApplication>

#ifndef VPNCONTROLDAEMONAPPLICATION_H
#define	VPNCONTROLDAEMONAPPLICATION_H

class VpnControlDaemon;

class VpnControlDaemonApplication : public QCoreApplication
{
   Q_OBJECT

public:
   VpnControlDaemonApplication(int iArgc, char** ppArgv);
   virtual ~VpnControlDaemonApplication();

   int daemonize() const;

private:
   VpnControlDaemonApplication(const VpnControlDaemonApplication& orig);
   VpnControlDaemonApplication& operator=(const VpnControlDaemonApplication& orig);

   static bool createPidFile();

   VpnControlDaemon* const m_pDaemon;
};

#endif	/* VPNCONTROLDAEMONAPPLICATION_H */

