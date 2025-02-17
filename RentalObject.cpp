
#include "RentalObject.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

RentalObject::RentalObject()
    : id(0), riderId(0), scooterId(0), totalCost(0.0),
      exists(true), selfAddress(0), nextAddress(-1) {
    memset(startTime, 0, sizeof(startTime));
    memset(endTime, 0, sizeof(endTime));
}

int RentalObject::getId() const { return id; }
void RentalObject::setId(int id) { this->id = id; }

int RentalObject::getRiderId() const { return riderId; }
void RentalObject::setRiderId(int riderId) { this->riderId = riderId; }

int RentalObject::getScooterId() const { return scooterId; }
void RentalObject::setScooterId(int scooterId) { this->scooterId = scooterId; }

const char *RentalObject::getStartTime() const { return startTime; }

void RentalObject::setStartTime(const string &startTime) {
    strncpy(this->startTime, startTime.c_str(), sizeof(this->startTime) - 1);
    this->startTime[sizeof(this->startTime) - 1] = '\0';
}

const char *RentalObject::getEndTime() const { return endTime; }

void RentalObject::setEndTime(const string &endTime) {
    strncpy(this->endTime, endTime.c_str(), sizeof(this->endTime) - 1);
    this->endTime[sizeof(this->endTime) - 1] = '\0';
}

double RentalObject::getTotalCost() const { return totalCost; }
void RentalObject::setTotalCost(double totalCost) { this->totalCost = totalCost; }

bool RentalObject::isExists() const { return exists; }
void RentalObject::setExists(bool exists) { this->exists = exists; }

int RentalObject::getSelfAddress() const { return selfAddress; }
void RentalObject::setSelfAddress(int selfAddress) { this->selfAddress = selfAddress; }

int RentalObject::getNextAddress() const { return nextAddress; }
void RentalObject::setNextAddress(int nextAddress) { this->nextAddress = nextAddress; }

int insertRental(const RiderObject &rider, RentalObject &rental, string &error) {
    FILE* database = fopen(RENTAL_DATA, "r+b");
    if (!database) {
        error = "Failed to open rentals database";
        return 0;
    }

    RentalObject lastRental;
    int currentAddress = rider.getRentalFirstAddress();
    int maxId = 0;
    while (currentAddress != -1) {
        fseek(database, currentAddress, SEEK_SET);
        if (fread(&lastRental, RENTAL_SIZE, 1, database) != 1) {
            error = "Failed to read rental";
            fclose(database);
            return 0;
        }
        if (lastRental.getId() > maxId) {
            maxId = lastRental.getId();
        }
        currentAddress = lastRental.getNextAddress();
    }

    rental.setId(maxId + 1);

    fseek(database, 0, SEEK_END);
    int newAddress = ftell(database);
    rental.setSelfAddress(newAddress);
    rental.setNextAddress(-1);

    // Записуємо нову оренду
    if (fwrite(&rental, RENTAL_SIZE, 1, database) != 1) {
        error = "Failed to write rental";
        fclose(database);
        return 0;
    }

    // Оновлюємо попередню оренду (якщо вона існує)
    if (maxId != 0) {
        lastRental.setNextAddress(newAddress);
        fseek(database, lastRental.getSelfAddress(), SEEK_SET);
        if (fwrite(&lastRental, RENTAL_SIZE, 1, database) != 1) {
            error = "Failed to update previous rental";
            fclose(database);
            return 0;
        }
    }

    fclose(database);
    return 1;
}

int getRental(const RiderObject &rider, RentalObject *rental, int rentalId, string &error) {
    FILE *dataFile = fopen(RENTAL_DATA, "rb");
    fseek(dataFile, rider.getRentalFirstAddress(), SEEK_SET);

    for (int i = 0; i < rider.getRentalCount(); i++) {
        fread(rental, RENTAL_SIZE, 1, dataFile);
        if (rental->getId() == rentalId && rental->isExists()) { // Перевірка exists
            fclose(dataFile);
            return 1;
        }
        fseek(dataFile, rental->getNextAddress(), SEEK_SET);
    }

    error = "Rental not found";
    fclose(dataFile);
    return 0;
}


int updateRental(const RentalObject &rental) {
    FILE *database = fopen(RENTAL_DATA, "r+b");
    if (!database) {
        return 0;
    }

    fseek(database, rental.getSelfAddress(), SEEK_SET);
    if (fwrite(&rental, RENTAL_SIZE, 1, database) != 1) {
        fclose(database);
        return 0;
    }

    fclose(database);
    return 1;
}



int deleteRental(const RiderObject &rider, const RentalObject &rental, string &error) {
    FILE *dataFile = fopen(RENTAL_DATA, "r+b");
    if (!dataFile) {
        error = "Failed to open rentals database";
        return 0;
    }

    RentalObject target;
    fseek(dataFile, rental.getSelfAddress(), SEEK_SET);
    fread(&target, RENTAL_SIZE, 1, dataFile);

    if (!target.isExists()) {
        error = "Rental already deleted";
        fclose(dataFile);
        return 0;
    }

    target.setExists(false);
    fseek(dataFile, rental.getSelfAddress(), SEEK_SET);
    fwrite(&target, RENTAL_SIZE, 1, dataFile);
    fclose(dataFile);

    // Оновлюємо кількість оренд у водія
    RiderObject updatedRider = rider;
    updatedRider.setRentalCount(rider.getRentalCount() - 1);
    updateRider(updatedRider, error, rider.getId());

    // Додаємо ID до сміттєвої зони
    FILE *garbage = fopen(RENTAL_GARBAGE, "r+");
    int count;
    fscanf(garbage, "%d", &count);

    freopen(RENTAL_GARBAGE, "w", garbage);
    fprintf(garbage, "%d", count + 1);

    vector<int> ids(count);
    for (int i = 0; i < count; i++) {
        fscanf(garbage, "%d", &ids[i]);
    }

    for (int num : ids) {
        fprintf(garbage, " %d", num);
    }
    fprintf(garbage, " %d", rental.getId());

    fclose(garbage);
    return 1;
}

void PrintRentals(const RiderObject &rider) {
    FILE* database = fopen(RENTAL_DATA, "rb");
    if (!database) {
        cerr << "Error to open rental file" << endl;
        return;
    }

    int currentAddress = rider.getRentalFirstAddress();
    RentalObject rental;

    if (currentAddress == -1) {
        cout << "This rider doesn't have any active rentals." << endl;
        fclose(database);
        return;
    }

    bool hasActiveRentals = false; // Прапорець для перевірки активних оренд

    while (currentAddress != -1) {
        fseek(database, currentAddress, SEEK_SET);
        if (fread(&rental, RENTAL_SIZE, 1, database) != 1) {
            cerr << "Error to read rentals ." << endl;
            break;
        }

        // Виводимо тільки активні оренди
        if (rental.isExists()) {
            hasActiveRentals = true; // Знайдено активну оренду
            cout << "---------------------" << endl;
            cout << "Rental ID: " << rental.getId() << endl;
            cout << "Scooter ID: " << rental.getScooterId() << endl;
            cout << "Start Time: " << rental.getStartTime() << endl;
            cout << "End Time: " << rental.getEndTime() << endl;
            cout << "Total Cost: " << rental.getTotalCost() << endl;
        }

        currentAddress = rental.getNextAddress();
    }

    if (!hasActiveRentals) {
        cout << "This rider doesn't have any active rentals." << endl;
    }

    cout << "---------------------" << endl;
    fclose(database);
}