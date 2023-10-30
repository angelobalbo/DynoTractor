
#ifndef _STRUCTS
#define _STRUCTS

/// Dimensioni vettori
#define DIM_ACQ_SERIALE   25
#define DIM_PARAMS    150
#define DIM_ACQ 7500
#define DIM_MEDIA 500
#define DIM_DATA_BYTE 200000

#define MAX_BYTE  255
#define MAX_WORD  4095

#define RPM2KMH           0.000188495 /**Fattore conversione rpm->Km/h=PI*60/10^6*/
#define KMH2MPH           0.62 /**Fattore conversione Km/h->Miglia/h*/
#define KMH2MS            0.277 /**Fattore conversione Km/h->m/s*/
#define MIN2USEC          60000000 /**<costante di conversione G/min -> G/us */
#define KM2MM             1000000/**<Costante di conversione Km to mm */

/// Definizioni parametri sonda lambda
#ifndef N_VAL_LAMBDA
#define N_VAL_LAMBDA 255
#define SOFT_TEST   0
#define LAMBDA_1V   1
#define AFR_1V      2
#define LAMBDA_5V   3
#define AFR_DIMTECH 4
#define AFR_USER    5

#define LAMBDA_USER 6
#define LAMBDA_V    7
#endif
/// costanti di filtraggio
#define F_ANTISPIKE_GIRI  30 /**< Dimens. filtro anti spike su giri rullo*/
#define F_ANTISPIKE_CELLA 200 /**< Dimens. filtro anti spike su Cella*/
#define F_INERZIALE 	  30 /**< Dimens. filtro inerziale*/

/// Definizione tipo test
#define TEST_ACCEL_RPM    0
#define TEST_ACCEL_VEL    1
#define TEST_TACHO_CONST  2
#define TEST_STEP_RPM     3

/// Definizioni tipo test per il trattore
#define TEST_STEP_TIMED 0
#define TEST_STEP       1
#define TEST_DECEL      2
#define TEST_COST_TRQ   4
#define TEST_BRAKE_MAN  5

#define TAB_STEP_TIMED  0
#define TAB_STEP        1
#define TAB_BRAKE       2

/// Tipo banco trattore
#define TRACTOR 0
#define TRACTOR_TWIN 1
#define TRAILER 2
/// Tipo rapporto per il trattore
#define RAP_RPMFIX 0
#define RAP_MANUAL 2

/** definizioni per salvataggio su file*/
#define FILE_SAVE_ACQ     1
#define FILE_READ_SERIAL  2
#define FILE_READ_ACQ     3/**<Riprocessamento dati acquisiti*/
#define FILE_SAVE_MISURE  5

/** Definizione dei livelli di diagnosi attivabili */
#define DIAGNOSTIC  1
#define DIAG_NO     0
#define DIAG_LEVEL1 1
#define DIAG_LEVEL2 2

/** Definisce label per identificare diverse tipologie di file parametri*/
#define SETS_TYPE_IMPO_DMT  0 /**< file parametri tipo file_impo.dmt*/
#define SETS_TYPE_FILTERS   1
#define SETS_TYPE_TARA_DMT  2
#define SETS_TYPE_INI       3
#define SETS_TYPE_LAMBDA    4
#define SETS_TYPE_ALL       5 /**< file parametri completo: banco+taratura+prova*/


/** Tipo norma sonda lambda */
#define LAMBDA_MANUAL 0

/** Valori di default filtraggio */
#define DEF_RPM_SPIKE_FILT   100.0
#define DEF_CEL_SPIKE_FILT   100.0
#define DEF_CEL_BENZA_FILT   0.0
#define DEF_RPM_BENZA_FILT   0.0
#define DEF_CEL_SAMPL_FILT   25.0
#define DEF_TRQ_INERZ_FILT   30.0
#define DEF_TRQ_POINT_FILT   50.0

#define ACQ_SAMPLES     8000
#define DIM_SERIAL_DATA 25
#define MAP_HEADER      0
#define MAX_DIGIT       3

/**Tipi di file in elaborazione*/
#define FILE_TYPE_NO       0
#define FILE_TYPE_SER      1
#define FILE_TYPE_ACQ      2

