#include "CInterface.h"








void CInterface::DeActivateTable(bool on)
{
    table->setDisabled(on);
}

void CInterface::InitReady(bool Ready)
{
    ready = Ready;
}


void CInterface::setNameinterface(QString name)
{
    name_interface =  name;
}

void CInterface::setNumDev(int devNum)
{
    dev_num = devNum;
}

void CInterface::setTypeDev(QString typeDev)
{
    dev_Type = typeDev;
}
void CInterface::setNameDev(QString nameDev)
{
    dev_name = nameDev;
}

void CInterface::SetDataTable(const QStringList &headers,
                           const QStringList &Values,
                           const QStringList &defaultPar )
{
        table->setColumnCount(headers.length()); // Указываем число колонок
        table->setShowGrid(true); // Включаем сетку
        // Разрешаем выделение только одного элемента
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        // Разрешаем выделение построчно
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        // Устанавливаем заголовки колонок
        table->setHorizontalHeaderLabels(headers);
        table->setRowCount(Values.length());
        buff.clear();
        for (int i = 0; i < Values.length(); ++i) {
            QTableWidgetItem* header =  new QTableWidgetItem(Values.at(i));
            header->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            table->setItem(i,0, header);
            table->setItem(i,1, new QTableWidgetItem(defaultPar.at(i)));
            buff.append(QString(defaultPar.at(i)).toDouble());
        }
        table->resizeColumnsToContents();
        table->horizontalHeader()->setStretchLastSection(true);
}




CInterface::CInterface(QWidget *parent) : QWidget(parent)
{
   Error = false;
   proc =  new QProcess[2];
   dev_num = 0;
}



void CInterface::InitWidget( )
{
    //QMessageBox::information(0, "Information", "InitWidget");
    if (Error) return;
        layout_inOut_data = new QHBoxLayout();
        layout_main =  new QVBoxLayout();
        btn_add =  new QPushButton("Добавить");
        btn_del =  new QPushButton("Удалить");
        btn_save =  new QPushButton("Сохранить");
        cb_list_item = new QComboBox();
        table  =  new QTableWidget();
        btn_add->setMaximumWidth(100);
        btn_del->setMaximumWidth(100);
        btn_save->setMaximumWidth(100);


}



void CInterface::InitChemLayout(int type_layout)
{
    if (Error) return;

          if ( btn_add==NULL ||
               btn_del==NULL ||
               btn_save==NULL ||
               cb_list_item == NULL ||
               layout_inOut_data == NULL ||
               layout_main == NULL ||
               table == NULL
             )
          {
            ////qDebug()<< "Error: инициализация Widgets";
            Error = 1;
            return;
          }

            switch (type_layout) {
            case 1:
                SetDataTable(Header_table,name_parametr_table, defaultValue);
                layout_inOut_data->addWidget(cb_list_item);
                layout_inOut_data->addWidget(btn_add);
                layout_inOut_data->addWidget(btn_del);
                layout_inOut_data->addWidget(btn_save);
                layout_main->addLayout(layout_inOut_data);
                layout_main->addWidget(table);
                break;
            case 2:
                break;
            default:
                break;
            }
}


void CInterface::InitSignals()
{
      connect( btn_add,SIGNAL(clicked()),this,SLOT(slotAddItem()));
      connect( btn_del,SIGNAL(clicked()),this,SLOT(slotDelItem()));
      connect( btn_save,SIGNAL(clicked()),this,SLOT(slotSaveItem()));
      connect( cb_list_item, SIGNAL(currentIndexChanged(int)),this,SLOT(slotCurrentIndexChanged(int)));
      connect( table,SIGNAL(cellChanged(int,int)), this, SLOT(slotChangeCell(int,int)));


}

QList<double> CInterface::getChanges()
{
    return buff;
}

void CInterface::Init()
{
        InitWidget();
        InitChemLayout(1);
        InitSignals();
        setLayout(layout_main);
}

int CInterface::getCoutRowTable()
{
    return table->rowCount();
}

QStringList CInterface::getValueTable(int index_column)
{
    QStringList strList;
    for (int i = 0; i < table->rowCount(); ++i) {
        strList.append(table->item(i,index_column)->text());
    }
    return strList;
}

void CInterface::slotSaveItem()
{
    if (name_interface == "генератор")
        QMessageBox::information(0, "Information", "Параметры генератора сохранены");
    else
         QMessageBox::information(0, "Information", name_interface);
    emit signalSaveData(cb_list_item->currentIndex(),buff);
}

void CInterface::slotClear()
{
//    QMessageBox::information(0, "Information", QString::number(cb_list_item->count()));
        blockSignals(true);
        cb_list_item->clear();
        blockSignals(false);
//     QMessageBox::information(0, "Information", QString::number(cb_list_item->count()));
       current_index_item = 0;
       count_item = 0;
}

void CInterface::slotDelItem()
{
   // QMessageBox::information(0, "Information", QString::number(cb_list_item->currentIndex()));
    if (cb_list_item->currentIndex()!=-1)
    {
        emit signalDelItem(cb_list_item->currentIndex());
        cb_list_item->removeItem(cb_list_item->currentIndex());
        emit signalRefreshSchema();
    }
    else
        return;

}

void CInterface::slotAddItem()
{

    t_table data;
    for (int i = 0; i < table->rowCount(); ++i) {
        data.append(table->item(i,1)->text().toDouble());
    }
    emit signalAddItem(data);
    cb_list_item->addItem(name_interface + " " + QString::number(cb_list_item->count()));
    emit signalRefreshSchema();
}




void CInterface::slotCurrentIndexChanged(int index)
{
    emit ChangeIndexDev(index);
}



void CInterface::slotAddItemCB()
{

    //QMessageBox::information(0, "Information", "vvv");
    cb_list_item->addItem(name_interface + " " + QString::number(count_item++));
}

void CInterface::slotChangeCell( int row, int column)
{


     if (column == 1)
     {

        if (row< buff.count())
            buff.takeAt(row);
        buff.insert(row,table->item(row,1)->text().toDouble());

     }
}




void CInterface::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if (key == Qt::Key_Enter)
    {
         QMessageBox::information(0, "Information", "Сохранено...");
         emit signalSaveData(cb_list_item->currentIndex(),buff);
    }

}








