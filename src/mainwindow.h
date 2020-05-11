#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QSortFilterProxyModel;
class DataModel;
class BuildingBlock;
class Project;
class ProjectListModel;
class BuildingBlockListModel;
class BuildingBlockTreeModel;
class AvailableBuildingBlockChildrenModel;


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

private slots:
    void load();
    void save();
    void updateUI();
    void filter(const QString& filter);
    void addProject();
    void editCurrentProject();
    void delCurrentProject();
    void select(BuildingBlock* bb);
    void select(Project* project);
    void saveProject();
    void hideRightPanel();

private:
    Ui::MainWindow *ui;
    DataModel* m_datamodel;

    /// MODELS
    ProjectListModel* projectListModel;
    QSortFilterProxyModel* filteredProjectListModel;
    BuildingBlockListModel*  bbListModel;
    QSortFilterProxyModel* filteredBbListModel;
    BuildingBlockTreeModel* bbTreeModel;
    QSortFilterProxyModel* filteredBbTreeModel;
    AvailableBuildingBlockChildrenModel* availableBbChildrenModel;
};

#endif // MAINWINDOW_H
