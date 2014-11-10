#ifndef HEXVIEWER_H
#define HEXVIEWER_H

#include <QtCore>

class HexViewer : public QScrollArea
{
public:
    int AddressOffset(void);
    void SetAddressOffset(int offset);

    int AddressWidth(void);
    void SetAddressWidth(int width);

    QByteArray& Data(void);
    void SetData(QByteArray data);

    int RealAddressNumbers(void);
    int Size(void);

    QChar AsciiChar(int index);

signals:
    void CurrentAddressChanged(int address);
    void DataChanged(void);
protected:
    void keyPressEvent(QKeyEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);
    int CursorPosition(QPoint pos);
private slots:
    void UpdateCursor(void);
private:
    QByteArray data;
    int addressnumbers;
    int addressoffset;
    int realaddressnumbers;
    QScrollArea* scrollarea;
    QHBoxLayout* layout;
    int charwidth, charheight;
    int cursorx, cursory;
    int cursorposition;
    int labelposition, hexposition, asciiposition;
    int selectionstart;
    int selectionend;
    int selectioninitial;

};

#endif // HEXVIEWER_H
