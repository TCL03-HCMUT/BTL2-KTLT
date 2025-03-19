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

bool Utility::isSquare(int n)
{
    int a_sq = sqrt(n);
    return (a_sq * a_sq) == n;
}

// abstract class Unit
Unit::Unit(int quantity, int weight, Position pos)
{
    this->quantity = quantity;
    this->weight = weight;
    this->pos = pos;
    //this->attackScore = getAttackScore();
}

Unit::~Unit()
{

}

int Unit::getAttackScore()
{
    return 0;
}

Position Unit::getCurrentPosition() const
{
    return pos;
}

string Unit::str() const
{
    return "Unit";
}

UnitType Unit::instance()
{
    return UNIT;
}

VehicleType Unit::getVehicleType()
{
    return TRUCK;
}

InfantryType Unit::getInfantryType()
{
    return SNIPER;
}

int Unit::getCurrentScore()
{
    return this->attackScore;
}

void Unit::setAttackScore(int score)
{
    this->attackScore = score;
}

void Unit::multiplyScore(double multiplier)
{
    double tmpScore = this->attackScore;
    tmpScore *= multiplier;
    this->attackScore = (int)ceil(tmpScore);
}

void Unit::addScore(double num)
{
    double tmpScore = this->attackScore;
    tmpScore += num;
    this->attackScore = (int)ceil(tmpScore);
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
    this->attackScore = getAttackScore();
}

int Vehicle::getAttackScore()
{
    double score = (vehicleType * 304 + quantity * weight) / 30.0;
    return ceil(score);
}

string Vehicle::str() const
{
    stringstream result;
    result << "Vehicle[vehicleType=" << vehicleName << ",quantity=" << quantity 
        << ",weight=" << weight << ",pos=" << pos.str() << "]";
    return result.str();
}

UnitType Vehicle::instance()
{
    return VEHICLE;
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
    this->attackScore = getAttackScore();
}



