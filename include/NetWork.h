#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QQueue>
#include <QPair>
#include <QUrl>
#include <QMutex>
#include <QNetworkAccessManager>

#define CALLBACK(f) (##f)

using NMap = QMap<QString, QVariant>;
using NPair = QPair<QObject*, char*>;

struct RequestData
{
    int        m_rid;
    int        m_type;
    QUrl       m_url;
    QByteArray m_data;
    NPair      m_callBack;
};

class NetWork : public QObject
{
    Q_OBJECT
public:
    explicit NetWork(QObject *parent = 0);
    ~NetWork();

    enum requestType
    {
        Post,
        Get,
        Put,
        Head,
        Count
    };

public:
    int post(const QString &url, NMap args);
    int get(const QString &url);
    void registerInvokeFunc(int rid, NPair callback);

private:
    int appendRequest(requestType type, const QString &url, const QByteArray &data = "");
    RequestData* peekRequest();
    RequestData* findRequestData(int rid);
    QByteArray parseArgs(NMap args);
    void invokeMethod();
    void clear();

signals:

public slots:
    void onHttpRequest();
    void onHttpReply();

private:
    static QNetworkAccessManager    m_manager;
    QString                         m_userAgent;
    int                             m_requestId;
    QMutex                          m_mutex;
    QQueue<RequestData*>            m_ready;
    QMap<int, RequestData*>         m_running;
};

#endif // NETWORK_H
