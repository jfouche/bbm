#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "datamodel.h"
#include "dbitemmodel.h"
#include "projectlistmodel.h"
#include "QStringListModel"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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
    void editProject(const QModelIndex &index);
    void load();
    void save();
    void showProjectPage();
    void showBuildingBlockPage();


private:
    Ui::MainWindow *ui;

    DataModel* m_datamodel;
    ProjectListModel* projectListModel;
};

#endif // MAINWINDOW_H
