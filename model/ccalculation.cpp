#include "ccalculation.h"

QString CCalculation::allotInTeg(QString name_teg, QString str)
{
    return QString(" <%1>  %2  </%1>").arg(name_teg).arg(str);

}

CCalculation::CCalculation(QObject *parent) : QObject(parent)
{

}

QString CCalculation::getResult()
{
    return buffer;
}



QString creatStrForReport(QString teg, QString nameParam, QString valueParam, QString unitsParam )
{
    return QString(" <%1> %2 %3 %4 </%5>").arg(teg).arg(nameParam).arg(valueParam).arg(unitsParam).arg(teg) ;
}


void CCalculation::Calc()
{

    //double xtr = 0;
    int Ng = 0;//count_gen;

    double     Eeqv , E1eqv , E2eqv,// ;     //Эквивалентные ЭДС всякие
         I2o, Z2p, z1p,// ;     //Начальные значения сверхпереходного и переходного токов эквивалентного генератора при КЗ
         Zpust;// real;     //Установившийся ток КЗ






    double I2osum, I1osum,  T2averigesum , T1averigesum;// ;   // рабочие переменные для определения постоянной времени
    double Xeqv, Reqv, X1eqv, X2eqv;//  ;   // вычисленные эквивалентные сопротивления синхронное, переходное, сверхпереходное
    double Asum, ARsum,  A1sum, A2sum , Ysum,YRsum, Y1sum, Y2sum ;//  ;


    double Iudarnyjg;
    double I1o, Io, Iust, IoA, Io01A, Io01, Io1A, Io1, T2, T1;// ;    // Такие же  параметры локализованные в других модулях!

//     { Далее для генераторов вводятся О.Е. За базисное Ub, Ib примимается номинальное ЗДЕСЬ первого генератора Ugenmass[1], Ingmass[1]. Переделать???!!!}
     Ub= Unmass[0];
     Ib= Ingmass[0];   // После можно принять через суммарную базисную мощность. Переделать???!!!
     Sb=Ib*Ub*sqrt(3); // После можно принять через суммарную базисную мощность. Переделать???!!!
     Zb=Ub/(Ib*sqrt(3)); // [Ом]

/*
      *text_result <<QString::fromUtf8("Базисное напряжение    (Ub):")<< Ub << '\n';
      *text_result <<QString::fromUtf8("Базисная Сила тока     (Ib):")<< Ib << '\n';
      *text_result <<QString::fromUtf8("Базисная Мощность      (Sb):")<< Sb << '\n';
      *text_result <<QString::fromUtf8("Базисное сопротивление (Zb):")<< Zb << '\n';
*/



      Ng = count_gen;
      //qDebug() << "count_gen = " << Ng;
       *text_result <<QString::fromUtf8("  Число генераторов = " )<< Ng << '\n';

      double* A =  new double[Ng];
      double* AR =  new double[Ng];  // промежуточный массив
      double* Y =  new double[Ng];
      double* YR =  new double[Ng]; // промежуточный массив проводимостей
      double* E2ogen =  new double[Ng];
      double* E1ogen =  new double[Ng];// промежуточный массив рассчитанных сверхпереходных ЭДС

      double* I2ovetvi =  new double[Ng];
      double* I1ovetvi =  new double[Ng];// генераторные сверхпереходные токи
      double* rgenmass =  new  double[Ng];
      double* xgenmass =  new  double[Ng];
      double* x2genmass =  new  double[Ng];
      double* x1genmass =  new  double[Ng];

      double* RvneshmassOE =  new  double[Ng];
      double* XvneshmassOE =  new  double[Ng];

      double* Ingenoe  =  new  double[Ng];
      double* Unmassoe =  new  double[Ng];//

      double* A1 = new  double[Ng];              // x1genmass[i] ???? или только Xvneshmass[i] ?????!!!!!, а если =0 ????
      double* Y1 = new  double[Ng];

      double* A2 = new  double[Ng];              // x1genmass[i] ???? или только Xvneshmass[i] ?????!!!!!, а если =0 ????
      double* Y2 = new  double[Ng];
      for (int i = 0; i < Ng; ++i) {
          RvneshmassOE[i] = Rvneshmass[i]/Zb;
          XvneshmassOE[i] = Xvneshmass[i]/Zb;
      }

    //{Здесь запускается упрощенное эвивалентирование Генератора в пренебрежении активной составляющей внешнего сопротивления}
      for (int i = 0; i < Ng; ++i) {
                      //{ Массивы сопротивлений цепей Генераторов }
                       rgenmass[i]=rgmass[i]+RvneshmassOE[i];
                       x2genmass[i]= x2dmass[i]+XvneshmassOE[i];
                       x1genmass[i]= x1dmass[i]+XvneshmassOE[i];
                       xgenmass[i] = xdmass[i]+ XvneshmassOE[i];
       }
     //{ Приведение к базисным относительным единицам }
      //if (Ub!= 0 || Ib!=0)
      for (int i = 0; i < Ng; ++i) {

                          Unmassoe[i]= Unmass[i]/Ub;

                          Ingenoe[i] = Ingmass[i]/Ib;
                          qDebug() << "Unmass[i]" << i << Unmass[i];
      }
//      else
//          QMessageBox::critical(0,"Error","Деление на ноль");

     //{ Промежуточный массив A и Y }
      for (int i = 0; i < Ng; ++i) {

                if (xgenmass[i]!= 0 || rgenmass[i]!=0)
                {
                         A[i]=Unmassoe[i]/xgenmass[i];                       // в О.Е.
                         Y[i]=1/xgenmass[i];


                         AR[i]=Unmassoe[i]/rgenmass[i];                       // в О.Е.  для активной составляющей
                         YR[i]=1/rgenmass[i];
                }
               else{
                             QMessageBox::critical(0,"Error","Деление на ноль");
                             return;
                         }

      }
      ////////////////////////////////////////////////////////////////////////////
     Asum=0;  Ysum=0;  ARsum=0;  YRsum=0;
     for (int i = 0; i < Ng; ++i){
                         Asum=Asum+A[i];
                         Ysum=Ysum+Y[i];
                         ARsum=ARsum+AR[i];
                         YRsum=YRsum+YR[i];
      }
     /////////////////////////////////////////////////////////////////////////////
     //   {Эквивалентные сопротивления для КЗ на сборных шинах распредустройства}
      Xeqv=1/Ysum;           //Соответственно, проводимостям эквивалентные сопротивления Генераторной ветви
      Reqv=1/YRsum;
      Eeqv=Asum/Ysum;        // Эквивалентное ЭДС Генераторной ветви
      //qDebug << "Eeqv= " + Eeqv +"о.е. или"  +  Eeqv*Ub  + Xeqv+ " о.е.";
     /////////////////////////////////////////////////////////////////////////////
     //{Эквивалентные сопротивления для КЗ за Трансформатором1}


     if (FlagTR1==0) {    // Возвращаемся к начальным условиям
                         XTR1=0;
                         RTR1=0;
                         //QMessageBox::information(0, "Информация", QString::number(FlagTR1) + ": Эквивалентные сопротивления при КЗ на шинах");
      }
     if (FlagTR1==1) {
          //         {Вызов процедуры расчета параметров схемы замещения однообмоточного Трансформатора}
                     CalcTr2();
                     XTR1=xtroeb ;                              // должны получить из Unit3  в О. Е.  !
                     RTR1=10000;                                //ЗАГЛУШКА
          //    Label17.Caption='xtroeb из Unit_Trans2w = '+FloatToStrF(xtroeb,ffGeneral,6,4 )+' О.Е.';
                     Xeqv=Xeqv + XTR1;           //Эквивалентные сопротивления при КЗ за Трансформатором1
                     Reqv=Reqv + RTR1;           //Эквивалентные сопротивления при КЗ за Трансформатором1

                      // QMessageBox::information(0, "Информация", QString::number(FlagTR1) + ": Эквивалентные сопротивления при КЗ 2-х оботочного Трансформатора");
     }
    //-----------------------------------------------------------------------------
    if (FlagTR1==2) {
                  // {Вызов процедуры расчета параметров схемы замещения трехобмоточного Трансформатора, сторона СН}
                    CalcTr3();
                     XTR1=xtroeb1;                              // должны получить из Unit3  в О. Е.  !
                     RTR1=10000;                                //ЗАГЛУШКА
         //     Label17.Caption='xtroeb1 из Unit_Trans3w СН= '+FloatToStrF(xtroeb1,ffGeneral,6,4 )+' О.Е.';
                     Xeqv=Xeqv + XTR1;           //Эквивалентные сопротивления при КЗ за Трансформатором1
                     Reqv=Reqv + RTR1;           //Эквивалентные сопротивления при КЗ за Трансформатором1

                     //  QMessageBox::information(0, "Информация", QString::number(FlagTR1) + ": Эквивалентные сопротивления при КЗ за 3-х оботочным Трансформатором, сторона СН");
     }
    //-----------------------------------------------------------------------------
      if (FlagTR1==3) {
             //      {Вызов процедуры расчета параметров схемы замещения однообмоточного Трансформатора}
                     CalcTr3();
                     XTR1=xtroeb2;                              // должны получить из Unit3  в О. Е.  !
                     RTR1=10000;                                //ЗАГЛУШКА
             // Label17.Caption='xtroeb1 из Unit_Trans3w СН= '+FloatToStrF(xtroeb1,ffGeneral,6,4 )+' О.Е.';
                     Xeqv=Xeqv + XTR1;           //Эквивалентные сопротивления при КЗ за Трансформатором1
                     Reqv=Reqv + RTR1;           //Эквивалентные сопротивления при КЗ за Трансформатором1

                      // QMessageBox::information(0, "Информация", QString::number(FlagTR1) + ": Эквивалентные сопротивления при  КЗ за 3-х Трансформатором, сторона НН");
       }


     //{ Расчет эквивалентной переходной ЭДС }
     /////////////////////////////////////////////////////////////////////////////
     //   Сверхпереходная и переходная ЭДС генератора
     // Fi=arccos(Fi);  //Вообще говоря, определяется предшествующим до КЗ режимом работы сети.
     double cosFivar = 0.8; // Из таблиц нагрузок cos(Fi)=0.8
     // E2o=1+1*x2d*sin(ArcСos(Fi));
     for (int i = 0; i < Ng; ++i){
                        //     {Ниже использованы Uo (Unmass[i]), приведенные к базису и номинальные токи генераторов принятые за 1 О.Е. !!!}
                        E2ogen[i] = Unmassoe[i] + Ingenoe[i] *x2dmass[i]* sin(acos(cosFivar));                // после заменить на выражене с радикалом !
                        E1ogen[i] = Unmassoe[i] + Ingenoe[i] *x1dmass[i]* sin(acos(cosFivar));                // после заменить на выражене с радикалом !
                        I2ovetvi[i]  = E2ogen[i]/ sqrt(pow(x2genmass[i],2)+pow(rgenmass[i],2));                     // будет использован для сверхпереходной постоянной времени системы
                        I1ovetvi[i]  = E1ogen[i]/ sqrt(pow(x1genmass[i],2)+pow(rgenmass[i],2));                     // будет использован для переходной постоянной времени системы
    }
    // --------------------------------------------------------------------------
   //{ Промежуточный массив A1 и Y1 }
      for (int i = 0; i < Ng; ++i) {

                             A1[i]=E1ogen[i]/x1genmass[i];                  // x1genmass[i] ???? или только Xvneshmass[i] ?????!!!!!, а если =0 ????
                             Y1[i]=1/x1genmass[i];

      }
      ////////////////////////////////////////////////////////////////////////////
     A1sum=0;  Y1sum=0;
     for (int i = 0; i < Ng; ++i) {
                         A1sum=A1sum+A1[i];
                         Y1sum=Y1sum+Y1[i];
     }
     /////////////////////////////////////////////////////////////////////////////
      X1eqv=1/Y1sum;           //Соответственно, проводимости эквивалентные сопротивлениям Генераторной ветви
      E1eqv=A1sum/Y1sum;       // Эквивалентное сопротивление Генераторной ветви
      QString teg_h = "h3";
      QString s = allotInTeg( "h1" , "==========================================================================================================");
       *text_result <<s<<'\n';

     *text_result << QString(" %1 ЭДС эквивалентного генератора:  %2").arg("<h1>").arg("</h1>")   << '\n';
//     *text_result << QString(" <%1> %2 %3 %4 </%5>").arg(teg_h).arg(" E1eqv  = ").arg(QString::number(E1eqv)).arg("o.e.").arg(teg_h) ;
//     *text_result << QString(" <%1> %2 %3 %4 </%5>").arg(teg_h).arg(" E1eqv  = ").arg(QString::number( E1eqv*Ub)).arg(" кВ ").arg(teg_h);

     *text_result <<creatStrForReport(teg_h, " E1eqv  = ", QString::number(E1eqv),"o.e.");
     *text_result <<creatStrForReport(teg_h, " E1eqv  = ", QString::number(E1eqv*Ub)," кВ ");

     *text_result << QString(" %1 Сопротивление эквивалентного генератора:  %2").arg("<h1>").arg("</h1>")   << '\n';

     *text_result <<creatStrForReport(teg_h, " X1eqv  = ", QString::number(X1eqv)," o.e.");

     /////////////////////////////////////////////////////////////////////////////
     ///{ Промежуточный массив A2 и Y2 }
      for (int i = 0; i < Ng; ++i)

      {
                        // if (x2genmass[i] == 0) x2genmass[i]=0.001;     // Вывести предупреждение о коррекции нуля!!!!!!!!!!!!!
                         A2[i]=E2ogen[i]/x2genmass[i];                  // x1genmass[i] ???? или только Xvneshmass[i] ?????!!!!!, а если =0 ????
                         Y2[i]=1/x2genmass[i];
      }
      ////////////////////////////////////////////////////////////////////////////
     A2sum=0;  Y2sum=0;
     for (int i = 0; i < Ng; ++i) {
                         A2sum=A2sum+A2[i];
                         Y2sum=Y2sum+Y2[i];
     }
     /////////////////////////////////////////////////////////////////////////////
     //{Поправка сверхпереходной постоянной времени  системы}
       I2osum=0;  T2averigesum=0;
      for (int i = 0; i < Ng; ++i) {
                         I2osum=I2osum+I2ovetvi[i];
                         T2averigesum=T2averigesum+T2dmass[i]*I2ovetvi[i];

     }
     if (I2osum == 0){
         //Label1.Caption='Все генераторы отключены? I2osum= '+doubleToStr(I2osum);
         qDebug() << " Все генераторы отключены? I2osum=  " << I2osum ;
     }
     else
     {
          T2averigesum= T2averigesum/I2osum;  //Средневзвешенная постоянная времени системы
          //Label1.Caption='Средневзвешенная сверхпереходная постоянная времени системы T2= '+doubleToStr(T2averigesum)+' c';
          //qDebug() << " Средневзвешенная сверхпереходная постоянная времени системы T2 = " << T2averigesum ;
          //*text_result <<QString::fromUtf8(" Средневзвешенная сверхпереходная постоянная времени системы T2 = ") << T2averigesum  <<QString::fromUtf8(" c ") << '\n';

         // *text_result << QString(" <%1> %2 %3 %4 </%5>").arg(teg_h).arg(" Средневзвешенная сверхпереходная постоянная времени системы T2 = ").arg(QString::number(T2averigesum)).arg("o.e.").arg(teg_h)  ;
          *text_result << QString(" %1 Средневзвешенная сверхпереходная постоянная времени системы:  %2").arg("<h1>").arg("</h1>")   << '\n';
          *text_result <<creatStrForReport(teg_h, " T2 =  ", QString::number(T2averigesum)," o.e.");

     }
     /////////////////////////////////////////////////////////////////////////////
     // {Поправка переходной постоянной времени  системы}
       I1osum=0;  T1averigesum=0;
      for (int i = 0; i < Ng; ++i) {
          I1osum=I1osum+I1ovetvi[i];
          T1averigesum=T1averigesum+Tfmass[i]*I1ovetvi[i];
          qDebug()<< Tfmass[i];
      }
     if (I1osum == 0){
         //Label1.Caption='Все генераторы отключены? I1osum = '+doubleToStr(I1osum);
         qDebug() << " Все генераторы отключены? I1osum =  " << I1osum  ;

     }
     else
     {
                        T1averigesum= T1averigesum/I1osum;                             //Средневзвешенная постоянная времени системы
                        //Label8.Caption='Средневзвешенная переходная постоянная времени системы T1= '+doubleToStrF(T1averigesum, ffGeneral,8,5)+' c';
                        //qDebug() << " Средневзвешенная переходная постоянная времени системы T1 =  " << T1averigesum ;
                        //*text_result <<QString::fromUtf8(" Средневзвешенная переходная постоянная времени системы T1 =  ") << T1averigesum  << QString::fromUtf8(" c ")<<'\n';
         //*text_result << QString(" <%1> %2 </%3>").arg(teg_h).arg("Сопротивление эквивалентного генератора:").arg(teg_h)  ;
     }

     //Так как внешние активные сопротивления генераторных ветвей уже были учтены, дополнительная поправка к постоянным времени не нужна.
     /////////////////////////////////////////////////////////////////////////////
      X2eqv=1/Y2sum;           //Соответственно, проводимости эквивалентные сопротивлениям Генераторной ветви
      E2eqv=A2sum/Y2sum;       // Эквивалентное сопротивление Генераторной ветви
      //Label4.Caption='E2eqv= '+doubleToStrF(E2eqv, ffGeneral,8,5)+' о.е. или  '+doubleToStrF(E2eqv*Ub, ffGeneral,8,5)+' кВ    X2eqv= '+doubleToStrF(X2eqv, ffGeneral,7,5)+' о.е.';
      qDebug() << " E2eqv =  " << E2eqv  ;
      qDebug() << " о.е. или  " << E2eqv*Ub  ;


      *text_result << QString(" %1 ЭДС :  %2").arg("<h1>").arg("</h1>")   << '\n';
      *text_result <<creatStrForReport(teg_h, " E2eqv =  ", QString::number(E2eqv)," o.e.");
      *text_result <<creatStrForReport(teg_h, " E2eqv =  ", QString::number(E2eqv*Ub)," кВ ");

      // --------------------------------------------------------------------------
    //Label15.Caption=  'флаг ' + doubleToStr(FlagTR1);   // Добавляем Тр-р
      qDebug() << " флаг  " << FlagTR1  ;
      *text_result << allotInTeg("h2","Тип КЗ: " + QString::number(FlagTR1))<< '\n' ;//QString::fromUtf8("Тип КЗ: ") << FlagTR1  << '\n';


    // Начальные значения сверхпереходного и переходного токов эквивалентного генератора при КЗ
    //{   (см. аналогиное в UnitSimGenKZ505new)
    ///  Полное сопротивление генераторной цепи для одной ветви ///
    // Z2p = sqrt(Sqr(rg1eqv) + Sqr(x2g1eqv));
    // Z1p = sqrt(Sqr(rg1eqv) + Sqr(x1g1eqv));
    // Zpust = sqrt(Sqr(rg1eqv) + Sqr(xg1eqv));                                          // Исправил!!!
    // }
    // Допускается не учитывать активные составляющие сопротивлений генераторных ветвей
    //{ Замечание - в ЭДС вошли собственные внутренние реактансы генераторов, а в ветви еще и внешние сопротивления}
    Z2p=X2eqv;      // Напоминание- внешние сопротивления генераторной ветви учтены. Это здесь для отладки, временно!!!!!!!
    I2o = E2eqv/Z2p;
    z1p=X1eqv ;     // Напоминание- внешние сопротивления генераторной ветви учтены. Это здесь для отладки, временно !!!!!!!
    I1o = E1eqv/z1p;
    //Label5.Caption = 'I2o= ' + doubleToStrF(I2o, ffGeneral,8,5)+' о.е.'+'= '+doubleToStrF(I2o*Ib, ffGeneral,8,5)+'кА';
    //Label6.Caption = 'I1o= ' + doubleToStrF(I1o, ffGeneral,8,5)+' о.е.'+'= '+doubleToStrF(I1o*Ib, ffGeneral,8,5)+'кА';
    //qDebug() << " I2o=  " << I2o << " о.е. " << I2o*Ib;
    //qDebug() << " флаг  " << I1o << " о.е. " << I1o*Ib;

    *text_result << QString(" %1 Переодический ток :  %2").arg("<h1>").arg("</h1>")   << '\n';
    *text_result <<creatStrForReport(teg_h, " I2o =  ", QString::number(I2o)," o.e.");
    *text_result <<creatStrForReport(teg_h, " I2o =  ", QString::number(I2o*Ib)," кА ");
    *text_result <<creatStrForReport(teg_h, " I1o =  ", QString::number(I1o)," o.e.");
    *text_result <<creatStrForReport(teg_h, " I1o =  ", QString::number(I1o*Ib),"  кА ");

    // Установившийся ток КЗ    (см. аналогиное в UnitSimGenKZ505new)
    Zpust= Xeqv;  // Напоминание - Эквивалентные сопртивления здесь также включают в себя внешние сопротивления генераторных ветвей. Заменить после отладки на модуль импеданса!
    //Iust = xd*Iustg/Zpust; // Для одной ветви или средневзешенного тока из заданных
    Iust= Eeqv/Zpust;  // Условно принимаем так, полагая наличие влияния внешнего и синхронного реактивного сопротилений эквивалентного генератора.

    *text_result << QString(" %1 Установившийся ток  :  %2").arg("<h1>").arg("</h1>")   << '\n';
    *text_result <<creatStrForReport(teg_h, " Iust =  ", QString::number(Iust)," o.e. ");
    *text_result <<creatStrForReport(teg_h, " Iust =  ", QString::number(Iust*Ib)," кА ");

    //{ Это место (выше) можно уточнить введя определение Iustg через средневзвешенные заданный в исходных данных Iustgmass[i] и еще уточнив Zpust как модуль эквивалентного реактивного и активного включая внешние сопротивления }
    //Label7.Caption = 'Iust=' + doubleToStrF(Iust, ffGeneral,8,5)+' о.е.'+'= '+doubleToStrF(Iust*Ib, ffGeneral,8,5)+'кА';
   // qDebug() << " Iust=  " << Iust << "Iust*Ib " << I1o*Ib;

    //////////////////////////////////////////////////////////////////////////////
    //iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii
    // Постоянные времени
    // x1deqv = x1d; // эквивалентные параметры [с508 ОСТ...-81], возможно лишние?
    // x2deqv = x2d; // эквивалентные параметры [с508 ОСТ...-81], возможно лишние?
    // T2 = ((T2d * x1deqv) / x2deqv) * (Sqr(x2g1eqv) + Sqr(rg1eqv)) /
    // T2 = ((T2averigesum *X1eqv ) /X2eqv ) * (Sqr(X2eqv) + Sqr(Reqv)) /( X1eqv*X2eqv + Sqr(Reqv));  // Заменил Т2 на средневзвешенную  близкое по значению.
    //Label10.Caption = 'T2=' + doubleToStr(T2) + ' c';
    // T1 = Tf * (x1g1eqv * xg1eqv + Sqr(rg1eqv)) / (Sqr(xg1eqv) + Sqr(rg1eqv));
    // Label19.Caption = 'T1=' + doubleToStr(T1) + ' c';
    //Ta = x2g1eqv / (3.14159 * 2 * f * rg1eqv);
    ////////////////////////////////////////////////////////////////////////////////
    T2=T2averigesum;         // Убрать потом лишнюю переменную!!!!!
    T1=T1averigesum;         // Убрать потом лишнюю переменную!!!!!


    *text_result << QString(" %1 Постоянная времени  :  %2").arg("<h1>").arg("</h1>")   << '\n';
    *text_result <<creatStrForReport(teg_h, " T2 =  ", QString::number(T2averigesum)," ");
    *text_result <<creatStrForReport(teg_h, " T1 =  ", QString::number(T1averigesum)," ");


    //Label10.Caption = 'T2=' + doubleToStrF(T2,ffGeneral,5,4) + ' c';
    Ta =X2eqv/(3.14159 * 2 * fmass[0] * Reqv);                        //X2eqv,Reqv правомерно использовать?  После отладки взять усредненную по мощности частоту!

//    qDebug() << " fmass =  " << fmass[0];

//    qDebug() << " X2eqv =  " << X2eqv;
//    qDebug() << " Reqv =  " << Reqv;
//    *text_result <<QString::fromUtf8(" fmass = ") << fmass[0] << '\n';
//    *text_result <<QString::fromUtf8(" X2eqv = ") << X2eqv  << '\n';
//    *text_result <<QString::fromUtf8(" Reqv  = ") << Reqv  <<  '\n';

    // Действующие значения периодической составляющей тока КЗ генераторов для моментов времени 0,01с и 1с
    Io01 = ((I2o - I1o) * exp(-(0.01 / T2))) + ((I1o - Iust) * exp(-(0.01 / T1)))+ Iust; // в О.Е.
    Io01A = Io01 * Ib; // в кА   , момент времени 0,01с.
    //Label11.Caption = 'I(0.01c)=' + doubleToStrF(Io01A,ffGeneral,5,4 ) + ' кА';

//    qDebug() << " Ta =  " << QString("%1").arg(Ta, 0, 'g', 5)  << " c " ;

//    *text_result <<QString::fromUtf8(" Ta     = ") << QString("%1").arg(Ta, 0, 'g', 5) << QString::fromUtf8(" кА ") <<  '\n';
//    *text_result <<QString::fromUtf8(" Io01   = ") << QString("%1").arg(Ta, 0, 'g', 5) << QString::fromUtf8(" кА ") <<  '\n';
//    *text_result <<QString::fromUtf8(" Io01A  = ") << QString("%1").arg(Ta, 0, 'g', 5) << QString::fromUtf8(" кА ") <<  '\n';





    Io1 = ((I2o - I1o) * exp(-(1.0 / T2))) + ((I1o - Iust) * exp(-(1.0 / T1)))+Iust; // в О.Е.
    Io1A = Io1 * Ib; // в кА , момент времени  1с.
    //Label12.Caption = 'I(1c)=' + doubleToStrF(Io1A,ffGeneral,5,4 ) + ' кА';
    //qDebug() << " I(1c) =  " << Ta << " кА " << I1o*Ib;
    IoA = I2o * Ib;
    //Label13.Caption = 'Io=' + doubleToStrF(IoA,ffGeneral,5,4 ) + ' кА';
    // Ударный ток генератора
    Iudarnyjg = (Io01 + I2o * exp(-(0.01 / Ta))) * sqrt(2) * Ib;
    //qDebug() << " Io =  " << IoA << " кА " ;

    *text_result << QString(" %1 Результат  :  %2").arg("<h1>").arg("</h1>")   << '\n';
    *text_result <<creatStrForReport(teg_h, " Ta =  ", QString("%1").arg(Ta, 0, 'g', 5),"  c ");
    *text_result <<creatStrForReport(teg_h, " Io01 =  ", QString("%1").arg(Io01, 0, 'g', 5)," кА ");
    *text_result <<creatStrForReport(teg_h, " Io01A =  ",QString("%1").arg(Io01A, 0, 'g', 5)," кА ");
    *text_result <<creatStrForReport(teg_h, " Io =  ", QString("%1").arg(IoA, 0, 'g', 5)," кА ");
    *text_result <<creatStrForReport(teg_h, " Iударный =  ", QString("%1").arg(Iudarnyjg, 0, 'g', 5)," кА ");


   // qDebug() << " Iударный =  " << Iudarnyjg << " кА " ;
   // qDebug() << " Sb =  " << Sb << " МВА " << "XTr = "<< Sb;


}

