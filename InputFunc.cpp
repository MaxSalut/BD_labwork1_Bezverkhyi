#include "InputFunc.h"
#include <iostream>
using namespace std;
void inputRider(RiderObject& rider) {
    string firstName, lastName, phoneNumber, password;

    cout << "Enter rider's first name: ";
    cin >> firstName;
    rider.setFirstName(firstName);

    cout << "Enter rider's last name: ";
    cin >> lastName;
    rider.setLastName(lastName);

    cout << "Enter rider's phone number: ";
    cin >> phoneNumber;
    rider.setPhoneNumber(phoneNumber);

    cout << "Enter rider's password: ";
    cin >> password;
    rider.setPassword(password);
}

void inputRental(RentalObject& rental) {
    int scooterId;
    string startTime, endTime;
    double totalCost;

    cout << "Enter scooter ID: ";
    cin >> scooterId;
    rental.setScooterId(scooterId);

    cout << "Enter start time: ";
    cin >> startTime;
    rental.setStartTime(startTime);

    cout << "Enter end time: ";
    cin >> endTime;
    rental.setEndTime(endTime);

    cout << "Enter total cost: ";
    cin >> totalCost;
    rental.setTotalCost(totalCost);
}