/**\struct CELL_CALIB_PARAMS
 * Contiene dati di taratura delle celle di carico (ex TARA_CELL)
*/
struct CELL_CALIB_PARAMS
{
  float dataCalibA;/**<Data ultima taratura cella Anteriore*/
  float pesoCampioneA;/**<Valore in Kg del peso campione passato da DATI_VARI[19]*/
  float null2;/**< Non utilizzato*/
  float readFase1A;/**<valore lettura Anteriore fase 1 (solo barra)*/
  float readFase2A;/**<valore lettura Anteriore fase 2 (barra + peso noto)*/
  float readFase3A;/**<valore lettura Anteriore fase 3 (senza nulla Tara)*/
  float deltaFase21A;/**<Delta tra lettura fase 2 e fase 1 cella Anteriore*/
  float offsetA;/**<Offset di azzeramento valore a riposo cella Anteriore*/
  float braccioA; /**<valore costante del braccio Barra di taratura passato da DATI_VARI[18]*/
  float taraManuA;/**< 0-> usa caratt. elett. della cella 1-> taratura manuale*/
  float null10;/**< Non utilizzato*/
  float dataCalibP;/**<Data ultima taratura cella Posteriore*/
  float pesoCampioneP;/**<Valore in Kg del peso campione passato da DATI_VARI[19]*/
  float null13;/**< Non utilizzato*/
  float readFase1P;/**<valore lettura Posteriore fase 1 (solo barra)*/
  float readFase2P;/**<valore lettura Posteriore fase 2 (barra + peso noto)*/
  float readFase3P;/**<valore lettura Posteriore fase 3 (senza nulla Tara)*///16
  float deltaFase21P;/**<Delta tra lettura fase 2 e fase 1 cella Posteriore*/
  float offsetP;/**<Offset di azzeramento valore a riposo cella Posteriore*/
  float braccioP; /**<valore costante del braccio Barra di taratura passato da DATI_VARI[18]*/
  float taraManuP;/**< 0-> usa caratt. elett. della cella   1-> taratura manuale*/
  float null21_49[29];/**< Non utilizzati*/
};

/** \struct SETTINGS
 *  contiene le impostazioni generali del banco (DATI_VARI[0..24])
*/
struct BENCH_SETS
{
  float null0;
  float revFile;
  float diamRullo;/**<[mm] Diametro rullo*/
  float inerziaAnte;/**<[Kg/m^2] Inerzia della bancata anteriore*/
  float inerziaPost;/**<[Kg/m^2] Inerzia della bancata posteriore*/
  float inerziaFrenoAnte;/**< Kg/m^2 */
  float inerziaFrenoPost;/**< Kg/m^2 */
  float braccioReazCellaAnte;/**<[m] Lunghezza del braccio di reazione della cella anteriore*/
  float braccioReazCellaPost;/**<[m] Lunghezza del braccio di reazione della cella posteriore*/
  float caricoMaxCellaAnte;/**<[Kg] Carico massimmo per la cella anteriore*/
  float caricoMaxCellaPost;/**<[Kg] Carico massimmo per la cella posteriore*/
  float KmV2VCellaAnte;/**<[mV/V] K costante di taratura cella anteriore*/
  float KmV2VCellaPost;/**<[mV/V] K costante di taratura cella posteriore*/
  float VAlimCellaAnte;/**<[V] Tensione di alimentazione cella anteriore*/ 
  float VAlimCellaPost;/**<[V] Tensione di alimentazione cella posteriore*/
  float timeSerial;/**< [ms] Tempo di lettura dalla scheda su e (default 25ms)*/
  float nImpulsiEncoder;/**< Numero impulsi/giro encoder su rulli*/
  float KTermocoppia;/**<Coefficiente di taratura termocoppia di tipo K 0-1000 °C*/
  float KbarraTar;/**<Costante della barra di taratura*/
  float pesoCampione;/**<[Kg] Peso campione*/
  float inerziaVolanoAnte;/**< Inerzia del volano anteriore*/
  float inerziaVolanoPost;/**< Inerzia del volano posteriore*/
  float inerziaTotale;
  // 23
  float MGT300;
  float OBDII;
};

