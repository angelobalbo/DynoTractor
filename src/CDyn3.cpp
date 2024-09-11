//---------------------------------------------------------------------------

#include <windows.h> <WINDOWS.H>
#include <stdio.h>
#include <shfolder.h>
#include <time.h>
#pragma hdrstop
#pragma package(smart_init)
#include "CDyn3.h"
#include "Log.h"


/** Definizioni Rx header*/
/// Messaggi di stato
#define DYN3_H_STATUS   0xBB
/// Messaggi risposta a comando
#define DYN3_H_MSG      0xAA
#define DYN3_H2_MSG      0x55

/// Definizioni tipi di Ack
#define DYN3_ACK_OK         0x01
#define DYN3_ACK_WRONG_CRC  0x07
#define DYN3_ACK_TIMEOUT    0x05
/// Definizione numero tentativi invio messaggio
#define MAX_STATUS_ERR  3
#define MAX_CMD_ERR     3
#define TIMEOUT_MSG     1000
#define TIME_TO_REINIT  2000
#define TIME_TO_AUTODETECT  5000

/// Definizione lunghezza risposte a comando
#define LEN_RESP_FW_REQ   10
#define LEN_RESP_FW_REQ1   8
#define LEN_RESP_READ_PID  8
#define LEN_ACK            2
#define LEN_STATUS_SHORT   2
#define LEN_STATUS_LONG    6
//---------------------------------------------------------------------------


HANDLE g_hThreadDyn3=0;


CBrakeControl g_brakeControl;

//char g_cDyn3Log[MAX_PATH]="C:\\Dyn3.log";
extern char g_cDyn3Log[MAX_PATH];



unsigned char CDyn3::m_cDyn3Action[16]=
{
        0x00,// NoCmd (not mapped)
        0x00,// Stop
        0x01,// Brake %
        0x02,// Setup test acc/dec
        0x03,// Fw req
        0x04,// RPM const
        0x05,// PWM const
        0x06,// Debug
        0x08,// Start test acc/dec
        0x0A,// PID write
        0x0C,// PID read
        0x0D,// Debug PWM
        0x0E,// Light vehicle
        0x40, // Test decel
        };                                

void CDyn3::Connect(LPCTSTR cCom)
{
  m_Dyn3Tx=EDyn3TxNoCmd;
  if(m_pNotify)
    *m_pNotify=true;
  m_iTxErrCounter=0;
  m_iRxErrCounter=0;
  m_benchState.c=0xFF;
  if( m_Dyn3Status==EDyn3NotAvail ||
      m_Dyn3Status==EDyn3NotDefined ||
      m_Dyn3Status==EDyn3StatusErr ||
      m_Dyn3Status==EDyn3DataErr)
    m_Dyn3Status=(CPort::Connect(cCom,TIMEOUT_MSG))?EDyn3Open:EDyn3NotAvail;
  if(m_Dyn3Status==EDyn3Open)
  {
    iCmdIndex==0;
   // m_errorStatus.cStatus=0x00;
    m_errorStatus.b.bDisconn=1;
  }
  else if (bPortFinder==false)
    m_errorStatus.b.bLostConn=1;
}

void CDyn3::Connect(int iPortIndex)
{
  m_Dyn3Tx=EDyn3TxNoCmd;
  if(m_pNotify)
    *m_pNotify=true;
  m_iTxErrCounter=0;
  m_iRxErrCounter=0;
  m_benchState.c=0xFF;
  if( m_Dyn3Status==EDyn3NotAvail ||
      m_Dyn3Status==EDyn3NotDefined ||
      m_Dyn3Status==EDyn3StatusErr ||
      m_Dyn3Status==EDyn3DataErr)
    m_Dyn3Status=(CPort::Connect(iPortIndex,TIMEOUT_MSG))?EDyn3Open:EDyn3NotAvail;
  if(m_Dyn3Status==EDyn3Open)
  {
    iCmdIndex==0;
    m_errorStatus.b.bDisconn=1;
  }
}


