#ifndef DATABASE_H
#define DATABASE_H

class QTime;
class QFile;
class QAbstractItemModel;
class QDomDocument;
class QDomNode;
class QStringList;
class QObject;
class QString;
class QDateTime;

class database{
public:
    database(QFile*);

    QDomDocument* getDomRoot(QDomNode& root);
    void deleteGoods(QStringList);
    void addGoods(QAbstractItemModel *model);
    QAbstractItemModel* createMailModel(QObject *parent);
    void addGoodsToModel(QAbstractItemModel *model, const QString &subject,const double price,
                 const double number, const QDateTime &date);
    QFile *file;


};

#endif // DATABASE_H
