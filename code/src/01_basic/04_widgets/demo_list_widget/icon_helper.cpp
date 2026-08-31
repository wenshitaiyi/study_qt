#include "icon_helper.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QPainter>
#include <QLinearGradient>

static const QStringList s_symbols = {
    QStringLiteral("🗡️"), QStringLiteral("🛡️"), QStringLiteral("🏹"), QStringLiteral("⚔️"), QStringLiteral("🔮"),
    QStringLiteral("👑"), QStringLiteral("💍"), QStringLiteral("💎"), QStringLiteral("📜"), QStringLiteral("🧪"),
    QStringLiteral("🪓"), QStringLiteral("👢"), QStringLiteral("🧤"), QStringLiteral("🗝️"), QStringLiteral("🔱"),
    QStringLiteral("🪄"), QStringLiteral("🔥"), QStringLiteral("⚡"), QStringLiteral("❄️"), QStringLiteral("🌟"),
    QStringLiteral("🪙"), QStringLiteral("🩸"), QStringLiteral("🍖"), QStringLiteral("🦅"), QStringLiteral("🐉"),
    QStringLiteral("🎖️"), QStringLiteral("📿"), QStringLiteral("🔔"), QStringLiteral("🪶"), QStringLiteral("🌪️"),
    QStringLiteral("💫"), QStringLiteral("🪐"), QStringLiteral("⚓"), QStringLiteral("🧭"), QStringLiteral("🎯"),
    QStringLiteral("🧿"), QStringLiteral("🎪"), QStringLiteral("🎨"), QStringLiteral("🎭"), QStringLiteral("🏆"),
    QStringLiteral("🥇"), QStringLiteral("🎫"), QStringLiteral("🪃"), QStringLiteral("🧩"), QStringLiteral("🪬"),
    QStringLiteral("🎇"), QStringLiteral("🎆"), QStringLiteral("✨"), QStringLiteral("🎉"), QStringLiteral("📦")
};

QString IconHelper::getIconsDir()
{
    QString appDir = QCoreApplication::applicationDirPath() + QStringLiteral("/data/icons");
    return appDir;
}

QPixmap IconHelper::drawIcon(int index, int size)
{
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);

    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing);

    // 5 种品质主题色 (普通绿, 精良蓝, 史诗紫, 传说金, 不朽红)
    struct Theme {
        QColor top;
        QColor bot;
        QColor border;
    } themes[] = {
        { QColor(46, 204, 113), QColor(39, 174, 96),   QColor(230, 245, 230) }, // 绿色
        { QColor(52, 152, 219), QColor(41, 128, 185),  QColor(210, 235, 255) }, // 蓝色
        { QColor(155, 89, 182), QColor(142, 68, 173), QColor(245, 225, 255) }, // 紫色
        { QColor(241, 196, 15), QColor(230, 126, 34),  QColor(255, 250, 200) }, // 金色
        { QColor(231, 76, 60),  QColor(192, 57, 43),   QColor(255, 225, 225) }  // 红色
    };

    int themeIdx = (index - 1) % 5;
    const Theme &theme = themes[themeIdx];

    // 绘制圆角渐变底板
    QRectF rect(2, 2, size - 4, size - 4);
    QLinearGradient grad(rect.topLeft(), rect.bottomRight());
    grad.setColorAt(0.0, theme.top);
    grad.setColorAt(1.0, theme.bot);

    painter.setBrush(grad);
    painter.setPen(QPen(theme.border, 2.0));
    painter.drawRoundedRect(rect, 8.0, 8.0);

    // 绘制内层高光光圈
    painter.setPen(QPen(QColor(255, 255, 255, 70), 1.0));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(rect.adjusted(2, 2, -2, -2), 6.0, 6.0);

    // 绘制中央装备图标符号
    int symbolIdx = (index - 1) % s_symbols.size();
    QString symbol = s_symbols[symbolIdx];

    QFont font = painter.font();
    font.setPointSize(size / 3 + 2);
    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(rect, Qt::AlignCenter, symbol);

    return pix;
}

void IconHelper::ensureIconsExist()
{
    const QStringList targetDirs = {
        QCoreApplication::applicationDirPath() + QStringLiteral("/data/icons"),
        QDir::currentPath() + QStringLiteral("/data/icons"),
        QDir::currentPath() + QStringLiteral("/app/data/icons"),
        QDir::currentPath() + QStringLiteral("/app/debug/data/icons"),
        QDir::currentPath() + QStringLiteral("/app/release/data/icons")
    };

    for (const QString &dirPath : targetDirs) {
        QDir dir(dirPath);
        if (!dir.exists()) {
            dir.mkpath(QStringLiteral("."));
        }

        // 检查并生成 50 个 64x64 图标
        for (int i = 1; i <= 50; ++i) {
            QString fileName = QStringLiteral("icon_%1.png").arg(i, 2, 10, QChar('0'));
            QString fullPath = dir.filePath(fileName);
            if (!QFileInfo::exists(fullPath)) {
                QPixmap pix = drawIcon(i, 64);
                pix.save(fullPath, "PNG");
            }
        }
    }
}

QIcon IconHelper::getIcon(int index)
{
    QString fileName = QStringLiteral("icon_%1.png").arg(index, 2, 10, QChar('0'));
    QString localPath = getIconsDir() + QStringLiteral("/") + fileName;

    if (QFileInfo::exists(localPath)) {
        return QIcon(localPath);
    }

    // 若本地文件未就绪，直接动态用 QPainter 绘制生成
    return QIcon(drawIcon(index, 64));
}
