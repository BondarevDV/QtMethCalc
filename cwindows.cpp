#include "cwindows.h"
#include <QMessageBox>
#include <QMenu>
#include <QVBoxLayout>



CWindows::CWindows(QWidget *parent) : QMainWindow(parent)
{
    name_file_report =  "src/web/report.html";
}

void CWindows::readTemplateHtmlReport()
{
    QFile f_template_report;
    f_template_report.setFileName(QApplication::applicationDirPath()+ "/" + URL_report);
    if (!f_template_report.open(QIODevice::ReadWrite))
    {
        QMessageBox::critical(0, "ERROR", "Ошибка чтения шаблона отчётного документа ");
        return;
    }
    else
    {
        htmlTemplate = f_template_report.readAll();
    }
    f_template_report.close();
}



void CWindows::WriteReport(QString source, QString sourcePath)
{
    QFile mFile(QApplication::applicationDirPath()+ "/" + sourcePath);
    qDebug()<<QApplication::applicationDirPath()+ "/" + sourcePath;
    if (!mFile.open(QIODevice::WriteOnly | QIODevice::Text ))
    {
             QMessageBox::critical(0, "ERROR", "Ошибка открытия файла записи");
    }
    else
    {
        QTextStream stream(&mFile);
        if (stream.status() != QTextStream::Ok)
        {
             QMessageBox::critical(0, "ERROR", "Ошибка записи файла");
        }
        stream.setCodec("utf-8");
        stream <<source;
    }
    mFile.close();
}


void CWindows::writeResultToReport(QString name_header, QString Data, QString flag_write, bool add)
{

    QFile f_report;
    f_report.setFileName(QApplication::applicationDirPath()+ "/" + URL_report_save);
    if (!f_report.open(QIODevice::ReadWrite))
    {
        QMessageBox::critical(0, "ERROR", "Ошибка открытия файла ");
    }
    else
    {
        int n = 0;
        QString htmlDoc;
        if (add)
             htmlDoc = f_report.readAll();
        else
             htmlDoc =  htmlTemplate;
        QString str = "";
        str = "%" + flag_write;
        n = htmlDoc.indexOf(str);// + flag_write.length() + QString("</div>").length();
        QString str_html = QString(" <h1> %1 </h1> ").arg(name_header);

        if (n!=-1)
        {
            str_html.append(Data);
        }
        else
            str_html = "Нет данных...";
        htmlDoc.insert(n + str.length(),str_html);
        f_report.close();
        htmlDoc.remove(n,flag_write.length()+1);
        WriteReport(htmlDoc,URL_report_save);
    }


}


QString CWindows::writeDataToReport(QString name_header, t_data_list_dev report, QList<int> list_id, QString flag_write, bool add)
{

    QFile f_report;
    f_report.setFileName(QApplication::applicationDirPath()+ "/" + URL_report_save);
    if (!f_report.open(QIODevice::ReadWrite))
    {
        QMessageBox::critical(0, "ERROR", "Ошибка открытия файла ");
    }
    else
    {
        int n = 0;
        QString htmlDoc;
        if (add)
             htmlDoc = f_report.readAll();
        else
             htmlDoc =  htmlTemplate;
        QString str = "";
        str = flag_write;

        n = htmlDoc.indexOf(str) + flag_write.length() + QString("</div>").length();
        t_value data;
        QString str_html = "";
        if (list_id.count() > 0)
        {
                    int count_row = report[list_id[0]].keys().count();
                    int count_column = list_id.count()+1;
                    QString** table =  new QString* [count_row];
                    for (int count = 0; count < count_row; count++)
                            table[count] = new QString [count_column];
                    QString table_header;
                     str_html.append("<table class=\"jewel\"  cellpadding=\"10\" border=\"1\" width=\"80%\" border=\"1\">");
                    table_header.append("<tr> \n " );
                    if (n!=-1)
                    {
                            for (int i = 0; i < list_id.count(); ++i)
                            {
                                data = report[list_id[i]];
                                int a = 0;
                                foreach (QString key, report[list_id[i]].keys())
                                {
                                        table[a][0] = localeRU[key];
                                        table[a][i+1] = QString::number(data[key]);
                                        a++;
                                }
                            }
                      }
                            for (int i = 0; i < count_row; ++i)
                            {
                                str_html.append("<tr>");
                                for (int j = 0; j < count_column ; ++j) {
                                     str_html.append("<td>");
                                     str_html.append(table[i][j]);
                                     str_html.append("</td>");
                                }


                                str_html.append("</tr>");
                            }

                str_html.append("</table>");
                }
                else
                str_html = "Значения не заданы...";
                htmlDoc.insert(n + str.length() + QString("<\div>").length(),str_html);
        f_report.close();
        return htmlDoc;
    }
    return " ";


}


