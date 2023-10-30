//---------------------------------------------------------------------------

#ifndef CDyraH
#define CDyraH
//#include <Classes.hpp>
#include "CPort.h"

class CDyra : public CPort
{
public:
  typedef enum
  {
    EDyraNotAvail,/**<Dyra non risponde*/
    EDyraOpen,/**< Dyra porta aperta */
    EDyraWaitCmd,/**< Dyra in attesa di comando*/
    EDyraSendCmd,
    EDyraReceiveResp,
    EDyraReceiveData,
    EDyraDataErr,
    EDyraDisconnect,
    EDyraExit,
    EDyraNotDefined
  }
  EDyraStatus;
  typedef enum
  {
    EDyraTxNoCmd,
    EDyraTxStatus,  /**< 0x11 Richiesta di stato */
    EDyraTxStart,   /**< 0x81 Start command */
    EDyraTxStop,    /**< 0x82 Stop command*/
    EDyraTxFwVerReq,/**< 0x10 Richiesta versione firmware*/
    EDyraTxRele,    /**< 0x40 Comando al rele */
    EDyraTxLambda,  /**< 0x41 Set lambda param */
    EDyraTxDisconnect,
    EDyraTerminate,
    EDyraTxFlashPrepare, // 9
  }
  EDyraTx;

  typedef union DataIn1
  {
    unsigned char c[25];
    struct RaceData
    {
      unsigned char zeroChar;         //0
      unsigned char header;           //1
      unsigned char hlsProxi;         //2
      unsigned char sync;             //3 -> 0x55
      unsigned char encoderFrH;       // 4
      unsigned char encoderFrL;       // 5
      unsigned char encoderReH;       // 6
      unsigned char encoderReL;       // 7
      unsigned char consH;            // 8
      unsigned char consL;            // 9
      unsigned char TExt;             // 10
      unsigned char PExt;             // 11
      unsigned char PMap;             // 12
      unsigned char TOil;             // 13
      unsigned char Lambda1; // nel Truck corrisponde a sonda lock/unlock (non piu inserita nei nuovi banchi)
      unsigned char Lambda2;          // 15
      unsigned char TkH; //16
      unsigned char TkL; //17
      unsigned char cellReL;//18-19
      unsigned char cellReH;//18-19
      unsigned char cellFrL; //20-21
      unsigned char cellFrH; //20-21
      unsigned char time;
      unsigned char unused23;
      unsigned char checksum;
    } raceData;
    struct TractorData
    {
      unsigned char zeroChar;         //0
      unsigned char header;           //1
      unsigned char notUsed2;         //2
      unsigned char sync;             //3 -> 0x55
      unsigned char encoderH;       // 4
      unsigned char encoderL;       // 5
      unsigned char notUsed6;       // 6
      unsigned char notUsed7;       // 7
      unsigned char consH;            // 8
      unsigned char consL;            // 9
      unsigned char TExt;             // 10
      unsigned char PExt;             // 11
      unsigned char PMap;             // 12
      unsigned char TOil;             // 13
      unsigned char notUsed14; // nel Truck corrisponde a sonda lock/unlock (non piu inserita nei nuovi banchi)
      unsigned char notUsed15;          // 15
      unsigned char TkH; //16
      unsigned char TkL; //17
      unsigned char notUsed18;//18-19
      unsigned char notUsed19;//18-19
      unsigned char cellL; //20-21
      unsigned char cellH; //20-21
      unsigned char time;
      unsigned char unused23;
      unsigned char checksum;
    } tractorData;
  } DataIn;

protected:

  union EDyraErrorStatus
  {
    unsigned char cStatus;
    struct
    {
      unsigned char bDisconn:1;
      unsigned char bLostConn:1;
      unsigned char bRxTimeout:1;
      unsigned char bRxWrongCRC:1;
      unsigned char bAck:3; 
      unsigned char bWrongCell:1;
    } b;
  };

  union DyraTxCmd
  {
    struct
    {
      unsigned char msg[5];
      unsigned char len;
    } serial;

    struct Cmd
    {
      unsigned char head;
      unsigned char action;
      unsigned char sync;
      unsigned char ckSum;
//      unsigned char unused;
    } cmd;
    struct CmdPrm1
    {
      unsigned char head;
      unsigned char action;
      unsigned char sync;
      unsigned char param;
      unsigned char ckSum;

    } cmdPrm1;
  };

  DataIn m_dataIn;
  int iCmdIndex;
  unsigned char cCmdQueue[10];
private:
  HANDLE hEvDyraReady;
  EDyraStatus m_DyraStatus;

  int m_iRxErrCounter;
  int m_iTxErrCounter;
  int m_iCRCErrCounter;
  long m_lTimestamp;
  bool bIsOpen;
  char m_cBuffer[2*MAX_BUFFER_LEN+4];
  unsigned char m_Rx[MAX_BUFFER_LEN];
  union EDyraErrorStatus m_errorStatus;
  union EDyraErrorStatus m_errorStatusPrec;
  void ReceiveResp();
  bool ReceiveData();
  bool SendCmd();
  void Disconnect();
  unsigned char GetCRC(int iByteCount);
  bool GetAck();
  bool GetData();
  bool GetStatus();
  bool GetFwVer();
  int m_iNFrameErr;
  int m_iNFrameErrPrec;
protected:
  DyraTxCmd   m_dyraCmd;
  bool m_bDataNotified;
  void(*pNotify)(unsigned char);
  unsigned char  m_fwVers;
  unsigned char  m_fwType;

