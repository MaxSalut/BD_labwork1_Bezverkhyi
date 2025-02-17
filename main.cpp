#include <iostream>
#include "RiderObject.h"
#include "RentalObject.h"
#include "InputFunc.h"
#include "OutputFunc.h"
#include "Validator.h"
#include "DataBaseCore.h"
#include <locale>

using namespace std;

void displayMenu() {
    cout << "===== Main Menu =====\n";
    cout << "1. Insert Master Record (Rider)\n";
    cout << "2. Get Master Record (Rider)\n";
    cout << "3. Update Master Record (Rider)\n";
    cout << "4. Delete Master Record (Rider)\n";
    cout << "5. Print All Master Records (Riders)\n";
    cout << "6. Insert Slave Record (Rental)\n";
    cout << "7. Get Slave Record (Rental)\n";
    cout << "8. Update Slave Record (Rental)\n";
    cout << "9. Delete Slave Record (Rental)\n";
    cout << "10. Print All Slave Records (Rentals)\n";
    cout << "11. Calculate Statistics\n";
    cout << "12. Exit\n";
    cout << "=====================\n";
    cout << "Enter your choice: ";
}

int main() {
    initializeDatabase();

    RiderObject rider;
    RentalObject rental;
    int choice, id;
    int  riderId, rentalId; // Оголошення змінних
    string error;

    while (true) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1: { // Insert Master Record (Rider)
                inputRider(rider);
                int newId = insertRider(rider);
                if (newId != -1) {
                    cout << "Rider added successfully with ID: " << newId << endl;
                } else {
                    cout << "Failed to add rider." << endl;
                }
                break;
            }

            case 2: { // Get Master Record (Rider)
                cout << "Enter Rider ID: ";
                cin >> id;
                if (getRider(&rider, id, error)) {
                    outputRider(rider);
                } else {
                    cout << "Error: " << error << endl;
                }
                break;
            }

            case 3: { // Update Master Record (Rider)
                cout << "Enter Rider ID: ";
                cin >> id;
                if (getRider(&rider, id, error)) {
                    inputRider(rider);
                    if (updateRider(rider, error, id)) {
                        cout << "Rider updated successfully." << endl;
                    } else {
                        cout << "Error: " << error << endl;
                    }
                } else {
                    cout << "Error: " << error << endl;
                }
                break;
            }

            case 4: { // Delete Master Record (Rider)
                cout << "Enter Rider ID: ";
                cin >> id;
                if (deleteRider(id, error)) {
                    cout << "Rider deleted successfully." << endl;
                } else {
                    cout << "Error: " << error << endl;
                }
                break;
            }

            case 5: { // Print All Master Records (Riders)
                if (PrintListOfRiders(error)) {
                    cout << "All riders displayed successfully." << endl;
                } else {
                    cout << "Error: " << error << endl;
                }
                break;
            }

            case 6: {
                cout << "Enter Rider ID: ";
                cin >> id;
                if (getRider(&rider, id, error)) {
                    inputRental(rental);
                    rental.setRiderId(rider.getId());

                    if (insertRental(rider, rental, error)) {
                        // Оновлюємо rentalFirstAddress тільки для першої оренди
                        if (rider.getRentalFirstAddress() == -1) {
                            rider.setRentalFirstAddress(rental.getSelfAddress());
                        }

                        rider.setRentalCount(rider.getRentalCount() + 1);
                        if (updateRider(rider, error, id)) {
                            cout << "Rental added\n";
                        } else {
                            cout << "Error updating rider: " << error << endl;
                        }
                    } else {
                        cout << "Error: " << error << endl;
                    }
                } else {
                    cout << "Error: " << error << endl;
                }
                break;
            }

            case 7: { // Get Slave Record (Rental)
                cout << "Enter Rider ID: ";
                cin >> id;
                if (getRider(&rider, id, error)) {
                    cout << "Enter Rental ID: ";
                    cin >> id;
                    if (validateRentalExists(rider, id, error)) {
                        outputRental(rental, rider);
                    } else {
                        cout << "Error: " << error << endl;
                    }
                } else {
                    cout << "Error: " << error << endl;
                }
                break;
            }

            case 8: { // Update Slave Record (Rental)
                cout << "Enter Rider ID: ";
                cin >> id;
                if (getRider(&rider, id, error)) {
                    cout << "Enter Rental ID: ";
                    cin >> id;
                    if (validateRentalExists(rider, id, error)) {
                        inputRental(rental);
                        cout << "Updated successfully\n";
                    } else {
                        cout << "Error: " << error << endl;
                    }
                } else {
                    cout << "Error: " << error << endl;
                }
                break;
            }

            case 9: { // Delete Rental
                cout << "Enter Rider ID: ";
                cin >> riderId;
                if (getRider(&rider, riderId, error)) {
                    cout << "Enter Rental ID: ";
                    cin >> rentalId;
                    if (getRental(rider, &rental, rentalId, error)) {
                        if (deleteRental(rider, rental, error)) {
                            cout << "Rental deleted!" << endl;
                        } else {
                            cerr << "Error: " << error << endl;
                        }
                    } else {
                        cerr << "Error: " << error << endl;
                    }
                } else {
                    cerr << "Error: " << error << endl;
                }
                break;
            }

                case 10: { // Print All Slave Records (Rentals)
                    cout << "Enter Rider ID: ";
                    cin >> id;
                    if (getRider(&rider, id, error)) {
                        if (rider.getRentalCount() != 0) {
                            PrintRentals(rider);
                        } else {
                            cout << "No rentals found\n";
                        }
                    } else {
                        cout << "Error: " << error << endl;
                    }
                    break;
                }

                case 11: { // Calculate Statistics
                    printStatistics();
                    break;
                }

                case 12: { // Exit
                    cout << "Exiting the program. Goodbye!" << endl;
                    return 0;
                }

                default: {
                    cout << "Invalid choice. Please try again." << endl;
                    break;
                }
            }
            cout << endl;
        }
    }
