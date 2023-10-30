//---------------------------------------------------------------------------

//#include <vcl.h>
#include <windows.h> <WINDOWS.H>
#include <stdio.h>
#include <shfolder.h>
#include <time.h>
#pragma hdrstop

#pragma package(smart_init)
#include "CDyra.h"
#include "Log.h"


/** Definizioni Rx header*/
/// Messaggi di stato
#define DYRA_H_CHAR   0xAA
#define DYRA_SYNC_CHAR  0x55
/// Messaggi risposta a comando
/// Definizioni tipi di Ack
#define DYRA_ACK_OK         0x01
#define DYRA_ACK_WRONG_CRC  0x07
#define DYRA_ACK_TIMEOUT    0x05
/// Definizione numero tentativi invio messaggio
#define MAX_ACQ_ERR  10
#define MAX_CMD_ERR  3
#define TIME_TO_REINIT  2000
#define TIMEOUT_MSG 100

/// Definizione lunghezza risposte a comando
#define LEN_RESP_FW_REQ   8
#define LEN_RESP_STATUS   6
#define LEN_ACK           1

HANDLE g_hThreadDyra=0;


CDyraControl g_dyraControl;

//char g_cDyraLog[MAX_PATH]="C:\\Dyra.log";
extern char g_cDyraLog[MAX_PATH];
unsigned char CDyra::m_cDyraAction[11]= {
        0x00,// NoCmd (not mapped)
        0x11,// Status req
        0x81,// Start acq
        0x82,// Stop  acq
        0x10,// Fw req
        0x40,// Rele
        0x41,// Lambda write                        
        0x00,// Not mapped
        0x00,// Not mapped
        0x21,// Flash prepare
        0x00,
        };

void CDyra::Connect(LPCTSTR cCom)
{
  m_iNFrameErr=0;
  m_iNFrameErrPrec=0;
  m_DyraTx=EDyraTxNoCmd;
  if(pNotify)
    pNotify(m_DyraTx);
  m_iTxErrCounter=0;
  m_iRxErrCounter=0;
  m_lTimestamp=0;

  if( m_DyraStatus==EDyraNotAvail ||
      m_DyraStatus==EDyraNotDefined ||
      m_DyraStatus==EDyraDataErr)
    m_DyraStatus=(CPort::Connect(cCom,TIMEOUT_MSG))?EDyraOpen:EDyraNotAvail;
  if(m_DyraStatus==EDyraOpen)
  {
    iCmdIndex=0;
    cCmdQueue[0]=EDyraTxStop;
    cCmdQueue[1]=EDyraTxStop;
    cCmdQueue[2]=EDyraTxFwVerReq;
    cCmdQueue[3]=EDyraTxStatus;
    cCmdQueue[4]=EDyraTxLambda;
    cCmdQueue[5]=EDyraTxStart;
  } else if (bPortFinder==false)
    m_errorStatus.b.bLostConn=1;

}

void CDyra::Connect(int iPortNumber)
{
  m_iNFrameErr=0;
  m_iNFrameErrPrec=0;
  m_DyraTx=EDyraTxNoCmd;
  if(pNotify)
    pNotify(m_DyraTx);
  m_iTxErrCounter=0;
  m_iRxErrCounter=0;
  m_lTimestamp=0;

  if( m_DyraStatus==EDyraNotAvail ||
      m_DyraStatus==EDyraNotDefined ||
      m_DyraStatus==EDyraDataErr)
    m_DyraStatus=(CPort::Connect(iPortNumber,TIMEOUT_MSG))?EDyraOpen:EDyraNotAvail;
  if(m_DyraStatus==EDyraOpen)
  {
    iCmdIndex=0;
    cCmdQueue[0]=EDyraTxStop;
    cCmdQueue[1]=EDyraTxStop;
    cCmdQueue[2]=EDyraTxFwVerReq;
    cCmdQueue[3]=EDyraTxStatus;
    cCmdQueue[4]=EDyraTxLambda;
    cCmdQueue[5]=EDyraTxStart;
    //m_errorStatus.cStatus=0x00;
  }

}


