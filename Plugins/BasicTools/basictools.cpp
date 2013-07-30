#include "basictools.h"

QStringList BasicTools::evidenceActions() const
{
    return QStringList() << tr("Add Evidence") << tr("Remove Evidence");
}

QStringList BasicTools::evidenceActionIcons() const
{
    return QStringList() << tr(":/basic/addevidence") << tr(":/basic/remevidence");
}

QStringList BasicTools::evidenceToolboxIcons() const
{
    return QStringList() << tr(":/basictools/images/treefile") << tr(":/basictools/images/treefolder") << (":/basictools/images/treepartition") << tr(":/basictools/images/treefilemanager") << tr(":/basictools/images/treeimage");
}

QStringList BasicTools::toolboxViews() const
{
    return QStringList() << tr("Directory Listing") << tr("File Signature Category");
}

void BasicTools::addEvidence(int currentCaseID)
{
    // add evidence here
    currentcaseid = currentCaseID;
    QString evidenceFile = QFileDialog::getOpenFileName(0, tr("Select Evidence Item"), tr("./"));
    if (evidenceFile != "")
    {
        QString evidenceFileName = evidenceFile.split("/").last();
        QString evidenceDBname = evidenceFileName + ".db";
        /*
        SqlWrapper *sqlObject = new SqlWrapper(sqlStatement, "3.1", "WombatData.db");
        sqlObject->PrepareSql("INSERT INTO caseimages (imagefullpath, imagename, caseid) VALUES(?, ?, ?);");
        sqlObject->BindValue(1, evidenceFile.toStdString().c_str());
        sqlObject->BindValue(2, evidenceFileName.toStdString().c_str());
        sqlObject->BindValue(3, currentcaseid);
        sqlObject->StepSql();
        sqlObject->FinalizeSql();
        //sqlObject->CloseSql();
        // set individual db's for each image...
        TskFunctions *testcase = new TskFunctions();
        testcase->InitializeFrameworkProperties();
        testcase->InitializeFrameworkLog();
        //testcase->SetupTskFramework();
        std::auto_ptr<TskImgDB> wImgDB(NULL);
        //wImgDB = std::auto_ptr<TskImgDB>(new WombatTskImgDBSqlite(evidenceDBname));
        wImgDB = std::auto_ptr<TskImgDB>(new TskImgDBSqlite("./testout"));
        if(wImgDB->initialize() != 0)
        {
            //LOGERROR("Error initializing the Image DB.");
        }

        wImgDB->open();

        TskServices::Instance().setImgDB(*wImgDB);
        TskServices::Instance().setBlackboard((TskBlackboard &) TskDBBlackboard::instance());
        TskSchedulerQueue scheduler;
        TskServices::Instance().setScheduler(scheduler);
        TskServices::Instance().setFileManager(TskFileManagerImpl::instance());
        //InitializeFrameworkBlackboard();
        //InitializeFrameworkScheduler();
        //InitializeFrameworkFileManager();

        TskImageFileTsk imageFileTsk;
        imageFileTsk.open(evidenceFile.toStdString().c_str()); // open evidence image: dd, e01, etc.
        TskServices::Instance().setImageFile(imageFileTsk);
        imageFileTsk.extractFiles();
        */
    }
}

void BasicTools::remEvidence(int currentCaseID)
{
    currentcaseid = currentCaseID;
    // remove evidence here
}
QWidget* BasicTools::setupToolBox() const
{
    QTreeView* directoryTreeView = new QTreeView();

    return directoryTreeView;
}

QWidget* BasicTools::setupColumnView() const
{
    QColumnView* directoryTreeColumnView = new QColumnView();

    return directoryTreeColumnView;
}

