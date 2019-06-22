#include "Setting.h"

Setting::Setting(const QString& file, QObject *parent)
    : QObject(parent)
    , m_setting(file, QSettings::IniFormat)
{

}

Setting::~Setting()
{

}

void Setting::setValue(const QString &key, const QString &value, bool encryption)
{
    if (key.isEmpty()) {
        return;
    }

    QString val = value;
    if (encryption) {
        val = value.toUtf8().toBase64();
    }

    m_setting.setValue(key, val);
}

QString Setting::value(const QString &key, bool encryption)
{
    if (key.isEmpty()) {
        return "";
    }

    QString val = m_setting.value(key).toString();
    if (encryption) {
        val = QByteArray::fromBase64(val.toUtf8());
    }

    return val;
}

QString Setting::value(const QString &key, const QString &def, bool encryption)
{
    QString val = value(key, encryption);

    if (val.isEmpty()) {
        return def;
    }

    return val;
}
