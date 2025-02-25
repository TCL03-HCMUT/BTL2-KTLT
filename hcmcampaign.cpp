#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////



// abstract class Unit
Unit::Unit(int quantity, int weight, Position pos)
{
    this->quantity = quantity;
    this->weight = weight;
    this->pos = pos;
}

Unit::~Unit()
{

}

Position Unit::getCurrentPosition() const
{
    return pos;
}


// class Vehicle
Vehicle::Vehicle(int quantity, int weight, Position pos, VehicleType vehicleType) : Unit(quantity,weight,pos)
{
    this->vehicleType = vehicleType;
}

int Vehicle::getAttackScore()
{
    return vehicleType * 304 + quantity * weight;
}

string Vehicle::str()
{
    stringstream result;
    string vehicleName;
    switch (vehicleType)
    {
    case 0:
        vehicleName = "TRUCK";
        break;
    case 1:
        vehicleName = "MORTAR";
        break;
    case 2:
        vehicleName = "ANTIAIRCRAFT";
        break;
    case 3:
        vehicleName = "ARMOREDCAR";
        break;
    case 4:
        vehicleName = "APC";
        break;
    case 5:
        vehicleName = "ARTILLERY";
        break;
    case 6:
        vehicleName = "TANK";
        break;
    
    default:
        break;
    }
    result << "Vehicle[vehicleType=" << vehicleName << ",quantity=" << quantity 
    << ",weight=" << weight << ",pos=" << pos.str() << "]";
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////