bool CDyra::ReceiveData()
{
  #define STR_DATA_LEN  25
  #define MATCH_HEADER (m_cBuffer[0]==0x00 && m_cBuffer[1]==0xAA && m_cBuffer[3]=0x55)
  static bool bResult;
  static int iRxCursor=0,i,j;
  bResult=false;
  // In caso di frame errato bisogna allinearsi al primo header corretto ricevuto
    i=0;
  if(m_iNFrameErr>0)
  {
    do
    {
      Rx(1);
      Sleep(1);
      if(m_dwRxCount==1)
      {
        if((iRxCursor==0 && m_cRxBuffer[0]==0x00) ||
           (iRxCursor==1 && m_cRxBuffer[0]==0xAA) ||
           (iRxCursor==2)                               ||
           (iRxCursor==3 && m_cRxBuffer[0]==0x55))
          m_Rx[iRxCursor++]=m_cRxBuffer[0];
        else
          iRxCursor=0;
        i++;
      }
    }
    while(iRxCursor<4 && i<STR_DATA_LEN);
    if(m_bActiveLog)
    {
      sprintf(m_cBuffer,"%08d RE ",clock());
      for(int i=0;i<iRxCursor;i++)
        sprintf(&m_cBuffer[12+i*2],"%02X",m_Rx[i]);
      LogError(g_cDyraLog,LOG_INFO,m_cBuffer);
    }
  }
  m_dwRxCount=0;
  Rx(STR_DATA_LEN-iRxCursor);
  if(m_dwRxCount>0)
  {
    if(m_bActiveLog)
    {
      sprintf(m_cBuffer,"%08d Rx ",clock());
      for(int i=0;i<m_dwRxCount;i++)
        sprintf(&m_cBuffer[12+i*2],"%02X",m_cRxBuffer[i]);
      LogError(g_cDyraLog,LOG_INFO,m_cBuffer);
    }
    memcpy(&m_Rx[iRxCursor],GetBufferPos(0),m_dwRxCount);
    iRxCursor+=m_dwRxCount;
    if(iRxCursor>=2)
    {
      i=0;

      while(((i<=iRxCursor-4)&&(m_Rx[i]!=0x00 || m_Rx[i+1]!=DYRA_H_CHAR || m_Rx[i+3]!=DYRA_SYNC_CHAR)) ||
            ((i>iRxCursor-4 && i<=iRxCursor-2)&&(m_Rx[i]!=0x00 || m_Rx[i+1]!=DYRA_H_CHAR)))
        i++;
      if(i>0)
      {
        for(j=i;j<iRxCursor;j++)
          m_Rx[j-i]=m_Rx[j];
        iRxCursor-=i;
        if(m_bActiveLog)
        {
          sprintf(m_cBuffer,"R%08d Rx ",clock());
          for(int i=0;i<iRxCursor;i++)
            sprintf(&m_cBuffer[13+i*2],"%02X",m_Rx[i]);
          LogError(g_cDyraLog,LOG_INFO,m_cBuffer);
        }
      }
      if(m_Rx[0]==0x00 && m_Rx[1]==DYRA_H_CHAR && m_Rx[3]==DYRA_SYNC_CHAR)
      {
        if(iRxCursor>=STR_DATA_LEN)
        {
          if(GetCRC(STR_DATA_LEN)==m_Rx[STR_DATA_LEN-1])
          {
            m_bDataNotified=true;
            bResult=true;
            m_iNFrameErr=0;
            memcpy(m_dataIn.c,m_Rx,sizeof(m_dataIn));
            if(m_bActiveLog)
              LogError(g_cDyraLog,LOG_INFO,"OK");
          }
          if(iRxCursor>STR_DATA_LEN)
            iRxCursor=iRxCursor;
          iRxCursor=0;
        }
        else
          return true;

      }
      else
        iRxCursor=0;
    }
    else
      return true;
  }
  if(bResult)
  {
    m_iRxErrCounter=0;
    m_errorStatus.cStatus&=0xF0; // Azzera errore lettura e disconnessione
    bIsOpen=true;
    m_lTimestamp=clock();
    m_DyraStatus=EDyraReceiveData;
    return true;
  }
  if(m_dwRxCount)
  {
    m_iNFrameErr++;
  }
  m_iRxErrCounter++;

  if(m_iNFrameErr)
    sprintf(m_cBuffer,"%08d Errore di lettura o dati parziali (n %d consecutivi)",clock(),m_iNFrameErr);
  else
    sprintf(m_cBuffer,"%08d Nessuna ricezione di dati",clock());
  LogError(g_cDyraLog,LOG_INFO,m_cBuffer);

  if(m_iRxErrCounter>=MAX_ACQ_ERR)
  {
     m_DyraStatus=EDyraDataErr;
     m_iRxErrCounter=0;
  }
  SetEvent(hEvDyraReady);
  return false;
}

