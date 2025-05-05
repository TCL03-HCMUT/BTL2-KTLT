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

string Unit::instance()
{
    return "UNIT";
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

int Unit::getQuantity()
{
    return this->quantity;
}

void Unit::addQuantity(int quantity)
{
    this->quantity += quantity;
}

void Unit::multiplyQuantity(double multiplier)
{
    double tmpQuantity = this->quantity;
    tmpQuantity *= multiplier;
    this->quantity = (int)ceil(tmpQuantity);
}

void Unit::setQuantity(int quantity)
{
    this->quantity = quantity;
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
        << ",weight=" << weight << ",position=" << pos.str() << "]";
    return result.str();
}

string Vehicle::instance()
{
    return "VEHICLE";
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
        << ",weight=" << weight << ",position=" << pos.str() << "]";
    return result.str();
}

string Infantry::instance()
{
    return "INFANTRY";
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
    while (tmp != nullptr)
    {
        if (tmp->unit->instance() == "VEHICLE")
        {
            LF_tmp += tmp->unit->getAttackScore();
        }
        else if (tmp->unit->instance() == "INFANTRY")
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
        if (unitArray[i]->instance() == "VEHICLE")
        {
            this->LF += unitArray[i]->getAttackScore();
        }
        else if (unitArray[i]->instance() == "INFANTRY")
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

string Army::instance()
{
    return "ARMY";
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
    if (defense) // defense is true
    {
        LF = (int)ceil(LF * 1.3);
        EXP = (int)ceil(EXP * 1.3);
        bool betterLF = this->LF >= enemy->getLF();
        bool betterEXP = this->EXP >= enemy->getEXP();
        if (betterLF && betterEXP)
        {
            // TODO: do the confiscation
        }
        else if (!betterLF && !betterEXP)
        {
            Node* tmp = this->unitList->getHead();
            while (tmp != nullptr)
            {
                int currentQuantity = tmp->unit->getQuantity();
                tmp->unit->setQuantity(closestFibonacci(currentQuantity));
                tmp = tmp->next;
            }
            updateParameters();
        }
        else
        {
            Node* tmp = this->unitList->getHead();
            while (tmp != nullptr)
            {
                tmp->unit->multiplyQuantity(0.9);
                tmp = tmp->next;
            }
            updateParameters();
        }
    }
    else
    {
        LF = (int)ceil(LF * 1.5);
        EXP = (int)ceil(EXP * 1.5);
        // TODO: implement the attack case
        vector<Node*> combinationA = unitList->findMinSubset(enemy->getEXP(), true);
        vector<Node*> combinationB = unitList->findMinSubset(enemy->getLF(), false);

        bool battleOccurs;

        if (combinationA.empty() && combinationB.empty())
        {
            battleOccurs = false;
        }
        else if (!combinationA.empty() && !combinationB.empty())
        {
            for (auto node : combinationA)
            {
                unitList->deleteNode(node);
            }

            for (auto node : combinationB)
            {
                unitList->deleteNode(node);
            }
            battleOccurs = true;
        }
        // TODO: need to fix a lot of this problem
        else if (combinationA.empty())
        {
            battleOccurs = this->EXP > enemy->getEXP();
        }
        else if (combinationB.empty())
        {
            battleOccurs = this->LF > enemy->getLF();
        }
    }
}

string LiberationArmy::str() const
{
    stringstream result;
    result << "LiberationArmy[name=" << name << ",LF=" << LF << ",EXP=" << EXP 
        << ",unitList=" << unitList->str() << ",battleField=" << battleField->str();
    return result.str();
}

string LiberationArmy::instance()
{
    return "LIBERATIONARMY";
}

int LiberationArmy::closestFibonacci(int num)
{
    int a = 0, b = 1, c;
    while (b <= num)
    {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

// class ARVN
ARVN::ARVN(Unit **unitArray, int size, string name, BattleField *battleField)
: Army(unitArray, size, name, battleField)
{

}

void ARVN::fight(Army *enemy, bool defense)
{
    //TODO: implement this method
    if (defense)
    {
        // TODO: implement this case
    }
    else
    {
        
        Node *tmp = unitList->getHead();
        while (tmp != nullptr)
        {
            tmp->unit->multiplyQuantity(0.8);
        }
        unitList->deleteMatchingQuantity(1);
        this->updateParameters();
    }
}

string ARVN::str() const
{
    stringstream result;
    result << "ARVN[name=" << name << ",LF=" << LF << ",EXP=" << EXP 
        << ",unitList=" << unitList->str() << ",battleField=" << battleField->str();
    return result.str();
}

string ARVN::instance()
{
    return "ARVN";
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
    unit = nullptr;
    next = nullptr;
}

Node::Node(Unit *unit)
{
    this->unit = unit;
    this->next = nullptr;
}


//class UnitList
UnitList::UnitList(int capacity)
{
    this->capacity = capacity;
    this->currentSize = 0;
    this->listHead = nullptr;
    this->listEnd = nullptr;
    this->vehicleCount = 0;
    this->infantryCount = 0;
}

void UnitList::setCapacity(int capacity)
{
    this->capacity = capacity;
}

void UnitList::insertAtHead(Unit *unit)
{
    if (listHead == nullptr)
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
    if (listHead == nullptr)
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

    if (unit->instance() == "VEHICLE")
    {
        VehicleType type = unit->getVehicleType();
        int addedQuantity = unit->getQuantity();
        if (this->isContain(type))
        {
            Node *tmp = listHead;
            while (tmp->unit->getVehicleType() != type)
            {
                tmp = tmp->next;
            }
            tmp->unit->addQuantity(addedQuantity);
            return false;
        }
        else
        {
            insertAtEnd(unit);
            vehicleCount++;
            currentSize++;
        }
    }
    else if (unit->instance() == "INFANTRY")
    {
        InfantryType type = unit->getInfantryType();
        int addedQuantity = unit->getQuantity();
        if (this->isContain(type))
        {
            Node *tmp = listHead;
            while (tmp->unit->getInfantryType() != type)
            {
                tmp = tmp->next;
            }
            tmp->unit->addQuantity(addedQuantity);
            return false;
        }
        else
        {
            insertAtHead(unit);
            infantryCount++;
            currentSize++;
        }
    }
    return true;
}

bool UnitList::isContain(VehicleType vehicleType)
{
    Node *tmp = listHead;
    while (tmp != nullptr)
    {
        if (tmp->unit->instance() == "VEHICLE")
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
    while (tmp != nullptr)
    {
        if (tmp->unit->instance() == "INFANTRY")
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

bool UnitList::deleteNode(Node *node)
{
    // Case 1: null or empty list
    if (listHead == nullptr || node == nullptr)
    {
        return false;
    }

    // Case 2: deletion at head
    if (node == listHead)
    {
        listHead = listHead->next;
        
        // if the head was also the tail (one node)
        if (node == listEnd)
        {
            listEnd = nullptr;
        }
        delete node;
        currentSize--;
        return true;
    }

    // Case 3: deletion at middle or tail
    Node *tmp = listHead;
    // Find the node before the node to delete
    while (tmp != nullptr && tmp->next != node)
    {
        tmp = tmp->next;
    }

    // Cannot find the correct node 
    if (tmp == nullptr)
    {
        return false;
    }

    // if deletion at tail
    if (node == listEnd)
    {
        listEnd = tmp; // move the tail to the previous node
    }

    tmp->next = node->next;
    delete node;
    currentSize--;
    return true;
}

bool UnitList::deleteMatchingQuantity(int quantity)
{
    Node *curr = listHead;
    while (curr != nullptr)
    {
        Node *next = curr->next;
        if (curr->unit->getQuantity() == quantity)
        {
            if (!deleteNode(curr))
            {
                return false;
            }
        }
        curr = next; // Move to the next node after handling the current one
    }
    return true;
}

string UnitList::str() const
{
    stringstream result;
    result << "UnitList[" << "count_vehicle=" << vehicleCount << ";count_infantry=" << infantryCount << ";";
    Node *tmp = listHead;
    while (tmp != nullptr)
    {
        result << tmp->unit->str();
        if (tmp->next != nullptr)
        {
            result << ",";
        }
        tmp = tmp->next;
    }
    result << "]";
    return result.str();
}

vector<Unit *> UnitList::convertToVector()
{
    vector<Unit*> result;
    Node* tmp = this->listHead;
    while (tmp != nullptr)
    {
        result.push_back(tmp->unit);
        tmp = tmp->next;
    }
    return result;
}

vector<Node *> UnitList::findMinSubset(int threshold, bool isInfantry)
{
    // vector<Unit*> units = convertToVector();
    int minSize = INT_MAX;
    int size = isInfantry ? infantryCount : vehicleCount;
    int lowBound = isInfantry ? 0 : infantryCount;
    vector<int> bestSubset;
    vector<Node*> result;

    // uses bitmasking (go trhough all combinations coded with binary numbers)
    for (int mask = 0; mask < (1 << size); mask++)
    {
        int sum = 0;
        vector<int> subsetIndices;
        for (int i = 0; i < size; i++)
        {
            if (mask & (1 << i))
            {
                sum += getNodeAtIndex(i + lowBound)->unit->getCurrentScore();
                subsetIndices.push_back(i + lowBound);
            }
        }

        if (sum > threshold && subsetIndices.size() < minSize)
        {
            minSize = subsetIndices.size();
            bestSubset = subsetIndices;
        }
    }
    
    if (bestSubset.empty())
    {
        return {};
    }

    for (auto index : bestSubset)
    {
        result.push_back(getNodeAtIndex(index));
    }

    return result;
}

Node *UnitList::getHead()
{
    return listHead;
}

Node *UnitList::getNodeAtIndex(int index)
{
    Node* tmp = listHead;
    for (int i = 0; i < index && tmp != nullptr ; i++)
    {
        tmp = tmp->next;
    }
    return tmp;
}

Node *UnitList::getFirstVehicle()
{
    return getNodeAtIndex(infantryCount);
}

// class TerrainElement
TerrainElement::TerrainElement(Position *pos)
{
    this->pos = pos;
}

TerrainElement::~TerrainElement()
{

}

void TerrainElement::getEffect(Army *army)
{

}

Road::Road() : TerrainElement(nullptr)
{

}

void Road::getEffect(Army *army)
{

}

Mountain::Mountain(Position *pos) : TerrainElement(pos)
{

}

void Mountain::getEffect(Army *army)
{
    double tempLF = army->getLF();
    double tempEXP = army->getEXP();
    Node *tmp = army->getListHead();
    double distanceThreshold = (army->instance() == "LIBERATIONARMY") ? 2.0 : 4.0;
    double infantryEXPMultiplier = (army->instance() == "LIBERATIONARMY") ? 0.3 : 0.2;
    double vehicleLFMultiplier = (army->instance() == "LIBERATIONARMY") ? -0.1 : -0.05;

    while (tmp != nullptr)
    {
        if (this->pos->getDistance(tmp->unit->getCurrentPosition()) <= distanceThreshold)
        {
            if (tmp->unit->instance() == "INFANTRY")
            {
                tempEXP += infantryEXPMultiplier * tmp->unit->getCurrentScore();
            }
            else if (tmp->unit->instance() == "VEHICLE")
            {
                tempLF += vehicleLFMultiplier * tmp->unit->getCurrentScore();
            }
        }
        tmp = tmp->next;
    }
    army->setLF((int)ceil(tempLF));
    army->setEXP((int)ceil(tempEXP));
}

River::River(Position *pos) : TerrainElement(pos)
{

}

void River::getEffect(Army *army)
{
    Node *tmp = army->getListHead();
    while (tmp != nullptr)
    {
        if (this->pos->getDistance(tmp->unit->getCurrentPosition()) <= 2)
        {
            if (tmp->unit->instance() == "INFANTRY")
                tmp->unit->multiplyScore(0.9);
        }
        tmp = tmp->next;
    }
}


Urban::Urban(Position *pos) : TerrainElement(pos)
{

}

void Urban::getEffect(Army *army)
{
    Node *tmp = army->getListHead();
    while (tmp != nullptr)
    {
        double distance = this->pos->getDistance(tmp->unit->getCurrentPosition());
        if (army->instance() == "LIBERATIONARMY")
        {
            if (
                (tmp->unit->instance() == "INFANTRY") &&
                (tmp->unit->getInfantryType() == SPECIALFORCES || tmp->unit->getInfantryType() == REGULARINFANTRY) &&
                (distance <= 5.0)
            )
            {
                tmp->unit->addScore( ((2.0*tmp->unit->getCurrentScore()) / distance) );
            }
            if (
                (tmp->unit->instance() == "VEHICLE")  &&
                (tmp->unit->getVehicleType() == ARTILLERY) &&
                (distance <= 2.0)
            )
            {
                tmp->unit->multiplyScore(0.5);
            }
        }
        else if (army->instance() == "ARVN")
        {
            if (
                (tmp->unit->instance() == "INFANTRY") &&
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


Fortification::Fortification(Position *pos) : TerrainElement(pos)
{

}

void Fortification::getEffect(Army *army)
{
    Node *tmp = army->getListHead();
    while (tmp != nullptr)
    {
        double distance = this->pos->getDistance(tmp->unit->getCurrentPosition());
        if (army->instance() == "LIBERATIONARMY" && distance <= 2.0)
        {
            tmp->unit->multiplyScore(0.8);
        }
        else if (army->instance() == "ARVN" && distance <= 2.0)
        {
            tmp->unit->multiplyScore(1.2);
        }
        tmp = tmp->next;
    }
}

SpecialZone::SpecialZone(Position *pos) : TerrainElement(pos)
{

}

void SpecialZone::getEffect(Army *army)
{
    Node *tmp = army->getListHead();
    while (tmp != nullptr)
    {
        double distance = this->pos->getDistance(tmp->unit->getCurrentPosition());
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

    // Resizes the 2d vector to match the size of the battlefield and initializes it with nullptr
    terrain.resize(n_rows);
    for (int i = 0; i < n_cols; i++)
    {
        terrain[i].resize(n_cols, nullptr);
    }

    // Initialize corresponding terrain elements based on the position
    for (auto pos : arrayForest)
    {
        terrain[pos->getRow()][pos->getCol()] = new Mountain(pos);
    }
    for (auto pos : arrayRiver)
    {
        terrain[pos->getRow()][pos->getCol()] = new River(pos);
    }
    for (auto pos : arrayFortification)
    {
        terrain[pos->getRow()][pos->getCol()] = new Fortification(pos);
    }
    for (auto pos : arrayUrban)
    {
        terrain[pos->getRow()][pos->getCol()] = new Urban(pos);
    }
    for (auto pos : arraySpecialZone)
    {
        terrain[pos->getRow()][pos->getCol()] = new SpecialZone(pos);
    }

    // If the remaining pointers are all nullptr, initialize to Road
    for (int i = 0; i < n_rows; i++)
    {
        for (int j = 0; j < n_cols; j++)
        {
            if (terrain[i][j] == nullptr)
            {
                terrain[i][j] = new Road;
            }
        }
    }
}

BattleField::~BattleField()
{
    for (int i = 0; i < n_rows; i++)
    {
        for (int j = 0; j < n_cols; j++)
        {
            if (terrain[i][j] != nullptr)
            {
                delete terrain[i][j];
                terrain[i][j] = nullptr;
            }
            
        }
    }
}

string BattleField::str()
{
    stringstream result;
    result << "BattleField[n_rows=" << n_rows << ",n_cols=" << n_cols << "]";
    return result.str();
}

vector<Position *> Configuration::toPositionVector(const string &positionList)
{
    vector<Position *> positions;
    stringstream ss(positionList);
    char ch;
    int row, col;

    // skip the opening '['
    ss >> ch;

    while (ss >> ch) {
        if (ch == '(') {
            // read row and column values
            ss >> row >> ch >> col >> ch; // read row, ',', col, ')'
            positions.push_back(new Position(row, col));
        }
    }

    return positions;
}

vector<string> Configuration::splitUnits(const string& unitListString)
{
    std::vector<std::string> units;
    int parenLevel = 0;
    std::string current;
    
    for (char c : unitListString) {
        if (c == ',' && parenLevel == 0) {
            units.push_back(current);
            current.clear();
        } else {
            if (c == '(') parenLevel++;
            if (c == ')') parenLevel--;
            current += c;
        }
    }
    
    if (!current.empty())
        units.push_back(current);

    return units;
}

bool Configuration::isInfantry(const string& unitName)
{
    static vector<string> infantryUnits = {"SNIPER", "ANTIAIRCRAFTSQUAD", "MORTARSQUAD", "ENGINEER", "SPECIALFORCES", "REGULARINFANTRY"};
    for (string unit : infantryUnits)
    {
        if (unit == unitName)
        {
            return true;
        }
    }
    return false;
}

InfantryType Configuration::getInfantryType(const string& unitName)
{
    if (unitName == "SNIPER")
    {
        return SNIPER;
    }
    else if (unitName == "ANTIAIRCRAFTSQUAD")
    {
        return ANTIAIRCRAFTSQUAD;
    }
    else if (unitName == "MORTARSQUAD")
    {
        return MORTARSQUAD;
    }
    else if (unitName == "ENGINEER")
    {
        return ENGINEER;
    }
    else if (unitName == "SPECIALFORCES")
    {
        return SPECIALFORCES;
    }
    else
    {
        return REGULARINFANTRY;
    }
}

VehicleType Configuration::getVehicleType(const string& unitName)
{
    if (unitName == "TRUCK")
    {
        return TRUCK;
    }
    else if (unitName == "MORTAR")
    {
        return MORTAR;
    }
    else if (unitName == "ANTIAIRCRAFT")
    {
        return ANTIAIRCRAFT;
    }
    else if (unitName == "ARMOREDCAR")
    {
        return ARMOREDCAR;
    }
    else if (unitName == "APC")
    {
        return APC;
    }
    else if (unitName == "ARTILLERY")
    {
        return ARTILLERY;
    }
    else
    {
        return TANK;
    }
}

void Configuration::parseUnits(vector<string> units)
{
    liberationCount = 0;
    ARVNCount = 0;

    liberationUnits = new Unit*[units.size()];
    ARVNUnits = new Unit*[units.size()];

    for (string unit: units)
    {
        size_t openParen = unit.find('(');
        size_t closingParen = unit.rfind(')');
        string unitName = unit.substr(0, openParen);
        string unitDetails = unit.substr(openParen + 1, closingParen - openParen - 1);

        // Extract the unit details
        char ch;
        stringstream ss(unitDetails);
        int quantity, weight, row, col, armyBelong;
        ss >> quantity >> ch >> weight >> ch >> ch >> row >> ch >> col >> ch >> ch >> armyBelong;
        
        Position pos(row,col);

        // Create the unit object
        Unit* unitObj = nullptr;
        if (isInfantry(unitName))
        {
            InfantryType infantryType = getInfantryType(unitName);
            unitObj = new Infantry(quantity, weight, pos, infantryType);
        }
        else
        {
            VehicleType vehicleType = getVehicleType(unitName);
            unitObj = new Vehicle(quantity, weight, pos, vehicleType);
        }

        // Add the unit object to the list
        if (armyBelong == 0)
        {
            liberationUnits[liberationCount++] = unitObj;
        }
        else
        {
            ARVNUnits[ARVNCount++] = unitObj;
        }
    }
}

Configuration::Configuration(const string &filepath)
{
    ifstream configFile(filepath);

    string line;

    while (getline(configFile, line))
    {
        int equalIndex = line.find('=');
        string lineCode = line.substr(0,equalIndex);
        string lineValue = line.substr(equalIndex + 1);
        if (lineCode == "NUM_ROWS")
        {
            num_rows = stoi(lineValue);
        }
        else if (lineCode == "NUM_COLS")
        {
            num_cols = stoi(lineValue);
        }
        
        else if (lineCode == "EVENT_CODE")
        {
            int temp = stoi(lineValue);

            if (temp > 99)
            {
                temp = temp & 100;
            }

            if (temp < 0)
            {
                temp = 0;
            }

            eventCode = temp;
        }

        else if (lineCode == "ARRAY_FOREST")
        {
            arrayForest = toPositionVector(lineValue);
        }
        else if (lineCode == "ARRAY_RIVER")
        {
            arrayRiver = toPositionVector(lineValue);
        }
        else if (lineCode == "ARRAY_FORTIFICATION")
        {
            arrayFortification = toPositionVector(lineValue);
        }
        else if (lineCode == "ARRAY_URBAN")
        {
            arrayUrban = toPositionVector(lineValue);
        }
        else if (lineCode == "ARRAY_SPECIAL_ZONE")
        {
            arraySpecialZone = toPositionVector(lineValue);
        }

        else if (lineCode == "UNIT_LIST")
        {
            // TODO: do this case
            // Remove the square brackets
            lineValue = lineValue.substr(1, lineValue.size() - 2);

            // Split the string into individual string
            vector<string> units = splitUnits(lineValue);

            parseUnits(units);
        }
    }
}

Configuration::~Configuration()
{
    for (auto pos: arrayForest)
    {
        delete pos;
    }
    for (auto pos: arrayRiver)
    {
        delete pos;
    }
    for (auto pos: arrayFortification)
    {
        delete pos;
    }
    for (auto pos: arrayUrban)
    {
        delete pos;
    }
    for (auto pos: arraySpecialZone)
    {
        delete pos;
    }
    
    for (int i = 0; i < liberationCount;i++)
    {
        delete liberationUnits[i];
    }
    delete[] liberationUnits;

    for (int i = 0; i < ARVNCount;i++)
    {
        delete ARVNUnits[i];
    }
    delete[] ARVNUnits;
}

vector<int> Configuration::getBattleFieldDimensions()
{
    return {num_rows,num_cols};
}

string Configuration::str() const
{
    stringstream result;
    // result << "Configuration[\n";
    result << "[";
    result << "num_rows=" << num_rows << ',';
    result << "num_cols=" << num_cols << ',';

    result << "arrayForest=[";
    for (int i = 0; i < arrayForest.size(); i++)
    {
        result << arrayForest[i]->str();
        if (i < arrayForest.size() - 1)
            result << ',';
    }
    result << "],";

    result << "arrayRiver=[";
    for (int i = 0; i < arrayRiver.size(); i++)
    {
        result << arrayRiver[i]->str();
        if (i < arrayRiver.size() - 1)
            result << ',';
    }
    result << "],";

    result << "arrayUrban=[";
    for (int i = 0; i < arrayUrban.size(); i++)
    {
        result << arrayUrban[i]->str();
        if (i < arrayUrban.size() - 1)
            result << ',';
    }
    result << "],";

    result << "arrayFortification=[";
    for (int i = 0; i < arrayFortification.size(); i++)
    {
        result << arrayFortification[i]->str();
        if (i < arrayFortification.size() - 1)
            result << ',';
    }
    result << "],";

    result << "arraySpecialZone=[";
    for (int i = 0; i < arraySpecialZone.size(); i++)
    {
        result << arraySpecialZone[i]->str();
        if (i < arraySpecialZone.size() - 1)
            result << ',';
    }
    result << "],";

    result << "liberationUnits=[";
    for (int i = 0; i < liberationCount; i++)
    {
        result << liberationUnits[i]->str();
        if (i < liberationCount - 1)
        {
            result << ',';
        }
    }
    result << "],";

    result << "ARVNUnits=[";
    for (int i = 0; i < ARVNCount; i++)
    {
        result << ARVNUnits[i]->str();
        if (i < ARVNCount - 1)
        {
            result << ',';
        }
    }
    result << "],";

    result << "eventCode=" << eventCode << ']';

    return result.str();
}

// HCMCampaign
HCMCampaign::HCMCampaign(const string &config_file_path)
{
    config = new Configuration(config_file_path);
}

void HCMCampaign::run()
{
    // TODO:
}

string HCMCampaign::printResult()
{
    stringstream result;
    result << "LIBERATIONARMY[LF=" << liberationArmy->getLF() << ",EXP=" << liberationArmy->getEXP() 
           << "]-ARVN[LF=" << ARVN->getLF() << ",EXP=" << ARVN->getEXP() << "]";

    return result.str();
}
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////