void CWindows::InitWidget()
{
    btn_calculate = new  QPushButton("Рассчитать");
    btn_calculate->setMaximumWidth(100);
    main_layout =  new QVBoxLayout();
    control =  new  QTabWidget();
    gen_interface = new CInterface();
    mov_interface = new CInterface();
    transf_interface =  new CInterface3();
    other_interface =  new CInterface();
    gen_interface->Init();
    mov_interface->Init();
    transf_interface->Init();
    other_interface->Init();
    gen_interface->setNameinterface("генератор");
    mov_interface->setNameinterface("двигатель");

    other_interface->setNameinterface("Прочее");

    InitSignals();
    InitSettings();

    InitMenu();

    control->addTab( gen_interface,"Цепь генератора");
    control->addTab( mov_interface,"Цепь двигателя");
    control->addTab( transf_interface,"Цепь нагрузки");
    control->setWindowTitle( tr("Расчёт КЗ") );

    result =  new QTextEdit();
    webView =  new QWebView();
    w_scheme =  new QWidget();

    QHBoxLayout* w_scheme_btn_layout =  new QHBoxLayout();
    QVBoxLayout* w_scheme_main_layout =  new QVBoxLayout();
    w_scheme_main_layout->addLayout(w_scheme_btn_layout);
    w_scheme_main_layout->addWidget(webView);
    w_scheme_main_layout->setAlignment(w_scheme_btn_layout, Qt::AlignLeft);
    w_scheme->setLayout(w_scheme_main_layout);

    main_wdt =  new QWidget();
    main_bottom_wdt = new QWidget();
    main_layout->addWidget(control);
    main_layout->addWidget(btn_calculate);
    main_wdt->setLayout(main_layout);
    main_wdt->setMinimumWidth(this->width()*0.65);

    QVBoxLayout* resultLayout =  new QVBoxLayout();
    webReport =  new  QWebView();
    resultLayout->addWidget(webReport);
    resultLayout->addWidget(btn_calculate);
    resultLayout->setAlignment(btn_calculate, Qt::AlignHCenter);
    main_bottom_wdt->setLayout(resultLayout);
    createActions();
    createStatusBar();
    createDockWindows();

    setWindowTitle(tr("Расчёт КЗ"));
    setUnifiedTitleAndToolBarOnMac(true);
    this->setCentralWidget(w_scheme);
    InitReport();
    this->show();

    webReport->load(QUrl("file:///"+QApplication::applicationDirPath() + "/" + URL_report_save));


}


QJsonArray write( QJsonObject &device,
             QString IDdev,
             QString DeviceType,
             QString DeviceName,
             t_value parametr)
{
    QJsonArray devices;
    device["ID"] = IDdev;
    device["DeviceType"] = DeviceType;
    device["DeviceName"] = DeviceName;
    QJsonArray params;
    devices.append(device);
    return devices;
}

void CWindows::InitSettings(QString name_file_settings)
{

        QFile f_settings(name_file_settings);
        QString JSON_settings;
        int k = 2;
        if (!f_settings.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::warning(0, "Предупреждение", "Расчётная схема не загружена");
            return;
        }
        else
             while (!f_settings.atEnd()) {
                 JSON_settings = f_settings.readLine();
             }
        //////qDebug()<<"JSON_settings = "<<"src/"+JSON_settings;
        f_settings.setFileName("src/"+JSON_settings);
        if (!f_settings.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::critical(0, "ERROR", "Ошибка открытия файла конфигурации JSON ");
        }
        else
        {
                   QJsonParseError* json_error =  new  QJsonParseError();
                   QJsonDocument document = QJsonDocument::fromJson(f_settings.readAll(), json_error);
                   //////qDebug()<< json_error->errorString();
                   if (QJsonParseError::NoError != json_error->error)
                   {
                       QMessageBox::critical(0, "ERROR", "Ошибка структуры JSON в конфигурационном файле ");
                       return;
                   }
                   // Забираем из документа корневой объект
                   QJsonObject root = document.object();
                   QJsonValue jv = root.value(SCHEME);
                   // Если значение является массивом, ...
                   if(jv.isArray())
                   {
                       // ... то забираем массив из данного свойства
                       QJsonArray ja = jv.toArray();
                       // Перебирая все элементы массива ...
                       QString typeDev ="";
                       QString nameDev ="";

                       t_value data;
                       for(int i = 0; i < ja.count(); i++)
                       {
                                  id_dev++;
                                  QJsonObject subtree = ja.at(i).toObject();
                                  typeDev = subtree.value(DEVICETYPE).toString();
                                  map_ID.insert(subtree.value(ID).toString(),id_dev);
                                  nameDev = subtree.value(DEVICENAME).toString();
                                  bool gen = false;
                                  bool mov = false;
                                  bool tr2 = false;
                                  bool tr3 = false;
                                  if (subtree.value(PARAMETRS).isArray())
                                  {
                                         QJsonArray j_parametrs = subtree.value(PARAMETRS).toArray();
                                         for(int j = 0; j < j_parametrs.count(); j++)
                                         {
                                                QJsonObject subtree = j_parametrs.at(j).toObject();
                                                QString str = subtree.value(PARAMVALUE).toString();
                                                data.insert(subtree.value(PARAMTYPE).toString(),subtree.value(PARAMVALUE).toDouble());
                                                //QJsonObject subtree = j_parametrs.at(j).toObject();
                                                localeRU.insert(subtree.value(PARAMTYPE).toString(),subtree.value(PARAMNAMERU).toString());
                                                if (typeDev == GEN && !gen)
                                                {
                                                      list_param_gen.insert(j,subtree.value(PARAMTYPE).toString());

                                                }
                                                if (typeDev == MOV && !mov)
                                                {
                                                      list_param_mov.insert(j,subtree.value(PARAMTYPE).toString());
                                                }
                                                if (typeDev == TRANSF2 && !tr2)
                                                {
                                                      list_param_transe2.insert(j,subtree.value(PARAMTYPE).toString());
                                                }
                                                if (typeDev == TRANSF3 && !tr3)
                                                {
                                                     list_param_transe3.insert(j,subtree.value(PARAMTYPE).toString());
                                                }
                                         }

                                  }
                                  gen = true;
                                  mov = true;
                                  tr2 = true;
                                  tr3 = true;
                                  if (typeDev == GEN)
                                  {
                                        map_dev_data_GEN.insert(map_ID[subtree.value(ID).toString()],data);
                                        num_gen++;
                                        list_id_gen.append(map_ID[subtree.value(ID).toString()]);
                                  }
                                  if (typeDev == MOV)
                                  {
                                        map_dev_data_MOV.insert(map_ID[subtree.value(ID).toString()],data);
                                        num_mov++;
                                        list_id_mov.append(map_ID[subtree.value(ID).toString()]);
                                  }
                                  if (typeDev == TRANSF2)
                                  {

                                        map_dev_data_TRANS2.insert(map_ID[subtree.value(ID).toString()],data);
                                        num_transe2++;
                                        list_id_transe2.append(map_ID[subtree.value(ID).toString()]);
                                  }
                                  if (typeDev == TRANSF3)
                                  {
                                        map_dev_data_TRANS3.insert(map_ID[subtree.value(ID).toString()],data);
                                        num_transe3++;
                                        list_id_transe3.append(map_ID[subtree.value(ID).toString()]);

                                  }
                                  data.clear();
                      }

                 }

          }
        QDesktopWidget screen;
        QRect rectXY = screen.availableGeometry();
        this->resize(rectXY.width()/k,rectXY.height()/k);
        emit getSettings();
}

