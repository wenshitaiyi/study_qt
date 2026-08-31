#include "url_demo.h"

#include <iostream>
#include <iomanip>

static void printTitle(const QString &title)
{
    std::cout << "\n============================================================" << std::endl;
    std::cout << "  " << title.toUtf8().constData() << std::endl;
    std::cout << "============================================================" << std::endl;
}

static void printLine(const QString &title, const QString &content)
{
    std::cout << QStringLiteral("  • %1 : %2")
                    .arg(title.leftJustified(28, ' '))
                    .arg(content)
                    .toUtf8().constData()
              << std::endl;
}

// 演示 RESTful API 请求 URL 建造者模式 (UrlBuilder)
class UrlBuilder
{
public:
    UrlBuilder &setScheme(const QString &scheme) { m_url.setScheme(scheme); return *this; }
    UrlBuilder &setHost(const QString &host) { m_url.setHost(host); return *this; }
    UrlBuilder &setPort(int port) { m_url.setPort(port); return *this; }
    UrlBuilder &setPath(const QString &path) { m_url.setPath(path); return *this; }
    UrlBuilder &addParam(const QString &key, const QString &value)
    {
        m_query.addQueryItem(key, value);
        return *this;
    }
    UrlBuilder &addParam(const QString &key, int value)
    {
        m_query.addQueryItem(key, QString::number(value));
        return *this;
    }

    QUrl build()
    {
        QUrl result = m_url;
        result.setQuery(m_query);
        return result;
    }

private:
    QUrl m_url;
    QUrlQuery m_query;
};

void UrlDemo::testUrlComponents()
{
    printTitle(QStringLiteral("[模块 1] QUrl 基础解析与组成部分提取"));

    QString rawUrl = QStringLiteral("https://admin:secret123@api.studyqt.org:8443/v2/modules/core_types/data.json?env=prod&debug=true#section-metrics");
    QUrl url(rawUrl);

    printLine(QStringLiteral("原始完整 URL"), rawUrl);
    printLine(QStringLiteral("URL 是否有效 isValid()"), url.isValid() ? QStringLiteral("有效 (Valid)") : QStringLiteral("无效 (Invalid)"));
    printLine(QStringLiteral("协议 (Scheme)"), url.scheme());
    printLine(QStringLiteral("主机域名 (Host)"), url.host());
    printLine(QStringLiteral("端口号 (Port)"), QString::number(url.port(443)));
    printLine(QStringLiteral("用户信息 (User:Password)"), QStringLiteral("%1 : %2").arg(url.userName(), url.password()));
    printLine(QStringLiteral("路径 (Path)"), url.path());
    printLine(QStringLiteral("文件名 (FileName)"), url.fileName());
    printLine(QStringLiteral("锚点片段 (Fragment)"), url.fragment());
    printLine(QStringLiteral("是否本地文件 isLocalFile()"), url.isLocalFile() ? QStringLiteral("是 (True)") : QStringLiteral("否 (False)"));
}

void UrlDemo::testUrlQuery()
{
    printTitle(QStringLiteral("[模块 2] QUrlQuery 查询参数键值对解析与动态修改"));

    QUrl url(QStringLiteral("https://api.studyqt.org/v1/search?category=hardware&page=1&limit=20&sort=desc"));
    printLine(QStringLiteral("解析前 URL"), url.toString());

    // 1. 从 URL 提取 QUrlQuery
    QUrlQuery query(url);

    printLine(QStringLiteral("是否存在 'page' 参数"), query.hasQueryItem(QStringLiteral("page")) ? QStringLiteral("存在 (True)") : QStringLiteral("不存在 (False)"));
    printLine(QStringLiteral("获取 'limit' 参数值"), query.queryItemValue(QStringLiteral("limit")));

    std::cout << "  遍历所有查询参数项 (Key -> Value) :" << std::endl;
    QList<QPair<QString, QString>> items = query.queryItems();
    for (const auto &pair : items) {
        std::cout << QStringLiteral("    • %1 = %2").arg(pair.first.leftJustified(15, ' '), pair.second).toUtf8().constData() << std::endl;
    }

    // 2. 动态修改与追加参数
    query.removeQueryItem(QStringLiteral("sort"));                    // 移除参数
    query.addQueryItem(QStringLiteral("keyword"), QStringLiteral("Qt5")); // 追加新参数
    query.addQueryItem(QStringLiteral("page"), QStringLiteral("2"));      // 修改页码为 2 (覆盖前需移除或直接重组)

    // 重新写回 QUrl
    url.setQuery(query);
    printLine(QStringLiteral("动态更新后 URL"), url.toString());
}

