/*
 * $Id$
 *
 * File:   VpnControlDaemon.cpp
 * Author: Werner Jaeger
 *
 * Created on August 26, 2010, 5:35 AM
 */
#include <QLocalServer>

#include <sys/stat.h>

#include "VpnClientConnection.h"
#include "VpnControlDaemon.h"

VpnControlDaemon::VpnControlDaemon(const QString& strKey, QObject* pParent) : QObject(pParent), m_strKey(strKey), m_pServer(new QLocalServer)
{
}

VpnControlDaemon::~VpnControlDaemon()
{
   delete m_pServer;
}

bool VpnControlDaemon::start()
{
   QLocalServer::removeServer(m_strKey);

   // makes sure, sockets are world read/writable
   const int iUMask(::umask(0));

   const bool fStarted(m_pServer->listen(m_strKey));
   if (fStarted)
      connect(m_pServer, SIGNAL(newConnection()), SLOT(incomingLocalConnection()));

   ::umask(iUMask);

   return(fStarted);
}

void VpnControlDaemon::incomingLocalConnection()
{
   if (m_pServer->hasPendingConnections())
      new VpnClientConnection(m_pServer->nextPendingConnection(), this);
}
