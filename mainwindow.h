#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class database;
class QSortFilterProxyModel;
class QAbstractItemModel;
class QTreeView;
class QLineEdit;
class QPushButton;
class QGroupBox;
class QGridLayout;
class QDomNode;
class QKeyEvent;
class QStringList;
class QDomDocument;
class QTextStream;
class QFile;
class QDateTime;
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QFile*);
    void keyPressEvent(QKeyEvent*);
    virtual ~MainWindow();

private slots:
    void filterRegExpChanged();
    void addNewGoods();

private:
    void setSourceModel();

    void deleteInfo();
private:
    //void closeEvent(QCloseEvent*);
    QSortFilterProxyModel *proxyModel;

    QTreeView *infoView;

    QLineEdit *filterPatternLineEdit;
    QPushButton *infoAddButton;

    QGroupBox *infoGroupBox;
    database *db;

};

#endif // MAINWINDOW_H
