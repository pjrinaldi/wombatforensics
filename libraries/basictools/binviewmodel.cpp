    #include "binviewmodel.h"

qint64 BinViewModel::read(qint64 first)
{
    if (first < 0)
    {
        first = 0;
    }

    if (!m_file.seek(first))
    {
        m_buffer = QByteArray();
    }
    else
    {
        m_buffer = m_file.read(m_maxBufferSize);
    }

    m_bufferFirst = first;
    m_bufferLast = m_bufferFirst + m_buffer.length() - 1;

    return first;
}

BinViewModel::BinViewModel()
{
    m_fileSize = -1;
}

BinViewModel::~BinViewModel()
{
    close();
}

bool BinViewModel::open(const QString& fileName)
{
    m_file.setFileName(fileName);
    if (!m_file.open(QIODevice::ReadOnly | QIODevice::Unbuffered))
    {
        qWarning("Error %i: %s", m_file.error(), m_file.errorString().toUtf8().constData());
        m_errorString = m_file.errorString();
        return false;
    }

    m_fileSize = m_file.size();

    m_maxBufferSize = qMin<qint64>(m_fileSize, 65536);

    read(0);

    return true;
}

void BinViewModel::close()
{
    if (m_file.isOpen())
    {
        m_file.close();
    }

    m_fileSize = -1;
}

qint64 BinViewModel::size() const
{
    return m_fileSize;
}

QByteArray BinViewModel::read(qint64 first, int length)
{
    if ((m_fileSize <= 0) || (first >= m_fileSize))
    {
        return QByteArray();
    }

    if (first < m_bufferFirst)
    {
        read(first - m_maxBufferSize * 3 / 4);
    }
    else
    {
        qint64 last = qMin(first + length, m_fileSize) - 1;

        if (m_bufferLast < last)
        {
            read(first - m_maxBufferSize / 4);
        }
    }

    return m_buffer.mid(first - m_bufferFirst, length);
}

QString BinViewModel::errorString() const
{
    return m_file.errorString();
}
