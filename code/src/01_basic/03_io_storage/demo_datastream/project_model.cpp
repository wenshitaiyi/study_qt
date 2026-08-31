#include "project_model.h"

CadProject::CadProject(const QString &name, const QString &author)
    : m_projectName(name)
    , m_author(author)
    , m_createTime(QDateTime::currentDateTime())
{
}

// 序列化 GraphicShape
QDataStream &operator<<(QDataStream &out, const GraphicShape &shape)
{
    out << static_cast<qint32>(shape.shapeType)
        << shape.label
        << shape.boundingBox
        << shape.fillColor;
    return out;
}

// 反序列化 GraphicShape
QDataStream &operator>>(QDataStream &in, GraphicShape &shape)
{
    qint32 type;
    in >> type >> shape.label >> shape.boundingBox >> shape.fillColor;
    shape.shapeType = static_cast<int>(type);
    return in;
}

// 序列化 CadProject
QDataStream &operator<<(QDataStream &out, const CadProject &proj)
{
    // 1. 写入魔数与格式版本
    out << CadProject::MAGIC_HEADER;
    out << CadProject::CURRENT_VERSION;

    // 2. 写入工程元数据与图元数组
    out << proj.m_projectName;
    out << proj.m_author;
    out << proj.m_createTime;
    out << proj.m_shapes;

    return out;
}

// 反序列化 CadProject (支持向后兼容版本读取)
QDataStream &operator>>(QDataStream &in, CadProject &proj)
{
    // 1. 校验魔数
    quint32 magic = 0;
    in >> magic;
    if (magic != CadProject::MAGIC_HEADER) {
        in.setStatus(QDataStream::ReadCorruptData);
        return in;
    }

    // 2. 读取版本号
    quint32 version = 0;
    in >> version;

    // 3. 读取元数据
    in >> proj.m_projectName;
    in >> proj.m_author;
    in >> proj.m_createTime;

    // 4. 根据版本兼容读取图元
    if (version == 1) {
        // 假设 v1 没有 fillColor，这里做版本降级兼容处理
        proj.m_shapes.clear();
        qint32 shapeCount = 0;
        in >> shapeCount;
        for (qint32 i = 0; i < shapeCount; ++i) {
            GraphicShape s;
            qint32 type;
            in >> type >> s.label >> s.boundingBox;
            s.shapeType = static_cast<int>(type);
            s.fillColor = Qt::white; // 赋予 v1 默认颜色
            proj.m_shapes.append(s);
        }
    } else {
        // v2 直接读取完整图元列表
        in >> proj.m_shapes;
    }

    return in;
}