/** \struct FILTERS
 *  contiene tutte le costanti di filtraggio impostate (DATI_VARI[25..49])
*/
struct FILTERS
{
  float KCInerIstant;/**< Costante di filtraggio x real time: applicata su coppia inerz. e potenza*/
  float KSatGiriRullo;/**<Coefficiente di saturazione filtro giri rullo*/
  float KLambda;              
  float KMap;
  float KTempAtm;
  float KTempOil; //30
  float KPressAtm;
  float KUmidRel;
  float nCampMediatiCella;
  float KAdFut;
  float KAccel;       //35
  float KSpikeRpm;
  float KSampleRpm;
  float KBenzaRpm;
  float KSpikeCella;
  float KSampleCella;   //40
  float KBenzaCell;
  float KInerzCella;
  float KPunti;
  float inerziaIntegrata;//44
  float null45_49[5];    //45
};

/** \struct THRESHOLDS
 * contiene i valori di soglia generalmente utilizzati nei calcoli del banco(DATI_VARI[50..74]).
*/
struct THRESHOLDS
{
  float minRpm;/**<[rpm] Regime minimo per il calcolo dei grafici*/
  float DeltaInterpRpm;/**<Intervallo di interpolazione grafica*/
  float samples;/**<Numero dei campioni da mediare per il calcolo delle misure accessorie*/
  float KCalcPrestRpm;/**<Costante di calcolo prestazioni giri motore*/
  float maxRpm;/**<[rpm] Regime massimo per il calcolo dei grafici*/
  float nSamplesMedia;     /**<Numero di campioni medio di calcolo Lam/K/Map*/
  float IntervCalcPrestRpm;/**<Intervallo di calcolo prestazioni G/m (Rullo x passiva)*/
  float nSamplesCalcAccel; /**<Numero di letture valide calcolo accelerazione veicolo m/s^2*/
  float KIncPrestAsseAnte; /**<Costante d'incremento prestazioni asse Anteriore (test 4WD) (Cv)*/
  float null59_80[22];
};

/** \struct COUNTERS
 *  contiene i contatori di test laciati e terminati (DATI_VARI[81..99]).
*/
struct COUNTERS
{
  // 81
  float nLanciAvv;/**<Contatore numero lanci avviati Totali */
  float nLanciTerm;/**<Contatore numero lanci terminati Totali*/
  float nLanciAvvPar;  /**<Contatore numero lanci avviati parziali (azzerabile)*/
  float nLanciTermPar;/**<Contatore numero lanci terminati parziali (azzerabile)*/
  float dataResetLanci;/**<Data azzeramento contatore lanci parziale*/
  float nCountMinTotRpmRulli;/**<Contatore minuti totali di rotazione rulli*/
  float nCountMinParRpmRulli;/**<Contatore minuti parziali di rotazione rulli (azzerabile)*/
  float dataResetCountMinPar;/**<Data azzeramento contatore minuti di rotazione rulli parziale*/
  // 89
  float dataInstallL; // Data di installazione della app Dyno
  float dataInstallH;
  float dataOggiL;
  float dataOggiH;
  float scadenza;// N giorni di utilizzo della app. -1=app sbloccata,
  float pwdL;
  float pwdH;
  float null96_99[4];
};

/** \struct TEST_SETS
 *  Parametri della prova (DAT)
 */
