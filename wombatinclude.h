#ifndef WOMBATINCLUDE_H
#define WOMBATINCLUDE_H

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include <bitset>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <QAbstractItemModel>
#include <QAbstractListModel>
#include <QApplication>
#include <QActionGroup>
#include <QBoxLayout>
#include <QByteArray>
#include <QDateTime>
#include <QDebug>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFrame>
#include <QFuture>
#include <QFutureWatcher>
#include <QGridLayout>
#include <QImage>
#include <QInputDialog>
#include <QLineEdit>
#include <QIODevice>
//#include <QJsonDocument>
//#include <QJsonObject>
#include <QList>
#include <QMainWindow>
//#include <QMediaPlayer>
#include <QMessageBox>
#include <QMutex>
#include <QObject>
#include <QProgressBar>
#include <QSizePolicy>
#include <QSlider>
//#include <QtSql/QtSql>
//#include <QSqlDatabase>
//#include <QSqlError>
//#include <QSqlQuery>
//#include <QSqlQueryModel>
//#include <QSqlRecord>
#include <QStatusBar>
#include <QString>
#include <QStringList>
#include <QTextBlock>
#include <QTextCodec>
#include <QTextCursor>
#include <QTextEdit>
#include <QTextStream>
#include <QThread>
#include <QThreadPool>
#include <QTimeZone>
#include <QTreeWidgetItem>
#include <QVector>
#include <QtConcurrent>
//#include <QtPlugin>
#include <QtWidgets>
#include <QMouseEvent>
#include <QFocusEvent>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
//#include <QVideoWidget>
#include <QWebEngineView>
#include <QWidget>

#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdexcept>
#include <vector>
#include <math.h>
#include <assert.h>
#include <errno.h>
#include <ctype.h>
#include <algorithm>
#include <new>
#include <unistd.h>
#include <string.h>

#include <tar.h>
#include <libtar.h>

extern "C" {
#include "squash.h"
}
//#include <filmstripfilter.h>
//#include <videothumbnailer.h>
//#include <Magick++.h>

#include <libtsk.h>
#include <libewf.h>
#include <tsk_bsd.h>
#include <tsk_mac.h>
#include <tsk_sun.h>
#include <tsk_gpt.h>
#include <tsk_ntfs.h>
#include <tsk_exfatfs.h>
#include <tsk_fatfs.h>
#include <tsk_fatxxfs.h>
#include <tsk_ffs.h>
#include <tsk_ext2fs.h>
#include <tsk_iso9660.h>
#include <tsk_hfs.h>
#include <tsk_yaffs.h>
#include <tsk_pool.h>
#include <pool.hpp>
#include <apfs_pool_compat.hpp>
#include <apfs_fs.h>
#include <decmpfs.h>
#include <tsk_apfs.hpp>
#include <apfs_fs.hpp>
#include <tsk_apfs.h>
#include <apfs_compat.hpp>

// THESE NEED TO BE MANUALLY UPDATED AND COPIED TO /USR/INCLUDE/TSK/../ PERIODICALLY AS NEW TSK RELEASES ARE MADE. CURRENTLY AT 4.8.0
#include <tsk_base_i.h>
#include <tsk_fs_i.h>
#include <tsk_img_i.h>
#include <tsk_vs_i.h>

#endif // WOMBATINCLUDE_H
