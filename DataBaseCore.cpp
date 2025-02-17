#include "DataBaseCore.h"
#include <fstream>

#include "RentalObject.h"
#include "RiderObject.h"

const size_t INDEXER_SIZE = sizeof(Indexer);
const size_t RIDER_SIZE = sizeof(RiderObject);
const size_t RENTAL_SIZE = sizeof(RentalObject);

void initializeDatabase() {
    ofstream file;

    // Створюємо файл індексів, якщо його немає
    file.open(RIDER_IND, ios::app);
    if (file.is_open()) file.close();

    // Створюємо файл даних, якщо його немає
    file.open(RIDER_DATA, ios::app);
    if (file.is_open()) file.close();

    // Створюємо файл з видаленими ID, якщо його немає
    file.open(RIDER_GARBAGE, ios::app);
    if (file.is_open()) {
        if (file.tellp() == 0) file << "0"; // Якщо файл порожній, записуємо "0"
        file.close();
    }

    // Створюємо файл оренд, якщо його немає
    file.open(RENTAL_DATA, ios::app);
    if (file.is_open()) file.close();

    // Створюємо файл для зберігання видалених оренд, якщо його немає
    file.open(RENTAL_GARBAGE, ios::app);
    if (file.is_open()) {
        if (file.tellp() == 0) file << "0"; // Якщо файл порожній, записуємо "0"
        file.close();
    }
}
