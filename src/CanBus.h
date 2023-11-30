//---------------------------------------------------------------------------

#ifndef CanBusH
#define CanBusH


#include "CanLib.h"

#define CON_TYPE_NONE       0
#define CON_TYPE_USB        1
#define CON_TYPE_REMOTE     2

typedef enum EStatus
{
  ENoCmd,
	EDisc,/**<CAN non connessa>*/
	EOpen,/**<apertura della comunicazione>*/
	EConnCAN,/**<CAN diretta connessa>*/
	ETerm,/**<CAN terminata*/
	EStartTrasm, /**<Comunicazione CAN diretto attiva*/
  EReadyCAN, /** Posso trasmettere con continuità */
  EStopTrasm,
  EErrCAN
} ECanStatus;


extern char g_cCanLog[MAX_PATH];

extern int hChannel;
#if 0
extern int  bufIdx1,
            bufIdx2,
            bufIdxRpmTrac; // Indice che identifica il buffer invio automatico messaggi
extern unsigned char msg1[8]; // Messaggio da inviare al CAN
extern unsigned char msg2[8]; // Messaggio da inviare al CAN
extern unsigned char msgRpmTrac[8]; // Messaggio da inviare al CAN
#endif

bool openCan();
void closeCan();
bool connectCAN(long,long,long,long,int);
bool txCan(long id,unsigned char data[]);
bool CreateCan(TCHAR* cLog);
void DestroyCan();
extern long lIdReqCan,lTimeRateCan, lTimestamp;
extern ECanStatus statusCan;

//---------------------------------------------------------------------------

#endif
