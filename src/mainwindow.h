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
    DataModel* m_datamodel;
    ProjectListModel* projectListModel;

    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addProject();
    void addBuildingBlock();
    void updateProjectList();
    void editProject(const QModelIndex &index);
    void addProject(QString name);
    void addBuildBlock(QString name, QString ref);
    void load();
    void save();


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
