#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSortFilterProxyModel>

class DataModel;
class ProjectListModel;
class BuildingBlockTreeModel;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class FilteredBbTreeModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    FilteredBbTreeModel(BuildingBlockTreeModel* model, QObject* parent);
};


/**
 * @brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addProject();
    void addBuildingBlock();
    void updateProjectList();
    void updateBuildingBlockList();
    void editProject(const QModelIndex &index);
    void editCurrentBuildingBlock();
    void load();
    void save();
    void showProjectPage();
    void showBuildingBlockPage();
    void showBuildingBlockManager();
    void updateUI();

private:
    Ui::MainWindow *ui;
    DataModel* m_datamodel;
    ProjectListModel* m_projectListModel;
    BuildingBlockTreeModel* m_bbTreeModel;
    FilteredBbTreeModel* m_filteredBbTreeModel;
};

#endif // MAINWINDOW_H