bool CDyn3::ReceiveStatus()
{
  #define LEN_DEBUG_MSG 20
  static bool bResult;
  static int iRxCursor=0;
  bResult=false;

  if(m_bDebugPwmMode==false && m_bDebugMode==false)
  {
    Rx(2);
    if(m_dwRxCount==2)
    {
      if(GetBuffer(m_dwRxCount-2)==DYN3_H_STATUS)
      {
        bResult=true;
        /// Modifica per gestire il banco Tractor:
        /// Messaggio di stato 0xBB 0x55 STATUS PWM ACK CKSUM
        if(GetBuffer(m_dwRxCount-1)==DYN3_H2_MSG)
        {
          /// Modalità PWM mode o estesa sempre attiva nel banco trattore
          m_bDebugPwmMode=true;
          m_fwType='T';
          m_fwVers=2;
          /// Scarto il messaggio attuale
          ClearRx();
          return true;
        }
        else
          m_benchState.c=GetBuffer(m_dwRxCount-1);
      }
    }
  }
  else if(m_bDebugMode)// Debug Mode
  {
    Rx(LEN_DEBUG_MSG-iRxCursor);
    if(m_dwRxCount>0)
    {
      memcpy(&m_debugInfo.cData[iRxCursor],GetBufferPos(0),m_dwRxCount);
      iRxCursor+=m_dwRxCount;
      if(iRxCursor>=2)
      {
        if(m_debugInfo.cData[0]==DYN3_H_MSG &&
           m_debugInfo.cData[1]==DYN3_H2_MSG)
        {
          if(iRxCursor>=LEN_DEBUG_MSG)
          {
            if(GetCRC(LEN_DEBUG_MSG)==GetBuffer(m_dwRxCount-1))
            {
              bResult=true;
              if(m_pNotify)
                *m_pNotify=true;
            }
            iRxCursor=0;
            bResult=true;
          }
          else
            return true;
        }
        else
        {
          iRxCursor=0;
          bResult=true;
        }
      }
      else
        return true;
    }
  }
  else if(m_bDebugPwmMode)
  {
    Rx(6);
    if(m_dwRxCount==6)
    {

      m_errorStatus.b.bDisconn=0;
      m_errorStatus.b.bLostConn=0;
      if(GetBuffer(m_dwRxCount-6)==DYN3_H_STATUS && GetCRC(6)==GetBuffer(m_dwRxCount-1))
      {
        /// Distinguo il banco Tractor versione precedenti la T03
        if(m_fwVers<5 && m_fwType=='T')
        {
          /// Status dopo l'header di 2 Byte
          m_benchState.c=GetBuffer(m_dwRxCount-4);
          /// PWM dopo lo Status, 0..255 (firmware dynT02)
          m_pwmAnte=GetBuffer(m_dwRxCount-3);
        }
        else
        {
          m_benchState.c=GetBuffer(m_dwRxCount-5);
          m_pwmAnte=((GetBuffer(m_dwRxCount-4)&0xF0) << 4) |
                      GetBuffer(m_dwRxCount-3);
          m_pwmPost=((GetBuffer(m_dwRxCount-4)&0x0F) << 8) |
                      GetBuffer(m_dwRxCount-2);
        }
        bResult=true;
      }
    }
  }
  if(bResult)
  {
    m_iRxErrCounter=0;
    m_errorStatus.b.bDisconn=0;
    m_errorStatus.b.bLostConn=0;
    bIsOpen=true;
    m_lTimestamp=clock();
    bPortFinder=false;
    if(m_Dyn3Status==EDyn3Open && m_pNotify)
      *m_pNotify=true;
    m_Dyn3Status=EDyn3Ready;

    if(m_dwRxCount && m_bActiveLog)
    {
      sprintf(m_cBuffer,"Rx ");
      for(int i=0;i<m_dwRxCount;i++)
        sprintf(&m_cBuffer[3+i*2],"%02X",m_cRxBuffer[i]);
      LogError(g_cDyn3Log,LOG_INFO,m_cBuffer);
    }
    return true;
  }
  m_iRxErrCounter++;
  if(m_iRxErrCounter>=MAX_STATUS_ERR)
  {
     m_Dyn3Status=EDyn3StatusErr;
     m_iRxErrCounter=0;
     LogError(g_cDyn3Log,LOG_ERR,"Errori di ricezione pacchetti (5)");
  }
  if(m_dwRxCount && m_bActiveLog)
  {
      sprintf(m_cBuffer,"Rx ");
      for(int i=0;i<m_dwRxCount;i++)
        sprintf(&m_cBuffer[3+i*2],"%02X",m_cRxBuffer[i]);
      LogError(g_cDyn3Log,LOG_INFO,m_cBuffer);
  }

  return false;
}

bool CDyn3::GetAck()
{
  static unsigned char cByteStatus;
  if(m_dwRxCount && m_bActiveLog)
  {
    sprintf(m_cBuffer,"Rx ");
    for(int i=0;i<m_dwRxCount;i++)
      sprintf(&m_cBuffer[3+i*2],"%02X",m_cRxBuffer[i]);
     LogError(g_cDyn3Log,LOG_INFO,m_cBuffer);
  }
  if(m_fwVers==0xC3)
  {
    if(m_pNotify)
      *m_pNotify=true;
    m_Dyn3Tx=EDyn3TxNoCmd;
    m_errorStatus.cStatus=0x00;
    m_iRxErrCounter=0;
    m_Dyn3Status=EDyn3Ready;
    return true;
  }
  if(m_dwRxCount>=LEN_ACK)
  {
    if(GetBuffer(m_dwRxCount-LEN_ACK)==DYN3_H_MSG)
    {
      // Check stato ricezione
      cByteStatus=GetBuffer(m_dwRxCount-1);
      m_errorStatus.b.bRxTimeout=(cByteStatus==DYN3_ACK_TIMEOUT);
      m_errorStatus.b.bRxWrongCRC=(cByteStatus==DYN3_ACK_WRONG_CRC);
      sprintf(m_cBuffer,"Rx ");
      for(int i=0;i<m_dwRxCount;i++)
        sprintf(&m_cBuffer[3+i*2],"%02X",m_cRxBuffer[i]);

      switch(cByteStatus)
      {
        case DYN3_ACK_OK:
          m_Dyn3Tx=EDyn3TxNoCmd;
          m_errorStatus.cStatus=0x00;
          m_iRxErrCounter=0;
          m_Dyn3Status=EDyn3Ready;
        return true;
        case DYN3_ACK_TIMEOUT:
          LogError(g_cDyn3Log,LOG_ERR,"ACK: Timeout ricezione comando");
        break;
        case DYN3_ACK_WRONG_CRC:
          LogError(g_cDyn3Log,LOG_ERR,"ACK: Checksum comando ERRATO");
        break;
      }

    }
    else
    {
      m_errorStatus.b.bRx=1;
      //LogError(g_cDyn3Log,LOG_ERR,"ACK: Errore ricezione comando");
    }
  }
  else
  {
    m_errorStatus.b.bRx=1;
    LogError(g_cDyn3Log,LOG_ERR,"ACK: risposta incompleta");
  }
  return false;
}