void CWindows::InitReport()
{
     schema = new ccruelty();
     schema->setNameSChema(sourceSchem);
     webView->load(QUrl("file:///"+QApplication::applicationDirPath() + "/"  + sourceSchem));
}

void CWindows::InitSignals()
{

    connect(this,SIGNAL(getSettings()),this,SLOT(setViewSettings()));
    connect(gen_interface, SIGNAL(ChangeIndexDev(int)), this, SLOT(setViewGen(int)));
    connect(mov_interface, SIGNAL(ChangeIndexDev(int)), this, SLOT(setViewMov(int)));
    connect(gen_interface, SIGNAL(signalDelItem(int)), this, SLOT(slotDelGen(int)));
    connect(mov_interface, SIGNAL(signalDelItem(int)), this, SLOT(slotDelMov(int)));
    connect(gen_interface, SIGNAL(signalAddItem(t_table)), this, SLOT(slotAddGen(t_table)));
    connect(mov_interface, SIGNAL(signalAddItem(t_table)), this, SLOT(slotAddMov(t_table)));

    connect(gen_interface, SIGNAL(signalSaveData(int,QList<double>)),this, SLOT(slotSaveDataLBufferGen(int,QList<double>)));
    connect(mov_interface, SIGNAL(signalSaveData(int,QList<double>)),this, SLOT(slotSaveDataLBufferMov(int,QList<double>)));
    connect(transf_interface, SIGNAL(signalSaveDataTr2(int,QList<double>)),this, SLOT(slotSaveDataLBufferTr2(int,QList<double>)));
    connect(transf_interface, SIGNAL(signalSaveDataTr3(int,QList<double>)),this, SLOT(slotSaveDataLBufferTr3(int,QList<double>)));
    connect(this, SIGNAL(S_addGen()),gen_interface, SLOT(slotAddItemCB()));
    connect(this, SIGNAL(S_addMov()),mov_interface, SLOT(slotAddItemCB()));
    connect(btn_calculate, SIGNAL(clicked(bool)),this , SLOT(slotCalc()));

    connect(transf_interface, SIGNAL(signalChooseRload(int)),this, SLOT(setViewRload(int)));
    connect(transf_interface, SIGNAL(signalChooseTr2(int)),this, SLOT(setViewTr2(int)));
    connect(transf_interface, SIGNAL(signalChooseTr3(int)),this, SLOT(setViewTr3(int)));

    connect(gen_interface, SIGNAL(signalChagetTab(int)),this, SLOT(slotChageTab(int)));
    connect(mov_interface, SIGNAL(signalChagetTab(int)),this, SLOT(slotChageTab(int)));
    connect(transf_interface, SIGNAL(signalChagetTab(int)),this, SLOT(slotChageTab(int)));
    connect(gen_interface, SIGNAL(signalRefreshSchema()), this, SLOT(undo()));
    connect(mov_interface, SIGNAL(signalRefreshSchema()), this, SLOT(undo()));
    connect(transf_interface, SIGNAL(signalRefreshSchema()), this, SLOT(undo()));

     connect(this, SIGNAL(signalClear()), gen_interface, SLOT(slotClear()));
     connect(this, SIGNAL(signalClear()), mov_interface, SLOT(slotClear()));

}


void CWindows::InitMenu()
{
    mb_bar =  new  QMenuBar();
    file = new QMenu("&Файл") ;
    newAct =  new QAction("Новый",file);
    openAct =  new QAction("открыть",file);
    saveAct =  new QAction("Сохранить",file);
    file->addAction(newAct);
    file->addAction(openAct);
    file->addAction(saveAct);
    file->addAction("Exit");
    Settings =  new QMenu("&Настройки");
    newAct =  new QAction("Новый",file);
    openAct =  new QAction("открыть",file);
    saveAct =  new QAction("Сохранить",file);
    checkType1 =  new QAction("КЗ на шинах РУ",Settings);
    checkTypeTrans2 =  new QAction("КЗ за 2-х обмоточным трансформатором",Settings);
    checkTypeTrans3HL =  new QAction("КЗ за 3-х обмоточным трансформатором СН",Settings);
    checkTypeTrans3LH =  new QAction("КЗ за 3-х обмоточным трансформатором НН",Settings);
    Settings->addAction(checkTypeTrans2);
    Settings->addAction(checkTypeTrans3HL);
    Settings->addAction(checkTypeTrans3LH);
    Settings->addAction(checkType1);

    connect(checkType1, SIGNAL(triggered(bool)), this, SLOT(slotModeWork(bool)));
    connect(checkTypeTrans2, SIGNAL(triggered(bool)), this, SLOT(slotModeWork(bool)));
    connect(checkTypeTrans3HL, SIGNAL(triggered(bool)), this, SLOT(slotModeWork(bool)));
    connect(checkTypeTrans3LH, SIGNAL(triggered(bool)), this, SLOT(slotModeWork(bool)));

    checkType1->setCheckable(true);
    checkTypeTrans2->setCheckable(true);
    checkTypeTrans3HL->setCheckable(true);
    checkTypeTrans3LH->setCheckable(true);
    mb_bar->addMenu(file);
    mb_bar->addMenu(Settings);
}

