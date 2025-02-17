#include "OutputFunc.h"
#include <iostream>
using namespace std;
void outputRider(const RiderObject& rider) {
    cout << "---------------------\n";
    cout << "ID: " << rider.getId() << "\n";
    cout << "Name: " << rider.getFirstName() << " " << rider.getLastName() << "\n";
    cout << "Phone: " << rider.getPhoneNumber() << "\n";
    cout << "Rentals:" << rider.getRentalCount() << "\n";

}

void outputRental(const RentalObject& rental, const RiderObject& rider) {
    cout << "---------------------\n";
    cout << "Rental ID: " << rental.getId() << "\n";
    cout << "Scooter ID: " << rental.getScooterId() << "\n";
    cout << "Period: " << rental.getStartTime() << " - " << rental.getEndTime() << "\n";
    cout << "Cost: " << rental.getTotalCost() << "\n";
}