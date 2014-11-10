#ifndef HEXVIEWER_H
#define HEXVIEWER_H

#include <QtCore>
#include "xbytearray.h"

class HexViewer : public QScrollArea
{
    QOBJECT

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

    void SetCursorPosition(int position);
    int GetCursorPosition(void);

signals:
    void CurrentAddressChanged(int address);
    void DataChanged(void);
protected:
    void keyPressEvent(QKeyEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);
    int CursorPosition(QPoint pos);
    void ResetSelection(int pos);
    void ResetSelection(void);
    void SetSelection(int pos);
    int GetSelectionBegin(void);
    int GetSelectionEnd(void);
private slots:
    void UpdateCursor(void);
private:
    QByteArray bytedata;
    XByteArray xdata;
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

    void EnsureVisible(void);

};

#endif // HEXVIEWER_H
