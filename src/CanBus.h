//---------------------------------------------------------------------------

#ifndef CanBusH
#define CanBusH


#include "CanLib.h"

#define CON_TYPE_NONE       0
#define CON_TYPE_USB        1
#define CON_TYPE_REMOTE     2

typedef enum EStatus
{
	EDisc,/**<OBD non connessa>*/
	EOpen,/**<apertura della comunicazione>*/
	EConnOBD,/**<OBD connessa>*/
	EConnCAN,/**<CAN diretta connessa>*/
	ETerm,/**<OBD terminata*/
	EClrDTC,/**<Comando clear DTC*/
	EReadyOBD, /**<Comunicazione OBD attiva*/ 
	EReadyCAN, /**<Comunicazione CAN diretto attiva*/ 
	EChangeId, /**< azione di cambio scelta Id centralina rispondente*/
  EErrCAN
} ECanStatus;


bool openCan();
void closeCan();
bool connectCAN(long idReq,long time,int rateIndex);
bool txCan(long id,unsigned char data[]);

extern long lIdReqCan,lTimeRateCan, lTimestamp;
extern ECanStatus statusCan;

//---------------------------------------------------------------------------

#endif
