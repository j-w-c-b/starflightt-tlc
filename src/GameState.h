/*
        STARFLIGHT - THE LOST COLONY
        GameState.h - ?
        Author: ?
        Date: ?

*/

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include <allegro5/allegro.h>

#include "DataMgr.h"
#include "Loan.h"
#include "Officer.h"
#include "Point2D.h"
#include "Stardate.h"
#include "debug.h" //prefs for debug modes, keys, etc.

class Items;
class Quest;

enum AlienRaces
{
    ALIEN_NONE = 0,
    ALIEN_PIRATE,
    ALIEN_ELOWAN,
    ALIEN_SPEMIN,
    ALIEN_THRYNN,
    ALIEN_BARZHON,
    ALIEN_NYSSIAN,
    ALIEN_TAFEL,
    ALIEN_MINEX,
    ALIEN_COALITION,
    NUM_ALIEN_RACES
};

enum ProfessionType
{
    PROFESSION_SCIENTIFIC = 1,
    PROFESSION_FREELANCE = 2,
    PROFESSION_MILITARY = 4
};

// OFFICER CLASS

constexpr int CREW_SIZE = 7;

// SHIP CLASS
#define STARTING_HULL_INTEGRITY 100

enum ClassType
{
    NotInstalledType,
    Class1Type,
    Class2Type,
    Class3Type,
    Class4Type,
    Class5Type,
    Class6Type
};

// NUM_REPAIR_PARTS is the number of repairable ship systems
// MAX_REPAIR_COUNT is the number of repair iteration the player can do before
// spending a repair mineral
const int NUM_REPAIR_PARTS = 5;
const int MAX_REPAIR_COUNT = 3;

// NOTE: the repair code rely on this starting at 0, having no hole, PART_NONE
// first, and NUM_REPAIR_PARTS+1 members.
enum ShipPart
{
    PART_NONE = 0,
    PART_LASERS = 1,
    PART_MISSILES = 2,
    PART_HULL = 3,
    PART_SHIELDS = 4,
    PART_ENGINES = 5
};

// repair minerals
const ID ITEM_COBALT = 32;
const ID ITEM_MOLYBDENUM = 36;
const ID ITEM_ALUMINUM = 39;
const ID ITEM_TITANIUM = 40;
const ID ITEM_SILICA = 44;

// capacity of a Cargo Pod
const int POD_CAPACITY = 10;

class Ship {
  public:
    Ship();
    virtual ~Ship();
    Ship &operator=(const Ship &rhs);

    static constexpr std::string_view class_name = "Ship";

    // accessors
    std::string getName() const;
    int getCargoPodCount() const;
    int getEngineClass() const;
    int getShieldClass() const;
    int getArmorClass() const;
    float getHullIntegrity() const;
    float getArmorIntegrity() const;
    float getShieldIntegrity() const;
    float getShieldCapacity() const;
    float getEngineIntegrity() const;
    int getMissileLauncherClass() const;
    int getMissileLauncherDamage();
    int getMissileLauncherFiringRate();
    float getMissileLauncherIntegrity() const;
    float getLaserIntegrity() const;
    int getLaserClass() const;
    int getLaserDamage();
    int getLaserFiringRate();
    bool getHasTV() const { return hasTV; }
    float getMaxArmorIntegrity();
    float getMaxShieldCapacity();

    void damageRandomSystemOrCrew(
        int odds = 33,
        int mindamage = 10,
        int maxdamage = 30);

    std::string getCargoPodCountString() const;
    std::string getEngineClassString() const;
    std::string getShieldClassString() const;
    std::string getArmorClassString() const;
    std::string getMissileLauncherClassString() const;
    std::string getLaserClassString() const;
    bool HaveEngines() const;

    int getMaxEngineClass() { return maxEngineClass; }
    int getMaxArmorClass() { return maxArmorClass; }
    int getMaxShieldClass() { return maxShieldClass; }
    int getMaxLaserClass() { return maxLaserClass; }
    int getMaxMissileLauncherClass() { return maxMissileLauncherClass; }
    float get_maximum_velocity() const;
    float get_fuel_usage(float distance) const;

