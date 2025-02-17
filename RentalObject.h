#ifndef RENTAL_H
#define RENTAL_H

#include "RiderObject.h"

using namespace std;

class RentalObject {
private:
    int id;
    int riderId;
    int scooterId;
    char startTime[20];
    char endTime[20];
    double totalCost;
    bool exists;
    int selfAddress;
    int nextAddress;

public:
    RentalObject();

    int getId() const;

    void setId(int id);

    int getRiderId() const;

    void setRiderId(int riderId);

    int getScooterId() const;

    void setScooterId(int scooterId);

    const char *getStartTime() const; // Змінено на const char*
    void setStartTime(const string &startTime);

    const char *getEndTime() const; // Змінено на const char*
    void setEndTime(const string &endTime);

    double getTotalCost() const;

    void setTotalCost(double totalCost);

    bool isExists() const;

    void setExists(bool exists);

    int getSelfAddress() const;

    void setSelfAddress(int selfAddress);

    int getNextAddress() const;

    void setNextAddress(int nextAddress);
};

int insertRental(const RiderObject &rider, RentalObject &rental, string &error);

int getRental(const RiderObject &rider, RentalObject *rental, int rentalId, string &error);

int updateRental(const RentalObject &rental);

int deleteRental(const RiderObject &rider, const RentalObject &rental, string &error);

void PrintRentals(const RiderObject &rider);

#endif // RENTAL_H