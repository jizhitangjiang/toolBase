#ifndef SETTING_H
#define SETTING_H

#include <QObject>
#include <QSettings>

/**
 * @class Setting
 * @brief 设置配置文件信息
 * @note set时,若key有分组，则需按照"group/key"形式
 */
class Setting : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param .ini配置文件路径
     * @param 父对象
     */
    explicit Setting(const QString& file, QObject *parent = 0);
    ~Setting();

public:
    /**
     * @brief 设置配置文件值
     * @param 键
     * @param 值
     * @param 是否加密，base64加密
     */
    void setValue(const QString &key, const QString &value,
        bool encryption = false);

    /**
     * @brief 获得对应键的值
     * @param 键
     * @param 该值是否加密
     * @return 返回值字符串
     */
    QString value(const QString &key, bool encryption = false);

    /**
     * @brief 获得对应键的值
     * @param 键
     * @param 默认值
     * @param 该值是否加密
     * @return 返回值字符串，若获得的值为空，返回默认值
     */
    QString value(const QString &key, const QString &def = "",
        bool encryption = false);

private:
    QSettings   m_setting;
};

#endif // SETTING_H
