//---------------------------------------------------------------------------

#ifndef CDyn3H
#define CDyn3H
//#include <Classes.hpp>
#include "CPort.h"

//---------------------------------------------------------------------------
class CDyn3 : public CPort
{
public:
  typedef enum
  {
    EAllAxle,
    EFrontAxle,
    ERearAxle,
  } EAxle;

  typedef enum
  {
    EDyn3NotAvail,/**<Dyn3 non risponde*/
    EDyn3Open,/**< Dyn3 non risponde */
    EDyn3WaitStatus,/**< Dyn3 in attesa di messaggio di stato freno */
    EDyn3SendCmd,
    EDyn3ReceiveData,
    EDyn3Ready,
    EDyn3DataErr,
    EDyn3StatusErr,
    EDyn3Disconnect,
    EDyn3Exit,
    EDyn3NotDefined
  }
  EDyn3Status;
  typedef enum
  {
    EDyn3TxNoCmd,
    EDyn3TxStop,    /**< 0x00 Stop command */
    EDyn3TxBrakeMan,/**< 0x01 Frenatura manuale */
    EDyn3TxSetup,   /**< 0x02 Impostazione prove accelerative*/
    EDyn3TxFwVerReq,/**< 0x03 Richiesta Fw version */
    EDyn3TxStep,    /**< 0x04 Controllo step rpm*/
    EDyn3TxPwm,     /**< 0x05 controllo PWM */
    EDyn3TxDebug,   /**< 0x06 scheda in modalità Debug On/Off */
    EDyn3TxStart,   /**< 0x08 Start prove accelerative*/
    EDyn3TxWritePID,/**< 0x0A Scrittura valori PID*/
    EDyn3TxReadPID, /**< 0x0C Lettura valori PID*/
    EDyn3TxDebugPwm,/**< 0x0D Modalità debug Pwm (messaggio di stato con PWM)*/
    EDyn3TxLight,   /**< 0x0E Impostazione veicoli leggeri*/
    EDyn3TxDecel,   /**< 0x40 Prova decelerativa*/
    EDyn3TxDisconnect,/**< Disconnetti porta seriale*/
    EDyn3TxTerminate, /**< Terminazione del thread associato */
  }
  EDyn3Tx;

  union EDyn3ErrorStatus
  {
    unsigned char cStatus;
    struct
    {
      unsigned char bDisconn:1;
      unsigned char bLostConn:1;
      unsigned char bTx:1;
      unsigned char bRxTimeout:1;
      unsigned char bRxWrongCRC:1;
      unsigned char bRx:1;
      unsigned char bBrakeDisconn:1;
      unsigned char bBrakeNoPower:1;
    } b;
  };

  union EDyn3DebugInfo
  {
    unsigned char cData[14];
    struct
    {
      unsigned char header[2];
      unsigned char targetRearH;
      unsigned char targetRearL;
      unsigned char pwmRear;
      unsigned char notUsed;
      unsigned char targetFrontH;
      unsigned char targetFrontL;
      unsigned char rpmRearH;
      unsigned char rpmRearL;
      unsigned char rpmFrontH;
      unsigned char rpmFrontL;
      unsigned char pwmFront;
      unsigned char CRC;
    } b;
  };


  /** Enumera Stati della comunicazione con scheda Dyn3 */
protected:

  typedef enum
  {
    EDyn3RxNull,
    EDyn3RxBrakeState,
    EDyn3RxResp,
    EDyn3RxFwVer,
    EDyn3RxPID,
    EDyn3RxError,
  }
  EDyn3Rx;

  union BrakeAction
  {
//    unsigned char c;
    struct
    {
      unsigned char cmd:4;
      unsigned char axle:2;
      unsigned char dec:1;
    };
  };

  union BrakeData
  {
    unsigned char c[7];
    struct BrakePerc
    {
      unsigned char value;
      unsigned char unused[6];
    } brakePerc;

    struct BrakePerc2
    {
      unsigned char valueH;
      unsigned char valueL;
      unsigned char unused[5];
    } brakePerc2;

    struct WritePID
    {
      unsigned char kPH;
      unsigned char kPL;
      unsigned char kIH;
      unsigned char kIL;
      unsigned char kDH;
      unsigned char kDL;
      unsigned char unused;
    } writePID;

    struct BrakeMan
    {
      unsigned char valuePercH;
      unsigned char valuePercL;
      unsigned char unused[5];
    };
    struct SetupTestAcc
    {
      unsigned char accH;
      unsigned char accL;
      unsigned char startH;
      unsigned char startL;
      unsigned char stopH;
      unsigned char stopL;
      unsigned char unused;
    } setupTestAcc;
    struct SetupSupercar
    {
      unsigned char unused[6];
      unsigned char valPerc;

    } setupSupercar;
    struct Step
    {
      unsigned char valueH;
      unsigned char valueL;
      unsigned char unused[5];
    } step;
    struct Pwm
    {
      unsigned char pwmFrontH;
      unsigned char pwmFrontL;
      unsigned char weight;
      unsigned char deriv;
      unsigned char pwmRearH;
      unsigned char pwmRearL;
      unsigned char unused;
    } pwm;
    struct PwmTractor
    {
      unsigned char pwmH;
      unsigned char pwmL;
      unsigned char kInteH;
      unsigned char kInteL;
      unsigned char kPropH;
      unsigned char kPropL;
      unsigned char unused;
    } pwmTractor;
    struct OnOff
    {
      unsigned char on;
      unsigned char unused[6];
    } onOff;
  };