void CWindows::ClearAll()
{
    map_dev_data_GEN.clear();
    map_dev_data_MOV.clear();
    list_param_gen.clear();
    list_param_mov.clear();
    list_param_transe2.clear();
    list_param_transe3.clear();
    list_id_gen.clear();
    list_id_mov.clear();
    localeRU.clear();


}




template<class ListID ,class ListDev, class ListDataParam,class ListLocalRU, class Interface>
void initInterface(ListID listID,
                   ListDev listDev,
                   ListDataParam listDataParam,
                   ListLocalRU listLocalRU,
                   Interface* view,
                   int number)
{
    QStringList  par, value;
    QStringList header = { "Параметр",  "значение"};
    t_value data;
    if (listID.count() > 0)
    {
        if (number > listID.count() ) return;
        data = listDev[listID[number]];
    }

       for (int i = 0; i < listDataParam.count(); ++i) {
           par.append(listLocalRU[listDataParam[i]]);
           value.append(QString::number(data.value(listDataParam[i])));
       }
    view->SetDataTable(header,par,value);
}




void CWindows::setViewGen(int number)
{
     initInterface(list_id_gen,map_dev_data_GEN,list_param_gen,localeRU,gen_interface, number);
}

void CWindows::setViewMov(int number)
{
    initInterface(list_id_mov,map_dev_data_MOV,list_param_mov,localeRU,mov_interface, number);
}






void CWindows::setViewTr2(int number)
{
    transf_interface->DeActivateTable(false);
        initInterface(list_id_transe2,map_dev_data_TRANS2,list_param_transe2,localeRU,transf_interface, number);
        changeTypeKZ(KZTRANS2);
         calc.SetModeWorkFlagTR1(KZTRANS2);
}

void CWindows::setViewTr3(int number)
{
    //QMessageBox::critical(0, "ERROR", QString::number(number));
    transf_interface->DeActivateTable(false);
    if (number == KZTRANS31)changeTypeKZ(KZTRANS31);
    if (number == KZTRANS32)changeTypeKZ(KZTRANS32);

    initInterface(list_id_transe3,map_dev_data_TRANS3,list_param_transe3,localeRU,transf_interface, 0);

      calc.SetModeWorkFlagTR1(number);
}

void CWindows::setViewRload(int number)
{
    qDebug()<<number;
    transf_interface->DeActivateTable(true);
     changeTypeKZ(KZR);
      calc.SetModeWorkFlagTR1(KZR);
}


template<class TListID, class TListParam,class TMapData, class TinData >
void setData(TListID* listId,  TListParam param,TMapData* data_map_save, TinData in_data)
{
    listId->append(*std::max_element(listId->begin(),listId->end())+1);
    t_value data;
    for (int i = 0; i < param.count(); ++i) {

        data.insert(param[i],in_data[i]);
    }
    data_map_save->insert(listId->last(),data);
}

void CWindows::slotAddGen(t_table data)
{

    setData(&list_id_gen, list_param_gen, &map_dev_data_GEN, data);

}

void CWindows::slotAddMov(t_table data)
{
    setData(&list_id_mov, list_param_mov, &map_dev_data_MOV, data);
}
void CWindows::slotAddTrans2(t_table data)
{

    setData(&list_id_transe2, list_param_transe2, &map_dev_data_TRANS2, data);

}
void CWindows::slotAddTrans3(t_table data)
{
    setData(&list_id_transe3, list_param_transe3, &map_dev_data_TRANS3, data);
}



template<class ListID ,class MapDev>
void removeItem(   ListID* listID,
                   MapDev* mapDev,
                   int index)
{

    mapDev->remove(listID->at(index));
    listID->removeAt(index);
}

void CWindows::slotDelGen(int index)
{
    removeItem(&list_id_gen,&map_dev_data_GEN,index);
}

void CWindows::slotDelMov(int index)
{


    removeItem(&list_id_mov,&map_dev_data_MOV,index);
}

void CWindows::slotDelTrans2(int index)
{
     removeItem(&list_id_transe2,&map_dev_data_TRANS2,index);
}

void CWindows::slotDelTrans3(int index)
{
    removeItem(&list_id_transe3,&map_dev_data_TRANS3,index);
}


template<class List,class Map, class ListId>
void saveData(int ind, QList<double> data, ListId list_id, List& list_param_dev, Map& map_dev_data_dev)
{
    t_value bf;
        for (int i = 0; i < list_param_dev.count(); ++i) {
                 bf.insert(list_param_dev[i], data[i]);

        }

        map_dev_data_dev.insert(list_id[ind],bf);//,QPair(list_param_gen)
}

void CWindows::slotSaveDataLBufferGen(int ind, QList<double> data)
{
   // QMessageBox::critical(0, "ERROR", QString::number(ind));
    saveData(ind,data, list_id_gen, list_param_gen,map_dev_data_GEN);

}

