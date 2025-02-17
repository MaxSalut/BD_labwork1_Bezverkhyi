#ifndef RIDER_H
#define RIDER_H

#include "DataBaseCore.h"
#include <string>

using namespace std;

class RiderObject {
private:
    int id;
    char firstName[15];
    char lastName[15];
    char phoneNumber[20];
    char password[25];
    int rentalCount;
    int rentalFirstAddress;

public:
    RiderObject();

    int getId() const;

    void setId(int id);

    const char *getFirstName() const;

    void setFirstName(const string &firstName);

    const char *getLastName() const;

    void setLastName(const string &lastName);

    const char *getPhoneNumber() const;

    void setPhoneNumber(const string &phoneNumber);

    const char *getPassword() const;

    void setPassword(const string &password);

    int getRentalCount() const;

    void setRentalCount(int rentalCount);

    int getRentalFirstAddress() const;

    void setRentalFirstAddress(int rentalFirstAddress);
};

int insertRider(const RiderObject &rider);

int getRider(RiderObject *rider, int id, string &error);

int updateRider(const RiderObject &rider, string &error, int id);

int deleteRider(int id, string &error);

int PrintListOfRiders(string &error);

#endif // RIDER_H
