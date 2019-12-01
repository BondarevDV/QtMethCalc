#include "CInterface3.h"
#include <QMessageBox>


QGroupBox *CInterface3::createGroup()
{
    gr_box = new QGroupBox(tr("Тип нагрузки"));
    rbtn_rload =  new QRadioButton(tr("Короткое замыкание на шине ")); // R-нагрузка
    rbtn_Transformer_2winding =  new QRadioButton(tr( "Трансформатор двухобм."));; // Трансформатор 2-ух обмоточный
    rbtn_Transformer_3windingML =  new QRadioButton(tr( "Трансформатор трёхобм. СН"));; // трансформатор 3-ёх обмоточный
    rbtn_Transformer_3windingLL =  new QRadioButton(tr( "Трансформатор трёхобм. НН"));; // трансформатор 3-ёх обмоточный



        QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addWidget(rbtn_rload);
        vbox->addWidget(rbtn_Transformer_2winding);
        vbox->addWidget(rbtn_Transformer_3windingML);
        vbox->addWidget(rbtn_Transformer_3windingLL);
        vbox->addStretch(1);
        gr_box->setLayout(vbox);
        gr_box->setMaximumHeight(100);
        rbtn_rload->setChecked(true);
        return gr_box;
}




CInterface3::CInterface3(QWidget *parent) : CInterface (parent)//(QWidget *QWidget) : CInterface(QWidget)
{

}

void CInterface3::InitWidget()
{
    // QMessageBox::information(0, "Information", "CInterface3 InitWidget");
    btn_save_tr = new QPushButton("Сохранить");
    btn_save_tr->setFixedHeight(30);
    btn_save_tr->setFixedWidth(100);
        layout_inOut_data = new QVBoxLayout();
        layout_main =  new QVBoxLayout();
        table  =  new QTableWidget();
}



void CInterface3::InitChemLayout(int type_layout)
{


            switch (type_layout) {
            case 1:
                //QMessageBox::information(0, "Information","Интерфейс трансформатора загружен");
                layout_inOut_data->addWidget(createGroup());
                layout_inOut_data->addWidget(btn_save_tr);
                layout_main->addLayout(layout_inOut_data);
                layout_main->addWidget(table);
                break;
            case 2:


                break;
            default:
                break;
            }





}


void CInterface3::InitSignals()
{
     //QMessageBox::information(0, "Information", "slotChooseTr3");
      connect(rbtn_rload ,SIGNAL(clicked(bool)),this,SLOT(slotChooseRload()));
      connect(rbtn_Transformer_2winding ,SIGNAL(clicked(bool)),this,SLOT(slotChooseTr2()));
      connect(rbtn_Transformer_3windingML ,SIGNAL(clicked(bool)),this,SLOT(slotChooseTr3()));
      connect(rbtn_Transformer_3windingLL ,SIGNAL(clicked(bool)),this,SLOT(slotChooseTr3()));
      connect(table,SIGNAL(cellChanged(int,int)), this, SLOT(slotChangeCell(int,int)));
      connect(btn_save_tr,SIGNAL(clicked()),this,SLOT(slotSaveItem()));
}

void CInterface3::Init()
{
    Error = false;
    InitWidget();
    InitChemLayout(1);
    InitSignals();
    setLayout(layout_main);

}

void CInterface3::slotSaveItem()
{
    QMessageBox::information(0, "Information", "Параметры трансформатора сохранены");
    if (rbtn_Transformer_2winding->isChecked())
       emit signalSaveDataTr2(0,buff);
    if (rbtn_Transformer_3windingML->isChecked())
       emit signalSaveDataTr3(0,buff);  
    if (rbtn_Transformer_3windingLL->isChecked())
       emit signalSaveDataTr3(0,buff);
}


void CInterface3::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if (key == Qt::Key_Enter)
    {
         QMessageBox::information(0, "Information", "Сохранено...");
         qDebug()<<buff;
         if (rbtn_Transformer_2winding->isChecked())
            emit signalSaveDataTr2(0,buff);
         if (rbtn_Transformer_3windingML->isChecked())
            emit signalSaveDataTr3(0,buff);
         if (rbtn_Transformer_3windingLL->isChecked())
            emit signalSaveDataTr3(0,buff);
    }
}



void CInterface3::slotChooseRload()
{
    int number = 0;
    //QMessageBox::information(0, "Information", "slotChooseTr2");

    emit signalChooseRload(number);
    emit signalRefreshSchema();
}

void CInterface3::slotChooseTr2()
{
    int number = 0;
    //QMessageBox::information(0, "Information", "slotChooseTr2");

    emit signalChooseTr2(number);
    emit signalRefreshSchema();
}

void CInterface3::slotChooseTr3()
{
    int number = 0;
    //QMessageBox::information(0, "Information", "slotChooseTr3");
    if (rbtn_Transformer_3windingML->isChecked())
        number = KZTRANS31;
    if (rbtn_Transformer_3windingLL->isChecked())
        number = KZTRANS32;
    emit signalChooseTr3(number);
    emit signalRefreshSchema();
}




