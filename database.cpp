#include "database.h"
#include <QtWidgets>
#include <QtXml/QtXml>

database::database(QFile *_file):file(_file){

}

void database::addGoodsToModel(QAbstractItemModel *model, const QString &subject,
             const double price,const double number, const QDateTime &date)
{
    int row = model->rowCount();
    model->insertRow(row);
    model->setData(model->index(row, 0), subject);
    model->setData(model->index(row, 1), price);
    model->setData(model->index(row, 2), number);
    model->setData(model->index(row, 3), date);
}
QDomDocument* database::getDomRoot(QDomNode& root){
    if(!file->open(QFile::ReadOnly)){
        return nullptr;
    }

    QDomDocument* doc = new QDomDocument;

    if(!doc->setContent(file)){
        file->close();
        return nullptr;
    }
    file->close();
    root = doc->documentElement().firstChild();
    return doc;
}

QAbstractItemModel* database::createMailModel(QObject *parent)
{
    QDomNode node;
    getDomRoot(node);

    QStandardItemModel *model = new QStandardItemModel(0, 4, parent);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("商品名称"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("价格"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("剩余"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("上货日期"));
    while(!node.isNull()){
        if(node.isElement()){
            QDomElement e = node.toElement();

            addGoodsToModel(model, e.attribute("name"),e.attribute("price").toFloat(),e.attribute("leftNumber").toInt(),QDateTime::fromString(e.attribute("time")));
            node=node.nextSibling();
        }
    }
    return model;
}

void database::deleteGoods(QStringList names){
    QDomNode node;
    QDomDocument* doc = getDomRoot(node);
    QDomNode root = doc->documentElement();
    QDomNodeList list=doc->elementsByTagName("goods"); //由标签名定位
    for(int j=0;j<list.count();j++){
        QDomElement e = list.at(j).toElement();
        for(int i=0;i<names.size();i++){
            if(e.attribute("name") == names.at(i)){
                root.removeChild(list.at(j));
                break;
            }
        }
    }
    if(!file->open(QFile::WriteOnly|QFile::Truncate))
        return;
    QTextStream out_stream(file);
    doc->save(out_stream,4); //缩进4格
    file->close();
}

void database::addGoods(QAbstractItemModel *model){
    QDomDocument *doc = nullptr;
    QDomElement root;
    doc = new QDomDocument;
    QDomProcessingInstruction instruction;
    instruction = doc->createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc->appendChild(instruction);
    root = doc->createElement(QString("info"));
    doc->appendChild(root); //添加根元素

    QDomElement goods;
    QString name;
    double price;
    int lastNumber;
    QDateTime time;
    for(int i=0;i<model->rowCount();i++){
        name = model->index(i,0).data().toString();
        price = model->index(i,1).data().toDouble();
        lastNumber = model->index(i,2).data().toInt();
        time = model->index(i,3).data().toDateTime();

        goods = doc->createElement("goods");
        goods.setAttribute("name",name);
        goods.setAttribute("price",price);
        goods.setAttribute("lastNumber",lastNumber);
        goods.setAttribute("time",time.toString());
        root.appendChild(goods);
    }


    if(!file->open(QFile::WriteOnly|QFile::Truncate)) //先读进来，再重写，如果不用truncate就是在后面追加内容，就无效了
            return;
        //输出到文件
    QTextStream out_stream(file);
    doc->save(out_stream,4); //缩进4格
    file->close();
}
