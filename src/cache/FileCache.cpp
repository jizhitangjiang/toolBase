#include "FileCache.h"

#include <QCryptographicHash>
#include <QDir>
#include <QFile>
#include <QString>

FileCache::FileCache(const QString &tempRoot)
{
    setTempRoot(tempRoot);
}

FileCache::~FileCache()
{

}

void FileCache::setTempRoot(QString path)
{
    if (path.isEmpty()) {
        return;
    }

    QString temp = path.replace('\\', '/');
    if (!temp.endsWith("/")) {
        temp.append("/");
    }

    QDir dir;
    if (dir.mkpath(temp)) {
        m_tempRoot = temp;
    }
}

QString FileCache::cacheUrl(const QString &url)
{
    if (!isUrlValid(url)) {
        return "";
    }

    QString path = mapUrlToFile(url);
    if (path.isEmpty()) {
        return "";
    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return "";
    }

    file.close();

    return path;
}

QByteArray FileCache::cacheData(const QString &url)
{
    if (!isUrlValid(url)) {
        return "";
    }

    QString path = mapUrlToFile(url);
    if (path.isEmpty()) {
        return "";
    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return "";
    }

    QByteArray data = file.readAll();
    file.close();

    return data;
}

QString FileCache::setCache(const QString &url, const QString &filePath)
{
    QUrl u(url);
    QFile file(filePath);
    if (!u.isValid() || !file.exists()) {
        return "";
    }

    QString cachePath = setDir(url);
    QString fullPath = cachePath + u.fileName();
    if (!QFile::copy(filePath, fullPath + ".tmp")) {
        return "";
    }

    QDir dir;
    dir.remove(fullPath);
    dir.rename(fullPath + ".tmp", fullPath);

    return fullPath;
}

QString FileCache::setCache(const QString &url, const QByteArray &data)
{
    QUrl u(url);
    if (!u.isValid() || data.isEmpty()) {
        return "";
    }

    QString cachePath = setDir(url);
    QString fullPath = cachePath + u.fileName();
    QFile file(fullPath + ".tmp");
    if (!file.open(QIODevice::WriteOnly)) {
        return "";
    }

    file.write(data);
    file.close();

    QDir dir;
    dir.remove(fullPath);
    dir.rename(fullPath + ".tmp", fullPath);

    return fullPath;
}

QString FileCache::setDir(const QString &url)
{
    if (!isUrlValid(url)) {
        return "";
    }

    QString cachePath = mapUrlToDir(url);
    QDir dir;
    if (!dir.mkpath(cachePath)) {
        return "";
    }

    return cachePath;
}

void FileCache::removeCache(const QString &url)
{
    if (!isUrlValid(url)) {
        return;
    }

    QString filePath = mapUrlToFile(url);
    QFile file(filePath);

    if (file.exists()) {
        file.remove();
    }

    return;
}

QString FileCache::mapUrlToDir(const QString &url)
{
    QString lowerUrl = url.toLower();
    QByteArray md5 = QCryptographicHash::hash(lowerUrl.toLocal8Bit(),
        QCryptographicHash::Md5);

    QString hex = md5.toHex();
    QString cachePath = m_tempRoot +
            hex.mid(0, 1) + "/" +
            hex.mid(1, 2) + "/" +
            hex.mid(3, 4) + "/";

    return cachePath;
}

QString FileCache::mapUrlToFile(const QString &url)
{
    QUrl u(url);
    QString dir = mapUrlToDir(url);
    QString fileName = u.fileName();

    return dir + fileName;

}