bool CDyn3::GetPID()
{
  if(m_dwRxCount && m_bActiveLog)
  {
    sprintf(m_cBuffer,"Rx ");
    for(int i=0;i<m_dwRxCount;i++)
      sprintf(&m_cBuffer[3+i*2],"%02X",m_cRxBuffer[i]);
    LogError(g_cDyn3Log,LOG_INFO,m_cBuffer);
  }
  if(!(m_dwRxCount>=LEN_RESP_READ_PID))
    m_errorStatus.b.bRx=1;
  else if(!(GetBuffer(m_dwRxCount-LEN_RESP_READ_PID)==0xAA))
    m_errorStatus.b.bRx=1;
  else if(GetCRC(LEN_RESP_READ_PID)==GetBuffer(m_dwRxCount-1))
  {
    m_PIDkP=(GetBuffer(m_dwRxCount-LEN_RESP_READ_PID+1)<<8);
    m_PIDkP+=GetBuffer(m_dwRxCount-LEN_RESP_READ_PID+2);
    m_PIDkI=(GetBuffer(m_dwRxCount-LEN_RESP_READ_PID+3)<<8);
    m_PIDkI+=GetBuffer(m_dwRxCount-LEN_RESP_READ_PID+4);
    m_PIDkD=(GetBuffer(m_dwRxCount-LEN_RESP_READ_PID+5)<<8);
    m_PIDkD+=GetBuffer(m_dwRxCount-LEN_RESP_READ_PID+6);
    if(m_pNotify)
      *m_pNotify=true;
    m_Dyn3Tx=EDyn3TxNoCmd;
    m_errorStatus.cStatus=0x00;
    m_iRxErrCounter=0;
    m_Dyn3Status=EDyn3Ready;
    return true;
  }
  else
  {
    m_errorStatus.b.bRxWrongCRC=1;
    LogError(g_cDyn3Log,LOG_ERR,"Get PID: checksum ERRATO");
  }
  return false;
}

bool CDyn3::GetFwVer()
{
  /// Check lunghezza messaggio
  if(m_dwRxCount && m_bActiveLog)
  {
    sprintf(m_cBuffer,"Rx ");
    for(int i=0;i<m_dwRxCount;i++)
      sprintf(&m_cBuffer[3+i*2],"%02X",m_cRxBuffer[i]);
    LogError(g_cDyn3Log,LOG_INFO,m_cBuffer);
  }
  if(!(m_dwRxCount==LEN_RESP_FW_REQ || m_dwRxCount==LEN_RESP_FW_REQ1))
  {

    m_errorStatus.b.bRxTimeout=1;
    LogError(g_cDyn3Log,LOG_ERR,"Get Fw Ver: header non corretto");
  }
  /// check header messaggio non corretto
  else if(!(GetBuffer(0)==0x00))
  {
    m_errorStatus.b.bRx=1;
    LogError(g_cDyn3Log,LOG_ERR,"Get Fw Ver: header non corretto");
  }
  /// Controllo CRC
  else if((GetCRC(m_dwRxCount)==GetBuffer(m_dwRxCount-1)) ||
          (GetCRC(m_dwRxCount)-GetBuffer(2)-GetBuffer(3))==GetBuffer(m_dwRxCount-1))
  {
    /// Se CRC Ok prelevo Fw ver e tipo, aggiorna stato
    if(m_dwRxCount==LEN_RESP_FW_REQ1 && GetBuffer(1)==0x12)
    {
      m_fwVers=0x02;
      m_fwType='T';
    }
    else
    {
      m_fwVers=GetBuffer(1);
      m_fwType=GetBuffer(8);
      /// Se rilevato il firware del trattore, attiva sempre la modalita estesa dei messaggi
      if(m_fwType=='T' && m_fwVers<5)
        m_bDebugPwmMode=true;
    }
    if(m_pNotify)
      *m_pNotify=true;
    m_Dyn3Tx=EDyn3TxNoCmd;
    m_errorStatus.cStatus=0x00;
    m_iRxErrCounter=0;
    m_Dyn3Status=EDyn3Ready;
    return true;
  }
  else
    LogError(g_cDyn3Log,LOG_ERR,"Get Fw Ver: checksum ERRATO");
  m_errorStatus.b.bRxWrongCRC=1;
  return false;
}
void CDyn3::ReceiveData()
{
  static bool bResp;



  Rx(m_iRxLen);
   switch (m_Dyn3Tx)
  { // Risposta semplice: 0xAA 0x01 ok / 0x05 / 0x07 errore
    case EDyn3TxDebugPwm:
      bResp=GetAck();
      if(bResp)
      {
        m_bDebugPwmMode=m_bNewDebugPwmMode;
        if(m_bDebugPwmMode==false)
        {
          m_pwmAnte=0;
          m_pwmPost=0;
        }
      }
    break;
    case EDyn3TxDebug:
      bResp=GetAck();
      if(bResp)
        m_bDebugMode=m_bNewDebugMode;
    break;
    case EDyn3TxStop:
    case EDyn3TxBrakeMan:
    case EDyn3TxSetup:
    case EDyn3TxStep:
    case EDyn3TxPwm:
    case EDyn3TxStart:
    case EDyn3TxWritePID:
    case EDyn3TxLight:
      bResp=GetAck();
    break;
    case EDyn3TxFwVerReq:
      bResp=GetFwVer();
    break;
    case EDyn3TxReadPID:
      bResp=GetPID();
    break;
    default:
      m_Dyn3Status=EDyn3Ready;
    break;
  }

  if(!bResp)
  {
    m_iRxErrCounter++;
    m_Dyn3Status=EDyn3Ready;
  }
  else
  {
    bPortFinder=false;
    m_lTimestamp=clock();
  }
  if(m_iRxErrCounter>=MAX_CMD_ERR)
  {
    m_Dyn3Status=EDyn3DataErr;
    m_iRxErrCounter=0;
    m_Dyn3Tx=EDyn3TxNoCmd;
    LogError(g_cDyn3Log,LOG_ERR,"Rx error receiving response");    
  }
  SetEvent(hEvDyn3Ready);
}

