#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QJsonObject>
#include "Setting.h"
#include "NetWork.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void init();
    void load();

private slots:
    void onLoadFinished(bool stat, const QString msg, const QJsonObject obj);

    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    Setting *m_setting;
    NetWork *m_network;
};

#endif // WIDGET_H