bool CDyra::GetAck()
{
  if(m_dwRxCount)
  {
    if(m_bActiveLog)
    {
      sprintf(m_cBuffer,"Rx ");
      for(int i=0;i<m_dwRxCount;i++)
        sprintf(&m_cBuffer[3+i*2],"%02X",m_cRxBuffer[i]);
      LogError(g_cDyraLog,LOG_INFO,m_cBuffer);
    }
  }
  if(m_dwRxCount==LEN_ACK)
  {      // Check stato ricezione
    m_errorStatus.cStatus&=0xF0;
    m_errorStatus.b.bAck=GetBuffer(m_dwRxCount-LEN_ACK);
    if(m_errorStatus.b.bAck==0)
    {
      if(pNotify)
        pNotify(m_DyraTx);
      m_iRxErrCounter=0;
      return true;
    }
  }
  else
    m_errorStatus.b.bRxTimeout=1;
  return false;
}

bool CDyra::GetFwVer()
{
  /// Check lunghezza messaggio
  if(m_dwRxCount)
  {
    if(m_bActiveLog)
    {
      sprintf(m_cBuffer,"Rx ");
      for(int i=0;i<m_dwRxCount;i++)
        sprintf(&m_cBuffer[3+i*2],"%02X",m_cRxBuffer[i]);
      LogError(g_cDyraLog,LOG_INFO,m_cBuffer);
    }
  }
  memcpy(m_Rx,GetBufferPos(0),m_dwRxCount);  
  if(!(m_dwRxCount==LEN_RESP_FW_REQ))
    m_errorStatus.b.bRxTimeout=1;
  /// check header messaggio non corretto
  else if(!(GetBuffer(0)==0x00))
    m_errorStatus.b.bRxWrongCRC=1;
  /// Controllo CRC
  else if(GetCRC(LEN_RESP_FW_REQ)==m_Rx[LEN_RESP_FW_REQ-1]/*GetBuffer(LEN_RESP_FW_REQ-1)*/)
  {
    /// Se CRC Ok prelevo Fw ver e tipo, aggiorna stato
    m_fwVers=GetBuffer(1);
    if(pNotify)
      pNotify(m_DyraTx);
    m_errorStatus.cStatus&=0xF0;
    m_iRxErrCounter=0;
    return true;
  }
  m_errorStatus.b.bRxWrongCRC=1;
  return false;
}

bool CDyra::GetStatus()
{
  /// Check lunghezza messaggio
  if(m_dwRxCount)
  {
    if(m_bActiveLog)
    {
      sprintf(m_cBuffer,"Rx ");
      for(int i=0;i<m_dwRxCount;i++)
        sprintf(&m_cBuffer[3+i*2],"%02X",m_cRxBuffer[i]);
      LogError(g_cDyraLog,LOG_INFO,m_cBuffer);
    }
  }
  memcpy(m_Rx,GetBufferPos(0),m_dwRxCount);
  if((m_dwRxCount!=LEN_RESP_STATUS) && (m_dwRxCount!=LEN_ACK))
    m_errorStatus.b.bRxTimeout=1; // Nessun messaggio coerente
  else if(m_dwRxCount==LEN_ACK)
    m_errorStatus.b.bAck=GetBuffer(0);
  else if(!(GetBuffer(0)==0x00))
    m_errorStatus.b.bRxWrongCRC=1;
  /// Controllo CRC
  else if(GetCRC(LEN_RESP_STATUS)==GetBuffer(LEN_RESP_STATUS-1))
  {
    /// Se CRC Ok prelevo Fw ver e tipo, aggiorna stato
    m_errorStatus.b.bWrongCell=(GetBuffer(1) & 0x10);
    m_errorStatus.cStatus&=0xF0;
    m_iRxErrCounter=0;
    if(pNotify)
      pNotify(m_DyraTx);
    return true;
  }
  m_errorStatus.b.bRxWrongCRC=1;
  return false;
}

