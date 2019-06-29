#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QJsonObject>
#include "Setting.h"
#include "NetWork.h"
#include "FileCache.h"
#include "NetWorkDownload.h"
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
    void request();
    void cache();
    void download();

private slots:
    void onLoadFinished(bool stat, const QString msg, const QJsonObject obj);

    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    Setting *m_setting;
    NetWork *m_network;
    FileCache *m_fileCache;
    NetWorkDownload *m_download;
};

#endif // WIDGET_H