void  TermoPuls(double t,
                double I2o,
                double Io1,
                double I1o,
                double Ta,
                double Ib,
                double& Bn,
                double& Ban,
                double I2oA,
                double& Bk )       // Io01 что это ,skj???
//{ Тепловой импульс тока в точке КЗ за время от начала КЗ до t оценочно }
 // var Bk, Bn,Ban;
{
  double  Io1A, I1oA;

    I2oA = I2o*Ib  ;
    Io1A = Io1*Ib  ;
    I1oA = I1o*Ib  ;

    Bn  =((pow(I2oA,2)+pow(Io1A,2))/2.0)*0.01+((pow(Io1A,2)+pow(I1oA,2))/2.0)*0.99; // От переиодической составляющей тока
    Ban = pow(I2oA,2)*Ta ;//*(1-exp(-t/0.5*Ta));   // От апериодической составляющей тока      ???????????????????????!!!!!!
    Bk  = Bn + Ban;

}


//void CCalculation::CalcMOV()
//{
//    double rgk, x1dv, x1dvk, z, xdv, Z1pdv, xdvk;
//    double Uc, iuddv, T1reqvd, T1seqv, Io1cdv, IudSigma;
////    t   = 1.5;    // для отладки
////    I2  =40.5/Ib;          // для отладки [c.509]
////    Io1 =28.1/Ib;          // для отладки [c.509]
////    I1o =11.1/Ib;          // для отладки ,контрольный результат В=463,9 kAAc.
////    TermoPuls(t,I2o, Io1,I1o , Ta,Ib,Bn, Ban,I2oA, Bk);
////    //Label25.Caption = 'Bn=' + FloatToStr(Bn) + ' I2oA=' + FloatToStr(I2oA)+ 'Ban=' + FloatToStr(Ban)+ ' Bk=' + FloatToStr(Bk);
////    *text_result <<QString::fromUtf8(" Bn   = ")  <<  Bn    << QString::fromUtf8( " о.е. " )<< '\n';
////    *text_result <<QString::fromUtf8(" I2oA = ")  <<  I2oA  << QString::fromUtf8( " о.е. " )<< '\n';
////    *text_result <<QString::fromUtf8(" Ban  = ")  <<  Ban   << QString::fromUtf8( " о.е. " )<< '\n';
////    *text_result <<QString::fromUtf8(" Bk   = ")  <<  Bk    <<  QString::fromUtf8( " о.е. " )<< '\n';
////    { Данные эквивалентной цепи с Асинхронным двигателем }
//    // см.стр. 512 ОСТ 5.6181-81
//    rgk = 0.0945;
//    // Эти данные передавать из расчета сопротивлений эквивалентной цепи.
//    x1dv = 0.1587;
//    // Эти данные передавать из расчета сопротивлений эквивалентной цепи.
//    x1dvk = 0.1744;
//    // Эти данные передавать из расчета сопротивлений эквивалентной цепи.
//    Z1pdv = 0.1984;
//    // Эти данные передавать из расчета сопротивлений эквивалентной цепи.
//    z = 0.853;
//    Uc = 0.95; // Задается < 1
//    /////////////////////////////////////////////////////////////////////////////
//    xdv = xs[current_num_mov] * z;
//    xdvk = xdv + x1dvk - x1dv;
//    T1reqvd = Tr[current_num_mov] * (x1dvk * xdvk + pow(rAD[current_num_mov],2)) / (pow(xdvk,2) + pow(rAD[current_num_mov],2));
//    T1seqv = x1dvk / (314.159 * rgk);
//    // предусмотреть смену частоты потом здесь ?!!!
//    /// ////////////////////////////////////////////////////////////////////////////
//    // Ток подпитки от эквиваленнтного асинхронного двигателя для  t=0,001с
//    Io1cdv =  (Uc/Z1pdv)*mu[current_num_mov]*exp(-0.01/ T1reqvd);
//    //Контрольный вывод
//    // Постоянная времени обмотки ротора Асинхроннонго Двигателя.
//    //Label9.Caption = ' Io1cdv =' + FloatToStr(Io1cdv) + ' О.Е.';    //В примере 2,582 ое   !???
//    iuddv=( Io1cdv + (Uc/Z1pdv)*exp(-0.01/T1seqv))*sqrt(2)*Ib;     // здесь базовый ток - глобальная переменная из основного модуля
//    //Label10.Caption = 'iuddv =' + FloatToStr(iuddv) + ' кА';
//    IudSigma= Iudarnyjg + iuddv ;            // Суммарное значение ударного тока в заданной точке
//    //Label11.Caption = ' IudSigma =' + FloatToStr( IudSigma) + ' кА';