struct TEST_SETS
{
  float rappAnt;     /**<Rap_tot1 (rapporto di trasmissione totale asse anteriore)*/
  float rappPos;     /**<Rap_tot2 (rapporto di trasmissione totale asse posteriore)*/
  float rapp4wd;     /**<vuoto*/
  float null3;       /**<vuoto*/
  float tipoTraz;     /**<Tipo_traz (tipo di trazione asse in uso vedi sopra)*/
  float tempAtm;     /**<Val_t (temperatura atmosferica di esecuzione del test °C)*/
  float pressAtm;    /**<Val_pat (pressione atmosferica di esecuzione del test mbar)*/
  float umidRel;     /**<Val_Hr  (umidità relativa di esecuzione del test %)*/
  float fFact;       /**<fFact (fattore di correzione prestazioni calcolato)*/
  float tipoBanco;   /**<Tipo_banco (tipo di banco utilizzato)*/
  float tipoTest;    /**<Tipo_test (tipo di test utilizzato per eseguire la prova)*/
  float tipoCiclo;   /**<Tipo_ciclo (tipo di ciclo del motore)*/
  float tipoAlim;    /**<Tipo_Alim (alimentazione B/D)*/
  float tipoNorm;    /**<Tipo_norm (tipo di normativa utilizzata*/
  float tipoRappCalc;/**<tipoRapp (tipo di sistema scelto per calcolo rapporto)*/
  float regRappCalc; /**<Reg_calc_rap (regime scelto per calcolo rapporto)*/
  float marciaTest;  /**<Marcia_test (marcia di prova)*/
  float tipoCambio;  /**<Tipo_cambio (tipo di cambio)*/
  float sDRuotaA;    /**<sD_ruota_A (contiene il diametro di rotolamento della ruota)*/
  float sL;          /**<sL (larghezza impronta a terra)*/
  float sH;          /**<sH (spalla)*/ //20
  float sR;          /**<sR (raggio di rotolamento)*/
#ifdef _DYNOTRACTOR
  float ptoConn;
  float rendPto;
#else
  float assiCollegati;/**<Collegamento assi*/
  float passivaCalc;
#endif  
  float visAirFuel;
  float unitCop;
  float unitPot;
  float unitTem;
  float unitVel;
  float unitMap;
  float velIniTac;//30
  float stepVelTac;
  float Lingua;
  float regIniAcc;
  float regFineAcc;
  float tempoAcc;
  float regIniDec;
  float regFineDec;
  float tempoDec;
  float velIni;
  float velFine;//40
  float regMin;//41
  float regMax;
  float deltaReg;
  float regStop;
  float tempoStaz;//45
  float coeffFm;
#ifdef _DYNOTRUCK
  float asse2Inerziale;//47
  float null48_53[6];
#else
  float null47_53[7];
#endif
  float diamRulli;
  float densCerchio;
  float densGomma;    
  float null57_72[16];
//#else  
//  float null55_72[18];
//#endif
  float rpmMaxCMotore;// rpmCoppiaMax
  float calcInerziaRuota;
  float null75_86[12];
  float inerTempoAcc;
  float inerTempoAccTot;
  float inerMaxRegAnte;
  float inerMaxRegPost;//90
  float maxPMotore;
  float maxCMotore;
  float maxPRuota;//93
  float maxCRuota;//94
  float maxPDissip;//95
  float rpmMaxPMotore;//96
  float maxManifol;
  float maxTermoK;
  float maxTempOil;
  float maxLambda;//100
  float maxLambda2;
  float IntervCalcPrestRpm;// [56] = Intervallo di calcolo prestazioni G/m rullo x passiva
#ifdef _DYNOTRACTOR  
  float verSw;
  float selPto;// 104
  float posMaxP;
#else
  float null103;
  float fCx;
  float fS;
#endif
  float testInerz;
  float rpmMaxCRuota;
  float rpmMaxPRuota;
  float K_R;// Offset calcolo pressione (default 3000 mbar)
#ifdef _DYNOTRACTOR
  float consMaxP; // 110
  float consMaxC;
  float coppiaMaxPow;  
  float nPunti;
  float dataL;
  float dataH;
  float nLancio;
  float nMotRap;
  float nPtoRap;
  float null119_149[31];
#else  
  float dataL;
  float dataH;
  float nLancio;
  float nPunti;
  float null114_149[36];
#endif  
}; // len 150*sizeof (float)


struct DYNO_SETS
{
  struct BENCH_SETS bench;
  struct FILTERS    filt;
  struct THRESHOLDS thres;
  struct COUNTERS   count;
};

/** \struct DYNO_SETTINGS
 *  tutti i parametri del banco:
 * filtraggi, soglie, contatori, calibrazione celle e dati della prova.
 */
