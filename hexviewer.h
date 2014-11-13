#ifndef HEXVIEWER_H
#define HEXVIEWER_H

#include "xbytearray.h"
#include <QtGui>
#include <QtWidgets>

class HexViewer : public QWidget 
{
    Q_OBJECT

public:
    HexViewer(QWidget* parent = 0, TskObject* tskobject = NULL);
    ~HexViewer();
    int AddressOffset(void);
    void SetAddressOffset(int offset);

    int AddressWidth(void);
    void SetAddressWidth(int width);

    QByteArray Data(void);
    void SetData(QByteArray const &data);

    int RealAddressNumbers(void);
    int Size(void);

    QChar AsciiChar(int index);

    void SetCursorPosition(int position);
    int GetCursorPosition(void);

    XByteArray& XData(void);

    bool OpenImage();
    bool CloseImage();
    size_t ReadImage(size_t numbytes); // here is where i need to map the QFileDevice (as in # of pages)
    bool LoadImagePage(off_t pageindex);
    off_t SeekImage(off_t offset);

signals:
    void CurrentAddressChanged(int address);
    void DataChanged(void);
    void StepValues(int singlestep, int pagestep);
    void SetRange(off_t low, off_t high);
protected:
    void keyPressEvent(QKeyEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);
    int CursorPosition(QPoint pos);
    int CursorPosition(void);
    void ResetSelection(int pos);
    void ResetSelection(void);
    void SetSelection(int pos);
    int GetSelectionBegin(void);
    int GetSelectionEnd(void);

    TskObject* tskptr;

private slots:
    void UpdateCursor(void);
    void AdjustData(int offset);
private:
    XByteArray xdata;
    QColor addressareacolor;
    QColor highlightcolor;
    QColor selectioncolor;
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
    int selectioninit;
    int size;

    void EnsureVisible(void);
    void Adjust(void);

    int currentoffset;
    int firstoffset;
    int lastoffset;
};

#endif // HEXVIEWER_H
