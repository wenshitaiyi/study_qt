#include "app_settings.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>

AppSettings &AppSettings::instance()
{
    static AppSettings s_instance;
    return s_instance;
}

AppSettings::AppSettings()
{
    initSettings();
}

AppSettings::~AppSettings()
{
    if (m_settings) {
        m_settings->sync();
    }
}

void AppSettings::initSettings()
{
    // 将配置文件保存在应用程序运行目录下的 config/app_config.ini 中，便于观察与便携分发
    QString configDir = QCoreApplication::applicationDirPath() + QStringLiteral("/config");
    QDir().mkpath(configDir);
    m_configPath = configDir + QStringLiteral("/app_config.ini");

    m_settings = std::make_unique<QSettings>(m_configPath, QSettings::IniFormat);
    m_settings->setIniCodec("UTF-8");
}

// ============================================================================
// 1. 常规设置
// ============================================================================
QString AppSettings::theme() const
{
    return m_settings->value(QStringLiteral("General/theme"), QStringLiteral("Dark")).toString();
}

void AppSettings::setTheme(const QString &theme)
{
    m_settings->setValue(QStringLiteral("General/theme"), theme);
}

QString AppSettings::language() const
{
    return m_settings->value(QStringLiteral("General/language"), QStringLiteral("zh_CN")).toString();
}

void AppSettings::setLanguage(const QString &lang)
{
    m_settings->setValue(QStringLiteral("General/language"), lang);
}

bool AppSettings::autoStart() const
{
    return m_settings->value(QStringLiteral("General/autoStart"), false).toBool();
}

void AppSettings::setAutoStart(bool enable)
{
    m_settings->setValue(QStringLiteral("General/autoStart"), enable);
}

// ============================================================================
// 2. 网络配置
// ============================================================================
QString AppSettings::serverHost() const
{
    return m_settings->value(QStringLiteral("Network/serverHost"), QStringLiteral("127.0.0.1")).toString();
}

void AppSettings::setServerHost(const QString &host)
{
    m_settings->setValue(QStringLiteral("Network/serverHost"), host);
}

int AppSettings::serverPort() const
{
    return m_settings->value(QStringLiteral("Network/serverPort"), 8080).toInt();
}

void AppSettings::setServerPort(int port)
{
    m_settings->setValue(QStringLiteral("Network/serverPort"), port);
}

int AppSettings::timeoutMs() const
{
    return m_settings->value(QStringLiteral("Network/timeoutMs"), 5000).toInt();
}

void AppSettings::setTimeoutMs(int ms)
{
    m_settings->setValue(QStringLiteral("Network/timeoutMs"), ms);
}

// ============================================================================
// 3. 用户偏好
// ============================================================================
bool AppSettings::rememberPassword() const
{
    return m_settings->value(QStringLiteral("User/rememberPassword"), true).toBool();
}

void AppSettings::setRememberPassword(bool remember)
{
    m_settings->setValue(QStringLiteral("User/rememberPassword"), remember);
}

QString AppSettings::lastUsername() const
{
    return m_settings->value(QStringLiteral("User/lastUsername"), QStringLiteral("admin_developer")).toString();
}

void AppSettings::setLastUsername(const QString &username)
{
    m_settings->setValue(QStringLiteral("User/lastUsername"), username);
}

// ============================================================================
// 4. 窗口几何状态
// ============================================================================
QByteArray AppSettings::windowGeometry() const
{
    return m_settings->value(QStringLiteral("Window/geometry")).toByteArray();
}

void AppSettings::setWindowGeometry(const QByteArray &geometry)
{
    m_settings->setValue(QStringLiteral("Window/geometry"), geometry);
}

QByteArray AppSettings::windowState() const
{
    return m_settings->value(QStringLiteral("Window/state")).toByteArray();
}

void AppSettings::setWindowState(const QByteArray &state)
{
    m_settings->setValue(QStringLiteral("Window/state"), state);
}

// ============================================================================
// 5. 辅助方法
// ============================================================================
QString AppSettings::iniFilePath() const
{
    return m_configPath;
}

QString AppSettings::rawIniContent() const
{
    QFile file(m_configPath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        return in.readAll();
    }
    return QStringLiteral("(暂无配置文件内容，点击保存后生成)");
}

void AppSettings::resetToDefaults()
{
    m_settings->clear();
    setTheme(QStringLiteral("Dark"));
    setLanguage(QStringLiteral("zh_CN"));
    setAutoStart(false);
    setServerHost(QStringLiteral("127.0.0.1"));
    setServerPort(8080);
    setTimeoutMs(5000);
    setRememberPassword(true);
    setLastUsername(QStringLiteral("admin_developer"));
    m_settings->sync();
}

void AppSettings::sync()
{
    if (m_settings) {
        m_settings->sync();
    }
}
