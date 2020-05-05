#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "datamodel.h"
#include "page_project.h"
#include "page_buildingblock.h"
#include <QFileDialog>

// ===========================================================================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_datamodel = new DataModel(this);
    // TODO : remove me
    m_datamodel->load("test.json");

    m_pageProject = new ProjectPage(m_datamodel, this);
    m_pageBb = new BuildingBlockPage(m_datamodel, this);

    ui->stackedWidget->addWidget(m_pageProject);
    ui->stackedWidget->addWidget(m_pageBb);

    showProjectPage();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::load()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("BB Files (*.json)"));
    if (!filename.isNull()) {
        m_datamodel->load(filename);
    }
}

void MainWindow::save()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Open file"), "", tr("BB Files (*.json)"));
    if (!filename.isNull()) {
        m_datamodel->save(filename);
    }
}

void MainWindow::showProjectPage()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::showBuildingBlockPage()
{
    ui->stackedWidget->setCurrentIndex(1);
}




