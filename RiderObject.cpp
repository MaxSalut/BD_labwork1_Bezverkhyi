#include "RiderObject.h"
#include "DataBaseCore.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

RiderObject::RiderObject() : id(0), rentalCount(0), rentalFirstAddress(-1) {
    memset(firstName, 0, sizeof(firstName));
    memset(lastName, 0, sizeof(lastName));
    memset(phoneNumber, 0, sizeof(phoneNumber));
    memset(password, 0, sizeof(password));
}

int RiderObject::getId() const { return id; }
void RiderObject::setId(int id) { this->id = id; }

const char *RiderObject::getFirstName() const { return firstName; }

void RiderObject::setFirstName(const string &firstName) {
    strncpy(this->firstName, firstName.c_str(), sizeof(this->firstName) - 1);
    this->firstName[sizeof(this->firstName) - 1] = '\0'; // Гарантуємо нуль-термінатор
}

const char *RiderObject::getLastName() const { return lastName; }

void RiderObject::setLastName(const string &lastName) {
    strncpy(this->lastName, lastName.c_str(), sizeof(this->lastName) - 1);
    this->lastName[sizeof(this->lastName) - 1] = '\0';
}

const char *RiderObject::getPhoneNumber() const { return phoneNumber; }

void RiderObject::setPhoneNumber(const string &phoneNumber) {
    strncpy(this->phoneNumber, phoneNumber.c_str(), sizeof(this->phoneNumber) - 1);
    this->phoneNumber[sizeof(this->phoneNumber) - 1] = '\0';
}

const char *RiderObject::getPassword() const { return password; }

void RiderObject::setPassword(const string &password) {
    strncpy(this->password, password.c_str(), sizeof(this->password) - 1);
    this->password[sizeof(this->password) - 1] = '\0';
}

int RiderObject::getRentalCount() const { return rentalCount; }
void RiderObject::setRentalCount(int rentalCount) { this->rentalCount = rentalCount; }

int RiderObject::getRentalFirstAddress() const { return rentalFirstAddress; }
void RiderObject::setRentalFirstAddress(int address) { rentalFirstAddress = address; }

int insertRider(const RiderObject &rider) {
    FILE *garbage = fopen(RIDER_GARBAGE, "r+b");
    FILE *indexFile = fopen(RIDER_IND, "a+b");
    FILE *dataFile = fopen(RIDER_DATA, "a+b");

    if (!garbage || !indexFile || !dataFile) {
        cerr << "Failed to open database files" << endl;
        return -1;
    }

    int newId = 0;
    int garbageCount = 0;
    fscanf(garbage, "%d", &garbageCount);

    if (garbageCount > 0) {
        vector<int> deletedIds(garbageCount);
        for (int i = 0; i < garbageCount; i++) {
            fscanf(garbage, "%d", &deletedIds[i]);
        }
        newId = deletedIds[0];

        freopen(RIDER_GARBAGE, "w", garbage);
        fprintf(garbage, "%d", garbageCount - 1);
        for (size_t i = 1; i < deletedIds.size(); i++) {
            fprintf(garbage, " %d", deletedIds[i]);
        }
    } else {
        fseek(indexFile, 0, SEEK_END);
        newId = (ftell(indexFile) / INDEXER_SIZE + 1);
    }

    Indexer indexer;
    indexer.id = newId;
    indexer.exists = 1;
    indexer.address = (newId - 1) * RIDER_SIZE;

    // Встановлюємо ID для нового запису
    RiderObject newRider = rider;
    newRider.setId(newId);

    fseek(dataFile, indexer.address, SEEK_SET);
    fwrite(&newRider, RIDER_SIZE, 1, dataFile);

    fseek(indexFile, (newId - 1) * INDEXER_SIZE, SEEK_SET);
    fwrite(&indexer, INDEXER_SIZE, 1, indexFile);

    fclose(garbage);
    fclose(indexFile);
    fclose(dataFile);

    return newId;
}

