
#ifndef LogH
#define LogH
#include "fstream.h"
bool LogError (char* cFileLog,int iLevel, const char* cText);

bool SaveDiag (char* filename, char* title, double y[], int size, bool header=false);
bool SaveDiag (char* filename, char* title, float  y[], int size, bool header=false);
bool SaveDiag (char* filename, char* title, int    y[], int size, bool header=false);
bool SaveDiag (char* filename, char* title, long   y[], int size, bool header=false);
bool SaveAll  (char* filename, int iRpm1[],int iRpm2[],float dCel1[],float dCel2[], float dL1[],float dL2[], float dPresMap[],float dTOil[], float dTK[], int size);
bool SavePerformance  (ofstream* datfile,
                        int iRpm[],
                        float fFact,
                        float fWheelTorque[],
                        float fWheelPower[],
                        float fTermoOil[],
                        float fTermoK[],
                        float fPresMAP[],
                        float fCons[],
                        float fConsTot[],
                        int size);

/** Livelli di log */
#define LOG_INFO      0
#define LOG_WARNING   1
#define LOG_ERR_DIV0  2
#define LOG_ERR_NULLP 3
#define LOG_ERR_IO    4
#define LOG_ERR       5

#endif