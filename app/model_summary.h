#ifndef SUMMARYMODEL_H
#define SUMMARYMODEL_H

#include <QAbstractListModel>

class DataModel;
class BuildingBlock;

class SummaryModel : public QAbstractListModel 
{
    Q_OBJECT

public:
    explicit SummaryModel(DataModel* datamodel, QObject* parent);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private slots:
    void clear();
    void add(BuildingBlock* bb);
    void del(BuildingBlock* bb);
    void update(BuildingBlock* bb);

private:
    enum Columns {
        COL_NAME,
        COL_REF,
        COL_MATURITY,
        COL_INFO,
        COL_COUNT
    };

private:
    DataModel* m_model;
};

#endif // SUMMARYMODEL_H