void CWindows::slotSaveDataLBufferMov(int ind, QList<double> data)
{
    saveData(ind,data, list_id_mov, list_param_mov,map_dev_data_MOV);
}

void CWindows::slotSaveDataLBufferTr2(int ind, QList<double> data)
{
    saveData(ind,data, list_id_transe2, list_param_transe2,map_dev_data_TRANS2);
}

void CWindows::slotSaveDataLBufferTr3(int ind, QList<double> data)
{
    saveData(ind,data, list_id_transe3, list_param_transe3,map_dev_data_TRANS3);
}

void CWindows::slotModeWork(bool checked)
{
    int mode_work = 0;
    qDebug()<<checked;
   // QMessageBox::information(0, "modeWork", QString::number(mode_work));
    if (checkType1->isChecked())
    {
        mode_work = 0;
        changeTypeKZ(KZR);
        calc.SetModeWorkFlagTR1(KZR);
    }

    if (checkTypeTrans2->isChecked())
    {
        mode_work = 1;
        changeTypeKZ(KZTRANS2);
        calc.SetModeWorkFlagTR1(KZTRANS2);
    }
    if (checkTypeTrans3HL->isChecked())
    {
        mode_work = 2;
        changeTypeKZ(KZTRANS31);
        calc.SetModeWorkFlagTR1(KZTRANS31);
    }
    if (checkTypeTrans3LH->isChecked())
    {
        mode_work = 3;
        changeTypeKZ(KZTRANS32);
        calc.SetModeWorkFlagTR1(KZTRANS32);
    }



    emit S_modeWork(mode_work);
}



void CWindows::slotChageTab(int index)
{

    control->setCurrentIndex(index);

    // QMessageBox::critical(0, "ERROR", QString::number(index));
}







void CWindows::slotCalc()
{
    undo();
    t_value data;
    calc.setNumMOV(list_id_mov.count());
    calc.Init(list_id_gen.count());

   for (int i = 0; i < list_id_gen.count(); ++i) {

        data = map_dev_data_GEN[list_id_gen[i]];
        calc.setSngmass(data[G_SNGMASS],i);
        calc.setIngmass(data[G_INGMASS],i);
        calc.setUnmass(data[G_UNGMASS],i);
        calc.setTfmass(data[G_TFMASS],i);
        calc.setfmass(data[G_FMASS],i);
        calc.setrgmass(data[G_RGMASS],i);
        calc.setx2dmass(data[G_X2DMASS],i);
        calc.setx1dmass(data[G_X1DMASS],i);
        calc.setxdmass(data[G_XDMASS],i);
        calc.setT2dmass(data[G_T2DMASS],i);
        calc.setIustgmass(data[G_IUSTGMASS],i);
        calc.setRvneshmass(data[G_RVNESHMASS],i);
        calc.setXvneshmass(data[G_XVNESHMASS],i);
    }

    for (int i = 0; i < list_id_mov.count(); ++i) {


                data = map_dev_data_MOV[list_id_mov[i]];
                calc.setIADnommass(data[M_IADNOM],i);
                calc.setUADnommass(data[M_UADNOM],i);
                calc.setmumass(data[M_MU],i);
                calc.setPADnommass(data[M_PADNOM],i);
                calc.setrADmass(data[M_RAD],i);
                calc.setT1rmass(data[M_T1R],i);
                calc.setTrmass(data[M_TR],i);
                calc.setx1smass(data[M_X1S],i);
                calc.setxsmass(data[M_XS],i);
                calc.setRkmass(data[M_RVNESHMASS],i);
                calc.setXkmass(data[M_XVNESHMASS],i);
                calc.setCosFimass(data[M_COSfi],i);
     }


    if (CurrenKZ == KZTRANS2)
        for (int i = 0; i < 1; ++i) {

            data = map_dev_data_TRANS2[list_id_transe2[i]];
            calc.setdeltaPTr(data[TR2_DELTAPTR],i);
            calc.setSnomtr(data[TR2_SNOMTR],i);
            calc.setU2tr(data[TR2_U2TR],i);
            calc.setuktr(data[TR2_UKTR],i);
            calc.setunomtr(data[TR2_UNOMTR],i);


         }
     if (CurrenKZ == KZTRANS31 || CurrenKZ == KZTRANS32)
            for (int i = 0; i < 1; ++i) {

                data = map_dev_data_TRANS3[list_id_transe3[i]];
                calc.setdeltaPTrBH(data[TR3_DELTAPTRBH],i);
                calc.setdeltaPTrBC(data[TR3_DELTAPTRBC],i);
                calc.setdeltaPTrCH(data[TR3_DELTAPTRCH],i);
                calc.setSnomtr3w(data[TR3_SNOMTR3W],i);
                calc.setuktrBC(data[TR3_UKTRBC],i);
                calc.setuktrCH(data[TR3_UKTRCH],i);
                calc.setuktrBH(data[TR3_UKTRBH],i);
                calc.setUnomtrB(data[TR3_UNOMTRB],i);
                calc.setUnomtrC(data[TR3_UNOMTRC],i);
                calc.setUnomtrH(data[TR3_UNOMTRH],i);
             }

    calc.Calc();
    calc.CalcMOV();

    QString Result = calc.getResult();
    text_result =  new QTextStream(&Result,QIODevice::ReadOnly);

    readTemplateHtmlReport();
    QString html;
    QString flag;

    flag = "%list_id_gen";
    html = writeDataToReport("Генератор",map_dev_data_GEN,list_id_gen, flag,false);
    html.remove(html.indexOf(flag),flag.length());
    WriteReport(html,URL_report_save);


    flag = "%list_id_mov";
    html = writeDataToReport("Двигатель",map_dev_data_MOV,list_id_mov, flag,true);
    html.remove(html.indexOf(flag),flag.length());
    WriteReport(html,URL_report_save);

    if (CurrenKZ == KZTRANS2 )
    {
        flag = "list_id_tr2";
        html = writeDataToReport("Трансформатор двухобмоточный",map_dev_data_TRANS2,list_id_transe2, flag,true);
        WriteReport(html,URL_report_save);
    }


    if (CurrenKZ == KZTRANS31 || CurrenKZ == KZTRANS32 )
    {
        flag = "list_id_tr3";
        html =  writeDataToReport("Трансформатор трёхобмоточный",map_dev_data_TRANS3,list_id_transe3, flag,true);
        WriteReport(html,URL_report_save);
    }


    flag = "%list_id_tr3";
    html.remove(html.indexOf(flag),flag.length());
    flag = "%list_id_tr2";
    html.remove(html.indexOf(flag),flag.length());

     WriteReport(html,URL_report_save);





    writeResultToReport("Результаты расчётов", text_result->readAll(), "result_calculate",true);

    calc.ClearAll();
    webReport->reload();


}