  union BrakeSerial
  {
    unsigned char serial[11];
    struct BrakeStruct
    {
      unsigned char head[2];
      union BrakeAction action;
      union BrakeData data;
      unsigned char crc;
    } protocol;
  };


private:
  HANDLE hEvDyn3Ready;
  EDyn3Status m_Dyn3Status;
  BenchState  m_benchState;
  BenchState  m_benchStatePrec;
  EDyn3DebugInfo m_debugInfo;
  int m_iRxLen;
  int m_iRxErrCounter;
  int m_iTxErrCounter;
  int m_iCRCErrCounter;
  long m_lTimestamp;
  bool bIsOpen;
  union EDyn3ErrorStatus m_errorStatus;
  union EDyn3ErrorStatus m_errorStatusPrec;
  char m_cBuffer[2*MAX_BUFFER_LEN+4];
  bool ReceiveStatus();
  void ReceiveData();
  bool SendCmd();
  void Disconnect();
  unsigned char GetCRC(int iByteCount);
  bool GetAck();
  bool GetPID();
  bool GetFwVer();
protected:
  BrakeSerial m_brakeCmd;
  bool          *m_pNotify;
  unsigned char  m_fwVers;
  unsigned char  m_fwType;
  unsigned short m_PIDkP;
  unsigned short m_PIDkI;
  unsigned short m_PIDkD;
  unsigned short m_pwmAnte;
  unsigned short m_pwmPost;
  unsigned char cCmdQueue[10];
  static unsigned char m_cDyn3Action[16];
  EDyn3Tx     m_Dyn3Tx;
  EDyn3Rx     m_Dyn3Rx;
  bool m_bDebugPwmMode;
  bool m_bNewDebugPwmMode;
  bool m_bDebugMode;
  bool m_bNewDebugMode;
  EAxle m_defaultAxle;
  int iCmdIndex;
  void SetCRC();
  void CancelError()
  {
    if(m_Dyn3Status==EDyn3DataErr || m_Dyn3Status==EDyn3StatusErr)
      Connect(m_iPortIndex);
  }


  void Connect (int iPortIndex);

public:
  bool GetDebugPwmMode()
  {
    return m_bDebugPwmMode;
  }
  
  bool GetDebugMode()
  {
    return m_bDebugMode;
  }
  void GetDebugInfo(EDyn3DebugInfo* pDebugInfo)
  {
    memcpy (pDebugInfo,&m_debugInfo.cData,sizeof(EDyn3DebugInfo));
  }
  
  bool GetBrakeDisconnect(EAxle axle)
  {
    return ((axle==EFrontAxle && m_benchState.b.bBrakeAnteDisconnected) ||
            (axle==ERearAxle  && m_benchState.b.bBrakePostDisconnected) ||
            (axle==EAllAxle  && m_benchState.b.bBrakePostDisconnected && m_benchState.b.bBrakeAnteDisconnected));
  }

  bool GetBrakeNoPower(EAxle axle)
  {
    return ((axle==EFrontAxle && m_benchState.b.bBrakeAnteNotPowered) ||
            (axle==ERearAxle && m_benchState.b.bBrakePostNotPowered) ||
            (axle==EAllAxle && m_benchState.b.bBrakePostNotPowered && m_benchState.b.bBrakeAnteNotPowered));
  }
  bool isOn()
  {
    return (m_Dyn3Status!=EDyn3NotDefined &&
            m_Dyn3Status!=EDyn3Disconnect &&
            m_Dyn3Status!=EDyn3NotAvail   &&
            m_Dyn3Status!=EDyn3StatusErr  &&            
            m_Dyn3Status!=EDyn3Open       &&
            m_errorStatus.b.bDisconn!=1);
  }

