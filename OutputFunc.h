#ifndef OUTPUTENTITIES_H
#define OUTPUTENTITIES_H

#include "RiderObject.h"
#include "RentalObject.h"

void outputRider(const RiderObject& rider);
void outputRental(const RentalObject& rental, const RiderObject& rider);

#endif