QWidget* BasicTools::setupTabWidget() const
{
    QTabWidget* directoryTreeTabWidget = new QTabWidget();
    // probably add the tabs

    return directoryTreeTabWidget;
}
/*
BasicTools::BasicTools(QObject *parent)
{
}
/*
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(BasicTools, BasicTools)
#endif // QT_VERSION < 0x050000
*/
/*
#include <QtWidgets>

#include <math.h>
#include <stdlib.h>

#include "basictoolsplugin.h"

const float Pi = 3.14159f;

QStringList BasicToolsPlugin::brushes() const
{
    return QStringList() << tr("Pencil") << tr("Air Brush")
                         << tr("Random Letters");
}

QRect BasicToolsPlugin::mousePress(const QString &brush, QPainter &painter,
                                   const QPoint &pos)
{
    return mouseMove(brush, painter, pos, pos);
}

QRect BasicToolsPlugin::mouseMove(const QString &brush, QPainter &painter,
                                  const QPoint &oldPos, const QPoint &newPos)
{
    painter.save();

    int rad = painter.pen().width() / 2;
    QRect boundingRect = QRect(oldPos, newPos).normalized()
                                              .adjusted(-rad, -rad, +rad, +rad);
    QColor color = painter.pen().color();
    int thickness = painter.pen().width();
    QColor transparentColor(color.red(), color.green(), color.blue(), 0);

    if (brush == tr("Pencil")) {
        painter.drawLine(oldPos, newPos);
    } else if (brush == tr("Air Brush")) {
        int numSteps = 2 + (newPos - oldPos).manhattanLength() / 2;

        painter.setBrush(QBrush(color, Qt::Dense6Pattern));
        painter.setPen(Qt::NoPen);

        for (int i = 0; i < numSteps; ++i) {
            int x = oldPos.x() + i * (newPos.x() - oldPos.x()) / (numSteps - 1);
            int y = oldPos.y() + i * (newPos.y() - oldPos.y()) / (numSteps - 1);

            painter.drawEllipse(x - (thickness / 2), y - (thickness / 2),
                                thickness, thickness);
        }
    } else if (brush == tr("Random Letters")) {
        QChar ch('A' + (qrand() % 26));

        QFont biggerFont = painter.font();
        biggerFont.setBold(true);
        biggerFont.setPointSize(biggerFont.pointSize() + thickness);
        painter.setFont(biggerFont);

        painter.drawText(newPos, QString(ch));

        QFontMetrics metrics(painter.font());
        boundingRect = metrics.boundingRect(ch);
        boundingRect.translate(newPos);
        boundingRect.adjust(-10, -10, +10, +10);
    }
    painter.restore();
    return boundingRect;
}

QRect BasicToolsPlugin::mouseRelease(const QString & /* brush *//*,
                                     QPainter & /* painter *//*,
                                     const QPoint & /* pos *//*)/*
{
    return QRect(0, 0, 0, 0);
}

QStringList BasicToolsPlugin::shapes() const
{
    return QStringList() << tr("Circle") << tr("Star") << tr("Text...");
}

QPainterPath BasicToolsPlugin::generateShape(const QString &shape,
                                             QWidget *parent)
{
    QPainterPath path;

    if (shape == tr("Circle")) {
        path.addEllipse(0, 0, 50, 50);
    } else if (shape == tr("Star")) {
        path.moveTo(90, 50);
        for (int i = 1; i < 5; ++i) {
            path.lineTo(50 + 40 * cos(0.8 * i * Pi),
                        50 + 40 * sin(0.8 * i * Pi));
        }
        path.closeSubpath();
    } else if (shape == tr("Text...")) {
        QString text = QInputDialog::getText(parent, tr("Text Shape"),
                                             tr("Enter text:"),
                                             QLineEdit::Normal, tr("Qt"));
        if (!text.isEmpty()) {
            QFont timesFont("Times", 50);
            timesFont.setStyleStrategy(QFont::ForceOutline);
            path.addText(0, 0, timesFont, text);
        }
    }

    return path;
}

QStringList BasicToolsPlugin::filters() const
{
    return QStringList() << tr("Invert Pixels") << tr("Swap RGB")
                         << tr("Grayscale");
}

QImage BasicToolsPlugin::filterImage(const QString &filter, const QImage &image,
                                     QWidget * /* parent *//*) /*
{
    QImage result = image.convertToFormat(QImage::Format_RGB32);

    if (filter == tr("Invert Pixels")) {
        result.invertPixels();
    } else if (filter == tr("Swap RGB")) {
        result = result.rgbSwapped();
    } else if (filter == tr("Grayscale")) {
        for (int y = 0; y < result.height(); ++y) {
            for (int x = 0; x < result.width(); ++x) {
                int pixel = result.pixel(x, y);
                int gray = qGray(pixel);
                int alpha = qAlpha(pixel);
                result.setPixel(x, y, qRgba(gray, gray, gray, alpha));
            }
        }
    }
    return result;
}
 */
