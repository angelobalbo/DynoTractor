//---------------------------------------------------------------------------


#pragma hdrstop
#include <time.h>
#include "CanLib.h"
#include "CanBus.h"

int hChannel=-1;
unsigned char conType;
canStatus gCanStatus;
bool bMonitorCANMode=false;
bool bExtendedId;

int iRateIndex;
long lBitRate;
long lTimestamp;
long lTimeRateCan;
long lIdReqCan;
unsigned int uiSeg1;
unsigned int uiSeg2;
unsigned int uiSjw;
unsigned int uiNoSamp;
unsigned int uiSyncMode;
unsigned long ulTimeout;

long lIdReq,lTime;

ECanStatus actionCan,statusCan=EDisc;


const struct CanParams
{
  long lBitRate;
  unsigned int uiSeg1;
  unsigned int uiSeg2;
  unsigned int uiSjw;
  unsigned int uiNoSamp;
  unsigned int uiSyncMode;
};

static const struct CanParams canPrms[3] =
  {
    {canBITRATE_500K,0,0,0,0,0},//41,14,2,1,0},
    {canBITRATE_250K,0,0,0,0,0},
    {canBITRATE_125K,0,0,0,0,0}
  };

canStatus getCanStatus()
{
   int st1, st2;
   gCanStatus=canGetBusParams(hChannel,&lBitRate,&uiSeg1,&uiSeg2,&uiSjw,&uiNoSamp,&uiSyncMode);
   gCanStatus = canGetDriverMode(hChannel, &st1, &st2);
   return gCanStatus;
}

bool openCan() // OpenBus
{
	DWORD tmp;
	unsigned long ulChannelData;

  int ver;

  canInitializeLibrary();

  ver = canGetVersion();
	if(hChannel>=0)
	  closeCan();

  gCanStatus=/*canStatus::*/canERR_NOCHANNELS;
  if(canGetChannelData(0,canCHANNELDATA_CHANNEL_CAP,&tmp,sizeof(tmp))==canOK)
	{

	  ulChannelData=(unsigned long)tmp;
	  if(((ulChannelData&canCHANNEL_CAP_VIRTUAL)==0) &&
	     ((ulChannelData&canCHANNEL_CAP_SIMULATED)==0))
	  {
      if(canGetChannelData(0,canCHANNELDATA_CARD_TYPE,&tmp,sizeof(tmp))==canOK)
      {
          ulChannelData=(unsigned long)tmp;

          if(ulChannelData==canHWTYPE_LEAF)
          {
            conType=CON_TYPE_USB;

//              CLogger::Log(_T("Open Kvaser Leaf Family tool"));
          }
          else if(ulChannelData!=canHWTYPE_BLACKBIRD_V2)
            conType=CON_TYPE_NONE;
          else
          {

//              CLogger::Log(_T("Open Kvaser BlackBird v2 tool"));
          }

          if(conType!=CON_TYPE_NONE)
          {
            hChannel = canOpenChannel(0, canOPEN_ACCEPT_VIRTUAL);
	          if(hChannel>=0)
	          {
	            gCanStatus=getCanStatus();
	            if(gCanStatus==canOK)
              {
	              gCanStatus=canSetNotify(hChannel, NULL, canNOTIFY_NONE);
//                     CLogger::Log(_T("canSetNotify(m_hChannel, NULL, canNOTIFY_TX/RX)== canOK"));
              }
	          }
          }
          else
            gCanStatus=canERR_NOTFOUND;
      }
	  }
	}
	if(gCanStatus<canOK)
	{
	  closeCan();
	  hChannel=-1;
	}

	if(gCanStatus!=canOK)
  {
	  canUnloadLibrary();
    statusCan=EDisc;
  }

//	m_status=(gCanStatus==canOK)?EOpen:EDisc;
	return (gCanStatus==canOK);
}

void closeCan()
{
	bMonitorCANMode=false;;

  canBusOff(hChannel);
  canClose(hChannel);
  canUnloadLibrary();
  hChannel=-1;
  statusCan=EDisc;
  conType=CON_TYPE_NONE;
  bExtendedId=true;
}

bool setCanBitRate(int iRateIndex)
{

  gCanStatus=canERR_NOMSG;
  if(iRateIndex>=0 && iRateIndex<3)
  {
      gCanStatus=canBusOff(hChannel);

      gCanStatus=canSetBusParams( hChannel,
                            canPrms[iRateIndex].lBitRate,
                            canPrms[iRateIndex].uiSeg1,
                            canPrms[iRateIndex].uiSeg2,
                            canPrms[iRateIndex].uiSjw,
                            canPrms[iRateIndex].uiNoSamp,
                            canPrms[iRateIndex].uiSyncMode );
      if(gCanStatus==canOK)
        gCanStatus=canBusOn(hChannel);
  }
  if(gCanStatus==canOK)
    statusCan=EConnCAN;
  return(gCanStatus==canOK);
}

bool txCan(long id,unsigned char data[])
{
  static char sLogStr[50];
  gCanStatus = getCanStatus();
  if(hChannel>=0)
  {
    lTimestamp=clock();
    
    gCanStatus=canWriteWait(hChannel,id,data,8,canMSG_EXT,80);
    if (gCanStatus != 0)
    {
/*        if (m_bActiveLog)
        {
            CLogger::Log(_T(" Failure on Write to CAN bus"));
        }*/
      statusCan=EErrCAN;
    }
    else
      statusCan=EReadyCAN;
    return (gCanStatus==0);
  }
  return false;
}

bool connectCAN(long idReq,long time,int rateIndex)
{
  bool res;
  iRateIndex=rateIndex;
  lIdReqCan=idReq;
  lTimeRateCan=time;
  res=setCanBitRate(rateIndex);
  actionCan=EConnCAN;
  return res;
}


//---------------------------------------------------------------------------

#pragma package(smart_init)
