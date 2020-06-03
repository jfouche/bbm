#include "test-bbm.h"
#include <QTest>
#include <QSignalSpy>

struct BuildingBlockSpy {
    BuildingBlock* const bb;
    const QSignalSpy changedSpy;
    const QSignalSpy childAddedSpy;
    const QSignalSpy childRemovedSpy;
    const QSignalSpy parentBbAddedSpy;
    const QSignalSpy parentProjectAddedSpy;
    const QSignalSpy parentBbRemovedSpy;
    const QSignalSpy parentProjectRemovedSpy;

    BuildingBlockSpy(BuildingBlock* bb)
    : bb(bb)
    , changedSpy(bb, &BuildingBlock::changed)
    , childAddedSpy(bb, &BuildingBlock::childAdded)
    , childRemovedSpy(bb, &BuildingBlock::childRemoved)
    , parentBbAddedSpy(bb, qOverload<BuildingBlock*>(&BuildingBlock::parentAdded))
    , parentProjectAddedSpy(bb, qOverload<Project*>(&BuildingBlock::parentAdded))
    , parentBbRemovedSpy(bb, qOverload<BuildingBlock*>(&BuildingBlock::parentRemoved))
    , parentProjectRemovedSpy(bb, qOverload<Project*>(&BuildingBlock::parentRemoved))
    {}
};

struct DataModelSpy {
    DataModel* const model;
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
    QCOMPARE(model->projects().size(), 2);
    auto& p1 = model->projects().at(0);
    auto& p2 = model->projects().at(1);
    QCOMPARE(p1->name(), "P1");
    QCOMPARE(p2->name(), "P2");

    QCOMPARE(model->buildingBlocks().size(), 4);
    auto& bb1 = model->buildingBlocks().at(0);
    auto& bb2 = model->buildingBlocks().at(1);
    auto& bb11 = model->buildingBlocks().at(2);
    auto& bb12 = model->buildingBlocks().at(3);
    QCOMPARE(bb1->name(), "BB1");
    QCOMPARE(bb2->name(), "BB2");
    QCOMPARE(bb11->name(), "BB11");
    QCOMPARE(bb12->name(), "BB12");

    QCOMPARE(p1->buildingBlocks().size(), 1);
    QCOMPARE(p1->buildingBlocks().at(0), bb1);
    QCOMPARE(p2->buildingBlocks().size(), 2);
    QCOMPARE(p2->buildingBlocks().at(0), bb1);
    QCOMPARE(p2->buildingBlocks().at(1), bb2);

    QCOMPARE(bb1->children().size(), 2);
    QCOMPARE(bb1->children().at(0), bb11);
    QCOMPARE(bb1->children().at(1), bb12);
    QCOMPARE(bb2->children().size(), 0);
    QCOMPARE(bb11->children().size(), 0);
    QCOMPARE(bb12->children().size(), 0);

    /// Parents

    QCOMPARE(bb1->parentProject().size(), 2);
    QVERIFY(bb1->parentProject().contains(p1));
    QVERIFY(bb1->parentProject().contains(p2));
    QCOMPARE(bb1->parentBb().size(), 0);

    QCOMPARE(bb2->parentProject().size(), 1);
    QVERIFY(bb2->parentProject().contains(p2));
    QCOMPARE(bb2->parentBb().size(), 0);

    QCOMPARE(bb11->parentProject().size(), 0);
    QCOMPARE(bb11->parentBb().size(), 1);
    QVERIFY(bb11->parentBb().contains(bb1));

    QCOMPARE(bb12->parentProject().size(), 0);
    QCOMPARE(bb12->parentBb().size(), 1);
    QVERIFY(bb12->parentBb().contains(bb1));
}

void TestDataModel::testSignals()
{
    model->load("../../test/test.json");

    DataModelSpy spy(model);
    auto p = model->addProject();
    QCOMPARE(spy.projectAddedSpy.count(), 1);
    QCOMPARE(spy.projectAddedSpy.at(0).at(0).data_ptr().data.ptr, p);
    QCOMPARE(spy.projectDeletingSpy.count(), 0);
    QCOMPARE(spy.buildingBlockAddedSpy.count(), 0);
    QCOMPARE(spy.buildingBlockDeletingSpy.count(), 0);

    auto& bb1 = model->buildingBlocks().at(0);
    auto& bb2 = model->buildingBlocks().at(1);
    auto& bb11 = model->buildingBlocks().at(2);
    auto& bb12 = model->buildingBlocks().at(3);

    BuildingBlockSpy bb2Spy(bb2);
    BuildingBlockSpy bb12Spy(bb12);
    bb2->add(bb12);
    QCOMPARE(bb2Spy.childAddedSpy.count(), 1);
    QCOMPARE(bb2Spy.childAddedSpy.at(0).at(0).data_ptr().data.ptr, bb12);
    QCOMPARE(bb2Spy.parentBbAddedSpy.count(), 0);

    QCOMPARE(bb12Spy.parentBbAddedSpy.count(), 1);
    QCOMPARE(bb12Spy.parentBbAddedSpy.at(0).at(0).data_ptr().data.ptr, bb2);
    QCOMPARE(bb12Spy.childAddedSpy.count(), 0);
}

QTEST_MAIN(TestDataModel);