void CDyn3::Disconnect()
{
  CPort::Disconnect();
  iCmdIndex=0;
  cCmdQueue[0]=EDyn3TxFwVerReq;
  cCmdQueue[1]=EDyn3TxStop;
  cCmdQueue[2]=EDyn3TxDebugPwm;  
  m_Dyn3Status=EDyn3NotAvail;
  m_errorStatus.b.bDisconn=1;
  m_benchState.c=0xFF;
}

bool CDyn3::SendCmd()
{
  /// check condizioni per invio messaggi ai freni
  m_errorStatus.b.bBrakeDisconn=GetBrakeDisconnect(EFrontAxle);//m_brakeCmd.protocol.action.axle);
  m_errorStatus.b.bBrakeNoPower=GetBrakeNoPower(EFrontAxle);//m_brakeCmd.protocol.action.axle);
  /// Al primo tentantivo di trasmissione setta il timestamp per timeout operazioni

  if(!(m_errorStatus.b.bDisconn ||
       m_errorStatus.b.bLostConn ))
  {
    if(!(/*m_errorStatus.b.bBrakeDisconn ||*/
         m_errorStatus.b.bBrakeNoPower)||
          m_Dyn3Tx==EDyn3TxFwVerReq    ||
          m_Dyn3Tx==EDyn3TxReadPID     ||
          m_Dyn3Tx==EDyn3TxWritePID    ||
          m_Dyn3Tx==EDyn3TxDebugPwm    ||
          m_Dyn3Tx==EDyn3TxDebug       ||
          m_Dyn3Tx==EDyn3TxStop        ||
          m_Dyn3Tx==EDyn3TxLight         )
    {
      if(m_bActiveLog)
      {
        sprintf(m_cBuffer,"Tx ");
        for(int i=0;i<sizeof(m_brakeCmd);i++)
          sprintf(&m_cBuffer[3+i*2],"%02X",((unsigned char*)&m_brakeCmd)[i]);
        LogError(g_cDyn3Log,LOG_INFO,m_cBuffer);
      }
      if(Tx((unsigned char*)&m_brakeCmd,sizeof(m_brakeCmd)))
      {
        /// Banco Tractor versioni precedenti la dynT05:
        /// Nessun ACK e semaforo verde (SetEvent) per nuove operazioni
        if(m_fwType=='T' && m_fwVers<5 &&
           (!(m_Dyn3Tx==EDyn3TxFwVerReq || m_Dyn3Tx==EDyn3TxReadPID)))
        {
          m_Dyn3Tx=EDyn3TxNoCmd;
          m_Dyn3Status=EDyn3Ready;
          SetEvent(hEvDyn3Ready);
          return true;
        }
        m_Dyn3Status=EDyn3ReceiveData;
        m_iTxErrCounter=0;
        m_errorStatus.b.bTx=0;
        switch(m_Dyn3Tx)
        {
          case EDyn3TxDebugPwm:
          case EDyn3TxDebug:
          case EDyn3TxStop:
          case EDyn3TxBrakeMan:
          case EDyn3TxSetup:
          case EDyn3TxStep:
          case EDyn3TxPwm:
          case EDyn3TxStart:
          case EDyn3TxWritePID:
          case EDyn3TxLight:
            m_iRxLen=LEN_ACK;
          break;
          case EDyn3TxFwVerReq:
            m_iRxLen=LEN_RESP_FW_REQ;
          break;
          case EDyn3TxReadPID:
            m_iRxLen=LEN_RESP_READ_PID;
          break;
          default:
          break;
        }
        return true;
      }
      else // Notifica errore di trasmissione
      {
        m_iTxErrCounter++;
        m_errorStatus.b.bTx=1;
        sprintf(m_cBuffer,"Tx ERROR");
        LogError(g_cDyn3Log,LOG_INFO,m_cBuffer);
      }
    }
    else  // Notifica errore perche la scheda indica stato errato del freno
    {
      // m_iTxErrCounter++;
       if(m_errorStatus.b.bBrakeNoPower)
         sprintf(m_cBuffer,"Tx Command not sent (Brake not powered)");       
       else
         sprintf(m_cBuffer,"Tx Command not sent (m_Dyn3Tx!=NoCmd)");
       LogError(g_cDyn3Log,LOG_INFO,m_cBuffer);
    }
  }
  else  // Notifica errore perche la scheda è disconnessa
  {
    m_iTxErrCounter++;
    sprintf(m_cBuffer,"Tx Command not sent (Brake disconnected)");
    LogError(g_cDyn3Log,LOG_INFO,m_cBuffer);
  }

  if(m_iTxErrCounter>MAX_CMD_ERR)
  {                                              
     m_iTxErrCounter=0;
     m_Dyn3Status=EDyn3DataErr;
     m_Dyn3Tx=EDyn3TxNoCmd;
     SetEvent(hEvDyn3Ready);
  }
  return false;
}
void CDyn3::SetCRC()
{
  static int i;
  m_brakeCmd.serial[sizeof(m_brakeCmd)-1]=0;
  for(i=0;i<sizeof(m_brakeCmd)-1;i++)
    m_brakeCmd.serial[sizeof(m_brakeCmd)-1]+=m_brakeCmd.serial[i];
}

