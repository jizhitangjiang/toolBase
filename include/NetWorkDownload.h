#ifndef NETWORKDOWNLOAD_H
#define NETWORKDOWNLOAD_H

#include <QObject>
#include <QFile>
#include <QNetworkAccessManager>

class NetWorkDownload : public QObject
{
    Q_OBJECT
public:
    explicit NetWorkDownload(QObject *parent = 0);
    explicit NetWorkDownload(const QString &url, const QString &filePath, QObject *parent = 0);
    ~NetWorkDownload();

public:
    bool download(const QString &url, const QString &filePath = "");
    bool download();
    void stop();
    QString url(){return m_url;}
    QString filePath(){return m_filePath;}

signals:
    void downloadFinished(bool status, const QString &msg);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

public slots:
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onDownloadFinished();

private:
    static QNetworkAccessManager m_manager;
    QString m_url;
    QString m_filePath;
    QFile   m_file;
    QNetworkReply *m_reply;
};

#endif // NETWORKDOWNLOAD_H
