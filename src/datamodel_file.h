#ifndef DATAMODELFILE_H
#define DATAMODELFILE_H

#include "datamodel.h"
#include <QJsonObject>

void read(DataModel& data, const QJsonObject& jsonObj);

void write(const DataModel& model, QJsonObject& obj);

#endif // DATAMODELFILE_H