unsigned char CDyn3::GetCRC(int iByteCount)
{
  static int i;
  static unsigned char cCRC;
  cCRC=0;
  for(i=m_dwRxCount-iByteCount;i<m_dwRxCount-1;i++)
    cCRC+=GetBuffer(i);
  return cCRC;

}

/// Macchina a stati finiti implementa il ciclo di gestione della scheda Dyn3
bool CDyn3::RunStateMachine()
{
  switch(m_Dyn3Status)
  {
    case EDyn3NotAvail:
    case EDyn3NotDefined:
      if(m_Dyn3Tx==EDyn3TxTerminate)
        return false;
      if(bPortFinder)
      {
        m_iPortIndex++;
        if(m_iPortIndex<GetComCount())
        {

          Connect(m_iPortIndex);
        }
        else
        {
          bPortFinder=false;
          m_lTimestamp=clock();
        }

      }
      else if(m_errorStatus.b.bLostConn)
      {
        if(clock()-m_lTimestamp>TIME_TO_AUTODETECT)
          autoDetect(true);
        else if(clock()-m_lTimestamp>TIME_TO_REINIT)
          Connect(m_cPort);
      }

    break;
    case EDyn3Ready:
      /// Richiesta di terminazione app
       if(m_Dyn3Tx==EDyn3TxTerminate)
        m_Dyn3Status=EDyn3Exit;
      /// Fase di sconnessione porta
      else if(m_Dyn3Tx==EDyn3TxDisconnect)
        m_Dyn3Status=EDyn3Disconnect;
      /// Richiesta di comando da inoltrare
      else if(m_Dyn3Tx!=EDyn3TxNoCmd)
        m_Dyn3Status=EDyn3SendCmd;
      /// Nessun comando presente, ricevi msg di stato
      else
        m_Dyn3Status=EDyn3WaitStatus;
    break;
    case EDyn3Open:
    /// Porta seriale aperta, bisogna verificare se è una Dyn3 ricevendo msg di stato
      SetEvent(hEvDyn3Ready);
    case EDyn3WaitStatus:
      if(m_Dyn3Tx==EDyn3TxDisconnect)
        m_Dyn3Status=EDyn3Disconnect;
      else
        ReceiveStatus();
    break;
    case EDyn3SendCmd:
      SendCmd();
    break;
    case EDyn3ReceiveData:
      ReceiveData();
    break;
    case EDyn3Disconnect:
       Disconnect();
    break;
    case EDyn3StatusErr:
    case EDyn3DataErr:
    /// Settaggio perdita connessione per permettere tentativi successivi di riconnessione
      m_errorStatus.b.bLostConn=1;
      m_bDebugPwmMode=false;
      Disconnect();

    break;
    case EDyn3Exit:
       Disconnect();
       return false;
  }
  m_errorStatus.b.bBrakeDisconn=(GetBrakeDisconnect(EFrontAxle));
  m_errorStatus.b.bBrakeNoPower=GetBrakeNoPower(EFrontAxle);
  /// Notifica all'esterno i cambi di stato della scheda e gli errori
  if(m_pNotify/* && m_Dyn3Tx==EDyn3TxNoCmd*/)
  {
    /// Prima di notificare controlla alcune condizioni
    if(m_Dyn3Status!=EDyn3Exit)
    if(m_benchStatePrec.c!=m_benchState.c ||               /// Cambio di stato
       m_errorStatusPrec.cStatus!=m_errorStatus.cStatus || /// Notifica di errore
       m_Dyn3Status==EDyn3NotDefined ||                    /// Stato iniziale
       m_Dyn3Status==EDyn3Ready ||
       m_bDebugPwmMode)                                    /// Stato debug PWM (ricezione valori PWM dalla scheda)

    {
       *m_pNotify=true;
       m_errorStatusPrec.cStatus=m_errorStatus.cStatus;
       m_benchStatePrec.c=m_benchState.c;
    }
  }
  if(m_Dyn3Tx==EDyn3TxNoCmd)
    Sleep(50);
  return true;
}