void UrlDemo::testLocalFileAndRelativePath()
{
    printTitle(QStringLiteral("[模块 3] 本地路径与 file:/// 协议互转及相对路径解析"));

    // 1. 本地 Windows 路径转 file:/// 协议 URL
    QString windowsPath = QStringLiteral("D:/zcode/study/study_qt/app/debug/config/settings data.json");
    QUrl fileUrl = QUrl::fromLocalFile(windowsPath);

    printLine(QStringLiteral("本地 Windows 路径"), windowsPath);
    printLine(QStringLiteral("fromLocalFile 转为 URI"), fileUrl.toString());
    printLine(QStringLiteral("toLocalFile 还原路径"), fileUrl.toLocalFile());

    // 2. 相对路径基准解析 (resolved)
    QUrl baseUrl(QStringLiteral("https://doc.qt.io/qt-5/qurl.html"));
    QUrl relativeUrl(QStringLiteral("qurlquery.html"));
    QUrl resolvedUrl = baseUrl.resolved(relativeUrl);

    printLine(QStringLiteral("基准 Base URL"), baseUrl.toString());
    printLine(QStringLiteral("相对 Relative URL"), relativeUrl.toString());
    printLine(QStringLiteral("解析合并后的绝对 URL"), resolvedUrl.toString());
}

void UrlDemo::testPercentEncoding()
{
    printTitle(QStringLiteral("[模块 4] Percent Encoding 百分号编码与特殊字符转义"));

    QString rawChinese = QStringLiteral("Qt 5.15 & 工业互联网 架构设计 (v1.0)");
    
    // 1. 百分号编码 (Percent Encoding)
    QByteArray percentEncoded = QUrl::toPercentEncoding(rawChinese);
    printLine(QStringLiteral("原始中文字符串"), rawChinese);
    printLine(QStringLiteral("Percent 百分号编码结果"), QString::fromLatin1(percentEncoded));

    // 2. 百分号解码还原
    QString decodedStr = QUrl::fromPercentEncoding(percentEncoded);
    printLine(QStringLiteral("百分号解码还原结果"), decodedStr);

    // 3. FullyEncoded vs PrettyDecoded 对比
    QUrl testUrl(QStringLiteral("https://www.google.com/search?q=Qt开发"));
    printLine(QStringLiteral("FullyEncoded 编码全量传输"), testUrl.toString(QUrl::FullyEncoded));
    printLine(QStringLiteral("PrettyDecoded 人性化展示"), testUrl.toString(QUrl::PrettyDecoded));
}

void UrlDemo::testUrlBuilderPattern()
{
    printTitle(QStringLiteral("[模块 5] 实战：RESTful API 请求 URL 链式建造者"));

    // 使用链式 API 优雅构造复杂网络请求
    QUrl requestUrl = UrlBuilder()
        .setScheme(QStringLiteral("https"))
        .setHost(QStringLiteral("api.weather.com"))
        .setPort(443)
        .setPath(QStringLiteral("/v2/city/forecast"))
        .addParam(QStringLiteral("city"), QStringLiteral("Beijing"))
        .addParam(QStringLiteral("lang"), QStringLiteral("zh-CN"))
        .addParam(QStringLiteral("days"), 7)
        .addParam(QStringLiteral("apiKey"), QStringLiteral("AIzaSyQt515StudyKey"))
        .build();

    printLine(QStringLiteral("UrlBuilder 链式构建 URL"), requestUrl.toString(QUrl::PrettyDecoded));
    printLine(QStringLiteral("网络请求传输格式"), requestUrl.toString(QUrl::FullyEncoded));
}