void CDyra::ReceiveResp()
{
  static bool bResp;


  bResp=true;
  switch (m_DyraTx)
  { // Risposta semplice: 0xAA 0x01 ok / 0x05 / 0x07
    case EDyraTxStart:
    case EDyraTxLambda:
    case EDyraTxRele:
      Rx(1);
      bResp=GetAck();     
    break;
    case EDyraTxFwVerReq:
      Sleep(50);
      Rx(LEN_RESP_FW_REQ);
      bResp=GetFwVer();
    break;
    case EDyraTxStatus:
      Rx(LEN_RESP_STATUS);
      bResp=GetStatus();
    break;
    case EDyraTxStop: // La risposta allo stop viene inviata una volta sola,
                      // se invio altri stop successivi la scheda non risponde!
                      // Inviare stop piu volte per essere sicuri dell'invio
      Sleep(25);
    default:
      m_DyraStatus=EDyraWaitCmd;
    //break;
  }
  /// Gestione degli errori, cambio di stato e comando pendente
  /// Su RISPOSTA ERRATA:
  /// - incremento il numero di errori,
  /// - controllo se ho raggiunto max tentativi,
  ///   in tal caso aggiorno lo status a DataErr e azzero errore e comando pendente;
  /// Su RISPOSTA CORRETTA:
  /// - Port finder (autodetect port) termina se attivo
  /// - se ultimo comando è Start porta lo stato a ReceiveData (acquisizione continua a25ms)
  ///   altrimenti porta lo stato a WaitCmd per processare nuovo comando
  /// - Cancella comando pendente
  if(!bResp)
  {
    m_iRxErrCounter++;
    if(m_iRxErrCounter>=MAX_CMD_ERR)
    {
      m_DyraStatus=EDyraDataErr;
      m_iRxErrCounter=0;
      m_DyraTx=EDyraTxNoCmd;
    }
    else
      m_DyraStatus=EDyraWaitCmd;
  }
  else
  {
    if(m_DyraTx!=EDyraTxStop)
      bPortFinder=false;
    if(m_DyraTx==EDyraTxStart)
      m_DyraStatus=EDyraReceiveData;
    else
     m_DyraStatus=EDyraWaitCmd;
    m_DyraTx=EDyraTxNoCmd;
    m_iRxErrCounter=0;
  }

  SetEvent(hEvDyraReady);
}

void CDyra::Disconnect()
{

  m_DyraStatus=EDyraNotAvail;
  m_DyraTx=EDyraTxNoCmd;
  m_errorStatus.b.bDisconn=1;
  CPort::Disconnect();
}

bool CDyra::SendCmd()
{
  /// Al primo tentantivo di trasmissione setta il timestamp per timeout operazioni
  if(m_iTxErrCounter==0 && m_iRxErrCounter==0)
    m_lTimestamp=clock();
  //if(!(m_errorStatus.b.bDisconn))
  {
    if(m_bActiveLog)
    {
      sprintf(m_cBuffer,"Tx ");
      for(int i=0;i<m_dyraCmd.serial.len;i++)
        sprintf(&m_cBuffer[3+i*2],"%02X",((unsigned char*)&m_dyraCmd)[i]);
      LogError(g_cDyraLog,LOG_INFO,m_cBuffer);
    }
    Sleep(20);
    if(Tx((unsigned char*)&m_dyraCmd,m_dyraCmd.serial.len))
    {
      m_DyraStatus=EDyraReceiveResp;
      m_iTxErrCounter=0;

      return true;
    }
    else
    {
      m_iTxErrCounter++;
    }
  }
  //else
  //  m_DyraStatus=EDyraDataErr;
    
  if(m_iTxErrCounter>MAX_CMD_ERR)
  {
     m_iTxErrCounter=0;
     m_DyraStatus=EDyraDataErr;
     m_DyraTx=EDyraTxNoCmd;

  }
  if(m_dwTxCount)
  {
    sprintf(m_cBuffer,"Tx ERROR");
    LogError(g_cDyraLog,LOG_INFO,m_cBuffer);
  }
  //SetEvent(hEvDyraReady);
  return false;
}

void CDyra::SetCRC()
{
  static int i;
  m_dyraCmd.serial.msg[m_dyraCmd.serial.len-1]=0;
  for(i=0;i<m_dyraCmd.serial.len-1;i++)
    m_dyraCmd.serial.msg[m_dyraCmd.serial.len-1]+=m_dyraCmd.serial.msg[i];
}

/// Calcola il chechsum su 1 byte degli ultimi iByteCount del buffer rx
unsigned char CDyra::GetCRC(int iByteCount)
{
  static int i;
  static unsigned char cCRC;
  cCRC=0;
  for(i=0;i<iByteCount-1;i++)
    cCRC+=m_Rx[i];
  return cCRC;

}


