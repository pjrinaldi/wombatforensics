#ifndef Q_DEBUGSTREAM_H
#define Q_DEBUGSTREAM_H

#include <iostream>
#include <streambuf>
#include <string>
#include <QTextEdit>

/*
 * HOW TO USE IN CODE
 *
 * new Q_DebugStream(std::cout, ui->TXT_Console); // redirect console output to QTextEdit
 * Q_DebugStream::registerQDebugMessageHandler(); // redirect qDebug() output to QTextEdit
 *
 */ 
class Q_DebugStream : public std::basic_streambuf<char>
{
public:
    Q_DebugStream(std::ostream &stream, QTextEdit* text_edit) : m_stream(stream)
    {
        log_window = text_edit;
        m_old_buf = stream.rdbuf();
        stream.rdbuf(this);
    }

    ~Q_DebugStream()
    {
        m_stream.rdbuf(m_old_buf);
    }

    static void registerQDebugMessageHandler(){
        qInstallMessageHandler(myQDebugMessageHandler);
    }

private:

    static void myQDebugMessageHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
    {
        std::cout << msg.toStdString().c_str();
    }

protected:

    //This is called when a std::endl has been inserted into the stream
    virtual int_type overflow(int_type v)
    {
        if (v == '\n')
        {
            log_window->append("\n");
        }
        return v;
    }


    virtual std::streamsize xsputn(const char *p, std::streamsize n)
    {
        QString str(p);
        if(str.contains("\n")){
            QStringList strSplitted = str.split("\n");

            log_window->moveCursor (QTextCursor::End);
            log_window->insertPlainText (strSplitted.at(0)); //Index 0 is still on the same old line

            for(int i = 1; i < strSplitted.size(); i++){
                log_window->append(strSplitted.at(i));
            }
        }else{
            log_window->moveCursor (QTextCursor::End);
            log_window->insertPlainText (str);
        }
        return n;
    }

private:
    std::ostream &m_stream;
    std::streambuf *m_old_buf;
    QTextEdit* log_window;
};

Q_DECLARE_METATYPE(QTextCursor);
Q_DECLARE_METATYPE(QTextBlock);

#endif // Q_DEBUGSTREAM_H
