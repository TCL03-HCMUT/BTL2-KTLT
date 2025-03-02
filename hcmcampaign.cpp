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
Vehicle::Vehicle(int quantity, int weight, Position pos, VehicleType vehicleType) 
: Unit(quantity,weight,pos)
{
    this->vehicleType = vehicleType;
}

int Vehicle::getAttackScore()
{
    double score = (vehicleType * 304 + quantity * weight) / 30.0;
    return ceil(score);
}

string Vehicle::str() const
{
    stringstream result;
    string vehicleName;
    switch (vehicleType)
    {
    case TRUCK:
        vehicleName = "TRUCK";
        break;
    case MORTAR:
        vehicleName = "MORTAR";
        break;
    case ANTIAIRCRAFT:
        vehicleName = "ANTIAIRCRAFT";
        break;
    case ARMOREDCAR:
        vehicleName = "ARMOREDCAR";
        break;
    case APC:
        vehicleName = "APC";
        break;
    case ARTILLERY:
        vehicleName = "ARTILLERY";
        break;
    case TANK:
        vehicleName = "TANK";
        break;
    }
    result << "Vehicle[vehicleType=" << vehicleName << ";quantity=" << quantity 
    << ";weight=" << weight << ";pos=" << pos.str() << "]";
    return result.str();
}

string Vehicle::instance()
{
    return "Vehicle";
}


// class Infantry
Infantry::Infantry(int quantity, int weight, const Position pos, InfantryType infantryType) 
: Unit(quantity,weight,pos)
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
    int personalNum = personalNumber(score, 1975);
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
    return score;
}

string Infantry::str() const
{
    stringstream result;
    string infantryName;
    switch(infantryType)
    {
        case SNIPER:
            infantryName = "SNIPER";
            break;
        case ANTIAIRCRAFTSQUAD:
            infantryName = "ANTIAIRCRAFTSQUAD";
            break;
        case MORTARSQUAD:
            infantryName = "MORTARSQUAD";
            break;
        case ENGINEER:
            infantryName = "ENGINEER";
            break;
        case SPECIALFORCES:
            infantryName = "SPECIALFORCES";
            break;
        case REGULARINFANTRY:
            infantryName = "REGULARINFANTRY";
            break;
    }
    result << "Infantry[infantryType=" << infantryType << ",quantity=" << quantity 
    << ",weight=" << weight << ",pos=" << pos.str() << "]";
    return result.str();
}

string Infantry::instance()
{
    return "Infantry";
}


// class Army
Army::Army(Unit **unitArray, int size, string name, BattleField *battleField)
{
    this->name = name;
    this->battleField = battleField;
    for (int i = 0; i < size; i++)
    {
        unitList->insert(unitArray[i]);
    }
}

void Army::fight(Army *enemy, bool defense = false)
{

}

string Army::str() const
{

}


// class LiberationArmy
LiberationArmy::LiberationArmy(Unit **unitArray, int size, string name, BattleField *battleField) 
: Army(unitArray, size, name, battleField)
{
    
}

void LiberationArmy::fight(Army *enemy, bool defense = false)
{
    //TODO: implement this method
}

string LiberationArmy::str() const
{
    stringstream result;
    // TODO: deal with the stringstream
    // result << "LiberationArmy[name=" << name << ",LF=" << LF << "EXP"
}


// class Position
Position::Position(int r = 0, int c = 0)
{
    this->r = r;
    this->c = c;
}

Position::Position(const string &str_pos)
{
    string pos_cpy = str_pos;
    pos_cpy.erase(pos_cpy.begin());
    pos_cpy.erase(pos_cpy.end());
    pos_cpy.replace(pos_cpy.find(','), 1, ",", 1);
    stringstream nums(pos_cpy);
    nums >> this->r >> this->c;
}

int Position::getRow() const
{
    return r;
}

int Position::getCol() const
{
    return c;
}

void Position::setRow(int r)
{
    this->r = r;
}

void Position::setCol(int c)
{
    this->c = c;
}

string Position::str() const
{
    stringstream result;
    result << "(" << r << "," << c << ")";
    return result.str();
}
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////