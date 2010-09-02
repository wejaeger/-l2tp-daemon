/*
 * $Id$
 *
 * $Id$
 *
 * File:   VpnClientConnection.cpp
 * Author: Werner Jaeger
 *
 * Created on August 26, 2010, 8:26 AM
 */
#include <QCoreApplication>
#include <QLocalSocket>
#include <QFile>
#include <QStringList>
#include <QTimer>
#include <QProcess>

#include <fcntl.h>
#include <pwd.h>

#include "VpnClientConnection.h"

enum CommandType { PROCESS, PIPE, INTERNAL };

static const char* const SYSLOGUSERNAME = "syslog";
static const QFile XL2TPCONTROLPIPE("/var/run/xl2tpd/l2tp-control");

struct
{
   QString strCommand;
   quint8 iArgc;
   CommandType iType;
   QFile* pPipe;
}
static COMMANDS[] =
{
   {"/etc/init.d/pcscd start", 0, PROCESS, NULL},
   {"/etc/init.d/pcscd stop", 0, PROCESS, NULL},
   {"ipsec setup start", 0, PROCESS, NULL},
   {"ipsec setup stop", 0, PROCESS, NULL},
   {"/etc/init.d/xl2tpd start", 0, PROCESS, NULL},
   {"/etc/init.d/xl2tpd stop", 0, PROCESS, NULL},
   {"start rsyslog", 0, PROCESS, NULL},
   {"stop rsyslog", 0, PROCESS, NULL},
   {"ipsec auto --up", 1, PROCESS, NULL},
   {"ipsec auto --down", 1, PROCESS, NULL},
   {"ipsec auto --rereadall", 0, PROCESS, NULL},
   {"c", 1, PIPE, const_cast<QFile*>(&XL2TPCONTROLPIPE)},
   {"d", 1, PIPE, const_cast<QFile*>(&XL2TPCONTROLPIPE)},
   {"createVpnLogPipe", 1, INTERNAL, NULL},
   {"quit", 0, INTERNAL, NULL},
};

static const size_t NOOFCOMMANDS = sizeof(COMMANDS) / sizeof(COMMANDS[0]);

