#include <time.h>
#include <fstream.h>
#include <stdio.h>
#include <vcl.h>
#include "Log.h"


extern bool Vis_T_N;
extern bool Vis_T_K;
extern bool Vis_Pmap;
extern bool Vis_cons;
extern bool Vis_consTot;

static char sDiagnostic[][25]={
                   "Info",               // 0
                   "Warning",             // 1
                   "Error: division by 0",// 2
                   "Error: NULL pointer", // 3
                   "Error: I/O",          // 4
                   "Error: critical",     // 5
                   };

bool LogError (char* cFileLog, int iLevel, const char* cText)
{
  time_t rawtime;
  struct tm * timeinfo;
  char d[11];
  char t[9];
  ofstream out;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  strftime (d,11,"%d/%m/%Y",timeinfo);
  strftime (t,9,"%H.%M.%S.%MS",timeinfo);

  // apre il file in modalità append
  ifstream inFile(cFileLog);
  if (inFile)
  {
    out.open(cFileLog,ios_base::app | ios::binary);
    inFile.close();
  }
  else
  {
     // alla prima creazione del file log
    out.open(cFileLog, ios_base::out | ios::binary);
    if(!out.is_open()) // se non è possibile aprire il file messaggio d'errore
      return false;
     out << "************************************************************** \r\n";
     out << "*  File: Log function error                                  * \r\n";
     out << "*  Contiene lo storico degli eventi ed errori verificatisi     \r\n";
     out << "*  durante il funzionamento del programma                      \r\n";
     out << "*  Inviare al servizio assistenza questo file nel caso in cui  \r\n";
     out << "*  si verifichino problematiche d'uso.                         \r\n";
     out << "*  Data e ora primo avvio: " << d << " " << t << "\r\n";
     out << "*                                                            *\r\n";
     out << "************************************************************** \r\n";
  }
  //  ********  Aggiornamento del file Log Error... ******************

  out << d << " " << t << " " << sDiagnostic[iLevel] << ": " << cText << "\r\n";

  out.close();

  return true;
}

/**@brief salvataggio dati diagnostici.
   @param filename nome del file diagnostico (.log viene appeso al file)
   @param title stringa in testa al vettore da stampare (in riga)
   @param y[] vettore dati double da stampare in unica riga
   @param size dimensione del vettore
   @param header se a true indica di creare la testata del file con data e ora.
   @return esito del salvataggio diagnostico.
*/
bool SaveAll  (char* filename, int iRpm1[],int iRpm2[],float dCel1[],float dCel2[], float dL1[],float dL2[], float dPresMap[],float dTOil[], float dTK[], int size)
{
  char strTmp[MAX_PATH]="";
  int i,j;
  time_t rawtime;
  struct tm * timeinfo;
  char d[12];
  char t[9];

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  strftime (d,12,"%Y-%m-%d_",timeinfo);
  strftime (t,9,"%H.%M.%S",timeinfo);

  ofstream datfile;
  strcat(strTmp, filename);
  strcat(strTmp,d);
  strcat(strTmp,t);
  strcat(strTmp, ".csv");

  datfile.open(strTmp,ios_base::app | ios::binary);
  if (!datfile)
    datfile.open(strTmp, ios_base::out | ios::binary);
    /// Testa del file
  if (datfile.is_open()==false) return false;
  if (datfile.is_open() && size > 0)
  {
    strftime (d,12,"%d/%m/%Y",timeinfo);
    strftime (t,9,"%H.%M.%S",timeinfo);
    datfile << endl;
    datfile << "--------------------------------------------------------\n";
    datfile << "Salvataggio dati banco - Data: " << d << " Ora: " << t << endl;
    datfile << "--------------------------------------------------------\n\n";
  }

  datfile << endl;
  if (iRpm1!=NULL && iRpm2!=NULL && dCel1!=NULL && dCel2!=NULL && dL1!=NULL && dL2!=NULL && dPresMap!=NULL && dTOil!=NULL && dTK!=NULL)
  {
    /// Intestazione colonne

    datfile << "Campione;RpmAnte;RpmPost;CelAnte[N·m];CelPost[N·m];L1;L2;PressMap;TempOlio;Termocoppia"  << endl;
    /// Colonne
    for (i=0;i<size;i++)
    {
      datfile << i << ";" << iRpm1[i] << ";" << iRpm2[i] << ";" << dCel1[i] << ";"
              << dCel2[i] << ";" << dL1[i] << ";" << dL2[i] << ";" << dPresMap[i]
              << ";"  << dTOil[i] << ";" << dTK[i] << endl;
    }
  }
  datfile.close();

  return true;
}

