#ifndef CINTERFACE3_H
#define CINTERFACE3_H

#include <QRadioButton>
#include <QGroupBox>

#include <view/CInterface.h>

class CInterface3 : public CInterface
{
    Q_OBJECT
private:
    bool Error;
    QPushButton* btn_save_tr;
    QVBoxLayout* layout_inOut_data;
    QTableView* table_Data;
    QProcess* proc;
    QStringList Header_table;
    QStringList name_parametr_table;
    QGroupBox *createGroup();
    QGroupBox *gr_box;
    QRadioButton *rbtn_rload;                  // R-нагрузка
    QRadioButton *rbtn_Transformer_2winding;   // Трансформатор 2-ух обмоточный
    QRadioButton *rbtn_Transformer_3windingML; // трансформатор 3-ёх обмоточный
    QRadioButton *rbtn_Transformer_3windingLL; // трансформатор 3-ёх обмоточный

public:
    QVBoxLayout* layout_main;

    explicit CInterface3(QWidget *parent = 0);

    void InitWidget();
    void InitChemLayout(int type_layout);
    void InitSignals();
    void Init();
    void hide(int id);

signals:
     void signalChooseRload(int);
     void signalChooseTr2(int);
     void signalChooseTr3(int);
     void signalSaveDataTr2( int, QList<double>);
     void signalSaveDataTr3( int, QList<double>);

public slots:
     void slotSaveItem();
     void slotChooseRload();
     void slotChooseTr2();
     void slotChooseTr3();
protected:
     void keyPressEvent(QKeyEvent *event);
};

#endif // CINTERFACE3_H
