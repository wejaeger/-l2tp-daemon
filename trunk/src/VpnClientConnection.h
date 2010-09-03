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
#include <QQueue>

class QTextStream;
class QLocalSocket;
class QProcess;

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
      ERR_WRITE_PIPE = 200,
      ERR_OPEN_PIPE = 210,
      ERR_CREATE_VPN_LOG_PIPE = 210,
      ERR_CHMOD_VPN_LOG_PIPE = 211,
      ERR_CHOWN_VPN_LOG_PIPE = 212,
      ERR_START_SYSLOG_DAEMON = 213,
   };

   enum ResponseInformation
   {
      CLOSED
   };

   VpnClientConnection(QLocalSocket* pSocket, QObject* pParent = NULL);
   virtual ~VpnClientConnection();

private slots:
   void readyRead();
   void executeExternalCommand();
   void readyReadStandardOutput();
   void onCommandFinished(int iExitCode);

private:
   VpnClientConnection(const VpnClientConnection& orig);
   VpnClientConnection& operator=(const VpnClientConnection& orig);

   bool send(ResponseType responseType, ResponseResult resultCode, const QString& strCommand);
   bool send(ResponseType responseType, ResponseInformation responseInformation);
   bool send(ResponseType responseType, const QString& strOutputline);

   QTextStream*     const m_pStream;
   QLocalSocket*    const m_pSocket;
   QQueue<QString>* const m_pCommandQueue;
   QProcess*        const m_pProcess;

   QString m_strActiveCommand;
   volatile bool m_fProcessIsActive;
};

#endif	/* VPNCLIENTCONNECTION_H */