//    *text_result <<QString::fromUtf8(" Io1cdv = ")  <<  Io1cdv    <<  QString::fromUtf8( " о.е. " )<< '\n';
//    *text_result <<QString::fromUtf8(" iuddv = ")   <<   iuddv    <<  QString::fromUtf8( " кА " )<< '\n';
//    *text_result <<QString::fromUtf8(" IudSigma = ")<<  IudSigma  <<  QString::fromUtf8( " кА " )<< '\n';


//}

void CCalculation::CalcMOV()
{

    int NAD = count_mov;//: integer; //Кол.АДвиг. Затем расширить область видимости переменной!!!!!!!!
    //var  PADnommass, IADnommass, UADmassOE , rADmass ,x1smass , xsmass, mumass , Trmass , T1rmass , CosFimass, Rkmass, Xkmass :  array[1..16] of real;
    //var //,  XkmassOE , rADsummass, x1summass , xSsummass : array[1..16] of real;
    //var  IADnommassOE , E2ADoe , I2ADoe , AT, BT  : array[1..16] of real;


//    var
//     NAD: integer; //Кол.АДвиг. Затем расширить область видимости переменной!!!!!!!!
//    var  PADnommass, IADnommass, UADmassOE , rADmass ,x1smass , xsmass, mumass , Trmass , T1rmass , CosFimass, Rkmass, Xkmass :  array[1..16] of real;
//    var  RkmassOE ,  XkmassOE , rADsummass, x1summass , xSsummass : array[1..16] of real;
//    var  IADnommassOE , E2ADoe , I2ADoe , AT, BT  : array[1..16] of real;
//    var  I2ADsummOE, ATsumm, T1eqvAD , BTsumm , TAeqvAD :real;
//    var  Ip01ADoe, Ip001ADoe , Ia01ADoe , Ia001ADoe , Ip01AD, Ip001AD , Ia01AD , Ia001AD : real;

    double* rADsummass = new double[NAD];
    double* x1summass = new double[NAD] ;
    double* xSsummass = new double[NAD];
    double* IADnommassOE = new double[NAD] ;
    double* RkmassOE = new double[NAD] ;
    double* XkmassOE = new double[NAD] ;
    double* E2ADoe = new double[NAD];
    double* I2ADoe = new double[NAD];
    double* UADmassOE = new double[NAD];
    double* AT = new double[NAD];
    double* BT = new double[NAD];
    double  I2ADsummOE, ATsumm, T1eqvAD , BTsumm , TAeqvAD;// :real;
    double  Ip01ADoe, Ip001ADoe , Ia01ADoe , Ia001ADoe , Ip01AD, Ip001AD , Ia01AD , Ia001AD;// : real;
    qDebug()<< "Эквивалентирование группы Асинхронных Двигателей ";
    //int  j;
    double kq;
    ATsumm=0; BTsumm=0; I2ADsummOE=0;   // инициализация

    qDebug()<< "Приведение внешних от АД сопротивлений [Ом] к базисным первой ступени ";
//        for j := 1 to NAD do begin
//                             RkmassOE[j]:= Rkmass[j]/Zb;
//                             XkmassOE[j]:= Xkmass[j]/Zb;
//                            end;
                            for (int j = 0; j < NAD; ++j) {
                                RkmassOE[j]= Rkmass[j]/Zb;
                                XkmassOE[j]= Xkmass[j]/Zb;
                            }
      qDebug()<< "Здесь запускается эвивалентирование группы АД в пренебрежении активной составляющей внешнего сопротивления";
//       for j := 1 to NAD do
//                        begin
//                        { Массивы сопротивлений цепей AD }
//                        rADsummass[j]:=rADmass[j]+RkmassOE[j];
//                        x1summass[j]:=x1smass[j]+XkmassOE[j];
//                        xSsummass[j]:=xsmass[j]+XkmassOE[j];
//                        end;
         for (int j = 0; j < NAD; ++j) {
             // Массивы сопротивлений цепей AD
             rADsummass[j]=rADmass[j]+RkmassOE[j];
             x1summass[j]=x1smass[j]+XkmassOE[j];
             xSsummass[j]=xsmass[j]+XkmassOE[j];
         }
       qDebug()<< " Приведение к базисным относительным единицам ";
//       for j := 1 to NAD do begin
//                            IADnommassOE[j]:=IADnommass[j]/Ib;
//                            end;

                            for (int j = 0; j < NAD; ++j) {
                               IADnommassOE[j]=IADnommass[j]/Ib;
                            }
                            for (int j = 0; j < NAD; ++j) {
                               UADmassOE[j]=UADnommass[j];///Ub;
                            }
      // Определение сверхпереходного тока двигателей группы }
//         for j := 1 to NAD do begin
//          E2ADoe[j]:=UADmassOE[j]-IADnommassOE[j]* x1smass[j]*sin(arccos( CosFimass[j])); // сверхпереходная ЭДС Асинхронного двигателя (Справочник т.2 стр.489)
//                              end;

          for (int j = 0; j < NAD; ++j) {

              E2ADoe[j]=UADmassOE[j]-IADnommassOE[j]* x1smass[j]*sin(acos(CosFimass[j])); // сверхпереходная ЭДС Асинхронного двигателя (Справочник т.2 стр.489)

          }
//         for j := 1 to NAD do begin
//          I2ADoe[j]:=  E2ADoe[j]/x1summass[j];    // сверхпереходноq ток AD c учетом кабеля
//                              end;

        for (int j = 0; j < NAD; ++j) {

            I2ADoe[j] =  E2ADoe[j]/x1summass[j];    // сверхпереходноq ток AD c учетом кабеля

        }



//         for j := 1 to NAD do begin
//          I2ADsummOE:=I2ADsummOE+I2ADoe[j] ;    // сверхпереходной ток  от группы двигателей
//                              end;

       for (int j = 0; j < NAD; ++j) {

                 I2ADsummOE = I2ADsummOE+I2ADoe[j] ;    // сверхпереходной ток  от группы двигателей
         }
        //{ Промежуточные массивы  для определения экв. пост.вр. }
//        for j := 1 to NAD do begin
//        AT[j]:=T1rmass[j]*I2ADoe[j];
//        BT[j]:=Trmass[j]*I2ADoe[j];
//                             end;

        for (int j = 0; j < NAD; ++j) {


                 AT[j]=T1rmass[j]*I2ADoe[j];
                 BT[j]=Trmass[j]*I2ADoe[j];
        }

//       for j := 1 to NAD do begin
//        ATsumm:=  ATsumm + AT[j] ;      //числитель для средневзвешенного
//        BTsumm:=  BTsumm + BT[j] ;      //числитель для средневзвешенного
//                             end;

        for (int j = 0; j < NAD; ++j) {


                    ATsumm=  ATsumm + AT[j] ;      //числитель для средневзвешенного
                    BTsumm=  BTsumm + BT[j] ;      //числитель для средневзвешенного
        }

        T1eqvAD = ATsumm/I2ADsummOE;       //постоянная времени эквивалентного ас. двигателя Периодическая
        TAeqvAD = BTsumm/I2ADsummOE;       //постоянная времени эквивалентного ас. двигателя Апериодическая

      //Label2.Caption:='T1eqvAD= '+FloatToStr(T1eqvAD )+ '   TAeqvAD = '+FloatToStr(TAeqvAD );
      qDebug()<<"T1eqvAD = "<<T1eqvAD;
      qDebug()<<"TAeqvAD = "<<TAeqvAD;
      //{ Ток подпитки от эквивалентного асинхронного двигателя к шинам РУ}

      Ip01ADoe = I2ADsummOE*exp(-0.01/T1eqvAD);   // Периодическая составляющая в момент 0,01с (Справочник т.2 стр 491)
      Ip001ADoe =I2ADsummOE*exp(-0.001/T1eqvAD); // Периодическая составляющая в момент 0,001с
      kq = sqrt(2.0);
      Ia01ADoe = kq* I2ADsummOE*exp(-0.01/TAeqvAD);  // Апериодическая составляющая в момент 0,01с (Справочник т.2 стр 491)
      Ia001ADoe =kq* I2ADsummOE*exp(-0.001/TAeqvAD); // Апериодическая составляющая в момент 0,001с
      //{В именованных единицах, кА }
       Ip01AD = Ip01ADoe*Ib;
       Ip001AD =Ip001ADoe*Ib;
       Ia01AD =Ia01ADoe*Ib;
       Ia001AD =Ia001ADoe*Ib;
       QString teg = "h3";
       *text_result << QString(" %1 Периодическая составляющая в момент 0,01с :  %2").arg("<h1>").arg("</h1>")   << '\n';
       *text_result << creatStrForReport(teg,"Ip01AD = ", QString::number(Ip01AD), " кА ");

       //qDebug()<<"Периодическая составляющая в момент 0,01с  Ip01AD = "<<T1eqvAD;


       *text_result << QString(" %1 Периодическая составляющая в момент 0,001с :  %2").arg("<h1>").arg("</h1>")   << '\n';
       *text_result << creatStrForReport(teg,"Ip001AD = ", QString::number(Ip001AD), " кА ");

       //qDebug()<<"Периодическая составляющая в момент 0,001с Ip001AD = "<<TAeqvAD;

       *text_result << QString(" %1 Апериодическая составляющая в момент 0,01с :  %2").arg("<h1>").arg("</h1>")   << '\n';
       *text_result << creatStrForReport(teg,"Ia01ADoe = ", QString::number(Ia01AD), " кА ");

       //qDebug()<<"Апериодическая составляющая в момент 0,01с Ia01ADoe = "<<T1eqvAD;

       *text_result << QString(" %1 Апериодическая составляющая в момент 0,001с :  %2").arg("<h1>").arg("</h1>")   << '\n';
       *text_result << creatStrForReport(teg,"Ia01ADoe = ", QString::number(Ia001AD), " кА ");




      //Label4.Caption:='Периодическая составляющая в момент 0,01с  Ip01AD= '+FloatToStrF(Ip01ADoe,ffGeneral,5,4)+' о.е.  '+FloatToStrF(Ip01AD,ffGeneral,5,4)+ ' кА';
      //Label3.Caption:='Периодическая составляющая в момент 0,001с Ip001AD= '+FloatToStrF(Ip001ADoe,ffGeneral,5,4)+' о.е.  '+FloatToStrF(Ip001AD,ffGeneral,5,4)+' кА';
      //Label6.Caption:='Апериодическая составляющая в момент 0,01с Ia01AD= '+FloatToStrF(Ia01ADoe,ffGeneral,5,4)+' о.е.  '+FloatToStrF(Ia01AD,ffGeneral,5,4)+ ' кА';
      //Label5.Caption:='Апериодическая составляющая в момент 0,001с Ia001AD= '+FloatToStrF(Ia001ADoe,ffGeneral,5,4)+' о.е.  '+FloatToStrF(Ia001AD,ffGeneral,5,4)+ ' кА';


}


