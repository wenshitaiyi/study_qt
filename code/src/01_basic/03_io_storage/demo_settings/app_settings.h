#pragma once

#include <QString>
#include <QByteArray>
#include <QSettings>
#include <memory>

class AppSettings
{
public:
    // 单例访问
    static AppSettings &instance();

    // 禁用拷贝与移动
    AppSettings(const AppSettings &) = delete;
    AppSettings &operator=(const AppSettings &) = delete;

    // ========================================================================
    // 1. 常规设置 (General)
    // ========================================================================
    QString theme() const;
    void setTheme(const QString &theme);

    QString language() const;
    void setLanguage(const QString &lang);

    bool autoStart() const;
    void setAutoStart(bool enable);

    // ========================================================================
    // 2. 网络配置 (Network)
    // ========================================================================
    QString serverHost() const;
    void setServerHost(const QString &host);

    int serverPort() const;
    void setServerPort(int port);

    int timeoutMs() const;
    void setTimeoutMs(int ms);

    // ========================================================================
    // 3. 用户偏好 (User)
    // ========================================================================
    bool rememberPassword() const;
    void setRememberPassword(bool remember);

    QString lastUsername() const;
    void setLastUsername(const QString &username);

    // ========================================================================
    // 4. 窗口几何状态 (Window)
    // ========================================================================
    QByteArray windowGeometry() const;
    void setWindowGeometry(const QByteArray &geometry);

    QByteArray windowState() const;
    void setWindowState(const QByteArray &state);

    // ========================================================================
    // 5. 辅助与管理方法
    // ========================================================================
    QString iniFilePath() const;
    QString rawIniContent() const;
    void resetToDefaults();
    void sync();

private:
    AppSettings();
    ~AppSettings();

    void initSettings();

    std::unique_ptr<QSettings> m_settings;
    QString m_configPath;
};
