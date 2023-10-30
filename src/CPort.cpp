//#include <vcl.h>
#include <stdio.h>
//#include <stdlib.h>

#include <windows.h> <WINDOWS.H>

#include "CPort.h"
#ifndef CPortH
#define CPortH

void CPort::ClearPort ()
{
  m_dwByteCount = MAX_BUFFER_LEN;
  ReadFile (m_hCom, m_cRxBuffer,m_dwByteCount,&m_dwByteCount,NULL);
  PurgeComm (m_hCom,PURGE_TXCLEAR);
  PurgeComm (m_hCom,PURGE_RXCLEAR);
}

void CPort::autoDetect(bool bDetect, char* cType)
{
  m_iCommCount=GetCommList(cType);
  m_iPortIndex=-1;
  bPortFinder=bDetect;
}

bool CPort::Connect(int iPortIndex,
                    int iTotalTimeMsg,
                    DWORD dwBaudRate,
                    DWORD dwParity,
                    BYTE byByteSize,
                    BYTE byStopBits)
{
  m_iPortIndex=iPortIndex;
  if(iPortIndex<m_iCommCount)
    return Connect(m_CommList[iPortIndex],
                   iTotalTimeMsg,
                   dwBaudRate,
                   dwParity,
                   byByteSize,
                   byStopBits);
  return false;
}

bool CPort::Connect(LPCTSTR sCom,
        int   iTotalTimeMsg, 
        DWORD dwBaudRate,
        DWORD dwParity,
        BYTE  byByteSize,
        BYTE  byStopBits)

{
  TCHAR sComStr[20]="\\\\.\\";
  if(m_iStatus!=EComOff)
  {
    if(m_hCom !=  INVALID_HANDLE_VALUE)
    {
      CloseHandle(m_hCom);
      m_hCom=INVALID_HANDLE_VALUE;

    }
    m_iStatus=EComOff;
  }
  

    strcat(sComStr,sCom);
    strcpy(m_cPort,sCom);    
    m_hCom = CreateFile (sComStr,
                         GENERIC_READ | GENERIC_WRITE,
                         0,
                         NULL,
                         OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL,
                         NULL);
    if (m_hCom !=  INVALID_HANDLE_VALUE)
    {
      ZeroMemory (&m_dcb, sizeof (m_dcb));
      m_dcb.DCBlength = sizeof(DCB);
      m_dcb.BaudRate = dwBaudRate;
      m_dcb.ByteSize = byByteSize;
      m_dcb.Parity = dwParity;
      m_dcb.StopBits = byStopBits;
      m_dcb.fParity = true;
      m_dcb.fBinary =  true;
      m_dcb.fNull = 0;
      m_dcb.fDtrControl=DTR_CONTROL_ENABLE;
      m_dcb.fRtsControl=RTS_CONTROL_ENABLE;
      if (!SetCommState (m_hCom, &m_dcb))
      {
        m_iStatus=EComOff;
        if(m_hCom!=INVALID_HANDLE_VALUE)
        {

          CloseHandle (m_hCom);
          m_hCom=INVALID_HANDLE_VALUE;
        }
        return false;
      }
      m_BaudRate=dwBaudRate;
      m_Parity=dwParity;
      m_ByteSize=byByteSize;
      m_StopBits=byStopBits;

      COMMTIMEOUTS *pto = new COMMTIMEOUTS;
      /** Massimo ritardo consentito fra un byte e il successivo
          durante la lettura di un pacchetto di dati*/
      if (pto)
      {
        pto->ReadIntervalTimeout = 35;   // Timeout tra 2 caratteri stesso messaggio
        pto->ReadTotalTimeoutMultiplier = 0;
        pto->ReadTotalTimeoutConstant = iTotalTimeMsg; // timeout per l'invio di un messaggio
        pto->WriteTotalTimeoutConstant=iTotalTimeMsg;
        SetCommTimeouts (m_hCom,pto);
        delete pto;
      }

      PurgeComm (m_hCom,PURGE_TXCLEAR);
      PurgeComm (m_hCom,PURGE_RXCLEAR);
      PurgeComm (m_hCom,PURGE_TXABORT);
      PurgeComm (m_hCom,PURGE_RXABORT);
      ClearCommBreak (m_hCom);
      SetupComm (m_hCom,MAX_BUFFER_LEN,MAX_BUFFER_LEN);
      m_iStatus = EComOk;
      return true;
    }
    m_iStatus=EComOff;
    return false;
}

void CPort::Disconnect()
{
  if (m_iStatus!=EComOff)
  {
    ClearPort();
    if(m_hCom!=INVALID_HANDLE_VALUE)
    {
       CloseHandle (m_hCom);
      m_hCom=INVALID_HANDLE_VALUE;
    }
    m_iStatus=EComOff;
  }
}

void CPort::Rx (DWORD dwByteCount)
{
  m_dwRxCount=0;
  if (m_iStatus!=EComOff)
  {
    if(ReadFile (m_hCom, m_cRxBuffer,dwByteCount,&m_dwRxCount,NULL))
    {
      m_cRxBuffer[m_dwRxCount]=0;
      m_iStatus=EComOk;
    }
    else
      m_iStatus=EComRxErr;
  }

}

bool CPort::Tx(unsigned char m_cTxBuffer[],DWORD dwByteCount)
{
  if (m_iStatus!=EComOff)
  {
    //PurgeComm (m_hCom,PURGE_TXCLEAR);
    PurgeComm (m_hCom,PURGE_RXCLEAR);
    if(WriteFile (m_hCom,m_cTxBuffer,dwByteCount,&m_dwTxCount,NULL))
    {
      if(dwByteCount==m_dwTxCount)
      {
        m_iStatus=EComOk;
        return true;
      }
    }
    m_iStatus=EComTxErr;
  }

  return false;
}

int CPort::GetCommList(char* cType)
{
  DWORD resVal;
  HKEY hSerialCommKey = NULL;
  TCHAR lpSubKey[100]="HARDWARE\\DEVICEMAP\\SERIALCOMM";
  DWORD i, status;
  DWORD keyType, nameBufSize, dataBufSize;
  UCHAR valueKeyName[64], valueKeyData[64];
  if(cType==NULL)
    strcpy(cType,"VCP");
  memset(m_CommList,0,sizeof(m_CommList));
  //
  // First - Get access to the registry key we need
  //
  resVal = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                        lpSubKey,
                        0, 
                        KEY_READ,
                        &hSerialCommKey);
  // If successful, read all the registry entries under this key
  int j=0;
  if( resVal == ERROR_SUCCESS )
  {
    for( i = 0;;i++ )
    {
      nameBufSize = dataBufSize = 64;
      memset(valueKeyName,0,sizeof(valueKeyName));
      memset(valueKeyData,0,sizeof(valueKeyData));      
      status = RegEnumValue(hSerialCommKey,
                            i,
                            (CHAR *)valueKeyName,
                            &nameBufSize,
                            NULL,
                            &keyType,
                            (UCHAR *)valueKeyData,
                            &dataBufSize
                    );
      //
      // If we got something, the value of the key is the COM name
      // otherwise we've come to the end of the list.
      //
      if (status == ERROR_SUCCESS)
      {
        if(strstr((char*)valueKeyName,cType)!=NULL)
        {
          strcpy(m_CommList[j], (const char *)valueKeyData);
          j++;
        }
      }
      else
        break;
    }
    //
    // Done, close down the registry before returning
    //
    RegCloseKey(hSerialCommKey);
    //
    // And return the number we found
    //
    return j;
  }
  else
    return -1; // Return -1 as an error if we can't access the registry
}

#endif

