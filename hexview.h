#ifndef HEXVIEW_H 
#define HEXVIEW_H

#include <QPlainTextEdit>
#include <QObject>

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

class AddressArea;


class HexView : public QPlainTextEdit
{
    Q_OBJECT

public:
    HexView(QWidget *parent = 0);

    void AddressAreaPaintEvent(QPaintEvent *event);
    int AddressAreaWidth();
    bool showaddressarea;

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void UpdateAddressAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void UpdateAddressArea(const QRect &, int);

private:
    QWidget *addressarea;
};


class AddressArea : public QWidget
{
public:
    AddressArea(HexView* editor) : QWidget(editor) {
        hexview = editor;
    }

    QSize sizeHint() const {
        return QSize(hexview->AddressAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) {
        hexview->AddressAreaPaintEvent(event);
    }

private:
    HexView *hexview;
};

#endif
