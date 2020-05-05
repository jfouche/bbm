#ifndef PAGE_PROJECT_H
#define PAGE_PROJECT_H

#include <QWidget>

class DataModel;
class ProjectListModel;

namespace Ui {
class ProjectPage;
}

class ProjectPage : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectPage(DataModel* model, QWidget *parent = nullptr);
    ~ProjectPage();

private slots:
    void editCurrentProject();
    void addProject();

private:
    Ui::ProjectPage *ui;
    DataModel* m_model;
    ProjectListModel* m_projectListModel;
};

#endif // PAGE_PROJECT_H
