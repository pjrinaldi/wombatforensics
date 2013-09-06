#ifndef BINVIEWMODEL_H
#define BINVIEWMODEL_H

#include <QFile>
#include <QByteArray>

class BinViewModel
{
    QFile m_file;
    qint64 m_fileSize;

    QByteArray m_buffer;
    qint64 m_bufferFirst;
    qint64 m_bufferLast;

    int m_maxBufferSize;

    QString m_errorString;

    qint64 read(qint64 first);

public:

    BinViewModel();

    ~BinViewModel();

    bool open(const QString& fileName);

    void close();

    qint64 size() const;

    QByteArray read(qint64 first, int length);

    QString errorString() const;
};

#endif // BINVIEWMODEL_H