void CBrakeControl::autoDetect(bool bDetect)
{
  if(bDetect && (GetDyn3Status()==EDyn3NotAvail || GetDyn3Status()==EDyn3NotDefined))
  {
    iCmdIndex=0;
    cCmdQueue[0]=EDyn3TxFwVerReq;
    cCmdQueue[1]=EDyn3TxStop;
    cCmdQueue[2]=EDyn3TxDebugPwm;
    cCmdQueue[3]=EDyn3TxDebug;
    CPort::autoDetect(bDetect);
  }
}
bool CBrakeControl::CmdStop()
{
  if(CmdReady())
  {
    m_brakeCmd.protocol.action.cmd=m_cDyn3Action[EDyn3TxStop];
    m_brakeCmd.protocol.action.axle=m_defaultAxle;
    SetCRC();
    m_Dyn3Tx=EDyn3TxStop;
    return true;
  }
  return false;
}                        

bool CBrakeControl::CmdFwVerReq()
{
  if(CmdReady())
  {
    m_brakeCmd.protocol.action.cmd=(unsigned char)m_cDyn3Action[EDyn3TxFwVerReq];
    m_brakeCmd.protocol.action.axle=m_defaultAxle;
    SetCRC();
    m_Dyn3Tx=EDyn3TxFwVerReq;
    return true;
  }
  return false;
}

bool CBrakeControl::CmdBrakeMan(unsigned short value, int axle)
{
  if(CmdReady())
  {
    m_brakeCmd.protocol.action.cmd=(unsigned char)m_cDyn3Action[EDyn3TxBrakeMan];
    m_brakeCmd.protocol.action.axle=axle;
    if((m_fwType=='R' && m_fwVers>=8) || m_fwType=='K' || (m_fwType=='T' && m_fwVers>=5))
    {
      /// Limitazione freno per corrente eccessiva
      if(value>4080) value=4080;
      m_brakeCmd.protocol.data.brakePerc2.valueH=(value>>8);
      m_brakeCmd.protocol.data.brakePerc2.valueL=value;
    }
    else
      m_brakeCmd.protocol.data.brakePerc.value=value;
    SetCRC();
    m_Dyn3Tx=EDyn3TxBrakeMan;
    return true;
  }
  return false;
}

bool CBrakeControl::CmdStep(unsigned short value, int axle)
{
  if(CmdReady())
  {
    if(m_bActiveLog)
      LogError(g_cDyn3Log,LOG_INFO,"Cmd Step Inviato");
    m_brakeCmd.protocol.action.cmd=(unsigned char)m_cDyn3Action[EDyn3TxStep];
    m_brakeCmd.protocol.action.axle=axle;
    m_brakeCmd.protocol.data.step.valueH=value>>8;
    m_brakeCmd.protocol.data.step.valueL=value;
    SetCRC();
    m_Dyn3Tx=EDyn3TxStep;
    return true;
  }
  LogError(g_cDyn3Log,LOG_INFO,"Cmd Step Fallito");
  return false;
}

bool CBrakeControl::CmdSetup(unsigned short acc,
                            unsigned short start,
                            unsigned short stop,
                            int axle)
{
  if(CmdReady())
  {
    m_brakeCmd.protocol.action.cmd=(unsigned char)m_cDyn3Action[EDyn3TxSetup];
    m_brakeCmd.protocol.action.axle=axle;
    m_brakeCmd.protocol.action.dec=(start>stop)?0:1;
    m_brakeCmd.protocol.data.setupTestAcc.accH=acc>>8;
    m_brakeCmd.protocol.data.setupTestAcc.accL=acc;
    m_brakeCmd.protocol.data.setupTestAcc.startH=start>>8;
    m_brakeCmd.protocol.data.setupTestAcc.startL=start;
    m_brakeCmd.protocol.data.setupTestAcc.stopH=stop>>8;
    m_brakeCmd.protocol.data.setupTestAcc.stopL=stop;
    SetCRC();
    m_Dyn3Tx=EDyn3TxSetup;
    return true;
  }
  return false;
}
#ifdef _DYNORACE
bool CBrakeControl::CmdPwm(unsigned short value, unsigned short weight, int axle)
{
  if(CmdReady())
  {
    m_brakeCmd.protocol.action.cmd=(unsigned char)m_cDyn3Action[EDyn3TxPwm];
    m_brakeCmd.protocol.action.axle=axle;
    if(axle==EAllAxle || axle==EFrontAxle)
    {
      if((m_fwType=='R' && m_fwVers>=8) || (m_fwType=='T' && m_fwVers>=5))
        m_brakeCmd.protocol.data.pwm.pwmFrontH=value>>8;

      m_brakeCmd.protocol.data.pwm.pwmFrontL=value;
    }
    m_brakeCmd.protocol.data.pwm.weight=weight/10;
    if(axle==EAllAxle || axle==ERearAxle)
    {
      if(m_fwType=='R' && m_fwVers>=8 || (m_fwType=='T' && m_fwVers>=5))
        m_brakeCmd.protocol.data.pwm.pwmRearH=value>>8;
      m_brakeCmd.protocol.data.pwm.pwmRearL=value;
    }
    SetCRC();
    m_Dyn3Tx=EDyn3TxStep;
    return true;
  }
  return false;
}
#endif

