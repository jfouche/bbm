#ifndef PAGE_BUILDINGBLOCK_H
#define PAGE_BUILDINGBLOCK_H

#include <QWidget>
#include <QSortFilterProxyModel>

class DataModel;
class BuildingBlock;
class BuildingBlockTreeModel;


class FilteredBbTreeModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    FilteredBbTreeModel(BuildingBlockTreeModel* model, QObject* parent);
};

namespace Ui {
class BuildingBlockPage;
}

class BuildingBlockPage : public QWidget
{
    Q_OBJECT

public:
    explicit BuildingBlockPage(DataModel* model, QWidget *parent = nullptr);
    ~BuildingBlockPage();

private slots:
    void addBuildingBlock();
    void editCurrentBuildingBlock();
    void removeCurrentBuildingBlock();
    void showBuildingBlockManager();
    void updateUI();
    void filter(const QString& filter);

private:
    BuildingBlock* getSelection();

private:
    Ui::BuildingBlockPage *ui;
    DataModel* m_model;
    BuildingBlockTreeModel* m_bbTreeModel;
    FilteredBbTreeModel* m_filteredBbTreeModel;
};

#endif // PAGE_BUILDINGBLOCK_H