void CCalculation::CalcTr3()
{

   //Calc();
    qDebug()<< "Zb ============="<< Zb;
   double xtr3wBC, xtr3wBH, xtr3wCH, x1, x2, x3 ;// :real;

        for (int i = 0; i < count_tr3; ++i) {


            xtr3wBC=uktrBC[i]*pow(UnomtrB[i],2 )/(Snomtr3w[i] *100); //Это сопротивление x тр в именованных единицах, Ом ;напряжение на высшей стороне.
            xtr3wBH=uktrBH[i]*pow(UnomtrB[i],2 )/(Snomtr3w[i] *100); //Это сопротивление x тр в именованных единицах, Ом ;напряжение на высшей стороне.
            xtr3wCH=uktrCH[i]*pow(UnomtrB[i],2 )/(Snomtr3w[i] *100); //Это сопротивление x тр в именованных единицах, Ом ;напряжение на высшей стороне.
            x1=(xtr3wBC+xtr3wBH-xtr3wCH)/2;
            x2=(xtr3wBC+xtr3wCH-xtr3wBH)/2;
            x3=(xtr3wBH+xtr3wCH-xtr3wBC)/2;
            xtroeb1=(x1+x2)/Zb; // в о.е.
            xtroeb2=(x1+x3)/Zb; // в о.е.
            qDebug()<< "xtroeb1 ============="<< xtroeb1;
            qDebug()<< "xtroeb2 ============="<< xtroeb2;

            ktr12w3= UnomtrB[i]/UnomtrC[i];    // коэффициенты трансформации
            ktr13w3= UnomtrB[i]/UnomtrH[i];   // коэффициенты трансформации
            qDebug()<< "ktr12w3 ============="<< ktr12w3;
            qDebug()<< "ktr13w3 ============="<< ktr13w3;
            //Label12.Caption='x1 тр ='+FloatToStrF(x1, ffGeneral,6,3)+' Ом'+ 'x2 тр ='+FloatToStrF(x2, ffGeneral,6,3)+' Ом'+ 'x3 тр ='+FloatToStrF(x2, ffGeneral,6,3)+' Ом';
            //Label13.Caption='xtroeb1 ='+FloatToStrF(xtroeb1,ffGeneral,6,3)+' О.Е.Приведенное к базисному сопротивлению на первой ступени '+FloatToStrF(Zb,ffGeneral,7,3)+' Ом';
            //Label14.Caption='xtroeb2 ='+FloatToStrF(xtroeb2,ffGeneral,6,3)+' О.Е.Приведенное к базисному сопротивлению на первой ступени '+FloatToStrF(Zb,ffGeneral,7,3)+' Ом';
         }
            //       }
//    }

}