    int getTotalSpace();
    int getOccupiedSpace();
    int getAvailableSpace();

    // mutators
    void setName(const std::string &initName);
    void setCargoPodCount(int initCargoPodCount);
    void cargoPodPlusPlus();
    void cargoPodMinusMinus();
    void setEngineClass(int initEngineClass);
    void setShieldClass(int initShieldClass);
    void setArmorClass(int initArmorClass);
    void setMissileLauncherClass(int initMissileLauncherClass);
    void setLaserClass(int initLaserClass);
    void setHullIntegrity(float initHullIntegrity);
    void augHullIntegrity(float amount);
    void setArmorIntegrity(float initArmorIntegrity);
    void setShieldIntegrity(float initShieldIntegrity);
    void augShieldIntegrity(float amount);
    void setShieldCapacity(float initShieldCapacity);
    void setEngineIntegrity(float initEngineIntegrity);
    void augEngineIntegrity(float amount);
    void setMissileLauncherIntegrity(float initMissileLauncherIntegrity);
    void augMissileLauncherIntegrity(float amount);
    void setLaserIntegrity(float initLaserIntegrity);
    void augLaserIntegrity(float amount);

    void setMaxEngineClass(int engineClass);
    void setMaxArmorClass(int armorClass);
    void setMaxShieldClass(int shieldClass);
    void setMaxLaserClass(int laserClass);
    void setMaxMissileLauncherClass(int missileLauncherClass);

    void setHasTV(bool initHasTV) { hasTV = initHasTV; }

    // specials
    void Reset();

    // fuel consumption
    float getFuel();
    void augFuel(float percentage);
    void injectEndurium();
    void ConsumeFuel(int iterations = 1);
    int getEnduriumOnBoard();

    // in-space repair
    void initializeRepair();
    int repairMinerals[NUM_REPAIR_PARTS];
    int repairCounters[NUM_REPAIR_PARTS];
    ShipPart partInRepair;

    void SendDistressSignal();

  private:
    std::string name;
    int cargoPodCount;
    int engineClass;
    int shieldClass;
    int armorClass;
    int missileLauncherClass;
    int laserClass;
    float hullIntegrity;
    float armorIntegrity;
    float shieldIntegrity;
    float shieldCapacity;
    float engineIntegrity;
    float missileLauncherIntegrity;
    float laserIntegrity;
    int maxEngineClass;
    int maxShieldClass;
    int maxArmorClass;
    int maxMissileLauncherClass;
    int maxLaserClass;
    bool hasTV;
    float fuelPercentage; // 1.0 = full, 0.0 = empty, 0.5 = half tank
    void capFuel(); // tops off the fuel tank... basically this function just
                    // makes sure that the fuel is within the proper limits
    friend InputArchive &operator>>(InputArchive &ar, Ship &ship);
    friend OutputArchive &operator<<(OutputArchive &ar, const Ship &ship);
};

// GAMESTATE CLASS
#define STARTING_CREDITS 1000

// Info about the flux in the galaxy that the user has
// interacted with either by seeing it in the interstellar
// flight or by flying through it with enough knowledge to map
// it.
struct FluxInfo {
    bool endpoint_1_visible;
    bool endpoint_2_visible;
    bool path_visible;

    FluxInfo()
        : endpoint_1_visible(false), endpoint_2_visible(false),
          path_visible(false) {}
};

class PlayerInfo {
  public:
    PlayerInfo() { Reset(); }

    static constexpr std::string_view class_name = "PlayerInfo";
    friend InputArchive &operator>>(InputArchive &ar, PlayerInfo &info);
    friend OutputArchive &operator<<(OutputArchive &ar, const PlayerInfo &info);

    void Reset();
    PlayerInfo &operator=(const PlayerInfo &rhs);

    int currentStar;
    int currentPlanet;
    int controlPanelMode;

