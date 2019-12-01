#ifndef CINTERFACE2_H
#define CINTERFACE2_H

#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLayout>
#include <QMenu>
#include <QMenuBar>
#include <QButtonGroup>
#include <QPushButton>
#include <QComboBox>
#include <QTableView>
#include <QTableWidget>
#include <QTextEdit>
#include <QSettings>
#include <QLabel>
#include <QDebug>
#include <QWebView>
#include <QDesktopWidget>
#include <QProcess>
#include <QDir>
#include <QHeaderView>
#include <QByteArray>
#include <QDataStream>
#include <QMessageBox>
#include <QKeyEvent>
#include "buff/h_struct1.h"


class CInterface : public QWidget
{
    Q_OBJECT
private:
    bool ready = false;
    QList<QString> keysData;
    QString name_interface;
    int  current_index_item;
    bool Error;
    int dev_num;
    int count_item = 0;
    QString dev_Type;
    QString dev_name;

    QHBoxLayout* layout_inOut_data;
    QPushButton* btn_add;
    QPushButton* btn_del;
    QPushButton* btn_save;
    QComboBox* cb_list_item;
    QProcess* proc;


    QStringList Header_table;
    QStringList name_parametr_table;
    QStringList defaultValue;

public:

    QList<double> buff;
    QTableWidget* table;
    void DeActivateTable(bool on);
    void InitReady(bool Ready);
    void setNameinterface(QString name = "items");
    void setNumDev(int devNum);
    void setTypeDev(QString typeDev);
    void setNameDev(QString nameDev);
    void SetDataTable(const QStringList &headers, const QStringList &Value, const QStringList &defaultPar);
    QVBoxLayout* layout_main;
    explicit CInterface(QWidget *parent = 0);
    virtual void InitWidget();
    virtual void InitChemLayout(int type_layout = 0);
    virtual void InitSignals();
    QList<double> getChanges();
    void Init();
    void hide(int id);
    int getCoutRowTable();
    QStringList getValueTable(int index_column);
    QDataStream* stream;

signals:
     void Calculate();
     void ChangeIndexDev(int);
     void signalDelItem(int index);
     void signalAddItem(t_table data);
     void signalSaveData( int, QList<double>);

     void signalChagetTab(int);
     void signalRefreshSchema();


public slots:
     virtual void slotSaveItem();
     void slotDelItem();
     void slotAddItem();
     void slotCurrentIndexChanged(int );
     //void slotAdditem();
     void slotAddItemCB();
     void slotClear();
     void slotChangeCell(int row, int column);

protected:
     virtual void keyPressEvent(QKeyEvent *event);





};



#endif // CINTERFACE2_H
