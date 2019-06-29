#include "NetWorkDownload.h"
#include <QUrl>
#include <QNetworkReply>
#include <QDebug>
#include <QApplication>
#include <QDir>

#include "FileCache.h"

QNetworkAccessManager NetWorkDownload::m_manager;

NetWorkDownload::NetWorkDownload(QObject *parent)
    : QObject(parent)
    , m_url("")
    , m_filePath("")
    , m_reply(nullptr)
{

}

NetWorkDownload::NetWorkDownload(const QString &url, const QString &filePath,
    QObject *parent)
    : QObject(parent)
    , m_url(url)
    , m_filePath(filePath)
    , m_reply(nullptr)
{

}

NetWorkDownload::~NetWorkDownload()
{
    stop();
}

bool NetWorkDownload::download(const QString &url, const QString &filePath)
{
    QUrl u(url);
    if (!u.isValid() || m_reply != nullptr) {
        return false;
    }

    FileCache cache(QApplication::applicationDirPath() + "/temp/");
    QString fileName = filePath;
    if (fileName.isEmpty()) {
        fileName = cache.setDir(url) + u.fileName();
    }

    m_file.setFileName(fileName + ".tmp");
    if (!m_file.open(QIODevice::Append)) {
        return false;
    }

    QNetworkRequest req(u);
    qint64 size = m_file.size();
    if (size > 0) {
        QByteArray recvByte = QString("bytes=%1-").arg(size).toUtf8();
        req.setRawHeader("Range", recvByte);
    }

    m_reply = m_manager.get(req);
    connect(m_reply, &QNetworkReply::finished, this, &NetWorkDownload::onDownloadFinished);
    connect(m_reply, &QNetworkReply::downloadProgress,
        this,&NetWorkDownload::onDownloadProgress);

    m_url = url;
    m_filePath = fileName;

    return true;
}

bool NetWorkDownload::download()
{
    return download(m_url, m_filePath);
}

void NetWorkDownload::stop()
{
    if (m_reply) {
        m_reply->deleteLater();
        m_reply = nullptr;
    }

    if (m_file.isOpen()) {
        m_file.close();
        m_file.deleteLater();
    }
}

void NetWorkDownload::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (m_reply == nullptr) {
        return;
    }

    QByteArray data = m_reply->readAll();
    if (m_file.isOpen() && !data.isEmpty()) {
        int size = m_file.write(data);
        m_file.flush();
        if (size != data.size()) {
            emit downloadFinished(false, "文件写入失败:" + m_file.errorString());
            stop();
            return;
        }
    }

    //断点续传，读出请求头range范围
    QRegExp exp("bytes\\s(\\d*)");
    QString range = m_reply->rawHeader("Content-Range");
    qint64 size = 0;
    if (exp.indexIn(range) != -1) {
       size = exp.cap(1).toLongLong();
    }

    qDebug()<<range;
    qDebug()<<bytesReceived + size<<"     " << bytesTotal + size;
    emit downloadProgress(bytesReceived + size, bytesTotal + size);
}

void NetWorkDownload::onDownloadFinished()
{
    if (m_reply == nullptr) {
        return;
    }

    if (m_reply->error() != QNetworkReply::NoError) {
        emit downloadFinished(false, "下载失败");
        return;
    }

    QByteArray data = m_reply->readAll();
    if (m_file.isOpen() && !data.isEmpty()) {
        int size = m_file.write(data);
        m_file.flush();
        if (size != data.size()) {
            emit downloadFinished(false, "文件写入失败:" + m_file.errorString());
            stop();
            return;
        }
    }

    QDir dir;
    dir.remove(m_filePath);
    m_file.rename(m_filePath);
    m_file.close();

    m_reply = nullptr;

    emit downloadFinished(true, "");
}