void CWindows::setViewSettings()
{
    //QMessageBox::information(0, "Информация", "Загружена новая конфигурация для представления ");
    QStringList header, par, value;
    t_value data;
    int number = 0;
    //initInterface(list_id_gen,map_dev_data_GEN,list_param_gen,localeRU,gen_interface, number);
    number = 0;

    for (int i = 0; i < list_id_gen.count(); ++i) {
        emit S_addGen();
    }

   // initInterface(list_id_mov,map_dev_data_MOV,list_param_mov,localeRU,mov_interface, number);
    for (int i = 0; i < list_id_mov.count(); ++i) {
        emit S_addMov();
    }
    initInterface(list_id_transe2,map_dev_data_TRANS2,list_param_transe2,localeRU,transf_interface, number);
    initInterface(list_id_transe3,map_dev_data_TRANS3,list_param_transe3,localeRU,transf_interface, number);
    transf_interface->DeActivateTable(true);
    // gen_interface->InitReady(true);


}







void CWindows::print()
{

#ifndef QT_NO_PRINTDIALOG
    QPrinter printer;

        QPrintDialog dlg(&printer, this);
        if (dlg.exec() != QDialog::Accepted) {
            return;
        }
        webReport->print(&printer);

#endif
}




void CWindows::save()
{


        QString fileName = QFileDialog::getSaveFileName(this,
                            tr("Choose a file name"), ".",
                            "PDF-Files (*.pdf)");


 if (fileName!=NULL)
 {
    QWebView* printHtmlView;

    printHtmlView = new QWebView;
    connect(printHtmlView, SIGNAL(loadFinished(bool)), this, SLOT(htmlRender()));
    printHtmlView->setHtml(webReport->page()->mainFrame()->toHtml()); // загружаем ваш html.
    // слот вызывается сразу после полной загрузки веб-страницы
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName); // устанавливаем путь к pdf файлу
    printHtmlView->print(&printer);
 }
}




