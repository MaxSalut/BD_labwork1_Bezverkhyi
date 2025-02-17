#ifndef CHECK_H
#define CHECK_H

#include "RiderObject.h"
#include "RentalObject.h"
#include <string>
using namespace std;
bool validateDatabaseFiles(FILE* indexTable, FILE* database, string& error);
bool validateIndexExists(FILE* indexTable, string& error, int id);
bool validateRecordActive(const Indexer& indexer, string& error);
bool validateRentalExists(const RiderObject& rider, int rentalId, string& error);
void printStatistics();

#endif