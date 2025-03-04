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

class Utility
{
public:
    int sumDigits(int n);
    int personalNumber(int num, int year);
    bool isSpecialNumber(int num, int k);
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
public:
    Army(Unit **unitArray, int size, string name, BattleField *battleField);
    virtual void fight(Army *enemy, bool defense = false) = 0;
    virtual string str() const = 0;
};

class LiberationArmy : public Army
{
public:
    LiberationArmy(Unit **unitArray, int size, string name ,BattleField *battleField);
    void fight(Army *enemy, bool defense = false) override;
    string str() const override;
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
};

class Unit
{
protected:
    int quantity, weight;
    Position pos;

public:
    Unit(int quantity, int weight, Position pos);
    virtual ~Unit();
    virtual int getAttackScore() = 0;
    Position getCurrentPosition() const;
    virtual string str() const = 0;
    virtual string instance();
    virtual VehicleType getVehicleType();
    virtual InfantryType getInfantryType();
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
    VehicleType getVehicleType();
};

class Infantry : public Unit
{
private:
    InfantryType infantryType;
    bool isCommando();
    int getScore();
    string infantryName;
public:
    Infantry (int quantity, int weight, const Position pos, InfantryType infantryType);
    int getAttackScore() override;
    string str() const override;
    string instance();
    InfantryType getInfantryType();
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
    int infantryCount;
    void insertAtHead(Unit *unit);
    void insertAtEnd(Unit *unit);
public:
    UnitList(int capacity);
    bool insert(Unit *unit);                   // return true if insert successfully
    bool isContain(VehicleType vehicleType);   // return true if it exists
    bool isContain(InfantryType infantryType); // return true if it exists
    string str() const;
    // TODO:
};

class TerrainElement
{
public:
    TerrainElement();
    ~TerrainElement();
    virtual void getEffect(Army *army) = 0;
};

class BattleField
{
private:
    int n_rows, n_cols;
    // TODO
public:
    BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                vector<Position *> arrayUrban, vector<Position *> arraySpecialZone);
    ~BattleField();
    string str();
};

class HCMCampaign
{
private:
    Configuration *config;
    BattleField *battleField;
    LiberationArmy *liberationArmy;
    ARVN *ARVN;

public:
    HCMCampaign(const string &config_file_path);
    void run();
    string printResult();
};

#endif