void CWindows::open()
{
    QFile f_settings;
    QMimeDatabase mimeDatabase;
    QString fileName = QFileDialog::getOpenFileName(this,
                        tr("Choose a file name"), ".",
                        mimeDatabase.mimeTypeForName("json").filterString());
    f_settings.setFileName(fileName);
    if (f_settings.exists())
    {
        emit signalClear();
        ClearAll();
        if (!f_settings.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::critical(0, "ERROR", "Ошибка открытия файла конфигурации JSON ");
        }
        else
        {
                   QJsonParseError* json_error =  new  QJsonParseError();
                   QJsonDocument document = QJsonDocument::fromJson(f_settings.readAll(), json_error);
                   //////qDebug()<< json_error->errorString();
                   if (QJsonParseError::NoError != json_error->error)
                   {
                       QMessageBox::critical(0, "ERROR", "Ошибка структуры JSON в конфигурационном файле ");
                       return;
                   }
                   // Забираем из документа корневой объект
                   QJsonObject root = document.object();
                   QJsonValue jv = root.value(SCHEME);
                   // Если значение является массивом, ...
                   if(jv.isArray())
                   {
                       // ... то забираем массив из данного свойства
                       QJsonArray ja = jv.toArray();
                       // Перебирая все элементы массива ...
                       QString typeDev ="";
                       QString nameDev ="";

                       t_value data;

                       for(int i = 0; i < ja.count(); i++)
                       {
                                  id_dev++;
                                  QJsonObject subtree = ja.at(i).toObject();
                                  typeDev = subtree.value(DEVICETYPE).toString();
                                  map_ID.insert(subtree.value(ID).toString(),id_dev);
                                  nameDev = subtree.value(DEVICENAME).toString();
                                  if (subtree.value(PARAMETRS).isArray())
                                  {
                                         QJsonArray j_parametrs = subtree.value(PARAMETRS).toArray();
                                         for(int j = 0; j < j_parametrs.count(); j++)
                                         {
                                                QJsonObject subtree = j_parametrs.at(j).toObject();
                                                QString str = subtree.value(PARAMVALUE).toString();
                                                data.insert(subtree.value(PARAMTYPE).toString(),subtree.value(PARAMVALUE).toDouble());
                                                localeRU.insert(subtree.value(PARAMTYPE).toString(),subtree.value(PARAMNAMERU).toString());
                                                if (typeDev == GEN)
                                                {
                                                      list_param_gen.insert(j,subtree.value(PARAMTYPE).toString());
                                                      //gen = true;
                                                }
                                                if (typeDev == MOV )
                                                {
                                                      list_param_mov.insert(j,subtree.value(PARAMTYPE).toString());
                                                      //mov = true;
                                                }
                                                if (typeDev == TRANSF2)
                                                {
                                                      list_param_transe2.insert(j,subtree.value(PARAMTYPE).toString());
                                                      //tr2 = true;
                                                }
                                                if (typeDev == TRANSF3)
                                                {
                                                      list_param_transe3.insert(j,subtree.value(PARAMTYPE).toString());
                                                      //tr3 = true;
                                                }

                                         }

                                  }

                                  if (typeDev == GEN)
                                  {
                                        map_dev_data_GEN.insert(map_ID[subtree.value(ID).toString()],data);
                                        num_gen++;
                                        list_id_gen.append(map_ID[subtree.value(ID).toString()]);
                                  }
                                  if (typeDev == MOV)
                                  {
                                        map_dev_data_MOV.insert(map_ID[subtree.value(ID).toString()],data);
                                        num_mov++;
                                        list_id_mov.append(map_ID[subtree.value(ID).toString()]);
                                  }
                                  if (typeDev == TRANSF2)
                                  {
                                      //if (list_id_transe2.count() == 0)
                                      //{
                                            list_id_transe2.clear();
                                            map_dev_data_TRANS2.clear();
                                            map_dev_data_TRANS2.insert(map_ID[subtree.value(ID).toString()],data);
                                            num_transe2++;
                                            list_id_transe2.append(map_ID[subtree.value(ID).toString()]);
                                      //}
                                  }
                                  if (typeDev == TRANSF3)
                                  {
                                      //if (list_id_transe3.count() == 0)
                                      //{
                                            list_id_transe3.clear();
                                            map_dev_data_TRANS3.clear();
                                            map_dev_data_TRANS3.insert(map_ID[subtree.value(ID).toString()],data);
                                            num_transe3++;
                                            list_id_transe3.append( map_ID[subtree.value(ID).toString()]);
                                            //emit S_addTr3();
                                      //}
                                  }
                                  data.clear();
                      }

                 }

          }

       emit getSettings();
        undo();
    }

}



    void  CWindows::writeDev(t_data_list_dev dev,
                             QString srcImg,
                             TYPEDEV  IDDEV,
                             ccruelty* schema,
                             int& w ,
                             int& h,
                             QString id, bool horizontal, bool vertical)
    {

        for (int i = 0; i < dev.count(); ++i) {

                QString str = schema->creatObj("absolute",
                                               schema->getSizeimg(IDDEV).width(),
                                               schema->getSizeimg(IDDEV).height(),
                                               w,h,id + QString::number(i),
                                               srcImg,
                                               schema->getSizeimg(IDDEV).width(),
                                               schema->getSizeimg(IDDEV).height());

                schema->addObj("%obj",str);
                schema->writeTemplate();
                if (horizontal)
                   w = w + schema->getSizeimg(IDDEV).width();
                //QMessageBox::information(0, "ERROR", QString::number(schema->getSizeimg(IDDEV).width()));
        }
         if (vertical)
            h = h +  schema->getSizeimg(IDDEV).height();



    }










void CWindows::undo()
{

     bool kz_onBus = false;
     schema = new ccruelty();
     QRect desk_top = QApplication::desktop()->availableGeometry(0);
     //qDebug() <<"height = "<< desk_top.height();
     //qDebug() <<"width = " << desk_top.width();
     //QRect size;

     int h  = 0;
     int w  = 0;
     QString id = "id";
     QString srcGen = "css/images/gen.jpg" ;
     writeDev(map_dev_data_GEN,srcGen,IDGEN,schema,w,h,id,true,true);
     int width_bus = w + schema->getSizeimg(IDTRANS3).width();//desk_top.width();
     int heigth_bus = schema->getSizeimg(IDBUS).height();
     int h_bus = schema->getSizeimg(IDGEN).height();
     QString srcBus = "css/images/bus.jpg" ;
     QString strBus = schema->creatObj("absolute",schema->getSizeimg(IDGEN).width(),schema->getSizeimg(IDGEN).height(),0,h,id + "bus",srcBus,width_bus,heigth_bus);
     schema->addObj("%obj",strBus);
     schema->writeTemplate();
     h = h + heigth_bus;
     int h_kz = h;
     int for_tr = w;
     w = 0;
     QString srcMov = "css/images/mov.jpg" ;
     writeDev(map_dev_data_MOV,srcMov,IDMOV,schema,w,h,id,true,false);


     if (w > for_tr)
        for_tr =  w;// + schema->getSizeimg(IDMOV).width();
     if (CurrenKZ == KZTRANS2)
     {
         QString srcTr2 = "css/images/tr2.jpg" ;
        writeDev(map_dev_data_TRANS2,srcTr2,IDTRANS2,schema,for_tr,h,id,true,false);
     }
     else
     if(CurrenKZ == KZTRANS31 || CurrenKZ == KZTRANS32)
     {
        QString srcTr3_1 = "css/images/tr3_1.jpg" ;
         QString srcTr3_2 = "css/images/tr3_2.jpg" ;
        if (CurrenKZ == KZTRANS31)

        writeDev(map_dev_data_TRANS3,srcTr3_1,IDTRANS3,schema,for_tr,h,id,true,false);
        if (CurrenKZ == KZTRANS32)
        writeDev(map_dev_data_TRANS3,srcTr3_2,IDTRANS3,schema,for_tr,h,id,true,false);
        //kz_onBus = false;
     }
     else
        kz_onBus = true;



     strBus = schema->creatObj("absolute",w,schema->getSizeimg(IDBUS).height(),0,h_bus,id + "bus",srcBus,for_tr,schema->getSizeimg(IDBUS).height());
     schema->addObj("%obj",strBus);
     schema->writeTemplate();

     if (kz_onBus)
     {
         QString srcKZ = "css/images/KZ.png" ;
         int w_srcKZ = schema->getSizeimg(IDKZ).width();
         int h_srcKZ = schema->getSizeimg(IDKZ).height();
         QString strKZ = schema->creatObj("absolute",w_srcKZ,
                                          h_srcKZ,
                                          for_tr - 40,
                                          h_kz-35,
                                          id + "kz",
                                          srcKZ,w_srcKZ,h_srcKZ);
         schema->addObj("%obj",strKZ);
         schema->writeTemplate();

     }
     webView->reload();//QUrl("file:///"+QApplication::applicationDirPath() + "/" + sourceSchem));
     // webView->load(QUrl("file:///"+QApplication::applicationDirPath() + "/" + sourceSchem));



}