#ifdef _DYNOTRUCK

bool CBrakeControl::CmdPwm(unsigned short value, unsigned short weight, int axle)
{
  if(CmdReady())
  {
    m_brakeCmd.protocol.action.cmd=(unsigned char)m_cDyn3Action[EDyn3TxPwm];
    m_brakeCmd.protocol.action.axle=axle;
    if(axle==EAllAxle || axle==EFrontAxle)
    {
      if(m_fwType=='K')
        m_brakeCmd.protocol.data.pwm.pwmFrontH=value>>8;

      m_brakeCmd.protocol.data.pwm.pwmFrontL=value;
    }
    m_brakeCmd.protocol.data.pwm.weight=weight/10;
    if(axle==EAllAxle || axle==ERearAxle)
    {
      if(m_fwType=='K')
        m_brakeCmd.protocol.data.pwm.pwmRearH=value>>8;
      m_brakeCmd.protocol.data.pwm.pwmRearL=value;
    }
    SetCRC();
    m_Dyn3Tx=EDyn3TxStep;
    return true;
  }
  return false;
}
#endif

#ifdef _DYNOTRACTOR
bool CBrakeControl::CmdPwm(unsigned short value, unsigned short kI, unsigned short kP)
{
  if(CmdReady())
  {
    m_brakeCmd.protocol.action.cmd=(unsigned char)m_cDyn3Action[EDyn3TxPwm];
    m_brakeCmd.protocol.action.axle=m_defaultAxle;
    //if(m_fwType=='T' && m_fwVers>4)
    m_brakeCmd.protocol.data.pwmTractor.pwmH=value>>8;
    m_brakeCmd.protocol.data.pwmTractor.pwmL=value;
    m_brakeCmd.protocol.data.pwmTractor.kInteH=kI>>8;
    m_brakeCmd.protocol.data.pwmTractor.kInteL=kI;
    m_brakeCmd.protocol.data.pwmTractor.kPropH=kP>>8;
    m_brakeCmd.protocol.data.pwmTractor.kPropL=kP;
    SetCRC();
    m_Dyn3Tx=EDyn3TxStep;
    return true;
  }
  return false;
}
#endif
bool CBrakeControl::CmdDebug(bool on)
{
  if(CmdReady())
  {
    m_brakeCmd.protocol.action.cmd=(unsigned char)m_cDyn3Action[EDyn3TxDebug];
    m_brakeCmd.protocol.action.axle=m_defaultAxle;
    m_brakeCmd.protocol.data.onOff.on=on;
    m_bNewDebugMode=on;
    SetCRC();
    m_Dyn3Tx=EDyn3TxDebug;
    return true;
  }
  return false;
}

bool CBrakeControl::CmdDebugPwm(bool on)
{
  if(CmdReady())
  {
    m_brakeCmd.protocol.action.cmd=(unsigned char)m_cDyn3Action[EDyn3TxDebugPwm];
    m_brakeCmd.protocol.data.onOff.on=on;
    m_brakeCmd.protocol.action.axle=m_defaultAxle;
    m_bNewDebugPwmMode=on;
    SetCRC();
    m_Dyn3Tx=EDyn3TxDebugPwm;
    return true;
  }
  return false;
}

bool CBrakeControl::CmdStart(unsigned char dec)
{
  if(CmdReady())
  {
    m_brakeCmd.protocol.action.cmd=(unsigned char)m_cDyn3Action[EDyn3TxStart];
    m_brakeCmd.protocol.action.dec=dec;
    m_brakeCmd.protocol.action.axle=m_defaultAxle;
    SetCRC();
    m_Dyn3Tx=EDyn3TxStart;
    return true;
  }
  return false;
}

bool CBrakeControl::CmdSupercar(unsigned char valuePerc, unsigned short acc,
                            unsigned short start,
                            unsigned short stop,int axle)
{
  if(CmdReady())
  {
    m_brakeCmd.protocol.action.cmd=(unsigned char)m_cDyn3Action[EDyn3TxSetup];
    m_brakeCmd.protocol.action.axle=axle;
    m_brakeCmd.protocol.data.setupTestAcc.accH=acc>>8;
    m_brakeCmd.protocol.data.setupTestAcc.accL=acc;
    m_brakeCmd.protocol.data.setupTestAcc.startH=start>>8;
    m_brakeCmd.protocol.data.setupTestAcc.startL=start;
    m_brakeCmd.protocol.data.setupTestAcc.stopH=stop>>8;
    m_brakeCmd.protocol.data.setupTestAcc.stopL=stop;
    switch(valuePerc)
    {
      case 10:
        m_brakeCmd.protocol.data.setupTestAcc.unused=1;
      break;
      case 20:
        m_brakeCmd.protocol.data.setupTestAcc.unused=3;
      break;
      case 30:
        m_brakeCmd.protocol.data.setupTestAcc.unused=5;
      break;
      default:
        valuePerc=(valuePerc*255/100)&0xFE;
        m_brakeCmd.protocol.data.setupTestAcc.unused=valuePerc+0x07;
        //m_brakeCmd.protocol.data.setupSupercar.valPerc=((valuePerc*255/100) << 3)|0x07;
      break;
    }
    SetCRC();
    m_Dyn3Tx=EDyn3TxSetup;
    return true;
  }
  return false;
}

