/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 2
 * Programming Fundamentals Spring 2025
 * Date: 02.02.2025
 */

// The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_HCM_CAMPAIGN_H_
#define _H_HCM_CAMPAIGN_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class Unit;
class UnitList;
class Army;
class TerrainElement;

class Vehicle;
class Infantry;

class LiberationArmy;
class ARVN;

class Position;

class Road;
class Mountain;
class River;
class Urban;
class Fortification;
class SpecialZone;

class BattleField;

class HCMCampaign;
class Configuration;

class Utility;
class Node;

enum VehicleType
{
    TRUCK,
    MORTAR,
    ANTIAIRCRAFT,
    ARMOREDCAR,
    APC,
    ARTILLERY,
    TANK
};
enum InfantryType
{
    SNIPER,
    ANTIAIRCRAFTSQUAD,
    MORTARSQUAD,
    ENGINEER,
    SPECIALFORCES,
    REGULARINFANTRY
};

class Army
{
protected:
    int LF, EXP;
    string name;
    UnitList *unitList;
    BattleField *battleField;
    int clampLF(int LF);
    int clampEXP(int EXP);
    bool isSpecialNumber(int num, int base);

public:
    Army(Unit **unitArray, int size, string name, BattleField *battleField);
    virtual ~Army();
    int getLF() const;
    int getEXP() const;
    void setLF(int LF);
    void setEXP(int EXP);
    virtual void fight(Army *enemy, bool defense = false) = 0;
    void removeUnitsAfterFight();
    void updateParameters();
    virtual string str() const = 0;
    virtual string instance();
    Node *getListHead() const;
    UnitList *getList() const;
};

class LiberationArmy : public Army
{
private:
    int closestFibonacci(int num);
    void confiscate(Army *enemy);

public:
    LiberationArmy(Unit **unitArray, int size, string name, BattleField *battleField);
    void fight(Army *enemy, bool defense = false) override;
    string str() const override;
    string instance();
};

class ARVN : public Army
{
public:
    ARVN(Unit **unitArray, int size, string name, BattleField *battleField);
    void fight(Army *enemy, bool defense = false) override;
    string str() const override;
    string instance();
};

class Position
{
private:
    int r, c;

public:
    Position(int r = 0, int c = 0);
    Position(const string &str_pos); // Example: str_pos = "(1,15)"
    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);
    string str() const; // Example: returns "(1,15)"
    double getDistance(Position other) const;
};

class Unit
{
protected:
    int quantity, weight, attackScore;
    Position pos;
    bool affectedForest, affectedRiver, affectedFortification, affectedUrban, affectedSpecialZone;

public:
    Unit(int quantity, int weight, Position pos);
    virtual ~Unit();
    virtual int getAttackScore() = 0;
    Position getCurrentPosition() const;
    virtual string str() const = 0;
    virtual string instance();
    virtual VehicleType getVehicleType() const;
    virtual InfantryType getInfantryType() const;
    int getCurrentScore() const;
    int getQuantity() const;
    int getWeight() const;
    void addQuantity(int quantity);
    void multiplyQuantity(double multiplier);
    void setQuantity(int quantity);
    void multiplyWeight(double multiplier);
    void setWeight(int weight);
    void setAttackScore(int score);
    void multiplyScore(double multiplier);
    void addScore(double num);
    bool isAffected(int type);
    void setAffected(int type);
};

class Vehicle : public Unit
{
private:
    VehicleType vehicleType;
    string vehicleName;

public:
    Vehicle(int quantity, int weight, const Position pos, VehicleType vehicleType);
    int getAttackScore() override;
    string str() const override;
    string instance();
    VehicleType getVehicleType() const override;
};

class Infantry : public Unit
{
private:
    InfantryType infantryType;
    int sumDigits(int num);
    int personalNumber(int num, int year);
    bool isSquare(int num);
    bool isCommando();
    int getScore();
    string infantryName;

public:
    Infantry(int quantity, int weight, const Position pos, InfantryType infantryType);
    int getAttackScore() override;
    string str() const override;
    string instance();
    InfantryType getInfantryType() const override;
};

