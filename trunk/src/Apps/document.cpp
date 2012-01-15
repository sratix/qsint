#include "document.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include <QDebug>


namespace QSint
{


Document::Document(QObject *parent) :
    QObject(parent)
{
}


Document::Document(const QString& defaultName, QObject *parent) :
    m_name(defaultName),
    QObject(parent)
{
}


void Document::updateAfterLoad(const QString& fileName)
{
    m_name = QFileInfo(fileName).fileName();
    m_path = fileName;
    m_modified = false;
}


// static
bool Document::readFromFile(const QString& fileName, QString& text)
{
    QFile file(fileName);

    if (!file.open(QFile::ReadOnly))
        return false;

    QTextStream ts(&file);
    text = ts.readAll();
    file.close();

    //qDebug() << "Document::readFromFile ok";

    return true;
}


}
