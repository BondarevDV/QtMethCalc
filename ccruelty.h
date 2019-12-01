#ifndef CCRUELTY_H
#define CCRUELTY_H

#include <QObject>
#include "buff/h_struct1.h"
#include <QFile>
#include <QMessageBox>
#include <QApplication>
#include <QTextStream>
#include <QDebug>
#include <QImage>

class ccruelty : public QObject
{
    Q_OBJECT
private:
    QRect size_img_mov;
    QRect size_img_gen;
    QRect size_img_tr2;
    QRect size_img_tr3;
    QRect size_img_KZ;
    QRect size_img_bus;
    int count_obj = 0;
    int top_html = 200;
    int left_html = 200;
    QString schema =  "src/web/template/schema.html";
    QString template_obj;
    QString html;
    QMap<QString, QString> attrDiv;
    QRect loadImg(QString src);
    void setSizeimg(TYPEDEV IDDEV);
public:
    explicit ccruelty(QObject *parent = 0);
    void setNameSChema(QString file_name);
    QString AddAttr(QString attr, QString Value, char Quotes);
    QString creatObj(QString position,
                   int width,
                   int height,
                   int left,
                   int top,
                   QString id,
                   QString src, int img_w, int img_h);
    void writeTemplate();
    void addObj(QString flag, QString data);
    QRect getSizeimg(TYPEDEV IDDEV);


signals:

public slots:
};

#endif // CCRUELTY_H