bool SavePerformance  ( ofstream* datfile,
                        int iRpm[],
                        float fFact,
                        float fWheelTorque[] ,
                        float fWheelPower[],
                        float fTermoOil[],
                        float fTermoK[],
                        float fPresMAP[],
                        float fCons[],
                        float fConsTot[],
                        int size)
{
  char strTmp[MAX_PATH]="";
  int i,j;
  String sHeader;
  if (!datfile)
    datfile->open(strTmp,ios_base::app | ios::binary | ios_base::out);
  //else
  //  datfile->open(strTmp, ios_base::out | ios::binary);

  if (datfile->is_open()==false) return false;
  *datfile << endl;
  if (iRpm!=NULL && fWheelTorque!=NULL && fWheelPower!=NULL)
  {
    /// Intestazione colonne
    sHeader="RPM;[Kg·m];[Cv];";
    if(Vis_T_N) sHeader+="[°C] Temp Oil;";
    if(Vis_T_K) sHeader+="[°C] Termocouple;";
    if(Vis_Pmap) sHeader+="[mbar] Pres Manifold;";
    if(Vis_cons) sHeader+="[g/KWh] Cons;[L] Total Cons;";
    *datfile << sHeader.c_str() << endl;
    /// Colonne
    for (i=0;i<size;i++)
    {
      if(iRpm[i])
        *datfile << iRpm[i] << ";"
                << FormatFloat("0.#",fWheelTorque[i]*fFact).c_str() << ";"
                << FormatFloat("0.#",fWheelPower[i]*fFact).c_str() << ";";
      if(Vis_T_N) *datfile << FormatFloat("0.#",fTermoOil[i]).c_str() << ";";
      if(Vis_T_K) *datfile << FormatFloat("0.#",fTermoK[i]).c_str() << ";";
      if(Vis_Pmap) *datfile << FormatFloat("0.#",fPresMAP[i]).c_str() << ";";
      if(Vis_cons) *datfile << FormatFloat("0.#",fCons[i]).c_str() << ";";
      if(Vis_cons) *datfile << FormatFloat("0.#",fConsTot[i]).c_str() << ";";
      *datfile << endl;
    }
  }
  datfile->close();

  return true;
}



bool SaveDiag (char* filename, char* title, double y[], int size, bool header)
{
  char strTmp[MAX_PATH]="";
  int i,j;
//  float yTmp[DIM_ACQ];
  time_t rawtime;
  struct tm * timeinfo;
  char d[11];
  char t[9];

  ofstream datfile;
  strcat(strTmp, filename);
  strcat(strTmp, ".log");
  datfile.open(strTmp,ios_base::app | ios::binary);
  if (!datfile)
    datfile.open(strTmp, ios_base::out | ios::binary);
    /// Testa del file
  if (datfile.is_open()==false) return false;

  if (header==true)
  {
    if (datfile.is_open() && size > 0)
    {
      time ( &rawtime );
      timeinfo = localtime ( &rawtime );
      strftime (d,11,"%d/%m/%Y",timeinfo);
      strftime (t,9,"%H.%M.%S",timeinfo);
      datfile << endl;
      datfile << "--------------------------------------------------------\n";
      datfile << "Salvataggio diagnostico - Data: " << d << " Ora: " << t << endl;
      datfile << "--------------------------------------------------------\n\n";
    }
  }
  datfile << endl;
  if (y!=NULL)
  {
    /// Intestazione colonne
    datfile << "Index" << "\t" << title << endl;
    /// Colonne
    for (i=0;i<size;i++)
    {
      sprintf (strTmp, "%-5d\t%-10.5f\n",i, y[i]);
      datfile << strTmp;
    }
  }
  else
    datfile << title << endl;

  datfile.close();

  return true;
}

/**@brief salvataggio dati diagnostici.
   @param filename nome del file diagnostico (.log viene appeso al file)
   @param title stringa in testa al vettore da stampare (in riga)
   @param y[] vettore dati double da stampare in unica riga
   @param size dimensione del vettore
   @param header se a true indica di creare la testata del file con data e ora.
   @return esito del salvataggio diagnostico.
*/
bool SaveDiag (char* filename, char* title, float y[], int size, bool header)
{
  char strTmp[MAX_PATH]="";
  int i,j;
//  float yTmp[DIM_ACQ];
  time_t rawtime;
  struct tm * timeinfo;
  char d[11];
  char t[9];

  ofstream datfile;
  strcat(strTmp, filename);
  strcat(strTmp, ".log");
  datfile.open(strTmp,ios_base::app | ios::binary);
  if (!datfile)
    datfile.open(strTmp, ios_base::out | ios::binary);
    /// Testa del file
  if (datfile.is_open()==false) return false;

  if (header==true)
  {
    if (datfile.is_open() && size > 0)
    {
      time ( &rawtime );
      timeinfo = localtime ( &rawtime );
      strftime (d,11,"%d/%m/%Y",timeinfo);
      strftime (t,9,"%H.%M.%S",timeinfo);
      datfile << endl;
      datfile << "--------------------------------------------------------\n";
      datfile << "Salvataggio diagnostico - Data: " << d << " Ora: " << t << endl;
      datfile << "--------------------------------------------------------\n\n";
    }
  }
  datfile << endl;
  if (y!=NULL)
  {
    /// Intestazione colonne
    datfile << "Index" << "\t" << title << endl;
    /// Colonne
    for (i=0;i<size;i++)
    {
      sprintf (strTmp, "%-5d\t%-10.5f\n",i, y[i]);
      datfile << strTmp;
    }
  }
  else
    datfile << title << endl;
    
  datfile.close();

  return true;
}