/// Macchina a stati finiti implementa il ciclo di gestione della scheda Dyra
bool CDyra::RunStateMachine()
{
  if(m_DyraStatus!=EDyraNotAvail &&
     m_DyraStatus!=EDyraNotDefined &&
     m_DyraStatus!=EDyraDisconnect &&
     m_DyraStatus!=EDyraExit)
     if(m_DyraTx==EDyraTxDisconnect)
       m_DyraStatus=EDyraDisconnect;
  switch(m_DyraStatus)
  {
    case EDyraNotAvail:
    case EDyraNotDefined:
       if(m_DyraTx==EDyraTerminate)
         return false;

      if(bPortFinder)
      {
        if(m_iPortIndex<GetComCount()-1)
        {
          m_iPortIndex++;        
          Connect(m_iPortIndex);
        }
        else
        {
          m_lTimestamp=clock();
          bPortFinder=false;
        }

      }
      else if(m_errorStatus.b.bLostConn)
      {
        if(clock()-m_lTimestamp>TIME_TO_REINIT)
        {
          Connect(m_cPort);
        }
      }
    break;
    case EDyraOpen:
    case EDyraWaitCmd:
      if(m_DyraTx==EDyraTerminate)
        m_DyraStatus=EDyraExit;
      else if(m_DyraTx!=EDyraTxNoCmd)
        m_DyraStatus=EDyraSendCmd;
    break;
    case EDyraSendCmd:
      SendCmd();
    break;
    case EDyraReceiveResp:
      ReceiveResp();
    break;
    case EDyraReceiveData:
      ReceiveData();
      if(m_DyraTx==EDyraTxStop || m_DyraTx==EDyraTxRele)
        m_DyraStatus=EDyraSendCmd;
    break;
    case EDyraDisconnect:
      Disconnect();
    break;
    case EDyraDataErr:
        m_errorStatus.b.bLostConn=1;
        Disconnect();
    break;
    case EDyraExit:
       Disconnect();
    return false;
  }

  /// Notifica all'esterno i cambi di stato della scheda e gli errori
  if(pNotify && m_DyraTx==EDyraTxNoCmd)
  {
    /// Controlla se è cambiato lo stato scheda o errore
    if(m_errorStatusPrec.cStatus!=m_errorStatus.cStatus ||
       m_DyraStatus==EDyraNotDefined ||
       m_bDataNotified ||
       m_iNFrameErr>=MAX_ACQ_ERR)
    {
       //m_iNFrameErrPrec=m_iNFrameErr;
       pNotify(m_DyraTx);
       m_errorStatusPrec.cStatus=m_errorStatus.cStatus;
    }
  }
  Sleep(5);
  return true;
}

void CDyraControl::autoDetect(bool bDetect)
{
  if(bDetect && (GetDyraStatus()==EDyraNotAvail || GetDyraStatus()==EDyraNotDefined))
  {
    iCmdIndex=0;
    cCmdQueue[0]=EDyraTxStop;
    cCmdQueue[1]=EDyraTxStop;
    cCmdQueue[2]=EDyraTxFwVerReq;
    cCmdQueue[3]=EDyraTxStatus;
    cCmdQueue[4]=EDyraTxLambda;
    cCmdQueue[5]=EDyraTxStart;
    CPort::autoDetect(bDetect);
  }
}
bool CDyraControl::CmdStop()
{


  if(CmdReady())
  {
    m_dyraCmd.cmd.action=m_cDyraAction[EDyraTxStop];
    m_dyraCmd.serial.len=4;
    SetCRC();
    m_DyraTx=EDyraTxStop;
    return true;
  }
  return false;
}

bool CDyraControl::CmdFwVerReq()
{
  if(CmdReady())
  {
    m_dyraCmd.cmd.action=m_cDyraAction[EDyraTxFwVerReq];
    m_dyraCmd.serial.len=4;
    SetCRC();
    m_DyraTx=EDyraTxFwVerReq;
    return true;
  }
  return false;
}

bool CDyraControl::CmdStart()
{
  if(CmdReady())
  {
    m_dyraCmd.cmd.action=m_cDyraAction[EDyraTxStart];
    m_dyraCmd.serial.len=4;    
    SetCRC();
    m_DyraTx=EDyraTxStart;
    return true;
  }
  return false;
}

