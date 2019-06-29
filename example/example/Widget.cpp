#include "Widget.h"
#include "ui_Widget.h"

#include <QDir>
#include <QDebug>
#include <QFile>

#pragma execution_character_set("utf-8")

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    m_setting(nullptr),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    init();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init()
{
//    m_setting = new Setting(QDir::currentPath() + "/" + "toolBase.ini", this);
//    m_setting->setValue("eee", "8888", true);
//    qDebug()<<m_setting->value("54545", false);
      //m_network = new NetWork(this);

    // QString url = "http://wx.qlogo.cn/mmhead/ver_1/uxKW0ynxkTUNMiaibBo7C7mfHIrVZkQAeQFLU3teHqr3wWTiaUgJSGOASAwsLRJK3Czjiahmvycp01onMMYvJIOuyxZGIBbtqdMmZquD0OSHR5U/96";
     // m_fileCache = new FileCache("E:/temp/", this);
     // qDebug()<<m_fileCache->cacheUrl("https://testmv.xesimg.com/assist/2019/06/24/b20e1b7e8d0aba40/7620/68384271d3514cb7_200849437.aac");
        //FileCache::setTempRoot("E:\\");
    // FileCache::setCache(url, QString("E:\\develop\\assist\\bin\\temp\\0\\4b\\95a2\\96"));
     // m_fileCache->setTempRoot("E:\\qwe\\qwe");

    m_download = new NetWorkDownload(this);
}

void Widget::request()
{
    QString url = "https://crm.xueersi.com/class/statistic";
    QList<NArg> args;
    args << NArg("apiKey", "Q3VzdG9tZXItUmVsYXRpb25zaGlwLU1hbmFnZW1lbnQtYXBpMTYyOQ==")
         << NArg("class_id", 35629);
    int rid = m_network->post(url, args);
    m_network->registerInvokeFunc(rid, NPair(this, "onLoadFinished"));
}

void Widget::cache()
{
    QFile file("E:\\develop\\assist\\bin\\temp\\0\\4b\\95a2\\96");
    qDebug()<<file.open(QIODevice::ReadWrite);
    QByteArray data = file.readAll();

    m_fileCache = new FileCache("E:\\temp");
    QString url = "http://wx.qlogo.cn/mmhead/ver_1/uxKW0ynxkTUNMiaibBo7C7mfHIrVZkQAeQFLU3teHqr3wWTiaUgJSGOASAwsLRJK3Czjiahmvycp01onMMYvJIOuyxZGIBbtqdMmZquD0OSHR5U/96";
    m_fileCache->setCache(url, data);
}

void Widget::download()
{
    m_download->download("http://client.xesimg.com/oa/win/4.4/xes_assist_win_setup_4.4.6.1.exe", "E://aa.exe");
}

void Widget::onLoadFinished(bool stat, const QString msg, const QJsonObject obj)
{
    qDebug()<<stat;
    qDebug()<<msg;
    qDebug()<<obj;
}

void Widget::on_pushButton_clicked()
{
    //request();
   download();
   // cache();

}
