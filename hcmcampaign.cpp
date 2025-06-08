#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////
// abstract class Unit
Unit::Unit(int quantity, int weight, Position pos) : quantity(quantity), weight(weight), pos(pos)
{
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
    return "";
}

string Unit::instance()
{
    return "UNIT";
}

VehicleType Unit::getVehicleType() const
{
    return TRUCK;
}

InfantryType Unit::getInfantryType() const
{
    return SNIPER;
}

int Unit::getCurrentScore() const
{
    return this->attackScore;
}

int Unit::getQuantity() const
{
    return this->quantity;
}

int Unit::getWeight() const
{
    return this->weight;
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

void Unit::multiplyWeight(double multiplier)
{
    double tmpWeight = this->weight;
    tmpWeight *= multiplier;
    this->weight = (int)ceil(tmpWeight);
}

void Unit::setWeight(int weight)
{
    this->weight = weight;
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

bool Unit::isAffected(int type)
{
    switch (type)
    {
    case 1:
        return affectedForest;
    case 2:
        return affectedRiver;
    case 3:
        return affectedFortification;
    case 4:
        return affectedRiver;
    default:
        return affectedSpecialZone;
    }
}

void Unit::setAffected(int type)
{
    switch (type)
    {
    case 1:
        affectedForest = true;
        break;
    case 2:
        affectedRiver = true;
        break;
    case 3:
        affectedFortification = true;
        break;
    case 4:
        affectedUrban = true;
        break;
    default:
        affectedSpecialZone = true;
        break;
    }
}

// class Vehicle
Vehicle::Vehicle(int quantity, int weight, Position pos, VehicleType vehicleType)
    : Unit(quantity, weight, pos), vehicleType(vehicleType)
{
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
    this->attackScore = (int)ceil(score);
    return (int)ceil(score);
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

VehicleType Vehicle::getVehicleType() const
{
    return vehicleType;
}

// class Infantry
Infantry::Infantry(int quantity, int weight, const Position pos, InfantryType infantryType)
    : Unit(quantity, weight, pos), infantryType(infantryType)
{
    switch (infantryType)
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

bool Infantry::isSquare(int num)
{
    int a_sq = sqrt(num);
    return (a_sq * a_sq) == num;
}

bool Infantry::isCommando()
{
    return (infantryType == SPECIALFORCES) && isSquare(weight);
}

int Infantry::getScore()
{
    int score = infantryType * 56 + quantity * weight;

    return score;
}

int Infantry::sumDigits(int num)
{
    int sumDigit = 0;
    while (num != 0)
    {
        sumDigit += num % 10;
        num /= 10;
    }
    return sumDigit;
}

int Infantry::personalNumber(int num, int year)
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
    if (isCommando())
    {
        score += 75;
    }
    int personalNum = personalNumber(score, 1975);
    if (personalNum > 7)
    {
        double quantity_tmp = quantity * 1.2;
        quantity = (int)ceil(quantity_tmp);
    }
    else if (personalNum < 3)
    {
        double quantity_tmp = quantity * 0.9;
        quantity = (int)ceil(quantity_tmp);
    }
    score = getScore();
    this->attackScore = score;
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

InfantryType Infantry::getInfantryType() const
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
    Node *tmp = unitList->getHead();
    while (tmp != nullptr)
    {
        if (tmp->unit->instance() == "VEHICLE")
        {
            LF_tmp += tmp->unit->getAttackScore();
        }
        else if (tmp->unit->instance() == "INFANTRY")
        {
            if (tmp->unit->updated == false)
                EXP_tmp += tmp->unit->getAttackScore();
            else
            {
                EXP_tmp += tmp->unit->getCurrentScore();
                tmp->unit->updated = false;
            }
        }
        tmp = tmp->next;
    }
    LF_tmp = clampLF(LF_tmp);
    EXP_tmp = clampEXP(EXP_tmp);
    this->LF = LF_tmp;
    this->EXP = EXP_tmp;
}

void Army::updateAttackScores()
{
    Node *tmp = unitList->getHead();
    while (tmp != nullptr)
    {
        // if (tmp->unit->instance() == "VEHICLE")
        // {
        //     tmp->unit->getAttackScore();
        // }
        // else if (tmp->unit->instance() == "INFANTRY")
        // {
        //     tmp->unit->getAttackScore();
        // }
        if (tmp->unit->updated)
        {
            tmp->unit->updated = false;
        }
        else
        {
            tmp->unit->getAttackScore();
        }
        tmp = tmp->next;
    }
}

bool Army::isSpecialNumber(int num, int base)
{
    if (num < 0)
        return false;

    // This is essentially converting a number to other bases but the only digits that are valid are 0 and 1
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

Army::Army(Unit **unitArray, int size, string name, BattleField *battleField) : name(name), battleField(battleField), LF(0), EXP(0)
{
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

    bool isSpecialSize = isSpecialNumber(S, 3) || isSpecialNumber(S, 5) || isSpecialNumber(S, 7);
    int capacity = isSpecialSize ? 12 : 8;
    this->unitList = new UnitList(capacity);

    for (int i = 0; i < size; i++)
    {
        (this->unitList)->insert(unitArray[i]);
    }
    updateAttackScores();
}

Army::~Army()
{
    delete unitList;
}

int Army::getLF() const
{
    return LF;
}

int Army::getEXP() const
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

void Army::removeUnitsAfterFight()
{
    unitList->deleteLowerScore(5);
}

string Army::str() const
{
    return "";
}

string Army::instance()
{
    return "ARMY";
}

Node *Army::getListHead() const
{
    return this->unitList->getHead();
}

UnitList *Army::getList() const
{
    return unitList;
}

// class LiberationArmy
LiberationArmy::LiberationArmy(Unit **unitArray, int size, string name, BattleField *battleField)
    : Army(unitArray, size, name, battleField)
{
}

void LiberationArmy::fight(Army *enemy, bool defense)
{
    if (defense) // defense case
    {
        LF = clampLF((int)ceil(LF * 1.3));
        EXP = clampLF((int)ceil(EXP * 1.3));
        bool betterLF = this->LF >= enemy->getLF();
        bool betterEXP = this->EXP >= enemy->getEXP();
        if (betterLF && betterEXP)
        {
        }
        else if (!betterLF && !betterEXP)
        {
            Node *temp = this->unitList->getHead();
            while (temp != nullptr)
            {
                int currentQuantity = temp->unit->getQuantity();
                temp->unit->setQuantity(closestFibonacci(currentQuantity));
                temp = temp->next;
            }
        }
        else
        {
            Node *temp = this->unitList->getHead();
            while (temp != nullptr)
            {
                temp->unit->multiplyQuantity(0.9);
                temp = temp->next;
            }
        }
    }
    else // attack case
    {
        LF = clampEXP((int)ceil(LF * 1.5));
        EXP = clampEXP((int)ceil(EXP * 1.5));
        vector<Node *> combinationA = unitList->findMinSubset(enemy->getEXP(), true);
        vector<Node *> combinationB = unitList->findMinSubset(enemy->getLF(), false);

        // Evaluate if battle occurs, and delete units if applicable
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
        else if (combinationA.empty())
        {
            battleOccurs = (this->EXP) > (enemy->getEXP());

            if (battleOccurs)
            {
                this->unitList->deleteAllInfantry();
                for (auto node : combinationB)
                {
                    unitList->deleteNode(node);
                }
            }
        }

        else if (combinationB.empty())
        {
            battleOccurs = (this->LF) > (enemy->getLF());

            if (battleOccurs)
            {
                this->unitList->deleteAllVehicle();
                for (auto node : combinationA)
                {
                    unitList->deleteNode(node);
                }
            }
        }

        // Evaluate the battle entirely
        if (battleOccurs)
        {
            confiscate(enemy);
        }
        else
        {
            Node *temp = this->unitList->getHead();
            while (temp != nullptr)
            {
                temp->unit->multiplyWeight(0.9);
                temp = temp->next;
            }
        }
    }
    updateParameters();
}

void LiberationArmy::confiscate(Army *enemy)
{
    UnitList *enemyList = enemy->getList();

    enemyList->reverse();
    Node *temp = enemyList->getHead();
    while (temp != nullptr)
    {
        Node *next = temp->next;
        // bool merged;
        // if (temp->unit->instance() == "VEHICLE")
        // {
        //     merged = this->unitList->isContain(temp->unit->getVehicleType());
        // }
        // else
        // {
        //     merged = this->unitList->isContain(temp->unit->getInfantryType());
        // }
        if (this->unitList->insert(temp->unit) || this->unitList->merged)
        {
            enemyList->deleteNode(temp);
        }

        temp = next;
    }
    enemyList->reverse();
}

string LiberationArmy::str() const
{
    stringstream result;
    result << "LiberationArmy[LF=" << LF << ",EXP=" << EXP
           << ",unitList=" << unitList->str()
           << ",battleField=" << ((battleField == NULL || battleField == nullptr) ? "" : battleField->str()) << "]";
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
    if (defense) // defense case
    {
        // confiscation has already happened in Liberation Army
        if (enemy->battleOccurs)
        {
            Node *temp = unitList->getHead();
            while (temp != nullptr)
            {
                temp->unit->multiplyWeight(0.8);
                temp = temp->next;
            }
        }
    }
    else // attack case
    {
        Node *temp = unitList->getHead();
        while (temp != nullptr)
        {
            temp->unit->multiplyQuantity(0.8);
            temp = temp->next;
        }
        unitList->deleteMatchingQuantity(1);
    }

    updateParameters();
}

string ARVN::str() const
{
    stringstream result;
    result << "ARVN[LF=" << LF << ",EXP=" << EXP
           << ",unitList=" << unitList->str()
           << ",battleField=" << ((battleField == NULL || battleField == nullptr) ? "" : battleField->str()) << "]";
    return result.str();
}

string ARVN::instance()
{
    return "ARVN";
}

// class Position
Position::Position(int r, int c) : r(r), c(c)
{
}

Position::Position(const string &str_pos)
{
    stringstream ss(str_pos);
    char ch;
    ss >> ch >> r >> ch >> c >> ch;
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

double Position::getDistance(Position other) const
{
    return sqrt(pow(this->getRow() - other.getRow(), 2) + pow(this->getCol() - other.getCol(), 2));
}

Node::Node(Unit *unit) : unit(unit)
{
}

// class UnitList
UnitList::UnitList(int capacity) : capacity(capacity)
{
}

UnitList::~UnitList()
{
    while (listHead != nullptr)
    {
        Node *temp = listHead;
        listHead = listHead->next;
        delete temp;
    }
    listEnd = nullptr;
    currentSize = 0;
    infantryCount = 0;
    vehicleCount = 0;
}

Node *UnitList::operator[](int index)
{
    if (index < 0 || index >= currentSize)
    {
        return nullptr;
    }

    Node *temp = listHead;
    for (int i = 0; i < index && temp != nullptr; i++)
    {
        temp = temp->next;
    }
    return temp;
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
    // if (currentSize >= capacity)
    // {
    //     return false;
    // }

    if (unit->instance() == "VEHICLE")
    {
        VehicleType type = unit->getVehicleType();
        int addedQuantity = unit->getQuantity();
        if (this->isContain(type))
        {
            Node *temp = listHead;
            while (temp->unit->getVehicleType() != type)
            {
                temp = temp->next;
            }
            temp->unit->addQuantity(addedQuantity);
            temp->unit->setWeight(max(temp->unit->getWeight(), unit->getWeight()));
            temp->unit->getAttackScore();
            temp->unit->updated = true;
            merged = true;
            return false;
        }
        else
        {
            merged = false;
            if (currentSize < capacity)
            {
                insertAtEnd(unit);
                vehicleCount++;
                currentSize++;
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    else if (unit->instance() == "INFANTRY")
    {
        InfantryType type = unit->getInfantryType();
        int addedQuantity = unit->getQuantity();
        if (this->isContain(type))
        {
            Node *temp = listHead;
            while (temp->unit->getInfantryType() != type)
            {
                temp = temp->next;
            }
            temp->unit->addQuantity(addedQuantity);
            temp->unit->setWeight(max(temp->unit->getWeight(), unit->getWeight()));
            temp->unit->getAttackScore();
            temp->unit->updated = true;
            merged = true;
            return false;
        }
        else
        {
            merged = false;
            if (currentSize < capacity)
            {
                insertAtHead(unit);
                infantryCount++;
                currentSize++;
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    return true;
}

bool UnitList::isContain(VehicleType vehicleType)
{
    Node *temp = listHead;
    while (temp != nullptr)
    {
        if (temp->unit->instance() == "VEHICLE")
        {
            if (temp->unit->getVehicleType() == vehicleType)
            {
                return true;
            }
        }
        temp = temp->next;
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

void UnitList::deleteNode(Node *&node)
{
    // Case 1: null or empty list
    if (listHead == nullptr || node == nullptr)
    {
        return;
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

        if (node->unit->instance() == "INFANTRY")
        {
            infantryCount--;
        }
        else
        {
            vehicleCount--;
        }
        delete node;
        node = nullptr;
        currentSize--;
        return;
    }

    // Case 3: deletion at middle or tail
    Node *prev = listHead;
    // Find the node before the node to delete
    while (prev != nullptr && prev->next != node)
    {
        prev = prev->next;
    }

    // Cannot find the correct node
    if (prev == nullptr)
    {
        return;
    }

    // if deletion at tail
    if (node == listEnd)
    {
        listEnd = prev; // move the tail to the previous node
    }

    prev->next = node->next;
    if (node->unit->instance() == "INFANTRY")
    {
        infantryCount--;
    }
    else
    {
        vehicleCount--;
    }
    delete node;
    node = nullptr;
    currentSize--;
    return;
}

void UnitList::deleteAllInfantry()
{
    Node *current = listHead;

    while (current != nullptr)
    {
        Node *next = current->next;
        if (current->unit->instance() == "INFANTRY")
        {
            deleteNode(current);
        }
        current = next;
    }
    infantryCount = 0;
}

void UnitList::deleteAllVehicle()
{
    Node *current = listHead;

    while (current != nullptr)
    {
        Node *next = current->next;
        if (current->unit->instance() == "VEHICLE")
        {
            deleteNode(current);
        }
        current = next;
    }
    vehicleCount = 0;
}

void UnitList::deleteMatchingQuantity(int quantity)
{
    Node *temp = listHead;
    while (temp != nullptr)
    {
        Node *next = temp->next;
        if (temp->unit->getQuantity() == quantity)
        {
            deleteNode(temp);
        }
        temp = next;
    }
}

void UnitList::deleteLowerScore(int score)
{
    Node *temp = listHead;
    while (temp != nullptr)
    {
        Node *next = temp->next;
        if (temp->unit->getAttackScore() <= score)
        {
            deleteNode(temp);
        }
        temp = next;
    }
}

void UnitList::reverse()
{
    Node *prev = nullptr, *current = listHead, *next;
    listEnd = listHead;

    while (current != nullptr)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    listHead = prev;
}

string UnitList::str() const
{
    stringstream result;
    result << "UnitList[" << "count_vehicle=" << vehicleCount
           << ";count_infantry=" << infantryCount << ((currentSize > 0) ? ";" : "");
    Node *temp = listHead;
    while (temp != nullptr)
    {
        result << temp->unit->str();
        if (temp->next != nullptr)
        {
            result << ",";
        }
        temp = temp->next;
    }
    result << "]";
    return result.str();
}

vector<Unit *> UnitList::convertToVector()
{
    vector<Unit *> result;
    Node *temp = this->listHead;
    while (temp != nullptr)
    {
        result.push_back(temp->unit);
        temp = temp->next;
    }
    return result;
}

vector<Node *> UnitList::findMinSubset(int threshold, bool isInfantry)
{
    vector<Unit *> units = convertToVector();
    int minSize = INT_MAX;
    int size = isInfantry ? infantryCount : vehicleCount;
    int lowBound = isInfantry ? 0 : infantryCount;
    vector<int> bestSubset;
    vector<Node *> result;

    // uses bitmasking (go trhough all combinations coded with binary numbers)
    for (int mask = 0; mask < (1 << size); mask++)
    {
        int sum = 0;
        vector<int> subsetIndices;
        for (int i = 0; i < size; i++)
        {
            if (mask & (1 << i))
            {
                sum += units[i + lowBound]->getCurrentScore();
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
        result.push_back(((*this)[index]));
    }

    return result;
}

Node *UnitList::getHead() const
{
    return listHead;
}

// class TerrainElement
TerrainElement::TerrainElement(Position *pos) : pos(pos)
{
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

Road::Road(Position *pos) : TerrainElement(pos)
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

    double tempLF = (double)army->getLF();
    double tempEXP = (double)army->getEXP();
    Node *temp = army->getListHead();
    double distanceThreshold = (army->instance() == "LIBERATIONARMY") ? 2.0 : 4.0;
    double infantryEXPMultiplier = (army->instance() == "LIBERATIONARMY") ? 0.3 : 0.2;
    double vehicleLFMultiplier = (army->instance() == "LIBERATIONARMY") ? -0.1 : -0.05;

    while (temp != nullptr)
    {
        // if (!temp->unit->isAffected(1))
        // {
            double distance = this->pos->getDistance(temp->unit->getCurrentPosition());
            if (distance <= distanceThreshold)
            {
                if (temp->unit->instance() == "INFANTRY")
                {
                    tempEXP = ceil(tempEXP + infantryEXPMultiplier * temp->unit->getCurrentScore());
                }
                else if (temp->unit->instance() == "VEHICLE")
                {
                    tempLF = ceil(tempLF + vehicleLFMultiplier * temp->unit->getCurrentScore());
                }
            }
        //     temp->unit->setAffected(1);
        // }
        temp = temp->next;
    }
    army->setLF((int)ceil(tempLF));
    army->setEXP((int)ceil(tempEXP));
}

River::River(Position *pos) : TerrainElement(pos)
{
}

void River::getEffect(Army *army)
{
    Node *temp = army->getListHead();
    while (temp != nullptr)
    {
        // if (!temp->unit->isAffected(2))
        // {
            double distance = this->pos->getDistance(temp->unit->getCurrentPosition());
            if (distance <= 2)
            {
                if (temp->unit->instance() == "INFANTRY")
                    temp->unit->multiplyScore(0.9);
            }
        //     temp->unit->setAffected(2);
        // }
        temp = temp->next;
    }
}

Urban::Urban(Position *pos) : TerrainElement(pos)
{
}

void Urban::getEffect(Army *army)
{
    Node *temp = army->getListHead();
    while (temp != nullptr)
    {
        // if (!temp->unit->isAffected(4))
        // {
            double distance = this->pos->getDistance(temp->unit->getCurrentPosition());
            if (army->instance() == "LIBERATIONARMY")
            {
                if (
                    (temp->unit->instance() == "INFANTRY") &&
                    (temp->unit->getInfantryType() == SPECIALFORCES || temp->unit->getInfantryType() == REGULARINFANTRY) &&
                    (distance <= 5.0))
                {
                    temp->unit->addScore(((2.0 * temp->unit->getCurrentScore()) / distance));
                }
                if (
                    (temp->unit->instance() == "VEHICLE") &&
                    (temp->unit->getVehicleType() == ARTILLERY) &&
                    (distance <= 2.0))
                {
                    temp->unit->multiplyScore(0.5);
                }
            }
            else if (army->instance() == "ARVN")
            {
                if (
                    (temp->unit->instance() == "INFANTRY") &&
                    (temp->unit->getInfantryType() == REGULARINFANTRY) &&
                    (distance <= 3.0))
                {
                    temp->unit->addScore(((3.0 * temp->unit->getCurrentScore()) / (2.0 * distance)));
                }
            }
            // temp->unit->setAffected(4);
        // }
        temp = temp->next;
    }
}

Fortification::Fortification(Position *pos) : TerrainElement(pos)
{
}

void Fortification::getEffect(Army *army)
{
    Node *temp = army->getListHead();
    while (temp != nullptr)
    {
        // if (!tmp->unit->isAffected(3))
        // {
            double distance = this->pos->getDistance(temp->unit->getCurrentPosition());
            if (army->instance() == "LIBERATIONARMY" && distance <= 2.0)
            {
                temp->unit->multiplyScore(0.8);
            }
            else if (army->instance() == "ARVN" && distance <= 2.0)
            {
                temp->unit->multiplyScore(1.2);
            }
        //     tmp->unit->setAffected(3);
        // }
        temp = temp->next;
    }
}

SpecialZone::SpecialZone(Position *pos) : TerrainElement(pos)
{
}

void SpecialZone::getEffect(Army *army)
{
    Node *temp = army->getListHead();
    while (temp != nullptr)
    {
        // if (!tmp->unit->isAffected(5))
        // {
            double distance = this->pos->getDistance(temp->unit->getCurrentPosition());
            if (distance <= 1.0)
            {
                temp->unit->setAttackScore(0);
            }
            // tmp->unit->setAffected(5);
        // }
        temp = temp->next;
    }
}

// class Battlefield
BattleField::BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                         vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                         vector<Position *> arrayUrban, vector<Position *> arraySpecialZone) 
                         : n_rows(n_rows), n_cols(n_cols), arrayForest(arrayForest), arrayRiver(arrayRiver),
                         arrayFortification(arrayFortification), arrayUrban(arrayUrban), arraySpecialZone(arraySpecialZone)
{
    // Resizes the 2d vector to match the size of the battlefield and initializes it with nullptr
    terrain.resize(n_rows);
    for (int i = 0; i < n_rows; i++)
    {
        terrain[i].resize(n_cols, nullptr);
    }

    // Initialize corresponding terrain elements based on the position
    for (auto &pos : arrayForest)
    {
        terrain[pos->getRow()][pos->getCol()] = new Mountain(pos);
    }
    for (auto &pos : arrayRiver)
    {
        terrain[pos->getRow()][pos->getCol()] = new River(pos);
    }
    for (auto &pos : arrayFortification)
    {
        terrain[pos->getRow()][pos->getCol()] = new Fortification(pos);
    }
    for (auto &pos : arrayUrban)
    {
        terrain[pos->getRow()][pos->getCol()] = new Urban(pos);
    }
    for (auto &pos : arraySpecialZone)
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

void BattleField::affectArmy(Army *army)
{
    // for (int i = 0; i < n_rows; i++)
    // {
    //     for (int j = 0; j < n_cols; j++)
    //     {
    //         terrain[i][j]->getEffect(army);
    //     }
    // }
    for (auto &pos : arrayForest)
    {
        terrain[pos->getRow()][pos->getCol()]->getEffect(army);
    }
    for (auto &pos : arrayRiver)
    {
        terrain[pos->getRow()][pos->getCol()]->getEffect(army);
    }
    for (auto &pos : arrayUrban)
    {
        terrain[pos->getRow()][pos->getCol()]->getEffect(army);
    }
    for (auto &pos : arrayFortification)
    {
        terrain[pos->getRow()][pos->getCol()]->getEffect(army);
    }
    for (auto &pos : arraySpecialZone)
    {
        terrain[pos->getRow()][pos->getCol()]->getEffect(army);
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

    while (ss >> ch)
    {
        if (ch == '(')
        {
            // read row and column values
            ss >> row >> ch >> col >> ch; // read row, ',', col, ')'
            positions.push_back(new Position(row, col));
        }
    }

    return positions;
}

vector<string> Configuration::splitUnits(const string &unitListString)
{
    std::vector<std::string> units;
    int parenLevel = 0; // indicates the parenthesis level, higher number = inner scope
    std::string current;

    for (char c : unitListString)
    {
        if (c == ',' && parenLevel == 0)
        {
            units.push_back(current);
            current.clear();
        }
        else
        {
            if (c == '(')
                parenLevel++;
            if (c == ')')
                parenLevel--;
            current += c;
        }
    }

    if (!current.empty())
        units.push_back(current);

    return units;
}

bool Configuration::isInfantry(const string &unitName)
{
    static vector<string> infantryUnits = {
        "SNIPER",
        "ANTIAIRCRAFTSQUAD",
        "MORTARSQUAD",
        "ENGINEER",
        "SPECIALFORCES",
        "REGULARINFANTRY"};
    for (string unit : infantryUnits)
    {
        if (unit == unitName)
        {
            return true;
        }
    }
    return false;
}

InfantryType Configuration::getInfantryType(const string &unitName)
{
    static vector<pair<string, InfantryType>> infantryNames = {
        {"SNIPER", SNIPER},
        {"ANTIAIRCRAFTSQUAD", ANTIAIRCRAFTSQUAD},
        {"MORTARSQUAD", MORTARSQUAD},
        {"ENGINEER", ENGINEER},
        {"SPECIALFORCES", SPECIALFORCES},
        {"REGULARINFANTRY", REGULARINFANTRY}};

    for (const auto& infantryName : infantryNames)
    {
        if (infantryName.first == unitName)
        {
            return infantryName.second;
        }
    }

    return SNIPER;
}

VehicleType Configuration::getVehicleType(const string &unitName)
{
    static vector<pair<string, VehicleType>> vehicleNames = {
        {"TRUCK", TRUCK},
        {"MORTAR", MORTAR},
        {"ANTIAIRCRAFT", ANTIAIRCRAFT},
        {"ARMOREDCAR", ARMOREDCAR},
        {"APC", APC},
        {"ARTILLERY", ARTILLERY},
        {"TANK", TANK}};

    for (const auto &vehicleName : vehicleNames)
    {
        if (vehicleName.first == unitName)
        {
            return vehicleName.second;
        }
    }

    return TRUCK;
}

void Configuration::parseUnits(vector<string> units)
{
    liberationCount = 0;
    ARVNCount = 0;

    liberationUnits = new Unit *[units.size()];
    ARVNUnits = new Unit *[units.size()];

    for (string unit : units)
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

        Position pos(row, col);

        // Create the unit object
        Unit *unitObj = nullptr;
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
        string lineCode = line.substr(0, equalIndex);
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
    for (auto &pos : arrayForest)
    {
        delete pos;
    }
    for (auto &pos : arrayRiver)
    {
        delete pos;
    }
    for (auto &pos : arrayFortification)
    {
        delete pos;
    }
    for (auto &pos : arrayUrban)
    {
        delete pos;
    }
    for (auto &pos : arraySpecialZone)
    {
        delete pos;
    }

    for (int i = 0; i < liberationCount; i++)
    {
        delete liberationUnits[i];
    }
    delete[] liberationUnits;

    for (int i = 0; i < ARVNCount; i++)
    {
        delete ARVNUnits[i];
    }
    delete[] ARVNUnits;
}

pair<int, int> Configuration::getBattleFieldDimensions() const
{
    return {num_rows, num_cols};
}

vector<Position *> Configuration::getTerrainPosition(int identity) const
{
    switch (identity)
    {
    case 1:
        return arrayForest;
    case 2:
        return arrayRiver;
    case 3:
        return arrayFortification;
    case 4:
        return arrayUrban;
    default:
        return arraySpecialZone;
    }
}

Unit **Configuration::getUnitList(bool isLiber) const
{
    if (isLiber)
    {
        return liberationUnits;
    }
    else
    {
        return ARVNUnits;
    }
}

int Configuration::getListSize(bool isLiber) const
{
    if (isLiber)
    {
        return liberationCount;
    }
    else
    {
        return ARVNCount;
    }
}

int Configuration::getEventCode() const
{
    return eventCode;
}

string Configuration::toPositionString(const vector<Position *> &vec) const
{
    stringstream ss;
    ss << "[";
    for (int i = 0; i < vec.size(); i++)
    {
        ss << vec[i]->str();
        if (i < vec.size() - 1)
        {
            ss << ",";
        }
    }
    ss << "]";
    return ss.str();
}

string Configuration::toUnitListString(Unit **units, int size) const
{
    stringstream ss;
    ss << "[";
    for (int i = 0; i < size; i++)
    {
        ss << units[i]->str();
        if (i < size - 1)
        {
            ss << ",";
        }
    }
    ss << "]";
    return ss.str();
}

string Configuration::str() const
{
    stringstream result;
    result << "[";
    result << "num_rows=" << num_rows << ",";
    result << "num_cols=" << num_cols << ",";

    result << "arrayForest=" << toPositionString(arrayForest) << ",";
    result << "arrayRiver=" << toPositionString(arrayRiver) << ",";
    result << "arrayFortification=" << toPositionString(arrayFortification) << ",";
    result << "arrayUrban=" << toPositionString(arrayUrban) << ",";
    result << "arraySpecialZone=" << toPositionString(arraySpecialZone) << ",";

    result << "liberationUnits=" << toUnitListString(liberationUnits, liberationCount) << ",";
    result << "ARVNUnits=" << toUnitListString(ARVNUnits, ARVNCount) << ",";

    result << "eventCode=" << eventCode << "]";

    return result.str();
}

// HCMCampaign
HCMCampaign::HCMCampaign(const string &config_file_path)
{
    config = new Configuration(config_file_path);
    pair<int, int> dimensions = config->getBattleFieldDimensions();
    int row = dimensions.first;
    int col = dimensions.second;
    vector<Position *> arrayForest = config->getTerrainPosition(1);
    vector<Position *> arrayRiver = config->getTerrainPosition(2);
    vector<Position *> arrayFortification = config->getTerrainPosition(3);
    vector<Position *> arrayUrban = config->getTerrainPosition(4);
    vector<Position *> arraySpecialZone = config->getTerrainPosition(5);
    battleField = new BattleField(row, col, arrayForest, arrayRiver, arrayFortification, arrayUrban, arraySpecialZone);
    Unit **liberationUnits = config->getUnitList(true), **ARVNUnits = config->getUnitList(false);
    int liberationCount = config->getListSize(true), ARVNCount = config->getListSize(false);

    liberationArmy = new LiberationArmy(liberationUnits, liberationCount, "LiberationArmy", battleField);
    ARVNArmy = new ARVN(ARVNUnits, ARVNCount, "ARVN", battleField);

    eventCode = config->getEventCode();
}

HCMCampaign::~HCMCampaign()
{
    delete liberationArmy;
    delete ARVNArmy;
    delete battleField;
    delete config;
}

void HCMCampaign::run()
{
    if (battleField)
    {
        battleField->affectArmy(liberationArmy);
        battleField->affectArmy(ARVNArmy);
    }

    if (eventCode < 75)
    {
        liberationArmy->fight(ARVNArmy, false);
        ARVNArmy->fight(liberationArmy, true);
    }
    else
    {
        ARVNArmy->fight(liberationArmy, false);
        liberationArmy->fight(ARVNArmy, true);

        liberationArmy->fight(ARVNArmy, false);
        ARVNArmy->fight(ARVNArmy, true);
    }

    liberationArmy->removeUnitsAfterFight();
    ARVNArmy->removeUnitsAfterFight();

    liberationArmy->updateParameters();
    ARVNArmy->updateParameters();
}

string HCMCampaign::printResult()
{
    stringstream result;
    result << "LIBERATIONARMY[LF=" << liberationArmy->getLF() << ",EXP=" << liberationArmy->getEXP()
           << "]-ARVN[LF=" << ARVNArmy->getLF() << ",EXP=" << ARVNArmy->getEXP() << "]";

    return result.str();
}
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////