bool Infantry::isCommando()
{
    Utility util;
    return (infantryType == SPECIALFORCES) && util.isSquare(weight);
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
        score = infantryType*56 + quantity*weight;
    }
    else if (personalNum < 3)
    {
        double quantity_tmp = quantity * 0.9;
        quantity = ceil(quantity_tmp);
        score = infantryType*56 + quantity*weight;
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

UnitType Infantry::instance()
{
    return INFANTRY;
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
    return EXP < 0 ? 0 : (EXP > 500 ? 500 : EXP);    
}

void Army::updateParameters()
{
    int LF_tmp = 0, EXP_tmp = 0;
    Node* tmp = unitList->getHead();
    while (tmp != NULL)
    {
        if (tmp->unit->instance() == VEHICLE)
        {
            LF_tmp += tmp->unit->getAttackScore();
        }
        else if (tmp->unit->instance() == INFANTRY)
        {
            EXP_tmp += tmp->unit->getAttackScore();
        }
        tmp = tmp->next;
    }
    LF_tmp = clampLF(LF_tmp);
    EXP_tmp = clampEXP(EXP_tmp);
    this->LF = LF_tmp;
    this->EXP = EXP_tmp;
}

Army::Army(Unit **unitArray, int size, string name, BattleField *battleField)
{
    Utility util;
    this->name = name;
    this->battleField = battleField;
    this->LF = 0;
    this->EXP = 0;
    
    for (int i = 0; i < size; i++)
    {
        if (unitArray[i]->instance() == VEHICLE)
        {
            this->LF += unitArray[i]->getAttackScore();
        }
        else if (unitArray[i]->instance() == INFANTRY)
        {
            this->EXP += unitArray[i]->getAttackScore();
        }
    }
    this->LF = clampLF(this->LF);
    this->EXP = clampEXP(this->EXP);
    int S = this->LF + this->EXP;
    bool isSpecialSize = util.isSpecialNumber(S, 3) || util.isSpecialNumber(S, 5) || util.isSpecialNumber(S, 7);
    int capacity = isSpecialSize ? 12 : 8;
    this->unitList = new UnitList(capacity);
    for (int i = 0; i < size; i++)
    {
        (this->unitList)->insert(unitArray[i]);
    }
}

int Army::getLF()
{
    return LF;
}

int Army::getEXP()
{
    return EXP;
}

void Army::setLF(int LF)
{
    this->LF = clampLF(LF);
}

void Army::setEXP(int EXP)
{
    this->EXP = clampEXP(EXP);
}

void Army::fight(Army *enemy, bool defense)
{

}

string Army::str() const
{
    return "Army";
}

ArmyType Army::instance()
{
    return ARMY;
}

void Army::multiplyLF(double multiplier)
{
    double temp = LF;
    temp *= multiplier;
    LF = clampLF((int)ceil(temp));
}

void Army::multiplyEXP(double multiplier)
{
    double temp = EXP;
    temp *= multiplier;
    EXP = clampEXP((int)ceil(temp));
}

void Army::addLF(double num)
{
    double temp = LF;
    temp += num;
    LF = clampLF((int)ceil(temp));
}

void Army::addEXP(double num)
{
    double temp = EXP;
    temp += num;
    EXP = clampEXP((int)ceil(temp));
}

Node* Army::getListHead()
{
    return this->unitList->getHead();
}

// class LiberationArmy
LiberationArmy::LiberationArmy(Unit **unitArray, int size, string name, BattleField *battleField) 
: Army(unitArray, size, name, battleField)
{
    
}

void LiberationArmy::fight(Army *enemy, bool defense)
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

ArmyType LiberationArmy::instance()
{
    return LIBERATIONARMY;
}


// class ARVN
ARVN::ARVN(Unit **unitArray, int size, string name, BattleField *battleField)
: Army(unitArray, size, name, battleField)
{

}

void ARVN::fight(Army *enemy, bool defense)
{
    //TODO: implement this method
}

string ARVN::str() const
{
    stringstream result;
    result << "ARVN[name=" << name << ",LF=" << LF << ",EXP=" << EXP 
        << ",unitList=" << unitList->str() << ",battleField=" << battleField->str();
    return result.str();
}

ArmyType ARVN::instance()
{
    return ARVIETNAM;
}


// class Position
Position::Position(int r, int c)
{
    this->r = r;
    this->c = c;
}

Position::Position(const string &str_pos)
{
    string pos_cpy = str_pos;
    pos_cpy.erase(pos_cpy.begin()); // Remove the first '('
    pos_cpy.erase(pos_cpy.end() - 1); // Remove the last ')'
    size_t commaPos = pos_cpy.find(',');
    this->r = stoi(pos_cpy.substr(0, commaPos));
    this->c = stoi(pos_cpy.substr(commaPos + 1));
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

double Position::getDistance(Position other)
{
    return sqrt(pow(this->getRow() - other.getRow(), 2) + pow(this->getCol() - other.getCol(),2));
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

void UnitList::setCapacity(int capacity)
{
    this->capacity = capacity;
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

    if (unit->instance() == VEHICLE)
    {
        insertAtEnd(unit);
        vehicleCount++;
        currentSize++;
    }
    else if (unit->instance() == INFANTRY)
    {
        insertAtHead(unit);
        infantryCount++;
        currentSize++;
    }
    return true;
}

bool UnitList::isContain(VehicleType vehicleType)
{
    Node *tmp = listHead;
    while (tmp != NULL)
    {
        if (tmp->unit->instance() == VEHICLE)
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
        if (tmp->unit->instance() == INFANTRY)
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
    result << "UnitList[" << "count_vehicle=" << vehicleCount << ";count_infantry=" << infantryCount << ";";
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

Node* UnitList::getHead()
{
    return listHead;
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

Road::Road() : TerrainElement()
{

}

void Road::getEffect(Army *army)
{

}

Mountain::Mountain(Position pos) : TerrainElement()
{
    this->pos = pos;
}

void Mountain::getEffect(Army *army)
{
    double tempLF = army->getLF();
    double tempEXP = army->getEXP();
    Node *tmp = army->getListHead();
    double distanceThreshold = (army->instance() == LIBERATIONARMY) ? 2.0 : 4.0;
    double infantryEXPMultiplier = (army->instance() == LIBERATIONARMY) ? 0.3 : 0.2;
    double vehicleLFMultiplier = (army->instance() == LIBERATIONARMY) ? -0.1 : -0.05;

    while (tmp != NULL)
    {
        if (this->pos.getDistance(tmp->unit->getCurrentPosition()) <= distanceThreshold)
        {
            if (tmp->unit->instance() == INFANTRY)
            {
                tempEXP += infantryEXPMultiplier * tmp->unit->getCurrentScore();
            }
            else if (tmp->unit->instance() == VEHICLE)
            {
                tempLF += vehicleLFMultiplier * tmp->unit->getCurrentScore();
            }
        }
        tmp = tmp->next;
    }
    army->setLF((int)ceil(tempLF));
    army->setEXP((int)ceil(tempEXP));
}

River::River(Position pos) : TerrainElement()
{
    this->pos = pos;
}

void River::getEffect(Army *army)
{
    Node *tmp = army->getListHead();
    while (tmp != NULL)
    {
        if (this->pos.getDistance(tmp->unit->getCurrentPosition()) <= 2)
        {
            if (tmp->unit->instance() == INFANTRY)
                tmp->unit->multiplyScore(0.9);
        }
        tmp = tmp->next;
    }
}


Urban::Urban(Position pos) : TerrainElement()
{
    this->pos = pos;
}

void Urban::getEffect(Army *army)
{
    Node *tmp = army->getListHead();
    while (tmp != NULL)
    {
        double distance = this->pos.getDistance(tmp->unit->getCurrentPosition());
        if (army->instance() == LIBERATIONARMY)
        {
            if (
                (tmp->unit->instance() == INFANTRY) &&
                (tmp->unit->getInfantryType() == SPECIALFORCES || tmp->unit->getInfantryType() == REGULARINFANTRY) &&
                (distance <= 5.0)
            )
            {
                tmp->unit->addScore( ((2.0*tmp->unit->getCurrentScore()) / distance) );
            }
            if (
                (tmp->unit->instance() == VEHICLE)  &&
                (tmp->unit->getVehicleType() == ARTILLERY) &&
                (distance <= 2.0)
            )
            {
                tmp->unit->multiplyScore(0.5);
            }
        }
        else if (army->instance() == ARVIETNAM)
        {
            if (
                (tmp->unit->instance() == INFANTRY) &&
                (tmp->unit->getInfantryType() == REGULARINFANTRY) &&
                (distance <= 3.0)
            )
            {
                tmp->unit->addScore( ((3.0*tmp->unit->getCurrentScore()) / (2.0*distance)) );
            }
        }
        tmp = tmp->next;
    }
}


Fortification::Fortification(Position pos) : TerrainElement()
{
    this->pos = pos;
}

void Fortification::getEffect(Army *army)
{
    Node *tmp = army->getListHead();
    while (tmp != NULL)
    {
        double distance = this->pos.getDistance(tmp->unit->getCurrentPosition());
        if (army->instance() == LIBERATIONARMY && distance <= 2.0)
        {
            tmp->unit->multiplyScore(0.8);
        }
        else if (army->instance() == ARVIETNAM && distance <= 2.0)
        {
            tmp->unit->multiplyScore(1.2);
        }
        tmp = tmp->next;
    }
}

SpecialZone::SpecialZone(Position pos)
{
    this->pos = pos;
}

void SpecialZone::getEffect(Army *army)
{
    Node *tmp = army->getListHead();
    while (tmp != NULL)
    {
        double distance = this->pos.getDistance(tmp->unit->getCurrentPosition());
        if (distance <= 1.0)
        {
            tmp->unit->setAttackScore(0);
        }
        tmp = tmp->next;
    }
}

// class Battlefield
BattleField::BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                        vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                        vector<Position *> arrayUrban, vector<Position *> arraySpecialZone)
{
    this->n_rows = n_rows;
    this->n_cols = n_cols;
    vector<vector<TerrainElement*>> terrain(n_rows, vector<TerrainElement*>(n_cols, NULL)); // vector<type> name(number,val)
    for (auto pos : arrayForest)
    {
        terrain[pos->getRow()][pos->getCol()] = new Mountain(*pos);
    }
    for (auto pos : arrayRiver)
    {
        terrain[pos->getRow()][pos->getCol()] = new River(*pos);
    }
    for (auto pos : arrayFortification)
    {
        terrain[pos->getRow()][pos->getCol()] = new Fortification(*pos);
    }
    for (auto pos : arrayUrban)
    {
        terrain[pos->getRow()][pos->getCol()] = new Urban(*pos);
    }
    for (auto pos : arraySpecialZone)
    {
        terrain[pos->getRow()][pos->getCol()] = new SpecialZone(*pos);
    }
}

BattleField::~BattleField()
{

}

string BattleField::str()
{
    stringstream result;
    result << "BattleField[n_rows=" << n_rows << ",n_cols=" << n_cols << "]";
    return result.str();
}


HCMCampaign::HCMCampaign(const string &config_file_path)
{

}

void HCMCampaign::run()
{

}

string HCMCampaign::printResult()
{
    return "";
}
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////