  unsigned short m_pwmAnte;
  unsigned short m_pwmPost;
  static unsigned char m_cDyraAction[11];
  EDyraTx     m_DyraTx;
  int m_iPortNumber;
  void SetCRC();
  void CancelError()
  {
    if(m_DyraStatus==EDyraDataErr)
      Connect(m_iPortNumber);
  }
  void Connect(int iPortNumber);
public:

  bool isOn()
  {
    return (!m_errorStatus.b.bDisconn &&
             m_DyraStatus!=EDyraOpen  &&
             m_DyraStatus!=EDyraNotAvail &&
             m_DyraStatus!=EDyraNotDefined &&
             m_DyraStatus!=EDyraDataErr);
  }

  bool isRxError()
  {
    return (m_errorStatus.b.bRxTimeout || m_errorStatus.b.bRxWrongCRC);
  }

  bool isAckError()
  {
    return (m_errorStatus.b.bAck);
  }
  bool isWrongCell()
  {
    return (m_errorStatus.b.bWrongCell);
  }

  typedef enum
  {
    EAllAxle,
    EFrontAxle,
    ERearAxle,
  } EAxle;

  CDyra()
  {
    m_bDataNotified=false;
    m_iNFrameErr=0;
    m_iNFrameErrPrec=0;    
    m_lTimestamp=0;
    m_iRxErrCounter=0;
    m_iTxErrCounter=0;
    m_iCRCErrCounter=0;
    memset(m_dataIn.c,0,sizeof(m_dataIn));
    m_DyraStatus=EDyraNotDefined;
    m_errorStatus.cStatus=0x00;
    m_errorStatus.b.bDisconn=1;
    m_errorStatusPrec=m_errorStatus;
    bIsOpen=false;
    memset(&m_dyraCmd,0,sizeof(m_dyraCmd));
    m_dyraCmd.cmd.head=0xAA;
    m_dyraCmd.cmd.sync=0x55;
    m_DyraTx=EDyraTxNoCmd;
    hEvDyraReady=CreateEvent(NULL,true,false,NULL);
    SetEvent(hEvDyraReady);
    memset(m_Rx,0,sizeof(m_Rx));
  }

  void Connect (LPCTSTR cCom);

  bool RunStateMachine();

  bool CmdReady()
  {
    if(WaitForSingleObject(hEvDyraReady,200)==WAIT_OBJECT_0)
    {
      ResetEvent(hEvDyraReady);
      memset(&m_dyraCmd,0,sizeof(m_dyraCmd));
      m_dyraCmd.cmd.head=0xAA;
      m_dyraCmd.cmd.sync=0x55;
      return true;
    }
    SetEvent(hEvDyraReady);
    return false;
  }

  EDyraStatus GetDyraStatus()
  {
    return m_DyraStatus;
  }

  EDyraErrorStatus GetErrorStatus()
  {
    return m_errorStatus;
  }
  int GetErrorFrames()
  {
    return m_iNFrameErr;
  }
};

class CDyraControl : public CDyra
{
public:
  void SetNotifier(void(*pN)(unsigned char))
  {
    pNotify=pN;
  }
  void autoDetect(bool bDetect=true);

  CDyraControl(void (*pExternNotify)(unsigned char)=NULL)
  {
    bPortFinder=false;
           pNotify=pExternNotify;
    iCmdIndex=0;
    memset (cCmdQueue,0,sizeof(cCmdQueue));
  }
  void GetDyra(DataIn & dataIn)
  {
    memcpy(dataIn.c,m_dataIn.c,sizeof(DataIn));
    m_bDataNotified=false;
  }
  bool isDataReady()
  {
    return m_bDataNotified;
  }
  void Disconnect()
  {
    m_DyraTx=EDyraTxDisconnect;
  } 


  bool CmdFwVerReq();
  bool CmdStart();
  bool CmdStop();
  bool CmdStatus();
  bool CmdFlashPrepare();
  bool CmdLambda(unsigned char prm);
  bool CmdRele(unsigned char prm);

  void Terminate()
  {
    m_DyraTx=EDyraTerminate;
  }

  unsigned char GetCmd()
  {
    return m_DyraTx;
  }
  
  void Resume()
  {
     CancelError();
  }


  unsigned char GetFwVer()
  {
    return m_fwVers;
  }
  unsigned char GetFwType()
  {
    return m_fwType;
  }

  void SendCmdQueue();

};

bool CreateDyra(void(*pNotify)(),TCHAR* cLog);
void DestroyDyra();
bool ConnectDyra(int iCom);
void DisconnectDyra();

extern CDyraControl g_dyraControl;

//---------------------------------------------------------------------------
#endif
