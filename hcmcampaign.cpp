#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////


int Utility::sumDigits(int num)
{
    int sumDigit = 0;
    while (num != 0)
    {
        sumDigit += num % 10;
        num /= 10;
    }
    return sumDigit;
}

int Utility::personalNumber(int num, int year)
{
    int result = sumDigits(num) + sumDigits(year);
    while (result > 9)
    {
        result = sumDigits(result);
    }
    return result;
}

bool Utility::isSpecialNumber(int num, int base)
{
    // Edge case: 0 and negative numbers cannot be expressed as a sum of powers
    if (num < 0)
        return false;

    int remainder;
    while (num > 0)
    {
        remainder = num % base;

        // If any digit is greater than 1, it's not a special number
        if (remainder > 1)
            return false;
        
        num /= base;
    }
    return true;
}


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

string Unit::instance()
{
    return "Unit";
}

VehicleType Unit::getVehicleType()
{

}

InfantryType Unit::getInfantryType()
{

}

// class Vehicle
Vehicle::Vehicle(int quantity, int weight, Position pos, VehicleType vehicleType) 
: Unit(quantity,weight,pos)
{
    this->vehicleType = vehicleType;
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
}

int Vehicle::getAttackScore()
{
    double score = (vehicleType * 304 + quantity * weight) / 30.0;
    return ceil(score);
}

string Vehicle::str() const
{
    stringstream result;
    result << "Vehicle[vehicleType=" << vehicleName << ";quantity=" << quantity 
        << ";weight=" << weight << ";pos=" << pos.str() << "]";
    return result.str();
}

string Vehicle::instance()
{
    return "Vehicle";
}

VehicleType Vehicle::getVehicleType()
{
    return vehicleType;
}

// class Infantry
Infantry::Infantry(int quantity, int weight, const Position pos, InfantryType infantryType) 
: Unit(quantity,weight,pos)
{
    this->infantryType = infantryType;
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


int Infantry::getAttackScore()
{
    Utility util;
    int score = getScore();
    int personalNum = util.personalNumber(score, 1975);
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
    result << "Infantry[infantryType=" << infantryName << ",quantity=" << quantity 
        << ",weight=" << weight << ",pos=" << pos.str() << "]";
    return result.str();
}

string Infantry::instance()
{
    return "Infantry";
}

InfantryType Infantry::getInfantryType()
{
    return infantryType;
}

// class Army
int Army::clampLF(int LF)
{
    return LF < 0 ? 0 : (LF > 1000 ? 1000 : LF);
}

int Army::clampEXP(int EXP)
{
    return LF < 0 ? 0 : (LF > 500 ? 500 : LF);    
}


Army::Army(Unit **unitArray, int size, string name, BattleField *battleField)
{
    Utility util;
    this->name = name;
    this->battleField = battleField;
    for (int i = 0; i < size; i++)
    {
        if (unitArray[i]->instance() == "Vehicle")
        {
            this->LF += unitArray[i]->getAttackScore();
        }
        else if (unitArray[i]->instance() == "Infantry")
        {
            this->EXP += unitArray[i]->getAttackScore();
        }
    }
    this->LF = clampLF(LF);
    this->EXP = clampEXP(EXP);
    int S = this->LF + this->EXP;
    bool isSpecialSize = util.isSpecialNumber(S, 3) || util.isSpecialNumber(S, 5) || util.isSpecialNumber(S, 7);
    int capacity = isSpecialSize ? 12 : 8;
    *(this->unitList) = UnitList(capacity);
    for (int i = 0; i < size; i++)
    {
        (this->unitList)->insert(unitArray[i]);
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
    result << "LiberationArmy[name=" << name << ",LF=" << LF << ",EXP=" << EXP 
        << ",unitList=" << unitList->str() << ",battleField=" << battleField->str();
    return result.str();
}

// class ARVN
ARVN::ARVN(Unit **unitArray, int size, string name, BattleField *battleField)
: Army(unitArray, size, name, battleField)
{

}

void ARVN::fight(Army *enemy, bool defense = false)
{
    //TODO: implement this method
}

string ARVN::str() const
{
    stringstream result;
    result << "LiberationArmy[name=" << name << ",LF=" << LF << ",EXP=" << EXP 
        << ",unitList=" << unitList->str() << ",battleField=" << battleField->str();
    return result.str();
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

Node::Node()
{
    unit = NULL;
    next = NULL;
}

Node::Node(Unit *unit)
{
    this->unit = unit;
    this->next = NULL;
}
//class UnitList
UnitList::UnitList(int capacity)
{
    this->capacity = capacity;
    this->currentSize = 0;
    this->listHead = NULL;
    this->listEnd = NULL;
    this->vehicleCount = 0;
    this->infantryCount = 0;
}

void UnitList::insertAtHead(Unit *unit)
{
    if (listHead == NULL)
    {
        listHead = new Node(unit);
        listEnd = listHead;
        return;
    }
    Node *newNode = new Node(unit);
    newNode->next = listHead;
    listHead = newNode;
}

void UnitList::insertAtEnd(Unit *unit)
{
    if (listHead == NULL)
    {
        listHead = new Node(unit);
        listEnd = listHead;
        return;
    }
    Node *newNode = new Node(unit);
    listEnd->next = newNode;
    listEnd = listEnd->next;
    
}

bool UnitList::insert(Unit *unit)
{
    if (currentSize >= capacity)
    {
        return false;
    }

    if (unit->instance() == "Vehicle")
    {
        insertAtEnd(unit);
        vehicleCount++;
    }
    else if (unit->instance() == "Infantry")
    {
        insertAtHead(unit);
        infantryCount++;
    }
    return true;
}

bool UnitList::isContain(VehicleType vehicleType)
{
    Node *tmp = listHead;
    while (tmp != NULL)
    {
        if (tmp->unit->instance() == "Vehicle")
        {
            if (tmp->unit->getVehicleType() == vehicleType)
            {
                return true;
            }
        }
        tmp = tmp->next;
    }
    return false;
}

bool UnitList::isContain(InfantryType infantryType)
{
    Node *tmp = listHead;
    while (tmp != NULL)
    {
        if (tmp->unit->instance() == "Infantry")
        {
            if (tmp->unit->getInfantryType() == infantryType)
            {
                return true;
            }
        }
        tmp = tmp->next;
    }
    return false;
}

string UnitList::str() const
{
    stringstream result;
    result << "UnitList[" << "countvehicle=" << vehicleCount << ";count_infantry=" << infantryCount << ";";
    Node *tmp = listHead;
    while (tmp != NULL)
    {
        result << tmp->unit->str();
        if (tmp->next != NULL)
        {
            result << ",";
        }
        tmp = tmp->next;
    }
    result << "]";
    return result.str();
}

// class TerrainElement
TerrainElement::TerrainElement()
{

}

TerrainElement::~TerrainElement()
{

}

void TerrainElement::getEffect(Army *army)
{

}
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////