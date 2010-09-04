/*
 * $Id$
 *
 * File:   VpnClientConnection.h
 * Author: Werner Jaeger
 *
 * Created on August 26, 2010, 8:26 AM
 */

#ifndef VPNCLIENTCONNECTION_H
#define	VPNCLIENTCONNECTION_H

#include <QObject>
#include <QProcess>

class QTextStream;
class QLocalSocket;

class VpnClientConnection : public QObject
{
   Q_OBJECT

public:
   enum Command
   {
      CMD_START_PCSCD,
      CMD_STOP_PCSCD,
      CMD_START_IPSECD,
      CMD_STOP_IPSECD,
      CMD_START_L2TPD,
      CMD_STOP_L2TPD,
      CMD_START_RSYSLOGD,
      CMD_STOP_RSYSLOGD,
      CMD_IPSEC_UP,
      CMD_IPSEC_DOWN,
      CMD_IPSEC_REREAD,
      CMD_L2TP_CONNECT,
      CMD_L2TP_DISCONNECT,
      CMD_CREATE_VPN_LOGPIPE,
      CMD_QUIT,
      CMD_UNKNOWN
   };

   enum ResponseType
   {
      RESULT,
      OUTPUT,
      INFORMATION,
      UNKNOWN
   };

   enum ResponseResult
   {
      OK = 0,
      ERR_UNKNOWN_CMD = 90,
      ERR_INALID_NO_OF_ARGUMENTS = 91,
      ERR_COMMAND_FAILED_TO_START = 92,
      ERR_COMMAND_CRASHED_AFTER_START = 93,
      ERR_COMMAND_TIMEDOUT = 94,
      ERR_COMMAND_FAILED_TO_READ_FROM_PROCESS = 95,
      ERR_COMMAND_FAILED_TO_WRITE_TO_PROCESS = 96,
      ERR_COMMAND_FAILED_WITH_UNKNOW_ERROR = 97,
      ERR_WRITE_PIPE = 200,
      ERR_OPEN_PIPE = 210,
      ERR_CREATE_VPN_LOG_PIPE = 220,
      ERR_CHMOD_VPN_LOG_PIPE = 221,
      ERR_CHOWN_VPN_LOG_PIPE = 222,
      ERR_START_SYSLOG_DAEMON = 223,
   };

   enum ResponseInformation
   {
      CLOSED
   };

   VpnClientConnection(QLocalSocket* pSocket, QObject* pParent = NULL);
   virtual ~VpnClientConnection();

private slots:
   void readyRead();
   void readyReadStandardOutput();
   void onCommandError(QProcess::ProcessError iError);
   void onCommandFinished(int iExitCode);

private:
   VpnClientConnection(const VpnClientConnection& orig);
   VpnClientConnection& operator=(const VpnClientConnection& orig);

   bool send(ResponseType responseType, ResponseResult resultCode, const QString& strCommand);
   bool send(ResponseType responseType, ResponseInformation responseInformation);
   bool send(ResponseType responseType, const QString& strOutputline);

   QTextStream*     const m_pStream;
   QLocalSocket*    const m_pSocket;
   QProcess*        const m_pProcess;

   QString m_strActiveCommand;
};

#endif	/* VPNCLIENTCONNECTION_H */

