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

int Unit::getAttackScore()
{

}

Position Unit::getCurrentPosition() const
{
    return pos;
}

string Unit::str() const
{

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

string Vehicle::str() const
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
    result << "Vehicle[vehicleType=" << vehicleName << ";quantity=" << quantity 
    << ";weight=" << weight << ";pos=" << pos.str() << "]";
    return result.str();
}



// class Infantry
Infantry::Infantry(int quantity, int weight, const Position pos, InfantryType infantryType) : Unit(quantity,weight,pos)
{
    this->infantryType = infantryType;
}

bool isSquare(int a)
{
    int a_sq = sqrt(a);
    return (a_sq * a_sq) == a;
}

bool Infantry::isCommando()
{
    return (infantryType == SPECIALFORCES) && isSquare(weight);
}

int Infantry::getScore()
{
    int score = infantryType*56 + quantity*weight;
    if (isCommando())
    {
        score += 75;
    }
    return score;
}

int sumDigits(int n)
{
    int s = 0;
    while (n != 0)
    {
        s += n % 10;
        n /= 10;
    }
    return s;
}

int personalNumber(int num, int year)
{
    int result = sumDigits(num) + sumDigits(year);
    while (result > 9)
    {
        result = sumDigits(result);
    }
    return result;
}

int Infantry::getAttackScore()
{
    int score = getScore();
    int personalNum = personalNumber(score,1975);
    if (personalNum > 7)
    {
        double quantity_tmp = quantity * 1.2;
        quantity = ceil(quantity_tmp);
        score = getScore();
    }
    else if (personalNum < 3)
    {
        double quantity_tmp = quantity * 0.9;
        quantity = ceil(quantity_tmp);
        score = getScore();
    }
}
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////