#ifndef CPORT_
#define CPORT_

#include <windows.h>

#define MAX_BUFFER_LEN  200

  union BenchState
  {
    unsigned char c;
    struct
    {
      unsigned char bRpmAnte : 1;
      unsigned char bRpmPost : 1;
      unsigned char bNotUsed : 2;
      unsigned char bBrakeAnteDisconnected : 1;
      unsigned char bBrakeAnteNotPowered   : 1;
      unsigned char bBrakePostDisconnected : 1;
      unsigned char bBrakePostNotPowered   : 1;
    } b;
  };


/** \class CPort
  * Gestione della porta seriale generica
  * in questa classe vengono gestite tutte le operazioni e gli stati presenti in una porta
  * la classe realizza e gestisce una macchina a stati finiti che descrive il funzionamento
  * di ogni porta seriale gestita nell'applicazione. Essa implementa un thread permanente
  * e una funzione di gestione all'interno della quale vengo effettuate tutte le chiamate
  * di cambio di stato e vengono gestite tutte le funzioni di transizione.
*/
class CPort
{
  /** Enumera Stati della porta COM */
  typedef enum
  {
    EComOff,/**< Com non risponde */
    EComOk,/**< Com pronta */
    EComTxErr,/**<Errore durante la scrittura */
    EComRxErr,/**<Errore durante la lettura */
  } EComStatus;
private:
  TCHAR m_cLog[MAX_PATH];
  int GetCommList(char* cType="VCP");
  /**Parametri di porta */
  char   m_CommList[256][64];
  int    m_iCommCount;
  DWORD  m_BaudRate;/**<Velocita' di trasmissione*/
  DWORD  m_Parity;/**< Tipo di parita'*/
  BYTE   m_ByteSize;/**< Numero di bit parola*/
  BYTE   m_StopBits;/**< Numero di bit di stop*/

  HANDLE m_hCom;/**< Handle porta seriale*/
  DCB    m_dcb;/**<Struttura di controllo dei parametri di porta seriale*/


  DWORD m_dwByteCount;/**< Numero caratteri massimo da leggere*/
  EComStatus m_iStatus;/**<Variabile di Status porta*/
 /**<Nome porta seriale */
  int m_iTOErr;/**<Numero errori timeout porta*/
  void ClearPort();

public:
  CPort()
  {
    m_iPortIndex=-1;
    bPortFinder=false;
    m_dwRxCount=0;
    m_iStatus=EComOff;
    memset(m_cRxBuffer,0,sizeof(m_cRxBuffer));
    memset(m_cTxBuffer,0,sizeof(m_cTxBuffer));
    m_iCommCount=GetCommList();
    m_bActiveLog=false;
    m_hCom=INVALID_HANDLE_VALUE;
  }
  int GetComCount()
  {
    return m_iCommCount;
  }
  TCHAR* GetPort()
  {
    return m_cPort;
  }

  int GetPortIndex()
  {
    return m_iPortIndex;
  }

  bool isAutoDetect()
  {
    return bPortFinder;
  }
  void ActiveLog(bool bActive)
  {
    m_bActiveLog=bActive;
  }

protected:
  void ClearRx()
  {
    PurgeComm (m_hCom,PURGE_RXCLEAR);
  }
  void ClearCom()
  {
    PurgeComm (m_hCom,PURGE_TXCLEAR);
    PurgeComm (m_hCom,PURGE_RXCLEAR);
  }
  bool  Connect(LPCTSTR sCom,
        int   iTotalTimeMsg=100,
        DWORD dwBaudRate=CBR_57600,
        DWORD dwParity=NOPARITY,
        BYTE  byByteSize=8,
        BYTE  byStopBits=ONESTOPBIT);

  bool Connect(int iComIndex,
               int iTotalTimeMsg=100,
               DWORD dwBaudRate=CBR_57600,
               DWORD dwParity=NOPARITY,
               BYTE  byByteSize=8,
               BYTE  byStopBits=ONESTOPBIT);

  void Disconnect();
  virtual void autoDetect(bool bDetect, char* cType="VCP");
  int m_iPortIndex;
  bool Tx(unsigned char cTxBuffer[],DWORD dwByteCount);
  void Rx(DWORD dwByteCount);
  unsigned char GetBuffer(int i)
  {
    return m_cRxBuffer[i];
  }

  unsigned char* GetBufferPos(int pos)
  {
    return m_cRxBuffer+pos;
  }

  bool  m_bActiveLog;
  DWORD m_dwRxCount;
  DWORD m_dwTxCount;
  unsigned char m_cRxBuffer[MAX_BUFFER_LEN];
  unsigned char m_cTxBuffer[MAX_BUFFER_LEN];
  bool bPortFinder;
  char m_cPort[10];

};


#endif