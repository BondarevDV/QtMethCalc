#ifndef H_STRUCT1
#define H_STRUCT1
#include <QString>
#include <QMap>




#define  TNAMERU        QString
#define  TNAME          QString
#define  TIDNAME        int
#define  TIDTYPE        int
#define  TVALUE         double
#define  TIDTYPE        int
#define  TKEY           int
#define  TID            int
#define  TDEVICETYPE    QString



#define  ID          "Name"
#define  GEN         "gen"
#define  MOV         "mov"
#define  TRANSF2     "trans2"
#define  TRANSF3     "trans3"
#define  DEVICETYPE  "Class"
#define  DEVICENAME  "Name"
#define  PARAMTYPE   "Name"
#define  PARAMNAMERU "Caption"
#define  PARAMVALUE  "Value"
#define  PARAMETRS   "Properties"
#define  INDEXROW    int
#define  SCHEME     "Objects"


typedef QMap<TNAME, TNAMERU> t_rus_name;
typedef QMap<TNAME, TVALUE>  t_value;
typedef QMap<TID , t_value>  t_data_list_dev;
typedef QMap<INDEXROW , TNAME>  t_data_table;
typedef QList<double> t_table;

typedef QMap<TNAME,TID> t_dev;


#define G_SNGMASS      "genRatedFullPower"
#define G_INGMASS      "genRatedAmperage"
#define G_UNGMASS      "genRatedVoltage"
#define G_FMASS        "genRatedFrec"
#define G_RGMASS       "genResStatorR"
#define G_X2DMASS      "genResX2"
#define G_X1DMASS      "genResX1"
#define G_XDMASS       "genResSyncXd"
#define G_T2DMASS      "genConstTd"
#define G_TFMASS       "genConstTimeTf"
#define G_IUSTGMASS    "genStateAmperageKZ"

#define G_RVNESHMASS   "genResOutR"
#define G_XVNESHMASS   "genResOutX"


#define   M_PADNOM       "movRatedFullPower"
#define   M_IADNOM       "movRatedAmperage"
#define   M_UADNOM       "movRatedVoltage"
#define   M_RAD          "movResStatorR"
#define   M_X1S          "movResStatorX1"
#define   M_XS           "movResStatorX"
#define   M_MU           "movCoefM"
#define   M_TR           "movConstTimeT"
#define   M_T1R          "movConstTimeT1"
#define   M_RVNESHMASS   "movResOutR"
#define   M_XVNESHMASS   "movResOutX"
#define   M_COSfi        "movCosFi"




         #define TR2_UKTR      "trans2HigherVoltageU"       //"напряжение опыта короткого замыкания обмотки  трансформатора ВН, %",
         #define TR2_U2TR      "trans2Voltage2WindS"        //"Напряжение на вторичной обмотке, кВа",
         #define TR2_SNOMTR    "trans2RatedFullPowerS"      //"полная номинальная мощность трансформатора, кВа",
         #define TR2_UNOMTR    "Trans2RatedVoltageU"        //"Номинальное напряжение, кВ",
         #define TR2_DELTAPTR  "Trans2LostPowerP"           //"Паспортные данные трансформатора  - потеря мощности в обмотке, кВа",

        #define TR3_UKTRBH  "Trans3HigherVoltageHL"     //"(В-Н)*, напряжение опыта короткого замыкания обмотки  трансформатора ВН, %",
        #define TR3_UKTRBC   "Trans3MiddleVoltageHM"     //"(В-С)*,  напряжение опыта короткого замыкания трансформатора СН, %",
        #define TR3_UKTRCH  "Trans3LowVoltageML"        //"(С-Н)*, напряжение опыта короткого замыкания трансформатора НН, %",
        #define TR3_DELTAPTRBH  "Trans3LostPowerWindHL" //"(В-Н), Паспортные данные трансформатора  - потеря мощности в обмотке, кВа",
        #define TR3_DELTAPTRBC  "Trans3LostPowerWindHM" //"(В-C), Паспортные данные трансформатора  - потеря мощности в обмотке, кВа",
        #define TR3_DELTAPTRCH  "Trans3LostPowerWindML" //"(C-Н), Паспортные данные трансформатора  - потеря мощности в обмотке",
        #define TR3_UNOMTRB  "Trans3VoltageH"           //"Напряжение (В)",
        #define TR3_UNOMTRC  "Trans3VoltageM"           //"Напряжение (С)",
        #define TR3_UNOMTRH  "Trans3VoltageL"           //"Напряжение (Н)",
        #define TR3_SNOMTR3W  "Trans3RatedFullPowerS"   //"полная номинальная мощность трансформатора, кВа",




enum TYPEDEV
{
    IDTRANS2,
    IDTRANS3,
    IDGEN,
    IDMOV,
    IDKZ,
    IDBUS
};

enum TYPEKZ
{
    KZR = 0,
    KZTRANS31 = 2,
    KZTRANS32 = 3,
    KZTRANS2  = 1
};


#endif // H_STRUCT1

