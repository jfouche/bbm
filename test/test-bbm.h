#ifndef TEST_BBM_H_INCLUDED
#define  TEST_BBM_H_INCLUDED

#include "datamodel.h"

class TestDataModel : public QObject 
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void testClear();
    void testLoad();
    void testSignals();

private:
    DataModel* model;
};

#endif //  TEST_BBM_H_INCLUDED 