void CCalculation::CalcTr2()
{
    //double uktr, unomtr,U2tr, Snomtr ,deltaPTr ;
    double  xtroes;
    //Calc();
//    uktr = StrToFloat(Edit1.Text);   //Это Uк% трансформатора
//      unomtr = StrToFloat(Edit2.Text); //Это Uном траансформатора отличается от Ubas
//      U2tr = StrToFloat(Edit5.Text);   //Это паспортное номинальное напряжение обмотки второй ступени трансформатора.
//      Snomtr = StrToFloat(Edit3.Text);
//      deltaPTr =  StrToFloat(Edit4.Text);
//       if Zb=0 then  Label5.Caption = 'Сначала запустите расчет Эквивалентного генератора, должна определиться Zb отличная от 0.';
//       if Snomtr=0 then
//       Label5.Caption = 'Задайте  Sном трансформатора отличной от 0 .'
//      else
//       begin
       xtr = uktr[0]*pow(unomtr[0],2)/(Snomtr[0]*100); //это сопротивление x тр в именованных единицах
       xtroeb = xtr/Zb;
       xtroes = uktr[0]*Sb/(Snomtr[0]*100);      //тоже в относительных
       ktr12 = unomtr[0]/U2tr[0];               // коэффициент трансформации
       //Label5.Caption = 'x тр ='+FloatToStrF(xtr, ffGeneral,7,3)+' Ом' ;               // '+FloatToStr(xtroe)+' в о.е.';
       //{ Приведение к базисному сопротивлению на первой ступени}
       //Label6.Caption = 'xtroeb ='+FloatToStrF(xtroeb,ffGeneral,7,3)+' О.Е.Приведенное к базисному сопротивлению на первой ступени '+FloatToStrF(Zb,ffGeneral,7,3)+' Ом ktr12='+FloatToStrF(ktr12,ffGeneral,7,3);
       //Label7.Caption = 'xtroes ='+FloatToStrF(xtroes,ffGeneral,7,3)+' О.Е.Приведенное к базисной  мощности Sb  на первой ступени '+FloatToStrF(Sb,ffGeneral,7,4)+' MBA';


}