    Point2D posHyperspace;
    Point2D posSystem;
    Point2D posPlanet;
    Point2D posStarport;
    Point2D posCombat;
    double currentSpeed;

    double get_galactic_x() { return posHyperspace.x; }
    double get_galactic_y() { return posHyperspace.y; }

    Point2D get_galactic_pos() { return posHyperspace; }

    void set_galactic_pos(Point2D pos) { posHyperspace = pos; }

    void set_galactic_pos(double x, double y) {
        posHyperspace.x = x;
        posHyperspace.y = y;
    }

    /*
     * ENCOUNTER DATA STORED HERE
     * This data is shared by space travel and encounter modules
     */
    // GalacticRegion property is used to identify the area of the galaxy where
    // the player is located. It is set by the interstellar module and used by
    // the auxiliary window.
    AlienRaces galacticRegion;
    std::string getAlienRaceName(AlienRaces race);
    std::string getAlienRaceName(int race);
    std::string getAlienRaceNamePlural(AlienRaces race);
    AlienRaces getGalacticRegion() { return galacticRegion; }
    void setGalacticRegion(AlienRaces race) { galacticRegion = race; }

    int fleetSize;
    int getAlienFleetSize() { return fleetSize; }
    void setAlienFleetSize(int value) { fleetSize = value; }

    /*
     * END ENCOUNTER RELATED DATA
     */

    double getCurrentSpeed() { return currentSpeed; }
    void setCurrentSpeed(double value) { currentSpeed = value; }

    bool hasHyperspacePermit() { return m_bHasHyperspacePermit; }
    void set_HyperspacePermit(bool status) { m_bHasHyperspacePermit = status; }
    bool hasOverdueLoan() { return m_loan && m_loan->is_overdue(); }
    bool pay_loan(int amount);
    bool take_loan(int amount);
    const std::optional<Loan> get_loan() { return m_loan; }
    void update_loan();

    bool isLost() { return m_is_lost; }
    void isLost(bool is_lost) { m_is_lost = is_lost; }

    // alive property used for encounters
    bool alive;
    bool getAlive() { return alive; }
    void setAlive(bool value) { alive = value; }

  private:
    bool m_scanner, m_previous_scanner_state, m_bHasHyperspacePermit;
    bool m_is_lost;
    std::optional<Loan> m_loan;
    Stardate m_date_lost;
};

struct QuestScript {
    std::string script;
    Quest *parentQuest;

    QuestScript() : script(), parentQuest(nullptr) {}

    QuestScript(const std::string &script, Quest *parentQuest)
        : script(script), parentQuest(parentQuest) {}
};

class GameState {
  public:
    enum GameSaveSlot
    {
        GAME_SAVE_SLOT_UNKNOWN = -2,
        GAME_SAVE_SLOT_NEW = -1,
        GAME_SAVE_SLOT0 = 0,
        GAME_SAVE_SLOT1,
        GAME_SAVE_SLOT2,
        GAME_SAVE_SLOT3,
        GAME_SAVE_SLOT4,
        GAME_SAVE_SLOT5,
        GAME_SAVE_SLOT_MAX = GAME_SAVE_SLOT5
    };

    GameState();
    ~GameState();

    void operator=(const GameState &other);
    static constexpr std::string_view class_name = "GameState";
    friend InputArchive &operator>>(InputArchive &ar, GameState &game_state);
    friend OutputArchive &
    operator<<(OutputArchive &ar, const GameState &game_state);

    void Reset();

    bool SaveGame(GameSaveSlot slot);
    static GameState *ReadGame(GameSaveSlot slot);
    static GameState *LoadGame(GameSaveSlot slot);
    static void DeleteGame(GameSaveSlot slot);

    bool AutoSave();
    bool AutoLoad();

    bool has_officer(OfficerType type) const {
        auto officer = m_crew.find(type);

        return (
            officer != m_crew.end() && officer->second->get_vitality() != 0);
    }

    // calculate effective skill level taking into account vitality and captain
    // modifier
    int CalcEffectiveSkill(Skill skill) const;