bool CDyraControl::CmdStatus()
{
  if(CmdReady())
  {
    m_dyraCmd.cmd.action=m_cDyraAction[EDyraTxStatus];
    m_dyraCmd.serial.len=4;
    SetCRC();
    m_DyraTx=EDyraTxStatus;
    return true;
  }
  return false;
}

bool CDyraControl::CmdLambda(unsigned char prm)
{
  if(CmdReady())
  {
    m_dyraCmd.cmdPrm1.action=m_cDyraAction[EDyraTxLambda];
    m_dyraCmd.cmdPrm1.param=prm;
    m_dyraCmd.serial.len=5;
    SetCRC();
    m_DyraTx=EDyraTxLambda;
    return true;
  }
  return false;
}

bool CDyraControl::CmdRele(unsigned char prm)
{
  if(CmdReady())
  {
    m_dyraCmd.cmdPrm1.action=m_cDyraAction[EDyraTxRele];
    m_dyraCmd.cmdPrm1.param=prm;
    m_dyraCmd.serial.len=5;
    SetCRC();
    m_DyraTx=EDyraTxRele;
    return true;
  }
  return false;
}

bool CDyraControl::CmdFlashPrepare()
{
  if(CmdReady())
  {
    m_dyraCmd.cmdPrm1.action=m_cDyraAction[EDyraTxFlashPrepare];
    m_dyraCmd.serial.len=4;
    SetCRC();
    m_DyraTx=EDyraTxFlashPrepare;
    return true;
  }
  return false;
}


/// Gestisce l'invio dei messaggi in accodamento
void CDyraControl::SendCmdQueue()
{
  if(m_DyraTx==EDyraTxNoCmd &&
     (GetDyraStatus()==EDyraWaitCmd ||
      GetDyraStatus()==EDyraOpen))
  {
    if(iCmdIndex==10)
      iCmdIndex=0;
    while(cCmdQueue[iCmdIndex]==EDyraTxNoCmd && iCmdIndex<10)
      iCmdIndex++;
    if(iCmdIndex<10 && cCmdQueue[iCmdIndex])
    {
      switch(cCmdQueue[iCmdIndex])
      {
        case EDyraTxStop:
             CmdStop();
        break;
        case EDyraTxFwVerReq:
          CmdFwVerReq();
        break;
        case EDyraTxStart:
          CmdStart();
        break;
        case EDyraTxLambda:
          CmdLambda(1);
        break;
        case EDyraTxStatus:
          CmdStatus();
        break;
      }
      cCmdQueue[iCmdIndex]=EDyraTxNoCmd;
      iCmdIndex++;
    }
  }
}


DWORD WINAPI ThreadDyra(LPVOID lpParam)
{
  void(*pNotify)(unsigned char);
  pNotify=(void(*)(unsigned char))lpParam;

  g_dyraControl.SetNotifier(pNotify);
  //Priority = tpHighest;
  //libera la memoria heap al termine del thread
  //FreeOnTerminate = false;
  while(g_dyraControl.RunStateMachine())
    g_dyraControl.SendCmdQueue();
  return 0;
}

//---------------------------------------------------------------------------

bool CreateDyra(void (*pNotify)(),TCHAR* cLog)
{
  strcpy(g_cDyraLog,cLog);
  LogError(g_cDyraLog,LOG_INFO,"Avvio modulo comunicazione Dyra");
  if(!g_hThreadDyra)
  {
    g_hThreadDyra=CreateThread( NULL,0,ThreadDyra, pNotify, 0, NULL);
    if(g_hThreadDyra)
    {
      if(SetThreadPriority(g_hThreadDyra,THREAD_PRIORITY_NORMAL))
        return true;

    }
  }
  return false;
}

bool ConnectDyra(LPCTSTR cCom)
{
  LogError(g_cDyraLog,LOG_INFO,"Connessione porta seriale");
  if(g_hThreadDyra)
  {
    g_dyraControl.Connect(cCom);
    return true;
  }
  return false;

}

void DisconnectDyra()
{
  g_dyraControl.Disconnect();
  LogError(g_cDyraLog,LOG_INFO,"Sconnessione porta");
}

void DestroyDyra()
{
  g_dyraControl.Terminate();
  WaitForSingleObject(g_hThreadDyra,100);
  CloseHandle(g_hThreadDyra);
  g_hThreadDyra=0;
}


