#ifndef CCALCULATION_H
#define CCALCULATION_H

#include <QObject>
#include <QDebug>
#include <math.h>
#include <QMessageBox>
class CCalculation : public QObject
{
    Q_OBJECT
private:
    int current_item;
    int count_gen = 1;
    int count_mov = 1;
    int count_tr2 = 1;
    int count_tr3 = 1;
    double*  Sngmass; // Заполнили массив Номинальных мощностей Генераторов.
    double*  Ingmass; // Заполнили массив Номинальных токов Генераторов.
    double*  Unmass;  // Заполнили массив Номинальных напряжений Генераторов.
    double*  fmass;   // Заполнили массив Номинальной частоты Генераторов.  (для общности)
    double*  rgmass;  //

    double*  x2dmass; //
    double*  x1dmass; // Заполнили массив Переходных сопротивлений Генераторов
    double*  xdmass; // Заполнили массив Синхронного индуктивного сопротивления по продольной оси Генератора.
    double*  T2dmass;// Заполнили массив Сверхпереходная постоянная времени по продольной оси Генератора.
    double*  Tfmass; // Заполнили массив Постоянная времени обмотки возбуждения при разомкнутой обмотке статора Генератора.
    double*  Iustgmass;   // Заполнили массив Установившееся значение тока КЗ на зажимах Генератора в О. Е.
    double*  Rvneshmass;// Заполнили массив внешнего активного сопротивления Генератораторной ветви
    double*  Xvneshmass;// Заполнили массив внешнего активного сопротивления Генератораторной ветви


    int current_num_mov = 0;


     double* PADnom;  // Номинальная мощность Асинхронного Двигателя.
     double* IADnom ; // Номинальный ток Асинхронного Двигателя.
     double* rAD;     // Активное сопртивление статора Асинхронного Двигателя.
     double* x1s;     // Переходное индуктивное сопротивление статора Асинхроннонго Двигателя.
     double* xs ;     // Индуктивное сопротивление статора Асинхроннонго Двигателя.
     double* mu ;     // Коэффициент магнитной связи ротора и статора Асинхроннонго Двигателя.
     double* Tr ;       // Постоянная времени обмотки ротора Асинхроннонго Двигателя.
     double* T1r;// Переходная Постоянная времени обмотки ротора Асинхроннонго Двигателя.



     double* PADnommass;
     double* UADnommass;
     double* IADnommass;
     double* rADmass;
     double* x1smass;
     double* xsmass;
     double* mumass;
     double* Trmass;
     double* T1rmass;
     double* CosFimass;
     double* Rkmass;
     double* Xkmass;

     double Iudarnyjg;
     double Ta;
     double FlagTR1 = 0;
     double XTR1, RTR1;    //  базисные первой ступени (генераторной)
     double Ub;
     double Ib;            // После можно принять через суммарную базисную мощность. Переделать???!!!
     double Sb;            // После можно принять через суммарную базисную мощность. Переделать???!!!
     double Zb;            // [Ом]


      double* uktrBC;
      double* uktrBH;
      double* uktrCH;
      double* UnomtrB;
      double* UnomtrC;
      double* UnomtrH;
      double* Snomtr3w;
      double* deltaPTrBC;
      double* deltaPTrBH;
      double* deltaPTrCH;

      double*  uktr;    //Это Uк% трансформатора
      double*  unomtr;  //Это Uном траансформатора отличается от Ubas
      double*  U2tr;    //Это паспортное номинальное напряжение обмотки второй ступени трансформатора.
      double*  Snomtr;
      double*  deltaPTr;


    QTextStream* text_result;
    QString teg_next_string = "</br>";



public:
    QString allotInTeg(QString open_teg, QString str );
    explicit CCalculation(QObject *parent = 0);
    QString getResult();
    QString buffer;
    void  Calc();

    void CalcMOV();
    void CalcTr2();
    void CalcTr3();
    void SetModeWorkFlagTR1(int modeWork);
    void  ClearAll();
    void  Init(int num_gen);

    void setNumGen(int num_gen);
    void setNumMOV(int num_mov);

    void  DelItem(int index);

    double xtr = 0;
    double xtroeb1; // в о.е.
    double xtroeb2; // в о.е.
    double xtroeb;//
    double xtroes;//
    double ktr12;       //
    double ktr12w3;    // коэффициенты трансформации
    double ktr13w3;   // коэффициенты трансформации


    void setPADnommass(double value, int index);
    void setIADnommass(double value, int index);
    void setUADnommass(double value, int index);
    void setrADmass(double value, int index);
    void setx1smass(double value, int index);
    void setxsmass(double value, int index);
    void setmumass(double value, int index);
    void setTrmass(double value, int index);
    void setT1rmass(double value, int index);
    void setCosFimass(double value, int index);
    void setRkmass(double value, int index);
    void setXkmass(double value, int index);

    void  setSngmass(double value, int index);   // Заполнили массив Номинальных мощностей Генераторов.
    void  setIngmass(double value, int index);   // Заполнили массив Номинальных токов Генераторов.
    void  setUnmass(double value, int index);    // Заполнили массив Номинальных напряжений Генераторов.
    void  setfmass(double value, int index);     // Заполнили массив Номинальной частоты Генераторов.  (для общности)
    void  setrgmass(double value, int index);    //
    void  setx2dmass(double value, int index);   //
    void  setx1dmass(double value, int index);   // Заполнили массив Переходных сопротивлений Генераторов
    void  setxdmass(double value, int index);    // Заполнили массив Синхронного индуктивного сопротивления по продольной оси Генератора.
    void  setT2dmass(double value, int index);   // Заполнили массив Сверхпереходная постоянная времени по продольной оси Генератора.
    void  setTfmass(double value, int index);    // Заполнили массив Постоянная времени обмотки возбуждения при разомкнутой обмотке статора Генератора.
    void  setIustgmass(double value, int index); // Заполнили массив Установившееся значение тока КЗ на зажимах Генератора в О. Е.
    void  setRvneshmass(double value, int index);// Заполнили массив внешнего активного сопротивления Генератораторной ветви
    void  setXvneshmass(double value, int index);// Заполнили массив внешнего активного сопротивления Генератораторной ветви


    void setuktrBC(double value, int index);
    void setuktrBH(double value, int index);
    void setuktrCH(double value, int index);
    void setUnomtrB(double value, int index);
    void setUnomtrC(double value, int index);
    void setUnomtrH(double value, int index);
    void setSnomtr3w(double value, int index);
    void setdeltaPTrBC(double value, int index);
    void setdeltaPTrBH(double value, int index);
    void setdeltaPTrCH(double value, int index);


    void setuktr(double value, int index);     // Это Uк% трансформатора
    void setunomtr(double value, int index);   // Это Uном траансформатора отличается от Ubas
    void setU2tr(double value, int index);     // Это паспортное номинальное напряжение обмотки второй ступени трансформатора.
    void setSnomtr(double value, int index);   // StrToFloat(Edit3.Text);
    void setdeltaPTr(double value, int index); // StrToFloat(Edit4.Text);
signals:

public slots:
};

#endif // CCALCULATION_H