/**@brief salvataggio dati diagnostici.
   @param filename nome del file diagnostico (.log viene appeso al file)
   @param title stringa in testa al vettore da stampare (in riga)
   @param y[] vettore dati double da stampare in unica riga
   @param size dimensione del vettore
   @param header se a true indica di creare la testata del file con data e ora.
   @return esito del salvataggio diagnostico.
*/
bool SaveDiag (char* filename, char* title, int y[], int size, bool header)
{
  char strTmp[MAX_PATH]="";
  int i,j;
//  float yTmp[DIM_ACQ];
  time_t rawtime;
  struct tm * timeinfo;
  char d[11];
  char t[9];

  ofstream datfile;
  strcat(strTmp, filename);
  strcat(strTmp, ".log");
  datfile.open(strTmp,ios_base::app | ios::binary);
  if (!datfile)
    datfile.open(strTmp, ios_base::out | ios::binary);
    /// Testa del file
  if (datfile.is_open()==false) return false;

  if (header==true)
  {
    if (datfile.is_open() && size > 0)
    {
      time ( &rawtime );
      timeinfo = localtime ( &rawtime );
      strftime (d,11,"%d/%m/%Y",timeinfo);
      strftime (t,9,"%H.%M.%S",timeinfo);
      datfile << endl;
      datfile << "--------------------------------------------------------\n";
      datfile << "Salvataggio diagnostico - Data: " << d << " Ora: " << t << endl;
      datfile << "--------------------------------------------------------\n\n";
    }
  }
  datfile << endl;
  if (y!=NULL)
  {
    /// Intestazione colonne
    datfile << "Index" << "\t" << title << endl;
    /// Colonne
    for (i=0;i<size;i++)
    {
      sprintf (strTmp, "%-5d\t%-10d\n",i, y[i]);
      datfile << strTmp;
    }
  }
  else
    datfile << title << endl;
    
  datfile.close();

  return true;
}

/**@brief salvataggio dati diagnostici.
   @param filename nome del file diagnostico (.log viene appeso al file)
   @param title stringa in testa al vettore da stampare (in riga)
   @param y[] vettore dati double da stampare in unica riga
   @param size dimensione del vettore
   @param header se a true indica di creare la testata del file con data e ora.
   @return esito del salvataggio diagnostico.
*/
bool SaveDiag (char* filename, char* title, long y[], int size, bool header)
{
  char strTmp[MAX_PATH]="";
  int i,j;
//  float yTmp[DIM_ACQ];
  time_t rawtime;
  struct tm * timeinfo;
  char d[11];
  char t[9];

  ofstream datfile;
  strcat(strTmp, filename);
  strcat(strTmp, ".log");
  datfile.open(strTmp,ios_base::app | ios::binary);
  if (!datfile)
    datfile.open(strTmp, ios_base::out | ios::binary);
    /// Testa del file
  if (datfile.is_open()==false) return false;

  if (header==true)
  {
    if (datfile.is_open() && size > 0)
    {
      time ( &rawtime );
      timeinfo = localtime ( &rawtime );
      strftime (d,11,"%d/%m/%Y",timeinfo);
      strftime (t,9,"%H.%M.%S",timeinfo);
      datfile << endl;
      datfile << "--------------------------------------------------------\n";
      datfile << "Salvataggio diagnostico - Data: " << d << " Ora: " << t << endl;
      datfile << "--------------------------------------------------------\n\n";
    }
  }
  datfile << endl;
  if (y!=NULL)
  {
    /// Intestazione colonne
    datfile << "Index" << "\t" << title << endl;
    /// Colonne
    for (i=0;i<size;i++)
    {
      sprintf (strTmp, "%-5d\t%-10d\n",i, y[i]);
      datfile << strTmp;
    }
  }
  else
    datfile << title << endl;
    
  datfile.close();

  return true;
}