void CCalculation::SetModeWorkFlagTR1(int modeWork)
{
   // QMessageBox::information(0, "modeWork", QString::number(modeWork));
    FlagTR1 = modeWork;
}

template<class type, class t_buff>
void add(type value, t_buff* buff, int current_num_index)
{

         buff[current_num_index]=value;
         //current_num_index++;
}


void CCalculation::Init(int num_gen)
{
       count_gen = num_gen;

       text_result = new QTextStream(&buffer, QIODevice::WriteOnly);


       Sngmass =  new double [num_gen]; // Заполнили массив Номинальных мощностей Генераторов.
       Ingmass =  new double [num_gen]; // Заполнили массив Номинальных токов Генераторов.
       Unmass  =  new double [num_gen];  // Заполнили массив Номинальных напряжений Генераторов.
       fmass   =  new double [num_gen];   // Заполнили массив Номинальной частоты Генераторов.  (для общности)
       rgmass  =  new double [num_gen];  //
       x2dmass =  new double [num_gen]; //
       x1dmass =  new double [num_gen]; // Заполнили массив Переходных сопротивлений Генераторов
       xdmass  =  new double [num_gen]; // Заполнили массив Синхронного индуктивного сопротивления по продольной оси Генератора.
       T2dmass =  new double [num_gen];// Заполнили массив Сверхпереходная постоянная времени по продольной оси Генератора.
       Tfmass  =  new double [num_gen]; // Заполнили массив Постоянная времени обмотки возбуждения при разомкнутой обмотке статора Генератора.
       Iustgmass  =  new double [num_gen];   // Заполнили массив Установившееся значение тока КЗ на зажимах Генератора в О. Е.
       Rvneshmass =  new double [num_gen];   // Заполнили массив внешнего активного сопротивления Генератораторной ветви
       Xvneshmass =  new double [num_gen];   // Заполнили массив внешнего активного сопротивления Генератораторной ветви


//         PADnom  =  new double [count_mov];
//         IADnom   =  new double [count_mov]; // Номинальный ток Асинхронного Двигателя.
//         rAD  =  new double [count_mov]; // Активное сопртивление статора Асинхронного Двигателя.
//         x1s  =  new double [count_mov]; // Переходное индуктивное сопротивление статора Асинхроннонго Двигателя.
//         xs   =  new double [count_mov]; // Индуктивное сопротивление статора Асинхроннонго Двигателя.
//         mu   =  new double [count_mov]; // Коэффициент магнитной связи ротора и статора Асинхроннонго Двигателя.
//         Tr   =  new double [count_mov]; // Постоянная времени обмотки ротора Асинхроннонго Двигателя.
//         T1r  =  new double [count_mov];// Переходная Постоянная времени обмотки ротора Асинхроннонго Двигателя.

         PADnommass  =  new double [count_mov];
         UADnommass  =  new double [count_mov];
         IADnommass  =  new double [count_mov];
         rADmass  =  new double [count_mov];
         x1smass  =  new double [count_mov];
         xsmass  =  new double [count_mov];
         mumass  =  new double [count_mov];
         Trmass  =  new double [count_mov];
         T1rmass  =  new double [count_mov];
         CosFimass  =  new double [count_mov];
         Rkmass  =  new double [count_mov];
         Xkmass  =  new double [count_mov];




          uktrBC  =  new double [count_tr2];
          uktrBH  =  new double [count_tr2];
          uktrCH  =  new double [count_tr2];
          UnomtrB  =  new double [count_tr2];
          UnomtrC  =  new double [count_tr2];
          UnomtrH  =  new double [count_tr2];
          Snomtr3w  =  new double [count_tr2];
          deltaPTrBC  =  new double [count_tr2];
          deltaPTrBH  =  new double [count_tr2];
          deltaPTrCH  =  new double [count_tr2];

           uktr      =  new double [count_tr3];     // Это Uк% трансформатора
           unomtr    =  new double [count_tr3];   // Это Uном траансформатора отличается от Ubas
           U2tr      =  new double [count_tr3];     // Это паспортное номинальное напряжение обмотки второй ступени трансформатора.
           Snomtr    =  new double [count_tr3];   //
           deltaPTr  =  new double [count_tr3]; //
}