    // do a roll against given skill, using CalcEffectiveSkill to get the skill
    // value
    bool SkillCheck(Skill skill);
    bool SkillUp(Skill skill, int amount = 1);
    bool CanSkillCheck(Skill skill) const;
    bool add_experience(Skill skill, int amount = 0);

    bool HaveFullCrew() const;
    bool PreparedToLaunch() const;
    Ship getShip() const;

    ProfessionType getProfession() const;
    int getCredits() const;

    // mutators
    void setCaptainSelected(bool initCaptainSelected);
    void setProfession(const ProfessionType initProfession);
    void setCredits(int initCredits);
    void augCredits(int amount);
    void setShip(const Ship &initShip);

    OfficerType getCurrentSelectedOfficer() { return m_currentSelectedOfficer; }
    void setCurrentSelectedOfficer(OfficerType value) {
        m_currentSelectedOfficer = value;
    }
    const Officer *set_officer(OfficerType type, const Officer *officer);
    void hire_officer(const Officer *officer);
    void unassign_officer(const Officer *officer);
    void fire_officer(const Officer *officer);
    void create_captain(
        const std::string &name,
        const std::map<Skill, int> &attributes);

    int alienAttitudes[NUM_ALIEN_RACES]; // use enum AlienRaces for index
    std::map<ID, FluxInfo> flux_info;
    long alienAttitudeUpdate;
    std::string playerPosture; // for use in Encounters

    double m_baseGameTimeSecs; // base starting value for timer
    double getBaseGameTimeSecs() { return m_baseGameTimeSecs; }
    void setBaseGameTimeSecs(double value) { m_baseGameTimeSecs = value; }

    double m_gameTimeSecs; //# seconds since the start of the game
    double getGameTimeSecs() const { return m_gameTimeSecs; }
    void setGameTimeSecs(double value) { m_gameTimeSecs = value; }

    Stardate stardate; // the current Stardate

    bool m_captainSelected;      // is a captain selected for play?
    ProfessionType m_profession; // captain profession
    int m_credits;               // credits
    Items m_items;               // player inventory
    PlayerInfo player;           // Holds misc player data
    Ship m_ship;                 // ship data
    OfficerType
        m_currentSelectedOfficer; // currently selected officer in Control Panel

    // navigation status
    bool navigateStatus;

    // tactical shield up/down status
    bool shieldStatus;
    bool getShieldStatus() { return shieldStatus; }
    void setShieldStatus(bool value) { shieldStatus = value; }

    // tactical weapon arm status
    bool weaponStatus;
    bool getWeaponStatus() { return weaponStatus; }
    void setWeaponStatus(bool value) { weaponStatus = value; }

    // the plot stage represents the four stages of the game: INITIAL, VIRUS,
    // WAR, ANCIENTS as defined in the alien script files
    int plotStage;
    int getPlotStage() { return plotStage; }
    void setPlotStage(int value) {
        ALLEGRO_ASSERT(plotStage >= 1 && plotStage <= 4);
        plotStage = value;
    }
    bool dirty; // Does the game state need saving (for Captain's Lounge code)?

    ProfessionType getProfession() { return m_profession; }
    std::string getProfessionString() const;

    // Accessors & mutator imported from Encounter module to standardize access:
    AlienRaces getCurrentAlien();
    std::string getCurrentAlienName();
    int getAlienAttitude();
    void setAlienAttitude(int value);

    // the currently active module
    // Module names are found in ModeMgr.h
    std::string currentModule;
    std::string getCurrentModule() const { return currentModule; }
    std::string getSavedModule() const { return currentModeWhenGameSaved; }
    void setCurrentModule(const std::string &value) { currentModule = value; }
    std::string get_saved_module_name() const;

    // the currently active quest ID
    int getActiveQuest() { return activeQuest; }
    int getStoredValue() { return storedValue; }
    void setStoredValue(int v) { storedValue = v; }
    void setActiveQuest(int v) {
        if (v != activeQuest) {
            activeQuest = v;
            storedValue = -1;
            questCompleted = false;
        }
    }
    bool getQuestCompleted() { return questCompleted; }
    void setQuestCompleted(bool value) { questCompleted = value; }
    bool firstTimeVisitor;

