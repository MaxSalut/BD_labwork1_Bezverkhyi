// Master.h
#ifndef MASTER_H
#define MASTER_H
using namespace std;
#include <cstddef> // Add this

struct Indexer {
    int id;
    int address;
    bool exists;
};

extern const size_t INDEXER_SIZE;
extern const size_t RIDER_SIZE;
extern const size_t RENTAL_SIZE;

constexpr const char* const RIDER_IND = "rider.ind";
constexpr const char* const RIDER_DATA = "rider.fl";
constexpr const char* const RIDER_GARBAGE = "rider_garbage.txt";
constexpr const char* const RENTAL_DATA = "rental.fl";
constexpr const char* const RENTAL_GARBAGE = "rental_garbage.txt";

void initializeDatabase();

#endif