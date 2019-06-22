#include "Widget.h"
#include "ui_Widget.h"

#include <QDir>
#include <QDebug>

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

      m_network = new NetWork(this);
}

void Widget::load()
{
    QString url = "https://crm.xueersi.com/class/statistic";
    NMap args;
    args.insert("apiKey", "Q3VzdG9tZXItUmVsYXRpb25zaGlwLU1hbmFnZW1lbnQtYXBpMTYyOQ==");
    args.insert("class_id", 35629);
    int rid = m_network->post(url, args);
    m_network->registerInvokeFunc(rid, NPair(this, "onLoadFinished"));
}

void Widget::onLoadFinished(bool stat, const QString msg, const QJsonObject obj)
{
    qDebug()<<stat;
    qDebug()<<msg;
    qDebug()<<obj;
}

void Widget::on_pushButton_clicked()
{
    load();
}
