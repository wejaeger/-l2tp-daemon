/*
 * $Id$
 *
 * File:   VpnControlDaemon.h
 * Author: Werner Jaeger
 *
 * Created on August 26, 2010, 5:35 AM
 */

#ifndef VPNCONTROLDAEMON_H
#define	VPNCONTROLDAEMON_H

#include <QObject>

class QLocalServer;

class VpnControlDaemon : public QObject
{
  Q_OBJECT

public:
   VpnControlDaemon(const QString& strKey, QObject* pParent = NULL);
   virtual ~VpnControlDaemon();

   bool start();

private slots:
   void incomingLocalConnection();

private:
   VpnControlDaemon(const VpnControlDaemon& orig);
   VpnControlDaemon& operator=(const VpnControlDaemon& orig);

   QString const m_strKey;
   QLocalServer* const m_pServer;
};

#endif	/* VPNCONTROLDAEMON_H */