VpnClientConnection::VpnClientConnection(QLocalSocket* pSocket, QObject* pParent) : QObject(pParent), m_pStream(new QTextStream),
   m_pSocket(pSocket), m_pCommandQueue(new QQueue<QString>()), m_pProcess(new QProcess), m_strActiveCommand(QString::null), m_fProcessIsActive(false)
{
   if (m_pSocket)
   {
      m_pSocket->setParent(this);
      connect(m_pSocket, SIGNAL(readyRead()), SLOT(readyRead()));
      connect(m_pSocket, SIGNAL(disconnected()), SLOT(deleteLater()));
      m_pStream->setDevice(m_pSocket);

      m_pProcess->setProcessChannelMode(QProcess::MergedChannels);
      connect(m_pProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
      connect(m_pProcess, SIGNAL(finished(int)), this, SLOT(onCommandFinished(int)));
   }
   else
      deleteLater();
}

VpnClientConnection::~VpnClientConnection()
{
   if (m_pSocket && m_pSocket->state() == QLocalSocket::ConnectedState && send(INFORMATION, CLOSED))
      m_pSocket->close();

   delete m_pProcess;
   delete m_pCommandQueue;
   delete m_pStream;
}

void VpnClientConnection::readyRead()
{
   if (m_pSocket->canReadLine())
   {
      QString strResponseLine;
      while ((strResponseLine = m_pStream->readLine()).length() > 0)
      {
         const QStringList strCommandParts(strResponseLine.split(' ', QString::SkipEmptyParts));
         const int iParts(strCommandParts.count());

         if (iParts > 0)
         {
            bool fIsCommand;
            const unsigned int iCommand = strCommandParts[0].toUInt(&fIsCommand);
            if (fIsCommand && iCommand < NOOFCOMMANDS)
            {
               QString strCommand(COMMANDS[iCommand].strCommand);
               for (int i = 1; i < iParts; i++)
                  strCommand.append(" " + strCommandParts[i]);

               if (COMMANDS[iCommand].iArgc == (iParts - 1))
               {
                  switch (COMMANDS[iCommand].iType)
                  {
                     case PROCESS:
                        m_pCommandQueue->append(strCommand);
                        QTimer::singleShot(0, this, SLOT(executeExternalCommand()));
                        break;

                     case PIPE:
                        if (COMMANDS[iCommand].pPipe->open(QIODevice::WriteOnly))
                        {
                           if (COMMANDS[iCommand].pPipe->write(strCommand.toAscii().constData()) == strCommand.length())
                              send(RESULT, OK, strCommand);
                           else
                              send(RESULT, ERR_WRITE_PIPE, strCommand);

                           COMMANDS[iCommand].pPipe->close();
                        }
                        else
                           send(RESULT, ERR_OPEN_PIPE, strCommand);
                        break;

                     case INTERNAL:
                        switch (iCommand)
                        {
                           case CMD_CREATE_VPN_LOGPIPE:
                           {
                              ResponseResult result(OK);

                              QProcess::execute(COMMANDS[CMD_STOP_RSYSLOGD].strCommand);
                              if (::mkfifo(strCommandParts[1].toAscii().constData(), DEFFILEMODE) == 0)
                              {
                                 const int iChmodResult(::chmod(strCommandParts[1].toAscii().constData(), DEFFILEMODE));
                                 if (iChmodResult)
                                    result = ERR_CHMOD_VPN_LOG_PIPE;

                                 struct passwd* const pPwdInfo = ::getpwnam(SYSLOGUSERNAME);
                                 if (pPwdInfo)
                                 {
                                    const int iChownResult(::chown(strCommandParts[1].toAscii().constData(), pPwdInfo->pw_uid, pPwdInfo->pw_gid));
                                    if (iChownResult)
                                       result = ERR_CHOWN_VPN_LOG_PIPE;
                                 }
                                 else
                                    result = ERR_CHOWN_VPN_LOG_PIPE;
                              }
                              else
                                 result = ERR_CREATE_VPN_LOG_PIPE;

                              const int iStartResult(QProcess::execute(COMMANDS[CMD_START_RSYSLOGD].strCommand));
                              if (iStartResult)
                                 result = ERR_START_SYSLOG_DAEMON;

                              send(RESULT, result, strCommand);
                           }
                           break;

                           case CMD_QUIT:
                              QCoreApplication::quit();
                              break;

                           default:
                              Q_ASSERT(false);
                        }
                        break;

                     default:
                        Q_ASSERT(false);
                  }
               }
               else
                  send(RESULT, ERR_INALID_NO_OF_ARGUMENTS, strCommand);
            }
            else
               send(RESULT, ERR_UNKNOWN_CMD, "unknown");
         }
      }
   }
}

void VpnClientConnection::executeExternalCommand()
{
   while (!m_pCommandQueue->empty())
   {
      if (!m_fProcessIsActive)
      {
         m_strActiveCommand = m_pCommandQueue->dequeue();

         m_pProcess->start(m_strActiveCommand);

         if (!m_pProcess->waitForStarted())
         {
            // returns error codes from 0 - 5
            send(RESULT, static_cast<ResponseResult>(m_pProcess->error() + 100), m_strActiveCommand);
         }
         else
            m_fProcessIsActive = true;
      }
      QCoreApplication::processEvents();
   }
}

void VpnClientConnection::readyReadStandardOutput()
{
   while (m_pProcess->canReadLine())
   {
      const QByteArray bytes(m_pProcess->readLine());
      if (bytes.count() > 0)
         send(OUTPUT, bytes.data());
   }
}

void VpnClientConnection::onCommandFinished(int iExitCode)
{
   m_fProcessIsActive = false;
   send(RESULT, static_cast<ResponseResult>(iExitCode), m_strActiveCommand);
}

bool VpnClientConnection::send(ResponseType responseType, ResponseResult resultCode, const QString& strCommand)
{
   bool fRet = false;

   if (m_pSocket->state() == QLocalSocket::ConnectedState)
   {
      m_pSocket->write((QString::number(responseType) + " " + QString::number(resultCode) + " " + strCommand + '\n').toAscii().constData());
      fRet = true;
   }

   return(fRet);
}

bool VpnClientConnection::send(ResponseType responseType, ResponseInformation responseInformation)
{
   bool fRet = false;

   if (m_pSocket->state() == QLocalSocket::ConnectedState)
   {
      m_pSocket->write((QString::number(responseType) + " " + QString::number(responseInformation) + '\n').toAscii().constData());
      fRet = true;
   }

   return(fRet);
}

bool VpnClientConnection::send(ResponseType responseType, const QString& strOutputline)
{
   bool fRet = false;

   if (m_pSocket->state() == QLocalSocket::ConnectedState)
   {
      m_pSocket->write((QString::number(responseType) + " " + strOutputline).toAscii().constData());
      fRet = true;
   }

   return(fRet);
}
