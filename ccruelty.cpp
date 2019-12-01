 #include "ccruelty.h"





QRect ccruelty::loadImg(QString src)
{
    QImage img;
    if (img.load(QApplication::applicationDirPath() + "/" + src))
    {

        QRect rect;
        rect.setWidth(img.width());
        rect.setHeight(img.height());
        return  rect;
        //QMessageBox::information(0, "Information",QString::number(img.width()));
    }
    else
    {
        //QMessageBox::critical(0, "ERROR",QString::number(img.width()));
        return  QRect(0,0,0,0);

    }
}

void ccruelty::setSizeimg(TYPEDEV IDDEV)
{
    QImage img;
    QString src;
    switch (IDDEV) {
    case IDGEN:
            src = "src/web/template/css/images/gen.jpg";
            size_img_gen = loadImg(src);
        break;
    case IDMOV:
            src = "src/web/template/css/images/mov.jpg";
            size_img_mov = loadImg(src);
        break;
    case IDTRANS2:
            src = "src/web/template/css/images/tr2.jpg";
            size_img_tr2 = loadImg(src);
        break;
    case IDTRANS3:
            src = "src/web/template/css/images/tr3_1.jpg";
            size_img_tr3 = loadImg(src);
    break;
    case IDKZ:
            src = "src/web/template/css/images/KZ.png";
            size_img_KZ = loadImg(src);
    break;
    case IDBUS:
            src = "src/web/template/css/images/bus.jpg";
            size_img_bus = loadImg(src);
    break;
    default:
        break;
    }

}

ccruelty::ccruelty(QObject *parent) : QObject(parent)
{
    setSizeimg(IDGEN);
    setSizeimg(IDMOV);
    setSizeimg(IDTRANS2);
    setSizeimg(IDTRANS3);
    setSizeimg(IDKZ);
    setSizeimg(IDBUS);

}

void ccruelty::setNameSChema(QString file_name)
{
    schema = file_name;
}


QString ccruelty::AddAttr(QString attr, QString Value, char Quotes)
{
    QString str;
    //str = attr + "=" + Quotes + Value + Quotes;
    str = QString("%1 %2 %3 %4 %5").arg(attr).arg("=").arg(Quotes).arg(Value).arg(Quotes);
    //QMessageBox::critical(0, "ERROR",str);
    return str;
}

QString valueQuotes(QString attr_value, QString Quotes )
{
    QString str = Quotes + attr_value + Quotes;
    return str;
}


//QString

QString ccruelty::creatObj(QString position,
                      int width,
                      int height,
                      int left,
                      int top,
                      QString id,
                      QString src, int img_w, int img_h)
{
char b = '"';
top =  top + top_html;
left = left + left_html;
    QString attr = " ";
    attr =  QString(" position:%1 ;"
                    " width: %2 ; "
                    " height: %3 ; "
                    " left: %4 ; "
                    " top: %5 ;"
                    " id: %6 ;"
                    )
                    .arg(position).arg(width).arg(height).arg(left).arg(top).arg(id).arg(src);
     //QMessageBox::information(0, "ERROR", attr);
    QString obj ="";
    obj.append("<div");
    obj.append(AddAttr(" style",attr, b));

    attr =  QString(" %1 = \" %2 \"" )
                    .arg("id")
                    .arg(count_obj);

    obj.append(attr);
    obj.append(" > ");
    obj.append("<img ");
    QString str = QString( " %1 = %2 ").arg("border").arg("\"0\"");
    obj.append(str);
    str = QString( " %1 = \" %2 \" ").arg("width").arg(QString::number(img_w));
    obj.append(str);
    obj.append(QString( " %1 = \" %2 \" ").arg("height").arg(QString::number(img_h)));
    obj.append(QString( " %1 = \" %2 \" ").arg("src").arg(src));
    obj.append("> \n");
    obj.append("</div> \n");
    return obj;
}

void ccruelty::writeTemplate()
{

    QFile mFile(QApplication::applicationDirPath()+ "/" + schema);
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
             qDebug() << "Ошибка записи файла";
        }
        stream.setCodec("utf-8");
        stream <<template_obj;

    }
    mFile.close();
}

void ccruelty::addObj(QString flag, QString data)
{
     template_obj.append(data);
}

QRect ccruelty::getSizeimg(TYPEDEV IDDEV)
{

    switch (IDDEV) {
    case IDGEN:
            return size_img_gen;
        break;
    case IDMOV:
            return size_img_mov;
        break;
    case IDTRANS2:
            return size_img_tr2;
        break;
    case IDTRANS3:
           return size_img_tr3;
    case IDKZ:
           return size_img_KZ;
    case IDBUS:
           return size_img_bus;
    break;
    default:
        return  QRect(0,0,0,0);
        break;
    }

}