bool CBrakeControl::CmdReadPID()
{
  if(CmdReady())
  {
    m_brakeCmd.protocol.action.cmd=(unsigned char)m_cDyn3Action[EDyn3TxReadPID];
    m_brakeCmd.protocol.action.axle=m_defaultAxle;
    SetCRC();
    m_Dyn3Tx=EDyn3TxReadPID;
    return true;
  }
  return false;
}

bool CBrakeControl::CmdWritePID(unsigned short kP,
                               unsigned short kI,
                               unsigned short kD)
{
  if(CmdReady())
  {
    m_brakeCmd.protocol.action.cmd=(unsigned char)m_cDyn3Action[EDyn3TxWritePID];
    m_brakeCmd.protocol.action.axle=m_defaultAxle;
    m_brakeCmd.protocol.data.writePID.kPH=kP>>8;
    m_brakeCmd.protocol.data.writePID.kPL=kP;
    m_brakeCmd.protocol.data.writePID.kIH=kI>>8;
    m_brakeCmd.protocol.data.writePID.kIL=kI;
    m_brakeCmd.protocol.data.writePID.kDH=kD>>8;
    m_brakeCmd.protocol.data.writePID.kDL=kD;
    SetCRC();
    m_Dyn3Tx=EDyn3TxWritePID;
    return true;
  }
  return false;
}



bool CBrakeControl::CmdLight(bool on)
{
  if(CmdReady())
  {
    m_brakeCmd.protocol.action.cmd=(unsigned char)m_cDyn3Action[EDyn3TxLight];
    m_brakeCmd.protocol.action.axle=m_defaultAxle;
    m_brakeCmd.protocol.data.onOff.on=on;
    SetCRC();
    m_Dyn3Tx=EDyn3TxLight;
    return true;
  }
  return false;
}

void CBrakeControl::SendCmdQueue()
{
  if(m_Dyn3Tx==EDyn3TxNoCmd && GetDyn3Status()==EDyn3WaitStatus)
  {
    if(iCmdIndex==10)
      iCmdIndex=0;
    while(cCmdQueue[iCmdIndex]==EDyn3TxNoCmd && iCmdIndex<10)
      iCmdIndex++;
    if(iCmdIndex<10 && cCmdQueue[iCmdIndex])
    {
      switch(cCmdQueue[iCmdIndex])
      {
        case EDyn3TxStop:
             CmdStop();
        break;
        case EDyn3TxFwVerReq:
          CmdFwVerReq();
        break;
        case EDyn3TxDebugPwm:
           CmdDebugPwm(true);
        break;
        case EDyn3TxDebug:
          CmdDebug(false);
        break;
      }
      cCmdQueue[iCmdIndex]=EDyn3TxNoCmd;
      iCmdIndex++;
    }
  }
}


DWORD WINAPI ThreadDyn3(LPVOID lpParam)
{
  g_brakeControl.SetNotifier((bool*)lpParam);
  //Priority = tpHighest;
  //libera la memoria heap al termine del thread
  //FreeOnTerminate = false;
  while(g_brakeControl.RunStateMachine())
    g_brakeControl.SendCmdQueue();
  return 0;
}


//--

/*
void CreateDyn3(void(*pNotify)(unsigned char),TCHAR* cLog)
{
  strcpy(g_cDyn3Log,cLog);
  LogError(g_cDyn3Log,LOG_INFO,"Avvio modulo comunicazione Dyn3...");
  if(!g_pThreadDyn3)
  {
    g_pThreadDyn3=new CThreadDyn3(false,pNotify);
    if(g_pThreadDyn3)
      SetThreadPriority(g_pThreadDyn3,THREAD_PRIORITY_NORMAL);

  }
}
*/

bool CreateDyn3(bool *pNotify,TCHAR* cLog)
{
  strcpy(g_cDyn3Log,cLog);
  LogError(g_cDyn3Log,LOG_INFO,"Avvio modulo comunicazione Dyn3");
  if(!g_hThreadDyn3)
  {
    g_hThreadDyn3=CreateThread( NULL,0,ThreadDyn3, (void*)pNotify, 0, NULL);
    if(g_hThreadDyn3)
    {
      if(SetThreadPriority(g_hThreadDyn3,THREAD_PRIORITY_NORMAL))
        return true;

    }
  }
  return false;
}




bool ConnectDyn3(LPCTSTR cCom)
{
  LogError(g_cDyn3Log,LOG_INFO,"Connessione porta seriale Dyn3");
  if(g_hThreadDyn3)
  {
    g_brakeControl.Connect(cCom);
    return true;
  }
  return false;

}

void DisconnectDyn3()
{
  g_brakeControl.Disconnect();
  LogError(g_cDyn3Log,LOG_INFO,"Sconnessione porta Dyn3");
}

void DestroyDyn3()
{
  if (g_hThreadDyn3!=NULL)
  {
    g_brakeControl.Terminate();
    WaitForSingleObject(g_hThreadDyn3,400);
    CloseHandle(g_hThreadDyn3);
    g_hThreadDyn3=NULL;
  }
}



