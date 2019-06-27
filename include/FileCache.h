#ifndef FILECACHE_H
#define FILECACHE_H

#include <QObject>

class FileCache : public QObject
{
    Q_OBJECT

public:
    explicit FileCache(const QString &tempRoot, QObject *parent = 0);
    ~FileCache();

public:
    /**
     * @brief setTempRoot  创建缓存根目录
     * @param path         根目录
     */
    void setTempRoot(QString path);

    /**
     * @brief cacheUrl      根据url获得缓存文件路径
     * @param url           url
     * @return              缓存文件路径
     */
    QString cacheUrl(const QString &url);

    /**
     * @brief cacheData     根据url获得缓存的数据
     * @param url           url
     * @return              文件数据
     */
    QByteArray cacheData(const QString &url);

    /**
     * @brief setCache      将本地文件复制到缓存目录中
     * @param url           url
     * @param filePath      本地文件路径
     * @return              缓存文件路径
     */
    QString setCache(const QString &url, const QString &filePath);

    /**
     * @brief setCache      将数据转换为文件进行缓存
     * @param url           url
     * @param data          文件数据
     * @return              缓存文件路径
     */
    QString setCache(const QString &url, const QByteArray &data);

    /**
     * @brief removeCache   删除指定url缓存的文件
     * @param url           url
     */
    void removeCache(const QString &url);

private:
    /**
     * @brief mapUrlToDir   将url映射到缓存路径(不包含文件名)
     * @param url           url
     * @return              缓存文件目录路径
     */
    QString mapUrlToDir(const QString &url);

    /**
     * @brief mapUrlToFile  将url映射到缓存文件(包含文件名)
     * @param url           url
     * @return              缓存文件路径
     */
    QString mapUrlToFile(const QString &url);

private:
    QString m_tempRoot;

};

#endif // FILECACHE_H
