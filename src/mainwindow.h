#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "datamodel.h"
#include "dbitemmodel.h"
#include "QStringListModel"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    DataModel* m_datamodel;
    QStringListModel* model;

    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnAddProject_clicked();
    void on_btnAddBuildinBlock_clicked();
    void addProject(QString name);
    void addBuildBlock(QString name, QString ref);
    void updateList();
    void load();
    void save();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
