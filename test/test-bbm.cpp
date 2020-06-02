#include "test-bbm.h"
#include <QTest>
#include <QSignalSpy>

struct DataModelSpy {
    DataModel* model;
    const QSignalSpy projectAddedSpy;
    const QSignalSpy projectDeletingSpy;
    const QSignalSpy buildingBlockAddedSpy;
    const QSignalSpy buildingBlockDeletingSpy;

    DataModelSpy(DataModel* model) 
    : model(model)
    , projectAddedSpy(model, &DataModel::projectAdded)
    , projectDeletingSpy(model, &DataModel::projectDeleting)
    , buildingBlockAddedSpy(model, &DataModel::buildingBlockAdded)
    , buildingBlockDeletingSpy(model, &DataModel::buildingBlockDeleting)
    {}
};

void TestDataModel::init()
{
    model = new DataModel();
}

void TestDataModel::cleanup()
{
    delete model;
}

void TestDataModel::testLoad()
{
    model->load("../../test/test.json");
    QVERIFY(model->projects().size() == 2);
    QVERIFY(model->buildingBlocks().size() == 4);
}

void TestDataModel::testSignals()
{
    DataModelSpy spy(model);
    auto p = model->addProject();
    QCOMPARE(spy.projectAddedSpy.count(), 1);
    QCOMPARE(spy.projectAddedSpy.at(0).at(0).data_ptr().data.ptr, p);
}

int main()
{
    TestDataModel test;
    QTest::qExec(&test);

    return 0;
}