class Node
{
public:
    Unit *unit;
    Node *next;
    // default constructor
    Node();
    // parametrized constructor
    Node(Unit *unit);
};

class UnitList
{
private:
    int capacity;
    int currentSize;
    Node *listHead;
    Node *listEnd;
    int vehicleCount;
    int infantryCount; // can also be used as the index of the first vehicle
    void insertAtHead(Unit *unit);
    void insertAtEnd(Unit *unit);

public:
    UnitList(int capacity);
    ~UnitList();
    void setCapacity(int capacity);
    bool insert(Unit *unit);                   // return true if insert successfully
    bool isContain(VehicleType vehicleType);   // return true if it exists
    bool isContain(InfantryType infantryType); // return true if it exists
    void deleteNode(Node *&node);              // returns true if deletion sucessfull
    void deleteMatchingQuantity(int quantity);
    void deleteLowerScore(int score);
    void deleteAllInfantry();
    void deleteAllVehicle();
    void reverse();
    Node *getHead() const;
    Node *getNodeAtIndex(int index) const; // return a pointer to that specific node
    Node *getFirstVehicle() const;
    vector<Unit *> convertToVector();
    vector<Node *> findMinSubset(int threshold, bool isInfantry);
    string str() const;
};

class TerrainElement
{
protected:
    Position *pos;

public:
    TerrainElement(Position *pos);
    ~TerrainElement();
    virtual void getEffect(Army *army) = 0;
};

// TODO: delcare and implement the Elements of the terrain

class Road : public TerrainElement
{
public:
    Road();
    void getEffect(Army *army) override;
};

class Mountain : public TerrainElement
{
public:
    Mountain(Position *pos);
    void getEffect(Army *army) override;
};

class River : public TerrainElement
{
public:
    River(Position *pos);
    void getEffect(Army *army) override;
};

class Urban : public TerrainElement
{
public:
    Urban(Position *pos);
    void getEffect(Army *army) override;
};

class Fortification : public TerrainElement
{
public:
    Fortification(Position *pos);
    void getEffect(Army *army) override;
};

class SpecialZone : public TerrainElement
{
public:
    SpecialZone(Position *pos);
    void getEffect(Army *army) override;
};

class BattleField
{
private:
    int n_rows, n_cols;
    vector<vector<TerrainElement *>> terrain;

public:
    BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                vector<Position *> arrayUrban, vector<Position *> arraySpecialZone);
    ~BattleField();
    void affectArmy(Army *army);
    string str();
};

class Configuration
{
private:
    int num_rows, num_cols;
    vector<Position *> arrayForest, arrayRiver, arrayFortification, arrayUrban, arraySpecialZone;
    Unit **liberationUnits;
    Unit **ARVNUnits;
    int liberationCount, ARVNCount;
    int eventCode;

    vector<Position *> toPositionVector(const string &positionList);
    vector<string> splitUnits(const string &unitListString);
    void parseUnits(vector<string> unitListVector);
    bool isInfantry(const string &unitName);
    InfantryType getInfantryType(const string &unitName);
    VehicleType getVehicleType(const string &unitName);
    string toPositionString(const vector<Position *> &vec) const;
    string toUnitListString(Unit **units, int size) const;

public:
    Configuration(const string &filepath);
    ~Configuration();
    pair<int, int> getBattleFieldDimensions() const;
    vector<Position *> getTerrainPosition(int identity) const;
    Unit **getUnitList(bool isLiber) const;
    int getListSize(bool isLiber) const;
    int getEventCode() const;
    string str() const;
};

class HCMCampaign
{
private:
    Configuration *config;
    BattleField *battleField;
    LiberationArmy *liberationArmy;
    ARVN *ARVNArmy;
    int eventCode;

public:
    HCMCampaign(const string &config_file_path);
    ~HCMCampaign();
    void run();
    string printResult();
};

#endif