void CCalculation::setNumMOV(int num_mov)
{
    count_mov = num_mov;
}

void CCalculation::setPADnommass(double value, int index)
{
    add(value,PADnommass,index);
}

void CCalculation::setIADnommass(double value, int index)
{
    add(value,IADnommass,index);
}

void CCalculation::setUADnommass(double value, int index)
{
    add(value,UADnommass,index);
}

void CCalculation::setrADmass(double value, int index)
{
    add(value,rADmass,index);
}

void CCalculation::setx1smass(double value, int index)
{
    add(value,x1smass,index);
}

void CCalculation::setxsmass(double value, int index)
{
    add(value,xsmass,index);
}

void CCalculation::setmumass(double value, int index)
{
    add(value,mumass,index);
}

void CCalculation::setTrmass(double value, int index)
{
    add(value,Trmass,index);
}

void CCalculation::setT1rmass(double value, int index)
{
    add(value,T1rmass,index);
}

void CCalculation::setCosFimass(double value, int index)
{
    add(value,CosFimass,index);
}

void CCalculation::setRkmass(double value, int index)
{
    add(value,Rkmass,index);
}

void CCalculation::setXkmass(double value, int index)
{
    add(value,Xkmass,index);
}


void CCalculation::setNumGen(int num_gen)
{
    count_gen = num_gen;
}





