#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class DataModel;
class ProjectPage;
class BuildingBlockPage;

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

private slots:
    void load();
    void save();
    void showProjectPage();
    void showBuildingBlockPage();

private:
    Ui::MainWindow *ui;
    DataModel* m_datamodel;
    ProjectPage* m_pageProject;
    BuildingBlockPage* m_pageBb;
};

#endif // MAINWINDOW_H