struct DYNO_SETTINGS
{
  struct BENCH_SETS bench;
  struct FILTERS    filt;
  struct THRESHOLDS thres;
  struct COUNTERS   count;
  struct CELL_CALIB_PARAMS cell;
  struct TEST_SETS test;
};

struct LAMBDA_PARAMS
{
  float  TAB1[256];
  float  TAB2[256];
  float  TAB3[256];
  float  TAB4[256];
  float  TAB5[256];
  float  TAB6[256];
  float  TAB7[256];
};

/** Tutte le misure istantanee acquisite dal campo */
struct DATA_ACQ
{
  double dTime;/**<Ticks [ms] della stringa appena acquisita*/
  double T_K;/**<Temperatura termocoppia K*/
  double L_d;/**<dato 1 Sonda Lambda*/
  double L_d2;/**<dato 2 Sonda Lambda*/
  double P_MAP;/**<Pressione aspirazione*/
  double T_Oil;/**<temperatura olio Ntc1*/
  double dT_Ext;/**<temperatura esterna*/
  double dP_Atm;/**<Pressione atmosferica*/
  double dDistance;/**<Distanza percorsa in Km, in intervallo di 25ms*/
  double rpm[2];/**<Giri rullo: indice ant=0/post=1*/
  double fCella[2];/**<carico cella: indice ant=0/post=1*/
  double proximity;/**<Sensore di prossimità per rilevamento chiusura assi*/
  double consumi;
};

/**\struct MISURE
 * Tutti i dati acquisiti dal banco:
 * Misure, per ogni asse: giri rullo, carico cella
 */
struct MISURE
{
  double T_K[DIM_ACQ];/**<Temperatura termocoppia K*/
  double L_d[DIM_ACQ];/**<dato 1 Sonda Lambda*/
  double L_d2[DIM_ACQ];/**<dato 2 Sonda Lambda*/
  double P_MAP[DIM_ACQ];/**<Pressione aspirazione*/
  double T_Oil[DIM_ACQ];/**<temperatura olio Ntc1*/
  double dT_Ext[DIM_ACQ];/**<temperatura esterna*/
  double dP_Atm[DIM_ACQ];/**<Pressione atmosferica*/
  double dDistance[DIM_ACQ];/**<Distanza percorsa in Km, in intervallo di 25ms*/
  double rpm[2][DIM_ACQ];/**<Giri rullo: indice ant=0/post=1*/
  double fCella[2][DIM_ACQ];/**<carico cella: indice ant=0/post=1*/
  double deltaT[DIM_ACQ]; /**< Intervallo di tempo fra i campioni [sec] */
};

  typedef struct
  {
    float step;
    int rpm;
    float vel;
    float torque;
    float power;
    float lambda1;
    float lambda2;
    float termoK;
    float tempOil;
    float presMan;
    float cons;
    float engLoad;
    float throttle;
  } COLUMNSTEP;
  typedef struct
  {
    int  iTestType;
    bool bRpm;
    bool bVel;
    bool bTorque;
    bool bPower;
    bool bLambda1;
    bool bLambda2;
    bool bTermoK;
    bool bTempOil;
    bool bPresMan;
    bool bCons;
    bool bEngLoad;
    bool bThrottle;
  } HEADERSTEP;

struct UM_LIMIT
  {
    float fMaxPower;  // Cv
    float fMaxTorque; // Kgm
    float fMaxVel;    // Km/h
    float fYellowVel;    // Km/h
    float fRedVel;    // Km/h
    float fMaxCons;   // l/h
    float fMaxRpm;
    float fYellowRpm;
    float fRedRpm;    
    float fMinRpm;
    float fMaxPress;  // mbar
    float fGreenPress;
    float fYellowPress;
    float fRedPress;
    float fMinPress;
    float fMaxTermoK; // °C
    float fGreenTermoK; // °C
    float fYellowTermoK; // °C
    float fRedTermoK; // °C    
    float fMaxTempOil;// °C
    float fMinTempOil;// °C
    float fGreenTempOil;// °C
    float fYellowTempOil;// °C
    float fRedTempOil;// °C
  };


#endif