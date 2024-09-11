//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include <vcl.h>
#include <vector.h>
#pragma hdrstop
#include<fstream.h>
#include<iostream.h>
#include <IniFiles.hpp>
#include <dir.h>                           
#include <Registry.hpp>
#include <stdconvs.hpp>
#include <dstring.h>
#include <shfolder.h>
#include <dirent.h>
#include <time.h>
#include <windows.h>
#include "variabili.h"
#include "math.h"
#include "TMain.h"
#include "TGiri_Mot.h"
#include "TMap.h"
#include "TCoppia.h"
#include "TPotenza.h"
#include "TTermo.h"
#include "TImpostazioni.h"
#include "TInfo.h"
#include "TStrumenti.h"
#include "TMisure.h"
#include "TUtenti.h"
#include "TRapporto.h"
#include "TStart.h"
#include "TAcc.h"
#include "TOpzioni_grafici.h"
#include "TSetup.h"
#include "TStep_test.h"
#include "TPilot_Freno.h"
#include "TCarica_File.h"
#include "TConnessione.h"
#include "TSplash.h"
#include "TNote.h"
#include "TMt_cost.h"
#include "TOil_Temp.h"
#include "CUtils.h"
#include "CPort.h"
#include "CDyra.h"
#include "CDyn3.h"
#include "log.h"
#include "TFattCorr.h"
#include "autoSave.h"
#include "TFileList.h"
#include "TDebugMode.h"
#ifdef CAN_BUS
#include "CanBus.h"
#include "TCan.h"
#endif
#pragma package(smart_init)
#pragma link "AdPort"
#pragma link "OoMisc"
#pragma link "VrControls"
#pragma link "TeeTools"
#pragma link "TeeEdit"
#pragma link "VrDigit"
#pragma link "VrLabel"
#pragma link "VrRocker"
#pragma link "VrBlinkLed"
#pragma link "TeeSelectorTool"
#pragma link "TeeSeriesBandTool"
#pragma link "TeeSeriesTextEd"
#pragma link "TeeURL"
#pragma link "ErrorBar"
#pragma link "MyPoint"
#pragma link "CurvFitt"
#pragma link "StatChar"
#pragma link "TeeDragPoint"
#pragma link "NxCollection"
#pragma link "NxCustomGrid"
#pragma link "NxCustomGridControl"
#pragma link "NxGrid"
#pragma link "NxScrollControl"
#pragma link "NxColumnClasses"
#pragma link "NxColumns"
#pragma link "NxGridPrint"
#pragma link "VrControls"
#pragma link "CGAUGES"
#pragma link "TeeGanttTool"
#pragma link "VrScrollText"
#pragma link "NxToolBox"
#pragma link "NxLinkMenu"
#pragma link "NxEdit"
#pragma link "VrBorder"
#pragma link "ZIPLib_OCX"


#pragma resource "*.dfm"

#define K_WW 6      // costante di divisione per calcolo misura font
#define K_Font 62  //  massima size del font

#define Acc_ref 0.2//(0.25 - 5) per trattore 50 per kart valore di accelerazione per inizio test a step
#define KTempo 15//15 per trattore 100 per kart  finestra di tolleranza +/- applicata ai giri di riferimento da stazionare


#define K_ECER24 1.02   // K per corr. normativa
#define K_ISO9768 1.07  // K per corr. normativa
#define K_SAEJ 1.05     // K per corr. normativa
#define MEDIA_CONSUMI 5 //sec di calcolo media consumi
#define TIME_MED_CONS 100

#define TIMER_STOP_PWM 10
#define IMPULSI_LITRO 800
#define MAX_PERC_CONS 10
#define CONSUMO_G_KWH(l_h,p) ((p>0)?(l_h*1.36*860/p):0)
#define _CONSUMO ((Unit_Cons==3)?fConsumo:(litri_h*fK_convCons))
#define CONSUMO(l_h,l) ((Unit_Cons==3)?l:(l_h*fK_convCons))

// Calcolo pressione da sonde
/// Nuova tipologia di sonda (2016)
#define VRATE (2.5/2.56)
#define CALC_P_MAP1(x) ((float)x*VRATE/255*5<0.2)?0:((float)x*VRATE/255-0.04)/0.0012525*10
/// Vecchia tipologia di sonda
#define CALC_P_MAP(x) (((x*0.0098*2)+0.0433)/1.2452)*1000-K_R

#define CALC_P_ATM(x) (((((((x*0.01953)+0.067)/5)+0.095)/0.009)*10)-20)+iOffset_temp;

// Modifiche per Industry 4.0 equivalenti a modifiche Dynotruck 2.4.6.3 (Dynorace Turchia+assistenza)
#define _INDUSTRY4


// MACRO
// discriminazione tra consumi in g/kWh e l/h
  TMain *Main;


#ifdef _CAN_BUS
  canStatus status;

#endif

  unsigned short cellValue=0,cellValueTot;
  bool cellError=false;

  char swNbr[20];
  unsigned char cDynVersion=0x1;
  unsigned char ucDyraFwVer=0x0;
  String Unit_pw,Unit_M,Unit_V,sUnitCons;
  bool bOnInit=true;
  bool  bOnClose=false;
  char g_cBuffer[MAX_PATH];
  char g_cFileLog[MAX_PATH];
  char g_cFileDiag[MAX_PATH];
  char cDiagName[MAX_PATH];
//  unsigned int rowTabStepF1=1,rowTabStepF2=1,rowTabStepF4 = 1;


  struct UM_LIMIT limit;
  //unsigned int primaVoltaTabStep = 1;
  unsigned int iImpulsiLitro=800;
  bool bSavedTest=true;
  bool bNotifyDyn3=false;
  bool bNotifyDyra=false;
  bool bNotifyRTChart=false;
  bool bFilePerf=false;
  bool bPrimaEsec=true;
  bool bPidControl;
  bool bMapType=false;
  unsigned short sPidKp; // 0..FFFF x 10
  unsigned short sPidKi; // 0..FFFF
  unsigned short sPidKd; // 0..FFFF x 10000
  float fAccMax;
  int iDyraError=0; /// Numero pacchetti errati dyra
  int iMaxStepDistance=50;
  bool bFileTrace=false;
  unsigned char rpm_2[28],CKS2,startDebug = 0,stopDebug = 0 ;

  int num2=1,Pos_vector=0,num_cicli_acq=1,cntrIst = 0,Pos_consumi = 0;
  float N_val_Acq=1;

  double dDateTest;

  short int  bot5,tot5;
  long bot6,tot6,
       bot7,tot7,
       bot8,tot8,
       bot9,tot9;

  long p_term = 0, i_term = 0;
  short Ki_prop=3000,
        Ki_integ=350;//3500

  float C_Time=0.5,Facc=0,Fcel=0,velKm = 0;

  unsigned char buffer2[500],start_debug = 0;
  unsigned int selPto = 0;
    double Accel=0;
   unsigned char flagConsumi = 0,consumiGiaAttivo = 0;
   float   /*consumi = 0,*/litri_h = 0,litri_tot=0,fConsumoGKwh,fConsumoMedioGKwh;
   unsigned char eff = 0;

   float fConsumo,fConsumoSomma;
   unsigned short usSommaImpulsiCons=0;
   unsigned int tickMediaCons = 0;

   CDyn3::EDyn3Status dyn3Status=CDyn3::EDyn3NotDefined;
   CDyra::EDyraStatus dyraStatus=CDyra::EDyraNotDefined;
   union CDyn3::EDyn3ErrorStatus dyn3ErrorStatus;
   union BenchState brakeStatus;
   extern unsigned char connDypt,connDyra ;
   unsigned int okProcessData = 0;
   unsigned short uiImpulsiCons=0,uiPrecImpulsiCons=0;
   extern bool gbStepTestCancel=false;
   extern int Reg_Stp_1;
   extern bool Debugs_Status;
   unsigned char statusRelaisConfig = 0;
   HANDLE hEvDataReady;

   char g_cDyraLog[MAX_PATH];
   char g_cDyn3Log[MAX_PATH];

   char cDocPath[MAX_PATH];

  String txtLiftMsg;
  CDyra::DataIn dyraData;
  float fDelayStep=3,fStepCounter=0;
  extern TDateTime dataFile;
  int    vTestEseguiti[8];//Indici dei test effettivamente eseguiti e non caricati da file
  int    iTestEseguiti=0; // Numero dei test effettivamente eseguiti

  bool edPCh=false;
  bool edICh=false;
  bool edDCh=false;

  CDyn3::EDyn3DebugInfo g_debugInfo;

void TMain::addSerialDat(CDyra::DataIn1* row)
{
  if(row && g_dynoTest.testPrms.nPunti<MAX_SERIAL && statusDat==SAMPLING_DAT)
    memcpy(&(g_dynoTest.serialDat[(int)g_dynoTest.testPrms.nPunti++]),row,sizeof(g_dynoTest.serialDat[0]));
  else
  {
    if(g_dynoTest.testPrms.nPunti>=MAX_SERIAL)
       statusDat=SAVE_DAT;
  }

}

void TMain::getSerialDat(CDyra::DataIn1* row)
{
  if(row && iDatIndex<g_dynoTest.testPrms.nPunti && (statusDat==RUN_SIMUL_DAT || statusDat==START_SIMUL_DAT))
    memcpy(row,&(g_dynoTest.serialDat[iDatIndex++]),sizeof(g_dynoTest.serialDat[0]));
  else
  {
    if(iDatIndex>=g_dynoTest.testPrms.nPunti)
      statusDat=NO_DAT;
    //bDatLoad=false;
  }
}

void SetRealTimeData()
{
  static double dEncoder;
  static unsigned short primaVoltaFlagConsumi=1,usEncoder;
  static int iCountRpmAnteSpike=0;
  static char sLog[100];

  switch(Main->getStatusDat())
  {
    case SAMPLING_DAT:
      Main->addSerialDat(&dyraData);
    break;
    case RUN_SIMUL_DAT:
    case START_SIMUL_DAT:
      Main->getSerialDat(&dyraData);
    break;
    default:
    break;
  }
  /// Calcolo RPM da encoder

  dEncoder=dyraData.tractorData.encoderL+(dyraData.tractorData.encoderH<<8);

  if(dEncoder) // Nuovo valore !=0
  {

    if(lRpm && lRpmPrec) // Ultimi due valori !=0
    {
      // Rileva Spike
      if((abs(MIN2USEC/(dEncoder*dynoPrm.bench.nImpulsiEncoder)-lRpm)>30) && iCountRpmAnteSpike<3)
      {
        // In caso di spike imposta l'andamento tra gli ultimi 2 campioni
        lRpm+=lRpm-lRpmPrec;
        iCountRpmAnteSpike++;
      }
      else
      {
       lRpm=MIN2USEC/(dEncoder*dynoPrm.bench.nImpulsiEncoder);
       iCountRpmAnteSpike=0;
      }
    }
    else
      lRpm=MIN2USEC/(dEncoder*dynoPrm.bench.nImpulsiEncoder);
    // Filtro passa-basso
    if(lRpmPrec>0)
      lRpm=lRpmPrec+(lRpm-lRpmPrec)/3; // Filtraggio
  }
  else
    lRpm=0;

  lRpmPrec=lRpm;

  /// Velocità
  velKm=float(lRpm)*dynoPrm.bench.diamRullo*RPM2KMH;

  /// Temperatura esterna
  if(dyraData.tractorData.TExt)
  {
    if(dyraData.tractorData.TExt<=119)
      TAir0=(1/((0.000232564*log((22*dyraData.tractorData.TExt*0.0098*2/(4.4-(dyraData.tractorData.TExt*0.0098*2)))/22))+0.0033557))-273.15;
    else if(dyraData.tractorData.TExt>119 && dyraData.tractorData.TExt<220)
      TAir0=(1/((0.000256481*log((22*dyraData.tractorData.TExt*0.0098*2/(4.4-(dyraData.tractorData.TExt*0.0098*2)))/22))+0.0033557))-273.15;
    if(TAir0>-15 && dynoPrm.filt.KTempAtm)
      TAir=TAir+((TAir0-TAir)/dynoPrm.filt.KTempAtm);
    else           
      TAir=0;
  }
  else
    TAir=0;

  /// Temperatura Olio
  TOil0=(dyraData.tractorData.TOil*0.0098)/(4.2-(dyraData.tractorData.TOil*0.0098))* 1800; // Resistenza della sonda
  TOil0= (-3.9+pow((15.21-(4*-0.0005775*(1000-TOil0))),0.5))/(2*(-0.0005775));
  TOil=TOil+((TOil0-TOil)/dynoPrm.filt.KTempOil);


    if(bMapType) // Nuova sonda, introdotta in versione 3.1.1 del dynorace
      Pmap0=CALC_P_MAP1(dyraData.tractorData.PMap);
    else
      Pmap0=CALC_P_MAP(dyraData.tractorData.PMap);

  //Pmap0=(((dyraData.tractorData.PMap*0.0098*2)+0.0433)/1.2452)*1000-K_R; // new 3 bar rel. max
  Pmap=Pmap+((Pmap0-Pmap)/dynoPrm.filt.KMap);

  
  
  
  // pressione atmosferica    -20 -> OFFSET FISSO PER ALLINEAMENTO VALORE INDICATO
  Patmo =(((((((dyraData.tractorData.PExt*0.01953)+0.067)/5)+0.095)/0.009)*10)-20)+iOffset_temp;
  Val_pat=Val_pat +((Patmo-Val_pat)/dynoPrm.filt.KPressAtm);

  V_T=(dyraData.tractorData.TkL+(dyraData.tractorData.TkH<<8))*0.25-dynoPrm.bench.KTermocoppia;

  // ACCELERAZIONE MOTORE
  if(cicli_acc==dynoPrm.thres.nSamplesCalcAccel)
  {
    Cont_letture_acc++;
    Accel =((lRpm)-Old_vel) /(dynoPrm.thres.nSamplesCalcAccel*dynoPrm.bench.nImpulsiEncoder*Cont_letture_acc);
    if(Accel!=0)
      Cont_letture_acc=0;
    Old_vel=lRpm;
    /// Filtraggio
    Val_ACC+=((Accel-Val_ACC)/dynoPrm.filt.KAccel);
    cicli_acc=1;
  }
  else
    cicli_acc++;
  if(!flagConsumi)
  {
    primaVoltaFlagConsumi=1;
    uiPrecImpulsiCons=0;
    uiImpulsiCons=0;
    fConsumoGKwh=0;
    litri_h=0;
  }
  else
  {
    uiImpulsiCons=dyraData.tractorData.consL+(dyraData.tractorData.consH<<8);
    // Calcolo consumo totale in litri dall'inizio o dal reset consumi
    fConsumo=(float)(uiImpulsiCons)/(float)iImpulsiLitro;
    // Calcolo consumo istantaneo (ogni 0.25s)
    // Densità gasolio 825g/l
    if(primaVoltaFlagConsumi)
    {
      usSommaImpulsiCons = 0;
      tickMediaCons=0;
      fConsumoSomma=0;
      fConsumoGKwh=0;
      uiPrecImpulsiCons=uiImpulsiCons;
      //uiImpulsiCons=0;
      litri_h=0;
      fConsumo=0;
    }
    if((uiImpulsiCons-uiPrecImpulsiCons)<0)
      uiPrecImpulsiCons=0;
    usSommaImpulsiCons+= uiImpulsiCons - uiPrecImpulsiCons;
    // Integrale del consumo in Litri
    fConsumoSomma+=(float)(uiImpulsiCons - uiPrecImpulsiCons)/(float)iImpulsiLitro;
    uiPrecImpulsiCons=uiImpulsiCons;
    // Numero campioni da 0.025s
    tickMediaCons++;

    primaVoltaFlagConsumi = 0;
    if(usSommaImpulsiCons>=MAX_PERC_CONS)
    {
      litri_h=3600*fConsumoSomma/(tickMediaCons*dynoPrm.bench.timeSerial);
     if(Val_potenza_m1>0)
        fConsumoGKwh=860*litri_h/(Val_potenza_m1/1.36);
      else
        fConsumoGKwh=0;
      // litri consumati rispetto al precedente computo totale (oltre i 10 impulsi calcolo la quota litri relativa)
      // Rimango indipendente dal tempo, devo ottenere il numero di campioni su cui è calcolato?
      usSommaImpulsiCons=0;
      tickMediaCons=0;
      fConsumoSomma=0;
      primaVoltaFlagConsumi = 0;
    }
  }

  /// Cella di carico
  Vcel1=dynoPrm.bench.KmV2VCellaAnte*dynoPrm.bench.VAlimCellaAnte* float((unsigned short)(~(dyraData.tractorData.cellL+(dyraData.tractorData.cellH<<8))))/0xFFFF/1000;
  Val_cella_1+=dynoPrm.bench.caricoMaxCellaAnte*
              float((unsigned short)(~(dyraData.tractorData.cellL+(dyraData.tractorData.cellH<<8))))/0xFFFF*
              cellPrm.pesoCampioneA/cellPrm.deltaFase21A*cellPrm.braccioA-cellPrm.offsetA;
  cellValue+=(unsigned short)(~(dyraData.tractorData.cellL+(dyraData.tractorData.cellH<<8)));
  /// Calcolo media e coppia sulla cella di carico
  if(num_val_cel==dynoPrm.filt.nCampMediatiCella)
  {
    Val_cella_m1=Val_cella_1/dynoPrm.filt.nCampMediatiCella;
    Val_cella_1=0;
    cellValueTot=cellValue/dynoPrm.filt.nCampMediatiCella;
    /*
    if((cellValue==cellValueTot || cellValue>65000) && cellError==false)
    {
      if(Start_acq==true)
        Start->BitBtn2Click(NULL);
      else if (OK_TEST_STEP==true)
        Step_test->BitBtn3Click(NULL);
      Application->ProcessMessages();
      MessageBox(NULL,"Cell broken or Wrong value!","ERROR",MB_OK | MB_ICONERROR);
      LogError(g_cFileLog,LOG_ERR,"Errore di lettura cella (>65000 o bloccata)!");
      cellError=true;
    }*/
    cellValue=0;
    num_val_cel=1;
    if(Val_cella_m1<0) Val_cella_m1=0;
    // Coppia
    Val_coppia_m1=((Val_cella_m1/Rap_tot1*dynoPrm.bench.braccioReazCellaAnte)/Rend_PTO)/**fFact*/;
    Val_potenza_m1=(Val_coppia_m1 * (float)lRpm*Rap_tot1)/716.2;


  }   // fine calcola media celle di carico
  else
    num_val_cel++;

  // DATI REAL-TIME del test DECELERATIVO
  if(Tipo_test==TEST_DECEL)
  {
    if((Start_acq==true) && (lRpm*Rap_tot1<=Reg_iniz_dec))
    {
      /// Memorizza i dati durante la prova
      RPM_M[cntrIst]= lRpm*Rap_tot1;
      C_M[cntrIst]= Val_coppia_m1;
      vSerCons[cntrIst]=litri_h;
      vSerConsTot[cntrIst]=fConsumo;
      vSerTermoK[cntrIst]=V_T;
      vSerTermoOil[cntrIst]=TOil;
      vSerPresMap[cntrIst]=Pmap;
      cntrIst++;
    }
  }

  // GESTIONE TEST A STEP DETERMINATO
  if(OK_TEST_STEP && Tipo_test==TEST_STEP_TIMED)
  {
    //numero di acquisizioni dall'inizio dello step
    /// Nei test a step, acquisisci dati RPM e cella
    num_pass_step++;

    /// Questa fase serve a determinare l'inizio dell'acquisione all'interno dello step
    //se ho agganciato il regime
    if(OK_ACQ_TEST_STEP==false)
    {
      // test a step decelerativo, condizioni di decremento step
      // 1) Oscillazione eccessiva impone di saltare lo step...passo al successivo

      if((fabs(Val_ACC)<=Acc_ref) &&
         (lRpm*Rap_tot1>=Reg_Stp-KTempo) &&
         (lRpm*Rap_tot1<=Reg_Stp+KTempo))
      {
        fStepCounter=0;
        OK_ACQ_TEST_STEP=true;
      }
      else // Se sono nell'intorno di uno step per molto tempo, acquisisco nonostante tutto
      {
        //avvio comunque l'acquisizione dopo n secondi
        //anche se non si è stabilizzato
        if ((Contatore>0 && num_pass_step>=(num_sec_attesa/dynoPrm.bench.timeSerial)) ||
            (Contatore==0 && num_pass_step>=(num_sec_attesa*2/dynoPrm.bench.timeSerial)))
          // Indicatore inizio acquisione dati da mediare
          OK_ACQ_TEST_STEP=true;
      }
    }
    else /// Acquisizione e Terminazione Scalino TEST a STEP
    {
      if(fStepCounter>=fDelayStep)
      {
        // Condizione di Terminazione scalino TEST A STEP
        if(Tstep>int(Tempo_staz/dynoPrm.bench.timeSerial))
        {
          OK_ACQ_TEST_STEP=false;
          // CALCOLO MEDIA GRANDEZZE A GIRI COSTANTI
          if(Tstep==0)
            Tstep=1;
          Val_cop_medio/=Tstep;
          Val_reg_medio/=Tstep;
          Val_oil_medio/=Tstep;
          Val_map_medio/=Tstep;
          Val_k_medio  /=Tstep;
          Tstep=0;
          bNotifyRTChart=true;
          // *** acquisisco i valori medi ***
          // RPM
          RPM_M[Contatore]= Val_reg_medio;
          //   COPPIA
          C_M[Contatore]= Val_cop_medio;
          // consumo istantaneo in g/Kwh
          vettCons[Contatore]=litri_h;
          // consumo totale (litri)
          fvettConsTot[Contatore]=fConsumo;
          // TEMP OLIO
          fTermo_Oil[Contatore]= Val_oil_medio;
          // PRESSIONE MANIFOLD
          fPres_MAP[Contatore]= Val_map_medio;
          // TERMOCOPPIA
          fTermo_K[Contatore]= Val_k_medio;
          Contatore++;
          Val_cop_medio=0;
          Val_reg_medio=0;
          Val_oil_medio=0;
          Val_map_medio=0;
          Val_k_medio=0;
          // calcolo nuovo step
          // test a step accelerativo, incrementa step
          if(!gbStepTestCancel)
          {
            if(rpmStart<rpmStop && Reg_Stp<=rpmStop)
            {
              Reg_Stp += Delta_reg; // incrementa i giri
              usEncoder=MIN2USEC/((Reg_Stp/Rap_tot1)*dynoPrm.bench.nImpulsiEncoder);
              if(!(Main->getStatusDat()==START_SIMUL_DAT || Main->getStatusDat()==RUN_SIMUL_DAT)) g_brakeControl.CmdStep(usEncoder,CDyn3::EFrontAxle);
              if(bFileTrace)
              {
                sprintf(sLog,"Cmd Step RPM=%d",Reg_Stp);
                LogError(g_cDyn3Log,LOG_INFO,sLog);
              }
            }
            // test a step decelerativo, decrementa step
            if(rpmStart>rpmStop && Reg_Stp>=rpmStop)
            {
              Reg_Stp -= Delta_reg; // decrementa i giri
              if(Reg_Stp >= rpmStop)
              {
                usEncoder=MIN2USEC/((Reg_Stp/Rap_tot1)*dynoPrm.bench.nImpulsiEncoder);
                if(!(Main->getStatusDat()==START_SIMUL_DAT || Main->getStatusDat()==RUN_SIMUL_DAT)) g_brakeControl.CmdStep(usEncoder,CDyn3::EFrontAxle);
                if(bFileTrace)
                {
                  sprintf(sLog,"Cmd Step RPM=%d",Reg_Stp);
                  LogError(g_cDyn3Log,LOG_INFO,sLog);
                }
              }
            }
          }
          num_pass_step=0;
        }
        else // Condizione di Acquisizione scalino
        {
          // acquisisce i valori una volta agganciato il regime
          //if(Tstep<500)
          //{
            // coppia rilevata
            Val_cop_medio+=Val_coppia_m1;
            Val_reg_medio+=lRpm*Rap_tot1;
            Val_oil_medio+=TOil;
            Val_map_medio+=Pmap;
            Val_k_medio  +=V_T;
            Tstep++;
          //}
        }
      }
      else // Aggiorna counter ritardo rispetto alla reale acquisizone (pausa prima di acquisire)
      {
        fStepCounter+=dynoPrm.bench.timeSerial;
      }
    }
  }
}

void notifyDyra()
{
  static unsigned char ucCmd;
  ucCmd=g_dyraControl.GetCmd();
  dyraStatus=g_dyraControl.GetDyraStatus();
  if(ucCmd==CDyra::EDyraTxFwVerReq)
    ucDyraFwVer=g_dyraControl.GetFwVer();
  else if(ucCmd==CDyra::EDyraTxStatus || ucCmd==0)
    Connect_Dyra=g_dyraControl.isOn();

  if(bOnInit==false)
  {
    if(g_dyraControl.isOn() ||
       Main->getStatusDat()==RUN_SIMUL_DAT ||
       Main->getStatusDat()==START_SIMUL_DAT
        )
    {
      // Dyra Connessa
      if(g_dyraControl.isOn())
      {
        if(g_dyraControl.isDataReady())
          g_dyraControl.GetDyra(dyraData);
      }
      SetRealTimeData();
    }
  }
  bNotifyDyra=true;
}

TColor TMain::clTestColor[9]={clGray,clBlue,clRed,clLime,clMaroon,clAqua,clFuchsia,clOlive,clBlack};

void TMain::notifyDyra2Main()
{
  static bool bConnected=true;

  Application->ProcessMessages();
  if(bOnClose==true)
    return;
  dyraStatus=g_dyraControl.GetDyraStatus();

  if (Connect_Dyra==false)
  {
    Main->VrLabel8->Font->Color=clRed;
    Main->VrLabel8->Caption=Text80;
    Timer1->Enabled=false;
  }
  else
  {
    if(Main->VrLabel8->Font->Color==clRed)
    {
      Main->VrLabel8->Font->Color=clGreen;
      Main->VrLabel8->Caption=Text79;
      g_dyraControl.CmdStop();
      g_dyraControl.CmdRele(statusRelaisConfig);
      g_dyraControl.CmdStart();
    }
    Timer1->Enabled=true;
  }

  if(bOnInit==false)
  {
    if(g_dyraControl.isOn())
    {
      // Dyra Connessa
      Main->VrLabel8->Font->Color=clGreen;
      Main->VrLabel8->Caption=Text79;

      //if(dyraStatus==CDyra::EDyraReceiveData)
      //  SetRealTimeData();
      if(iDyraError!=g_dyraControl.GetErrorFrames())
      {
        if(g_dyraControl.GetErrorFrames()==0)
          iDyraError=0;
        else
        {
          sprintf(buffer2,"Ricevuto dato errato n° %d",g_dyraControl.GetErrorFrames());
          LogError(g_cFileLog,LOG_ERR,buffer2);
          iDyraError!=g_dyraControl.GetErrorFrames();
        }
      }
    }
    else
    {
      //Dyra Disconnessa
      Main->VrLabel8->Font->Color=clRed;
      Main->VrLabel8->Caption=Text80;
    }
  }
  if(bConnected!=Connect_Dyra && Connect_Dyra==false && bOnInit==false)
    PageControl1->ActivePage=TabSheet9;
  if(bOnInit==false)
    bConnected=Connect_Dyra;
  bNotifyDyra=false;
}


    __fastcall TMain::TMain(TComponent* Owner)
        : TForm(Owner)

{
  TIniFile *ini;

  ini = new TIniFile(
  utils.GetProgramDataName(".ini") );
  Profilo=ini->ReadString ("Profili", "Utente in uso","Default");


  memset(&g_dynoTest,0,sizeof(DAT_TYPE));
  //bDatEnabled=false;
  //bDatLoad=false;
  statusDat=NO_DAT;
  delete ini;

  bCanInUse=false;
  lCanIdMsg=100;
  iCanRate=100;

}

bool TMain::loadDat()
{

  int iLen;

  String sStrBuf;
  char sBuffer[100];
 
  OpenDialog1->InitialDir=(String)cArchivePath;
  OpenDialog1->Filter="dati seriali(*.dyno)|*.dyno;";

  if(OpenDialog1->Execute()) // Salvataggio del file sessione
  {

    LoadDyno(OpenDialog1->FileName.c_str());
    //memset(&g_dynoTest,0,sizeof (DAT_TYPE));
    memcpy(&dynoPrm,&g_dynoTest.dynoPrms,sizeof(dynoPrm));
    memcpy(&testPrm,&g_dynoTest.testPrms,sizeof(testPrm));
    strcpy(nomeTest,ExtractFileName(OpenDialog1->FileName).c_str());
    memcpy(Costruttore,&g_dynoTest.costruttore,sizeof(Costruttore));
    memcpy(Modello,&g_dynoTest.modello,sizeof(Modello));
    memcpy(Allestimento,&g_dynoTest.allestimento,sizeof(Allestimento));
    memcpy(Cliente,&g_dynoTest.cliente,sizeof(Cliente));
    memcpy(Targa,&g_dynoTest.targa,sizeof(Targa));

    LoadTestPrms(&g_dynoTest.testPrms);
    statusDat=F10_SIMUL_DAT;
    iDatIndex=0;
    return true;
  }
  return false;
}

void TMain::clearDat()
{
  memset(&g_dynoTest,0,sizeof(DAT_TYPE));
  if(statusDat==F10_SIMUL_DAT || statusDat==START_SIMUL_DAT || statusDat==RUN_SIMUL_DAT)
  {
    carica_imp();
    caricaBanco();
  }
  statusDat=NO_DAT;

}
//---------------------------------------------------------------------------
bool TMain::createDat()
{
  if(statusDat!=NO_DAT)
    return false;

  memcpy(g_dynoTest.nomeTest,nomeTest,sizeof(nomeTest));
  memcpy(g_dynoTest.costruttore,Costruttore,sizeof(Costruttore));
  memcpy(g_dynoTest.modello,Modello,sizeof(Modello));
  memcpy(g_dynoTest.allestimento,Allestimento,sizeof(Allestimento));
  memcpy(g_dynoTest.cliente,Cliente,sizeof(Cliente));
  memcpy(g_dynoTest.targa,Targa,sizeof(Targa));
  memset(&g_dynoTest.testPrms,0,sizeof(g_dynoTest.testPrms));
  memcpy(&g_dynoTest.dynoPrms,&dynoPrm,sizeof(g_dynoTest.dynoPrms));
  memset(g_dynoTest.serialDat,0,sizeof(g_dynoTest.serialDat));
  g_dynoTest.testPrms.nPunti=0;
  statusDat=SAMPLING_DAT;
  return true;
}

bool TMain::pressStartDat()
{
  if(statusDat==F10_SIMUL_DAT)
  {
    statusDat=START_SIMUL_DAT;
    lRpm=0;
    lRpmPrec=0;
    return true;
  }
  return false;
}

bool TMain::runDat()
{
  if(statusDat==START_SIMUL_DAT)
  {
    statusDat=RUN_SIMUL_DAT;
    return true;
  }
  return false;
}

bool TMain::stopAndSaveDat()
{
  bool bRes;
  if(!(getStatusDat()==SAMPLING_DAT || getStatusDat()==SAVE_DAT))
    return false;
  else
  {
    statusDat=NO_DAT;
    memcpy(&g_dynoTest.testPrms,&datPrm[sNum_test-1],sizeof(g_dynoTest.testPrms));
    bRes=SaveDyno();
    return bRes;
    //carica_imp();
    //caricaBanco();
  }

}

void TMain::caricaBanco()
{
  String nomefile;

  if(tipoBanco==2)//Trailer
    nomefile="File_impo_trailer.dmt";
  else
    nomefile="File_impo_trac.dmt";
  ifstream infile(utils.GetProgramDataPath(nomefile.c_str()).c_str(), ios::binary);  // carica impostazioni
  if(infile.is_open())
  {
    infile.read((unsigned char*)&dynoPrm,sizeof (dynoPrm));
    infile.close();
    okProcessData = 1;
#ifdef _DEBUG
  LogError(g_cFileLog,LOG_INFO,"Caricato il file_impo");
#endif
  }
  else
  {
    LogError(g_cFileLog,LOG_ERR,"Impossibilità di aprire file_imp");
    Timer1->Enabled=false;
    ShowMessage(Str7);
  }
}
void __fastcall TMain::carica_imp()
{
  int vers;
  char cDocPath[MAX_PATH];
  // apertura del file .ini e acquisizione impostazioni salvate o predefinite
  TIniFile *ini;
  ini = new TIniFile(utils.GetProgramDataName(".ini"));
  /// Caricamento conteggio ore lavoro e lanci
  bMapType=ini->ReadBool("Profili", "Tipo Manifold",0);;
  if(bMapType==false)
  {
    Strumenti->rbPressType0->Checked=true;
    Strumenti->UpDown13->Max=3000;
    //Map->AnalogMeterHQ1->MaximumValue=3000;

  }
  else
  {
    Strumenti->rbPressType1->Checked=true;
    Strumenti->UpDown13->Max=6000;
    //Map->AnalogMeterHQ1->MaximumValue=6000;
  }

  dataResetCountMinPar=ini->ReadFloat("Counters","dataResetCountMinPar",Now().operator double());
  dataResetLanci=ini->ReadFloat("Counters","dataResetLanci",Now().operator double());
  nCountMinTotRpmRulli=ini->ReadFloat("Counters","nCountMinTotRpmRulli",0);
  nLanciTotali=ini->ReadFloat("Counters","nLanciTotali",0);

  //controllo la versione del file ini
  vers=ini->ReadInteger ( "VerINI", "Versione", 1 );
  bFilePerf=ini->ReadInteger("Profili", "Performance",0);
  if (vers<2)
  {
     //file ini non ancora aggiornato
     //aggiorno il file per la rimozione degli accenti
     //per mercati non europei
     //chiudo il file
     delete ini;
     //eseguo l'aggiornamento
     aggiornaINI();
     //riapro il file
      ini = new TIniFile( utils.GetProgramDataName(".ini") );
  }

  limit.fMaxPress=ini->ReadFloat( "Map"+Profilo,"Press_max",2500);
  limit.fRedPress=ini->ReadFloat ( "Map"+Profilo,"Press_rosso",1800 );
  limit.fGreenPress=ini->ReadFloat ( "Map"+Profilo,"Press_giallo",1300 );
  limit.fYellowPress=ini->ReadFloat ( "Map"+Profilo,"Press_verde",1000 );
  limit.fMinPress=ini->ReadFloat( "Map"+Profilo,"Press_min",-100);

  Unit_Cop = ini->ReadInteger ( "Unita misura"+Profilo, "Coppia", 1 );
  Unit_Pot = ini->ReadInteger ( "Unita misura"+Profilo, "Potenza", 1 );
  Unit_Cons = ini->ReadInteger ( "Unita misura"+Profilo, "Consumi", 1 );
  Unit_Tem = ini->ReadInteger ( "Unita misura"+Profilo, "Temperatura", 0 );
  Unit_Vel = ini->ReadInteger ( "Unita misura"+Profilo, "Velocita", 0 );
  Unit_Map = ini->ReadInteger ( "Unita misura"+Profilo, "Manifold", 0 );
  tipoBanco =ini->ReadInteger ( "Tipologia Dyno"+Profilo, "Banco", 0 );
  canPresent=ini->ReadInteger ( "Tipologia Dyno"+Profilo, "CAN",0);
  btnCan->Visible=(canPresent>0);
  GroupBox4->Visible=(tipoBanco==TRACTOR_TWIN);

  Panel1->Width=ini->ReadInteger ( "Disposizione"+Profilo, "Pannelli", 640 );
  Panel33->Width=ini->ReadInteger ( "Disposizione"+Profilo, "Pannelli4", 65 );

  Panel4->Width=ini->ReadInteger ( "Disposizione"+Profilo, "Pannello_rpm_W", 184 );
  Panel8->Width=ini->ReadInteger ( "Disposizione"+Profilo, "Pannello_Mt_W", 184 );
  Panel10->Width=ini->ReadInteger ( "Disposizione"+Profilo, "Pannello_Pw_W", 184 );
  Panel20->Width=ini->ReadInteger ( "Disposizione"+Profilo, "Pannello_Tk_W", 184 );

  Panel34->Height=ini->ReadInteger ( "Disposizione"+Profilo, "Pannello_rpm_H", 105 );
  Panel43->Height=ini->ReadInteger ( "Disposizione"+Profilo, "Pannello_Mt_H", 105 );
  Panel44->Height=ini->ReadInteger ( "Disposizione"+Profilo, "Pannello_Pw_H", 105 );
  Panel45->Height=ini->ReadInteger ( "Disposizione"+Profilo, "Pannello_Tk_H", 105 );
  Panel46->Height=ini->ReadInteger ( "Disposizione"+Profilo, "Pannello_Ta_H", 105 );

  ToolBar5->Visible=ini->ReadInteger ( "Disposizione"+Profilo, "Barra_strumenti",1);

  Allarme = ini->ReadInteger ( "Allarmi"+Profilo, "Visivi", 0 );


  Strumenti->CheckBox6->Checked=ini->ReadBool ( "Indicatori"+Profilo, "Manifold",1);
  Strumenti->CheckBox7->Checked=ini->ReadBool ( "Indicatori"+Profilo, "TermoK",0);
  Strumenti->CheckBox8->Checked=ini->ReadBool ( "Indicatori"+Profilo, "TOlio",1);
  Strumenti->CheckBox1->Checked=ini->ReadBool ( "Indicatori"+Profilo, "Consumi",1);
  Strumenti->cbTAir->Checked=ini->ReadBool ( "Indicatori"+Profilo, "TAria",1);

  sTipo_dato =ini->ReadInteger ( "Indicatori"+Profilo, "Max valore",3);

  Lingua=ini->ReadInteger ( "Lingua in uso"+Profilo, "Codice", 0 );
  iMaxStepDistance=ini->ReadInteger( "Impostazioni test"+Profilo, "Massima distanza step",50);
  tmrInterval=ini->ReadInteger( "Impostazioni test"+Profilo, "Intervallo test comando rpm",50);
  trqInterval=ini->ReadInteger( "Impostazioni test"+Profilo, "Intervallo test comando trq",350);
  fAccMax=ini->ReadFloat( "Impostazioni test"+Profilo, "Max Accelerazione",1);
  Reg_iniz_acc=ini->ReadInteger( "Impostazioni test"+Profilo, "Reg_iniz_acc",1500);
  Reg_fine_acc=ini->ReadInteger( "Impostazioni test"+Profilo, "Reg_fine_acc",5000);
  Tempo_acc=ini->ReadInteger( "Impostazioni test"+Profilo, "Tempo_acc",20);

  Reg_iniz_dec=ini->ReadInteger( "Impostazioni test"+Profilo, "Reg_iniz_dec",2500);//a.balbo
  Reg_fine_dec=ini->ReadInteger( "Impostazioni test"+Profilo, "Reg_fine_dec",1200);//a.balbo
  Tempo_dec=ini->ReadInteger( "Impostazioni test"+Profilo, "Tempo_dec",20);

  Tempo_staz=ini->ReadInteger( "Impostazioni test"+Profilo, "Tempo_staz",2);//a.balbo
  num_sec_attesa=ini->ReadInteger( "Impostazioni test"+Profilo, "Tempo_timeout",20);
  fDelayStep=ini->ReadFloat( "Impostazioni test"+Profilo, "Tempo_delay",3.0);
  selPto=ini->ReadInteger( "Impostazioni test"+Profilo, "PTO",1);
  rpmStart=ini->ReadInteger( "Impostazioni test"+Profilo, "rpmStart",2500);//a.balbo
  rpmStop=ini->ReadInteger( "Impostazioni test"+Profilo, "rpmStop",1200);//a.balbo
  Soglia_min=ini->ReadFloat( "Impostazioni test"+Profilo, "Soglia_Min",10);
  Val_C_cost=ini->ReadFloat( "Impostazioni test"+Profilo, "Mt_cost",5);
  Delta_reg=ini->ReadInteger( "Impostazioni test"+Profilo, "Delta_reg",100);
  Tipo_test=ini->ReadInteger( "Impostazioni test"+Profilo, "Tipo_test",0);  
  Tipo_ciclo=ini->ReadInteger( "Impostazioni test"+Profilo, "Tipo_ciclo",1);
  Tipo_Alim=ini->ReadInteger( "Impostazioni test"+Profilo, "Tipo_Alim",1);
  Tipo_norm=ini->ReadInteger( "Impostazioni test"+Profilo, "Tipo_norm",0);
  Fattore_c=ini->ReadFloat( "Impostazioni test"+Profilo, "Fattore",1);
  Coeff_fm=ini->ReadFloat( "Impostazioni test"+Profilo, "Coeff_fm", Coeff_fm);
  K_R=ini->ReadFloat( "Offset"+Profilo, "Manifold", 1000);
  Ragione=ini->ReadString( "Dati_Cliente", "Ragione"+Profilo, "" );
  Indirizzo=ini->ReadString( "Dati_Cliente", "Indirizzo"+Profilo, "" );
  Telefono=ini->ReadString( "Dati_Cliente", "Telefono"+Profilo, "" );
  Fax=ini->ReadString( "Dati_Cliente", "Fax"+Profilo, "" );
  Internet=ini->ReadString( "Dati_Cliente", "Internet"+Profilo, "" );
  EMail=ini->ReadString( "Dati_Cliente", "E-mail"+Profilo, "" );
  Percorso=ini->ReadString( "Immagine","logo"+Profilo, "");
  sInPath=ini->ReadString("Export Data"+Profilo, "InPath","");
  if(sInPath=="")
  {
    SHGetFolderPath(NULL, CSIDL_PERSONAL,NULL,NULL,cDocPath);
    strcat(cDocPath,"\\DynoTractorTest\\In");
    sInPath=(String)cDocPath;//cArchivePath;
    if(!DirectoryExists(cDocPath))
      CreateDirectory(cDocPath,NULL);
    sInPath=(String)cDocPath;
  }
  sOutPath=ini->ReadString("Export Data"+Profilo, "OutPath","");
  if(sOutPath=="")
  {
    SHGetFolderPath(NULL, CSIDL_PERSONAL,NULL,NULL,cDocPath);
    strcat(cDocPath,"\\DynoTractorTest\\Out");
    sOutPath=(String)cDocPath;//cArchivePath;
    if(!DirectoryExists(cDocPath))
      CreateDirectory(cDocPath,NULL);
    sOutPath=(String)cDocPath;
  }
  Val_Hr=ini->ReadInteger( "Fattori ambientali"+Profilo, "Umidita",40);
  //PageControl1->ActivePageIndex=0;//ini->ReadInteger( "PageControl1"+Profilo,"Num", 0);
  //PageControl2->ActivePageIndex=0;// ini->ReadInteger( "PageCont"+Profilo,"Num", 0);
  tipoRapp=ini->ReadInteger( "Impostazioni Rapporto"+Profilo, "Rap_tipo_calc",0);
  Reg_calc_rap=ini->ReadInteger( "Impostazioni Rapporto"+Profilo, "Reg_calc_rap",2000);
  N_Cilindri=ini->ReadInteger( "Impostazioni Rapporto"+Profilo, "N_Cilindri",4);
  N_Tempi=ini->ReadInteger( "Impostazioni Rapporto"+Profilo, "N_Tempi",4);
  Rap_tot1=ini->ReadFloat( "Impostazioni Rapporto"+Profilo, "Rap_tot1",1);
  sCammes=ini->ReadInteger( "Impostazioni Rapporto"+Profilo, "Cammes", 1);
  iOffset_patm=ini->ReadInteger( "Offset"+Profilo, "Patmo",0);
  iOffset_temp=iOffset_patm;

          Vis_P_R = ini->ReadInteger( "Impostazioni grafico"+Profilo, "P_R",1);
          Vis_C_R = ini->ReadInteger( "Impostazioni grafico"+Profilo, "C_R",1);
          Vis_Pmap = ini->ReadInteger( "Impostazioni grafico"+Profilo, "Pmap",0);
          Vis_T_K = ini->ReadInteger( "Impostazioni grafico"+Profilo, "T_K",0);
          Vis_T_N = ini->ReadInteger( "Impostazioni grafico"+Profilo, "T_N",0);
          Vis_GV  = ini->ReadInteger( "Impostazioni grafico"+Profilo, "Giri/Vel",0);
          Vis_cons=ini->ReadInteger( "Impostazioni grafico"+Profilo, "Consumi",0);
  Opzioni_grafici->cbPZeroScale->Checked=ini->ReadInteger( "Impostazioni grafico"+Profilo, "Potenza_Min_0",0);
  Opzioni_grafici->cbCZeroScale->Checked=ini->ReadInteger( "Impostazioni grafico"+Profilo, "Coppia_Min_0",0);
  Giri_Mot->Left = ini->ReadInteger( "Disposizione Strumento Giri"+Profilo, "Left",652);
  Giri_Mot->Top = ini->ReadInteger( "Disposizione Strumento Giri"+Profilo, "Top",93);
  Giri_Mot->Width = ini->ReadInteger( "Disposizione Strumento Giri"+Profilo, "Width",364);
  Giri_Mot->Height = ini->ReadInteger( "Disposizione Strumento Giri"+Profilo, "Height",288);
  Vis_Rpm = ini->ReadInteger( "Disposizione Strumento Giri"+Profilo, "Show",1);

  Potenza->Left = ini->ReadInteger( "Disposizione Strumento Pot"+Profilo, "Left",652);
  Potenza->Top = ini->ReadInteger( "Disposizione Strumento Pot"+Profilo, "Top",380);
  Potenza->Width = ini->ReadInteger( "Disposizione Strumento Pot"+Profilo, "Width",364);
  Potenza->Height = ini->ReadInteger( "Disposizione Strumento Pot"+Profilo, "Height",308);
  Vis_Pot = ini->ReadInteger( "Disposizione Strumento Pot"+Profilo, "Show",0);

  Coppia->Left = ini->ReadInteger( "Disposizione Strumento Cop"+Profilo, "Left",652);
  Coppia->Top = ini->ReadInteger( "Disposizione Strumento Cop"+Profilo, "Top",380);
  Coppia->Width = ini->ReadInteger( "Disposizione Strumento Cop"+Profilo, "Width",364);
  Coppia->Height = ini->ReadInteger( "Disposizione Strumento Cop"+Profilo, "Height",308);
  Vis_Cop = ini->ReadInteger( "Disposizione Strumento Cop"+Profilo, "Show",0);

  Map->Left = ini->ReadInteger( "Disposizione Strumento Map"+Profilo, "Left",652);
  Map->Top = ini->ReadInteger( "Disposizione Strumento Map"+Profilo, "Top",380);
  Map->Width = ini->ReadInteger( "Disposizione Strumento Map"+Profilo, "Width",364);
  Map->Height = ini->ReadInteger( "Disposizione Strumento Map"+Profilo, "Height",308);
  Vis_Pres = ini->ReadInteger( "Disposizione Strumento Map"+Profilo, "Show",0);

  Termo->Left = ini->ReadInteger( "Disposizione Strumento TK"+Profilo, "Left",652);
  Termo->Top = ini->ReadInteger( "Disposizione Strumento TK"+Profilo, "Top",380);
  Termo->Width = ini->ReadInteger( "Disposizione Strumento TK"+Profilo, "Width",364);
  Termo->Height = ini->ReadInteger( "Disposizione Strumento TK"+Profilo, "Height",308);
  Vis_TemK = ini->ReadInteger( "Disposizione Strumento TK"+Profilo, "Show",0);

  Oil_Temp->Left=ini->ReadInteger( "Disposizione Strumento Olio"+Profilo, "Left",652);
  Oil_Temp->Top=ini->ReadInteger( "Disposizione Strumento Olio"+Profilo, "Top",380);
  Oil_Temp->Width=ini->ReadInteger( "Disposizione Strumento Olio"+Profilo, "Width",364);
  Oil_Temp->Height=ini->ReadInteger( "Disposizione Strumento Olio"+Profilo, "Height",308);
  Vis_Oil= ini->ReadInteger( "Disposizione Strumento Olio"+Profilo, "Show",0);

  Acc->Left = ini->ReadInteger( "Disposizione Strumento Acc"+Profilo, "Left",652);
  Acc->Top = ini->ReadInteger( "Disposizione Strumento Acc"+Profilo, "Top",380);
  Acc->Width = ini->ReadInteger( "Disposizione Strumento Acc"+Profilo, "Width",364);
  Acc->Height = ini->ReadInteger( "Disposizione Strumento Acc"+Profilo, "Height",308);
  Vis_Acc= ini->ReadInteger( "Disposizione Strumento Acc"+Profilo, "Show",0);


  iImpulsiLitro=ini->ReadInteger( "Consumometro"+Profilo, "ImpulsiLitro",800);
  N_mot_rap = ini->ReadFloat( "Regimi rapporti"+Profilo, "motore",2208);
  N_pto_rap = ini->ReadFloat( "Regimi rapporti"+Profilo, "Pto",1000);

  Rend_PTO = ini->ReadFloat( "Impostazioni test"+Profilo, "Rend_tras",0.90);


  Impostazioni->NxNumberEdit2->Value=Rend_PTO;


  N_val_Acq = ini->ReadFloat( "Acquisizione"+Profilo, "Time",10);



  sVentola = ini->ReadInteger( "Impostazioni grafico"+Profilo, "Ventilazione",0);


 Carica_File->Series1->Visible = ini->ReadInteger( "Impostazioni grafico anteprima"+Profilo, "pwruota",0);
 Carica_File->Series2->Visible = ini->ReadInteger( "Impostazioni grafico anteprima"+Profilo, "cruota",0);
 Avvio_OK = ini->ReadBool( "Stato_Chiusura", "OK",true);


 OK_TEST_AUTO = ini->ReadInteger( "Impostazioni test"+Profilo, "Inizio",0);


 delete ini;
 if( N_pto_rap && tipoRapp==RAP_MANUAL)
   Rap_tot1=N_mot_rap/N_pto_rap;
 else
   Rap_tot1=1;


 // if(PageControl1->ActivePageIndex==2)
 //   PageControl1->ActivePageIndex=0;


  if(Lingua==0)   // eseguito solo al primo avvio per selezione lingua in automatico
  {
        AnsiString SK;
        // lettura lingua in uso nel registro di sistema
        // e assegnazione della lingua presente
        TRegistry *Registry = new TRegistry(KEY_READ);
        try
        {
                Registry->RootKey = HKEY_CURRENT_USER;
                // controlla se esiste
                Registry->OpenKey("Control Panel\\International", false);
                SK = Registry->ReadString("sLanguage");
        }
        __finally
        {
                delete Registry;
        }

    if(SK!="")
    {
        if(SK=="ITA" || SK=="ITS" || SK=="MLT")

                Lingua=1;

        else
                if(SK=="DEU" || SK=="DES" || SK=="DEA" || SK=="DEL" || SK=="DEC")

                        Lingua=4;

                else
                        if(SK=="FRA" || SK=="FRS" || SK=="FRL" || SK=="FRM" || SK=="FRB" || SK=="FRC")

                                Lingua=3;

                        else
                                if(SK=="ESP" || SK=="CAT" || SK=="GLC" || SK=="ESM" || SK=="ESN" || SK=="ESC" || SK=="ESS" || SK=="ESF" || SK=="ESL")

                                        Lingua=5;

                                else
                                        Lingua=2; // Se la lingua del S.O. non è contemplata allora Default: Inglese

    }

  } // lingua==0



     Idioma(); // richiama lingua in uso

#ifdef _GAS_VER
  Label7->Visible=true;
  Edit1->Visible=true;
#endif
 //>>consumi impo iniziale

   flagConsumi = 0;
 //<<consumi impo iniziale
  if(selPto==1)
  {
    Impostazioni->RadioButton1->Checked=true;
    Chart1->BottomAxis->Title->Caption=Text202;
    Panel5->Caption=Text201;
    Rend_PTO=1;
    Rap_tot1=1;
  }
  else
  {
    Impostazioni->RadioButton2->Checked=true;
    Chart1->BottomAxis->Title->Caption=Text202;
    Panel5->Caption=Text200;
    selPto = 0;
  }

         if(Vis_P_R)
         {
         Series9->Visible=true;  // potenza ruota
         Series35->Visible=true;
         Series36->Visible=true;
         Series37->Visible=true;
         Series38->Visible=true;
         Series39->Visible=true;
         Series40->Visible=true;
         Series41->Visible=true;
         }
         else
         {
         Series9->Visible=false;
         Series35->Visible=false;
         Series36->Visible=false;
         Series37->Visible=false;
         Series38->Visible=false;
         Series39->Visible=false;
         Series40->Visible=false;
         Series41->Visible=false;

         }


         if(Vis_C_R)
         {
         Series10->Visible=true;   // coppia ruota
         Series21->Visible=true;
         Series22->Visible=true;
         Series23->Visible=true;
         Series24->Visible=true;
         Series25->Visible=true;
         Series26->Visible=true;
         Series27->Visible=true;
         }

         else
         {
          Series10->Visible=false;
          Series21->Visible=false;
          Series22->Visible=false;
          Series23->Visible=false;
          Series24->Visible=false;
          Series25->Visible=false;
          Series26->Visible=false;
          Series27->Visible=false;
          }


         if(Vis_Pmap)
         {
         Series3->Visible=true; // pressione aspirazione
         Series60->Visible=true;
         Series61->Visible=true;
         Series62->Visible=true;
         Series63->Visible=true;
         Series64->Visible=true;
         Series65->Visible=true;
         Series66->Visible=true;
         }
         else
         {
         Series3->Visible=false;
         Series60->Visible=false;
         Series61->Visible=false;
         Series62->Visible=false;
         Series63->Visible=false;
         Series64->Visible=false;
         Series65->Visible=false;
         Series66->Visible=false;

         }


         if(Vis_T_K)
         {
           Series6->Visible=true; // temperatura termocoppia K
           Series81->Visible=true;
           Series82->Visible=true;
           Series83->Visible=true;
           Series84->Visible=true;
           Series85->Visible=true;
           Series86->Visible=true;
           Series87->Visible=true;
         }

         else
         {
         Series6->Visible=false;
         Series81->Visible=false;
         Series82->Visible=false;
         Series83->Visible=false;
         Series84->Visible=false;
         Series85->Visible=false;
         Series86->Visible=false;
         Series87->Visible=false;
         }

         if(Vis_T_N)
         {
         Series7->Visible=true;   // Temperatura Ntc Olio
         Series88->Visible=true;
         Series89->Visible=true;
         Series90->Visible=true;
         Series91->Visible=true;
         Series92->Visible=true;
         Series93->Visible=true;
         Series94->Visible=true;
         }
         else
         {
          Series7->Visible=false;
          Series88->Visible=false;
          Series89->Visible=false;
          Series90->Visible=false;
          Series91->Visible=false;
          Series92->Visible=false;
          Series93->Visible=false;
          Series94->Visible=false;
         }
         if(Vis_cons)
         {
            Series15->Visible=true;
            Series16->Visible=true;
            Series17->Visible=true;
            Series18->Visible=true;
            Series28->Visible=true;
            Series29->Visible=true;
            Series30->Visible=true;
            Series31->Visible=true;
         }
         else
         {
            Series15->Visible=false;
            Series16->Visible=false;
            Series17->Visible=false;
            Series18->Visible=false;
            Series28->Visible=false;
            Series29->Visible=false;
            Series30->Visible=false;
            Series31->Visible=false;
         }


   if(Rap_tot1==0) Rap_tot1=4;

  Impostazioni->NxNumberEdit1->Value=Rap_tot1;

 ComboBox5->ItemIndex=Tipo_norm;



 if(Tipo_norm==7 )
 {
  TrackBar1->Visible=true;
  StaticText1->Visible=true;
  TrackBar1->Position=int(Fattore_c*1000);
  fFact = Fattore_c;
 }

   // configurazioni layout

      /*
     tipoBanco = 0  TRACTOR
     tipoBanco = 1  TRACTOR_TWIN
     tipoBanco = 2  TRAILER
    */

  // congela il tasto del calcolo rapporto se imp. manuale
  if(tipoRapp==RAP_MANUAL) 
    SpeedButton1->Enabled=false;




      // dati della barra di stato
  StatusBar1->Panels->Items[1]->Text=Text67 + " " + Profilo;



SpeedButton5->Flat=false;
SpeedButton10->Flat=false;
SpeedButton7->Flat=false;
SpeedButton11->Flat=false;
SpeedButton12->Flat=false;



 // attiva form...
 if(Vis_Rpm)
 {
 Giri_Mot->Show();
 SpeedButton5->OnClick = SpeedButton5Click2;
 SpeedButton5->Flat=true;
 }


 if(Vis_Pot)
 {
 Potenza->Show();
 SpeedButton10->OnClick = SpeedButton10Click2;
 SpeedButton10->Flat=true;
 }


 if(Vis_Cop)
 {
 Coppia->Show();
 SpeedButton7->OnClick = SpeedButton7Click2;
 SpeedButton7->Flat=true;
 }


 if(Vis_Pres)
 {
 Map->Show();
 SpeedButton11->OnClick = SpeedButton11Click2;
 SpeedButton11->Flat=true;
 }


 if(Vis_TemK)
 {
  Termo->Show();
  SpeedButton12->OnClick = SpeedButton12Click2;
  SpeedButton12->Flat=true;
 }



 if(Vis_Oil)
 {
 Oil_Temp->Show();
 SpeedButton26->OnClick = SpeedButton26Click2;
 SpeedButton26->Flat=true;
 }

 // attiva timer di verifica allarmi
 Timer_Allarm->Enabled=Allarme;
 GiriLabel->Color=clWhite;
 MapLabel->Color=clWhite;
 TKLabel->Color=clWhite;
 OilLabel->Color=clWhite;


}



void __fastcall TMain::salva_imp()
{


   // controllo plausibilità
   if(Panel1->Width==0) Panel1->Width=1;
   if(Panel33->Width==0) Panel33->Width=1;



  TIniFile *ini;
  ini = new TIniFile(
  utils.GetProgramDataName(".ini") );

  ini->WriteString ("Profili", "Utente in uso",Profilo);
  ini->WriteBool( "Stato_Chiusura", "OK",true);
  ini->WriteBool("Profili", "Tipo Manifold",bMapType);
 ini->WriteBool("Profili","Prima Esecuzione",bPrimaEsec);
  ini->WriteString( "Porta seriale", "Dyra", sComDyra);
  ini->WriteString( "Porta seriale", "Dyn3", sComDyn3);
  ini->WriteString( "Export Data"+Profilo, "InPath", sInPath);
  ini->WriteString( "Export Data"+Profilo, "OutPath", sOutPath);
//  ini->WriteInteger( "Porta seriale", "Com", ApdComPort1->ComNumber );
//  ini->WriteInteger( "Porta Brake", "Com", ApdComPort2->ComNumber );
  ini->WriteInteger( "Regime_max"+Profilo, "Giri/min", Giri_Mot->AnalogMeterHQ1->MaximumValue );
  ini->WriteInteger( "Regime_max"+Profilo, "Giri_giallo",Giri_Mot->AnalogMeterHQ1->YellowStartValue );
  ini->WriteInteger( "Regime_max"+Profilo, "Giri_rosso",Giri_Mot->AnalogMeterHQ1->RedStartValue );
  /*
  ini->WriteInteger( "Map"+Profilo, "Press_min", Map->AnalogMeterHQ1->MinimumValue );
  ini->WriteInteger( "Map"+Profilo, "Press_max", Map->AnalogMeterHQ1->MaximumValue );
  ini->WriteInteger( "Map"+Profilo, "Press_verde",Map->AnalogMeterHQ1->GreenStartValue );
  ini->WriteInteger( "Map"+Profilo, "Press_giallo",Map->AnalogMeterHQ1->YellowStartValue );
  ini->WriteInteger( "Map"+Profilo, "Press_rosso",Map->AnalogMeterHQ1->RedStartValue );
  */
  ini->WriteFloat( "Map"+Profilo, "Press_min", limit.fMinPress);
  ini->WriteFloat( "Map"+Profilo, "Press_max", limit.fMaxPress);
  ini->WriteFloat( "Map"+Profilo, "Press_verde",limit.fGreenPress);
  ini->WriteFloat( "Map"+Profilo, "Press_giallo",limit.fYellowPress);
  ini->WriteFloat( "Map"+Profilo, "Press_rosso",limit.fRedPress);

  ini->WriteInteger( "Coppia"+Profilo, "Kgfm",Coppia->AnalogMeterHQ1->MaximumValue );
  ini->WriteInteger( "Potenza"+Profilo, "KW",Potenza->AnalogMeterHQ1->MaximumValue );
  ini->WriteInteger( "Termocoppia"+Profilo, "Valore_max", Termo->AnalogMeterHQ1->MaximumValue );
  ini->WriteInteger( "Termocoppia"+Profilo, "Range_verde",Termo->AnalogMeterHQ1->GreenStartValue );
  ini->WriteInteger( "Termocoppia"+Profilo, "Range_giallo",Termo->AnalogMeterHQ1->YellowStartValue );
  ini->WriteInteger( "Termocoppia"+Profilo, "Range_rosso",Termo->AnalogMeterHQ1->RedStartValue );
  ini->WriteInteger( "Consumometro"+Profilo,"ImpulsiLitro",iImpulsiLitro);
  ini->WriteInteger( "Impostazioni test"+Profilo, "PTO",selPto);
  ini->WriteInteger( "Impostazioni test"+Profilo, "Massima distanza step",iMaxStepDistance);
  ini->WriteInteger( "Impostazioni test"+Profilo, "Intervallo test comando rpm",tmrInterval);
  ini->WriteInteger( "Impostazioni test"+Profilo, "Intervallo test comando trq",trqInterval);  
  ini->WriteInteger( "Oil"+Profilo, "Temp_min", Oil_Temp->AnalogMeterHQ1-> MinimumValue );
  ini->WriteInteger( "Oil"+Profilo, "Temp_max", Oil_Temp->AnalogMeterHQ1-> MaximumValue );
  ini->WriteInteger( "Oil"+Profilo, "Temp_verde", Oil_Temp->AnalogMeterHQ1->GreenStartValue );
  ini->WriteInteger( "Oil"+Profilo, "Temp_giallo", Oil_Temp->AnalogMeterHQ1->YellowStartValue );
  ini->WriteInteger ( "Oil"+Profilo, "Temp_rosso", Oil_Temp->AnalogMeterHQ1->RedStartValue );

  ini->WriteInteger ( "Unita misura"+Profilo, "Coppia", Unit_Cop );
  ini->WriteInteger ( "Unita misura"+Profilo, "Potenza", Unit_Pot );
  ini->WriteInteger ( "Unita misura"+Profilo, "Consumi", Unit_Cons );
  ini->WriteInteger ( "Unita misura"+Profilo, "Temperatura", Unit_Tem );
  //ini->WriteInteger ( "Unita misura"+Profilo, "Consumi_Grafico", iUnitConsumi );
  ini->WriteInteger ( "Unita misura"+Profilo, "Manifold", Unit_Map );

  ini->WriteFloat( "Offset"+Profilo, "Manifold",K_R);
  ini->WriteInteger ( "Fattori ambientali"+Profilo, "Umidita", Val_Hr );
  ini->WriteInteger( "Tipologia Dyno"+Profilo, "Banco",tipoBanco );
  ini->WriteInteger( "Tipologia Dyno"+Profilo, "CAN",canPresent );
  ini->WriteInteger( "Disposizione"+Profilo, "Air/Fuel",Vis_Air_Fuel );
  ini->WriteInteger ( "Disposizione"+Profilo, "Pannelli", Panel1->Width );
  ini->WriteInteger ( "Disposizione"+Profilo, "Pannelli4", Panel33->Width );


  ini->WriteInteger ( "Disposizione"+Profilo, "Pannello_rpm_W", Panel4->Width );
  ini->WriteInteger ( "Disposizione"+Profilo, "Pannello_Mt_W", Panel8->Width );
  ini->WriteInteger ( "Disposizione"+Profilo, "Pannello_Pw_W", Panel10->Width);
  ini->WriteInteger ( "Disposizione"+Profilo, "Pannello_Tk_W", Panel20->Width );


  ini->WriteInteger ( "Disposizione"+Profilo, "Pannello_rpm_H", Panel34->Height );
  ini->WriteInteger ( "Disposizione"+Profilo, "Pannello_Mt_H", Panel43->Height );
  ini->WriteInteger ( "Disposizione"+Profilo, "Pannello_Pw_H", Panel44->Height );
  ini->WriteInteger ( "Disposizione"+Profilo, "Pannello_Tk_H", Panel45->Height );
  ini->WriteInteger ( "Disposizione"+Profilo, "Pannello_Ta_H", Panel46->Height );


  ini->WriteInteger( "Lingua in uso"+Profilo, "Codice", Lingua );
  ini->WriteInteger( "Impostazioni test"+Profilo, "Reg_iniz_acc",Reg_iniz_acc);
  ini->WriteInteger( "Impostazioni test"+Profilo, "Reg_fine_acc",Reg_fine_acc);
  ini->WriteInteger( "Impostazioni test"+Profilo, "Tempo_acc",Tempo_acc);
  ini->WriteInteger( "Impostazioni test"+Profilo, "Reg_iniz_dec",Reg_iniz_dec);
  ini->WriteInteger( "Impostazioni test"+Profilo, "Reg_fine_dec",Reg_fine_dec);
  ini->WriteInteger( "Impostazioni test"+Profilo, "Tempo_dec",Tempo_dec);
  ini->WriteInteger( "Impostazioni test"+Profilo, "Tempo_staz",Tempo_staz);
  ini->WriteInteger( "Impostazioni test"+Profilo, "Tempo_timeout",num_sec_attesa);
  ini->WriteFloat( "Impostazioni test"+Profilo, "Tempo_delay",fDelayStep);

  ini->WriteInteger( "Impostazioni test"+Profilo, "rpmStart",rpmStart);
  ini->WriteInteger( "Impostazioni test"+Profilo, "rpmStop",rpmStop);
  ini->WriteInteger( "Impostazioni test"+Profilo, "Delta_reg",Delta_reg);
  ini->WriteInteger( "Impostazioni test"+Profilo, "Tipo_test",Tipo_test);

  ini->WriteInteger( "Impostazioni test"+Profilo, "Tipo_ciclo",Tipo_ciclo);
  ini->WriteInteger( "Impostazioni test"+Profilo, "Tipo_Alim",Tipo_Alim);
  ini->WriteInteger( "Impostazioni test"+Profilo, "Tipo_norm",Tipo_norm);
  ini->WriteFloat( "Impostazioni test"+Profilo, "Fattore",Fattore_c);
  ini->WriteFloat( "Impostazioni test"+Profilo, "Coeff_fm",Coeff_fm);
  ini->WriteFloat( "Impostazioni test"+Profilo, "Soglia_Min", Soglia_min);
  ini->WriteFloat( "Impostazioni test"+Profilo, "Mt_cost",Val_C_cost);
  ini->WriteString( "Dati_Cliente", "Ragione"+Profilo, Ragione );
  ini->WriteString( "Dati_Cliente", "Indirizzo"+Profilo, Indirizzo );
  ini->WriteString( "Dati_Cliente", "Telefono"+Profilo, Telefono );
  ini->WriteString( "Dati_Cliente", "Fax"+Profilo, Fax );
  ini->WriteString( "Dati_Cliente", "Internet"+Profilo, Internet );
  ini->WriteString( "Dati_Cliente", "E-mail"+Profilo, EMail );
  ini->WriteString( "Immagine","logo"+Profilo, Percorso);
  ini->WriteInteger( "PageControl1"+Profilo,"Num", PageControl1->ActivePageIndex);
  ini->WriteInteger( "PageCont"+Profilo,"Num", PageControl2->ActivePageIndex);
  ini->WriteInteger( "Impostazioni Rapporto"+Profilo, "Rap_tipo_calc", tipoRapp);
  ini->WriteInteger( "Impostazioni Rapporto"+Profilo, "Reg_calc_rap",Reg_calc_rap);
  ini->WriteInteger( "Impostazioni Rapporto"+Profilo, "N_Cilindri",N_Cilindri);
  ini->WriteInteger( "Impostazioni Rapporto"+Profilo, "N_Tempi",N_Tempi);
  ini->WriteFloat( "Impostazioni Rapporto"+Profilo, "Rap_tot1",Rap_tot1);
  ini->WriteInteger( "Impostazioni Rapporto"+Profilo, "Cammes", sCammes);
  ini->WriteInteger( "Impostazioni grafico"+Profilo, "P_R",Vis_P_R );
  ini->WriteInteger( "Impostazioni grafico"+Profilo, "C_R",Vis_C_R);
  ini->WriteInteger( "Impostazioni grafico"+Profilo, "Pmap", Vis_Pmap);
  ini->WriteInteger( "Impostazioni grafico"+Profilo, "T_K",Vis_T_K);
  ini->WriteInteger( "Impostazioni grafico"+Profilo, "T_N",Vis_T_N);
  ini->WriteInteger( "Impostazioni grafico"+Profilo, "Giri/Vel",Vis_GV);
  ini->WriteInteger( "Impostazioni grafico"+Profilo, "Consumi",Vis_cons);
  ini->WriteInteger( "Impostazioni grafico"+Profilo, "Potenza_Min_0",Opzioni_grafici->cbPZeroScale->Checked);
  ini->WriteInteger( "Impostazioni grafico"+Profilo, "Coppia_Min_0",Opzioni_grafici->cbCZeroScale->Checked);

  ini->WriteInteger( "Impostazioni grafico anteprima"+Profilo, "pwruota",Carica_File->Series1->Visible);
  ini->WriteInteger( "Impostazioni grafico anteprima"+Profilo, "cruota",Carica_File->Series2->Visible);
  ini->WriteInteger( "Impostazioni grafico"+Profilo, "Ventilazione",sVentola);
  ini->WriteInteger( "Disposizione Strumento Giri"+Profilo, "Left",Giri_Mot->Left);
  ini->WriteInteger( "Disposizione Strumento Giri"+Profilo, "Top",Giri_Mot->Top);
  ini->WriteInteger( "Disposizione Strumento Giri"+Profilo, "Width",Giri_Mot->Width);
  ini->WriteInteger( "Disposizione Strumento Giri"+Profilo, "Height",Giri_Mot->Height);
  ini->WriteInteger( "Disposizione Strumento Giri"+Profilo, "Show",Vis_Rpm);

  ini->WriteInteger( "Disposizione Strumento Pot"+Profilo, "Left",Potenza->Left);
  ini->WriteInteger( "Disposizione Strumento Pot"+Profilo, "Top",Potenza->Top);
  ini->WriteInteger( "Disposizione Strumento Pot"+Profilo, "Width",Potenza->Width);
  ini->WriteInteger( "Disposizione Strumento Pot"+Profilo, "Height",Potenza->Height);
  ini->WriteInteger( "Disposizione Strumento Pot"+Profilo, "Show",Vis_Pot);

  ini->WriteInteger( "Disposizione Strumento Cop"+Profilo, "Left",Coppia->Left);
  ini->WriteInteger( "Disposizione Strumento Cop"+Profilo, "Top",Coppia->Top);
  ini->WriteInteger( "Disposizione Strumento Cop"+Profilo, "Width",Coppia->Width);
  ini->WriteInteger( "Disposizione Strumento Cop"+Profilo, "Height",Coppia->Height);
  ini->WriteInteger( "Disposizione Strumento Cop"+Profilo, "Show",Vis_Cop);

  ini->WriteInteger( "Disposizione Strumento Map"+Profilo, "Left",Map->Left);
  ini->WriteInteger( "Disposizione Strumento Map"+Profilo, "Top",Map->Top);
  ini->WriteInteger( "Disposizione Strumento Map"+Profilo, "Width",Map->Width);
  ini->WriteInteger( "Disposizione Strumento Map"+Profilo, "Height",Map->Height);
  ini->WriteInteger( "Disposizione Strumento Map"+Profilo, "Show",Vis_Pres);

  ini->WriteInteger( "Disposizione Strumento TK"+Profilo, "Left",Termo->Left);
  ini->WriteInteger( "Disposizione Strumento TK"+Profilo, "Top",Termo->Top);
  ini->WriteInteger( "Disposizione Strumento TK"+Profilo, "Width",Termo->Width);
  ini->WriteInteger( "Disposizione Strumento TK"+Profilo, "Height",Termo->Height);
  ini->WriteInteger( "Disposizione Strumento TK"+Profilo, "Show",Vis_TemK);

  ini->WriteInteger( "Disposizione Strumento Acc"+Profilo, "Left",Acc->Left);
  ini->WriteInteger( "Disposizione Strumento Acc"+Profilo, "Top",Acc->Top);
  ini->WriteInteger( "Disposizione Strumento Acc"+Profilo, "Width",Acc->Width);
  ini->WriteInteger( "Disposizione Strumento Acc"+Profilo, "Height",Acc->Height);
  ini->WriteInteger( "Disposizione Strumento Acc"+Profilo, "Show",Vis_Acc);

  ini->WriteInteger( "Disposizione Strumento Olio"+Profilo, "Left",Oil_Temp->Left);
  ini->WriteInteger( "Disposizione Strumento Olio"+Profilo, "Top",Oil_Temp->Top);
  ini->WriteInteger( "Disposizione Strumento Olio"+Profilo, "Width",Oil_Temp->Width);
  ini->WriteInteger( "Disposizione Strumento Olio"+Profilo, "Height",Oil_Temp->Height);
  ini->WriteInteger( "Disposizione Strumento Olio"+Profilo, "Show",Vis_Oil);
  ini->WriteInteger( "Impostazioni test"+Profilo, "Inizio",OK_TEST_AUTO);
  ini->WriteFloat( "Impostazioni test"+Profilo, "Rend_tras",Rend_PTO);
  ini->WriteInteger ( "Disposizione"+Profilo, "Barra_strumenti",ToolBar5->Visible);
  ini->WriteInteger ( "Allarmi"+Profilo, "Visivi", Allarme );
  ini->WriteBool ( "Indicatori"+Profilo, "Manifold",Strumenti->CheckBox6->Checked);
  ini->WriteBool ( "Indicatori"+Profilo, "TermoK",Strumenti->CheckBox7->Checked);
  ini->WriteBool ( "Indicatori"+Profilo, "TAria",Strumenti->cbTAir->Checked);
  ini->WriteBool ( "Indicatori"+Profilo, "TOlio",Strumenti->CheckBox8->Checked);
  ini->WriteBool ( "Indicatori"+Profilo, "Consumi",Strumenti->CheckBox1->Checked);
  ini->WriteString( "Indicatori"+Profilo, "Max valore",sTipo_dato);
  ini->WriteFloat( "Regimi rapporti"+Profilo, "motore",N_mot_rap);
  ini->WriteFloat( "Regimi rapporti"+Profilo, "Pto",N_pto_rap);
  ini->WriteFloat( "Acquisizione"+Profilo, "Time",N_val_Acq);
  ini->WriteFloat( "Impostazioni test"+Profilo, "Max Accelerazione",fAccMax);
  delete ini;
}

void __fastcall TMain::FormClose(TObject *Sender, TCloseAction &Action)
{
  TIniFile *ini;

  //contaore
  //incremento il contaore
  if (contaoreSecondi>30) contaoreMinuti++;
  dynoPrm.count.nCountMinTotRpmRulli+=contaoreMinuti;      //minuti totali funzionamento
  dynoPrm.count.nCountMinParRpmRulli+=contaoreMinuti;      //minuti parziali funzionamento
  nCountMinTotRpmRulli+=contaoreMinuti;
  //azzero i contatori
  contaoreMinuti=0;
  contaoreSecondi=0;
  ini = new TIniFile(utils.GetProgramDataName(".INI"));
  /// Salvataggio conteggio ore lavoro e lanci
  ini->WriteFloat("Counters","dataResetCountMinPar",dataResetCountMinPar);
  ini->WriteFloat("Counters","dataResetLanci",dataResetLanci);
  ini->WriteFloat("Counters","nCountMinTotRpmRulli",nCountMinTotRpmRulli);
  ini->WriteFloat("Counters","nLanciTotali",nLanciTotali);
  ini->WriteBool( "Stato_Chiusura", "OK",Avvio_OK);
  delete ini;

  if(Start_acq)// stoppa acquisizione se attiva
    Start_acq=false;
  Timer1->Enabled=false;  // disattiva timer calcolo correzion
  bOnClose=true;
  // controlla e avvisa per salvare lavoro appena eseguito
  if(bSavedTest==false)
  {
    MessageBeep(MB_ICONASTERISK);
    if(MessageDlg(Str9,mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
    {
      Salvamisurazione1Click(this);
    }
    else Salvamisurazione1->Enabled=false;
  }

  g_dyraControl.CmdStop();
  if(g_brakeControl.GetDebugMode()==true)
    g_brakeControl.CmdDebug(false);
  Sleep(500);
  DestroyDyra();
  DestroyDyn3();

  scriviFileImpo();
  CloseArchive();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void __fastcall TMain::SpeedButton7Click(TObject *Sender)
{
 Coppia->Show();
 SpeedButton7->OnClick = SpeedButton7Click2;
  SpeedButton7->Flat=true;
 Vis_Cop=1;
  Mod_imp=true; // ok per salvataggio
}

void __fastcall TMain::SpeedButton7Click2(TObject *Sender)
{
 Coppia->Hide();
 SpeedButton7->OnClick = SpeedButton7Click;
  SpeedButton7->Flat=false;
 Vis_Cop=0;
  Mod_imp=true; // ok per salvataggio

}



//---------------------------------------------------------------------------

void __fastcall TMain::SpeedButton10Click(TObject *Sender)
{
 Potenza->Show();
 SpeedButton10->OnClick = SpeedButton10Click2;
  SpeedButton10->Flat=true;
 Vis_Pot=1;
  Mod_imp=true; // ok per salvataggio
}

void __fastcall TMain::SpeedButton10Click2(TObject *Sender)
{
 Potenza->Hide();
 SpeedButton10->OnClick = SpeedButton10Click;
  SpeedButton10->Flat=false;
 Vis_Pot=0;
  Mod_imp=true; // ok per salvataggio
}



//---------------------------------------------------------------------------

void __fastcall TMain::SpeedButton11Click(TObject *Sender)
{
 Map->Show();
  Vis_Pres=1;
   SpeedButton11->OnClick = SpeedButton11Click2;
    SpeedButton11->Flat=true;
     Mod_imp=true; // ok per salvataggio
}

void __fastcall TMain::SpeedButton11Click2(TObject *Sender)
{
 Map->Hide();
 Vis_Pres=0;
 SpeedButton11->OnClick = SpeedButton11Click;
  SpeedButton11->Flat=false;
   Mod_imp=true; // ok per salvataggio
}



//---------------------------------------------------------------------------

void __fastcall TMain::SpeedButton12Click(TObject *Sender)
{
 Termo->Show();
  Vis_TemK=1;
  SpeedButton12->OnClick = SpeedButton12Click2;
   SpeedButton12->Flat=true;
    Mod_imp=true; // ok per salvataggio
}

void __fastcall TMain::SpeedButton12Click2(TObject *Sender)
{
 Termo->Hide();
 Vis_TemK=0;
 SpeedButton12->OnClick = SpeedButton12Click;
  SpeedButton12->Flat=false;
   Mod_imp=true; // ok per salvataggio
}



//---------------------------------------------------------------------------


void __fastcall TMain::Impostazionistrumenti1Click(TObject *Sender)
{
 Strumenti->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMain::SpeedButton2Click(TObject *Sender)
{
  if(SpeedButton2->Enabled==false)
    return;
  Out_dat=false; // disattiva real-time
  PageControl2->ActivePageIndex=0;
  Out_dat=true; // attiva visualizzazione real time
  iPos=0;
  ChartTool9->Value=0;
  TabSheet2->Caption=Text90;
  TabSheet2->Highlighted=false;
  // aggiorna il primo valore di stazionamento giri per test Step...
  TimerStopPwm->Enabled=false;
  //incremento il contatore dei lanci effettuati
  //totali e parziali
  dynoPrm.count.nLanciAvv++;      //lanci totali avviati
  dynoPrm.count.nLanciAvvPar++;      //lanci parziali avviati
  sommaContaore();
  
 if(rpmStart < rpmStop )
        Reg_Stp=rpmStart;
 else
        Reg_Stp=rpmStart;
 switch(Tipo_test)
 {
  case TEST_STEP_TIMED:
    PageControl1->ActivePage=TabSheet1;
    SpeedButton4->Enabled=false;
    Step_test->Show();//Modal();  // avvia finestra di conf. inizio test
  break;
  case TEST_STEP:
    SpeedButton8->Enabled=false;
    Step_test->Show();//Modal();  // avvia finestra di conf. inizio test
  break;
  case TEST_DECEL:
    PageControl1->ActivePage=TabSheet1;  
    SpeedButton15->Enabled=false;
    Start->Show/*Modal*/();  // avvia finestra di conferma inizio test
  break;
  case TEST_BRAKE_MAN:
    SpeedButton17->Enabled=false;
    Pilot_Freno->Show();
  break;
  case TEST_COST_TRQ:
    SpeedButton21->Enabled=false;
    Mt_cost->ShowModal();//
  break;
 }

  
  
}
//---------------------------------------------------------------------------

void __fastcall TMain::Informazionisu1Click(TObject *Sender)
{
  Info = new TInfo(Application);
  Info->ShowModal();
  delete Info;
}
//---------------------------------------------------------------------------


void __fastcall TMain::Sequenzaparametriditest1Click(TObject *Sender)
{
  Impostazioni->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMain::Esci1Click(TObject *Sender)
{
 Close();
}
//---------------------------------------------------------------------------



void __fastcall TMain::Unitdimisura1Click(TObject *Sender)
{
 Misure->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMain::Cambiautente1Click(TObject *Sender)
{
 Utenti->ShowModal();
}
//---------------------------------------------------------------------------



void __fastcall TMain::SpeedButton1Click(TObject *Sender)
{
   // aggiorna il primo valore di stazionamento giri per test Step...
  Out_dat=true; // disattiva real-time
  Reg_Stp=rpmStart;
  if(Start->Active==false)
    Rapporto->ShowModal();
}
//---------------------------------------------------------------------------



void __fastcall TMain::SpeedButton5Click(TObject *Sender)
{
 Giri_Mot->Show();
 SpeedButton5->OnClick = SpeedButton5Click2;
 Vis_Rpm=1;
 SpeedButton5->Flat=true;
 Mod_imp=true; // ok per salvataggio
}

void __fastcall TMain::SpeedButton5Click2(TObject *Sender)
{
 Giri_Mot->Hide();
 SpeedButton5->OnClick = SpeedButton5Click;
 SpeedButton5->Flat=false;
 Vis_Rpm=0;
 Mod_imp=true; // ok per salvataggio
 }



//---------------------------------------------------------------------------


void __fastcall TMain::Impostazionigrafici1Click(TObject *Sender)
{
  Opzioni_grafici->ShowModal();
  // titoli del grafico
  String Titolo="";
  if(Vis_P_R) Titolo=Titolo+" " + Str1;
  if(Vis_C_R) Titolo=Titolo+" " + Str3;
  Chart1->Title->Text->Text=Titolo;
}
//---------------------------------------------------------------------------


void __fastcall TMain::FormShow(TObject *Sender)
{


  DWORD n;
  char *pBuf;
  dataFile=Now();

  SHGetFolderPath(NULL, CSIDL_PERSONAL,NULL,NULL,cDocPath);

  sprintf(cArchivePath,"%s\\%s",cDocPath,DYNO_TEST_PATH);
  if(!DirectoryExists(cArchivePath))
    CreateDirectory(cArchivePath,NULL);

  sprintf(cArchivePath,"%s\\%s\\archive",cDocPath,DYNO_TEST_PATH);
  if(!DirectoryExists(cArchivePath))
    CreateDirectory(cArchivePath,NULL);

  sprintf(cArchivePath,"%s\\%s\\archive\\%s",cDocPath,DYNO_TEST_PATH,Now().FormatString("YYYY"));
  if(!DirectoryExists(cArchivePath))
    CreateDirectory(cArchivePath,NULL);
  sprintf(cArchivePath,"%s\\%s\\archive\\%s\\%s",cDocPath,DYNO_TEST_PATH,Now().FormatString("YYYY"),Now().FormatString("MM"));
  if(!DirectoryExists(cArchivePath))
    CreateDirectory(cArchivePath,NULL);
  int j;
  contaoreMinuti=0;
  contaoreSecondi=0;
  KeyPreview = true; // attiva lettura da tastiera
  Tabella_test_1->TabVisible=false;
  Tabella_test_2->TabVisible=false;
  Tabella_test_3->TabVisible=false;
  Tabella_test_4->TabVisible=false;
  Tabella_test_5->TabVisible=false;
  Tabella_test_6->TabVisible=false;
  Tabella_test_7->TabVisible=false;
  Tabella_test_8->TabVisible=false;
  TabSheet11->TabVisible=false;
  TabSheet7->TabVisible=false;
  TabSheet8->TabVisible=false;
  NextGrid9->ClearRows();
  NextGrid11->ClearRows();
  NextGrid12->ClearRows();
  StaticText2->Color=Series105->Color;

  TIniFile *ini;
  ini = new TIniFile(utils.GetProgramDataName(".INI"));
  bFileTrace=ini->ReadInteger("Profili", "Trace    ",0);
  bPrimaEsec=ini->ReadBool("Profili", "Prima Esecuzione",true);
  sComDyra=ini->ReadString( "Porta seriale", "Dyra", "");
  sComDyn3=ini->ReadString( "Porta seriale", "Dyn3", "");
  statusRelaisConfig=0x10;
  delete ini;

  /// **** APERTURA PORTA DYN3 ****
  CreateDyn3(&bNotifyDyn3,g_cDyn3Log);


  /// **** APERTURA PORTA DYRA ****
  CreateDyra(notifyDyra,g_cDyraLog);

  hEvDataReady=CreateEvent(NULL,true,false,NULL);
  SetEvent(hEvDataReady);

  brakeStatus.b.bBrakeAnteDisconnected=0;
  brakeStatus.b.bBrakeAnteNotPowered=0;
  g_brakeControl.SetAxis(CDyn3::EFrontAxle);
  dyn3Status=g_brakeControl.GetDyn3Status();
  if(sComDyn3=="")
  {
    g_brakeControl.autoDetect();
    j=50;
    while(g_brakeControl.isAutoDetect() && (j--)>0)
    {
      Application->ProcessMessages();
      Sleep(10);
    }
  }
  else
    g_brakeControl.Connect(sComDyn3.c_str());

  Sleep(200);
  if(g_brakeControl.isOn())
  {
    Label29->Hint=g_brakeControl.GetPort();
    sComDyn3=(String)g_brakeControl.GetPort();
  }

  if(sComDyra=="")
  {
    g_dyraControl.autoDetect();
    j=50;
    while(g_dyraControl.isAutoDetect()  && (j--)>0)
    {
      Application->ProcessMessages();
      Sleep(20);
    }
    if(g_dyraControl.isOn())
      sComDyra=(String)g_dyraControl.GetPort();
  }
  else
    g_dyraControl.Connect(sComDyra.c_str());

  /// Invio comandi alla Dyra: STOP invio dati + rele consumi + START invio dati
  if(g_dyraControl.isOn())
  {
    Label28->Hint=g_dyraControl.GetPort();
    g_dyraControl.CmdStop();
    g_dyraControl.CmdRele(statusRelaisConfig);
    g_dyraControl.CmdStart();
  }

  g_dyraControl.ActiveLog(bFileTrace);

  g_brakeControl.ActiveLog(bFileTrace);

  ChartTool9->Visible=false;

  PageControl1->ActivePage=TabSheet1;
  SHGetFolderPath(NULL, CSIDL_PERSONAL,NULL,NULL,cDocPath);
  sprintf(g_cCanLog,"%s\\%s\\CAN.log",cDocPath,DYNO_TEST_PATH);
  sprintf(g_cDyraLog,"%s\\%s\\dyra.log",cDocPath,DYNO_TEST_PATH);
  sprintf(g_cDyn3Log,"%s\\%s\\dyn3.log",cDocPath,DYNO_TEST_PATH);

  DecimalSeparator='.';

  n = GetFileVersionInfoSize(Application->ExeName.c_str(), &n);
  if (n > 0)
  {
    pBuf= new char[n];
    AnsiString temp = "VersionInfoSize = ";
    GetFileVersionInfo(Application->ExeName.c_str(), 0,n, pBuf);
    sprintf (swNbr, "%u.%u.%u.%u",pBuf[58],pBuf[56], pBuf[54],pBuf[60]);
    delete [] pBuf;
  }

  // carica il logo iniziale....
  DecimalSeparator='.';

  m_minV_T=50;
  Splash = new TSplash(Application);
  Splash->ShowModal();
  delete Splash;
  // carica file d'impostazioni.......

  if(!DirectoryExists(utils.GetProgramDataPath("DB").c_str()))
  {
    CreateDirectory(utils.GetProgramDataPath("DB").c_str(),NULL);
  }

  carica_imp(); // richiama funzione di lettura ini file e idioma
  caricaBanco();
#if 0
  String nomefile;

  if(tipoBanco==2)//Trailer
    nomefile="File_impo_trailer.dmt";
  else
    nomefile="File_impo_trac.dmt";
  ifstream infile(utils.GetProgramDataPath(nomefile.c_str()).c_str(), ios::binary);  // carica impostazioni
  if(infile.is_open())
  {
    infile.read((unsigned char*)&dynoPrm,sizeof (dynoPrm));
    infile.close();
    okProcessData = 1;

  }
  else
  {
    LogError(g_cFileLog,LOG_ERR,"Impossibilità di aprire file_imp");
    Timer1->Enabled=false;
    Log_Error(3,NULL,NULL);
    ShowMessage(Str7);
    Close();
  }
#endif



  strcpy(g_cFileDiag,utils.GetProgramDataPath("dati").c_str());

  strcpy(cDiagName,utils.GetSaveDir().c_str());
  strcat(cDiagName,"\\performance");

  //char strTmp[MAX_PATH]="";
  time_t rawtime;
  struct tm * timeinfo;
  char d[12];
  char t[9];

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  strftime (d,12,"%Y-%m-%d_",timeinfo);
  strftime (t,9,"%H.%M.%S",timeinfo);
  strcat(cDiagName,d);
  strcat(cDiagName,t);
  strcat(cDiagName, ".csv");

  // ****** lettura file di taratura delle celle di carico *********************

  ifstream inf(utils.GetProgramDataPath("File_tara.cel").c_str(), ios::binary);  // carica impostazioni
  if(!inf)
  {
    Log_Error(4,NULL,NULL);
    ShowMessage(Str8);
    Fileditaraturacelle2->Enabled=false;

  }

  else
  {
    inf.read((unsigned char*)&cellPrm,sizeof (cellPrm));
    inf.close();
    Fileditaraturacelle2->Enabled=true;
  }
  // ***************************************************************************


  flagConsumi = 1;
  Main->ConsLabel->Caption = 0;
  Pos_consumi = 0;
  //azzero i litri totali
  litri_tot=0;
  uiPrecImpulsiCons = 0;
  litri_h = 0;
  consumiGiaAttivo = 1;

  switch(Tipo_test)
  {
    case 0:
            SpeedButton4->Click();
            break;
            case 1:
            SpeedButton8->Click();
            break;
            case 2:
            SpeedButton15->Click();
            break;

            case 4:
            SpeedButton21->Click();
            break;
            case 5:
            SpeedButton17->Click();
            break;

            }

  if(Avvio_OK==false)
  {
     if(MessageDlg(Text89,mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
       {
  Carica_File->FileListBox1Click(this);
  Carica_File->BitBtn2Click(this);
  Salvamisurazione1->Enabled=true;
       }

  }
  if(bPrimaEsec)
  {
    bPrimaEsec=false;
    WindowState=wsMaximized;
    Panel1->Width=3*Width/4;
  }
  if(g_brakeControl.isOn()==false || g_dyraControl.isOn()==false)
    PageControl1->ActivePage=TabSheet9;

#ifdef _INDUSTRY4
  bAssistance->Visible=true;
  bTestToExport=false;
  ExportData1->Visible=true;
  sbDataIn->Visible=true;
  sbDataOut->Visible=true;
  sbDataOut->Enabled=false;
#else
  bAssistance->Visible=false;
  ExportData1->Visible=false;
  sbDataIn->Visible=false;
  sbDataOut->Visible=false;
#endif

#ifdef CAN_BUS



#endif


  PrepareArchive(pZip);

  tmrProcessData->Enabled=true;
  bOnInit=false;
#ifdef _DEBUG
  LogError(g_cFileLog,LOG_INFO,"Terminata Main Show");
#endif
}
//---------------------------------------------------------------------------




void __fastcall TMain::VrRocker1LowerClick(TObject *Sender)
{
  VrBlinkLed1->EnableBlinking=false;
  Timer_Atm->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TMain::VrRocker1UpperClick(TObject *Sender)
{
 VrBlinkLed1->EnableBlinking=true;
 Timer_Atm->Enabled=true;
 Series11->Clear();
 Series12->Clear();
 Series13->Clear();
 Series14->Clear();

 Timer_Atm->Tag=0;
}
//---------------------------------------------------------------------------



void __fastcall TMain::Timer_AtmTimer(TObject *Sender)
{

  float Val_t_conv;


   if(Unit_Tem ==1)Val_t_conv = ConvertTo(TAir,tuFahrenheit);
      else Val_t_conv = TAir;

  Series11->AddXY(Time(),Val_t_conv,"",clTeeColor);
  Series12->AddXY(Time(),Val_pat,"",clTeeColor);
  Series13->AddXY(Time(),Val_Hr,"",clTeeColor);
  Series14->AddXY(Time(),fFact,"",clTeeColor);
}
//---------------------------------------------------------------------------






void __fastcall TMain::ComboBox5Change(TObject *Sender)
{

  TrackBar1->Visible=false;
  StaticText1->Visible=false;
  Tipo_norm=ComboBox5->ItemIndex;


 if(Tipo_norm==7 ) // MANUALE
 {
  TrackBar1->Visible=true;
  StaticText1->Visible=true;
  TrackBar1->Position=int(Fattore_c*1000);
  fFact = Fattore_c;
 }




 }
//---------------------------------------------------------------------------




void __fastcall TMain::Setupsistema2Click(TObject *Sender)
{
  Setup->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMain::Fileditaraturacelle1Click(TObject *Sender)
{

  // Importa file di taratura delle celle

 AnsiString NewFileName = utils.GetProgramDataPath("File_tara.cel").c_str(), SS;


 if(OpenDia_tara->Execute())
 {
   SS= OpenDia_tara->FileName;

   ifstream in(SS.c_str(), ios::in | ios::binary);  // apre file d'impostazioni
   in.read((unsigned char*)&cellPrm,sizeof (cellPrm));
   in.close();
     // salva file su disco
   ofstream out(NewFileName.c_str(), ios::out | ios::binary);  // salva le impostazioni
   out.write((unsigned char*)&cellPrm,sizeof (cellPrm));
   out.close();
   Log_Error(0,Errori[7],NULL);
 }





}
//---------------------------------------------------------------------------

void __fastcall TMain::Fileditaraturacelle2Click(TObject *Sender)
{



 if(SaveDia_tara->Execute())
 {
  ofstream out(utils.GetProgramDataPath("File_tara.cel").c_str(), ios::out | ios::binary);  // salva le impostazioni
  out.write((unsigned char*)&cellPrm,sizeof (cellPrm));
  out.close();
  Log_Error(0,Errori[8],NULL);
 }



}
//---------------------------------------------------------------------------



// provvede a impostare i coeff. necessari alla conversione delle unità di misura
void __fastcall TMain::carica_unita()
{
  String strUnit;

  // coppia ruota.......................
  if(Unit_Cop==0) // se unità coppia N/m
  {
      fK_convC   = 9.8;
      strUnit=="Nm";
  }
  if(Unit_Cop==1) // se unità coppia Kg·m
  {
    fK_convC=1;
    strUnit="Kgm";
  }
  if(Unit_Cop==2) // se unità coppia lb/ft
  {
    fK_convC   = 7.233;//0.67;
    strUnit="LbFt";
  }
  Coppia->StaticText1->Caption=Unit_M;
  Panel9->Caption=SpeedButton7->Caption+Unit_M;
  Chart1->RightAxis->Title->Caption=Unit_M;
  Carica_File->Chart1->RightAxis->Title->Caption=Unit_M;
  Strumenti->Label6->Caption=Unit_M;
  Impostazioni->Label29->Caption=Unit_M;
  Mt_cost->Label3->Caption=Unit_M;
  //..........................................

  switch(Unit_Pot)
  {
    case 0:  
      fK_convP=0.735;
      strUnit="kW";
    break;
    case 1:  
      fK_convP=1;
      strUnit="Cv";        
    break;
    case 2:
      fK_convP=0.986;
      strUnit="Hp";   
    break;
  }
  Potenza->StaticText1->Caption=Unit_pw;
  Panel11->Caption=SpeedButton10->Caption+Unit_pw;
  Chart1->LeftAxis->Title->Caption=Unit_pw;
  Carica_File->Chart1->LeftAxis->Title->Caption=Unit_pw;
  Strumenti->Label7->Caption=Unit_pw;
  //...........................................


  switch(Unit_Cons)
  {
    case 0: // litri/ora
      Panel7->Caption=TextCons+" g/kWh - l/h";
      fK_convCons= 1;
      Opzioni_grafici->rbL->Caption="l/h";
      Opzioni_grafici->rbL->Visible=true;
    break;
    case 1: // litri/minuto
      Panel7->Caption=TextCons+" g/kWh - l/min";
      fK_convCons= 0.01667;
      Opzioni_grafici->rbL->Caption="l/min";
      Opzioni_grafici->rbL->Visible=true;
    break;
    case 2: // grammi/Kwh
      Panel7->Caption=TextCons+" g/kWh";
      fK_convCons= 0;
      Opzioni_grafici->rbL->Visible=false;
      Opzioni_grafici->rbGKWh->Checked=true;
    break;
    case 3: // litri
        Panel7->Caption=TextCons+" g/kWh - l";
        fK_convCons= 0;
        Opzioni_grafici->rbL->Caption="l";
        Opzioni_grafici->rbL->Visible=true;
    break;
  }
  if(Opzioni_grafici->rbGKWh->Checked)
    Chart1->CustomAxes->Items[4]->Title->Caption="g/kWh";
  else
    Chart1->CustomAxes->Items[4]->Title->Caption=Opzioni_grafici->rbL->Caption;
  Chart1->CustomAxes->Items[4]->Visible=Vis_cons;


  if(Unit_Cons==2)
  {
    ConsLabel2->Visible=false;
    ConsLabel->Align=alClient;
    Splitter1->Align=alRight;    
    ConsLabel->Width=Panel6->Width;
  }
  else
  {
    ConsLabel2->Visible=true;
    ConsLabel->Align=alLeft;
    Splitter1->Align=alLeft;
    ConsLabel2->Width=Panel6->Width/2;
    ConsLabel->Width=Panel6->Width/2;
  }

 if(Unit_Pes == 0) // da N->Kgf
 {
   fK_convPes= 0.102;
 }
 if(Unit_Pes == 1) // da Kgf-> Kgf
 {
   fK_convPes = 1;
 }
 if(Unit_Pes == 2) // da Lb->Kgf
 {
   fK_convPes = 0.453;
 }

 if(Unit_Sup == 0) // da m²-> m²
 {
        fK_convSup = 1;
 }
 if(Unit_Sup == 1) // da ft²-> m²
 {
        fK_convSup = 0.093;
 }

  // Temperatura.....................................................
  if(Unit_Tem==0)  
  {
    strUnit="°C";
    Chart2->LeftAxis->Minimum=-10;
    Chart2->LeftAxis->Maximum=50;
  }
  if(Unit_Tem==1)
  {    
    strUnit="°F";
    Chart2->LeftAxis->Minimum=14;
    Chart2->LeftAxis->Maximum=122;
  }
  Termo->StaticText1->Caption=strUnit;
  Panel17 ->Caption="T(K)"+strUnit;
  Panel28 ->Caption=strUnit;
  Panel19 ->Caption=Str12+" "+strUnit;
  Panel13 ->Caption=Str13+" "+strUnit;
  Series6->CustomVertAxis->Title->Caption="T(K)"+strUnit;
  Series7->CustomVertAxis->Title->Caption=Str12+" "+strUnit;
  Chart2->LeftAxis->Title->Caption=strUnit;
  Strumenti->Label10->Caption=strUnit;
  Strumenti->Label11->Caption=strUnit;
  Oil_Temp->StaticText1->Caption=strUnit;

  // pressione manifold........................
  if(Unit_Map == 0) // se mbar
  {
    fK_convPr = 1;
    strUnit="mbar";
  }
  if(Unit_Map == 1) // se Psi
  {
    fK_convPr = 0.0145;
    strUnit="psi";
  }
  Map->AnalogMeterHQ1->MinimumValue=limit.fMinPress*fK_convPr;
  Map->AnalogMeterHQ1->MaximumValue=limit.fMaxPress*fK_convPr;
  Map->AnalogMeterHQ1->GreenStartValue=limit.fGreenPress*fK_convPr;
  Map->AnalogMeterHQ1->YellowStartValue=limit.fYellowPress*fK_convPr;
  Map->AnalogMeterHQ1->RedStartValue=limit.fRedPress*fK_convPr;

  Panel15 ->Caption=Str14+" "+strUnit;
  Map->StaticText1->Caption=strUnit;
  Series3->CustomVertAxis->Title->Caption="(Map)"+strUnit;
  Strumenti->Label9->Caption=strUnit;
  Main->Chart1->CustomAxes->Items[0]->Visible=Vis_Pmap;
  Main->Chart1->CustomAxes->Items[2]->Visible=Vis_T_K;
  Main->Chart1->CustomAxes->Items[3]->Visible=Vis_T_N;
  Main->Chart1->CustomAxes->Items[4]->Visible=Vis_cons;
  Main->Chart1->LeftAxis->Visible=Vis_P_R;
  Main->Chart1->RightAxis->Visible=Vis_C_R;


}



//debug




void TMain::ProcessData()
{
  static float Fcel=0;
  TVector pippoX;
  TVector pippoY;

  // VISUALIZZA DATI REAL TIME SE NON E' ATTIVA ANTEPRIMA DATI
  if(Out_dat)
  {
    //TENSIONE SENS TEMP esterna
    if(Unit_Tem ==1)
      sprintf(Buffer,"%-1.1f",ConvertTo(TAir,tuFahrenheit));
    else
      sprintf(Buffer,"%-1.1f",TAir);
    TatmLabel->Caption=Buffer;
    TAirLabel->Caption=Buffer;

    //TENSIONE SENS TEMP Oil
    if(Unit_Tem ==1)
      sprintf(Buffer,"%-1.1f",ConvertTo(TOil,tuFahrenheit));
    else
      sprintf(Buffer,"%-1.1f",TOil);
    Oil_Temp->AnalogMeterHQ1->Value=TOil;
    Oil_Temp->AnalogMeterHQ1->Caption=String(Buffer);
    OilLabel->Caption=Buffer;

    // PRESSIONE MANIFOLD
    if(Unit_Map==0)
      sprintf(Buffer,"%.f",Pmap * fK_convPr);
    else
      sprintf(Buffer,"%-1.1f",Pmap * fK_convPr);

    Map->AnalogMeterHQ1->Value=Pmap;
    Map->AnalogMeterHQ1->Caption=String(Buffer);
    MapLabel->Caption=Buffer;

    // pressione atmosferica    -20 -> OFFSET FISSO PER ALLINEAMENTO VALORE INDICATO
    sprintf(Buffer,"%.f",Val_pat);
    PatmLabel->Caption=Buffer;

    // TERMOCOPPIA
    if(V_T <=1000)
    {
      if(Unit_Tem ==1)
        sprintf(Buffer,"%-1.1f",ConvertTo(V_T,tuFahrenheit));
      else
        sprintf(Buffer,"%-1.1f",V_T);
      Termo->AnalogMeterHQ1->Value=V_T;
      Termo->AnalogMeterHQ1->Caption=Buffer;
      TKLabel->Caption=Buffer;
    }
    else
    {
      Termo->AnalogMeterHQ1->Value=0;
      Termo->AnalogMeterHQ1->Caption="0";
      TKLabel->Caption="0";
    }

    // CONSUMI
    if(!flagConsumi)
      CGauge3->Progress=0;
    else
    {
      CGauge3->Progress=usSommaImpulsiCons;
      //if(usSommaImpulsiCons>=MAX_PERC_CONS)
      {
        sprintf(Buffer,"%1.0f",fConsumoGKwh);
        ConsLabel->Caption = Buffer;
        sprintf(Buffer,"%1.2f",_CONSUMO);
        ConsLabel2->Caption = Buffer;
      }
    }

    Giri_Mot->AnalogMeterHQ1->Value=lRpm*Rap_tot1;
    GiriLabel->Caption=(int)(lRpm*Rap_tot1);
    Giri_Mot->AnalogMeterHQ1->Caption=String(lRpm*Rap_tot1);

    // COPPIA
    Coppia->AnalogMeterHQ1->Value= Val_coppia_m1 * fK_convC*fFact;
    sprintf(Buffer,"%-1.2f",Val_coppia_m1 * fK_convC*fFact);//;
    Coppia->AnalogMeterHQ1->Caption=Buffer;
    CopLabel->Caption=Buffer;

    // POTENZA
    Val_potenza_m1=(Val_coppia_m1*lRpm*Rap_tot1)/716.2;
    Potenza->AnalogMeterHQ1->Value= Val_potenza_m1*fK_convP*fFact;
    sprintf(Buffer,"%-1.2f",Val_potenza_m1*fK_convP*fFact);
    Potenza->AnalogMeterHQ1->Caption=Buffer;
    PotLabel->Caption=Buffer;


    if(OK_TEST_STEP || gbStepTestCancel)
    {
      Step_test->CGauge1->Progress=Tstep;
      Step_test->Label2->Caption=IntToStr(Reg_Stp)+" "+Str0;
      // Condizione di uscita dal test a step
      if(((rpmStart > rpmStop) && (Reg_Stp < rpmStop)) ||
          ((rpmStart < rpmStop) && (Reg_Stp > rpmStop)) ||
          gbStepTestCancel)
      {

        Step_test->BitBtn4->Enabled=true;
        Step_test->BitBtn3->Enabled=true;
        if(Step_test->Visible)
          Step_test->Close();


        StaticText2->Visible=false;
        Series105->Clear();
        Series106->Clear();
        Series107->Clear();
        Step_test->CGauge1->Progress=0;
        EndTest();


        Val_cop_medio=0;
        Val_reg_medio=0;
        Val_oil_medio=0;
        Val_map_medio=0;
        Val_k_medio=0;
        Tstep=0;
        OK_TEST_STEP = false;
        if(!gbStepTestCancel)
        {

          PrepareShowPerformance();
          CalcPerformance(Contatore);
          /// Aggiungo i dati del test nei vettori dati
          addTest();
          sbDataOut->Enabled=true;
          vTestEseguiti[iTestEseguiti]=iNTest;
          switch(iTestEseguiti)
          {
            case 0:
          t1->Visible=true;
          t1->Caption=vNomeTest[iNTest-1];
        break;
        case 1:
          t2->Visible=true;
          t2->Caption=vNomeTest[iNTest-1];
        break;
        case 2:
          t3->Visible=true;
          t3->Caption=vNomeTest[iNTest-1];
        break;
        case 3:
          t4->Visible=true;
          t4->Caption=vNomeTest[iNTest-1];
        break;
        case 4:
          t5->Visible=true;
          t5->Caption=vNomeTest[iNTest-1];
        break;
        case 5:
          t6->Visible=true;
          t6->Caption=vNomeTest[iNTest-1];
        break;
        case 6:
          t7->Visible=true;
          t7->Caption=vNomeTest[iNTest-1];
        break;
        case 7:
          t8->Visible=true;
          t8->Caption=vNomeTest[iNTest-1];
        break;
        }
    if(iTestEseguiti<8)
      iTestEseguiti++;
    bTestToExport=true;
          /// Salvo le impostazioni locali in vettore impostazioni
          salvaImpoDat(&datPrm[sNum_test-1]);
          Main->stopAndSaveDat();
          /// Visualizza i grafici,  le tabelle e il report laterale
          showPerformance(sNum_test-1);
          // Visualizza dati nei pannelli laterali e posiziona barra al valore max
          Visualizza_dat_test_strumenti(sNum_test-1,iPos);
          // abilita menu di salvataggio
          Salvamisurazione1->Enabled=true;
          // salvataggio automatico file temporaneo risultati
          Salvataggio_autom();
        }
        gbStepTestCancel=false;
        // imposta nome del test a pagina dati
        TabSheet2->Caption="Test"+IntToStr(sNum_test);
        TabSheet2->Highlighted=true;
        bSavedTest=false;
        Contatore=0;
        PageControl1->ActivePage=TabSheet1;
        Chart1->SetFocus();
      }


    }

   /****************************************************/
  if(Tipo_test==TEST_STEP_TIMED && bNotifyRTChart)
  {
    bNotifyRTChart=false;
    if(Contatore>0 && RPM_M[Contatore-1]>0)
    {
      Main->Series105->AddXY(RPM_M[Contatore-1],C_M[Contatore-1] * fK_convC*fFact,"",clTeeColor);
      Main->Series106->AddXY(RPM_M[Contatore-1],(RPM_M[Contatore-1]*C_M[Contatore-1]/716.2)*fK_convP*fFact,"",clTeeColor);
      if(Vis_cons)
        Main->Series107->AddXY(RPM_M[Contatore-1],fConsumoGKwh,"",clTeeColor);
    }
  }
    // GESTIONE TEST DECELERATIVO
    if(Tipo_test==TEST_DECEL)
    {
      if(Start_acq==1 && lRpm>0)
      {
         Main->Series105->AddXY(lRpm*Rap_tot1,Val_coppia_m1 * fK_convC*fFact,"",clTeeColor);
         Main->Series106->AddXY(lRpm*Rap_tot1,(lRpm*Rap_tot1*Val_coppia_m1/716.2)*fK_convP*fFact,"",clTeeColor);
         if(Vis_cons)
            Main->Series107->AddXY(lRpm*Rap_tot1,fConsumoGKwh,"",clTeeColor);
      }
      /// Condizione di uscita dal test decelerativo
      if((Start_acq==1) && (lRpm*Rap_tot1<= Reg_fine_dec))
      {
        /// PROVA TERMINATA, calcola le prestazioni
        Start_acq=false;
        Start->Close();
        EndTest();
        /// Chiude e salva i dati seriali della prova

        StaticText2->Visible=false;
        Series105->Clear();
        Series106->Clear();
        Series107->Clear();
        CalcPerformance(cntrIst);
        addTest();
          sbDataOut->Enabled=true;
          vTestEseguiti[iTestEseguiti]=iNTest;
          switch(iTestEseguiti)
          {
            case 0:
          t1->Visible=true;
          t1->Caption=vNomeTest[iNTest-1];
        break;
        case 1:
          t2->Visible=true;
          t2->Caption=vNomeTest[iNTest-1];
        break;
        case 2:
          t3->Visible=true;
          t3->Caption=vNomeTest[iNTest-1];
        break;
        case 3:
          t4->Visible=true;
          t4->Caption=vNomeTest[iNTest-1];
        break;
        case 4:
          t5->Visible=true;
          t5->Caption=vNomeTest[iNTest-1];
        break;
        case 5:
          t6->Visible=true;
          t6->Caption=vNomeTest[iNTest-1];
        break;
        case 6:
          t7->Visible=true;
          t7->Caption=vNomeTest[iNTest-1];
        break;
        case 7:
          t8->Visible=true;
          t8->Caption=vNomeTest[iNTest-1];
        break;
        }
    if(iTestEseguiti<8)
      iTestEseguiti++;
    bTestToExport=true;
        salvaImpoDat(&datPrm[sNum_test-1]);
        stopAndSaveDat();
        PrepareShowPerformance();
        showPerformance(sNum_test-1);
        Visualizza_dat_test_strumenti(sNum_test-1,iPos);
        // abilita menu di salvataggio
        Salvamisurazione1->Enabled=true;
        // salvataggio automatico file temporaneo risultati
        Salvataggio_autom();
        // imposta nome del test a pagina dati
        TabSheet2->Caption="Test"+IntToStr(sNum_test);
        TabSheet2->Highlighted=true;
        bSavedTest=false;
        PageControl1->ActivePage=TabSheet1;
        Chart1->SetFocus();
      }
    }



    // GESTIONE TEST A COPPIA COSTANTE
    if(Tipo_test==TEST_COST_TRQ && OK_N_cost)
    {
      // Invio comando PWM reference solo se sopra soglia minima RPM
      if(lRpm>Soglia_min && IO_ref)
      {
        // Indica che il PWM di riferimento non deve più essere inviato
        IO_ref=false;
        // forza da applicare sulla cella di carico (Kg)
        Fcel=(Valore_Cop/dynoPrm.bench.braccioReazCellaAnte);
        N_cost =((Fcel+cellPrm.readFase3A)*65535)/dynoPrm.bench.caricoMaxCellaAnte;
        // invia valore al freno
        if(g_brakeControl.CmdPwm(N_cost,Ki_integ,Ki_prop)==false)
          LogError(g_cDyn3Log,LOG_ERR,"CmdPwm failed!");
        else if(abs((int)(Valore_Cop-Val_coppia_m1))>5)
        {
          String sTmp;
          sTmp="Differenza Coppia Target - Coppia reale > 5Kg·m  ("+FloatToStr(Valore_Cop-Val_coppia_m1)+")";

          LogError(g_cFileLog,LOG_ERR,sTmp.c_str());
        }
      }
      // Se sono sotto soglia RPM devo fermare il controllo freno
      if(lRpm<Soglia_min && (!IO_ref))
      {
        IO_ref=true;
        // disattiva il freno
        EndTest();
      }
    }
  } // se visualizzazione real-time attiva
} //CHEKSUM CORRETTO

void __fastcall TMain::SpeedButton8Click(TObject *Sender)
{
Out_dat=true; // disattiva real-time
Tipo_test=1;
Caption=Str16;
SpeedButton8->Down=true;
SpeedButton4->Down=false;
SpeedButton15->Down=false;
SpeedButton17->Down=false;
SpeedButton21->Down=false;
Mod_imp=true; // ok per salvataggio
}
//---------------------------------------------------------------------------

void __fastcall TMain::SpeedButton4Click(TObject *Sender)
{

Out_dat=true; // disattiva real-time

Tipo_test=TEST_STEP_TIMED;
Caption=Str17;
SpeedButton4->Down=true;
SpeedButton8->Down=false;
SpeedButton15->Down=false;
SpeedButton17->Down=false;
SpeedButton21->Down=false;
Mod_imp=true; // ok per salvataggio

}
//---------------------------------------------------------------------------


void __fastcall TMain::SpeedButton15Click(TObject *Sender)
{
Out_dat=true; // disattiva real-time
Tipo_test=TEST_DECEL;  //2
Caption=Str19;
SpeedButton15->Down=true;
SpeedButton8->Down=false;
SpeedButton4->Down=false;
SpeedButton17->Down=false;
SpeedButton21->Down=false;
Mod_imp=true; // ok per salvataggio
}
//---------------------------------------------------------------------------



void __fastcall TMain::SpeedButton17Click(TObject *Sender)
{
Out_dat=true; // disattiva real-time
Tipo_test=TEST_BRAKE_MAN;
Caption=Str23;
SpeedButton17->Down=true;
SpeedButton8->Down=false;
SpeedButton4->Down=false;
SpeedButton15->Down=false;
SpeedButton21->Down=false;
Mod_imp=true; // ok per salvataggio
}

//---------------------------------------------------------------------------
void __fastcall TMain::SpeedButton21Click(TObject *Sender)
{
Out_dat=true; // disattiva real-time
Tipo_test=TEST_COST_TRQ;//4
Caption=Text44;
SpeedButton21->Down=true;
SpeedButton8->Down=false;
SpeedButton4->Down=false;
SpeedButton15->Down=false;
SpeedButton17->Down=false;
Mod_imp=true; // ok per salvataggio
}
//---------------------------------------------------------------------------



void __fastcall TMain::SpeedButton18Click(TObject *Sender)
{
  SpeedButton18->Enabled=false;
  Out_dat=true; // disattiva real-time
  Impostazioni->ShowModal();
}
//---------------------------------------------------------------------------



void __fastcall TMain::SpeedButton19Click(TObject *Sender)
{
 // se attivi li disattiva
 if( Vis_Rpm || 1)   Giri_Mot->Hide();
 if( Vis_Pot || 1)   Potenza->Hide();
 if( Vis_Cop || 1)   Coppia->Hide();
 if( Vis_Pres || 1)  Map->Hide();
 if( Vis_TemK || 1)  Termo->Hide();
 if( Vis_Oil || 1)   Oil_Temp->Hide();


 SpeedButton19->Glyph->LoadFromResourceName((int)HInstance, "Hide_All");
 SpeedButton19->Font->Color=clGreen;
 SpeedButton19->OnClick = SpeedButton19Click2;          
}
//---------------------------------------------------------------------------

void __fastcall TMain::SpeedButton19Click2(TObject *Sender)
{
  // se disattivi li attiva
  if( Vis_Rpm && 1)   Giri_Mot->Show();
  if( Vis_Pot && 1)   Potenza->Show();
  if( Vis_Cop && 1)   Coppia->Show();
  if( Vis_Pres && 1)  Map->Show();
  if( Vis_TemK && 1)  Termo->Show();
  if( Vis_Oil && 1)   Oil_Temp->Show();
  SpeedButton19->Glyph->LoadFromResourceName((int)HInstance, "Show_All");
  SpeedButton19->Font->Color=clRed;
  SpeedButton19->OnClick = SpeedButton19Click;
}

void __fastcall TMain::Azzera_var()
{
  // azzera tutte le variabili utilizzate nel test di potenza
  Cont_acq=0;
  memset(RPM_M,0,sizeof(RPM_M));
  memset(C_M,0,sizeof(C_M));
  memset(C_IST,0,sizeof(C_IST));
  memset(RPM_IST,0,sizeof(RPM_IST));
  memset(fTermo_K,0,sizeof(fTermo_K));
  memset(fTermo_Oil,0,sizeof(fTermo_Oil));
  memset(fPres_MAP,0,sizeof(fPres_MAP));
  memset(fvettConsTot,0,sizeof(fvettConsTot));
  memset(vettCons,0,sizeof(vettCons));
  Series105->Clear();
  Series106->Clear();
  Series107->Clear();
} // fine Azzera_var() ---------







void __fastcall TMain::CheckBox1ContextPopup(TObject *Sender,
      TPoint &MousePos, bool &Handled)
{
PopupMenu=PopupMenu1;
}
//---------------------------------------------------------------------------

void __fastcall TMain::Panel33CanResize(TObject *Sender, int &NewWidth,
      int &NewHeight, bool &Resize)
{
 // ridimensiona il caption width del testo del test
  CheckBox1->Width=Panel33->Width-10;
  CheckBox2->Width=Panel33->Width-10;
  CheckBox3->Width=Panel33->Width-10;
  CheckBox4->Width=Panel33->Width-10;
  CheckBox5->Width=Panel33->Width-10;
  CheckBox6->Width=Panel33->Width-10;
  CheckBox7->Width=Panel33->Width-10;
  CheckBox8->Width=Panel33->Width-10;

  Bevel12->Width=Panel33->Width-2;


}
//---------------------------------------------------------------------------


void __fastcall TMain::CheckBox1Click(TObject *Sender)
{

  if(CheckBox1->Checked)
  {
    if(Vis_C_R)Series10->Visible=true; // coppia ruota
    if(Vis_P_R)Series9->Visible=true;  // potenza ruota
    if(Vis_Pmap) Series3->Visible=true;
    if(Vis_T_K)  Series6->Visible=true;
    if(Vis_T_N)  Series7->Visible=true;
    if(Vis_cons) Series15->Visible=true;  
  }
  else
  {
    if(Vis_C_R)Series10->Visible=false;
    if(Vis_P_R)Series9->Visible=false;
    if(Vis_Pmap) Series3->Visible=false;
    if(Vis_T_K)  Series6->Visible=false;
    if(Vis_T_N)  Series7->Visible=false;
    if(Vis_cons) Series15->Visible=false;
  }
  graphEnabled[0]=CheckBox1->Checked;
  updateRichText();
}
//---------------------------------------------------------------------------

void __fastcall TMain::CheckBox2Click(TObject *Sender)
{
  if(CheckBox2->Checked)
  {
    if(Vis_C_R)Series21->Visible=true;  // coppia ruota
    if(Vis_P_R)Series35->Visible=true;  // potenza ruota
    if(Vis_Pmap) Series60->Visible=true;
    if(Vis_R_AB) Series67->Visible=true;
    if(Vis_T_K)  Series81->Visible=true;
    if(Vis_T_N)  Series88->Visible=true;
    if(Vis_cons) Series16->Visible=true;
  }
  else
  {
    if(Vis_C_R)Series21->Visible=false;
    if(Vis_P_R)Series35->Visible=false;
    if(Vis_Pmap) Series60->Visible=false;
    if(Vis_R_AB) Series67->Visible=false;
    if(Vis_T_K)  Series81->Visible=false;
    if(Vis_T_N)  Series88->Visible=false;
    if(Vis_cons) Series16->Visible=false;
  }
  graphEnabled[1]=CheckBox2->Checked;
  updateRichText();
}
//---------------------------------------------------------------------------

void __fastcall TMain::CheckBox3Click(TObject *Sender)
{

  if(CheckBox3->Checked)
  {
    if(Vis_C_R)Series22->Visible=true;  // coppia ruota
    if(Vis_P_R)Series36->Visible=true;  // potenza ruota
    if(Vis_Pmap) Series61->Visible=true;
    if(Vis_T_K)  Series82->Visible=true;
    if(Vis_T_N)  Series89->Visible=true;
    if(Vis_cons) Series17->Visible=true;
  }
  else
  {
    if(Vis_C_R)Series22->Visible=false;
    if(Vis_P_R)Series36->Visible=false;
    if(Vis_Pmap) Series61->Visible=false;
    if(Vis_T_K)  Series82->Visible=false;
    if(Vis_T_N)  Series89->Visible=false;
    if(Vis_cons) Series17->Visible=false;
  }
  graphEnabled[2]=CheckBox3->Checked;
  updateRichText(); 
}
//---------------------------------------------------------------------------

void __fastcall TMain::CheckBox4Click(TObject *Sender)
{
  if(CheckBox4->Checked)
  {
    if(Vis_C_R)Series23->Visible=true;  // coppia ruota
    if(Vis_P_R)Series37->Visible=true;  // potenza ruota
    if(Vis_Pmap) Series62->Visible=true;
    if(Vis_T_K)  Series83->Visible=true;
    if(Vis_T_N)  Series90->Visible=true;
    if(Vis_cons) Series18->Visible=true;
  }
  else
  {
    if(Vis_C_R)Series23->Visible=false;
    if(Vis_P_R)Series37->Visible=false;
    if(Vis_Pmap) Series62->Visible=false;
    if(Vis_T_K)  Series83->Visible=false;
    if(Vis_T_N)  Series90->Visible=false;
    if(Vis_cons) Series18->Visible=false;
  }
  graphEnabled[3]=CheckBox4->Checked;
  updateRichText();
}
//---------------------------------------------------------------------------

void __fastcall TMain::CheckBox5Click(TObject *Sender)
{
  if(CheckBox5->Checked) //5
  {
    if(Vis_C_R)  Series24->Visible=true;  // coppia ruota
    if(Vis_P_R)  Series38->Visible=true;  // potenza ruota
    if(Vis_Pmap) Series63->Visible=true;
    if(Vis_T_K)  Series84->Visible=true;
    if(Vis_T_N)  Series91->Visible=true;
    if(Vis_cons) Series28->Visible=true;
  }
  else
  {
    if(Vis_C_R)Series24->Visible=false;
    if(Vis_P_R)Series38->Visible=false;
    if(Vis_Pmap) Series63->Visible=false;
    if(Vis_T_K)  Series84->Visible=false;
    if(Vis_T_N)  Series91->Visible=false;
    if(Vis_cons) Series28->Visible=false;
  }
  graphEnabled[4]=CheckBox5->Checked;
  updateRichText();  
}
//---------------------------------------------------------------------------

void __fastcall TMain::CheckBox6Click(TObject *Sender)
{
  if(CheckBox6->Checked)
  {
    if(Vis_C_R)Series25->Visible=true;  // coppia ruota
    if(Vis_P_R)Series39->Visible=true;  // potenza ruota
    if(Vis_Pmap) Series64->Visible=true;
    if(Vis_T_K)  Series85->Visible=true;
    if(Vis_T_N)  Series92->Visible=true;
    if(Vis_cons) Series29->Visible=true;
  }
  else
  {
    if(Vis_C_R)Series25->Visible=false;
    if(Vis_P_R)Series39->Visible=false;
    if(Vis_Pmap) Series64->Visible=false;
    if(Vis_T_K)  Series85->Visible=false;
    if(Vis_T_N)  Series92->Visible=false;
    if(Vis_cons) Series29->Visible=false;
  }
  graphEnabled[5]=CheckBox6->Checked;
  updateRichText();
}
//---------------------------------------------------------------------------

void __fastcall TMain::CheckBox7Click(TObject *Sender)
{
  if(CheckBox7->Checked)
  {
    if(Vis_C_R)Series26->Visible=true;  // coppia ruota
    if(Vis_P_R)Series40->Visible=true;  // potenza ruota
    if(Vis_Pmap) Series65->Visible=true;
    if(Vis_T_K)  Series86->Visible=true;
    if(Vis_T_N)  Series93->Visible=true;
    if(Vis_cons) Series30->Visible=true;
  }
  else
  {
    if(Vis_C_R)Series26->Visible=false;
    if(Vis_P_R)Series40->Visible=false;
    if(Vis_Pmap) Series65->Visible=false;
    if(Vis_T_K)  Series86->Visible=false;
    if(Vis_T_N)  Series93->Visible=false;
    if(Vis_cons) Series30->Visible=false;
  }
  graphEnabled[6]=CheckBox7->Checked;
  updateRichText();  
}
//---------------------------------------------------------------------------

void __fastcall TMain::CheckBox8Click(TObject *Sender)
{
  if(CheckBox8->Checked)
  {
    if(Vis_C_R)Series27->Visible=true;  // coppia ruota
    if(Vis_P_R)Series41->Visible=true;  // potenza ruota
    if(Vis_Pmap) Series66->Visible=true;
    if(Vis_T_K)  Series87->Visible=true;
    if(Vis_T_N)  Series94->Visible=true;
    if(Vis_cons) Series31->Visible=true;
  }
  else
  {
    if(Vis_C_R)Series27->Visible=false;
    if(Vis_P_R)Series41->Visible=false;
    if(Vis_Pmap) Series66->Visible=false;
    if(Vis_T_K)  Series87->Visible=false;
    if(Vis_T_N)  Series94->Visible=false;
    if(Vis_cons) Series31->Visible=false;
  }
  graphEnabled[7]=CheckBox8->Checked;
  updateRichText();  
}
//---------------------------------------------------------------------------

void __fastcall TMain::Rinominailtest1Click(TObject *Sender)
{
  AnsiString NewString = CheckBox1->Caption;
  if (InputQuery("Input Box", Str24, NewString))
  {
    CheckBox1->Caption=NewString;
    CheckBox1->GetTextBuf(vNomeTest[0],sizeof(vNomeTest[0]));
    // aggiorna i dati dei test
    Tabella_test_1->Caption=vNomeTest[0];
    updateRichText();
  }
}
//---------------------------------------------------------------------------

void __fastcall TMain::MenuItem2Click(TObject *Sender)
{
  AnsiString NewString = CheckBox2->Caption;
  if (InputQuery("Input Box", Str24, NewString))
  {    
    CheckBox2->Caption=NewString;  
    CheckBox2->GetTextBuf(vNomeTest[1],sizeof(vNomeTest[1]));  
    // aggiorna i dati dei test
    Tabella_test_2->Caption=vNomeTest[1];
    updateRichText();
  }
}
//---------------------------------------------------------------------------

void __fastcall TMain::CheckBox2ContextPopup(TObject *Sender,
      TPoint &MousePos, bool &Handled)
{
PopupMenu=PopupMenu2;
}
//---------------------------------------------------------------------------


void __fastcall TMain::Rinominailtest3Click(TObject *Sender)
{
  AnsiString NewString = CheckBox4->Caption;
  if (InputQuery("Input Box", Str24, NewString))
  {
    CheckBox4->Caption=NewString;
    CheckBox4->GetTextBuf(vNomeTest[3],sizeof(vNomeTest[3]));
    // aggiorna i dati dei test
    Tabella_test_4->Caption=vNomeTest[3];
    updateRichText();
  }
}
//---------------------------------------------------------------------------

void __fastcall TMain::Rinominailtest4Click(TObject *Sender)
{
  AnsiString NewString = CheckBox5->Caption;
  if (InputQuery("Input Box", Str24, NewString))
  {    
    CheckBox5->Caption=NewString;
    CheckBox5->GetTextBuf(vNomeTest[4],sizeof(vNomeTest[4]));
    // aggiorna i dati dei test
    Tabella_test_5->Caption=vNomeTest[4];
    updateRichText();
  }
}
//---------------------------------------------------------------------------

void __fastcall TMain::Rinominailtest5Click(TObject *Sender)
{
  AnsiString NewString = CheckBox6->Caption;
  if (InputQuery("Input Box", Str24, NewString))
  {
    CheckBox6->Caption=NewString;
    CheckBox6->GetTextBuf(vNomeTest[5],sizeof(vNomeTest[5]));
    // aggiorna i dati dei test
    Tabella_test_6->Caption=vNomeTest[5];
    updateRichText();
  }
}
//---------------------------------------------------------------------------

void __fastcall TMain::Rinominailtest6Click(TObject *Sender)
{
  AnsiString NewString = CheckBox7->Caption;
  if (InputQuery("Input Box", Str24, NewString))
  {
    CheckBox7->Caption=NewString;
    CheckBox7->GetTextBuf(vNomeTest[6],sizeof(vNomeTest[6]));
    // aggiorna i dati dei test
    Tabella_test_7->Caption=vNomeTest[6];
    updateRichText();
  }
}
//---------------------------------------------------------------------------

void __fastcall TMain::Rinominailtest7Click(TObject *Sender)
{
  AnsiString NewString = CheckBox8->Caption;
  if (InputQuery("Input Box", Str24, NewString))
  {
    CheckBox8->Caption=NewString;
    CheckBox8->GetTextBuf(vNomeTest[7],sizeof(vNomeTest[7]));
    // aggiorna i dati dei test
    Tabella_test_8->Caption=vNomeTest[7];
    updateRichText();
  }
}
//---------------------------------------------------------------------------



void __fastcall TMain::CheckBox3ContextPopup(TObject *Sender,
      TPoint &MousePos, bool &Handled)
{
PopupMenu=PopupMenu3;
}
//---------------------------------------------------------------------------

void __fastcall TMain::CheckBox4ContextPopup(TObject *Sender,
      TPoint &MousePos, bool &Handled)
{
PopupMenu=PopupMenu4;
}
//---------------------------------------------------------------------------

void __fastcall TMain::CheckBox5ContextPopup(TObject *Sender,
      TPoint &MousePos, bool &Handled)
{
PopupMenu=PopupMenu5;
}
//---------------------------------------------------------------------------

void __fastcall TMain::CheckBox6ContextPopup(TObject *Sender,
      TPoint &MousePos, bool &Handled)
{
  PopupMenu=PopupMenu6;
}
//---------------------------------------------------------------------------

void __fastcall TMain::CheckBox7ContextPopup(TObject *Sender,
  TPoint &MousePos, bool &Handled)
{
  PopupMenu=PopupMenu7;
}
//---------------------------------------------------------------------------

void __fastcall TMain::CheckBox8ContextPopup(TObject *Sender,
  TPoint &MousePos, bool &Handled)
{
PopupMenu=PopupMenu8;
}
//---------------------------------------------------------------------------

void __fastcall TMain::Eliminadefinitivamenteiltest1Click(TObject *Sender)
{
  MessageBeep(MB_ICONEXCLAMATION);
  if(MessageDlg(Str25,mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
  {
    // cancella tutte le serie del test 1
    Series10->Clear();
    Series9->Clear();
    Series3->Clear();    
    Series6->Clear();
    Series7->Clear();
    Series15->Clear();
    graphEnabled[0]=false;    
    vNumTest[1]=0; // elimina test
    CheckBox1->Visible=false;
    CheckBox1->Caption="Test1";
    CheckBox1->Checked=true;
    Tabella_test_1->TabVisible=false;
    updateRichText();
    updateGrids();
    Def_rapporto();
    if(sTest_antep==0)
      azzeraPannelli();
  }
}
//---------------------------------------------------------------------------

void __fastcall TMain::Rinominailtest2Click(TObject *Sender)
{
  AnsiString NewString = CheckBox3->Caption;
  if (InputQuery("Input Box", Str24, NewString))
  {
    CheckBox3->Caption=NewString;
    CheckBox3->GetTextBuf(vNomeTest[2],sizeof(vNomeTest[2]));
    // aggiorna i dati dei test
    Tabella_test_3->Caption=vNomeTest[2];
    updateRichText();
  }
}
//---------------------------------------------------------------------------

void __fastcall TMain::MenuItem1Click(TObject *Sender)
{
  MessageBeep(MB_ICONEXCLAMATION);
  if(MessageDlg(Str25,mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
  {
    // cancella tutte le serie del test 2
    Series21->Clear();
    Series35->Clear();
    Series60->Clear();
    Series67->Clear();
    Series81->Clear();
    Series88->Clear();
    Series16->Clear();
    graphEnabled[1]=false;
    vNumTest[2]=0; // elimina test
    CheckBox2->Visible=false;
    CheckBox2->Checked=true;
    CheckBox2->Caption="Test2";
    Tabella_test_2->TabVisible=false;
    updateRichText();
    updateGrids();
    Def_rapporto();
    if(sTest_antep==1)
      azzeraPannelli();
  }
}
//---------------------------------------------------------------------------

void __fastcall TMain::Eliminadefinitivamenteiltest2Click(TObject *Sender)
{
  MessageBeep(MB_ICONEXCLAMATION);

  if(MessageDlg(Str25,mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
  {
    // cancella tutte le serie del test 3
    Series22->Clear();
    Series36->Clear();
    Series89->Clear();
    Series82->Clear();
    Series61->Clear();
    Series17->Clear();
    graphEnabled[2]=false;
    vNumTest[3]=0; // elimina test
    CheckBox3->Visible=false;
    CheckBox3->Checked=true;
    CheckBox3->Caption="Test3";
    Tabella_test_3->TabVisible=false;
    updateRichText();
    updateGrids();
    Def_rapporto();
    if(sTest_antep==2)
      azzeraPannelli();
  }
}
//---------------------------------------------------------------------------

void __fastcall TMain::Eliminadefinitivamenteiltest3Click(TObject *Sender)
{
  MessageBeep(MB_ICONEXCLAMATION);
  if(MessageDlg(Str25,mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
  {
    // cancella tutte le serie del test 4
    Series23->Clear();
    Series37->Clear();
    Series62->Clear();
    Series83->Clear();
    Series90->Clear();
    Series18->Clear();
    graphEnabled[3]=false;
    vNumTest[4]=0; // elimina test
    CheckBox4->Visible=false;
    CheckBox4->Checked=true;
    CheckBox4->Caption="Test4";
    Tabella_test_4->TabVisible=false;
    updateRichText();
    updateGrids();
    Def_rapporto();
    if(sTest_antep==3)
      azzeraPannelli();
  }
}
//---------------------------------------------------------------------------

void __fastcall TMain::Eliminadefinitivamenteiltest4Click(TObject *Sender)
{
  MessageBeep(MB_ICONEXCLAMATION);
  if(MessageDlg(Str25,mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
  {
    // cancella tutte le serie del test 5
    Series24->Clear();
    Series38->Clear();
    Series91->Clear();
    Series84->Clear();
    Series63->Clear();
    Series28->Clear();
    graphEnabled[4]=false;
    vNumTest[5]=0; // elimina test
    CheckBox5->Visible=false;
    CheckBox5->Checked=true;
    CheckBox5->Caption="Test5";
    Tabella_test_5->TabVisible=false;
    updateRichText();
    updateGrids();
    Def_rapporto();
    if(sTest_antep==4)
      azzeraPannelli();

  }
}
//---------------------------------------------------------------------------

void __fastcall TMain::Eliminadefinitivamenteiltest5Click(TObject *Sender)
{
  MessageBeep(MB_ICONEXCLAMATION);
  if(MessageDlg(Str25,mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
  {
    // cancella tutte le serie del test 6
    Series25->Clear();
    Series39->Clear();
    Series64->Clear();
    Series85->Clear();
    Series92->Clear();
    Series29->Clear();
    graphEnabled[5]=false;    
    vNumTest[6]=0; // elimina test
    CheckBox6->Visible=false;
    CheckBox6->Checked=true;
    CheckBox6->Caption="Test6";
    Tabella_test_6->TabVisible=false;
    updateRichText();
    updateGrids();
    Def_rapporto();
    if(sTest_antep==5)
      azzeraPannelli();    
  }
}
//---------------------------------------------------------------------------

void __fastcall TMain::Eliminadefinitivamenteiltest6Click(TObject *Sender)
{
  MessageBeep(MB_ICONEXCLAMATION);
  if(MessageDlg(Str25,mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
  {
    // cancella tutte le serie del test 7
    Series26->Clear();
    Series40->Clear();
    Series93->Clear();
    Series86->Clear();
    Series65->Clear();
    Series30->Clear();
    graphEnabled[6]=false;
    vNumTest[7]=0; // elimina test
    CheckBox7->Visible=false;
    CheckBox7->Checked=true;
    CheckBox7->Caption="Test7";
    Tabella_test_7->TabVisible=false;
    updateRichText();
    updateGrids();
    Def_rapporto();
    if(sTest_antep==6)
      azzeraPannelli();
  }
}
//---------------------------------------------------------------------------

void __fastcall TMain::Eliminadefinitivamenteiltest7Click(TObject *Sender)
{
  MessageBeep(MB_ICONEXCLAMATION);
  if(MessageDlg(Str25,mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
  {
    // cancella tutte le serie del test 8
    Series27->Clear();
    Series41->Clear();
    Series66->Clear();
    Series87->Clear();
    Series94->Clear();
    Series31->Clear();
    graphEnabled[7]=false;
    vNumTest[8]=0; // elimina test
    CheckBox8->Visible=false;
    CheckBox8->Checked=true;
    CheckBox8->Caption="Test8";
    Tabella_test_8->TabVisible=false;
    updateRichText();
    updateGrids();
    Def_rapporto();
    if(sTest_antep==7)
      azzeraPannelli();
  }
}
//---------------------------------------------------------------------------


void __fastcall TMain::Nuovasessioneditest1Click(TObject *Sender)
{

  ChartTool9->Visible=false;
  if(Salvamisurazione1->Enabled==true && bSavedTest==false)
  {
    MessageBeep(MB_ICONEXCLAMATION);
    if(MessageDlg(Str9,mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
    {
      Salvamisurazione1Click(this);
    }
  }

  gbStepTestCancel=false;
  Series105->Clear();
  Series106->Clear();
  Series107->Clear();
  NextGrid9->ClearRows();
  NextGrid11->ClearRows();
  NextGrid12->ClearRows();
  //primaVoltaTabStep=1;
  TabSheet11->TabVisible=false;
  TabSheet7->TabVisible=false;
  TabSheet8->TabVisible=false;
  //rowTabStepF1=1;
  //rowTabStepF2=1;
  //rowTabStepF4=1;
  OKF=true;
  if(sNum_test) // solo se presente qualche test
  {
    MessageBeep(MB_ICONEXCLAMATION);   // richiesta di cancellazione memoria...
    if(MessageDlg(Str26,mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
    {

#ifdef _INDUSTRY4
      bTestToExport=false;
      memset(vTestEseguiti,0,sizeof(vTestEseguiti));
      iTestEseguiti=0;
      t1->Visible=false;
      t2->Visible=false;
      t3->Visible=false;
      t4->Visible=false;
      t5->Visible=false;
      t6->Visible=false;
      t7->Visible=false;
      t8->Visible=false;
      sbDataIn->Enabled=true;
      sbDataOut->Enabled=false;
      Savetest1->Enabled=false;
#endif

      Salvamisurazione1->Enabled=false;

      OKF=true;
      // cancella tutte le serie del test 1
      for (int i=0;i<Chart1->SeriesCount()-1;i++)
      {
        Chart1->Series[i]->Clear();
        Chart1->Series[i]->ShowInLegend=false;
      }
      memset(graphEnabled,0,sizeof(graphEnabled));
      CheckBox1->Visible=false;
      CheckBox1->Checked=true;
      CheckBox1->Caption="Test1";

      CheckBox2->Visible=false;
      CheckBox2->Checked=true;
      CheckBox2->Caption="Test2";
      
      CheckBox3->Visible=false;
      CheckBox3->Checked=true;
      CheckBox3->Caption="Test3";
      
      CheckBox4->Visible=false;
      CheckBox4->Checked=true;
      CheckBox4->Caption="Test4";
      
      CheckBox5->Visible=false;
      CheckBox5->Checked=true;
      CheckBox5->Caption="Test5";
      
      CheckBox6->Visible=false;
      CheckBox6->Checked=true;
      CheckBox6->Caption="Test6";
      
      CheckBox7->Visible=false;
      CheckBox7->Checked=true;
      CheckBox7->Caption="Test7";
      
      CheckBox8->Visible=false;
      CheckBox8->Checked=true; 
      CheckBox8->Caption="Test8";
      if(bPidControl==false)
        Panel33->Visible=false;

      // oscura tabelle
      Main->TabSheet5->TabVisible=false;      
      Main->PageControl1->ActivePageIndex=0;            
      // inizializza il ciclo di test
      sNum_test=0;
      memset(vNumTest,0,sizeof(vNumTest));
      // cancella la presenza test selettivo in memoria
      // 0-> vuoto  1-> presente in memoria

      // cancellazione di tutte le variabili
      memset(vNoteTest,0,sizeof(vNoteTest));
      memset(vTermoK,0,sizeof(vTermoK));
      memset(vTermoOil,0,sizeof(vTermoOil));
      memset(vPresMAP,0,sizeof(vPresMAP));
      memset(vCoppiaRuota,0,sizeof(vCoppiaRuota));
      memset(vCons,0,sizeof(vCons));
      memset(vConsTot,0,sizeof(vConsTot));
      memset(vPotenzaRuota,0,sizeof(vPotenzaRuota));
      memset(vGiri,0,sizeof(vGiri));
      memset(datPrm,0,sizeof(datPrm));
      memset(vNomeTest,0,sizeof(vNomeTest));
      // aggiorna i dati dei test
      Tabella_test_1->TabVisible=false;
      Tabella_test_2->TabVisible=false;
      Tabella_test_3->TabVisible=false;
      Tabella_test_4->TabVisible=false;
      Tabella_test_5->TabVisible=false;
      Tabella_test_6->TabVisible=false;
      Tabella_test_7->TabVisible=false;
      Tabella_test_8->TabVisible=false;
      updateGrids();
      updateRichText();
    }  // se risponde Si
  } // if(sNum_test)


  Out_dat=true; // attiva visualizzazione real time
  iPos=0;
  ChartTool9->Value=0;
  TabSheet2->Caption=Text90;
  TabSheet2->Highlighted=false;
}
//---------------------------------------------------------------------------





void __fastcall TMain::Timer1Timer(TObject *Sender)
{

  /*
  0->Str180=SAE J1995
  1->Str190=ISO TR 14396
  2->Str200=CE 97/68-2000/25/CE
  3->Str210=ECE R24
  4->Str220=CEE 80/1269
  5->Str230=DIN 70020
  6->Str235=OCDE
  7->Manuale
  */
  // contiene tipo norma di correzione:
  // 0 -> DIN 70020
  // 1 -> EWG 80/1269
  // 2 -> ISO 1585
  // 3 -> SAE J1349
  // 4 -> JIS D1001
  // 5 -> NESSUNA

  switch(Tipo_norm)
  {
    case 0: // SAE J1995
      if(Val_pat)
              fFact = 1013/Val_pat * pow(((273+TAir)/293),0.5);
      else
              fFact = 1;
      fFact = fFact * K_SAEJ;
      sprintf(Buffer,"%-1.3f",fFact);
      FcLabel->Caption=Buffer;
    break;
    case 1: // ISO TR 14396
      if(Val_pat)
              fFact = 1013/Val_pat * pow(((273+TAir)/293),0.5);
      else
              fFact = 1;
      fFact = fFact * K_ISO9768;
      sprintf(Buffer,"%-1.3f",fFact);
      FcLabel->Caption=Buffer;
    break;
    case 2: // CE 97/68-2000/25/CE
      if(Val_pat)
              fFact = 1013/Val_pat * pow(((273+TAir)/293),0.5);
      else
              fFact = 1;
      fFact = fFact * K_ISO9768;
      sprintf(Buffer,"%-1.3f",fFact);
      FcLabel->Caption=Buffer;
    break;
    case 3: // ECE R24
      if(Val_pat)
              fFact = 1013/Val_pat * pow(((273+TAir)/293),0.5);
      else
              fFact = 1;
      fFact = fFact * K_ECER24;
      sprintf(Buffer,"%-1.3f",fFact);
      FcLabel->Caption=Buffer;
    break;  
    case 4: // 80/1269
      if(Val_pat)
              fFact = 1013/Val_pat * pow(((273+TAir)/293),0.5);
      else
              fFact = 1;
      fFact = fFact * K_ECER24;
      sprintf(Buffer,"%-1.3f",fFact);
      FcLabel->Caption=Buffer;
    break;
    case 6: // OCDE
  
      fFact = 1;
      sprintf(Buffer,"%-1.3f",fFact);
      FcLabel->Caption=Buffer;
    break;  
    case 5: // DIN 70020
      if(Val_pat)
        fFact = 1013/Val_pat * pow(((273+TAir)/293),0.5);
      else
        fFact = 1;
      sprintf(Buffer,"%-1.3f",fFact);
      FcLabel->Caption=Buffer;
    break;  
    case 7:   // manuale
      sprintf(Buffer,"%-1.3f",fFact);
      FcLabel->Caption=Buffer;
    break;
  }
  // visualizza umidità
  HrLabel->Caption= Val_Hr;
}

//---------------------------------------------------------------------------
bool TMain::DirectoryExists( const char* pzPath )
{
  if ( pzPath == NULL) return false; 
  
  DIR *pDir;
  bool bExists = false; 
  
  pDir = opendir (pzPath); 
  
  if (pDir != NULL) 
  { 
    bExists = true;
    (void) closedir (pDir); 
  }   
  return bExists;
}
//---------------------------------------------------------------------------

void __fastcall TMain::Salvamisurazione1Click(TObject *Sender)
{
  String saveDir=utils.GetSaveDir();
  // ***** raccoglie informazioni titoli dei test memorizzati ***********
  CheckBox1->GetTextBuf(vNomeTest[0],sizeof(vNomeTest[0]));
  CheckBox2->GetTextBuf(vNomeTest[1],sizeof(vNomeTest[1]));
  CheckBox3->GetTextBuf(vNomeTest[2],sizeof(vNomeTest[2]));
  CheckBox4->GetTextBuf(vNomeTest[3],sizeof(vNomeTest[3]));
  CheckBox5->GetTextBuf(vNomeTest[4],sizeof(vNomeTest[4]));
  CheckBox6->GetTextBuf(vNomeTest[5],sizeof(vNomeTest[5]));
  CheckBox7->GetTextBuf(vNomeTest[6],sizeof(vNomeTest[6]));
  CheckBox8->GetTextBuf(vNomeTest[7],sizeof(vNomeTest[7]));
  // controlla se i file sono stati salvati
  if(Sender==NULL)
    saveDir=utils.GetSaveDir()+"\\temp.dyn";
  else if (saveDir!="")
  {
    SaveDialog1->InitialDir=saveDir;
    if(SaveDialog1->Execute())
      saveDir=SaveDialog1->FileName;
    else
      saveDir="";
  }
  if(saveDir!="")
  {
    ofstream out(saveDir.c_str(), ios::out | ios::binary);  // salva la prova...
    // salva i test
    cDynVersion=0x01;
    out.write((char *)Costruttore, sizeof (Costruttore));
    out.write((char *)Modello, sizeof (Modello));
    out.write((char *)Allestimento, sizeof (Allestimento));
    for(int i=0;i<8;i++)
    {
      out.write((char *)vNomeTest[i], sizeof (vNomeTest[i]));
      if(i==0)
        out.write((unsigned char *)vNumTest , sizeof (vNumTest));
      out.write((unsigned char *)&datPrm[i] , sizeof (datPrm[i]));
      out.write((unsigned char *)vGiri[i], sizeof (vGiri[i]));
      out.write((unsigned char *)vCoppiaRuota[i] , sizeof (vCoppiaRuota[i]));
      out.write((unsigned char *)vCoppiaRuota[i] , sizeof (vCoppiaRuota[i]));
      out.write((unsigned char *)vPotenzaRuota[i] , sizeof (vPotenzaRuota[i]));
      out.write((unsigned char *)vPotenzaRuota[i] , sizeof (vPotenzaRuota[i]));
      out.write((unsigned char *)vPotenzaRuota[i] , sizeof (vPotenzaRuota[i]));
      out.write((unsigned char *)vPotenzaRuota[i] , sizeof (vPotenzaRuota[i]));
      out.write((unsigned char *)vPotenzaRuota[i] , sizeof (vPotenzaRuota[i]));
      out.write((unsigned char *)vTermoK[i] , sizeof (vTermoK[i]));
      out.write((unsigned char *)vTermoOil[i] , sizeof (vTermoOil[i]));
      out.write((unsigned char *)vPresMAP[i] , sizeof (vPresMAP[i]));
      out.write((unsigned char *)vConsTot[i] , sizeof (vConsTot[i]));
      out.write((unsigned char *)vConsTot[i] , sizeof (vConsTot[i]));
      out.write((char *)vNoteTest[i], sizeof (vNoteTest[i]));
      out.write((unsigned char *)vCons[i] , sizeof (vCons[i]));
    }
    out.write((char *)Targa, sizeof (Targa));
    out.write((char *)Cliente, sizeof (Cliente));
#if 0
    out.write((unsigned char*)tabStepGiri,sizeof(tabStepGiri));
    out.write((unsigned char*)tabStepCoppia,sizeof(tabStepCoppia));
    out.write((unsigned char*)tabStepPot,sizeof(tabStepPot));
    out.write((unsigned char*)tabStepCons,sizeof(tabStepCons));
    out.write((unsigned char*)tabStepLamb,sizeof(tabStepLamb));
    out.write((unsigned char*)tabStepMap,sizeof(tabStepMap));
    out.write((unsigned char*)tabStepTk,sizeof(tabStepTk));
#endif
    out.close();
    SS=saveDir;
    if(Sender!=NULL)
      bSavedTest=true;
    // aggiorna elenco test eseguiti/disponibili
    Carica_File->DirectoryListBox1->Update();
    Carica_File->FileListBox1->Update();
  }
}
//---------------------------------------------------------------------------

void __fastcall TMain::PrepareShowPerformance()
{
  /// Set preliminari alla traccia dei grafici
  Out_dat=false; // disattiva real-time
  Panel33->Visible=true;
  // attiva possibilità di stampa dati
  Stampadeigrafici1->Enabled=true;
  Stampadatideitest1->Enabled=true;
  TabSheet5->TabVisible=true;
  // cancella grafici di cattura real time......
  Series105->Clear();
  Series106->Clear();
  Series107->Clear();
  if(Opzioni_grafici->cbPZeroScale->Checked)
  {
    Main->Chart1->LeftAxis->Automatic=false;
    Main->Chart1->LeftAxis->AutomaticMaximum=true;
    Main->Chart1->LeftAxis->AutomaticMinimum=false;
    Main->Chart1->LeftAxis->Minimum=0;
  }
  else
  {
    Main->Chart1->LeftAxis->Automatic=true;
    Main->Chart1->LeftAxis->AutomaticMaximum=true;
    Main->Chart1->LeftAxis->AutomaticMinimum=true;
  }
  if(Opzioni_grafici->cbCZeroScale->Checked)
  {
    Main->Chart1->RightAxis->Automatic=false;
    Main->Chart1->RightAxis->AutomaticMaximum=true;
    Main->Chart1->RightAxis->AutomaticMinimum=false;
    Main->Chart1->RightAxis->Minimum=0;
  }
  else
  {
    Main->Chart1->RightAxis->Automatic=true;
    Main->Chart1->RightAxis->AutomaticMaximum=true;
    Main->Chart1->RightAxis->AutomaticMinimum=true;
  }
}

int TMain::addTest()
{
  static int i;
  /// Determina il posto libero nell'array elenco dei test
  for(i=1; i<=8; i++)
  {
    if(vNumTest[i]==0)
    {
      sNum_test=i;
      vNumTest[i]=1;
      break;
    }
    else // al max 8 test e poi sovrascrive sempre l'ultimo
      sNum_test=8;
  }
  iNTest=sNum_test;
  if(nomeTest[0]=='\0')
    sprintf(nomeTest,"Test%d",sNum_test);
  /// Associa il test attuale alla posizione nell'array di ogni grandezza misurata
  memcpy(vNomeTest[sNum_test-1],nomeTest,sizeof(vNomeTest[sNum_test-1]));
  memcpy(vCoppiaRuota[sNum_test-1],Coppia_ruota,sizeof(vCoppiaRuota[sNum_test-1]));
  memcpy(vPotenzaRuota[sNum_test-1],Potenza_ruota,sizeof(vPotenzaRuota[sNum_test-1]));
  memcpy(vGiri[sNum_test-1],Giri,sizeof(vGiri[sNum_test-1]));
  memcpy(vNoteTest[sNum_test-1],Note_test,sizeof(vNoteTest[sNum_test-1]));
  memcpy(vTermoK[sNum_test-1],fTermo_K,sizeof(vTermoK[sNum_test-1]));
  memcpy(vTermoOil[sNum_test-1],fTermo_Oil,sizeof(vTermoOil[sNum_test-1]));
  memcpy(vPresMAP[sNum_test-1],fPres_MAP,sizeof(vPresMAP[sNum_test-1]));
  memcpy(vConsTot[sNum_test-1],fvettConsTot,sizeof(vConsTot[sNum_test-1]));
  memcpy(vCons[sNum_test-1],vettCons,sizeof(vCons[sNum_test-1]));

  /// Salva in posizione array corretta le impostazioni del test
  memcpy(&datPrm[sNum_test-1],&testPrm,sizeof(testPrm));
  graphEnabled[sNum_test-1]=true;

  return sNum_test-1;
}

void __fastcall TMain::showPerformance(int iTest)
{
  static TFastLineSeries *srsCop,*srsPot,*srsCons,*srsTK,*srsTOil,*srsPMap;
  static TCheckBox *cbTest;
  static int i,nZero;
  static float fUnitX;
  // questa routine è utilizzata solo per la visualizzazione dei risultati

  /// Calcolo Unità di Misura usata in asse X: Giri, Vel (Km/h,m/s,mph)
  if(Vis_GV)
  {
    switch(Unit_Vel)
    {
      case 0: // km/h
        fUnitX=1/datPrm[iTest].rappAnt*datPrm[iTest].diamRulli*0.000188849;
      break;                                  
      case 1: // mph
        fUnitX=1/datPrm[iTest].rappAnt*datPrm[iTest].diamRulli*0.000188849*0.62;            
      break;
      case 2: // m/s
        fUnitX=1/datPrm[iTest].rappAnt*datPrm[iTest].diamRulli*0.000188849*0.277;         
      break;
    }              
  }
  else
    fUnitX=1;  
  switch(iTest)
  {
    case 0:
      cbTest=CheckBox1;
      srsCop=Series10;
      srsPot=Series9;
      srsPMap=Series3;      
      srsTK=Series6;
      srsTOil=Series7;
      srsCons=Series15;
    break;
    case 1:
      cbTest=CheckBox2;
      srsCop=Series21;
      srsPot=Series35;
      srsPMap=Series60;
      srsTK=Series81;
      srsTOil=Series88;
      srsCons=Series16;
    break;
    case 2:
      cbTest=CheckBox3;
      srsCop=Series22;
      srsPot=Series36;
      srsPMap=Series61;      
      srsTK=Series82;
      srsTOil=Series89;
      srsCons=Series17;
    break;
    case 3:
      cbTest=CheckBox4;
      srsCop=Series23;
      srsPot=Series37;
      srsPMap=Series62;      
      srsTK=Series83;
      srsTOil=Series90;
      srsCons=Series18;
    break;
    case 4:
      cbTest=CheckBox5;
      srsCop=Series24;
      srsPot=Series38;
      srsPMap=Series63;      
      srsTK=Series84;
      srsTOil=Series91;
      srsCons=Series28;
    break;
    case 5:
      cbTest=CheckBox6;
      srsCop=Series25;
      srsPot=Series39;
      srsPMap=Series64;      
      srsTK=Series85;
      srsTOil=Series92;
      srsCons=Series29;
    break;
    case 6:
      cbTest=CheckBox7;
      srsCop=Series26;
      srsPot=Series40;
      srsPMap=Series65;      
      srsTK=Series86;
      srsTOil=Series93;
      srsCons=Series30;
    break;
    case 7:
      cbTest=CheckBox8;
      srsCop=Series27;
      srsPot=Series41;
      srsPMap=Series66;
      srsTK=Series87;
      srsTOil=Series94;
      srsCons=Series31;
  }

  /// Clear grafici
  srsCop ->Clear();
  srsPot ->Clear();
  srsPMap->Clear();
  srsTK  ->Clear();
  srsTOil->Clear();
  srsCons->Clear();

  /// Visualizza o no

  cbTest->Visible=true;
  cbTest->Caption=vNomeTest[iTest];
  srsCop->Visible=Vis_C_R;
  srsPot->Visible=Vis_P_R;
  srsPMap->Visible=Vis_Pmap;
  srsTK->Visible=Vis_T_K;
  srsTOil->Visible=Vis_T_N;
  srsCons->Visible=Vis_cons;

  nZero=0;

  for(i=0;i<500;i++)
  {
    if(vGiri[iTest][i])
    {
      srsCop->AddXY(fUnitX*vGiri[iTest][i],datPrm[iTest].fFact*vCoppiaRuota[iTest][i]*fK_convC,"",clTeeColor);

      srsPot->AddXY(fUnitX*vGiri[iTest][i],datPrm[iTest].fFact*vPotenzaRuota[iTest][i]*fK_convP,"",clTeeColor);

      srsPMap->AddXY(fUnitX*vGiri[iTest][i],vPresMAP[iTest][i]*fK_convPr,"",clTeeColor);

      if(Unit_Tem==0) // °C
        srsTK->AddXY(fUnitX*vGiri[iTest][i],vTermoK[iTest][i],"",clTeeColor);
      else
        srsTK->AddXY(fUnitX*vGiri[iTest][i],ConvertTo(vTermoK[iTest][i],tuFahrenheit),"",clTeeColor);

      if(Unit_Tem==0) // °C
        srsTOil->AddXY(fUnitX*vGiri[iTest][i],vTermoOil[iTest][i],"",clTeeColor);
      else
        srsTOil->AddXY(fUnitX*vGiri[iTest][i],ConvertTo(vTermoOil[iTest][i],tuFahrenheit),"",clTeeColor);

      if(Opzioni_grafici->rbGKWh->Checked)
         srsCons->AddXY(fUnitX*vGiri[iTest][i],CONSUMO_G_KWH(vCons[iTest][i],(datPrm[iTest].fFact*vPotenzaRuota[iTest][i])),"",clTeeColor);
      else
        srsCons->AddXY(fUnitX*vGiri[iTest][i],CONSUMO(vCons[iTest][i],vConsTot[iTest][i]),"",clTeeColor);
    }
    else
      nZero++;
    if(nZero>3)
      break;  /// Interrompo il ciclo se ricevo almeno 3 RPM a zero
  }
   
  // aggiorna i dati dei test
  updateGrids();
  updateRichText();
    
} // fine rappresentazione

void TMain::showAllTests()
{
  for(int i=1;i<=8;i++)
  {
    if(vNumTest[i] && graphEnabled[i-1])
      showPerformance(i-1);
  }
}

void __fastcall TMain::Aprimisurazioni1Click(TObject *Sender)
{




  if(Salvamisurazione1->Enabled==true && bSavedTest==false)
   {
    MessageBeep(MB_ICONEXCLAMATION);
 if(MessageDlg(Str9,mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
 {
 Salvamisurazione1Click(this);
 }

   }



 Carica_File->ShowModal(); // carica file da visualizzare





}
//---------------------------------------------------------------------------





void __fastcall TMain::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{

  try
  {
  Remote(Key);

  if(Out_dat==false)  // solo se output dati file
  {

   if(Key == 39)  // Dx
  {
   iPos++;
   Visualizza_dat_test_strumenti(sTest_antep,iPos);
  }

  if(Key == 37)   // Sx
  {
   if(iPos)
   {
     iPos--;
     Visualizza_dat_test_strumenti(sTest_antep,iPos);
   }
  }

   }
  }
  catch (Exception &exception)
  {

  LogError(g_cFileLog,LOG_INFO,exception.Message.c_str());


  }
}
//---------------------------------------------------------------------------




// funzione di controllo e gestione telecomando remoto CMMRC
void __fastcall TMain::Remote(int Codice_Tasto)
{
  switch(Codice_Tasto)
  {
    case 73: // tasto Incremento
      if(Step_test->Active==true && Tipo_test==3)
        Step_test->BitBtn1Click(this);
      if(Pilot_Freno->Active==true && Tipo_test==6)
        Pilot_Freno->VrTrackBar1->Position+=2;
    break;
    case 68: // tasto Decremento
      if(Step_test->Active==true && Tipo_test==3)
        Step_test->BitBtn2Click(this);
      if(Pilot_Freno->Active==true && Tipo_test==6)
        Pilot_Freno->VrTrackBar1->Position-=2;
    break;
    case 83: // tasto MUTE del telecomando visualizza strumenti
      if(SpeedButton19->OnClick == SpeedButton19Click2)
        SpeedButton19Click2(this);
      else SpeedButton19Click(this);
    break;
    case 112: // Tasto F1
      if(Start->Active==false) SpeedButton4Click(this);
    break;
    case 113: // Tasto F2
      if(Start->Active==false) SpeedButton8Click(this);
    break;
    case 114: // Tasto F3
      if(Start->Active==false) SpeedButton21Click(this);
    break;

         case 115: // Tasto F4
         if(Start->Active==false)  SpeedButton17Click(this);
            break;

          case 116: // Tasto F5
         if(Start->Active==false)  SpeedButton15Click(this);
            break;

          case 118: // Tasto F7
         if(Start->Active==false)  SpeedButton18Click(this);
            break;

         case 120: // tasto di avvio procedura di calcolo rapporto
                  // e successivamente di test
          if(Rapporto->Active==false)
            SpeedButton1Click(this);
            break;

         case 121: // tasto inizio sessione di test
         if(Rapporto->Active==false)
         SpeedButton2Click(this);
            break;
           }

}









void TMain::updateUnits()
{
  switch(Unit_Pot)
  {
    case 0:
      Unit_pw=" Kw ";
    break;
    case 1:
      Unit_pw=" Cv ";
    break;
    case 2:
      Unit_pw=" Hp ";
    break;
  }

  switch(Unit_Cop)
  {
    case 0:
      Unit_M=" Nm ";
    break;
    case 1:
      Unit_M=" Kg·m ";
    break;
    case 2:
      Unit_M=" lb/ft ";
    break;
  }
  switch(Unit_Cons)
  {
    case 0:
      sUnitCons="l/h";
    break;
    case 1:
      sUnitCons="l/min";
    break;
    case 3:
      sUnitCons="l";
    break;
  }

}

void TMain::updateGrids()
{
  static int i,nValNull=0,df;
  static TNextGrid *nxGrid;
  static TTabSheet *tabSheet;
  // cancella edit di testo

  for(i=0;i<8;i++)
  {
    if(vNumTest[i+1]==0)
      continue;

    switch(i)
    {
      case 0:
        nxGrid=NextGrid1;
        tabSheet=Tabella_test_1;
      break;
      case 1:
        nxGrid=NextGrid2;
        tabSheet=Tabella_test_2;
      break;
      case 2:
        nxGrid=NextGrid3;
        tabSheet=Tabella_test_3;
      break;
      case 3:
        nxGrid=NextGrid4;
        tabSheet=Tabella_test_4;
      break;
      case 4:
        nxGrid=NextGrid5;
        tabSheet=Tabella_test_5;
      break;
      case 5:
        nxGrid=NextGrid6;
        tabSheet=Tabella_test_6;
      break;
      case 6:
        nxGrid=NextGrid7;
        tabSheet=Tabella_test_7;        
      break;
      case 7:
        nxGrid=NextGrid8;
        tabSheet=Tabella_test_8;        
      break;      
    }
    tabSheet->Caption=vNomeTest[i];
    tabSheet->TabVisible=true;
    nxGrid->ClearRows();
    nxGrid->AddRow(1);
    nxGrid->Cells[0][0] =Str0;
    nxGrid->Cells[1][0] =Unit_M;
    nxGrid->Cells[2][0] =Unit_pw;
    if(Unit_Tem==0)
    {
      nxGrid->Cells[4][0]=Text40+" °C";
      nxGrid->Cells[3][0]=Text41+" °C";
    }
    else
    {
      nxGrid->Cells[4][0]=Text40+" °F";
      nxGrid->Cells[3][0]=Text41+" °F";
    }
    if(Unit_Map==0)
      nxGrid->Cells[5][0]=Str14+" mbar";
    else
      nxGrid->Cells[5][0]=Str14+" psi";
    nxGrid->Cells[6][0]="g/kWh";
    nxGrid->Columns->operator [](7)->Visible=(Unit_Cons!=2);
    if(Unit_Cons!=2)
      nxGrid->Cells[7][0]=Opzioni_grafici->rbL->Caption;
    df=0;
    nValNull=0;
    for(int t=0; t<500; t++)
    {
      if(vGiri[i][t])
      {
        nxGrid->AddRow(1);
        df++;
        nxGrid->Cells[0][df]=IntToStr(vGiri[i][t]);
        nxGrid->Cells[1][df]=FormatFloat("0.#",datPrm[i].fFact*vCoppiaRuota[i][t]*fK_convC);
        nxGrid->Cells[2][df] =FormatFloat("0.#",datPrm[i].fFact*vPotenzaRuota[i][t]*fK_convP);
        if(Unit_Tem==0)
        {
          nxGrid->Cells[4][df]=FormatFloat("0.#",vTermoOil[i][t]);
          nxGrid->Cells[3][df]=FormatFloat("0.#",vTermoK[i][t]);
        }
        else
        {
          nxGrid->Cells[4][df]=FormatFloat("0.#",ConvertTo(vTermoOil[i][t],tuFahrenheit));
          nxGrid->Cells[3][df]=FormatFloat("0.#",ConvertTo(vTermoK[i][t],tuFahrenheit));
        }
        if(Unit_Map==0)
          nxGrid->Cells[5][df]=FormatFloat("0.#",vPresMAP[i][t]);
        else
          nxGrid->Cells[5][df]=FormatFloat("0.##",vPresMAP[i][t]*0.0145);  
        // Consumo in g/kWh
        nxGrid->Cells[6][df]=FormatFloat("0.",CONSUMO_G_KWH(vCons[i][t],vPotenzaRuota[i][t]));
        if(Unit_Cons!=2) // Consumo in l l/h l/m
          nxGrid->Cells[7][df] =FormatFloat("0.##",CONSUMO(vCons[i][t],vConsTot[i][t]));
      } 
      else
        nValNull++;
      if(nValNull>5)
        break;     
    }
  }
}

void TMain::updateRichText()
{
  static int i;
  static TDateTime *pData;        
  RichEdit1->Clear();
  RichEdit1->Lines->Add(Text0);

  for(i=0;i<8;i++)
  {
    if(vNumTest[i+1] && graphEnabled[i]) //  test 1
    {
      Stampadelletabelle1->Enabled=true;
      RichEdit1->Lines->Add("");
      RichEdit1->Lines->Add("  _____________________________________________");
      RichEdit1->Lines->Add(Text1+(String)vNomeTest[i]);
      RichEdit1->Lines->Add("  ");
      if((int)datPrm[i].nLancio)
        {
          pData=(TDateTime*)&(datPrm[i].dataL);
          RichEdit1->Lines->Add("#"+IntToStr((int)datPrm[i].nLancio)+"     "+ pData->FormatString("DD/MM/YYYY HH.NN.SS"));
        }
      if(Unit_Tem==0)// visualizza temperatura atmosferica
        RichEdit1->Lines->Add(Text3+FormatFloat("  0.#",datPrm[i].tempAtm)+"°C");
      if(Unit_Tem==1)
        RichEdit1->Lines->Add(Text3+FormatFloat("  0.#",ConvertTo(datPrm[i].tempAtm,tuFahrenheit))+"°F");
      // visualizza pressione atmosferica
      RichEdit1->Lines->Add(Text4+FormatFloat("  0.#",datPrm[i].pressAtm)+"mbar");
      // visualizza umidità atmosferica
      RichEdit1->Lines->Add(Text5+FormatFloat("  0.#",datPrm[i].umidRel)+"%");
      // visualizza tipo di normativa
      switch(int(datPrm[i].tipoNorm))
      {
        case 0:
        RichEdit1->Lines->Add("  "+Text6 + normSAE);
        break;
        case 1:
        RichEdit1->Lines->Add("  "+Text6 + normISO);
        break;
        case 2:
        RichEdit1->Lines->Add("  "+Text6 + normCE);
        break;
        case 3:
        RichEdit1->Lines->Add("  "+Text6 + normECE);
        break;
        case 4:
        RichEdit1->Lines->Add("  "+Text6 + normCEE);
        break;
        case 5:
        RichEdit1->Lines->Add("  "+Text6 + normDIN);
        break;
        case 6:
        RichEdit1->Lines->Add("  "+Text6 + normOCDE);
        break;
        case 7:        RichEdit1->Lines->Add("  "+Text6 + "MAN.");;
        break;
      }

      // visualizza fattore di correzione
      RichEdit1->Lines->Add(Text7+FormatFloat("  0.###",datPrm[i].fFact));
      // visualizza rapporto
      RichEdit1->Lines->Add(Text2+FormatFloat("  0.##",datPrm[i].rappAnt));
      RichEdit1->Lines->Add("");
      // dati max rilevati
      RichEdit1->Lines->Add(Text34);
      // coppia pto
      RichEdit1->Lines->Add(Str4+FormatFloat("  0.#",datPrm[i].fFact*datPrm[i].maxCRuota*fK_convC) + Unit_M);
      // potenza pto
      RichEdit1->Lines->Add(Str2+FormatFloat("  0.#",datPrm[i].fFact*datPrm[i].maxPRuota*fK_convP) + Unit_pw);
      //Cons. Spec.
      if(datPrm[i],consMaxC>0)
      {
        sprintf(Buffer,"  Spec. fuel cons. %-1.1f gr%/kWh @Max Torque",datPrm[i].consMaxC);
        RichEdit1->Lines->Add(Buffer);
        sprintf(Buffer,"  Spec. fuel cons. %-1.1f gr%/kWh @Max Power",datPrm[i].consMaxP);
        RichEdit1->Lines->Add(Buffer);
      }

      //Riserva di coppia
      if(datPrm[i].coppiaMaxPow>0)
      {
        sprintf(Buffer,"  Res.Torque %-1.1f %%",((datPrm[i].maxCRuota-datPrm[i].coppiaMaxPow)*100)/datPrm[i].coppiaMaxPow);
        RichEdit1->Lines->Add(Buffer);
      }
      // manifold
      if(Vis_Pmap)
      {
        if(Unit_Map==0)
          RichEdit1->Lines->Add("  "+Text26+FormatFloat("0.#",datPrm[i].maxManifol)+ " mbar");
        if(Unit_Map==1)
          RichEdit1->Lines->Add("  "+Text26+FormatFloat("0.##",datPrm[i].maxManifol*0.0145)+ " psi");
      }    

      // Termocoppia
      if(Vis_T_K)
      {
        if(Unit_Tem==0)        
          RichEdit1->Lines->Add("  "+Text28+FormatFloat("0.#",datPrm[i].maxTermoK)+" °C");
        if(Unit_Tem==1)
          RichEdit1->Lines->Add("  "+Text28+FormatFloat("0.#",ConvertTo(datPrm[i].maxTermoK,tuFahrenheit))+" °F");
      }      

      // Temperatura olio motore
      if(Vis_T_N)
      {
        if(Unit_Tem==0)
          RichEdit1->Lines->Add("  "+Text29+datPrm[i].maxTempOil+" °C");
        if(Unit_Tem==1)
          RichEdit1->Lines->Add("  "+Text29+FormatFloat("0.#",ConvertTo(datPrm[i].maxTempOil,tuFahrenheit))+" °F");
      }
      // visualizza le note del test
      RichEdit1->Lines->Add("");
      RichEdit1->Lines->Add("  "+Text36);
      RichEdit1->Lines->Add("  "+(String)vNoteTest[i]);
    }
  }
}

void __fastcall TMain::Def_rapporto()
{
  int totale=0;

  // imposta il rapporto corrente all'ultimo test della lista dopo cancellazione
  for(int i=8; i>0; i--)
  {
    if(vNumTest[i])
    {
       Rap_tot1=datPrm[i-1].rappAnt;
       totale+=vNumTest[i];
       Stampadeigrafici1->Enabled=true; // riattiva stampa se un test o più sono presenti
       Stampadatideitest1->Enabled=true;
       Panel33->Visible=true;
       break;
    }
  }
  if(totale==0)
  {
    Panel33->Visible=false;
    Stampadeigrafici1->Enabled=false;
    Stampadatideitest1->Enabled=false;
  }
}



void __fastcall TMain::HrLabelDblClick(TObject *Sender)
{

 AnsiString NewString ="";
  //if (InputQuery("Input Box", "Umidità relativa %:", NewString))
  if (InputQuery("Input Box", "Umidita relativa %:", NewString))
  {
  Val_Hr =StrToFloat(NewString);
  if(Val_Hr < 0) Val_Hr=0;
  if(Val_Hr > 100) Val_Hr=100;
  HrLabel->Caption=FloatToStr(Val_Hr);
  }
}
//---------------------------------------------------------------------------

 // funzione di verifica e assegnamento massimi valori raggiunti nel test appena eseguito
void __fastcall TMain::Max_value_test(int iSamples)
{
  float
  Massima_coppia_ruota=0,
  Massima_manifold=0,
  Massima_olio=0,
  Massima_termocoppia=0;

  Max_pot_ruota=0;  // massima potenza raggiunta alla ruota
  Max_cop_ruota=0;  // massima coppia ruota
  Max_manifol=0;  // massima pressione aspirazione raggiunta a max pot. mot.
  Max_termocoppia=0;  // massima temperatura K raggiunta a max pot. mot.
  Max_temp_olio=0;  // massima temperatura olio raggiunta a max pot. mot.
  Max_AD=0;                // massima tensione AD 0-15Vcc raggiunta a max pot. mot.
  Regime_motore=0;         // regime del motore di riferimento di tutti i valori massimi
  Regime_motore2=0;        // regime del motore di riferimento coppia massima
  consMaxP = 0;            // consumo alla max potenza
  consMaxC = 0;            // consumo alla max coppia
  coppiaMaxPow = 0;        // val.di coppia alla pot max

 for(int t=0;t<iSamples;t++)
 {
    // tolgo fFact da coppia e potenza perchè già inseriti nel calcolo della coppia più a monte
    // massimi valori riferiti alla max pot. motore
    if(Potenza_ruota[t] >= Max_pot_ruota)
    {
      Max_pot_ruota = Potenza_ruota[t];
      Regime_motore = Giri[t];
      Max_manifol = fPres_MAP[t];
      Max_termocoppia = fTermo_K[t];
      Max_temp_olio = fTermo_Oil[t];
      consMaxP = vettCons[t];
      coppiaMaxPow = Coppia_ruota[t];
      if(sTipo_dato==3)
        iPos=t;
    }
    // massima manifold
    if(fPres_MAP[t] >= Massima_manifold)
    {
      if(sTipo_dato==4) iPos=t;
    }
    // massima olio
    if(fTermo_Oil[t] >= Massima_olio)
    {
      if(sTipo_dato==5) iPos=t;
    }
    // massima termocoppia
    if(fTermo_K[t] >= Massima_termocoppia)
    {
      if(sTipo_dato==6) iPos=t;
    }
    // massimi valori riferiti alla max coppia. motore
    if(Coppia_ruota[t] >= Max_cop_ruota)
    {
      Max_cop_ruota = Coppia_ruota[t];
      Regime_motore2 = Giri[t];
      consMaxC = vettCons[t];
      if(sTipo_dato==1 || sTipo_dato==0)
        iPos=t;
    }
  }

}

void __fastcall TMain::Stampadatideitest1Click(TObject *Sender)
{

  // stampa dei dati risultanti
  if(PrintDialog1->Execute())
    RichEdit1->Print("Dati del test");

}
//---------------------------------------------------------------------------


int angStX,angStY,giaPassato = 0;
void __fastcall TMain::Stampadeigrafici1Click(TObject *Sender)
{
  char name_test_A[500] ;
  char name_test_B[500] ;
  struct TEST_SETS datPrmAB[2];
  float *dttr,aspectratio;
  int tp;
  int h,w,x,y,sp,crt,lft,rgt,bttn;
  String JKK,JJK,LLL,BBB,CCC,D,AS,SA,spc1=" ";
  char gg[70];
  float spl,xf,yf;
  unsigned int numTestLoad = 0;
  int t;

   memset(name_test_A,0,sizeof(name_test_A));
  memset(name_test_B,0,sizeof(name_test_B));
  giaPassato = 0;
  for(t=1; t<=8; t++)
    if(vNumTest[t]==1)
      numTestLoad++;

  memset(datPrmAB,0,sizeof(datPrmAB));

  JKK=Cliente;
  JJK=Targa;
  LLL=Costruttore;
  BBB=Modello;
  CCC=Allestimento;
  String da_ta;
  Graphics::TBitmap *Bitmap1 = new Graphics::TBitmap();
  if(PrintDialog1->Execute())
  {
    // disattiva cursore a croce
    ChartTool4->Active=false;
    try
    {
      Screen->Cursor = crHourGlass;
      Chart1->PrintResolution= -70;   // dettaglio di stampa
      Printer()->BeginDoc();
      try 
      {
        h = Printer()->PageHeight; // <-- get page height
        w = Printer()->PageWidth;  // <-- get page width
        crt = w/120; //larghezza di un carattere
        x=w/100;
        y=h/100;
        sp=h/100;


        Printer()->Canvas->Font->Name = "Thaoma";
        Printer()->Canvas->Font->Size = 8;             // <-- set the font size
        Printer()->Canvas->Font->Style.Clear();
        Printer()->Canvas->TextOut(x,y,"Sw rev. "+(String)swNbr);   // <-- print some text

        //if(dataFile != 0)
            Printer()->Canvas->TextOut(x + w/2,y," "+dataFile.FormatString("dd/mm/yyyy hh.nn")+"     Dimsport s.r.l. " + StatusBar1->Panels->Items[0]->Text);   // <-- print some text

        //strcpy(tep,SS.c_str());
        tp=SS.Length()+1;
        /*for (t=0;t<500;t++)
        {
          if(tep[t] == NULL)
          {
            tp = t;
            break;
          }
        }*/

        Printer()->Canvas->TextOut((w-(crt*tp)),y,ExtractFileName(SS));   // <-- print fileName
        y+=sp;
        Printer()->Canvas->TextOut(x,y,"¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯");
        y+=sp;
        if(FileExists(Percorso))
          Bitmap1->LoadFromFile(Percorso);
        else
          Bitmap1->LoadFromResourceName((int)HInstance,"Logo1");

        aspectratio=(float)Bitmap1->Width/(float)Bitmap1->Height;
        TRect RectImage(w*98/100-3*sp*aspectratio,y,w*98/100,y+3*sp/*h*4/100*/);//Bitmap1->Canvas->ClipRect;
        Printer()->Canvas->TextOut(x,y,Text47 + Ragione);   // <-- print Workshop/Tuner
        //>>sposto logo

        Printer()->Canvas->StretchDraw(RectImage,Bitmap1);
        //Printer()->Canvas->Draw(w*72/100,y,Bitmap1);
        //Bitmap1->Height = w*6.5/100;//850;//h*(5/100);//850;
        //Bitmap1->Width = w*30/100;//3500;//w*(20/100);//3500;
        //Printer()->Canvas->Draw(w*75/100,y,Bitmap1); // stampa il logo a partire dall'80% della max larghezza
        //<<sposto logo
        y+=sp;
        Printer()->Canvas->TextOut(x,y,Text48 + " " + Indirizzo + " Internet: "+ Internet);   // <-- print some text
        y+=sp;
        Printer()->Canvas->TextOut(x,y,Text49 +" "+ Telefono + " Fax: "+ Fax + " E-mail: " + EMail);   // <-- print some text
        y+=sp*2;
        Printer()->Canvas->TextOut(x,y,"¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯");
        y+=sp;
        Printer()->Canvas->TextOut(x,y,Text50 + spc1 + JKK);  // cliente
        Printer()->Canvas->TextOut(w-(w/1.7),y,Text51 + spc1 + JJK); //veicle
        Printer()->Canvas->TextOut(w-(w/3),y,Text52 +spc1+ LLL); //manufacture
        y+=sp;
        Printer()->Canvas->TextOut(x,y,Text53 + spc1 + BBB);
        Printer()->Canvas->TextOut(w-(w/1.7),y,Text54 + spc1 + CCC);
        y+=sp;
        //      Printer()->Canvas->TextOut(x,y,"____________________________________________________________________________________________________________________________________________________");
        //      y+=sp;
        Printer()->Canvas->Font->Name = "ErasBoldITC";
        Printer()->Canvas->Font->Size = 10;             // <-- set the font size
        Printer()->Canvas->Font->Color = clBlue;             // <-- set the color
        Printer()->Canvas->Font->Style.Clear();
        Printer()->Canvas->Font->Color = clBlack;// <-- set the color
        Printer()->Canvas->Font->Size = 10;             // <-- set the font size
        lft = w * 25/100;           // <-- 20% left margin  17 - 25
        rgt = w +  (w*1/100);     // <-- 20% right margin 3 - 1
        tp = y + sp;             // <-- 33% top margin
        bttn = tp + (h*60/100);     // <-- 20% bottom margin  60
        Chart1->PrintPartial( Rect(lft,tp ,rgt,bttn));//stampa GRAFICO
        x=w/100;
        y = y + 3*sp;
        for(t=0;t<8;t++)
        {
          if(graphEnabled[t])
          {
            y+=sp;
            Printer()->Canvas->Font->Color = clTestColor[t+1];//clBlue;
            Printer()->Canvas->TextOut(x,y,vNomeTest[t]);
            y+=sp;
            Printer()->Canvas->Font->Color = clBlack;
            if(datPrm[t].selPto==1)
            {
              Printer()->Canvas->TextOut(x,y,Str2+" - ");
              y+=sp;
            }
            else
            {
              Printer()->Canvas->TextOut(x,y,Str2+" "+FormatFloat("0.#",datPrm[t].fFact*datPrm[t].maxPRuota*fK_convP)+Unit_pw);
              y+=sp;
            }
            Printer()->Canvas->TextOut(x,y,Str1+" "+FormatFloat("0.#",datPrm[t].fFact*datPrm[t].maxPRuota*datPrm[t].rendPto*fK_convP)+Unit_pw);
            y+=sp;
            sprintf(Buffer,"%s %-1.1f%s  @%-1.0f %s",Str3,(datPrm[t].fFact*datPrm[t].maxCRuota*datPrm[t].rappAnt*datPrm[t].rendPto)*fK_convC,Unit_M,(datPrm[t].rpmMaxCMotore/datPrm[t].rappAnt),Str0);
            Printer()->Canvas->TextOut(x,y,Buffer);
            y+=sp;
            if(datPrm[t].selPto==1)
            {
                Printer()->Canvas->TextOut(x,y,Str4 + " -");
                y+= 2*sp;
            }
            else
            {
                //sprintf(Buffer,"Eng. Torque   %-1.1f%s  @%s Rpm",DAT_1[94] * fK_convC,Unit_M,FloatToStr(DAT_1[96]));
                sprintf(Buffer,"%s %-1.1f%s  @%4.0f %s",Str4,datPrm[t].fFact*datPrm[t].maxCRuota*fK_convC,Unit_M,datPrm[t].rpmMaxCMotore,Str0);
                Printer()->Canvas->TextOut(x,y,Buffer);
                y+= 2*sp;
            }
            Printer()->Canvas->TextOut(x,y,"¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯");
            if(datPrmAB[0].rappAnt==0)
            {
              memcpy(&datPrmAB[0],&datPrm[t],sizeof(datPrmAB[0]));
              strcpy(name_test_A,vNomeTest[t]);
            }
            else if(datPrmAB[1].rappAnt==0)
            {
              memcpy(&datPrmAB[1],&datPrm[t],sizeof(datPrmAB[1]));
              strcpy(name_test_B,vNomeTest[t]);
            }

          }
        }
        StringGrid1->Font->Size = h * (0.7/100);
        h = h * 95/100;
        unsigned int rigaGrid = 13;
        unsigned int colonnaGrid = 3;
        unsigned int gridWidth = 10;
        StringGrid1->ColCount = colonnaGrid + 1;
        StringGrid1->RowCount = rigaGrid;
        StringGrid1->GridLineWidth = gridWidth;
        int colLarg = (w * 90/100) / colonnaGrid;
        int rigAlt = (h - bttn) / rigaGrid;
        StringGrid1->DefaultColWidth = colLarg;
        StringGrid1->DefaultRowHeight =  rigAlt;
        StringGrid1->Height = rigAlt * (rigaGrid) + (rigaGrid * gridWidth);
        StringGrid1->Width = (colLarg * colonnaGrid) + (colonnaGrid * gridWidth);
        x = x + 3*sp;
        StringGrid1->Col = 3;
        float convKw = 0.735;
        float convNm   = 9.8;

        //colonna 0
        //StringGrid1->Cells[0][0] = "POWER DATA";
        StringGrid1->Cells[0][0] = " "+TextPowerData.UpperCase();
        //StringGrid1->Cells[0][1] = "ENGINE POWER";
        StringGrid1->Cells[0][1] = " "+Str2.UpperCase();
        if(datPrmAB[0].selPto==0)
          sprintf(Buffer,"   %-1.1f CV - %-1.1f KW  @%s %s ",datPrmAB[0].maxPRuota,datPrmAB[0].maxPRuota*convKw,FloatToStr(datPrmAB[0].rpmMaxPMotore),Str0);
        else
          sprintf(Buffer,"   ----");
        StringGrid1->Cells[0][2] = Buffer;

        if(numTestLoad > 1 && datPrmAB[1].selPto==0)
          sprintf(Buffer,"   %-1.1f CV - %-1.1f KW  @%s %s ",datPrmAB[1].maxPRuota,datPrmAB[1].maxPRuota*convKw,FloatToStr(datPrmAB[1].rpmMaxPMotore),Str0);
        else
          sprintf(Buffer," ");
        StringGrid1->Cells[0][3] = Buffer;
        //StringGrid1->Cells[0][4] = "PTO POWER";
        StringGrid1->Cells[0][4] = " "+Str1.UpperCase();
        sprintf(Buffer,"   %-1.1f CV - %-1.1f KW  @%-1.0f %s ",
                      datPrmAB[0].maxPRuota*datPrmAB[0].rendPto,
                      datPrmAB[0].maxPRuota*datPrmAB[0].rendPto*convKw,
                      (datPrmAB[0].rpmMaxPMotore/datPrmAB[0].rappAnt),Str0);
        StringGrid1->Cells[0][5] = Buffer;
        if(numTestLoad > 1)
        {
          sprintf(Buffer,"   %-1.1f CV - %-1.1f KW  @%-1.0f %s ",
                        datPrmAB[1].maxPRuota*datPrmAB[1].rendPto,
                        datPrmAB[1].maxPRuota*datPrmAB[1].rendPto*convKw,
                        (datPrmAB[1].rpmMaxPMotore/datPrmAB[1].rappAnt),Str0);
          StringGrid1->Cells[0][6] = Buffer;
        }
        //StringGrid1->Cells[0][7] = "PTO TORQUE";
        StringGrid1->Cells[0][7] = " "+Str3.UpperCase();
        //sprintf(Buffer,"   %-1.1f Nm - %-1.1f Kgm  @%-1.0f Rpm ",DAT_A[94] * DAT_A[0]* convNm * DAT_A[23],DAT_A[94] * DAT_A[0] * DAT_A[23],(DAT_A[73]/DAT_A[0]));
        sprintf(Buffer,"   %-1.1f Nm - %-1.1f Kgm  @%-1.0f %s ",datPrmAB[0].fFact*datPrmAB[0].maxCRuota*datPrmAB[0].rappAnt*convNm*datPrmAB[0].rendPto,datPrmAB[0].fFact*datPrmAB[0].maxCRuota*datPrmAB[0].rappAnt*datPrmAB[0].rendPto,(datPrmAB[0].rpmMaxCMotore/datPrmAB[0].rappAnt),Str0);
        StringGrid1->Cells[0][8] = Buffer;
        if(numTestLoad > 1)
        {
          sprintf(Buffer,"   %-1.1f Nm - %-1.1f Kgm  @%-1.0f %s ",datPrmAB[1].fFact*datPrmAB[1].maxCRuota*datPrmAB[1].rappAnt*convNm*datPrmAB[1].rendPto,datPrmAB[1].fFact*datPrmAB[1].maxCRuota*datPrmAB[1].rappAnt*datPrmAB[1].rendPto,(datPrmAB[1].rpmMaxCMotore/datPrmAB[1].rappAnt),Str0);
          StringGrid1->Cells[0][9] = Buffer;
        }
        //Titolo Coppia Motore
        StringGrid1->Cells[0][10] = " "+Str4.UpperCase();
        //Coppia Motore prova 1
        if(datPrmAB[0].selPto==0)
          sprintf(Buffer,"   %-1.1f Nm - %-1.1f Kgm  @%s %s ",datPrmAB[0].fFact*datPrmAB[0].maxCRuota * convNm,datPrmAB[0].fFact*datPrmAB[0].maxCRuota,FloatToStr(datPrmAB[0].rpmMaxCMotore),Str0);
        else
          sprintf(Buffer,"   ----");

        StringGrid1->Cells[0][11] = Buffer;
        // Coppia motore prova 2
        if(numTestLoad>1 && datPrmAB[1].selPto==0)
          sprintf(Buffer,"   %-1.1f Nm - %-1.1f Kgm  @%s %s ",datPrmAB[0].fFact*datPrmAB[1].maxCRuota*convNm,datPrmAB[1].fFact*datPrmAB[1].maxCRuota,FloatToStr(datPrmAB[1].rpmMaxCMotore),Str0);
        else
          sprintf(Buffer,"   ----");

        StringGrid1->Cells[0][12] = Buffer;

        //colonna 1
        StringGrid1->Cells[1][0] = " "+TextEnvironmentData.UpperCase();
        StringGrid1->Cells[1][1] = " "+Text3.UpperCase();
        sprintf(Buffer,"   %-1.1f °C - %-1.1f °Fth ",datPrmAB[0].tempAtm,ConvertTo(datPrmAB[0].tempAtm,tuFahrenheit));
        StringGrid1->Cells[1][2] = Buffer;
        if(numTestLoad > 1)
        {
          sprintf(Buffer,"   %-1.1f °C - %-1.1f °Fth ",datPrmAB[1].tempAtm,ConvertTo(datPrmAB[1].tempAtm,tuFahrenheit));
          StringGrid1->Cells[1][3] = Buffer;
        }
        //StringGrid1->Cells[1][4] = "AMB.PRESS - OIL TEMP - REL.HUMID.";
        StringGrid1->Cells[1][4] = " "+TextAmbPresOilHum.UpperCase();
        sprintf(Buffer,"   %.f mBar - %-1.1f °C - %.f %%",datPrmAB[0].pressAtm,datPrmAB[0].maxTempOil,datPrmAB[0].umidRel);
        StringGrid1->Cells[1][5] = Buffer;
        if(numTestLoad > 1)
        {
          sprintf(Buffer,"   %.f mBar - %-1.1f °C - %.f %%",datPrmAB[1].pressAtm,datPrmAB[1].maxTempOil,datPrmAB[1].umidRel);
          StringGrid1->Cells[1][6] = Buffer;
        }
        //StringGrid1->Cells[1][7] = "SPEC. FUEL CONS.";
        StringGrid1->Cells[1][7] = " "+TextConsSpec.UpperCase();
        //sprintf(Buffer,"   %.f/%.f gr/kWh @%s/%s Rpm",DAT_A[111],DAT_A[110],FloatToStr(DAT_A[73]),FloatToStr(DAT_A[96]));
        sprintf(Buffer,"   %.f/%.f gr/kWh @%s/%s %s",datPrmAB[0].consMaxC,datPrmAB[0].consMaxP,FloatToStr(datPrmAB[0].rpmMaxCMotore),FloatToStr(datPrmAB[0].rpmMaxPMotore),Str0);
        StringGrid1->Cells[1][8] = Buffer;
        if(numTestLoad >1)
        {
          sprintf(Buffer,"   %.f/%.f gr/kWh @%s/%s %s",datPrmAB[1].consMaxC,datPrmAB[1].consMaxP,FloatToStr(datPrmAB[1].rpmMaxCMotore),FloatToStr(datPrmAB[1].rpmMaxPMotore),Str0);
          StringGrid1->Cells[1][9] = Buffer;
        }
        //StringGrid1->Cells[1][10] = "CORR. FACTOR";
        StringGrid1->Cells[1][10] = " "+Text7.UpperCase();
        sprintf(Buffer,"   %-1.3f  ",datPrmAB[0].fFact);
        StringGrid1->Cells[1][11] = Buffer;
        if(numTestLoad > 1)
        {
          sprintf(Buffer,"   %-1.3f  ",datPrmAB[1].fFact);
          StringGrid1->Cells[1][12] = Buffer;
        }

        //colonna 2
        //StringGrid1->Cells[2][0] = "VHEICLE DATA";
        StringGrid1->Cells[2][0] = " "+TextTractorData.UpperCase();
        //StringGrid1->Cells[2][1] = "NAME OF TEST";
        StringGrid1->Cells[2][1] = Text1.UpperCase();
        StringGrid1->Cells[2][2] = name_test_A;
        StringGrid1->Cells[2][3] = "----";
        if(numTestLoad > 1)
          StringGrid1->Cells[2][3] = name_test_B;

        StringGrid1->Cells[2][4] = " "+Text26.UpperCase();
        sprintf(Buffer,"   %.f mBar",datPrmAB[0].maxManifol);
        StringGrid1->Cells[2][5] = Buffer;
        if(numTestLoad>1)
        {
          sprintf(Buffer,"   %.f mBar",datPrmAB[1].maxManifol);
          StringGrid1->Cells[2][6] = Buffer;
        }
        //StringGrid1->Cells[2][7] = "RATIO - GEAR";
        StringGrid1->Cells[2][7] = " "+TextRapporto.UpperCase();
        if(datPrmAB[0].rappAnt!=0)
          sprintf(Buffer,"   %-1.2f - %s",datPrmAB[0].rappAnt,FloatToStr(datPrmAB[0].marciaTest));
        StringGrid1->Cells[2][8] = Buffer;

        if(numTestLoad > 1)
        {
          if(datPrmAB[1].rappAnt!=0)
            sprintf(Buffer,"   %-1.2f - %s",datPrmAB[1].rappAnt,FloatToStr(datPrmAB[1].marciaTest));
        }

        StringGrid1->Cells[2][9] = Buffer;
        //StringGrid1->Cells[2][10] = "RPM. MIN - MAX";
        StringGrid1->Cells[2][10] = " "+TextRPMminmax.UpperCase();
        //sprintf(Buffer,"   %s - %s Rpm  ",FloatToStr(DAT_A[41]),FloatToStr(DAT_A[42]));
        sprintf(Buffer,"   %s - %s %s  ",FloatToStr(datPrmAB[0].regMin),FloatToStr(datPrmAB[0].regMax),Str0);
        StringGrid1->Cells[2][11] = Buffer;
        if(numTestLoad > 1)
        {
          sprintf(Buffer,"   %s - %s %s  ",FloatToStr(datPrmAB[1].regMin),FloatToStr(datPrmAB[1].regMax),Str0);
          StringGrid1->Cells[2][12] = Buffer;
        }

        angStY = bttn;
        angStX = x;
        StringGrid1->PaintTo(Printer()->Handle, x, bttn);
        Printer()->Canvas->MoveTo(x,bttn);
        Printer()->Canvas->LineTo(x,bttn+((rigaGrid * rigAlt) + (rigaGrid * gridWidth)));
        Printer()->Canvas->MoveTo(x,bttn);
        Printer()->Canvas->LineTo(x + ((colonnaGrid * colLarg) + (gridWidth * colonnaGrid)),bttn);
    
        x = x + (colonnaGrid * colLarg) + (gridWidth * colonnaGrid);
        Printer()->Canvas->MoveTo(x,bttn);
        Printer()->Canvas->LineTo(x,bttn+((rigaGrid * rigAlt) + (rigaGrid * gridWidth)));
        bttn = bttn +(rigaGrid * rigAlt) + (rigaGrid * gridWidth);
        Printer()->Canvas->MoveTo(x,bttn);
        Printer()->Canvas->LineTo(x - ((colonnaGrid * colLarg) + (gridWidth * colonnaGrid)),bttn);
    
        y+=sp;
        Printer()->Canvas->Font->Color = clBlack;
        Printer()->EndDoc(); // <-- end job and print !!
      }
      catch (...) 
      {
        // just in case an error happens
        Printer()->Abort();
        Printer()->EndDoc();
        throw;       // <-- rethrow the exception !!!
      } 
    }
    __finally 
    {
      Screen->Cursor = crDefault; // <-- restore cursor
    }
    delete Bitmap1;
    // riattiva cursore a croce
    ChartTool4->Active=true;    
  }
}
//---------------------------------------------------------------------------




void __fastcall TMain::Stampadelletabelle1Click(TObject *Sender)
{
  //Timer_con->Enabled=false; // disattivo controllo connessione
  switch(PageControl3->ActivePageIndex)
  {
    case 0:
      NxGridPrint1->Associate=NextGrid1;
    break;
    case 1:
      NxGridPrint1->Associate=NextGrid2;
    break;
    case 2:
      NxGridPrint1->Associate=NextGrid3;
    break;
    case 3:
      NxGridPrint1->Associate=NextGrid4;
    break;
    case 4:
      NxGridPrint1->Associate=NextGrid5;
    break;
    case 5:
      NxGridPrint1->Associate=NextGrid6;
    break;
    case 6:
    NxGridPrint1->Associate=NextGrid7;
    break;
    case 7:
      NxGridPrint1->Associate=NextGrid8;
    break;
    default:
      NxGridPrint1->Associate=NULL;
  }
  NxGridPrint1->Print();
  //Timer_con->Enabled=true;
}
//----------------------------------------------------------------------------











void __fastcall TMain::Setupdistampa1Click(TObject *Sender)
{
  PrinterSetupDialog1->Execute();
}
//---------------------------------------------------------------------------


void __fastcall TMain::Aggiunginote1Click(TObject *Sender)
{
  Note->Note(1);
}
//---------------------------------------------------------------------------

void __fastcall TMain::Aggiungimodificanotedeltest1Click(TObject *Sender)
{
  Note->Note(2);
}
//---------------------------------------------------------------------------

void __fastcall TMain::Aggiungimodificanotedeltest2Click(TObject *Sender)
{
  Note->Note(3);
}
//---------------------------------------------------------------------------

void __fastcall TMain::Aggiungimodificanotedeltest3Click(TObject *Sender)
{
  Note->Note(4);
}
//---------------------------------------------------------------------------

void __fastcall TMain::Aggiungimodificanotedeltest4Click(TObject *Sender)
{
  Note->Note(5);
}
//---------------------------------------------------------------------------

void __fastcall TMain::Aggiungimodificanotedeltest6Click(TObject *Sender)
{
  Note->Note(6);
}
//---------------------------------------------------------------------------

void __fastcall TMain::Aggiungimodificanotedeltest7Click(TObject *Sender)
{
  Note->Note(7);
}
//---------------------------------------------------------------------------

void __fastcall TMain::Aggiungimodificanotedeltest8Click(TObject *Sender)
{
  Note->Note(8);
}
//---------------------------------------------------------------------------

void __fastcall TMain::CalcPerformance(int iSamples)
{
  int iCounter=0,i,j;

  /// Reset grandezze fondamentali Giri,Coppia,Potenza
  memset(Giri,0,sizeof(Giri));
  memset(Coppia_ruota,0,sizeof(Coppia_ruota));
  memset(Potenza_ruota,0,sizeof(Potenza_ruota));

  if (bFilePerf)
  {
    SaveDiag(cDiagName,"RPM",RPM_M,iSamples, true);
    SaveDiag(cDiagName,"Torque",C_M,iSamples);
  }
  // calcolo potenza all'albero
  if(Tipo_test!=TEST_DECEL) // Test a step
  {
    for(int i=0; i<iSamples; i++)
    {
      Coppia_ruota[i]=C_M[i];
      Giri[i]=RPM_M[i];
      Potenza_ruota[i]=((Coppia_ruota[i]/716.2) * Giri[i]);//* fK_convP;
    }
    iCounter=iSamples;
  }
  else // Test decelerativo
  {
    memset(vettCons,0,sizeof(vettCons));
    memset(fvettConsTot,0,sizeof(fvettConsTot));
    memset(fTermo_Oil,0,sizeof(fTermo_Oil));
    memset(fTermo_K,0,sizeof(fTermo_K));
    memset(fPres_MAP,0,sizeof(fPres_MAP));
    iCounter=0;
    j=0;

    for(i=0;i<iSamples;i++)
    {
      Giri[iCounter]+=RPM_M[i];
      Coppia_ruota[iCounter]+=C_M[i];
      vettCons[iCounter]+=vSerCons[i];
      fTermo_Oil[iCounter]+=vSerTermoOil[i];
      fTermo_K[iCounter]+=vSerTermoK[i];
      fPres_MAP[iCounter]+=vSerPresMap[i];
      j++;
      if((i+1)%15==0 && i>0 || i==(iSamples-1))
      {
        Giri[iCounter]/=j;
        Coppia_ruota[iCounter]/=j;
        vettCons[iCounter]/=j;
        fvettConsTot[iCounter]=vSerConsTot[i];
        fTermo_Oil[iCounter]/=j;
        fTermo_K[iCounter]/=j;
        fPres_MAP[iCounter]/=j;
        Potenza_ruota[iCounter]=((Coppia_ruota[iCounter]/716.2) * Giri[iCounter]);
        iCounter++;
        j=0;
      }
    }
    Contatore=iCounter;
  } 
  Max_value_test(iCounter);
  testPrm.posMaxP=iPos;

  nomeTest[0]='\0';
  nLanciTotali++;
  dynoPrm.count.nLanciTermPar++;
  dynoPrm.count.nLanciTerm++;  
  testPrm.nLancio= nLanciTotali;
  dDateTest=Now().operator double();  
}  // end CalcPerformance

void __fastcall TMain::Taco1Click(TObject *Sender)
{
  Stampadeigrafici1Click(this);
}




//---------------------------------------------------------------------------



void __fastcall TMain::Idioma()
{
  String LG; // estensione del file di traduzione
  TIniFile *ini;
  ini=new TIniFile(utils.GetProgramPath("traduzioni.ini"));
  LG = ini->ReadString( "Codice", IntToStr(Lingua) , "" );
  Setup->ComboBox4->Clear();

  if(LG=="")
  {
    ShowMessage("Language is not available!");
    Log_Error(5,NULL,NULL);
    Close();
  }


  for(int t=1; t<=10; t++) // carica l'elenco delle lingua disponibili (max 10)
  {
    if( ini->ReadString( "Lingua", IntToStr(t) , "" )!="" )
    Setup->ComboBox4->Items->Add(ini->ReadString( "Lingua",  IntToStr(t) , "" ));
  }

  CGauge3->MaxValue = MAX_PERC_CONS;
  Str1=ini->ReadString( "Grafici_"+LG, "Str20" , "" );
  Str2=ini->ReadString( "Grafici_"+LG, "Str10" , "" );
  Str3=ini->ReadString( "Grafici_"+LG, "Str40" , "" );
  Str4=ini->ReadString( "Grafici_"+LG, "Str30" , "" );
  Str5=ini->ReadString( "Grafici_"+LG, "Str50" , "" );
  Str6=ini->ReadString( "Grafici_"+LG, "Str60" , "" );
  Str0=ini->ReadString( "Grafici_"+LG, "Str70" , "" );
  Text22=ini->ReadString( "Grafici_"+LG, "Str80" , "" );
  Text23=ini->ReadString( "Grafici_"+LG, "Str90" , "" );

   Text24=ini->ReadString( "Grafici_"+LG, "Str100" , "" );
   Text25=ini->ReadString( "Grafici_"+LG, "Str110" , "" );

   Text26=ini->ReadString( "Grafici_"+LG, "Str120" , "" );
   Text27=ini->ReadString( "Grafici_"+LG, "Str130" , "" );
   Text28=ini->ReadString( "Grafici_"+LG, "Str140" , "" );
   Text29=ini->ReadString( "Grafici_"+LG, "Str150" , "" );
   Text30=ini->ReadString( "Grafici_"+LG, "Str160" , "" );
   Text31=ini->ReadString( "Grafici_"+LG, "Str170" , "" );
   Text32=ini->ReadString( "Grafici_"+LG, "Str180" , "" );
   Text33=ini->ReadString( "Grafici_"+LG, "Str190" , "" );
   Text34=ini->ReadString( "Grafici_"+LG, "Str200" , "" );
   Text35=ini->ReadString( "Grafici_"+LG, "Str210" , "" );
   Text36=ini->ReadString( "Grafici_"+LG, "Str220" , "" );
   Text37=ini->ReadString( "Grafici_"+LG, "Str230" , "" );
   Text38=ini->ReadString( "Grafici_"+LG, "Str240" , "" );
   Text39=ini->ReadString( "Grafici_"+LG, "Str250" , "" );
   Text40=ini->ReadString( "Grafici_"+LG, "Str260" , "" );
   Text41=ini->ReadString( "Grafici_"+LG, "Str270" , "" );

   Text88=ini->ReadString( "Messaggi_"+LG, "Str87" , "" );
   Text89=ini->ReadString( "Messaggi_"+LG, "Str95" , "" );

   Str7=ini->ReadString( "Messaggi_"+LG, "Str10" , "" );
   Str8=ini->ReadString( "Messaggi_"+LG, "Str20" , "" );
   Str9=ini->ReadString( "Messaggi_"+LG, "Str30" , "" );
   Str10=ini->ReadString( "Messaggi_"+LG, "Str40" , "" );
   Str11=ini->ReadString( "Messaggi_"+LG, "Str50" , "" );
   Str15=ini->ReadString( "Messaggi_"+LG, "Str60" , "" );
   Str24=ini->ReadString( "Messaggi_"+LG, "Str70" , "" );
   Str25=ini->ReadString( "Messaggi_"+LG, "Str80" , "" );
   Str26=ini->ReadString( "Messaggi_"+LG, "Str90" , "" );
   Text91=ini->ReadString( "Messaggi_"+LG, "Str190" , "" );

   Str12=ini->ReadString( "Indicatori_"+LG, "Str10" , "" );
   Str13=ini->ReadString( "Indicatori_"+LG, "Str20" , "" );
   Str14=ini->ReadString( "Indicatori_"+LG, "Str30" , "" );


   Str16=ini->ReadString( "Impostazioni_"+LG, "Str10" , "" );
   Str17=ini->ReadString( "Impostazioni_"+LG, "Str20" , "" );
   Str18=ini->ReadString( "Impostazioni_"+LG, "Str30" , "" );
   Str19=ini->ReadString( "Impostazioni_"+LG, "Str40" , "" );
   Str21=ini->ReadString( "Impostazioni_"+LG, "Str50" , "" );
   Str22=ini->ReadString( "Impostazioni_"+LG, "Str60" , "" );
   Str23=ini->ReadString( "Impostazioni_"+LG, "Str70" , "" );
   Text43=ini->ReadString( "Impostazioni_"+LG, "Str80" , "" );
   Text44=ini->ReadString( "Impostazioni_"+LG, "Str85" , "" );

   Text0=ini->ReadString( "Grafici_"+LG, "Str60" , "" );
   Text1=ini->ReadString( "Dati test_"+LG, "Str20" , "" );
   Text2=ini->ReadString( "Dati test_"+LG, "Str30" , "" );
   Text3=ini->ReadString( "Dati test_"+LG, "Str40" , "" );
   Text4=ini->ReadString( "Dati test_"+LG, "Str50" , "" );
   Text5=ini->ReadString( "Dati test_"+LG, "Str60" , "" );
   Text6=ini->ReadString( "Dati test_"+LG, "Str70" , "" );
   Text7=ini->ReadString( "Dati test_"+LG, "Str80" , "" );
   Text8=ini->ReadString( "Dati test_"+LG, "Str90" , "" );
   Text9=ini->ReadString( "Dati test_"+LG, "Str100" , "" );
   Text10=ini->ReadString( "Dati test_"+LG, "Str110" , "" );
   Text11=ini->ReadString( "Dati test_"+LG, "Str120" , "" );
   Text12=ini->ReadString( "Dati test_"+LG, "Str130" , "" );
   Text13=ini->ReadString( "Dati test_"+LG, "Str140" , "" );
   Text14=ini->ReadString( "Dati test_"+LG, "Str150" , "" );
   Text15=ini->ReadString( "Dati test_"+LG, "Str160" , "" );
   Text16=ini->ReadString( "Dati test_"+LG, "Str170" , "" );
   Text17=ini->ReadString( "Dati test_"+LG, "Str180" , "" );

   TextAmbPresOilHum=ini->ReadString( "Dati test_"+LG, "Str220" , "" );
   TextConsSpec=ini->ReadString( "Dati test_"+LG, "Str210" , "" );
   TextRPMminmax=ini->ReadString( "Dati test_"+LG, "Str230" , "" );

   Text90=ini->ReadString( "Layout_"+LG, "Str10" , "" );

   TabSheet2->Caption=Text90;
   TabSheet3->Caption=ini->ReadString( "Layout_"+LG, "Str20" , "" );
   TabSheet4->Caption=ini->ReadString( "Layout_"+LG, "Str30" , "" );

   RadioGroup1->Caption=ini->ReadString( "Layout_"+LG, "Str40" , "" );
   StaticText20->Caption=ini->ReadString( "Layout_"+LG, "Str70" , "" );
   GroupBox2->Caption=ini->ReadString( "Layout_"+LG, "Str80" , "" );
   GroupBox1->Caption=ini->ReadString( "Layout_"+LG, "Str95" , "" );
   Label1->Caption=ini->ReadString( "Layout_"+LG, "Str100" , "" );
   Label2->Caption=ini->ReadString( "Layout_"+LG, "Str110" , "" );


   ComboBox5->Clear();

   ComboBox5->Items->Add(ini->ReadString( "Layout_"+LG, "Str180" , "" ));
   normSAE = ini->ReadString( "Layout_"+LG, "Str180" , "" );
   ComboBox5->Items->Add(ini->ReadString( "Layout_"+LG, "Str190" , "" ));
   normISO = ini->ReadString( "Layout_"+LG, "Str190" , "" );
   ComboBox5->Items->Add(ini->ReadString( "Layout_"+LG, "Str200" , "" ));
   normCE = ini->ReadString( "Layout_"+LG, "Str200" , "" );
   ComboBox5->Items->Add(ini->ReadString( "Layout_"+LG, "Str210" , "" ));
   normECE = ini->ReadString( "Layout_"+LG, "Str210" , "" );
   ComboBox5->Items->Add(ini->ReadString( "Layout_"+LG, "Str220" , "" ));
   normCEE = ini->ReadString( "Layout_"+LG, "Str220" , "" );
   ComboBox5->Items->Add(ini->ReadString( "Layout_"+LG, "Str230" , "" ));
   normDIN = ini->ReadString( "Layout_"+LG, "Str230" , "" );
   ComboBox5->Items->Add(ini->ReadString( "Layout_"+LG, "Str235" , "" ));
   normOCDE = ini->ReadString( "Layout_"+LG, "Str235" , "" );

   ComboBox5->Items->Add(ini->ReadString( "Dati Test", "Str120" , "" ));


   Chart2->BottomAxis->Title->Caption=ini->ReadString( "Grafici", "Str280" , "" );
   Chart2->Title->Caption=ini->ReadString( "Grafici_"+LG, "Str290" , "" );
   TabSheet1->Caption=ini->ReadString( "Layout_"+LG, "Str240" , "" );
   TabSheet5->Caption=ini->ReadString( "Layout_"+LG, "Str250" , "" );


   Series14->CustomVertAxis->Title->Caption=ini->ReadString( "Layout_"+LG, "Str260" , "" );
   StaticText1->Caption=ini->ReadString( "Layout_"+LG, "Str260" , "" );


   Eliminadefinitivamenteiltest1->Caption=ini->ReadString( "Layout_"+LG, "Str270" , "" );
   Rinominailtest1->Caption=ini->ReadString( "Layout_"+LG, "Str280" , "" );
   Aggiunginote1->Caption=ini->ReadString( "Layout_"+LG, "Str290" , "" );

   MenuItem1->Caption=ini->ReadString( "Layout_"+LG, "Str270" , "" );
   MenuItem2->Caption=ini->ReadString( "Layout_"+LG, "Str280" , "" );
   Aggiungimodificanotedeltest1->Caption=ini->ReadString( "Layout_"+LG, "Str290" , "" );

   Eliminadefinitivamenteiltest2->Caption=ini->ReadString( "Layout_"+LG, "Str270" , "" );
   Rinominailtest2->Caption=ini->ReadString( "Layout_"+LG, "Str280" , "" );
   Aggiungimodificanotedeltest2->Caption=ini->ReadString( "Layout_"+LG, "Str290" , "" );

   Eliminadefinitivamenteiltest3->Caption=ini->ReadString( "Layout_"+LG, "Str270" , "" );
   Rinominailtest3->Caption=ini->ReadString( "Layout_"+LG, "Str280" , "" );
   Aggiungimodificanotedeltest3->Caption=ini->ReadString( "Layout_"+LG, "Str290" , "" );

   Eliminadefinitivamenteiltest4->Caption=ini->ReadString( "Layout_"+LG, "Str270" , "" );
   Rinominailtest4->Caption=ini->ReadString( "Layout_"+LG, "Str280" , "" );
   Aggiungimodificanotedeltest4->Caption=ini->ReadString( "Layout_"+LG, "Str290" , "" );

   Eliminadefinitivamenteiltest5->Caption=ini->ReadString( "Layout_"+LG, "Str270" , "" );
   Rinominailtest5->Caption=ini->ReadString( "Layout_"+LG, "Str280" , "" );
   Aggiungimodificanotedeltest6->Caption=ini->ReadString( "Layout_"+LG, "Str290" , "" );


   Eliminadefinitivamenteiltest6->Caption=ini->ReadString( "Layout_"+LG, "Str270" , "" );
   Rinominailtest6->Caption=ini->ReadString( "Layout_"+LG, "Str280" , "" );
   Aggiungimodificanotedeltest7->Caption=ini->ReadString( "Layout_"+LG, "Str290" , "" );

   Eliminadefinitivamenteiltest7->Caption=ini->ReadString( "Layout_"+LG, "Str270" , "" );
   Rinominailtest7->Caption=ini->ReadString( "Layout_"+LG, "Str280" , "" );
   Aggiungimodificanotedeltest8->Caption=ini->ReadString( "Layout_"+LG, "Str290" , "" );


   Anteprimadati1->Caption=ini->ReadString( "Layout_"+LG, "Str1510" , "" );
   Anteprimadati2->Caption=ini->ReadString( "Layout_"+LG, "Str1510" , "" );
   AnteprimaDati3->Caption=ini->ReadString( "Layout_"+LG, "Str1510" , "" );
   Anteprimadati4->Caption=ini->ReadString( "Layout_"+LG, "Str1510" , "" );
   Anteprimadati5->Caption=ini->ReadString( "Layout_"+LG, "Str1510" , "" );
   Anteprimadati6->Caption=ini->ReadString( "Layout_"+LG, "Str1510" , "" );
   Anteprimadati7->Caption=ini->ReadString( "Layout_"+LG, "Str1510" , "" );
   Anteprimadati8->Caption=ini->ReadString( "Layout_"+LG, "Str1510" , "" );

   test1->Caption=ini->ReadString( "Layout_"+LG, "Str330" , "" );
   Aprimisurazioni1->Caption=ini->ReadString( "Layout_"+LG, "Str340" , "" );
   Nuovasessioneditest1->Caption=ini->ReadString( "Layout_"+LG, "Str350" , "" );
   Salvamisurazione1->Caption=ini->ReadString( "Layout_"+LG, "Str360" , "" );
   Stampatest1->Caption=ini->ReadString( "Layout_"+LG, "Str380" , "" );
   Setupdistampa1->Caption=ini->ReadString( "Layout_"+LG, "Str390" , "" );
   Esci1->Caption=ini->ReadString( "Layout_"+LG, "Str400" , "" );
   Installazioni1->Caption=ini->ReadString( "Layout_"+LG, "Str410" , "" );
   Setupsistema1->Caption=ini->ReadString( "Layout_"+LG, "Str420" , "" );
   Setupsistema2->Caption=ini->ReadString( "Layout_"+LG, "Str430" , "" );
   Esportafileditaratura1->Caption=ini->ReadString( "Layout_"+LG, "Str440" , "" );
   Fileditaraturacelle1->Caption=ini->ReadString( "Layout_"+LG, "Str450" , "" );
   Esportafileprofiloutenti1->Caption=ini->ReadString( "Layout_"+LG, "Str460" , "" );
   Fileditaraturacelle2->Caption=ini->ReadString( "Layout_"+LG, "Str450" , "" );
   Unitdimisura1->Caption=ini->ReadString( "Layout_"+LG, "Str470" , "" );
   Impostazionistrumenti1->Caption=ini->ReadString( "Layout_"+LG, "Str480" , "" );
   Impostazionigrafici1->Caption=ini->ReadString( "Layout_"+LG, "Str490" , "" );
   Sequenzaparametriditest1->Caption=ini->ReadString( "Layout_"+LG, "Str500" , "" );
   Utenti1->Caption=ini->ReadString( "Layout_"+LG, "Str510" , "" );
   Cambiautente1->Caption=ini->ReadString( "Layout_"+LG, "Str520" , "" );
   Info1->Caption="?";//=ini->ReadString( "Layout_"+LG, "Str530" , "" );
   Informazionisu1->Caption=ini->ReadString( "Layout_"+LG, "Str540" , "" );
//   Indice1->Caption=ini->ReadString( "Layout_"+LG, "Str550" , "" );
   exportTables->Caption=ini->ReadString( "Layout_"+LG, "Str1900","Export tables");
   SpeedButton5->Caption=ini->ReadString( "Pulsanti_"+LG, "Str10" , "" );
   SpeedButton7->Caption=ini->ReadString( "Unita_"+LG, "Str20" , "" );
   SpeedButton10->Caption=ini->ReadString( "Unita_"+LG, "Str30" , "" );
   SpeedButton11->Caption=ini->ReadString( "Pulsanti_"+LG, "Str50" , "" );
   SpeedButton12->Caption=ini->ReadString( "Pulsanti_"+LG, "Str60" , "" );


   SpeedButton26->Caption=ini->ReadString( "Pulsanti_"+LG, "Str90" , "" );

  Carica_File->Label1->Caption=ini->ReadString( "Archivio_"+LG, "Str10" , "" );
  Carica_File->Label2->Caption=ini->ReadString( "Archivio_"+LG, "Str20" , "" );
  Carica_File->Label6->Caption=ini->ReadString( "Archivio_"+LG, "Str50" , "" );

  Carica_File->BitBtn1->Caption=ini->ReadString( "Archivio_"+LG, "Str70" , "" );
  Carica_File->BitBtn2->Caption=ini->ReadString( "Archivio_"+LG, "Str60" , "" );
  TextRapporto=ini->ReadString( "Archivio_"+LG, "Str130" , "" );

  Carica_File->Series1->Title=ini->ReadString( "Grafici_"+LG, "Str20" , "" );
  Carica_File->Series2->Title=ini->ReadString( "Grafici_"+LG, "Str40" , "" );

  Carica_File->Eliminafile1->Caption=ini->ReadString( "Layout_"+LG, "Str275" , "" );
  Carica_File->Rinominalasessione1->Caption=ini->ReadString( "Layout_"+LG, "Str285" , "" );

  SaveDialog1->Title=ini->ReadString( "Archivio_"+LG, "Str170" , "" );
  SaveDialog1->Filter=ini->ReadString( "Archivio_"+LG, "Str180" , "" )+"|*.dyn";

  Text42=ini->ReadString( "Archivio_"+LG, "Str80" , "" );

  Connessione->Caption=ini->ReadString( "Comunicazione_"+LG, "Str40" , "" );
  Coppia->Caption=SpeedButton7->Caption;
  Giri_Mot->Caption=ini->ReadString( "Grafici_"+LG, "Str340" , "" );;
  Map->Caption=ini->ReadString( "Pulsanti_"+LG, "Str50" , "" );
  Acc->Caption=ini->ReadString( "Indicatori_"+LG, "Str40" , "" );
  Potenza->Caption=ini->ReadString( "Unita_"+LG, "Str30" , "" );
  Termo->Caption=ini->ReadString( "Pulsanti_"+LG, "Str60" , "" );
  Oil_Temp->Caption=ini->ReadString( "Pulsanti_"+LG, "Str90" , "" );

  Misure->Caption=ini->ReadString( "Unita_"+LG, "Str10" , "" );
  Misure->TabSheet1->Caption=ini->ReadString( "Unita_"+LG, "Str20" , "" );
  Misure->TabSheet2->Caption=ini->ReadString( "Unita_"+LG, "Str30" , "" );
  Misure->TabSheet3->Caption=ini->ReadString( "Unita_"+LG, "Str60" , "" );
  Misure->TabSheet5->Caption=ini->ReadString( "Unita_"+LG, "Str40" , "" );
  Misure->TabSheet6->Caption=ini->ReadString( "Unita_"+LG, "Str65" , "" );
  Misure->TabSheet4->Caption=ini->ReadString( "Unita_"+LG, "Str280" , "" );

  Misure->ListBox1->Clear();
  Misure->ListBox1->Items->Add(ini->ReadString( "Unita_"+LG, "Str80" , "" ));
  Misure->ListBox1->Items->Add(ini->ReadString( "Unita_"+LG, "Str90" , "" ));
  Misure->ListBox1->Items->Add(ini->ReadString( "Unita_"+LG, "Str100" , "" ));
  Misure->ListBox2->Clear();
  Misure->ListBox2->Items->Add(ini->ReadString( "Unita_"+LG, "Str110" , "" ));
  Misure->ListBox2->Items->Add(ini->ReadString( "Unita_"+LG, "Str120" , "" ));
  Misure->ListBox2->Items->Add(ini->ReadString( "Unita_"+LG, "Str130" , "" ));
  Misure->ListBox5->Clear();
  Misure->ListBox5->Items->Add(ini->ReadString( "Unita_"+LG, "Str140" , "" ));
  Misure->ListBox5->Items->Add(ini->ReadString( "Unita_"+LG, "Str150" , "" ));

  Misure->ListBox6->Clear();
  Misure->ListBox6->Items->Add(ini->ReadString( "Unita_"+LG, "Str230" , "" ));
  Misure->ListBox6->Items->Add(ini->ReadString( "Unita_"+LG, "Str240" , "" ));
  Misure->ListBox6->Items->Add(ini->ReadString( "Unita_"+LG, "Str250" , "" ));

  Misure->ListBox4->Clear();
  Misure->ListBox4->Items->Add("l/h");
  Misure->ListBox4->Items->Add("l/min");
//  Misure->ListBox4->Items->Add("l/Km");
  Misure->ListBox4->Items->Add("g/Kwh");
  //modifiche per aggiungere l
  Misure->ListBox4->Items->Add("l");



  Misure->ListBox3->Clear();
  Misure->ListBox3->Items->Add(ini->ReadString( "Unita_"+LG, "Str190" , "" ));
  Misure->ListBox3->Items->Add(ini->ReadString( "Unita_"+LG, "Str200" , "" ));



  // CARICA ELENCO DEI TEST
  Impostazioni->ComboBox1->Clear();


  // trattore/motore
  
  Impostazioni->ComboBox1->Items->Add(ini->ReadString( "Impostazioni_"+LG, "Str20" , "" ));
  Impostazioni->ComboBox1->Items->Add(ini->ReadString( "Impostazioni_"+LG, "Str10" , "" ));
  Impostazioni->ComboBox1->Items->Add(ini->ReadString( "Impostazioni_"+LG, "str85" , "" )); //str40
  Impostazioni->ComboBox1->Items->Add(ini->ReadString( "Impostazioni_"+LG, "str70" , "" ));///str80
  Impostazioni->ComboBox1->Items->Add(ini->ReadString( "Impostazioni_"+LG, "str40" , "" ));//str85
  
  Impostazioni->GroupBox2->Caption=ini->ReadString( "Impostazioni_"+LG, "Str100" , "" );
  Impostazioni->StaticText2->Caption=ini->ReadString( "Impostazioni_"+LG, "Str290" , "" );
  Impostazioni->Label1->Caption=ini->ReadString( "Grafici_"+LG, "Str70" , "" );
  Impostazioni->Label2->Caption=ini->ReadString( "Grafici_"+LG, "Str70" , "" );
  Impostazioni->StaticText3->Caption=ini->ReadString( "Impostazioni_"+LG, "Str300" , "" );
  Impostazioni->StaticText4->Caption=ini->ReadString( "Impostazioni_"+LG, "Str310" , "" );
  Impostazioni->Label3->Caption=ini->ReadString( "Impostazioni_"+LG, "Str320" , "" );
  Impostazioni->StaticText10->Caption=ini->ReadString( "Impostazioni_"+LG, "Str290" , "" );
  Impostazioni->StaticText11->Caption=ini->ReadString( "Impostazioni_"+LG, "Str300" , "" );
  Impostazioni->StaticText12->Caption=ini->ReadString( "Impostazioni_"+LG, "Str390" , "" );
  Impostazioni->StaticText13->Caption=ini->ReadString( "Dati test_"+LG, "Str180" , "" );
  Impostazioni->Label12->Caption=ini->ReadString( "Impostazioni_"+LG, "Str320" , "" );
  Impostazioni->Label9->Caption=ini->ReadString( "Grafici_"+LG, "Str70" , "" );
  Impostazioni->Label10->Caption=ini->ReadString( "Grafici_"+LG, "Str70" , "" );
  Impostazioni->Label11->Caption=ini->ReadString( "Grafici_"+LG, "Str70" , "" );
  Impostazioni->StaticText14->Caption=ini->ReadString( "Impostazioni_"+LG, "Str290" , "" );
  Impostazioni->StaticText15->Caption=ini->ReadString( "Impostazioni_"+LG, "Str300" , "" );
  Impostazioni->StaticText16->Caption=ini->ReadString( "Impostazioni_"+LG, "Str390" , "" );
  Impostazioni->Label13->Caption=ini->ReadString( "Grafici_"+LG, "Str70" , "" );
  Impostazioni->Label14->Caption=ini->ReadString( "Grafici_"+LG, "Str70" , "" );
  Impostazioni->Label15->Caption=ini->ReadString( "Grafici_"+LG, "Str70" , "" );
  Impostazioni->StaticText5->Caption=ini->ReadString( "Impostazioni_"+LG, "Str290" , "" );
  Impostazioni->StaticText9->Caption=ini->ReadString( "Impostazioni_"+LG, "Str300" , "" );
  Impostazioni->StaticText17->Caption=ini->ReadString( "Impostazioni_"+LG, "Str315" , "" );
  Impostazioni->Label18->Caption=ini->ReadString( "Grafici_"+LG, "Str70" , "" );
  Impostazioni->Label8->Caption=ini->ReadString( "Grafici_"+LG, "Str70" , "" );
  Impostazioni->Label46->Caption=ini->ReadString( "Grafici_"+LG, "Str70" , "" );

  Impostazioni->Label4->Caption=ini->ReadString( "Impostazioni_"+LG, "Str320" , "" );
  Impostazioni->Label19->Caption=ini->ReadString( "Impostazioni_"+LG, "Str400" , "" );
  Impostazioni->GroupBox1->Caption=ini->ReadString( "Impostazioni_"+LG, "Str120" , "" );
  Impostazioni->ComboBox6->Clear();
  Impostazioni->ComboBox6->Items->Add(ini->ReadString( "Impostazioni_"+LG, "Str440" , "" ));
  //Impostazioni->ComboBox6->Items->Add(ini->ReadString( "Impostazioni_"+LG, "Str450" , "" ));
  Impostazioni->ComboBox6->Items->Add(ini->ReadString( "Impostazioni_"+LG, "Str460" , "" ));

  Impostazioni->StaticText21->Caption=ini->ReadString( "Impostazioni_"+LG, "Str470" , "" );
  Impostazioni->StaticText22->Caption=ini->ReadString( "Impostazioni_"+LG, "Str480" , "" );
  Impostazioni->Label16->Caption=ini->ReadString( "Grafici_"+LG, "Str70" , "" );
  Impostazioni->StaticText28->Caption=ini->ReadString( "Dati test_"+LG, "Str30" , "" );
  Impostazioni->GroupBox9->Caption=ini->ReadString( "Impostazioni_"+LG, "Str170" , "" );
  Impostazioni->Label27->Caption=ini->ReadString( "Impostazioni_"+LG, "Str180" , "" );
  Impostazioni->Label28->Caption=ini->ReadString( "Impostazioni_"+LG, "Str190" , "" );
  Impostazioni->GroupBox8->Caption=ini->ReadString( "Impostazioni_"+LG, "Str200" , "" );
  Impostazioni->StaticText37->Caption=ini->ReadString( "Impostazioni_"+LG, "Str210" , "" );
  Impostazioni->StaticText38->Caption=ini->ReadString( "Impostazioni_"+LG, "Str220" , "" );
  Impostazioni->StaticText39->Caption=ini->ReadString( "Impostazioni_"+LG, "Str230" , "" );
  Impostazioni->StaticText42->Caption=ini->ReadString( "Impostazioni_"+LG, "Str240" , "" );
  Impostazioni->StaticText41->Caption=ini->ReadString( "Impostazioni_"+LG, "Str250" , "" );
  Impostazioni->BitBtn1->Caption=ini->ReadString( "Pulsanti_"+LG, "Str120" , "" );
  Impostazioni->Label39->Caption=ini->ReadString( "Impostazioni_"+LG, "Str290" , "" );
  
  Setup->GroupBox10->Caption=ini->ReadString( "Impostazioni_"+LG, "Str600","Utilization");
  txtLabelContaore=ini->ReadString( "Impostazioni_"+LG, "Str610","Hours");
  txtLabelLanci=ini->ReadString( "Impostazioni_"+LG, "Str620","Launches");

  Mt_cost->Label1->Caption=ini->ReadString( "Layout_"+LG, "Str1380" , "" );
  Mt_cost->BitBtn1->Caption=ini->ReadString( "Pulsanti_"+LG, "Str100" , "" );

  Note->Caption=ini->ReadString( "Layout_"+LG, "Str560" , "" );
  Note->BitBtn2->Caption=ini->ReadString( "Pulsanti_"+LG, "Str110" , "" );
  Note->BitBtn3->Caption=ini->ReadString( "Pulsanti_"+LG, "Str100" , "" );
  Note->BitBtn1->Caption=ini->ReadString( "Pulsanti_"+LG, "Str120" , "" );

  Opzioni_grafici->Caption=ini->ReadString( "Layout_"+LG, "Str570" , "" );

  Opzioni_grafici->GroupBox1->Caption=ini->ReadString( "Unita_"+LG, "Str30" , "" );
  Opzioni_grafici->GroupBox2->Caption=ini->ReadString( "Unita_"+LG, "Str20" , "" );


  Opzioni_grafici->CheckBox10->Caption=ini->ReadString( "Grafici_"+LG, "Str230" , "" );

  Text206=ini->ReadString( "Grafici_"+LG, "Str230" , "" );
  Text207=ini->ReadString( "Grafici_"+LG, "Str240" , "" );

  Opzioni_grafici->CheckBox12->Caption=ini->ReadString( "Grafici_"+LG, "Str230" , "" );

  Opzioni_grafici->GroupBox4->Caption=ini->ReadString( "Layout_"+LG, "Str240" , "" );
  Opzioni_grafici->CheckBox4->Caption=ini->ReadString( "Grafici_"+LG, "Str120" , "" );

  Opzioni_grafici->CheckBox5->Caption=ini->ReadString( "Grafici_"+LG, "Str140" , "" );
//  Opzioni_grafici->CheckBox7->Caption=ini->ReadString( "Grafici_"+LG, "Str160" , "" );
  Opzioni_grafici->CheckBox8->Caption=ini->ReadString( "Grafici_"+LG, "Str150" , "" );



  Opzioni_grafici->BitBtn1->Caption=ini->ReadString( "Pulsanti_"+LG, "Str120" , "" );

  Pilot_Freno->Caption=ini->ReadString( "Layout_"+LG, "Str590" , "" );
  Pilot_Freno->BitBtn2->Caption=ini->ReadString( "Pulsanti_"+LG, "Str130" , "" );
  Pilot_Freno->BitBtn1->Caption=ini->ReadString( "Pulsanti_"+LG, "Str140" , "" );

  Text45=ini->ReadString( "Layout_"+LG, "Str600" , "" );
  Text46=ini->ReadString( "Layout_"+LG, "Str610" , "" );

  Rapporto->Caption=ini->ReadString( "Layout_"+LG, "Str620" , "" );
  Rapporto->BitBtn1->Caption=ini->ReadString( "Pulsanti_"+LG, "Str150" , "" );
  Rapporto->BitBtn2->Caption=ini->ReadString( "Pulsanti_"+LG, "Str100" , "" );


  Setup->Caption=ini->ReadString( "Layout_"+LG, "Str630" , "" );
  Setup->VrLabel2->Caption=ini->ReadString( "Layout_"+LG, "Str650" , "" );
  Setup->GroupBox4->Caption=ini->ReadString( "Layout_"+LG, "Str660" , "" );
  /// Impostazioni Setup banco
  Setup->GroupBox2->Caption=ini->ReadString( "Layout_"+LG, "Str670" , "" );
  Setup->ComboBox3->Clear();
  Setup->ComboBox3->Items->Add(ini->ReadString( "Layout_"+LG, "Str680" , "" ));
  Setup->ComboBox3->Items->Add(ini->ReadString( "Layout_"+LG, "Str690" , "" ));
  Setup->ComboBox3->Items->Add(ini->ReadString( "Layout_"+LG, "Str720" , "" ));
  switch(tipoBanco)
  {
    case 0:
      StatusBar1->Panels->Items[0]->Text = ini->ReadString( "Layout_"+LG, "Str680" , "" );
    break;
    case 1:
      StatusBar1->Panels->Items[0]->Text = ini->ReadString( "Layout_"+LG, "Str690" , "" );
    break;    
    case 2:
      StatusBar1->Panels->Items[0]->Text = ini->ReadString( "Layout_"+LG, "Str720" , "" );
    break;
  }






   Setup->GroupBox3->Caption=ini->ReadString( "Layout_"+LG, "Str740" , "" );
   Text56 = ini->ReadString( "Layout_"+LG, "Str750" , "" );
   Setup->GroupBox8->Caption=ini->ReadString( "Layout_"+LG, "Str95" , "" );
   Setup->GroupBox7->Caption=ini->ReadString( "Layout_"+LG, "Str770" , "" );
   Setup->GroupBox6->Caption=ini->ReadString( "Layout_"+LG, "Str780" , "" );
   Setup->StaticText7->Caption=ini->ReadString( "Layout_"+LG, "Str790" , "" );
   Setup->stComDyn3->Caption=ini->ReadString( "Layout_"+LG, "Str790" , "" );
   Setup->stComDyra->Caption=ini->ReadString( "Layout_"+LG, "Str790" , "" );
   Setup->TabSheet1->Caption=ini->ReadString( "Layout_"+LG, "Str650" , "" );

   Setup->VrLabel3->Caption=ini->ReadString( "Layout_"+LG, "Str640" , "" );
   Text47=ini->ReadString( "Layout_"+LG, "Str800" , "" );
   Text48=ini->ReadString( "Layout_"+LG, "Str810" , "" );
   Text49=ini->ReadString( "Layout_"+LG, "Str820" , "" );
   Text50=ini->ReadString( "Impostazioni_"+LG, "Str250" , "" );
   Text51=ini->ReadString( "Impostazioni_"+LG, "Str240" , "" );
   Text52=ini->ReadString( "Impostazioni_"+LG, "Str210" , "" );
   Text53=ini->ReadString( "Impostazioni_"+LG, "Str220" , "" );
   Text54=ini->ReadString( "Impostazioni_"+LG, "Str230" , "" );
   Text55=ini->ReadString( "Dati test_"+LG, "Str20" , "" );

   Text58=ini->ReadString( "Layout_"+LG, "Str830" , "" );
   Text57=ini->ReadString( "Impostazioni_"+LG, "Str290" , "" );

   Setup->StaticText1->Caption=ini->ReadString( "Layout_"+LG, "Str795" , "" );
   Setup->StaticText2->Caption=Text48;
   Setup->StaticText3->Caption=Text49;
   Setup->SpeedButton1->Caption=ini->ReadString( "Layout_"+LG, "Str825" , "" );
   Setup->TabSheet2->Caption=ini->ReadString( "Layout_"+LG, "Str640" , "" );

   // cella ant
   Setup->TKLabel->Caption=ini->ReadString( "Layout_"+LG, "Str840" , "" );
   Setup->Label22->Caption=ini->ReadString( "Layout_"+LG, "Str860" , "" );
   Setup->Label5->Caption=ini->ReadString( "Layout_"+LG, "Str870" , "" );
   Setup->Label6->Caption=ini->ReadString( "Layout_"+LG, "Str880" , "" );
   Setup->Label9->Caption=ini->ReadString( "Layout_"+LG, "Str890" , "" );
   Setup->GroupBox1->Caption=ini->ReadString( "Layout_"+LG, "Str900" , "" );
   Setup->RadioButton1->Caption=ini->ReadString( "Layout_"+LG, "Str910" , "" );
   Setup->RadioButton2->Caption=ini->ReadString( "Layout_"+LG, "Str920" , "" );
   Setup->SpeedButton2->Caption=ini->ReadString( "Layout_"+LG, "Str930" , "" );
   Setup->Label2->Caption=ini->ReadString( "Layout_"+LG, "Str940" , "" );
   Setup->Label3->Caption=ini->ReadString( "Layout_"+LG, "Str950" , "" );
   Setup->Label4->Caption=ini->ReadString( "Layout_"+LG, "Str960" , "" );
   Setup->BitBtn6->Caption=ini->ReadString( "Layout_"+LG, "Str970" , "" );
   Setup->TabSheet3->Caption=ini->ReadString( "Layout_"+LG, "Str840" , "" );
   Setup->BitBtn2->Caption=ini->ReadString( "Pulsanti_"+LG, "Str150" , "" );
   Setup->BitBtn3->Caption=ini->ReadString( "Pulsanti_"+LG, "Str150" , "" );
   Setup->BitBtn4->Caption=ini->ReadString( "Pulsanti_"+LG, "Str150" , "" );

   Setup->GroupBox9->Caption=ini->ReadString( "Layout_"+LG, "Str1700" , "" );
   Setup->NxButton1->Caption=ini->ReadString( "Pulsanti_"+LG, "Str150" , "" );

   // cella post
/*
   Setup->VrLabel1->Caption=ini->ReadString( "Layout", "Str850" , "" );
   Setup->Label20->Caption=ini->ReadString( "Layout", "Str860" , "" );
   Setup->Label1->Caption=ini->ReadString( "Layout", "Str870" , "" );
   Setup->Label11->Caption=ini->ReadString( "Layout", "Str880" , "" );
   Setup->Label12->Caption=ini->ReadString( "Layout", "Str890" , "" );
   Setup->GroupBox5->Caption=ini->ReadString( "Layout", "Str900" , "" );
   Setup->RadioButton3->Caption=ini->ReadString( "Layout", "Str910" , "" );
   Setup->RadioButton4->Caption=ini->ReadString( "Layout", "Str920" , "" );
   Setup->Label18->Caption=ini->ReadString( "Layout", "Str940" , "" );
   Setup->Label17->Caption=ini->ReadString( "Layout", "Str950" , "" );
   Setup->Label16->Caption=ini->ReadString( "Layout", "Str960" , "" );
   Setup->BitBtn5->Caption=ini->ReadString( "Layout", "Str970" , "" );
   Setup->TabSheet4->Caption=ini->ReadString( "Layout", "Str850" , "" );
   Setup->BitBtn7->Caption=ini->ReadString( "Pulsanti", "Str150" , "" );
   Setup->BitBtn8->Caption=ini->ReadString( "Pulsanti", "Str150" , "" );
   Setup->BitBtn9->Caption=ini->ReadString( "Pulsanti", "Str150" , "" );
*/
   GroupBox11->Caption=ini->ReadString( "Layout_"+LG, "Str990" , "" );

   Label28->Caption=ini->ReadString( "Layout_"+LG, "Str95" , "" )+":";
   Label29->Caption=ini->ReadString( "Layout_"+LG, "Str770" , "" )+":";
   VrLabel13->Caption=ini->ReadString( "Layout_"+LG, "Str980" , "" );

   Stampadeigrafici1->Caption= ini->ReadString( "Layout_"+LG, "Str240" , "" );
   Stampadelletabelle1->Caption= ini->ReadString( "Layout_"+LG, "Str250" , "" );
   Stampadatideitest1->Caption= ini->ReadString( "Layout_"+LG, "Str30" , "" );
   Taco1->Caption=ini->ReadString( "Layout_"+LG, "Str320" , "" );
   GroupBox3->Caption=ini->ReadString( "Layout_"+LG, "Str1040" , "" );
   GroupBox4->Caption=ini->ReadString( "Layout_"+LG, "Str1040" , "" )+" (1)";

   Label4->Caption=ini->ReadString( "Layout_"+LG, "Str1050" , "" );
   Label8->Caption=ini->ReadString( "Layout_"+LG, "Str1050" , "" );   
   Label5->Caption=ini->ReadString( "Layout_"+LG, "Str1060" , "" );
   Label9->Caption=ini->ReadString( "Layout_"+LG, "Str1060" , "" );
   TabSheet9->Caption=ini->ReadString( "Layout_"+LG, "Str980" , "" );

   Setup->BitBtn1->Caption=ini->ReadString( "Pulsanti_"+LG, "Str120" , "" );
   Text59=ini->ReadString( "Messaggi_"+LG, "Str100" , "" );
   Text60=ini->ReadString( "Messaggi_"+LG, "Str110" , "" );
   Text61=ini->ReadString( "Messaggi_"+LG, "Str120" , "" );
   Text63=ini->ReadString( "Messaggi_"+LG, "Str130" , "" );
   Text62=ini->ReadString( "Messaggi_"+LG, "Str140" , "" );

   Start->Label1->Caption=ini->ReadString( "Layout_"+LG, "Str1110" , "" );
   Text86 = ini->ReadString( "Layout_"+LG, "Str1120" , "" );
   Start->BitBtn1->Caption=ini->ReadString( "Pulsanti_"+LG, "Str120" , "" );
   Start->BitBtn2->Caption=ini->ReadString( "Pulsanti_"+LG, "Str100" , "" );

   Step_test->Label3->Caption=ini->ReadString( "Grafici_"+LG, "Str70" , "" );
   Step_test->Label4->Caption=ini->ReadString( "Dati test_"+LG, "Str200" , "" );
   Step_test->BitBtn4->Caption=ini->ReadString( "Pulsanti_"+LG, "Str160" , "" );
   Step_test->BitBtn3->Caption=ini->ReadString( "Pulsanti_"+LG, "Str100" , "" );


   Strumenti->Caption=ini->ReadString( "Layout_"+LG, "Str480" , "" );
   Strumenti->TabSheet1->Caption=ini->ReadString( "Layout_"+LG, "Str1130" , "" );

   Strumenti->TabSheet3->Caption=ini->ReadString( "Grafici_"+LG, "Str70" , "" ) + "-" +
                                 ini->ReadString( "Unita_"+LG, "Str20" , "" ) + "-" +
                                 ini->ReadString( "Unita_"+LG, "Str30" , "" );

   Strumenti->GroupBox3->Caption=ini->ReadString( "Grafici_"+LG, "Str70" , "" );
   Strumenti->GroupBox4->Caption=ini->ReadString( "Grafici_"+LG, "Str40" , "" );
   Strumenti->GroupBox5->Caption=ini->ReadString( "Grafici_"+LG, "Str20" , "" );

   Strumenti->TabSheet4->Caption=ini->ReadString( "Indicatori_"+LG, "Str30" , "" ) + "-" +
                                 ini->ReadString( "Grafici_"+LG, "Str270" , "" );

   Strumenti->GroupBox6->Caption=ini->ReadString( "Indicatori_"+LG, "Str30" , "" );
   Strumenti->GroupBox7->Caption=ini->ReadString( "Grafici_"+LG, "Str270" , "" );
   Strumenti->SpeedButton3->Caption=ini->ReadString( "Layout_"+LG, "Str1140" , "" );

   Strumenti->TabSheet5->Caption=ini->ReadString( "Indicatori_"+LG, "Str10" , "" );

   Strumenti->GroupBox10->Caption=ini->ReadString( "Indicatori_"+LG, "Str10" , "" );
/*   a.balbo delete
   Strumenti->TabSheet6->Caption=ini->ReadString( "Grafici", "Str160" , "" );

   Strumenti->Label1->Caption=ini->ReadString( "Grafici", "Str160" , "" );

   Strumenti->StaticText1->Caption=ini->ReadString( "Layout", "Str900" , "" );
   Strumenti->SpeedButton1->Caption=ini->ReadString( "Layout", "Str1160" , "" );
   Strumenti->SpeedButton2->Caption=ini->ReadString( "Layout", "Str1170" , "" );
*/
  // Strumenti->BitBtn1->Caption=ini->ReadString( "Pulsanti", "Str120" , "" );

   Strumenti->LabeledEdit1->EditLabel->Caption=ini->ReadString( "Layout_"+LG, "Str1190" , "" );
   Strumenti->LabeledEdit2->EditLabel->Caption=ini->ReadString( "Layout_"+LG, "Str1190" , "" );
   Strumenti->LabeledEdit3->EditLabel->Caption=ini->ReadString( "Layout_"+LG, "Str1190" , "" );
   Strumenti->LabeledEdit4->EditLabel->Caption=ini->ReadString( "Layout_"+LG, "Str1190" , "" );
   Strumenti->LabeledEdit5->EditLabel->Caption=ini->ReadString( "Layout_"+LG, "Str1190" , "" );

   Impostazioni->GroupBox4->Caption=ini->ReadString( "Layout_"+LG, "Str1760" , "" );

   SpeedButton4->Hint = Str17;
   SpeedButton8->Hint = Str16;
   SpeedButton21->Hint = Text44;
   SpeedButton17->Hint = Str23;
   SpeedButton15->Hint = Str19;
   SpeedButton1->Hint = ini->ReadString( "Layout_"+LG, "Str620" , "" );

   Strumenti->LabeledEdit8->EditLabel->Caption=ini->ReadString( "Layout_"+LG, "Str1180" , "" );
   Strumenti->LabeledEdit9->EditLabel->Caption=ini->ReadString( "Layout_"+LG, "Str1190" , "" );
/* a.balbo delete
   Strumenti->Label2->Caption=ini->ReadString( "Layout_"+LG, "Str1180" , "" );
   Strumenti->Label3->Caption=ini->ReadString( "Layout_"+LG, "Str1190" , "" );
*/
   Strumenti->LabeledEdit6->EditLabel->Caption=ini->ReadString( "Layout_"+LG, "Str1180" , "" );

   Strumenti->TabSheet2->Caption=ini->ReadString( "Indicatori_"+LG, "Str50" , "" );


   Strumenti->CheckBox7->Caption=ini->ReadString( "Pulsanti_"+LG, "Str60" , "" );
   Strumenti->cbTAir->Caption="T atm";
   Strumenti->CheckBox6->Caption=ini->ReadString( "Indicatori_"+LG, "Str30" , "" );
   Strumenti->CheckBox8->Caption=ini->ReadString( "Indicatori_"+LG, "Str10" , "" );
   Strumenti->CheckBox1->Caption=ini->ReadString( "Indicatori_"+LG, "Str60" , "" );

   Opzioni_grafici->CheckBoxConsumi->Caption = ini->ReadString( "Indicatori_"+LG, "Str60" , "" );
   TextCons=ini->ReadString( "Indicatori_"+LG, "Str60","" );
   Strumenti->GroupBox2->Caption=ini->ReadString( "Layout_"+LG, "Str1390" , "" );
   Strumenti->CheckBox11->Caption=ini->ReadString( "Layout_"+LG, "Str1400" , "" );
   Strumenti->CheckBox12->Caption=ini->ReadString( "Layout_"+LG, "Str1420" , "" );
   Strumenti->GroupBox13->Caption=ini->ReadString( "Layout_"+LG, "Str1421" , "" );

   Strumenti->GroupBox8->Caption=ini->ReadString( "Layout_"+LG, "Str1410" , "" );

   Utenti->Caption=ini->ReadString( "Layout_"+LG, "Str15" , "" );
   Utenti->Impo->Caption=ini->ReadString( "Layout_"+LG, "Str1130" , "" );
   Utenti->Aggiungiutente1->Caption=ini->ReadString( "Layout_"+LG, "Str1220" , "" );
   Utenti->Eliminautente1->Caption=ini->ReadString( "Layout_"+LG, "Str1230" , "" );
   Text67=ini->ReadString( "Layout_"+LG, "Str1240" , "" );
   Text64=ini->ReadString( "Layout_"+LG, "Str1250" , "" );
   Text65=ini->ReadString( "Layout_"+LG, "Str1260" , "" );
   Text66=ini->ReadString( "Messaggi_"+LG, "Str85" , "" );

   Impostazioni->RadioButton1->Caption=ini->ReadString( "Layout_"+LG, "Str1720" , "" );
   Impostazioni->RadioButton2->Caption=ini->ReadString( "Layout_"+LG, "Str1710" , "" );
   Impostazioni->StaticText6->Caption=ini->ReadString( "Layout_"+LG, "Str1730" , "" );
   Impostazioni->StaticText7->Caption=ini->ReadString( "Layout_"+LG, "Str1740" , "" );
   Impostazioni->NxHeaderPanel1->Caption=ini->ReadString( "Layout_"+LG, "Str1750" , "" );
   Impostazioni->NxLabel2->Caption=ini->ReadString( "Layout_"+LG, "Str1710" , "" );
   Impostazioni->NxLabel1->Caption=ini->ReadString( "Layout_"+LG, "Str1720" , "" );
   Text200=ini->ReadString( "Layout_"+LG, "Str1710" , "" );
   Text201=ini->ReadString( "Layout_"+LG, "Str1720" , "" );
   Text202=ini->ReadString( "Layout_"+LG, "Str1820" , "" );
   Text203=ini->ReadString( "Layout_"+LG, "Str1830" , "" );
   Text204=ini->ReadString( "Layout_"+LG, "Str1840" , "" );
   Text205=ini->ReadString( "Unita_"+LG, "Str280" , "" );
   NxLabel11->Caption = ini->ReadString( "Layout_"+LG, "Str1850" , "" ) + ":";
   textSaveAcq = ini->ReadString( "Layout_"+LG, "Str1860" , "" );
   textCancAll = ini->ReadString( "Layout_"+LG, "Str1870" , "" );
   textAdd  = ini->ReadString( "Layout_"+LG, "Str1880" , "" );
   textSameAcq  = ini->ReadString( "Layout_"+LG, "Str1890" , "" );
   Label3->Caption=ini->ReadString( "Layout_"+LG, "Str1430" , "" );
   Label6->Caption=ini->ReadString( "Layout_"+LG, "Str1440" , "" );
   Label10->Caption=ini->ReadString( "Layout_"+LG, "Str1440" , "" );
   Utenti->CancelBtn->Caption=ini->ReadString( "Pulsanti_"+LG, "Str100" , "" );
   Utenti->OKBtn->Caption=ini->ReadString( "Pulsanti_"+LG, "Str120" , "" );

   Text68=ini->ReadString( "Messaggi_"+LG, "Str150" , "" );
   Text69=ini->ReadString( "Messaggi_"+LG, "Str160" , "" );
   Text70=ini->ReadString( "Messaggi_"+LG, "Str170" , "" );

   Connessione->Caption=ini->ReadString( "Comunicazione_"+LG, "Str40" , "" );
   Connessione->Label1->Caption=ini->ReadString( "Comunicazione_"+LG, "Str10" , "" );
   Connessione->Label2->Caption=ini->ReadString( "Comunicazione_"+LG, "Str20" , "" );
   Connessione->Label3->Caption=ini->ReadString( "Comunicazione_"+LG, "Str30" , "" );

   Text71=ini->ReadString( "Versione_"+LG, "Str10" , "" );
   Text72=ini->ReadString( "Versione_"+LG, "Str20" , "" );
   Text73=ini->ReadString( "Versione_"+LG, "Str30" , "" );

   Text74=ini->ReadString( "Versione_"+LG, "Str40" , "" );

   Text75=ini->ReadString( "Layout_"+LG, "Str1300" , "" );
   Text76=ini->ReadString( "Layout_"+LG, "Str1310" , "" );
   Text77=ini->ReadString( "Layout_"+LG, "Str1320" , "" );
   Text78=ini->ReadString( "Layout_"+LG, "Str1330" , "" );


   Text79=ini->ReadString( "Layout_"+LG, "Str1450" , "" );
   Text80=ini->ReadString( "Layout_"+LG, "Str1460" , "" );
   Text81=ini->ReadString( "Layout_"+LG, "Str1470" , "" );
   Text82=ini->ReadString( "Layout_"+LG, "Str1480" , "" );
   Text83=ini->ReadString( "Layout_"+LG, "Str1490" , "" );
   Text84=ini->ReadString( "Layout_"+LG, "Str1500" , "" );

   Text85=ini->ReadString( "Messaggi_"+LG, "Str180" , "" );

   Text87=ini->ReadString( "Versione_"+LG, "Str50" , "" );

   TextPowerData=ini->ReadString( "Grafici_"+LG, "Str350" , "" );
   TextEnvironmentData=ini->ReadString( "Grafici_"+LG, "Str360" , "" );
   TextTractorData=ini->ReadString( "Grafici_"+LG, "Str370" , "" );
   txtGraphTitle=ini->ReadString( "Layout_"+LG, "Str95" , "" );
   StaticText2->Caption=ini->ReadString( "Layout_"+LG, "Str95" , "" );
   Fattoredicorrezione1->Caption=Text7;
   Fattoredicorrezione2->Caption=Text7;
   Fattoredicorrezione3->Caption=Text7;
   Fattoredicorrezione4->Caption=Text7;
   Fattoredicorrezione5->Caption=Text7;
   Fattoredicorrezione6->Caption=Text7;
   Fattoredicorrezione7->Caption=Text7;
   Fattoredicorrezione8->Caption=Text7;
   CaricaImpostazioni1->Caption=ini->ReadString( "Layout_"+LG, "Str2000" , "Load settings" );
   CaricaImpostazioni2->Caption=ini->ReadString( "Layout_"+LG, "Str2000" , "Load settings" );
   CaricaImpostazioni3->Caption=ini->ReadString( "Layout_"+LG, "Str2000" , "Load settings" );
   CaricaImpostazioni4->Caption=ini->ReadString( "Layout_"+LG, "Str2000" , "Load settings" );
   CaricaImpostazioni5->Caption=ini->ReadString( "Layout_"+LG, "Str2000" , "Load settings" );
   CaricaImpostazioni6->Caption=ini->ReadString( "Layout_"+LG, "Str2000" , "Load settings" );
   CaricaImpostazioni7->Caption=ini->ReadString( "Layout_"+LG, "Str2000" , "Load settings" );
   CaricaImpostazioni8->Caption=ini->ReadString( "Layout_"+LG, "Str2000" , "Load settings" );
   delete ini;


    // titoli del grafico

  String Titolo="";


         if(Vis_P_R) Titolo=Titolo+" " + Str1;
         if(Vis_C_R) Titolo=Titolo+" " + Str3;

         Chart1->Title->Text->Text="";
         Chart1->Title->Text->Text=Titolo;


    String Text_=Str6;
    RichEdit1->Clear();
    RichEdit1->Lines->Add(Text_);



    // ************* impostazione unità di misura ********
   updateUnits();
   carica_unita();

   // aggiorna dati dei test se presenti

   updateRichText();
   updateGrids();

}

void __fastcall TMain::Panel8Resize(TObject *Sender)
{

  CopLabel->Font->Size=int(Panel8->Width /K_WW);
  if(CopLabel->Font->Size > K_Font) CopLabel->Font->Size=K_Font;

}
//---------------------------------------------------------------------------


void __fastcall TMain::Panel10Resize(TObject *Sender)
{
 PotLabel->Font->Size=int(Panel10->Width /K_WW);
  if(PotLabel->Font->Size > K_Font) PotLabel->Font->Size=K_Font;

}
//---------------------------------------------------------------------------

void __fastcall TMain::Panel14Resize(TObject *Sender)
{
 MapLabel->Font->Size=int(Panel14->Width /K_WW);
  if(MapLabel->Font->Size > K_Font) MapLabel->Font->Size=K_Font;
}
//---------------------------------------------------------------------------


void __fastcall TMain::Panel18Resize(TObject *Sender)
{
 OilLabel->Font->Size=int(Panel18->Width /K_WW);
  if(OilLabel->Font->Size > K_Font) OilLabel->Font->Size=K_Font;

}
//---------------------------------------------------------------------------

void __fastcall TMain::Panel20Resize(TObject *Sender)
{
  TKLabel->Font->Size=int(Panel20->Width /K_WW);
  if(TKLabel->Font->Size > K_Font) TKLabel->Font->Size=K_Font;

}
//---------------------------------------------------------------------------






void __fastcall TMain::TrackBar1Change(TObject *Sender)
{
  fFact = TrackBar1->Position;
  fFact /= 1000;
  sprintf(Buffer,"%-1.3f",fFact);
  FcLabel->Caption=Buffer;
  Fattore_c = fFact;

}
//---------------------------------------------------------------------------





void __fastcall TMain::Timer_AllarmTimer(TObject *Sender)
{

   // giri motore
  if(Giri_Mot-> AnalogMeterHQ1->Value < Giri_Mot->AnalogMeterHQ1->RedStartValue &&
     Giri_Mot-> AnalogMeterHQ1->Value >= Giri_Mot->AnalogMeterHQ1->YellowStartValue )
     GiriLabel->Color=clYellow;

   if(Giri_Mot-> AnalogMeterHQ1->Value >= Giri_Mot->AnalogMeterHQ1->RedStartValue)
   GiriLabel->Color=clRed;


    if(Giri_Mot-> AnalogMeterHQ1->Value < Giri_Mot->AnalogMeterHQ1->YellowStartValue )
    GiriLabel->Color=clWhite;
    //


   // termocoppia
  if(Termo-> AnalogMeterHQ1->Value < Termo->AnalogMeterHQ1->RedStartValue &&
     Termo-> AnalogMeterHQ1->Value >= Termo->AnalogMeterHQ1->YellowStartValue )
     TKLabel->Color=clYellow;

   if(Termo-> AnalogMeterHQ1->Value >= Termo->AnalogMeterHQ1->RedStartValue)
   TKLabel->Color=clRed;


    if(Termo-> AnalogMeterHQ1->Value < Termo->AnalogMeterHQ1->YellowStartValue )
    TKLabel->Color=clLime;



       // Map
  if(Map-> AnalogMeterHQ1->Value < Map->AnalogMeterHQ1->RedStartValue &&
     Map-> AnalogMeterHQ1->Value >= Map->AnalogMeterHQ1->YellowStartValue )
     MapLabel->Color=clYellow;

   if(Map-> AnalogMeterHQ1->Value >= Map->AnalogMeterHQ1->RedStartValue)
   MapLabel->Color=clRed;


    if(Map-> AnalogMeterHQ1->Value < Map->AnalogMeterHQ1->YellowStartValue )
    MapLabel->Color=clLime;


    // Tempreratura olio
  if(Oil_Temp-> AnalogMeterHQ1->Value < Oil_Temp->AnalogMeterHQ1->RedStartValue &&
     Oil_Temp-> AnalogMeterHQ1->Value >= Oil_Temp->AnalogMeterHQ1->YellowStartValue )
     OilLabel->Color=clYellow;

   if(Oil_Temp-> AnalogMeterHQ1->Value >= Oil_Temp->AnalogMeterHQ1->RedStartValue)
     OilLabel->Color=clRed;

    if(Oil_Temp-> AnalogMeterHQ1->Value < Oil_Temp->AnalogMeterHQ1->YellowStartValue )
      OilLabel->Color=clLime;





}
//---------------------------------------------------------------------------

void __fastcall TMain::SpeedButton26Click(TObject *Sender)
{

 Oil_Temp->Show();
 Vis_Oil=1;
 SpeedButton26->OnClick = SpeedButton26Click2;
  SpeedButton26->Flat=true;
  Mod_imp=true; // ok per salvataggio

}
//---------------------------------------------------------------------------

void __fastcall TMain::SpeedButton26Click2(TObject *Sender)
{
 Oil_Temp->Hide();
 Vis_Oil=0;
 SpeedButton26->OnClick = SpeedButton26Click;
  SpeedButton26->Flat=false;
   Mod_imp=true; // ok per salvataggio
}




void __fastcall TMain::Timer_freniTimer(TObject *Sender)
{

   Image3->Visible=false;

switch(Timer_freni->Tag){
            case 0:
             Image1->Visible=false;
             Image2->Visible=true;
             Timer_freni->Tag++;
             break;
            case 1:
             Image1->Visible=true;
             Image2->Visible=false;
             Timer_freni->Tag=0;
            break;
            }
}
//---------------------------------------------------------------------------

void __fastcall TMain::Panel4Resize(TObject *Sender)
{

  GiriLabel->Font->Size=int(Panel4->Width/ K_WW);
  if(GiriLabel->Font->Size > K_Font)
    GiriLabel->Font->Size=K_Font;

}
//---------------------------------------------------------------------------









void __fastcall TMain::Log_Error(int Errore , String Action, unsigned char Difetto)
{


  AnsiString NewFileName = utils.GetProgramDataName(".log");//ExtractFilePath(Application->ExeName)+ "Log_error.txt";



  TDateTime DateTime = Time();
  String S=DateToStr(Date());
  AnsiString str = TimeToStr(DateTime);

  char Buffer[100];


 ifstream infile(NewFileName.c_str());  // controlla se presente il file log

 if(!infile)
 {
     // alla prima creazione del file log

      ofstream out(NewFileName.c_str(),ios_base::out);
      if(!out) // se non è possibile aprire il file messaggio d'errore
      ShowMessage("Impossibile aprire il file di Log.\n");

   out << "************************************************************** " << endl;
   out << "*  Programma generante: DynoRace                               " << endl;
   out << "*  File: Log function error                                    " << endl;
   out << "*  Contiene lo storico degli eventi d'errore verificatisi      " << endl;
   out << "*  durante il funzionamento del programma DynoRace             " << endl;
   out << "*  Inviare al servizio assistenza questo file nel caso in cui  " << endl;
   out << "*  si verifichino problematiche d'uso.                         " << endl;
   sprintf(Buffer,"%s",S);
   out << "*  Data dei primo avvio: " << Buffer << endl;
   sprintf(Buffer,"%s",str);
   out << "*  Ora : " << Buffer << endl;
   out << "************************************************************** " << endl;
   out.close();

 }



     //  ********  Aggiornamento del file Log Error... ******************


   else  // se file esiste già allora aggiunge nuovo evento
   {


     // apre il file in modalità append
      ofstream out(NewFileName.c_str(),ios_base::app);

      if(!out){ // se non è possibile aprire il file messaggio d'errore
      ShowMessage("Not possible to open the file Log error.\n");
      Beep();
      }

   // raccolta informazioni da loggare...

   sprintf(Buffer,"%s",S); //data
   out << "_______________________________________________________________" << endl;
   out << "Data dell'evento: " << Buffer << endl;

   sprintf(Buffer,"%s",str); //ora

   out << "Ora dell'evento: " << Buffer << endl;

   if(Errore)
   out << "Errore: " << Errori[Errore] << endl;


   if(Action!=NULL)
   {
   sprintf(Buffer,"%s",Action);
   out << "Messaggio: " << Buffer << endl;
   }

   if(Difetto)
   {
   sprintf(Buffer,"%X",Difetto);
   out << "Difetto codice: " << Buffer << endl;
   out << "Tipo di banco: " << tipoBanco << endl;
   }


   // condizioni di contorno....






   out << "_______________________________________________________________" << endl;

   out.close();



   // ----

 } // di else



}

void __fastcall TMain::Salvataggio_autom(void)
{
  dataFile=Now();
  
  Salvamisurazione1Click(NULL);
  
}


void __fastcall TMain::NxButton3Click(TObject *Sender)
{
  Impostazionigrafici1Click(this);
}
//---------------------------------------------------------------------------


void __fastcall TMain::Visualizza_dat_test_strumenti(int i,int POS)
{
	// Parametri: n° test , Posizione array (iPos)
  ChartTool9->Pen->Color=clTestColor[i+1];
  ChartTool9->Visible=true;
  if(!(POS>=0 && POS<500 && i>=0 && i<8 && vGiri[i][POS]>0))
    return;

  if(Unit_Tem==1)
  {
    TatmLabel->Caption=FormatFloat("0.#",ConvertTo(datPrm[i].tempAtm,tuFahrenheit));
    TAirLabel->Caption=FormatFloat("0.#",ConvertTo(datPrm[i].tempAtm,tuFahrenheit));
  }
  else
  {
    TatmLabel->Caption=FormatFloat("0.#",datPrm[i].tempAtm);
    TAirLabel->Caption=FormatFloat("0.#",datPrm[i].tempAtm);
  }      
  // pressione atmosferica  
  PatmLabel->Caption=FormatFloat("0.#",datPrm[i].pressAtm);
  //  temperatura olio
  if(Vis_T_N)
  {
    Oil_Temp->AnalogMeterHQ1->Value=vTermoOil[i][POS];
    if(Unit_Tem ==1)
    {     
      Oil_Temp->AnalogMeterHQ1->Caption=FormatFloat("0.#",ConvertTo(vTermoOil[i][POS],tuFahrenheit));      
    }
    else
    {      
      Oil_Temp->AnalogMeterHQ1->Caption=FormatFloat("0.#",vTermoOil[i][POS]);      
    }          
    OilLabel->Caption=Oil_Temp->AnalogMeterHQ1->Caption;
  }
  else
  {
    Oil_Temp->AnalogMeterHQ1->Value=0;
    Oil_Temp->AnalogMeterHQ1->Caption="0";
    OilLabel->Caption=0;
  }
  // pressione manifold
  if(Vis_Pmap)
  {
    Map->AnalogMeterHQ1->Value=vPresMAP[i][POS];    
    Map->AnalogMeterHQ1->Caption=FormatFloat("0.#",vPresMAP[i][POS]*fK_convPr);
    MapLabel->Caption=Map->AnalogMeterHQ1->Caption;
  }
  else
  {
    Map->AnalogMeterHQ1->Value=0;
    Map->AnalogMeterHQ1->Caption="0";
    MapLabel->Caption=0;
  }
  // termocoppia
  if(Vis_T_K)
  {
    Termo->AnalogMeterHQ1->Value=vTermoK[i][POS];
    if(Unit_Tem ==1)
      Termo->AnalogMeterHQ1->Caption=FormatFloat("0.#",ConvertTo(vTermoK[i][POS],tuFahrenheit));      
    else
      Termo->AnalogMeterHQ1->Caption=FormatFloat("0.#",vTermoK[i][POS]);              
    TKLabel->Caption=Termo->AnalogMeterHQ1->Caption;
  }
  else
  {
    Termo->AnalogMeterHQ1->Value=0;
    Termo->AnalogMeterHQ1->Caption="0";
    TKLabel->Caption=0;
  }                             
  //RP=datPrm[i].rappAnt;			
//  ChartTool9->Pen->Color=clBlue;
	ChartTool9->Value=vGiri[i][POS];
	// giri motore
	Giri_Mot-> AnalogMeterHQ1->Value=(int)vGiri[i][POS];
	GiriLabel->Caption=String((int)vGiri[i][POS]);
	Giri_Mot->AnalogMeterHQ1->Caption=String((int)vGiri[i][POS]);
	// coppia motore corretta
	Coppia->AnalogMeterHQ1->Value=datPrm[i].fFact*vCoppiaRuota[i][POS]*fK_convC;
	Coppia->AnalogMeterHQ1->Caption=FormatFloat("0.##",datPrm[i].fFact*vCoppiaRuota[i][POS]*fK_convC);
	CopLabel->Caption=Coppia->AnalogMeterHQ1->Caption;
	// potenza motore corretta
	Potenza->AnalogMeterHQ1->Value=datPrm[i].fFact*vPotenzaRuota[i][POS]*fK_convP;//* DAT_1[8];
	Potenza->AnalogMeterHQ1->Caption=FormatFloat("0.##",datPrm[i].fFact*vPotenzaRuota[i][POS]*fK_convP);
	PotLabel->Caption=Potenza->AnalogMeterHQ1->Caption;
	// consumi
	if(Strumenti->CheckBox1->Checked)
	{
      ConsLabel->Caption=FormatFloat("0",CONSUMO_G_KWH(vCons[i][POS],vPotenzaRuota[i][POS]));
      ConsLabel2->Caption=FormatFloat("0.##",CONSUMO(vCons[i][POS],vConsTot[i][POS]));
	}
  PageControl2->ActivePage=TabSheet2;
}

void __fastcall TMain::Anteprimadati1Click(TObject *Sender)
{
  sTest_antep=0;
  if(datPrm[sTest_antep].posMaxP<500)
    iPos=datPrm[sTest_antep].posMaxP;
  else
    iPos=0 ;
  PageControl1->ActivePage=TabSheet1;
  Chart1->SetFocus();
  Visualizza_dat_test_strumenti(sTest_antep,iPos);
  Out_dat=false; // disattiva real-time
  TabSheet2->Caption=CheckBox1->Caption;
  TabSheet2->Highlighted=true;
}
//---------------------------------------------------------------------------

void __fastcall TMain::Anteprimadati2Click(TObject *Sender)
{
  sTest_antep=1;
  if(datPrm[sTest_antep].posMaxP<500)
    iPos=datPrm[sTest_antep].posMaxP;
  else iPos=0;
  PageControl1->ActivePage=TabSheet1;
  Chart1->SetFocus();
  Visualizza_dat_test_strumenti(sTest_antep,iPos);
  Out_dat=false; // disattiva real-time
  TabSheet2->Caption=CheckBox2->Caption;
  TabSheet2->Highlighted=true;
}
//---------------------------------------------------------------------------



void __fastcall TMain::AnteprimaDati3Click(TObject *Sender)
{
 sTest_antep=2;
 if(datPrm[sTest_antep].posMaxP<500)
   iPos=datPrm[sTest_antep].posMaxP;
 else
   iPos=0;
  PageControl1->ActivePage=TabSheet1;
 Chart1->SetFocus();
 Visualizza_dat_test_strumenti(sTest_antep,iPos);
  Out_dat=false; // disattiva real-time
   TabSheet2->Caption=CheckBox3->Caption;
 TabSheet2->Highlighted=true;
}
//---------------------------------------------------------------------------

void __fastcall TMain::Anteprimadati4Click(TObject *Sender)
{
 sTest_antep=3;
 if(datPrm[sTest_antep].posMaxP<500)
   iPos=datPrm[sTest_antep].posMaxP;
 else
   iPos=0;
  PageControl1->ActivePage=TabSheet1;   
 Chart1->SetFocus();
 Visualizza_dat_test_strumenti(sTest_antep,iPos);
  Out_dat=false; // disattiva real-time
   TabSheet2->Caption=CheckBox4->Caption;
 TabSheet2->Highlighted=true;
}
//---------------------------------------------------------------------------

void __fastcall TMain::Anteprimadati5Click(TObject *Sender)
{
 sTest_antep=4;
 if(datPrm[sTest_antep].posMaxP<500)
   iPos=datPrm[sTest_antep].posMaxP;
 else
   iPos=0;
  PageControl1->ActivePage=TabSheet1;
 Chart1->SetFocus();
 Visualizza_dat_test_strumenti(sTest_antep,iPos);
  Out_dat=false; // disattiva real-time
   TabSheet2->Caption=CheckBox5->Caption;
 TabSheet2->Highlighted=true;
}
//---------------------------------------------------------------------------

void __fastcall TMain::Anteprimadati6Click(TObject *Sender)
{
 sTest_antep=5;
 if(datPrm[sTest_antep].posMaxP<500)
   iPos=datPrm[sTest_antep].posMaxP;
 else
   iPos=0;
  PageControl1->ActivePage=TabSheet1;
 Chart1->SetFocus();
 Visualizza_dat_test_strumenti(sTest_antep,iPos);
  Out_dat=false; // disattiva real-time
   TabSheet2->Caption=CheckBox6->Caption;
 TabSheet2->Highlighted=true;
}
//---------------------------------------------------------------------------

void __fastcall TMain::Anteprimadati7Click(TObject *Sender)
{
 sTest_antep=6;
 if(datPrm[sTest_antep].posMaxP<500)
   iPos=datPrm[sTest_antep].posMaxP;
 else
   iPos=0;
  PageControl1->ActivePage=TabSheet1;
 Chart1->SetFocus();
 Visualizza_dat_test_strumenti(sTest_antep,iPos);
  Out_dat=false; // disattiva real-time
   TabSheet2->Caption=CheckBox7->Caption;
 TabSheet2->Highlighted=true;
}
//---------------------------------------------------------------------------

void __fastcall TMain::Anteprimadati8Click(TObject *Sender)
{
 sTest_antep=7;
 if(datPrm[sTest_antep].posMaxP<500)
   iPos=datPrm[sTest_antep].posMaxP;
 else
   iPos=0;
  PageControl1->ActivePage=TabSheet1;
 Chart1->SetFocus();
 Visualizza_dat_test_strumenti(sTest_antep,iPos);
  Out_dat=false; // disattiva real-time
   TabSheet2->Caption=CheckBox8->Caption;
 TabSheet2->Highlighted=true;
}
//---------------------------------------------------------------------------


















void __fastcall TMain::Button1Click(TObject *Sender)
{
        startDebug = 1;
        stopDebug = 0;
}
//---------------------------------------------------------------------------

void __fastcall TMain::Button2Click(TObject *Sender)
{
        Series56->Clear();
        Series57->Clear();
        Series59->Clear();
        Series19->Clear();
        Series20->Clear();
        Series4->Clear();
        Cont_Acq = 0;
}
//---------------------------------------------------------------------------

void __fastcall TMain::Button3Click(TObject *Sender)
{
  stopDebug = 1;
  startDebug = 0;
}
//---------------------------------------------------------------------------

void __fastcall TMain::TimerStopPwmTimer(TObject *Sender)
{
  static char sLog[100];
  #define N_STEP_PWM_TO_0 5
  /* Il timer viene abilitato ad ogni invio di stop prova, per tutte le prove
     tranne per i comandi di freno manuale:
     Invia una sequenza di comandi di freno manuale togliendo il 20% ad ogni step
     finchè raggiungo il 2% del freno, in tal caso invia pwm=0 e disabilita il timer
  */
  if(((float)usPwmWrite-(float)TimerStopPwm->Tag/N_STEP_PWM_TO_0) > (float)CGauge2->MaxValue*0.02)
    usPwmWrite-=(unsigned short)(int)((float)TimerStopPwm->Tag/N_STEP_PWM_TO_0);
  else
  {
    TimerStopPwm->Tag=0;
    TimerStopPwm->Enabled=false;
    usPwmWrite=0;
  }
  /// Invia settaggio manuale al freno
  g_brakeControl.CmdBrakeMan(usPwmWrite,CDyn3::EFrontAxle);
  if(bFileTrace)
  {
    sprintf(sLog,"Cmd Brake Manual PWM=%d",usPwmWrite);
    LogError(g_cDyn3Log,LOG_INFO,sLog);
  }
}
//---------------------------------------------------------------------------
void __fastcall TMain::ResetConsumi(void)
{

}
//---------------------------------------------------------------------------


void TMain::controllaAndamentoRPMdec(void)
{
  int i;
        //controllo l'andamento dei giri
        //rimuovo le risalite
        //RPM_IST[] vettore rpm
        //C_IST[] vettore coppia
        //cntrIst indice acquisizione (-1 ultima scritta)
        //vettori temporanei
        float tmpRPM[7500];
        float tmpC[7500];
        int tmpInd=0;
        float lastRPMval=0;
        lastRPMval=RPM_IST[0];
        tmpRPM[0]=RPM_IST[0];
        tmpC[0]=C_IST[0];
        tmpInd=1;
        for ( i=1;i<cntrIst;i++)
        {
                if (RPM_IST[i]<=lastRPMval)
                {
                        //passo corretto
                        //copio i valori nel nuovo vettore
                        tmpRPM[tmpInd]=RPM_IST[i];
                        tmpC[tmpInd]=C_IST[i];
                        //aggiorno ultimo valore valido
                        lastRPMval=RPM_IST[i];
                        //aggiorno l'indice
                        tmpInd++;
                }
                else
                {
                        //non corretto
                        //giri in risalita
                }
        }

        //ricopio i vettori temporanei negli originali
        //pulisco i vettori prece
        memset(RPM_IST,0,sizeof(RPM_IST));
        memset(C_IST,0,sizeof(C_IST));
        cntrIst=tmpInd;
        for( i=0;i<tmpInd;i++)
        {
           RPM_IST[i]=tmpRPM[i];
           C_IST[i]=tmpC[i];
        }

}

//---------------------------------------------------------------------------

void TMain::aggiornaINI(void)
{
        //aggiorno il file INI
        //rimuovo tutti gli accenti


        ifstream FOpenIN;
        ofstream FOpenOUT;
        char c;
        try
        {
                FOpenIN.open(utils.GetProgramDataName(".ini").c_str());
                FOpenOUT.open(utils.GetProgramDataName(".tmp").c_str());
                while(FOpenIN.good())
                {
                        FOpenIN.get(c);
                        if (c=='à')
                        {
                                c='a';
                        }
                        if (c=='è' || c=='è')
                        {
                                c='e';
                        }
                        if (c=='ì')
                        {
                                c='i';
                        }
                        if (c=='ò')
                        {
                                c='o';
                        }
                        if (c=='ù')
                        {
                                c='u';
                        }
                        FOpenOUT<<c;
                }
        }
        __finally
        {
                FOpenOUT.close();
                FOpenIN.close();

        }
        //devo eliminare il file INI originale e sostituirlo con quello nuovo
        try
        {
                DeleteFile(utils.GetProgramDataName(".ini").c_str());
                MoveFile(utils.GetProgramDataName(".tmp").c_str(), utils.GetProgramDataName(".ini").c_str());

        }
        catch(Exception &e)
        {
        }
        //apro il file INI e aggiungo la versione
        TIniFile *ini;
        try
        {
                ini = new TIniFile(utils.GetProgramDataName(".ini") );
                ini->WriteInteger( "VerINI", "Versione",2);
        }
        __finally
        {
                delete ini;
        }
        
}

void  TMain::ResetTabAcq()
{
  static TNextGrid* tbl;
  //unsigned int *rowTabStep;
  int nTbl;
  switch(Tipo_test)
  {
   case TEST_STEP_TIMED:
        tbl=NextGrid9;
        TabSheet11->TabVisible=true;
        nTbl=TAB_STEP_TIMED;
        //rowTabStep=&rowTabStepF1;
   break;
   case TEST_STEP:
        tbl=NextGrid11;
        TabSheet7->TabVisible=true;
        nTbl=TAB_STEP;
        //rowTabStep=&rowTabStepF2;
   break;
   case TEST_BRAKE_MAN:
        tbl=NextGrid12;
        TabSheet8->TabVisible=true;
        nTbl=TAB_BRAKE;
        //rowTabStep=&rowTabStepF4;
   break;
   default:
   break;
 }
   memset(tabStepGiri[nTbl],0,sizeof(tabStepGiri[nTbl]));
   memset(tabStepCoppia[nTbl],0,sizeof(tabStepCoppia[nTbl]));
   memset(tabStepPot[nTbl],0,sizeof(tabStepPot[nTbl]));
   memset(tabStepCons[nTbl],0,sizeof(tabStepCons[nTbl]));
   memset(tabStepLamb[nTbl],0,sizeof(tabStepLamb[nTbl]));
   memset(tabStepMap[nTbl],0,sizeof(tabStepMap[nTbl]));
   memset(tabStepTk[nTbl],0,sizeof(tabStepTk[nTbl]));

   tbl->ClearRows();

   //*rowTabStep=1;
   //primaVoltaTabStep = 1;
   TabStepPrint->Enabled = false;

}

void  TMain::AddTabAcq()
{
  static AnsiString strCol0;
  static TNextGrid* tbl;
  static int nTbl=0;
  //unsigned int *rowTabStep;
  switch(Tipo_test)
  {
   case TEST_STEP_TIMED:
        tbl=NextGrid9;
        strCol0=Step_test->getStep();
         nTbl=TAB_STEP_TIMED;
       // rowTabStep=&rowTabStepF1;
   break;
   case TEST_STEP:
        tbl=NextGrid11;
        strCol0=Step_test->getStep();
         nTbl=TAB_STEP;
        //rowTabStep=&rowTabStepF2;
   break;
   case TEST_BRAKE_MAN:
        tbl=NextGrid12;
        strCol0=FormatFloat("0.#%",Pilot_Freno->AnalogMeterHQ1->Value);
        nTbl=TAB_BRAKE;
        //rowTabStep=&rowTabStepF4;
   break;
   default:
   break;
 }
  if(tbl->RowCount==0)
  {
    tbl->InsertRow(0);
    switch(Tipo_test)
    {
      case TEST_STEP_TIMED:
        tbl -> Cells[0][0] = "STEP TIMED";
        TabSheet11->TabVisible=true;
        PageControl1->ActivePage = (TabSheet11);
      break;
      case TEST_STEP:
        tbl -> Cells[0][0] = "STEP";
        TabSheet7->TabVisible=true;
        PageControl1->ActivePage = (TabSheet7);
      break;
      case TEST_BRAKE_MAN:
        tbl -> Cells[0][0] = "BRAKE %";
        TabSheet8->TabVisible=true;
        PageControl1->ActivePage = (TabSheet8);
      break;
      default:
      break;
    }
    tbl -> Cells[1][0] = Panel5->Caption;    //nome colonna
    sprintf(tabStepNGiri,"%s",Panel5->Caption);
    tbl -> Cells[2][0] = Panel9->Caption;    //nome colonna
    sprintf(tabStepNCoppia,"%s",Panel9->Caption);
    tbl -> Cells[3][0] = Panel11->Caption;   //nome colonna
    sprintf(tabStepNPot,"%s",Panel11->Caption);
    tbl -> Cells[4][0] = "g/kWh";   //nome colonna
    sprintf(tabStepNCons,"%s",Panel7->Caption);
    tbl -> Cells[5][0] = Panel15->Caption;   //nome colonna
    sprintf(tabStepNMap,"%s",Panel15->Caption);
    tbl -> Cells[6][0] = Panel17->Caption;   //nome colonna
    sprintf(tabStepNTk,"%s",Panel17->Caption);
    //primaVoltaTabStep = 0;


  }


  tbl->InsertRow(tbl->RowCount/*(*rowTabStep)*/-(COLONNE_TABSTEP_VUOTE-1));
  //WideString st="step"+Step_test->getStep();
  tbl -> Cells[0][tbl->RowCount-1-(COLONNE_TABSTEP_VUOTE-1)] = strCol0;//st;
  tbl -> Cells[1][tbl->RowCount-1-(COLONNE_TABSTEP_VUOTE-1)] = GiriLabel->Caption;       //regime
  tabStepGiri[nTbl][tbl->RowCount-1-(COLONNE_TABSTEP_VUOTE-1)] = GiriLabel->Caption.ToInt();
  tbl -> Cells[2][tbl->RowCount-1-(COLONNE_TABSTEP_VUOTE-1)] = CopLabel->Caption;
  tabStepCoppia[nTbl][tbl->RowCount-1-(COLONNE_TABSTEP_VUOTE-1)] =CopLabel->Caption.ToDouble();//strtod(.c_str(),&endptr);// CopLabel->Caption.ToDouble();
  tbl -> Cells[3][tbl->RowCount-1-(COLONNE_TABSTEP_VUOTE-1)] = PotLabel->Caption;
  tabStepPot[nTbl][tbl->RowCount-1-(COLONNE_TABSTEP_VUOTE-1)] = PotLabel->Caption.ToDouble();//strtod(,&endptr);
  tbl -> Cells[4][tbl->RowCount-1-(COLONNE_TABSTEP_VUOTE-1)] = ConsLabel->Caption;
  tabStepCons[nTbl][tbl->RowCount-1-(COLONNE_TABSTEP_VUOTE-1)] = CONSUMO_G_KWH(litri_h,PotLabel->Caption.ToDouble());//strtod(ConsLabel->Caption,&endptr);
  tbl -> Cells[5][tbl->RowCount-1-(COLONNE_TABSTEP_VUOTE-1)] = MapLabel->Caption;
  try
  {
      tabStepMap[nTbl][tbl->RowCount-1-(COLONNE_TABSTEP_VUOTE-1)] = MapLabel->Caption.ToDouble();
  }
  catch(Exception &e)
  {
  }

  tbl -> Cells[6][tbl->RowCount-1-(COLONNE_TABSTEP_VUOTE-1)] = TKLabel->Caption;
  tabStepTk[nTbl][tbl->RowCount-1-(COLONNE_TABSTEP_VUOTE-1)] = TKLabel->Caption.ToDouble();//wcstod(TKLabel->Caption,&endptr);
 //(*rowTabStep)++;
}

void  TMain::LoadTabAcq()
{

  static AnsiString strCol0;
  static TNextGrid* tbl;
  static int nTbl=0;
  //unsigned int *rowTabStep;
  switch(Tipo_test)
  {
   case TEST_STEP_TIMED:
        tbl=NextGrid9;
        strCol0=Step_test->getStep();
         nTbl=TAB_STEP_TIMED;
       // rowTabStep=&rowTabStepF1;
   break;
   case TEST_STEP:
        tbl=NextGrid11;
        strCol0=Step_test->getStep();
         nTbl=TAB_STEP;
        //rowTabStep=&rowTabStepF2;
   break;
   case TEST_BRAKE_MAN:
        tbl=NextGrid12;
        strCol0=FormatFloat("0.#%",Pilot_Freno->AnalogMeterHQ1->Value);
        nTbl=TAB_BRAKE;
        //rowTabStep=&rowTabStepF4;
   break;
   default:
   break;
 }
  if(tbl->RowCount==0)
  {
    tbl->InsertRow(0);
    switch(Tipo_test)
    {
      case TEST_STEP_TIMED:
        tbl -> Cells[0][0] = "STEP TIMED";
        TabSheet11->TabVisible=true;
        PageControl1->ActivePage = (TabSheet11);
      break;
      case TEST_STEP:
        tbl -> Cells[0][0] = "STEP";
        TabSheet7->TabVisible=true;
        PageControl1->ActivePage = (TabSheet7);
      break;
      case TEST_BRAKE_MAN:
        tbl -> Cells[0][0] = "BRAKE %";
        TabSheet8->TabVisible=true;
        PageControl1->ActivePage = (TabSheet8);
      break;
      default:
      break;
    }
    tbl -> Cells[1][0] = Panel5->Caption;    //nome colonna
    sprintf(tabStepNGiri,"%s",Panel5->Caption);
    tbl -> Cells[2][0] = Panel9->Caption;    //nome colonna
    sprintf(tabStepNCoppia,"%s",Panel9->Caption);
    tbl -> Cells[3][0] = Panel11->Caption;   //nome colonna
    sprintf(tabStepNPot,"%s",Panel11->Caption);
    tbl -> Cells[4][0] = "g/kWh";   //nome colonna
    sprintf(tabStepNCons,"%s",Panel7->Caption);
    tbl -> Cells[5][0] = Panel15->Caption;   //nome colonna
    sprintf(tabStepNMap,"%s",Panel15->Caption);
    tbl -> Cells[6][0] = Panel17->Caption;   //nome colonna
    sprintf(tabStepNTk,"%s",Panel17->Caption);
    //primaVoltaTabStep = 0;


  }


  tbl->InsertRow(tbl->RowCount/*(*rowTabStep)*/-(COLONNE_TABSTEP_VUOTE-1));
  //WideString st="step"+Step_test->getStep();
  tbl -> Cells[0][tbl->RowCount-1-(COLONNE_TABSTEP_VUOTE-1)] = strCol0;//st;
  tbl -> Cells[1][tbl->RowCount-1-(COLONNE_TABSTEP_VUOTE-1)] = GiriLabel->Caption;       //regime
  tabStepGiri[nTbl][tbl->RowCount-1-(COLONNE_TABSTEP_VUOTE-1)] = GiriLabel->Caption.ToInt();
  tbl -> Cells[2][tbl->RowCount-1-(COLONNE_TABSTEP_VUOTE-1)] = CopLabel->Caption;
  tabStepCoppia[nTbl][tbl->RowCount-1-(COLONNE_TABSTEP_VUOTE-1)] =CopLabel->Caption.ToDouble();//strtod(.c_str(),&endptr);// CopLabel->Caption.ToDouble();
  tbl -> Cells[3][tbl->RowCount-1-(COLONNE_TABSTEP_VUOTE-1)] = PotLabel->Caption;
  tabStepPot[nTbl][tbl->RowCount-1-(COLONNE_TABSTEP_VUOTE-1)] = PotLabel->Caption.ToDouble();//strtod(,&endptr);
  tbl -> Cells[4][tbl->RowCount-1-(COLONNE_TABSTEP_VUOTE-1)] = ConsLabel->Caption;
  tabStepCons[nTbl][tbl->RowCount-1-(COLONNE_TABSTEP_VUOTE-1)] = CONSUMO_G_KWH(litri_h,PotLabel->Caption.ToDouble());//strtod(ConsLabel->Caption,&endptr);
  tbl -> Cells[5][tbl->RowCount-1-(COLONNE_TABSTEP_VUOTE-1)] = MapLabel->Caption;
  try
  {
      tabStepMap[nTbl][tbl->RowCount-1-(COLONNE_TABSTEP_VUOTE-1)] = MapLabel->Caption.ToDouble();
  }
  catch(Exception &e)
  {
  }

  tbl -> Cells[6][tbl->RowCount-1-(COLONNE_TABSTEP_VUOTE-1)] = TKLabel->Caption;
  tabStepTk[nTbl][tbl->RowCount-1-(COLONNE_TABSTEP_VUOTE-1)] = TKLabel->Caption.ToDouble();//wcstod(TKLabel->Caption,&endptr);
 //(*rowTabStep)++;
}

void __fastcall TMain::TabStepPrintClick(TObject *Sender)
{
     Graphics::TBitmap *Bitmap1 = new Graphics::TBitmap();
    int h,w,x,y,sp, i, j;
    String JKK,JJK,LLL,BBB,CCC,D,AS,SA,spc1=" ";
    static TNextGrid* tbl;
    if(PageControl1->ActivePage==TabSheet11)
      tbl=NextGrid9;
    else if(PageControl1->ActivePage==TabSheet7)
      tbl=NextGrid11;
    else  if(PageControl1->ActivePage==TabSheet8)
      tbl=NextGrid12;
    else
      return;
    JKK=Cliente;
    JJK=Targa;
    char buffer[200];
    LLL=Costruttore;
    BBB=Modello;
     CCC=Allestimento;
    //Timer_con->Enabled=false; // disattivo controllo connessione
    if(PrintDialog1->Execute())
    {


       Printer()->BeginDoc();

        h = Printer()->PageHeight; // <-- get page height
        w = Printer()->PageWidth;  // <-- get page width
        //crt = w/120; //larghezza di un carattere
        x=w/100;
        y=h/100;
        sp=h/100;

        Printer()->Canvas->Font->Name = "Thaoma";
        Printer()->Canvas->Font->Size = 8;             // <-- set the font size
        Printer()->Canvas->Font->Style.Clear();
        //stampa prima riga


        Printer()->Canvas->TextOut(x,y,"Sw rev. " + (String)swNbr);   // <-- print some text
        y+=2*sp;
        Printer()->Canvas->MoveTo(20,y);
        Printer()->Canvas->LineTo(w-20, y);
        y+=sp;
        Printer()->Canvas->TextOut(x,y,Text47 + Ragione);   // <-- print Workshop/Tuner

        if(FileExists(Percorso))
          Bitmap1->LoadFromFile(Percorso);
        else
          Bitmap1->LoadFromResourceName((int)HInstance,"Logo1");
        float aspectratio=(float)Bitmap1->Width/(float)Bitmap1->Height;
        TRect RectImage(w*98/100-3.5/100*h*aspectratio,y,w*98/100,y+3.5/100*h/*h*4/100*/);//Bitmap1->Canvas->ClipRect;
        // stampa il logo a partire dal 75% della max larghezza
        Printer()->Canvas->StretchDraw(RectImage,Bitmap1);
        y+=sp;
        Printer()->Canvas->TextOut(x,y,Text48 + " " + Indirizzo + " Internet: "+ Internet);   // <-- print some text
        y+=sp;
        Printer()->Canvas->TextOut(x,y,Text49 +" "+ Telefono + " Fax: "+ Fax + " E-mail: " + EMail);   // <-- print some text
        y+=sp*2;
        Printer()->Canvas->MoveTo(20,y);
        Printer()->Canvas->LineTo(w-20, y);
        y+=sp;
        Printer()->Canvas->TextOut(x,y,Text50 + spc1 + JKK);  // cliente
        Printer()->Canvas->TextOut(w-(w/1.7),y,Text51 + spc1 + JJK); //veicle
        Printer()->Canvas->TextOut(w-(w/3),y,Text52 +spc1+ LLL); //manufacture
        y+=sp*2;
        Printer()->Canvas->TextOut(x,y,Text53 + spc1 + BBB);
        Printer()->Canvas->TextOut(w-(w/1.7),y,Text54 + spc1 + CCC);
        y+=sp*3;
        Printer()->Canvas->Font->Name = "Times New Roman";
        Printer()->Canvas->Font->Size = 10;             // <-- set the font size
        Printer()->Canvas->Font->Style.Clear();


        sprintf (buffer, "%18s%24s%24s%24s%24s%26s%24s",
                  ((String)tbl->Cells[0][0]).c_str(),
                  ((String)tbl->Cells[1][0]).c_str(),
                  ((String)tbl->Cells[2][0]).c_str(),
                  ((String)tbl->Cells[3][0]).c_str(),
                  ((String)tbl->Cells[4][0]).c_str(),
                  ((String)tbl->Cells[5][0]).c_str(),
                  ((String)tbl->Cells[6][0]).c_str());
          Printer()->Canvas->TextOut(x,y, buffer);
          y+=sp*2;
        Printer()->Canvas->MoveTo(20,y);
        Printer()->Canvas->LineTo(w-20, y);
        Printer()->Canvas->Font->Size=12;             // <-- set the font size
        float f;
        y+=sp;
        for (j=1;j<tbl->RowCount ;j++)
        {
          sprintf (buffer,"%18s%24s%24s%24s%24s%26s%24s",
                  ((String)tbl->Cells[0][j]).c_str(),
                  ((String)tbl->Cells[1][j]).c_str(),
                  ((String)tbl->Cells[2][j]).c_str(),
                  ((String)tbl->Cells[3][j]).c_str(),
                  ((String)tbl->Cells[4][j]).c_str(),
                  ((String)tbl->Cells[5][j]).c_str(),
                  ((String)tbl->Cells[6][j]).c_str());
          Printer()->Canvas->TextOut(x,y, buffer);
          y+=sp*2;
        }


        Printer()->EndDoc();
        Printer()->Orientation=poPortrait;
    }

  delete Bitmap1;
}
//---------------------------------------------------------------------------

void __fastcall TMain::exportTablesClick(TObject *Sender)
{
  String S;
  //Timer_con->Enabled=false; // disattivo controllo connessione
  SaveDialog1->Filter = "Tabella testo (*.txt)|*.txt;";
  SaveDialog1->DefaultExt="txt";
  if(SaveDialog1->Execute()) // Salvataggio del file sessione
  {
    S=SaveDialog1->FileName;
    if(PageControl1->ActivePage==TabSheet11)
      NextGrid9->SaveToTextFile(S);
    else if(PageControl1->ActivePage==TabSheet7)
      NextGrid11->SaveToTextFile(S);
    else if(PageControl1->ActivePage==TabSheet8)
      NextGrid12->SaveToTextFile(S);
    else if(sNum_test>0)
    switch(PageControl3->ActivePageIndex)
    {
      case 0:
      NextGrid1->SaveToTextFile(S);
      break;
      case 1:
      NextGrid2->SaveToTextFile(S);
      break;
      case 2:
      NextGrid3->SaveToTextFile(S);
      break;
      case 3:
        NextGrid4->SaveToTextFile(S);
      break;
      case 4:
       NextGrid5->SaveToTextFile(S);
    break;
    case 5:
      NextGrid6->SaveToTextFile(S);
    break;
    case 6:
      NextGrid7->SaveToTextFile(S);
    break;
    case 7:
      NextGrid8->SaveToTextFile(S);
    break;
    default:
      
    break;
  }


//    NextGrid9->SaveToTextFile(S);
  }
  SaveDialog1->Filter = "Misurazione (*.dyn)|*.dyn;";
  SaveDialog1->DefaultExt="dyn";

}
//---------------------------------------------------------------------------

void __fastcall TMain::MapLabelDblClick(TObject *Sender)
{
  if (Panel6->Visible && Panel14->Visible)
  {
    Panel14->Width=Panel45->Width/2;
    Panel6->Width=Panel45->Width/2;
  }
}
//---------------------------------------------------------------------------


void __fastcall TMain::ConsLabelDblClick(TObject *Sender)
{
  MapLabelDblClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TMain::OilLabelDblClick(TObject *Sender)
{
  int nPanelVisible;
  nPanelVisible=Panel18->Visible+Panel12->Visible+Panel20->Visible;

  if (nPanelVisible==3)
  {
    Panel18->Width=Panel46->Width/3;
    Panel20->Width=Panel46->Width/3;
    Panel12->Width=Panel46->Width/3;
  }
  else if(nPanelVisible==2)
  {
    Panel18->Width=Panel46->Width/2;
    Panel20->Width=Panel46->Width/2;
    Panel12->Width=Panel46->Width/2;
  }
  else
  {
    Panel18->Width=Panel46->Width;
    Panel20->Width=Panel46->Width;
    Panel12->Width=Panel46->Width;
  }
}
//---------------------------------------------------------------------------

void __fastcall TMain::TKLabelDblClick(TObject *Sender)
{
  OilLabelDblClick(this);
}
//---------------------------------------------------------------------------




void __fastcall TMain::Panel6Resize(TObject *Sender)
{
  ConsLabel->Font->Size=int(Panel6->Width/(K_WW*2));
  ConsLabel2->Font->Size=int(Panel6->Width/(K_WW*2));
  if(Unit_Cons==2)
    ConsLabel->Width=Panel6->Width;
  else
  {
    ConsLabel2->Width=Panel6->Width/2;
    ConsLabel->Width=Panel6->Width/2;
  }
  if(ConsLabel->Font->Size > K_Font)
  {
    ConsLabel->Font->Size=K_Font;
    ConsLabel2->Font->Size=K_Font;
  }
}
//---------------------------------------------------------------------------



void __fastcall TMain::PageControl2Resize(TObject *Sender)
{
  Panel4->Height =Panel3->Height/5-12;
  Panel8->Height =Panel3->Height/5-12;
  Panel10->Height=Panel3->Height/5-12;
  Panel43->Height=Panel3->Height/5-12;
  Panel34->Height=Panel3->Height/5-12;
  Panel44->Height=Panel3->Height/5-12;
  Panel45->Height=Panel3->Height/5-12;
  Panel46->Height=Panel3->Height/5-12;
  MapLabelDblClick(this);
  OilLabelDblClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TMain::Panel45Resize(TObject *Sender)
{
  Panel6Resize(this);
}
//---------------------------------------------------------------------------


void __fastcall TMain::Edit1Change(TObject *Sender)
{
  try
  {
    m_minV_T=StrToFloat(Edit1->Text);
  }
  catch (EConvertError &e)
  {
    Edit1->Text=FloatToStr(m_minV_T);
    MessageDlg("Conversione numerica non valida!",mtError	,TMsgDlgButtons() << mbCancel,0);
  }
}
//---------------------------------------------------------------------------

void __fastcall TMain::SpeedButton3Click(TObject *Sender)
{
  g_dyraControl.CmdStop();
  Sleep(100);
  statusRelaisConfig=statusRelaisConfig&0xEF;
  g_dyraControl.CmdRele(statusRelaisConfig);
  Sleep(100);
  statusRelaisConfig=statusRelaisConfig|0x10;
  g_dyraControl.CmdRele(statusRelaisConfig);
  Sleep(100);
  g_dyraControl.CmdStart();
  uiImpulsiCons=0;
  uiPrecImpulsiCons=0;
}
//---------------------------------------------------------------------------

void __fastcall TMain::Series15GetMarkText(TChartSeries *Sender,
      int ValueIndex, AnsiString &MarkText)
{
  // Inserire Label sul grafico
  static float fConsumoGKwh;

  fConsumoGKwh=CONSUMO_G_KWH(vCons[0][ValueIndex],vPotenzaRuota[0][ValueIndex]);
  switch(Unit_Cons)
  {
    case 0:
      sprintf(g_cBuffer,"%1.0fg/KWh - %1.2fl/h",fConsumoGKwh,vCons[0][ValueIndex]);
    break;
    case 1:
      sprintf(g_cBuffer,"%1.0fg/KWh - %1.2fl/min",fConsumoGKwh,vCons[0][ValueIndex]/60);
    break;
    case 2:
      sprintf(g_cBuffer,"%1.0fg/KWh",fConsumoGKwh);
    break;
    case 3: //litri
      sprintf(g_cBuffer,"%1.0fg/KWh - %1.2fl",fConsumoGKwh,vConsTot[0][ValueIndex]);
    break;
  }

  MarkText=g_cBuffer;
}
//---------------------------------------------------------------------------

void __fastcall TMain::Series16GetMarkText(TChartSeries *Sender,
      int ValueIndex, AnsiString &MarkText)
{
  static float fConsumoGKwh;

  fConsumoGKwh=CONSUMO_G_KWH(vCons[1][ValueIndex],vPotenzaRuota[1][ValueIndex]);
  switch(Unit_Cons)
  {
    case 0:
      sprintf(g_cBuffer,"%1.0fg/KWh - %1.2fl/h",fConsumoGKwh,vCons[1][ValueIndex]);
    break;
    case 1:
      sprintf(g_cBuffer,"%1.0fg/KWh - %1.2fl/min",fConsumoGKwh,vCons[1][ValueIndex]/60);
    break;
    case 2:
      sprintf(g_cBuffer,"%1.0fg/KWh",fConsumoGKwh);
    break;
    case 3: //litri
      sprintf(g_cBuffer,"%1.0fg/KWh - %1.2fl",fConsumoGKwh,vConsTot[1][ValueIndex]);
    break;
  }

  MarkText=g_cBuffer;
}
//---------------------------------------------------------------------------

void __fastcall TMain::Series17GetMarkText(TChartSeries *Sender,
      int ValueIndex, AnsiString &MarkText)
{
  static float fConsumoGKwh;

  fConsumoGKwh=CONSUMO_G_KWH(vCons[2][ValueIndex],vPotenzaRuota[2][ValueIndex]);
  switch(Unit_Cons)
  {
    case 0:
      sprintf(g_cBuffer,"%1.0fg/KWh - %1.2fl/h",fConsumoGKwh,vCons[2][ValueIndex]);
    break;
    case 1:
      sprintf(g_cBuffer,"%1.0fg/KWh - %1.2fl/min",fConsumoGKwh,vCons[2][ValueIndex]/60);
    break;
    case 2:
      sprintf(g_cBuffer,"%1.0fg/KWh",fConsumoGKwh);
    break;
    case 3: //litri
      sprintf(g_cBuffer,"%1.0fg/KWh - %1.2fl",fConsumoGKwh,vConsTot[2][ValueIndex]);
    break;
  }

  MarkText=g_cBuffer;
}
//---------------------------------------------------------------------------

void __fastcall TMain::Series18GetMarkText(TChartSeries *Sender,
      int ValueIndex, AnsiString &MarkText)
{
  static float fConsumoGKwh;

  fConsumoGKwh=CONSUMO_G_KWH(vCons[3][ValueIndex],vPotenzaRuota[3][ValueIndex]);
  switch(Unit_Cons)
  {
    case 0:
      sprintf(g_cBuffer,"%1.0fg/KWh - %1.2fl/h",fConsumoGKwh,vCons[3][ValueIndex]);
    break;
    case 1:
      sprintf(g_cBuffer,"%1.0fg/KWh - %1.2fl/min",fConsumoGKwh,vCons[3][ValueIndex]/60);
    break;
    case 2:
      sprintf(g_cBuffer,"%1.0fg/KWh",fConsumoGKwh);
    break;
    case 3: //litri
      sprintf(g_cBuffer,"%1.0fg/KWh - %1.2fl",fConsumoGKwh,vConsTot[3][ValueIndex]);
    break;
  }

  MarkText=g_cBuffer;
}
//---------------------------------------------------------------------------

void __fastcall TMain::Series28GetMarkText(TChartSeries *Sender,
      int ValueIndex, AnsiString &MarkText)
{
  static float fConsumoGKwh;

  fConsumoGKwh=CONSUMO_G_KWH(vCons[4][ValueIndex],vPotenzaRuota[4][ValueIndex]);
  switch(Unit_Cons)
  {
    case 0:
      sprintf(g_cBuffer,"%1.0fg/KWh - %1.2fl/h",fConsumoGKwh,vCons[4][ValueIndex]);
    break;
    case 1:
      sprintf(g_cBuffer,"%1.0fg/KWh - %1.2fl/min",fConsumoGKwh,vCons[4][ValueIndex]/60);
    break;
    case 2:
      sprintf(g_cBuffer,"%1.0fg/KWh",fConsumoGKwh);
    break;
    case 3: //litri
      sprintf(g_cBuffer,"%1.0fg/KWh - %1.2fl",fConsumoGKwh,vConsTot[4][ValueIndex]);
    break;
  }

  MarkText=g_cBuffer;
}
//---------------------------------------------------------------------------

void __fastcall TMain::Series29GetMarkText(TChartSeries *Sender,
      int ValueIndex, AnsiString &MarkText)
{
  static float fConsumoGKwh;

  fConsumoGKwh=CONSUMO_G_KWH(vCons[5][ValueIndex],vPotenzaRuota[5][ValueIndex]);
  switch(Unit_Cons)
  {
    case 0:
      sprintf(g_cBuffer,"%1.0fg/KWh - %1.2fl/h",fConsumoGKwh,vCons[5][ValueIndex]);
    break;
    case 1:
      sprintf(g_cBuffer,"%1.0fg/KWh - %1.2fl/min",fConsumoGKwh,vCons[5][ValueIndex]/60);
    break;
    case 2:
      sprintf(g_cBuffer,"%1.0fg/KWh",fConsumoGKwh);
    break;
    case 3: //litri
      sprintf(g_cBuffer,"%1.0fg/KWh - %1.2fl",fConsumoGKwh,vConsTot[5][ValueIndex]);
    break;
  }

  MarkText=g_cBuffer;
}
//---------------------------------------------------------------------------




void __fastcall TMain::Series31GetMarkText(TChartSeries *Sender,
      int ValueIndex, AnsiString &MarkText)
{
  static float fConsumoGKwh;

  fConsumoGKwh=CONSUMO_G_KWH(vCons[7][ValueIndex],vPotenzaRuota[7][ValueIndex]);
  switch(Unit_Cons)
  {
    case 0:
      sprintf(g_cBuffer,"%1.0fg/KWh - %1.2fl/h",fConsumoGKwh,vCons[7][ValueIndex]);
    break;
    case 1:
      sprintf(g_cBuffer,"%1.0fg/KWh - %1.2fl/min",fConsumoGKwh,vCons[7][ValueIndex]/60);
    break;
    case 2:
      sprintf(g_cBuffer,"%1.0fg/KWh",fConsumoGKwh);
    break;
    case 3: //litri
      sprintf(g_cBuffer,"%1.0fg/KWh - %1.2fl",fConsumoGKwh,vConsTot[7][ValueIndex]);
    break;
  }

  MarkText=g_cBuffer;
}
//---------------------------------------------------------------------------

void __fastcall TMain::Series30GetMarkText(TChartSeries *Sender,
      int ValueIndex, AnsiString &MarkText)
{
  static float fConsumoGKwh;

  fConsumoGKwh=CONSUMO_G_KWH(vCons[6][ValueIndex],vPotenzaRuota[6][ValueIndex]);
  switch(Unit_Cons)
  {
    case 0:
      sprintf(g_cBuffer,"%1.0fg/KWh - %1.2fl/h",fConsumoGKwh,vCons[6][ValueIndex]);
    break;
    case 1:
      sprintf(g_cBuffer,"%1.0fg/KWh - %1.2fl/min",fConsumoGKwh,vCons[6][ValueIndex]/60);
    break;
    case 2:
      sprintf(g_cBuffer,"%1.0fg/KWh",fConsumoGKwh);
    break;
    case 3: //litri
      sprintf(g_cBuffer,"%1.0fg/KWh - %1.2fl",fConsumoGKwh,vConsTot[6][ValueIndex]);
    break;
  }

  MarkText=g_cBuffer;
}
//---------------------------------------------------------------------------

void __fastcall TMain::Panel12Resize(TObject *Sender)
{
  TAirLabel->Font->Size=int(Panel12->Width /K_WW);
  if(TAirLabel->Font->Size > K_Font) TAirLabel->Font->Size=K_Font;

}
//---------------------------------------------------------------------------

void __fastcall TMain::tmrProcessDataTimer(TObject *Sender)
{
  #define PWM_AVG_SAMPLES 13 // Circa 2 secondi (il dato pwm è notificato ogni 150ms)
  static unsigned short usPwmSum=0, i=0,debugInfoPwm;
  static bool bConnected=true;
  static  char sDebugInfo [200];
  static long debugInfoP,debugInfoI;
  static unsigned long debugInfoTarget,debugInfoCurrent;
  tmrProcessData->Enabled=false;
  switch (getStatusDat())
  {
    case F10_SIMUL_DAT:
       SpeedButton2->Click();
    break;
    case START_SIMUL_DAT:
      if(Tipo_test==TEST_DECEL && Start->Visible)
      {
        runDat();
        Start->BitBtn1->Click();
      }
      else if(Tipo_test==TEST_STEP_TIMED && Step_test->Visible)
      {
        runDat();
        Step_test->BitBtn4Click(this);
      }
    break;
    case RUN_SIMUL_DAT:
       if(Connect_Dyra==false)
         clearDat();
    break;
    default:
    break;
  }

  /// Notifica all'applicazione tutte le informazioni rilevate dalla Dyra, in dati validi:
  /// rpm,cella,coppia,potenza,Tk,TOil,TAtm,PMan,PAtm
  if(bNotifyDyra)
    notifyDyra2Main();
  /// Notifica le informazioni rilevate dalla Dyn3 all'applicazione
  if(bNotifyDyn3)
  {
    // In DEBUG MODE prelevo il dato contenuto nel messaggio da 18 byte
    if(g_brakeControl.GetDebugMode())
    {
      memset(&g_debugInfo,0,sizeof(g_debugInfo));
      memset(sDebugInfo,0,sizeof(sDebugInfo));
      g_brakeControl.GetDebugInfo(&g_debugInfo);
      sprintf(sDebugInfo,"%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n",
        g_debugInfo.cData[0],
        g_debugInfo.cData[1],
        g_debugInfo.cData[2],
        g_debugInfo.cData[3],
        g_debugInfo.cData[4],
        g_debugInfo.cData[5],
        g_debugInfo.cData[6],
        g_debugInfo.cData[7],
        g_debugInfo.cData[8],
        g_debugInfo.cData[9],
        g_debugInfo.cData[10],
        g_debugInfo.cData[11],
        g_debugInfo.cData[12],
        g_debugInfo.cData[13],
        g_debugInfo.cData[14],
        g_debugInfo.cData[15],
        g_debugInfo.cData[16],
        g_debugInfo.cData[17],
        g_debugInfo.cData[18],
        g_debugInfo.cData[19]);
        debugInfoP= (long)((unsigned long)g_debugInfo.b.pL2+
                    ((unsigned long)g_debugInfo.b.pL1<<8)+
                    ((unsigned long)g_debugInfo.b.pH2<<16)+
                    ((unsigned long)g_debugInfo.b.pH1<<24));
        debugInfoI= (long)((unsigned long)g_debugInfo.b.iL2+
                    (unsigned long)(g_debugInfo.b.iL1<<8)+
                    (unsigned long)(g_debugInfo.b.iH2<<16)+
                    (unsigned long)(g_debugInfo.b.iH1<<24));
        debugInfoTarget = (unsigned short)g_debugInfo.b.targetL+
                          (unsigned short)(g_debugInfo.b.targetH<<8);
        debugInfoCurrent= (unsigned short)g_debugInfo.b.currentL+
                          (unsigned short)(g_debugInfo.b.currentH<<8);
        debugInfoPwm=(unsigned short)(g_debugInfo.b.pwmL)+(unsigned short)(g_debugInfo.b.pwmH<<8);
        if(frmDebugMode->cbRefresh->Checked)
        {

          if(frmDebugMode->sbMaxPoints->Max<frmDebugMode->chDebug->BottomAxis->Maximum-10)
          {
            frmDebugMode->sbMaxPoints->Max=frmDebugMode->chDebug->BottomAxis->Maximum;
            frmDebugMode->Label3->Caption=IntToStr(frmDebugMode->sbMaxPoints->Max);
          }//frmDebugMode->sbMaxPoints->Position=frmDebugMode->chDebug->BottomAxis->Maximum;
          if(frmDebugMode->chDebug->BottomAxis->Maximum>frmDebugMode->sbMaxPoints->Position+frmDebugMode->chDebug->BottomAxis->Minimum)
          {
           frmDebugMode->chDebug->BottomAxis->Scroll(1,true);
           frmDebugMode->chDebug->BottomAxis->Minimum=frmDebugMode->chDebug->BottomAxis->Minimum+1;
          }
          frmDebugMode->chDebug->SubTitle->Caption=sDebugInfo;
          frmDebugMode->srsP->Add(debugInfoP);
          frmDebugMode->srsI->Add(debugInfoI);
          frmDebugMode->srsTarget->Add(debugInfoTarget);
          frmDebugMode->srsCurrent->Add(debugInfoCurrent);
          frmDebugMode->srsPwm->Add((int)((float)debugInfoPwm/4096*100));
          frmDebugMode->srsP->Title="P "+IntToStr(debugInfoP);
          frmDebugMode->srsI->Title="I "+IntToStr(debugInfoI);
          frmDebugMode->srsTarget->Title="Trg "+IntToStr(debugInfoTarget);
          frmDebugMode->srsCurrent->Title="Cur "+IntToStr(debugInfoCurrent);
          frmDebugMode->srsPwm->Title="pwm "+IntToStr((int)((float)debugInfoPwm/4096*100))+"%";

        }
    }
    bNotifyDyn3=false;
    /// PWM freno
    usPwm=g_brakeControl.GetPwmAnte();
    usPwmSum+usPwm;
    i++;
    if(i>=PWM_AVG_SAMPLES)
    {
      usPwmSum/=PWM_AVG_SAMPLES;
      usPwmAvg=usPwmSum;
      usPwmSum=0;
      i=0;
    }
    CGauge2->Progress=usPwm;
    /// Stato scheda freno
    if(g_brakeControl.GetDyn3Status()==CDyn3::EDyn3NotAvail ||
       g_brakeControl.GetDyn3Status()==CDyn3::EDyn3NotDefined)
    {
       Main->Image1->Visible=true;
       Main->Image3->Visible=false;
    }
    else
    {
       Main->Image3->Visible=true;
       Main->Image1->Visible=false;
    }
    /// STATO connessione freno
    dyn3Status=g_brakeControl.GetDyn3Status();

    if( dyn3ErrorStatus.cStatus!=g_brakeControl.GetErrorStatus().cStatus ||
        dyn3Status==CDyn3::EDyn3NotDefined ||
        brakeStatus.c!=g_brakeControl.GetBrakeStatus().c ||
        dyn3ErrorStatus.cStatus==0 ||
        brakeStatus.c==0)


    /// Se lo stato è cambiato notifica alla finestra di manutenzione
//    if(brakeStatus.c!=g_brakeControl.GetBrakeStatus().c || tmrProcessData->Tag==3)
    {
     dyn3ErrorStatus=g_brakeControl.GetErrorStatus();
      tmrProcessData->Tag=4;
      brakeStatus=g_brakeControl.GetBrakeStatus();

      /// Dyn3 labels connesso/alimentato
      VrLabel1->Caption=(g_brakeControl.GetBrakeDisconnect(CDyn3::EFrontAxle))?Text83:Text84;
      VrLabel1->Font->Color=(g_brakeControl.GetBrakeDisconnect(CDyn3::EFrontAxle))?clRed:clGreen;
      VrLabel2->Caption=(g_brakeControl.GetBrakeNoPower(CDyn3::EFrontAxle))?Text82:Text81;
      VrLabel2->Font->Color=(g_brakeControl.GetBrakeNoPower(CDyn3::EFrontAxle))?clRed:clGreen;
      if(tipoBanco==TRACTOR_TWIN)
      {
        VrLabel4->Caption=(g_brakeControl.GetBrakeDisconnect(CDyn3::ERearAxle))?Text83:Text84;
        VrLabel4->Font->Color=(g_brakeControl.GetBrakeDisconnect(CDyn3::ERearAxle))?clRed:clGreen;
        VrLabel5->Caption=(g_brakeControl.GetBrakeNoPower(CDyn3::ERearAxle))?Text82:Text81;
        VrLabel5->Font->Color=(g_brakeControl.GetBrakeNoPower(CDyn3::ERearAxle))?clRed:clGreen;
      }
      /// Dyn3 label RPM presente

      if(brakeStatus.b.bBrakeAnteDisconnected==0 &&
         brakeStatus.b.bBrakeAnteNotPowered==0 &&
         g_brakeControl.isOn() && dyn3ErrorStatus.b.bDisconn==0 && dyn3ErrorStatus.b.bLostConn==0)
      {
        Main->VrLabel3->Caption=(brakeStatus.b.bRpmAnte)?Text81:Text82;
        Main->VrLabel3->Font->Color=(brakeStatus.b.bRpmAnte)?clGreen:clRed;
        bConnected=true;
      }
      else if(bConnected!=(brakeStatus.b.bBrakeAnteDisconnected==0 &&
         brakeStatus.b.bBrakeAnteNotPowered==0 &&
         g_brakeControl.isOn() && dyn3ErrorStatus.b.bDisconn==0 && dyn3ErrorStatus.b.bLostConn==0))
      {
        Main->VrLabel3->Caption=Text82;
        Main->VrLabel3->Font->Color=clRed;
        Main->PageControl1->ActivePage=Main->TabSheet9;
        bConnected=false;

      }

      /// Dyn3 label RPM presente
      if(tipoBanco==TRACTOR_TWIN)
      {
        if(brakeStatus.b.bBrakePostDisconnected==0 &&
         brakeStatus.b.bBrakePostNotPowered==0 &&
         g_brakeControl.isOn() && dyn3ErrorStatus.b.bDisconn==0 && dyn3ErrorStatus.b.bLostConn==0)
        {
          Main->VrLabel6->Caption=(brakeStatus.b.bRpmAnte)?Text81:Text82;
          Main->VrLabel6->Font->Color=(brakeStatus.b.bRpmAnte)?clGreen:clRed;
        }
        else if(bConnected!=(brakeStatus.b.bBrakeAnteDisconnected==0 &&
         brakeStatus.b.bBrakeAnteNotPowered==0 &&
         g_brakeControl.isOn() && dyn3ErrorStatus.b.bDisconn==0 && dyn3ErrorStatus.b.bLostConn==0))
        {
          Main->PageControl1->ActivePage=Main->TabSheet9;
          Main->VrLabel6->Caption=Text82;
          Main->VrLabel6->Font->Color=clRed;
          bConnected=false;
        }
      }


      ucFwType=g_brakeControl.GetFwType();
      ucFwVer =g_brakeControl.GetFwVer();
      CGauge2->MaxValue=
        (ucFwVer>4)?MAX_WORD:MAX_BYTE;
    } /// Fine cambio di stato Dyno3
  }
  /// Notifica Dyn3 connessa/disconnessa
  if(g_brakeControl.isOn()==false)
  {
    Main->VrLabel10->Font->Color=clRed;
    Main->VrLabel10->Caption=Text80;
    Connect_Dypt=false;
  }
  else
  {
    Main->VrLabel10->Font->Color=clGreen;
    Main->VrLabel10->Caption=Text79;
    if(Connect_Dypt==false)
      if(bPidControl) // Solo per utenza pidcontrol, leggo i parametri del pid
        btnRfrClick(this);
    Connect_Dypt=true;


  }
  /// Semaforo per gestire processamento dati applicazione
  /// Serve a garantire che non ci siano chiamate annidate della funzione ProcessData()
  if(WaitForSingleObject(hEvDataReady,0)==WAIT_OBJECT_0)
  {
    ResetEvent(hEvDataReady);
    if(g_dyraControl.isOn())
      ProcessData();
    SetEvent(hEvDataReady);
  }
  
  tmrProcessData->Enabled=true;
}
//---------------------------------------------------------------------------

void TMain::EndTest()
{
  // Invocata per terminare i test, al posto di Brake_control(0,0,1)
  // I test di freno manuale (TEST_BRAKE_MAN) non devono utilizzare questo comando
  if(!TimerStopPwm->Enabled)
  {
    if(!(Main->getStatusDat()==START_SIMUL_DAT || Main->getStatusDat()==RUN_SIMUL_DAT)) g_brakeControl.CmdStop();
//    if(bFileTrace)
    //{
      LogError(g_cDyn3Log,LOG_INFO,"Cmd STOP test");
    //}
    usPwmWrite=usPwmAvg;
    TimerStopPwm->Tag=usPwm;
    TimerStopPwm->Enabled=true;
    Series105->Clear();
    Series106->Clear();
    Series107->Clear();
    Step_test->CGauge1->Progress=0;
  }
}

void TMain::salvaImpoDat (struct TEST_SETS* locTestPrm)
{
  if(Tipo_test==1 || Rap_tot1==0)
    locTestPrm->rappAnt=1;
  else
    locTestPrm->rappAnt=Rap_tot1;
  locTestPrm->nPunti=g_dynoTest.testPrms.nPunti;
  memcpy(&locTestPrm->dataL,&dDateTest,sizeof(dDateTest));
  locTestPrm->nLancio=(float)nLanciTotali;
  //locTestPrm->coppiaSblocco=coppiaSblocco;
  locTestPrm->tipoTraz=0; // assegna il tipo di trazione
  locTestPrm->tempAtm=TAir; // assegna temp atmosferica
  locTestPrm->pressAtm=Val_pat; // assegna pressione atmosferica
  locTestPrm->umidRel=Val_Hr; // umidità relativa
  locTestPrm->fFact=fFact; // fattore di correzione prestazioni
  locTestPrm->tipoBanco=tipoBanco; // tipo di banco utilizzato
  locTestPrm->tipoTest=Tipo_test; // tipo di test utilizzato per eseguire la prova
  locTestPrm->tipoCiclo=Tipo_ciclo;
  locTestPrm->tipoAlim=Tipo_Alim;
  locTestPrm->tipoNorm=Tipo_norm;
  locTestPrm->tipoRappCalc=tipoRapp;
  locTestPrm->regRappCalc=Reg_calc_rap;


  locTestPrm->ptoConn=selPto;   // pto/motore
  locTestPrm->nPtoRap=N_pto_rap;
  locTestPrm->nMotRap=N_mot_rap;

 	locTestPrm->rendPto=Rend_PTO;        // rendimento trasmissione


  locTestPrm->diamRulli=dynoPrm.bench.diamRullo;
  locTestPrm->unitCop=Unit_Cop;
  locTestPrm->unitPot=Unit_Pot;
  locTestPrm->unitTem=Unit_Tem;
  locTestPrm->unitVel=Unit_Vel;
  locTestPrm->unitMap=Unit_Map;
  locTestPrm->velIniTac=Vel_ini_tac;
  locTestPrm->stepVelTac=Step_vel_tac;
  locTestPrm->Lingua=Lingua;
  locTestPrm->regIniAcc=Reg_iniz_acc;
  locTestPrm->regFineAcc=Reg_fine_acc;
  locTestPrm->tempoAcc=Tempo_acc;
  locTestPrm->regIniDec=Reg_iniz_dec;
  locTestPrm->regFineDec=Reg_fine_dec;
  locTestPrm->tempoDec=Tempo_dec;
  locTestPrm->velIni=Vel_iniz;
  locTestPrm->velFine=Vel_fine;
  locTestPrm->regMin=rpmStart;
  locTestPrm->regMax=rpmStop;
  locTestPrm->deltaReg=Delta_reg;
  locTestPrm->regStop=Reg_Stp;
  locTestPrm->tempoStaz=Tempo_staz;
  locTestPrm->coeffFm=Coeff_fm;
  //locTestPrm->inerTempoAcc=Tempo_accel;      // contiene tempo di esecuzione del test parte accelerativa/dec
  //locTestPrm->inerTempoAccTot=Tempo_acc_tot;    // contiene tempo totale di esecuzione del test
  locTestPrm->inerMaxRegAnte=imax_reg_A;         // contiene il massimo regime asse anteriore
  //locTestPrm->inerMaxRegPost=imax_reg_P;         // contiene il massimo regime asse posteriore
  /// Valori massimi
  locTestPrm->maxPRuota    =Max_pot_ruota;
  locTestPrm->maxCRuota    =Max_cop_ruota;
  locTestPrm->rpmMaxPMotore=Regime_motore;
  locTestPrm->rpmMaxCMotore=Regime_motore2;
  locTestPrm->maxManifol   =Max_manifol;
  locTestPrm->maxTermoK    =Max_termocoppia;
  locTestPrm->maxTempOil   =Max_temp_olio;
  locTestPrm->posMaxP=iPos;
  locTestPrm->IntervCalcPrestRpm=dynoPrm.thres.IntervCalcPrestRpm;
  locTestPrm->verSw=Ver_sw;
  locTestPrm->selPto=selPto;  // selPto, inserita in locaz fCx
  locTestPrm->consMaxP=consMaxP;
  locTestPrm->consMaxC=consMaxC;
  locTestPrm->coppiaMaxPow=coppiaMaxPow;
}

void TMain::azzeraPannelli()
{
  GiriLabel->Caption="";
  CopLabel->Caption="";
  PotLabel->Caption="";
  MapLabel->Caption="";
  ConsLabel->Caption="";
  ConsLabel2->Caption="";
  TAirLabel->Caption="";
  TKLabel->Caption="";
  OilLabel->Caption="";
  sTest_antep=-1;
  Out_dat=true;
}

void __fastcall TMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  int iRes;
  // avvisa per salvare il layout utente se modificato
  if(Mod_imp==true)
  {
    MessageBeep(MB_ICONEXCLAMATION);
    iRes=MessageDlg(Str10,mtConfirmation,TMsgDlgButtons() << mbYes << mbNo << mbCancel,0);
    if(iRes==mrYes)
      salva_imp(); // Se SI salva funzione di lettura impostazioni
    else if(iRes==mrNo)
    {
      TIniFile *ini;
      ini = new TIniFile(utils.GetProgramDataName(".ini"));
      ini->WriteBool( "Stato_Chiusura", "OK",true);
      delete ini;
    }
    else if(iRes == mrCancel)
      CanClose = false;
  }
}


void TMain::sommaContaore()
{
        //sommo i contatori
        //e li riporto a 0

        if (contaoreSecondi>30) contaoreMinuti++;
        dynoPrm.count.nCountMinTotRpmRulli+=contaoreMinuti;      //minuti totali funzionamento
        dynoPrm.count.nCountMinParRpmRulli+=contaoreMinuti;      //minuti parziali funzionamento
        nCountMinTotRpmRulli+=contaoreMinuti;
        //azzero i contatori
        contaoreMinuti=0;
        contaoreSecondi=0;
}
//---------------------------------------------------------------------------





void __fastcall TMain::tmrContaOreTimer(TObject *Sender)
{
 //ogni 5 secondi controllo se i rulli girano
  if (lRpm>0)
  {
    //sono passati 5 secondi
    contaoreSecondi+=5;
    if(contaoreSecondi>=60)
    {
      //passato un minuto
      contaoreSecondi=0;
      contaoreMinuti++;
    }
  }
}
//---------------------------------------------------------------------------

bool TMain::scriviFileImpo()
{
  //riscrivo il fileImpo
  //modifica per tenere il conto dei lanci e durata funzionamento
  String flImpo;
  //salvo il file.dmt
  if(tipoBanco==2)
    flImpo=utils.GetProgramDataPath("File_impo_trailer.dmt");
  else
    flImpo=utils.GetProgramDataPath("File_impo_trac.dmt");
  try
  {
    ofstream out(flImpo.c_str(),ios::out | ios::binary);  // salva le impostazioni
    if(out.is_open())
    {
      out.write((unsigned char*)&dynoPrm,sizeof (DYNO_SETS));
      out.close();
      return true;
    }
  }
  catch (Exception &exception)
  {
      Application->ShowException(&exception);
      return false;
  }
  return false;
}

void TMain::caricaImpoTest(int i)
{

  if(i<0 || i>7)
    return;



  Rap_tot1=datPrm[i].rappAnt;
  selPto=datPrm[i].ptoConn;
  N_mot_rap=datPrm[i].nMotRap;
  N_pto_rap=datPrm[i].nPtoRap;

  TAir=datPrm[i].tempAtm; // assegna temp atmosferica
  Val_pat=datPrm[i].pressAtm; // assegna pressione atmosferica
  Val_Hr=datPrm[i].umidRel; // umidità relativa
  fFact=datPrm[i].fFact; // fattore di correzione prestazioni
  tipoBanco=datPrm[i].tipoBanco; // tipo di banco utilizzato
  Tipo_test=datPrm[i].tipoTest; // tipo di test utilizzato per eseguire la prova
  Tipo_norm=datPrm[i].tipoNorm;
  tipoRapp=datPrm[i].tipoRappCalc;
  Reg_calc_rap=datPrm[i].regRappCalc;
 	Rend_PTO=datPrm[i].rendPto;        // rendimento trasmissione
  Unit_Cop=datPrm[i].unitCop;
  Unit_Pot=datPrm[i].unitPot;
  Unit_Tem=datPrm[i].unitTem;
  Unit_Vel=datPrm[i].unitVel;
  Unit_Map=datPrm[i].unitMap;
  Vel_ini_tac=datPrm[i].velIniTac;
  Step_vel_tac=datPrm[i].stepVelTac;
  Lingua=datPrm[i].Lingua;
  Reg_iniz_acc=datPrm[i].regIniAcc;
  Reg_fine_acc=datPrm[i].regFineAcc;
  Tempo_acc=datPrm[i].tempoAcc;
  Reg_iniz_dec=datPrm[i].regIniDec;
  Reg_fine_dec=datPrm[i].regFineDec;
  Tempo_dec=datPrm[i].tempoDec;
  Vel_iniz=datPrm[i].velIni;
  Vel_fine=datPrm[i].velFine;
  rpmStart=datPrm[i].regMin;
  rpmStop=datPrm[i].regMax;
  Delta_reg=datPrm[i].deltaReg;
  Reg_Stp=datPrm[i].regStop;
  Tempo_staz=datPrm[i].tempoStaz;
  Coeff_fm=datPrm[i].coeffFm;
  imax_reg_A=datPrm[i].inerMaxRegAnte;         // contiene il massimo regime asse anteriore
  /// Valori massimi
  Max_pot_ruota=datPrm[i].maxPRuota;
  Max_cop_ruota=datPrm[i].maxCRuota;
  Regime_motore=datPrm[i].rpmMaxPMotore;
  Regime_motore2=datPrm[i].rpmMaxCMotore;
  Max_manifol=datPrm[i].maxManifol;
  Max_termocoppia=datPrm[i].maxTermoK;
  Max_temp_olio=datPrm[i].maxTempOil;
  consMaxP=datPrm[i].consMaxP;
  consMaxC=datPrm[i].consMaxC;
  coppiaMaxPow=datPrm[i].coppiaMaxPow;
}
//---------------------------------------------------------------------------
void __fastcall TMain::CaricaImpostazioni8Click(TObject *Sender)
{
  caricaImpoTest(7);
}
//---------------------------------------------------------------------------

void __fastcall TMain::CaricaImpostazioni7Click(TObject *Sender)
{
  caricaImpoTest(6);
}
//---------------------------------------------------------------------------

void __fastcall TMain::CaricaImpostazioni6Click(TObject *Sender)
{
  caricaImpoTest(5);
}
//---------------------------------------------------------------------------

void __fastcall TMain::CaricaImpostazioni5Click(TObject *Sender)
{
  caricaImpoTest(4);
}
//---------------------------------------------------------------------------

void __fastcall TMain::CaricaImpostazioni4Click(TObject *Sender)
{
  caricaImpoTest(3);
}
//---------------------------------------------------------------------------

void __fastcall TMain::CaricaImpostazioni3Click(TObject *Sender)
{
  caricaImpoTest(2);
}
//---------------------------------------------------------------------------

void __fastcall TMain::CaricaImpostazioni2Click(TObject *Sender)
{
  caricaImpoTest(1);
}
//---------------------------------------------------------------------------

void __fastcall TMain::CaricaImpostazioni1Click(TObject *Sender)
{
  caricaImpoTest(0);
}
//---------------------------------------------------------------------------

void __fastcall TMain::Fattoredicorrezione1Click(TObject *Sender)
{
  //modifica fattore di correzione della prova selezionata
  //imposto il fattore di correzione attuale
  FattCorr->setFatt(datPrm[0].fFact);
  FattCorr->ShowModal();
  //controllo l'esit
  if (FattCorr->getEsit())
  {
    //salvo il nuovo fattore di correzione
    datPrm[0].fFact=FattCorr->getFatt();
    //imposto come correzione manuale
    datPrm[0].tipoNorm=6;  //manuale
    //ricalcolo i grafici
    showPerformance(0);
  }
}
//---------------------------------------------------------------------------

void __fastcall TMain::Fattoredicorrezione2Click(TObject *Sender)
{
  //modifica fattore di correzione della prova selezionata
  //imposto il fattore di correzione attuale
  FattCorr->setFatt(datPrm[1].fFact);
  FattCorr->ShowModal();
  //controllo l'esit
  if (FattCorr->getEsit())
  {
    //salvo il nuovo fattore di correzione
    datPrm[1].fFact=FattCorr->getFatt();
    //imposto come correzione manuale
    datPrm[1].tipoNorm=6;  //manuale
    //ricalcolo i grafici
    showPerformance(1);
  }
}
//---------------------------------------------------------------------------

void __fastcall TMain::Fattoredicorrezione3Click(TObject *Sender)
{
  //modifica fattore di correzione della prova selezionata
  //imposto il fattore di correzione attuale
  FattCorr->setFatt(datPrm[2].fFact);
  FattCorr->ShowModal();
  //controllo l'esit
  if (FattCorr->getEsit())
  {
    //salvo il nuovo fattore di correzione
    datPrm[2].fFact=FattCorr->getFatt();
    //imposto come correzione manuale
    datPrm[2].tipoNorm=6;  //manuale
    //ricalcolo i grafici
    showPerformance(2);
  }
}
//---------------------------------------------------------------------------

void __fastcall TMain::Fattoredicorrezione4Click(TObject *Sender)
{
  //modifica fattore di correzione della prova selezionata
  //imposto il fattore di correzione attuale
  FattCorr->setFatt(datPrm[3].fFact);
  FattCorr->ShowModal();
  //controllo l'esit
  if (FattCorr->getEsit())
  {
    //salvo il nuovo fattore di correzione
    datPrm[3].fFact=FattCorr->getFatt();
    //imposto come correzione manuale
    datPrm[3].tipoNorm=6;  //manuale
    //ricalcolo i grafici
    showPerformance(3);
  }
}
//---------------------------------------------------------------------------

void __fastcall TMain::Fattoredicorrezione5Click(TObject *Sender)
{
  //modifica fattore di correzione della prova selezionata
  //imposto il fattore di correzione attuale
  FattCorr->setFatt(datPrm[4].fFact);
  FattCorr->ShowModal();
  //controllo l'esit
  if (FattCorr->getEsit())
  {
    //salvo il nuovo fattore di correzione
    datPrm[4].fFact=FattCorr->getFatt();
    //imposto come correzione manuale
    datPrm[4].tipoNorm=6;  //manuale
    //ricalcolo i grafici
    showPerformance(4);
  }
}
//---------------------------------------------------------------------------

void __fastcall TMain::Fattoredicorrezione6Click(TObject *Sender)
{
  //modifica fattore di correzione della prova selezionata
  //imposto il fattore di correzione attuale
  FattCorr->setFatt(datPrm[5].fFact);
  FattCorr->ShowModal();
  //controllo l'esit
  if (FattCorr->getEsit())
  {
    //salvo il nuovo fattore di correzione
    datPrm[5].fFact=FattCorr->getFatt();
    //imposto come correzione manuale
    datPrm[5].tipoNorm=6;  //manuale
    //ricalcolo i grafici
    showPerformance(5);
  }
}
//---------------------------------------------------------------------------

void __fastcall TMain::Fattoredicorrezione7Click(TObject *Sender)
{
  //modifica fattore di correzione della prova selezionata
  //imposto il fattore di correzione attuale
  FattCorr->setFatt(datPrm[6].fFact);
  FattCorr->ShowModal();
  //controllo l'esit
  if (FattCorr->getEsit())
  {
    //salvo il nuovo fattore di correzione
    datPrm[6].fFact=FattCorr->getFatt();
    //imposto come correzione manuale
    datPrm[6].tipoNorm=6;  //manuale
    //ricalcolo i grafici
    showPerformance(6);
  }
}
//---------------------------------------------------------------------------

void __fastcall TMain::Fattoredicorrezione8Click(TObject *Sender)
{
  //modifica fattore di correzione della prova selezionata
  //imposto il fattore di correzione attuale
  FattCorr->setFatt(datPrm[7].fFact);
  FattCorr->ShowModal();
  //controllo l'esit
  if (FattCorr->getEsit())
  {
    //salvo il nuovo fattore di correzione
    datPrm[7].fFact=FattCorr->getFatt();
    //imposto come correzione manuale
    datPrm[7].tipoNorm=6;  //manuale
    //ricalcolo i grafici
    showPerformance(7);
  }
}
//---------------------------------------------------------------------------

void __fastcall TMain::btnPIDClick(TObject *Sender)
{
  int iNewValue;

  iNewValue=StrToIntDef(edP->Text,sPidKp);
  if(iNewValue<1 || iNewValue > 65535)
  {
    edP->Text=sPidKp;
  }
  else
    sPidKp=iNewValue;

  iNewValue=StrToIntDef(edI->Text,sPidKi);
  if(iNewValue<0 || iNewValue > 5000)
  {
    edI->Text=sPidKi;
  }
  else
    sPidKi=iNewValue;

  iNewValue=StrToIntDef(edD->Text,sPidKd);
  if(iNewValue<0 || iNewValue > 5000)
  {
    edD->Text=sPidKd;
  }
  else
    sPidKd=iNewValue;


  g_brakeControl.CmdWritePID(sPidKp,sPidKi,sPidKd);
  do
    Application->ProcessMessages();
  while(g_brakeControl.IsCmdInProgress());
  g_brakeControl.CmdReadPID();
  do
    Application->ProcessMessages();
  while(g_brakeControl.IsCmdInProgress());
  g_brakeControl.GetPID(&sPidKp,&sPidKi,&sPidKd);

  edP->Text=sPidKp;
  edI->Text=sPidKi;
  edD->Text=sPidKd;
  TIniFile *ini;

  ini = new TIniFile(utils.GetProgramDataName(".ini"));
  ini->WriteInteger ("PID control", "K proporzionale",sPidKp);
  ini->WriteInteger ("PID control", "K integrale",sPidKi);
  ini->WriteInteger ("PID control", "K derivativo",sPidKd);
  delete ini;
}
//---------------------------------------------------------------------------




void __fastcall TMain::btnRfrClick(TObject *Sender)
{
  unsigned short sNewKp,sNewKi,sNewKd;

  g_brakeControl.CmdReadPID();
  do
    Application->ProcessMessages();
  while(g_brakeControl.IsCmdInProgress());
  g_brakeControl.GetPID(&sPidKp,&sPidKi,&sPidKd);
  TIniFile *ini;
  ini = new TIniFile(utils.GetProgramDataName(".ini"));
  sNewKp=ini->ReadInteger ("PID control", "K proporzionale",3500);
  sNewKi=ini->ReadInteger ("PID control", "K integrale",50);
  sNewKd=ini->ReadInteger ("PID control", "K derivativo",0);
  delete ini;

  String strPID;
  strPID=+" P="+IntToStr(sNewKp)+" I="+IntToStr(sNewKi)+" D="+IntToStr(sNewKd);
  if( sNewKp!=sPidKp ||
      sNewKi!=sPidKi ||
      sNewKd!=sPidKd)
    if(MessageDlg("aggiornare il PID sulla scheda con le ultime costanti memorizzate?("+strPID+")",mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
    {
      sPidKp=sNewKp;
      sPidKi=sNewKi;
      sPidKd=sNewKd;
      g_brakeControl.CmdWritePID(sPidKp,sPidKi,sPidKd);
    }


  edP->Text=sPidKp;
  edI->Text=sPidKi;
  edD->Text=sPidKd;
}
//---------------------------------------------------------------------------


void __fastcall TMain::ApriDatSerialeClick(TObject *Sender)
{
//  if(Connect_Dyra)
    loadDat();
}
//---------------------------------------------------------------------------
void TMain::LoadTestPrms(struct TEST_SETS* datPrm)
{
  Rap_tot1=datPrm->rappAnt;
  selPto=datPrm->ptoConn;
  N_mot_rap=datPrm->nMotRap;
  N_pto_rap=datPrm->nPtoRap;

  TAir=datPrm->tempAtm; // assegna temp atmosferica
  Val_pat=datPrm->pressAtm; // assegna pressione atmosferica
  Val_Hr=datPrm->umidRel; // umidità relativa
  fFact=datPrm->fFact; // fattore di correzione prestazioni
  tipoBanco=datPrm->tipoBanco; // tipo di banco utilizzato
  Tipo_test=datPrm->tipoTest; // tipo di test utilizzato per eseguire la prova

  SpeedButton8->Down=false;
  SpeedButton4->Down=false;
  SpeedButton15->Down=false;
  SpeedButton17->Down=false;
  SpeedButton21->Down=false;
  switch(Tipo_test)
  {
    case TEST_STEP_TIMED:
      SpeedButton4->Down=true;
      Impostazioni->Test->ActivePageIndex=1;
    break;
    case TEST_STEP:
      Impostazioni->Test->ActivePageIndex=2;
      SpeedButton8->Down=true;
    break;
    case TEST_DECEL:
      Impostazioni->Test->ActivePageIndex=3;
      SpeedButton15->Down=true;
    break;
    case TEST_COST_TRQ:
      Impostazioni->Test->ActivePageIndex=4;
      SpeedButton21->Down=true;
    break;
    case TEST_BRAKE_MAN:
      Impostazioni->Test->ActivePageIndex=5;
      SpeedButton17->Down=true;
      Impostazioni->Rap->Visible=false;
    break;
  }
  Tipo_norm=datPrm->tipoNorm;
  tipoRapp=datPrm->tipoRappCalc;
  Reg_calc_rap=datPrm->regRappCalc;
 	Rend_PTO=datPrm->rendPto;        // rendimento trasmissione
  Unit_Cop=datPrm->unitCop;
  Unit_Pot=datPrm->unitPot;
  Unit_Tem=datPrm->unitTem;
  Unit_Vel=datPrm->unitVel;
  Unit_Map=datPrm->unitMap;
  Vel_ini_tac=datPrm->velIniTac;
  Step_vel_tac=datPrm->stepVelTac;
  Lingua=datPrm->Lingua;
  Reg_iniz_acc=datPrm->regIniAcc;
  Reg_fine_acc=datPrm->regFineAcc;
  Tempo_acc=datPrm->tempoAcc;
  Reg_iniz_dec=datPrm->regIniDec;
  Reg_fine_dec=datPrm->regFineDec;
  Tempo_dec=datPrm->tempoDec;
  Vel_iniz=datPrm->velIni;
  Vel_fine=datPrm->velFine;
  rpmStart=datPrm->regMin;
  rpmStop=datPrm->regMax;
  Delta_reg=datPrm->deltaReg;
  Reg_Stp=datPrm->regStop;
  Tempo_staz=datPrm->tempoStaz;
  Coeff_fm=datPrm->coeffFm;
  imax_reg_A=datPrm->inerMaxRegAnte;         // contiene il massimo regime asse anteriore

}
void __fastcall TMain::bAssistanceClick(TObject *Sender)
{
  String s=utils.GetProgramPath("DIMSPORTSUPPORT.EXE");
  if(FileExists(s))
    ShellExecute(Handle,"open",s.c_str(),0,0,SW_SHOWNORMAL);
  else
    MessageDlg("DimsportSupport NOT FOUND!",mtError,TMsgDlgButtons() << mbCancel,0);
}
//---------------------------------------------------------------------------

void __fastcall TMain::sbDataInClick(TObject *Sender)
{
#ifdef _INDUSTRY4
  sFileExported="";
  /// File in cartella di ingresso
  frmFileList->Left=Mouse->CursorPos.x;
  frmFileList->Top=Mouse->CursorPos.y;
  frmFileList->flb->Directory=Main->sInPath;
  frmFileList->flb->Update();
  frmFileList->flb->Refresh();
  frmFileList->ShowModal();
#endif
}
//---------------------------------------------------------------------------

void __fastcall TMain::sbDataOutClick(TObject *Sender)
{
/*
  String sVehiclePlate,sPathSearch;
  ofstream outFile;
  TSearchRec sr;

  if(iNTest>0 && iNTest<=8)
  {
    /// CHECK Nome del file out csv
    if(Impostazioni->Edit28->Text.IsEmpty()||Impostazioni->Edit28->Text=="")
    {
      MessageDlg("VEHICLE PLATE not found, please insert it!",mtError,TMsgDlgButtons() << mbCancel,0);
      sVehiclePlate="untitled";
      return;
    }
    else
    {
      sVehiclePlate=Impostazioni->Edit28->Text;
      if(FileExists(Main->sOutPath+"\\"+sVehiclePlate+".csv"))
      {
        MessageDlg("VEHICLE PLATE present in output directory!",mtError,TMsgDlgButtons() << mbCancel,0);
        return;
      }
    }
    if(FindFirst(Main->sOutPath+"\\*.tmp",faArchive,sr)!=0)
    {

      Main->sFileExported=Main->sOutPath+"\\"+sVehiclePlate+".tmp";
      /// Creazione file out .tmp
      outFile.open(Main->sFileExported.c_str(),ios_base::out );
      if(outFile.is_open())
      {
        outFile << "Workshop;" << Ragione.c_str() <<endl;
        outFile << "Address;" << Indirizzo.c_str() << endl;
        outFile << "Phone;" << Telefono.c_str() << endl;
        outFile << "Fax;" << Fax.c_str() << endl;
        outFile << "E-mail;" << EMail.c_str() << endl;
        outFile << "Client name;" << Cliente << endl;
        if(Main->sFileExported!="untitled.tmp")
          outFile << "Vehicle plate;" << sVehiclePlate.c_str() << endl;
        outFile << "Manufacturer;" << Costruttore << endl;
        outFile << "Model;" << Modello << endl;
        outFile << "Type;" << Allestimento << endl;
        outFile.close();
//        Savetest1->Enabled=false;
        //sbDataIn->Enabled=false;
        //sbDataOut->Enabled=true;
      }
      else
        MessageBox(this,"ERROR","Out file not found!",MB_OK | MB_ICONERROR);
    }*/

    saveTest(iNTest);
    /*
    sbDataIn->Enabled=true;

  }
  else
    MessageBox(this,"ERROR","Test not found!",MB_OK | MB_ICONERROR);*/
}
//---------------------------------------------------------------------------
void TMain::saveTest(int i)
{
  ofstream datFile;
  char Buffer[500];
  float fField1;
  struct lconv * lc;
  String sLocalName;



  String sVehiclePlate,sPathSearch;
  ofstream outFile;
  TSearchRec sr;

  if(i>0 && i<=8)
  {
    /// CHECK Nome del file out csv
    if(Impostazioni->Edit28->Text.IsEmpty()||Impostazioni->Edit28->Text=="")
    {
      MessageDlg("VEHICLE PLATE not found, please insert it!",mtError,TMsgDlgButtons() << mbCancel,0);
      sVehiclePlate="untitled";
      return;
    }
    else
    {
      sVehiclePlate=Impostazioni->Edit28->Text;
      if(FileExists(Main->sOutPath+"\\"+sVehiclePlate+".csv"))
      {
        MessageDlg("VEHICLE PLATE present in output directory!",mtError,TMsgDlgButtons() << mbCancel,0);
        return;
      }
    }
    if(FindFirst(Main->sOutPath+"\\*.tmp",faArchive,sr)!=0)
    {

      Main->sFileExported=Main->sOutPath+"\\"+sVehiclePlate+".tmp";
      /// Creazione file out .tmp
      outFile.open(Main->sFileExported.c_str(),ios_base::out );
      if(outFile.is_open())
      {
        outFile << "Workshop;" << Ragione.c_str() <<endl;
        outFile << "Address;" << Indirizzo.c_str() << endl;
        outFile << "Phone;" << Telefono.c_str() << endl;
        outFile << "Fax;" << Fax.c_str() << endl;
        outFile << "E-mail;" << EMail.c_str() << endl;
        outFile << "Client name;" << Cliente << endl;
        if(Main->sFileExported!="untitled.tmp")
          outFile << "Vehicle plate;" << sVehiclePlate.c_str() << endl;
        outFile << "Manufacturer;" << Costruttore << endl;
        outFile << "Model;" << Modello << endl;
        outFile << "Type;" << Allestimento << endl;
        outFile.close();
//        Savetest1->Enabled=false;
        //sbDataIn->Enabled=false;
        //sbDataOut->Enabled=true;
      }
      else
        MessageBox(this,"ERROR","Out file not found!",MB_OK | MB_ICONERROR);
    }

  if(sFileExported!="" && bTestToExport)
  {
    sLocalName=setlocale( LC_NUMERIC, "");
    lc = localeconv ();
    DecimalSeparator=lc->decimal_point[0];
    datFile.open(sFileExported.c_str(),ios_base::out | ios::binary || ios::app);
    if (!datFile) return; // Uscita senza speranza
    datFile << endl;
    datFile << "RESULTANT DATA;" << endl;
    datFile << endl;
    if(i>0 && i<=8)
    {
      sprintf(Buffer,"%-1.1f",datPrm[i-1].tempAtm);
      datFile << "Atmospheric temperature [°C];" << Buffer << endl;
      sprintf(Buffer,"%-1.1f",datPrm[i-1].pressAtm);
      datFile << "Atmospheric pressure [mbar];" << Buffer << endl;
      sprintf(Buffer,"%-1.1f",datPrm[i-1].umidRel);
      datFile << "Relative humidity [%];" << Buffer << endl;
            switch(int(datPrm[i-1].tipoNorm))
            {
            case 0:
              strcpy(Buffer,normSAE.c_str());
            break;
            case 1:
              strcpy(Buffer,normISO.c_str());
            break;
            case 2:
              strcpy(Buffer,normCE.c_str());
            break;
            case 3:
              strcpy(Buffer,normECE.c_str());
            break;
            case 4:
              strcpy(Buffer,normCEE.c_str());
            break;
            case 5:
              strcpy(Buffer,normDIN.c_str());
            break;
            case 6:
              strcpy(Buffer,normOCDE.c_str());
            break;
            case 7:
              strcpy(Buffer,"MANUAL");
            break;
            }
      datFile << "Correcting regulation;" << Buffer << endl;

      sprintf(Buffer,"%1.3f",datPrm[i-1].fFact);
      datFile << "Correcting factor;" << Buffer << endl;
      sprintf(Buffer,"%1.2f",datPrm[i-1].rappAnt);
      datFile << "Total transmission ratio;" << Buffer << endl;
      if(datPrm[i-1].selPto==1)
        datFile << "RPM to PTO;Take off efficiency;" << datPrm[i-1].rendPto << endl;
      else
        datFile << "RPM to Engine;" << endl;
      datFile << "Type of test;" << Impostazioni->ComboBox1->Items->Strings[Tipo_test].c_str() << endl;

      datFile << "MAXIMUM MEASUREMENT TESTED;" <<  endl;
      fField1=datPrm[i-1].fFact*datPrm[i-1].maxCRuota*fK_convC;
      sprintf(Buffer,"%-1.1f;",fField1);
      datFile << "Maximum torque [Kg·m];" << Buffer << endl;
      fField1=datPrm[i-1].fFact*datPrm[i-1].maxPRuota*fK_convP;
      sprintf(Buffer,"%-1.1f;",fField1);
      datFile << "Maximum power  [Cv];" << Buffer << endl;
       if(datPrm[i-1],consMaxC>0)
      {

        sprintf(Buffer,"%-1.1f",datPrm[i-1].consMaxC);
        datFile << "Spec. fuel cons. gr%/kWh @Max Torque;" << Buffer << endl;
        sprintf(Buffer,"%-1.1f",datPrm[i-1].consMaxP);
        datFile << "Spec. fuel cons. gr%/kWh @Max Power;"<< Buffer << endl;

      }
      // manifold
      if(Vis_Pmap)
      {
        if(Unit_Map==0)
         datFile << Text26.c_str() << " [mbar];" << FormatFloat("0.#",datPrm[i-1].maxManifol).c_str() << endl;

        if(Unit_Map==1)
          datFile << Text26.c_str() << " [psi];" << FormatFloat("0.##",datPrm[i-1].maxManifol*0.0145).c_str() << endl;
      }

      // Termocoppia
      if(Vis_T_K)
      {
        if(Unit_Tem==0)
          datFile << Text28.c_str() << " [°C];"<<FormatFloat("0.#",datPrm[i-1].maxTermoK).c_str() << endl;
        if(Unit_Tem==1)
          datFile << Text28.c_str() << " [°F];"<<FormatFloat("0.#",ConvertTo(datPrm[i-1].maxTermoK,tuFahrenheit)).c_str() << endl;
      }

      // Temperatura olio motore
      if(Vis_T_N)
      {
        if(Unit_Tem==0)
          datFile << Text29.c_str() << " [°C];" << datPrm[i-1].maxTempOil << endl;
          RichEdit1->Lines->Add("  "+Text29+datPrm[i-1].maxTempOil+" °C");
        if(Unit_Tem==1)
          datFile << Text29.c_str() << " [°F];" << ConvertTo(datPrm[i-1].maxTempOil,tuFahrenheit) << endl;

      }


      SavePerformance(&datFile,
                      vGiri[i-1],
                      datPrm[i-1].fFact,
                      vCoppiaRuota[i-1],
                      vPotenzaRuota[i-1],
                      vTermoOil[i-1],
                      vTermoK[i-1],
                      vPresMAP[i-1],
                      vCons[i-1],
                      vConsTot[i-1],
                      DIM_MEDIA);

    }

    datFile.close();
    DeleteFile(sFileExported.SubString(0,sFileExported.Length()-3)+"csv");
    RenameFile(sFileExported,sFileExported.SubString(0,sFileExported.Length()-3)+"csv");
    /// Elimino il file di input una volta ultimato il file di output
    DeleteFile(sFileImported);
    bTestToExport=false;
    Savetest1->Enabled=false;
    memset(vTestEseguiti,0,sizeof(vTestEseguiti));
    iTestEseguiti=0;
    t1->Visible=false;
    t2->Visible=false;
    t3->Visible=false;
    t4->Visible=false;
    t5->Visible=false;
    t6->Visible=false;
    t7->Visible=false;
    t8->Visible=false;
    sLocalName=setlocale( LC_NUMERIC, "C");
    DecimalSeparator='.';
    sbDataOut->Enabled=false;
    //sbDataIn->Enabled=true;
//    return;

  }
  sbDataIn->Enabled=true;

  }
  else
    MessageBox(this,"ERROR","Test not found!",MB_OK | MB_ICONERROR);

}
void __fastcall TMain::t1Click(TObject *Sender)
{
  saveTest(vTestEseguiti[0]);
}
//---------------------------------------------------------------------------

void __fastcall TMain::t2Click(TObject *Sender)
{
  saveTest(vTestEseguiti[1]);  
}
//---------------------------------------------------------------------------

void __fastcall TMain::t3Click(TObject *Sender)
{
  saveTest(vTestEseguiti[2]);  
}
//---------------------------------------------------------------------------

void __fastcall TMain::t4Click(TObject *Sender)
{
  saveTest(vTestEseguiti[3]);
}
//---------------------------------------------------------------------------

void __fastcall TMain::t5Click(TObject *Sender)
{
  saveTest(vTestEseguiti[4]);  
}
//---------------------------------------------------------------------------

void __fastcall TMain::t6Click(TObject *Sender)
{
  saveTest(vTestEseguiti[5]);
}
//---------------------------------------------------------------------------

void __fastcall TMain::t7Click(TObject *Sender)
{
  saveTest(vTestEseguiti[6]);  
}
//---------------------------------------------------------------------------

void __fastcall TMain::t8Click(TObject *Sender)
{
  saveTest(vTestEseguiti[7]);
}
//---------------------------------------------------------------------------

void __fastcall TMain::In1Click(TObject *Sender)
{
  char cDocPath[MAX_PATH];
  if(sInPath=="")
  {
    SHGetFolderPath(NULL, CSIDL_PERSONAL,NULL,NULL,cDocPath);
    strcat(cDocPath,"\\DynoTractorTest\\In");
    sInPath=(String)cDocPath;//cArchivePath;
    if(!DirectoryExists(cDocPath))
      CreateDirectory(cDocPath,NULL);

  }
  if(SelectDirectory(sInPath,TSelectDirOpts(),0));
}
//---------------------------------------------------------------------------

void __fastcall TMain::Out1Click(TObject *Sender)
{
  char cDocPath[MAX_PATH];
  if(sOutPath=="")
  {
    SHGetFolderPath(NULL, CSIDL_PERSONAL,NULL,NULL,cDocPath);
    strcat(cDocPath,"\\DynoTractorTest\\Out");
    sOutPath=(String)cDocPath;//cArchivePath;
    if(!DirectoryExists(cDocPath))
      CreateDirectory(cDocPath,NULL);
    sOutPath=(String)cDocPath;
  }
  if(SelectDirectory(sOutPath,TSelectDirOpts(),0));
}
//---------------------------------------------------------------------------

void __fastcall TMain::btnCanClick(TObject *Sender)
{
  //Tipo_test=TEST_COST_TRQ;
  frmCan->Show();
}
//---------------------------------------------------------------------------




void __fastcall TMain::mnuDebugModeClick(TObject *Sender)
{
  if(g_brakeControl.GetDebugMode())
  {
     if(MessageDlg("Stop DEBUG mode?",mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
     {
       if(g_brakeControl.CmdDebug(false))
        frmDebugMode->Visible=false;
     }
  }
  else
    if(MessageDlg("DEBUG mode activation, confirm?",mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
    {
      if(g_brakeControl.CmdDebug(true))
      {
          frmDebugMode->Visible=true;

      }
    }
}
//---------------------------------------------------------------------------