//void CCalculation::setPADnom(double value, int index)
//{
//    add(value,PADnom,index);
//}

//void CCalculation::setIADnom(double value, int index)
//{
//    //add(value,IADnom,index);

//    add(value,IADnommass,index);

//}

//void CCalculation::setrAD(double value, int index)
//{
//    //add(value,rAD,index);
//    add(value,rADmass,index);

//}

//void CCalculation::setx1s(double value, int index)
//{
//    //add(value,x1s,index);

//    add(value,x1smass,index);

//}

//void CCalculation::setxs(double value, int index)
//{
//    //add(value,xs,index);

//    add(value,xsmass,index);

//}

//void CCalculation::setmu(double value, int index)
//{
//    //add(value,mu,index);

//    add(value,mumass,index);

//}

//void CCalculation::setTr(double value, int index)
//{
//    //add(value,Tr,index);
//    add(value,Trmass,index);
//}

//void CCalculation::setT1r(double value, int index)
//{
//    //add(value,T1r,index);
//    add(value,T1rmass,index);
//}

void CCalculation::setSngmass(double value , int index)
{
    add(value,Sngmass,index);

}

void CCalculation::setfmass(double value , int index)
{
    //qDebug()<<"setfmass = "<<value;
    add(value,fmass,index);
    //qDebug()<<"setfmass = "<<fmass[index];
}

void CCalculation::setIngmass(double value , int index)
{
    add(value,Ingmass,index);
}

void CCalculation::setIustgmass(double value , int index)
{
    add(value,Iustgmass,index);
}


void CCalculation::setrgmass(double value , int index)
{
    add(value,rgmass,index);
}

void CCalculation::setRvneshmass(double value , int index)
{
    add(value,Rvneshmass,index);
}

void CCalculation::setT2dmass(double value , int index)
{
    add(value,T2dmass,index);
}
void CCalculation::setTfmass(double value , int index)
{
    add(value,Tfmass,index);
}

void CCalculation::setUnmass(double value , int index)
{
    add(value,Unmass,index);
}

void CCalculation::setx1dmass(double value , int index)
{
    add(value,x1dmass,index);
}

void CCalculation::setx2dmass(double value , int index)
{
    add(value,x2dmass,index);
}

void CCalculation::setxdmass(double value , int index)
{
    add(value,xdmass,index);
}

void CCalculation::setXvneshmass(double value , int index)
{
    add(value,Xvneshmass,index);
}

void CCalculation::setuktrBC(double value, int index)
{
    add(value,uktrBC,index);
}

void CCalculation::setuktrBH(double value, int index)
{
    add(value,uktrBH,index);
}

void CCalculation::setuktrCH(double value, int index)
{
    add(value,uktrCH,index);
}

void CCalculation::setUnomtrB(double value, int index)
{
    add(value,UnomtrB,index);
}

void CCalculation::setUnomtrC(double value, int index)
{
    add(value,UnomtrC,index);
}

void CCalculation::setUnomtrH(double value, int index)
{
    add(value,UnomtrH,index);
}

void CCalculation::setSnomtr3w(double value, int index)
{
    add(value,Snomtr3w,index);
}

void CCalculation::setdeltaPTrBC(double value, int index)
{
    add(value,deltaPTrBC,index);
}

void CCalculation::setdeltaPTrBH(double value, int index)
{
    add(value,deltaPTrBH,index);
}

void CCalculation::setdeltaPTrCH(double value, int index)
{
    add(value,deltaPTrCH,index);
}

void CCalculation::setuktr(double value, int index)
{
     add(value,uktr,index);
}

void CCalculation::setunomtr(double value, int index)
{
     add(value,unomtr,index);
}

void CCalculation::setU2tr(double value, int index)
{
     add(value,U2tr,index);
}

void CCalculation::setSnomtr(double value, int index)
{
     add(value,Snomtr,index);
}

void CCalculation::setdeltaPTr(double value, int index)
{
     add(value,deltaPTr,index);
}
void CCalculation::ClearAll()
{
    buffer.clear();
}

