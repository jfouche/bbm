#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QSortFilterProxyModel;
class DataModel;
class BuildingBlock;
class Project;
class ProjectListModel;
class BuildingBlockListModel;
class UsesTreeModel;
class UsedByTreeModel;
class AvailableBuildingBlockChildrenModel;
class ProjectBuildingBlockListModel;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


/**
 * @brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Project* getSelectedProject();
    BuildingBlock* getSelectedBuildingBlock();
    void editProject(Project& project);
    void editBuildingBlock(BuildingBlock& bb);

private slots:
    void load();
    void save();
    void updateUI();
    void updateDetailModel();
    void filter(const QString& filter);
    void addProject();
    void editCurrentProject();
    void delCurrentProject();
    void addBuildingBlock();
    void editCurrentBuildingBlock();
    void delCurrentBuildingBlock();
    void select(BuildingBlock* bb);
    void select(Project* project);
    void saveProject();
    void saveBuildingBlock();

private:
    Ui::MainWindow *ui;
    DataModel* m_datamodel;

    /// MODELS
    ProjectListModel* projectListModel;
    QSortFilterProxyModel* filteredProjectListModel;
    BuildingBlockListModel*  bbListModel;
    QSortFilterProxyModel* filteredBbListModel;
    UsesTreeModel* usesTreeModel;
    UsedByTreeModel* usedByTreeModel;
    QSortFilterProxyModel* filteredDetailTreeModel;
    AvailableBuildingBlockChildrenModel* availableBbChildrenModel;
    ProjectBuildingBlockListModel* projectBbListModel;
};

#endif // MAINWINDOW_H