void CWindows::about()
{

    QMessageBox::about(this, tr("НИЦ Курчатовский институт"),
                       tr("Программа расчёта токов КЗ"));

}

void CWindows::reference()
{


    webView->load(QUrl("file:///"+QApplication::applicationDirPath() + "/" + referenceHTML));
}

void CWindows::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&Файл"));
    QToolBar *fileToolBar = addToolBar(tr("File"));

//    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
//    QAction *newLetterAct = new QAction(newIcon, tr("&New Letter"), this);


//    newLetterAct->setShortcuts(QKeySequence::New);
//    newLetterAct->setStatusTip(tr("Create a new form letter"));
//    connect(newLetterAct, &QAction::triggered, this, &CWindows::newLetter);
//    fileMenu->addAction(newLetterAct);
//    fileToolBar->addAction(newLetterAct);

//    const QIcon fileopenIcon = QIcon::fromTheme("document-new", QIcon(":/images/fileopen.png"));
//    QAction *OpenFile = new QAction(fileopenIcon, tr("&Open file"), this);
//    OpenFile->setShortcuts(QKeySequence::Open);
//    OpenFile->setStatusTip(tr("Create a new form letter"));
//    connect(OpenFile, &QAction::triggered, this, &CWindows::newLetter);
//    fileMenu->addAction(OpenFile);
//    fileToolBar->addAction(OpenFile);
    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/openfile256.png"));
    QAction *openAct = new QAction(openIcon, tr("&Загрузить схему json"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Загрузить конфигурационный файл"));
    connect(openAct, &QAction::triggered, this, &CWindows::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);


    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    QAction *saveAct = new QAction(saveIcon, tr("&Сохранить отчёт"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Сохранить текущий отчёт"));
    connect(saveAct, &QAction::triggered, this, &CWindows::save);
    fileMenu->addAction(saveAct);
    fileToolBar->addAction(saveAct);

    const QIcon printIcon = QIcon::fromTheme("document-print", QIcon(":/images/print.png"));
    QAction *printAct = new QAction(printIcon, tr("&Распечатать отчёт"), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Распечатать текущий отчёт"));
    connect(printAct, &QAction::triggered, this, &CWindows::print);
    fileMenu->addAction(printAct);
    fileToolBar->addAction(printAct);
    fileMenu->addSeparator();


    const QIcon referenceIcon = QIcon::fromTheme("Reference", QIcon(":/images/help.png"));
    QAction *referenceAct = new QAction(referenceIcon, tr("&Справка"), this);
    referenceAct->setShortcuts(QKeySequence::HelpContents);
    referenceAct->setStatusTip(tr("Справка"));
    connect(referenceAct, &QAction::triggered, this, &CWindows::reference);
    fileMenu->addAction(referenceAct);
    fileToolBar->addAction(referenceAct);
    fileMenu->addSeparator();


    QMenu *editMenu = menuBar()->addMenu(tr("&Правка"));
    QToolBar *editToolBar = addToolBar(tr("Edit"));
    const QIcon undoIcon = QIcon::fromTheme("edit-undo", QIcon(":/images/undo.png"));
    QAction *undoAct = new QAction(undoIcon, tr("&Обновить электрическую схему"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Обновление электрической схемы"));
    connect(undoAct, &QAction::triggered, this, &CWindows::undo);
    editMenu->addAction(undoAct);
    editToolBar->addAction(undoAct);
    viewMenu = menuBar()->addMenu(tr("&Вид"));
    menuBar()->addSeparator();
}

void CWindows::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void CWindows::createDockWindows()
{
    QDockWidget *dock;
    dock = new QDockWidget(tr("Панель управления"), this);
    dock->setWidget(main_wdt);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());
    dock = new QDockWidget(tr("Отчёт"), this);
    dock->setWidget(main_bottom_wdt);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

}

void CWindows::changeTypeKZ(TYPEKZ type_kz)
{
    checkType1->setChecked(false);
    checkTypeTrans2->setChecked(false);
    checkTypeTrans3HL->setChecked(false);
    checkTypeTrans3LH->setChecked(false);

    switch (type_kz) {
    case KZR:
        checkType1->setChecked(true);
        CurrenKZ = KZR;
        break;
    case KZTRANS2:
        checkTypeTrans2->setChecked(true);
        CurrenKZ = KZTRANS2;
        break;
    case KZTRANS31:
        checkTypeTrans3HL->setChecked(true);
        CurrenKZ = KZTRANS31;
        break;
    case KZTRANS32:
        CurrenKZ = KZTRANS32;
        checkTypeTrans3LH->setChecked(true);
        break;
    default:
        break;
    }
}