    Point2D getHyperspaceCoordinates() {
        return Point2D(
            player.posHyperspace.x / 128.0, player.posHyperspace.y / 128.0);
    }
    Point2D setHyperspaceCoordinates(const Point2D &pos) {
        return player.posHyperspace = Point2D(pos.x * 128.0, pos.y * 128.0);
    }

    Point2D getSystemCoordinates() {
        return Point2D(player.posSystem.x / 256.0, player.posSystem.y / 256.0);
    }

    GameSaveSlot get_current_game_save_slot() const {
        return currentSaveGameSlot;
    }

    void update_unemployed_officers();

    std::set<Officer *>::const_iterator unassigned_begin() const {
        return m_unassigned_officers.begin();
    }

    std::set<Officer *>::const_iterator unassigned_end() const {
        return m_unassigned_officers.end();
    }

    std::set<Officer *>::const_iterator unemployed_begin() const {
        return m_unemployed_officers.begin();
    }

    std::set<Officer *>::const_iterator unemployed_end() const {
        return m_unemployed_officers.end();
    }

    std::map<OfficerType, Officer *>::const_iterator crew_begin() const {
        return m_crew.begin();
    }

    std::map<OfficerType, Officer *>::const_iterator crew_end() const {
        return m_crew.end();
    }

    const Officer *get_officer(OfficerType type) const {
        auto i = m_crew.find(type);
        if (i != m_crew.end()) {
            return i->second;
        } else {
            return nullptr;
        }
    }
    const std::string get_officer_name(OfficerType type) const {
        auto i = m_crew.find(type);
        if (i != m_crew.end()) {
            return i->second->get_name();
        } else {
            return "";
        }
    }

    const Officer *get_effective_officer(OfficerType type) const {
        auto i = m_crew.find(type);
        if (i != m_crew.end() && i->second->get_vitality() > 0) {
            return i->second;
        } else {
            return m_crew.find(OFFICER_CAPTAIN)->second;
        }
    }

    void cease_healing() {
        for (auto &i : m_crew) {
            i.second->Recovering(false);
        }
    }

    void cease_healing(OfficerType officer_type) {
        ALLEGRO_ASSERT(officer_type != OFFICER_NONE);
        m_crew[officer_type]->Recovering(false);
    }

    void set_healing(OfficerType officer_type, bool is_healing) {
        m_crew[officer_type]->Recovering(is_healing);
    }
    void heal(OfficerType officer_type, float amount) {
        m_crew[officer_type]->heal(amount);
    }

    std::string get_abbreviated_title(OfficerType type) {
        return effective_officer(type)->get_abbreviated_title();
    }

    static const std::map<Skill, OfficerType> skill_map;
    const std::map<Skill, int> &base_skills(ProfessionType type);
    const std::map<Skill, int> &max_skills(ProfessionType type);
    const std::map<Skill, int> &base_skills();
    const std::map<Skill, int> &max_skills();

  private:
    Officer *effective_officer(OfficerType type) const {
        auto i = m_crew.find(type);
        if (i != m_crew.end() && i->second->get_vitality() > 0) {
            return i->second;
        } else {
            return m_crew.find(OFFICER_CAPTAIN)->second;
        }
    }
    int activeQuest;
    int storedValue; // stored value of quest requirement is a game state
                     // variable, just like others.
    bool questCompleted;

    static GameSaveSlot currentSaveGameSlot;
    std::string currentModeWhenGameSaved;

    static ALLEGRO_PATH *save_dir_path;
    static ALLEGRO_PATH *get_save_file_path(GameSaveSlot slot);
    static bool ensure_save_dir();
    std::set<Officer *> m_unemployed_officers;
    std::set<Officer *> m_unassigned_officers;
    std::map<OfficerType, Officer *> m_crew;
};

#endif
