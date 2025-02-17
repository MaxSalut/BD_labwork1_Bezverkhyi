#include "Validator.h"
#include "DataBaseCore.h"
#include <iostream>
using namespace std;
bool validateDatabaseFiles(FILE* indexTable, FILE* database, string& error) {
    if (!indexTable || !database) {
        error = "Database files not found";
        return false;
    }
    return true;
}

bool validateIndexExists(FILE* indexTable, string& error, int id) {
    fseek(indexTable, 0, SEEK_END);
    if (id <= 0 || (id - 1) * INDEXER_SIZE > ftell(indexTable)) {
        error = "Invalid ID";
        return false;
    }
    return true;
}

bool validateRecordActive(const Indexer& indexer, string& error) {
    if (!indexer.exists) {
        error = "Record deleted";
        return false;
    }
    return true;
}

bool validateRentalExists(const RiderObject& rider, int rentalId, string& error) {
    FILE* rentalsDb = fopen(RENTAL_DATA, "a+b");
    if (!rentalsDb) {
        error = "Failed to open rentals database";
        return false;
    }

    RentalObject rental;
    fseek(rentalsDb, rider.getRentalFirstAddress(), SEEK_SET);
    for (int i = 0; i < rider.getRentalCount(); i++) {
        fread(&rental, RENTAL_SIZE, 1, rentalsDb);
        if (rental.getId() == rentalId && rental.isExists()) {
            fclose(rentalsDb);
            return true;
        }
        fseek(rentalsDb, rental.getNextAddress(), SEEK_SET);
    }

    error = "Rental not found";
    fclose(rentalsDb);
    return false;
}

void printStatistics() {
    FILE* indexTable = fopen(RIDER_IND, "rb");
    if (!indexTable) {
        cout << "Database not initialized\n";
        return;
    }

    fseek(indexTable, 0, SEEK_END);
    long fileSize = ftell(indexTable);
    rewind(indexTable);

    if (fileSize == 0) {
        cout << "No riders in database\n";
        fclose(indexTable);
        return;
    }

    int riderCount = 0;
    int rentalCount = 0;

    for (int i = 1; i <= fileSize / INDEXER_SIZE; i++) {
        Indexer indexer;
        fseek(indexTable, (i - 1) * INDEXER_SIZE, SEEK_SET);
        if (fread(&indexer, INDEXER_SIZE, 1, indexTable) != 1) {
            cerr << "Failed to read index\n";
            fclose(indexTable);
            return;
        }

        if (indexer.exists) {
            RiderObject rider;
            string error;
            if (getRider(&rider, i, error)) {
                riderCount++;
                rentalCount += rider.getRentalCount();
            } else {
                cerr << "Error: " << error << endl;
            }
        }
    }

    fclose(indexTable);
    cout << "Total riders: " << riderCount << endl;
    cout << "Total rentals: " << rentalCount << endl;
}