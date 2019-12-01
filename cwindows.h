#ifndef CWINDOWS_H
#define CWINDOWS_H

#include <QObject>
#include <QTableWidget>
#include <QDataStream>
#include <QByteArray>
#include "view/CInterface.h"
#include "view/CInterface3.h"
#include "model/ccalculation.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMainWindow>
#include <QAction>
#include <QList>
#include "buff/h_struct1.h"
#include <QtAlgorithms>
#include <algorithm>
#include <QTextStream>
#include <QtPrintSupport>
#include <QSpacerItem>
#include <QLine>
#include "ccruelty.h"
#include <QWebFrame>

class CWindows : public QMainWindow
{
    Q_OBJECT
private:
    ccruelty* schema;

    void writeDev(t_data_list_dev dev,
                  QString srcImg,
                  TYPEDEV  IDDEV,
                  ccruelty* schema ,
                  int &w,
                  int &h,
                  QString id,
                  bool horizontal, bool vertical);
    QWebView* webView;
    QWebView* webReport;
    int id_dev = 0;
    int num_current_w = 0;
    CCalculation calc;
    QPushButton* btn_calculate;
    TYPEKZ CurrenKZ;
    QWidget* w_scheme;



    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    
    
    QAction *checkType1;
    QAction *checkTypeTrans2;
    QAction *checkTypeTrans3HL;
    QAction *checkTypeTrans3LH;


    QTextStream* text_result;

    t_dev map_ID;

    QWidget* main_wdt;
    QWidget* main_bottom_wdt;

    QWidget *horizontalLineWidget;
    QMenu* file;
    QMenu* Settings;
    QActionGroup* alignmentGroup;
    QMenuBar* mb_bar;
    QMenu* viewMenu;
    QGraphicsView* viewScheme;



    int num_gen = 0;
    int num_mov = 0;
    int num_transe2 = 0;
    int num_transe3 = 0;

    QList<int> list_id_mov;
    QList<int> list_id_gen;
    QList<int> list_id_transe2;
    QList<int> list_id_transe3;

    t_data_list_dev map_dev_data_GEN;
    t_data_list_dev map_dev_data_MOV;
    t_data_list_dev map_dev_data_TRANS2;
    t_data_list_dev map_dev_data_TRANS3;

    t_rus_name localeRU;
    QMap<int,QString> list_param_gen;
    QMap<int,QString> list_param_mov;
    QMap<int,QString> list_param_transe2;
    QMap<int,QString> list_param_transe3;



    void readMyJSONSettings();
    CInterface* gen_interface;
    CInterface* mov_interface;
    CInterface3* transf_interface;
    CInterface* other_interface;

    void createActions();
    void createStatusBar();
    void createDockWindows();


    QString name_file_report;

    QString referenceHTML = "src/web/template/reference.html";
    QString sourceSchem = "src/web/template/schema.html";
    QString resultSchem = "src/web/resultSchem.html";
    QString URL_report = "src/web/template/index.html";
    QString URL_report_save = "src/web/template/index_save.html";
    QString URL_report_save_result = "src/web/template/result.html";

    QString htmlTemplate;
    void changeTypeKZ(TYPEKZ type_kz);
public:


    explicit CWindows(QWidget *parent = 0);
    void readTemplateHtmlReport();
    QString writeDataToReport(QString name_header, t_data_list_dev report, QList<int> list_id, QString flag_write, bool add);
    void writeResultToReport(QString name_header,QString Data, QString flag_write, bool add);

    void readmyJSONSettings();
    void InitWidget();
    void InitSettings(QString name_file_settings = "src/settings.txt");
    void InitReport();
    void InitSignals();
    void InitMenu();
    void ClearAll();

    QVBoxLayout* main_layout;
    QTabWidget* control;
    QTextEdit* result;


signals:
    void getSettings();
    void S_addGen();
    void S_addMov();
    void S_addTr2();
    void S_addTr3();
    void S_modeWork(int mode);
    void signalClear();


public slots:

    void setViewSettings();
    void setViewGen(int number);
    void setViewMov(int number);
    void setViewTr2(int number);
    void setViewTr3(int number);
    void setViewRload(int number);

    void slotAddGen(t_table data);
    void slotAddMov(t_table data);
    void slotAddTrans2(t_table data);
    void slotAddTrans3(t_table data);

    void slotDelGen(int index);
    void slotDelMov(int index);
    void slotDelTrans2(int index);
    void slotDelTrans3(int index);

    void slotSaveDataLBufferGen(int ind, QList<double> data);
    void slotSaveDataLBufferMov(int ind, QList<double> data);
    void slotSaveDataLBufferTr2(int ind, QList<double> data);
    void slotSaveDataLBufferTr3(int ind, QList<double> data);


    void slotModeWork(bool checked);
    void slotChageTab(int index);

    void slotCalc();
    void save();
    void open();
    void print();
    void undo();
    void about();
    void reference();

 protected:
    void WriteReport(QString source, QString sourcePath);
 };



#endif // CWINDOWS_H
