#include <QtWidgets>
#include "database.h"
#include "mainwindow.h"
#include <QtXml/QtXml>
#include<limits>

MainWindow::MainWindow(QFile* goodsFile)
{
    db = new database(goodsFile);
    proxyModel = new QSortFilterProxyModel;

    infoView = new QTreeView;
    infoView->setRootIsDecorated(false);
    infoView->setAlternatingRowColors(true);
    infoView->setModel(proxyModel);
    infoView->setSortingEnabled(true);

    filterPatternLineEdit = new QLineEdit;
    infoAddButton = new QPushButton;

    infoGroupBox = new QGroupBox;

    connect(filterPatternLineEdit,&QLineEdit::textChanged,
            this,&MainWindow::filterRegExpChanged);
    connect(infoAddButton,&QPushButton::clicked,this,&MainWindow::addNewGoods);
    QGridLayout *infoLayout = new QGridLayout;
    infoLayout->addWidget(filterPatternLineEdit,0,0,1,2);
    infoLayout->addWidget(infoAddButton,0,4);
    infoLayout->addWidget(infoView, 1, 0, 1, 5);
    infoGroupBox->setLayout(infoLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(infoGroupBox);

    setLayout(mainLayout);

    setWindowTitle(tr("销售管理系统"));
    resize(500,450);

    setSourceModel();
    infoView->sortByColumn(3, Qt::AscendingOrder);

    filterPatternLineEdit->setText("");
    infoAddButton->setText("添加新商品");
}

MainWindow::~MainWindow(){
    this->hide();
    QRegExp regExp("",Qt::CaseInsensitive,QRegExp::FixedString);
    proxyModel->setFilterRegExp(regExp);
    db->addGoods(proxyModel);
}

void MainWindow::setSourceModel()
{
    proxyModel->setSourceModel(db->createMailModel(this));
    infoView->setModel(proxyModel);
}

void MainWindow::filterRegExpChanged()
{
    QRegExp regExp(filterPatternLineEdit->text(),Qt::CaseInsensitive,QRegExp::FixedString);
    proxyModel->setFilterRegExp(regExp);
}

void MainWindow::addNewGoods()
{
    QString name = "name";
    double price = 0;
    int lastNumber = 0;
    QDateTime time = QDateTime::currentDateTime();
    bool ok;
    name = QInputDialog::getText(this,tr("info input"),tr("请输入商品名称:"),QLineEdit::Normal,tr(""),&ok);
    ok = !name.isEmpty();
    if(ok){
        price = QInputDialog::getDouble(this,tr("info input"),tr("请输入商品价格:"),.0,.0,std::numeric_limits<double>::max(),1,&ok);

        if(ok){
            lastNumber = QInputDialog::getInt(this,tr("info input"),tr("请输入剩余数量:"),0,0,std::numeric_limits<int>::max(),1,&ok);
        }
    }
    if(ok){
        db->addGoodsToModel(proxyModel,name,price,lastNumber,time);
    }
}

void MainWindow::deleteInfo(){
    QModelIndexList list = infoView->selectionModel()->selectedRows();
    if(!list.isEmpty()){
        QStringList nameList;
        for(int i=0;i<list.size();i++){
            nameList.push_back(list.at(i).data().toString());
        }
        for(int i=0;i<list.size();i++){
            proxyModel->removeRow(list.at(i).row());
        }
        //db->deleteGoods(nameList);
    }
}

void MainWindow::keyPressEvent(QKeyEvent* e){
    int val = e->key();
    switch(val){
    case Qt::Key_Delete:
        deleteInfo();
        break;
    case Qt::Key_N:
        if(e->modifiers() & Qt::ControlModifier){
            addNewGoods();
        }
        break;
    default:
        break;
    }
}