  CDyn3()
  {
    m_fwType='\0';
    m_fwVers='\0';
    m_defaultAxle=EAllAxle;
    m_lTimestamp=0;
    m_iRxErrCounter=0;
    m_iTxErrCounter=0;
    m_iCRCErrCounter=0;
    m_Dyn3Status=EDyn3NotDefined;
    m_errorStatus.cStatus=0x00;
    m_errorStatus.b.bDisconn=1;
    m_errorStatus.b.bLostConn=0;
    m_errorStatusPrec=m_errorStatus;
    m_benchState.c=0xFF;
    m_benchStatePrec.c=0xFF;
    bIsOpen=false;
    memset(&m_brakeCmd.serial,0,sizeof(m_brakeCmd.serial));
    m_brakeCmd.protocol.head[0]=0xAA;
    m_brakeCmd.protocol.head[1]=0x55;
    m_Dyn3Tx=EDyn3TxNoCmd;
    m_Dyn3Rx=EDyn3RxNull;
	m_bDebugPwmMode=false;
    iCmdIndex=0;    
    memset (cCmdQueue,0,sizeof(cCmdQueue)); 
	cCmdQueue[0]=EDyn3TxFwVerReq;
    cCmdQueue[1]=EDyn3TxStop;
    cCmdQueue[2]=EDyn3TxDebugPwm;
    hEvDyn3Ready=CreateEvent(NULL,true,false,NULL);
    SetEvent(hEvDyn3Ready);
  }                                            
  bool IsCmdInProgress()
  {
    return m_Dyn3Tx!=EDyn3TxNoCmd;
  }

  void SetAxis(EAxle axle)
  {
    m_defaultAxle=axle;
  }
  void Connect (LPCTSTR cCom);
  bool RunStateMachine();

  bool CmdReady()
  {
    if(WaitForSingleObject(hEvDyn3Ready,50)==WAIT_OBJECT_0)
    {
      if(m_Dyn3Tx==EDyn3TxNoCmd)
      {
        ResetEvent(hEvDyn3Ready);
        memset(&m_brakeCmd,0,sizeof(m_brakeCmd));
        m_brakeCmd.protocol.head[0]=0xAA;
        m_brakeCmd.protocol.head[1]=0x55;
        return true;
      }                      
      else                
       return false;
    }

    return false;
  }

  unsigned char GetDefectCode()
  {
    return m_benchState.c;
  }

  EDyn3Status GetDyn3Status()
  {
    return m_Dyn3Status;
  }
  BenchState GetBrakeStatus()
  {
    return m_benchState;
  }
  EDyn3ErrorStatus GetErrorStatus()
  {
    return m_errorStatus;
  }

  unsigned short GetPwmAnte()
  {
    return m_pwmAnte;
  }

  unsigned short GetPwmPost()
  {
    return m_pwmPost;
  }

};

class CBrakeControl : public CDyn3
{
public:
  void SetNotifier(bool *pN)
  {
    m_pNotify=pN;
  }
  void autoDetect(bool bDetect=true);


  CBrakeControl(bool *pExternNotify=NULL)
  {

    m_pNotify=pExternNotify;
    /*

    memset (cCmdQueue,0,sizeof(cCmdQueue));
    cCmdQueue[0]=EDyn3TxFwVerReq;
    cCmdQueue[1]=EDyn3TxStop;
    cCmdQueue[2]=EDyn3TxDebugPwm;*/
  }
  
  void Disconnect()
  {
    m_Dyn3Tx=EDyn3TxDisconnect;
  }

  bool CmdStop();
  bool CmdFwVerReq();
  bool CmdBrakeMan(unsigned short value, int axle);

  bool CmdSetup(unsigned short accel,
                    unsigned short start,
                    unsigned short stop, int axle);
  bool CmdStep(unsigned short value, int axle);
#ifdef _DYNOTRUCK
  bool CmdPwm(unsigned short pwm,unsigned short weight,int axle);
#endif
#ifdef _DYNORACE
  bool CmdPwm(unsigned short pwm,unsigned short weight,int axle);
#endif
#ifdef _DYNOTRACTOR
  bool CmdPwm(unsigned short value, unsigned short kI=350, unsigned short kP=3000);
#endif
  bool CmdDebug(bool on);
  bool CmdStart(unsigned char dec=0);
  bool CmdWritePID(unsigned short kP,
                       unsigned short kI,
                       unsigned short kD);
  bool CmdReadPID();
  bool CmdDebugPwm(bool on);
  bool CmdLight(bool on);
  bool CmdSupercar(unsigned char valuePerc, unsigned short acc,
                            unsigned short start,
                            unsigned short stop,int axle);
  void Resume()
  {
     CancelError();
  }

  void SendCmdQueue();
  unsigned char GetFwVer()
  {
    return m_fwVers;
  }
  char GetFwType()
  {
    return m_fwType;
  }
  void GetPID(unsigned short* kP,unsigned short* kI,unsigned short* kD)
  {
    *kP=m_PIDkP;
    *kI=m_PIDkI;
    *kD=m_PIDkD;
  }
  void Terminate()
  {
    m_Dyn3Tx=EDyn3TxTerminate;
  }

private:
  unsigned short m_usecAnte;
  unsigned short m_usecPost;
  unsigned short m_celAnte;
  unsigned short m_celPost;
  unsigned short m_usecRefAnte;
  unsigned short m_usecRefPost;
//  unsigned char cCmdQueue[10];

};

bool CreateDyn3(bool*,TCHAR* cLog);
void DestroyDyn3();
bool ConnectDyn3(int iCom);
void DisconnectDyn3();

extern CBrakeControl g_brakeControl;

#endif
