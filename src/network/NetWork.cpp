#include "NetWork.h"
#include "QUrlQuery"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMutexLocker>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

QNetworkAccessManager NetWork::m_manager;

NetWork::NetWork(QObject *parent)
    : QObject(parent)
    , m_requestId(0)
{
}

NetWork::~NetWork()
{
    clear();
}

int NetWork::post(const QString &url, const QList<NArg> &args)
{
    if (url.isEmpty()) {
        return -1;
    }

   return appendRequest(requestType::Post, url, parseArgs(args));
}

int NetWork::get(const QString &url)
{
    if (url.isEmpty()) {
        return -1;
    }

    return appendRequest(requestType::Get, url);
}

void NetWork::registerInvokeFunc(int rid, NPair callback)
{
    if (rid < 0) {
        return;
    }

    RequestData *reqData = findRequestData(rid);
    if (reqData) {
        reqData->m_callBack = callback;
    }
}

int NetWork::appendRequest(NetWork::requestType type, const QString &url, const QByteArray &data)
{
    QUrl u(url);
    if (!u.isValid()) {
        return-1;
    }

    RequestData *reqData = new RequestData;
    reqData->m_url = u;
    reqData->m_type = type;
    reqData->m_data = data;

    QMutexLocker lock(&m_mutex);
    int rid = ++m_requestId;
    reqData->m_rid = rid;

    m_ready.enqueue(reqData);

    QTimer::singleShot(5, this, &NetWork::onHttpRequest);

    return rid;
}

RequestData *NetWork::peekRequest()
{
    QMutexLocker lock(&m_mutex);

    if (m_ready.isEmpty()) {
        return nullptr;
    }

    return m_ready.dequeue();
}

QByteArray NetWork::parseArgs(const QList<NArg> &args)
{
    QByteArray data;

    if (!args.isEmpty()) {
        QUrlQuery query;   
        for (int i = 0, s = args.size(); i < s; i++) {
            QString key = args.at(i).first;
            QString value = args.at(i).second.toString();
            query.addQueryItem(key, value);
        }

        data = query.toString().toUtf8();
    }

    return data;
}

void NetWork::clear()
{
    while (!m_ready.isEmpty()) {
        delete m_ready.dequeue();
    }

    QList<int> keys = m_running.keys();
    for (auto key : keys) {
        delete m_running.take(key);
    }
}

RequestData *NetWork::findRequestData(int rid)
{
    QMutexLocker lock(&m_mutex);

    if (m_ready.isEmpty()) {
        return nullptr;
    }

    RequestData *first = m_ready.first();
    int index = rid - first->m_rid;

    if (index < 0 || index >= m_ready.size()) {
        return nullptr;
    }

    RequestData *reqData = m_ready[index];
    if (reqData == nullptr || reqData->m_rid != rid) {
        return nullptr;
    }

    return reqData;
}

void NetWork::onHttpRequest()
{
    RequestData *reqData = peekRequest();
    if (reqData == nullptr) {
        return;
    }

    QNetworkRequest req(reqData->m_url);
    req.setHeader(QNetworkRequest::ContentTypeHeader,
         "application/x-www-form-urlencoded");
    //req.setHeader(QNetworkRequest::UserAgentHeader, "");
    QNetworkReply *reply = nullptr;

    switch (reqData->m_type) {
    case requestType::Post:
        reply = m_manager.post(req, reqData->m_data);
        break;
    case requestType::Get:
        reply = m_manager.get(req);
        break;
    default:
        break;
    }

    if (reply != nullptr) {
        QMutexLocker lock(&m_mutex);
        connect(reply, &QNetworkReply::finished, this, &NetWork::onHttpReply);
        reply->setProperty("requestId", reqData->m_rid);
        m_running.insert(reqData->m_rid, reqData);
    } else {
        QMetaObject::invokeMethod(reqData->m_callBack.first, reqData->m_callBack.second,
            Q_ARG(bool, false), Q_ARG(QString, "请求失败"), Q_ARG(QJsonObject, QJsonObject()));

        delete reqData;
        reqData = nullptr;
    }

    QTimer::singleShot(10, this, &NetWork::onHttpRequest);

    return;
}

void NetWork::onHttpReply()
{
    QNetworkReply *reply = dynamic_cast<QNetworkReply*>(sender());

    if (reply) {
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();

        int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        bool stat = false;
        QString msg;
        QJsonObject value;
        if (code == 200 && reply->error() == QNetworkReply::NoError) {
            if (obj.contains("status")) {
                stat = obj.value("status").toBool();
            } else {
                stat = (obj.value("stat").toInt() == 1);
            }
            msg = obj.value("msg").toString();
            value = obj.value("data").toObject();
        } else {
            msg = reply->errorString();
        }

        int rid = reply->property("requestId").toInt();
        if (m_running.contains(rid)) {
            QMutexLocker lock(&m_mutex);
            RequestData *reqData = m_running.value(rid);
            if (reqData != nullptr) {
                QMetaObject::invokeMethod(reqData->m_callBack.first, reqData->m_callBack.second,
                    Q_ARG(bool, stat), Q_ARG(QString, msg), Q_ARG(QJsonObject, value));

                delete reqData;
                reqData = nullptr;
            } 
           m_running.remove(rid);
        }

        reply->deleteLater();
    }
}