int getRider(RiderObject *rider, int id, string &error) {
    FILE *indexFile = fopen(RIDER_IND, "rb");
    if (!indexFile) {
        error = "Database not initialized";
        return 0;
    }

    fseek(indexFile, 0, SEEK_END);
    if (id <= 0 || (id - 1) * INDEXER_SIZE > ftell(indexFile)) {
        error = "Invalid rider ID";
        fclose(indexFile);
        return 0;
    }

    Indexer indexer;
    fseek(indexFile, (id - 1) * INDEXER_SIZE, SEEK_SET);
    fread(&indexer, INDEXER_SIZE, 1, indexFile);
    fclose(indexFile);

    if (!indexer.exists) {
        error = "Rider deleted";
        return 0;
    }

    FILE *dataFile = fopen(RIDER_DATA, "rb");
    if (!dataFile) {
        error = "Failed to open rider data file";
        return 0;
    }

    fseek(dataFile, indexer.address, SEEK_SET);
    fread(rider, RIDER_SIZE, 1, dataFile);
    fclose(dataFile);

    return 1;
}

int updateRider(const RiderObject &rider, string &error, int id) {
    FILE *indexFile = fopen(RIDER_IND, "r+b");
    if (!indexFile) {
        error = "Database error";
        return 0;
    }

    Indexer indexer;
    fseek(indexFile, (id - 1) * INDEXER_SIZE, SEEK_SET);
    fread(&indexer, INDEXER_SIZE, 1, indexFile);

    if (!indexer.exists) {
        error = "Rider not found";
        fclose(indexFile);
        return 0;
    }

    FILE *dataFile = fopen(RIDER_DATA, "r+b");
    fseek(dataFile, indexer.address, SEEK_SET);
    fwrite(&rider, RIDER_SIZE, 1, dataFile);

    fclose(dataFile);
    fclose(indexFile);
    return 1;
}

int deleteRider(int id, string &error) {
    FILE *indexFile = fopen(RIDER_IND, "r+b");
    if (!indexFile) {
        error = "Database error";
        return 0;
    }

    Indexer indexer;
    fseek(indexFile, (id - 1) * INDEXER_SIZE, SEEK_SET);
    fread(&indexer, INDEXER_SIZE, 1, indexFile);

    if (!indexer.exists) {
        error = "Rider already deleted";
        fclose(indexFile);
        return 0;
    }

    indexer.exists = 0;
    fseek(indexFile, (id - 1) * INDEXER_SIZE, SEEK_SET);
    fwrite(&indexer, INDEXER_SIZE, 1, indexFile);
    fclose(indexFile);

    FILE *garbage = fopen(RIDER_GARBAGE, "r+");
    int count;
    fscanf(garbage, "%d", &count);

    freopen(RIDER_GARBAGE, "w", garbage);
    fprintf(garbage, "%d", count + 1);

    vector<int> ids(count);
    for (int i = 0; i < count; i++) {
        fscanf(garbage, "%d", &ids[i]);
    }

    for (int num: ids) {
        fprintf(garbage, " %d", num);
    }
    fprintf(garbage, " %d", id);

    fclose(garbage);
    return 1;
}

int PrintListOfRiders(string &error) {
    FILE *indexFile = fopen(RIDER_IND, "rb");
    if (!indexFile) {
        error = "Database not initialized";
        return 0;
    }

    fseek(indexFile, 0, SEEK_END);
    int count = ftell(indexFile) / INDEXER_SIZE;

    cout << "=== Rider List ===" << endl;
    for (int i = 1; i <= count; i++) {
        Indexer indexer;
        fseek(indexFile, (i - 1) * INDEXER_SIZE, SEEK_SET);
        fread(&indexer, INDEXER_SIZE, 1, indexFile);

        if (indexer.exists) {
            RiderObject rider;
            if (getRider(&rider, i, error)) {
                cout << "ID: " << rider.getId() << endl;
                cout << "Name: " << rider.getFirstName() << " " << rider.getLastName() << endl;
                cout << "Phone: " << rider.getPhoneNumber() << endl;
                cout << "Rentals:" << rider.getRentalCount() << "\n";
                cout << "---------------------" << endl;
            }
        }
    }

    fclose(indexFile);
    return 1;
}