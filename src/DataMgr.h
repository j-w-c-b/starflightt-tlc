/*
        STARFLIGHT - THE LOST COLONY
        DataMgr.h - Provides access to game data and resources; for Get methods
   which have corresponding Free methods, the caller is responsible for calling
   the Free method to release loaded resources. If there is no Free method
   provided, then the caller IS NOT responsible for releasing memory as the
   memory is owned by the providing class. Author: Dave "coder1024" Calkins
        Date: 01/22/2007
*/

#ifndef DATAMGR_H
#define DATAMGR_H
//#pragma once

#include "Flux.h"
#include "Script.h"
#include <allegro5/allegro_font.h>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

#define MAX_FLUX 32

class Officer;
class DataMgr;
struct Point;
class Archive;

// type used for object IDs
typedef int ID;

// type used for coordinate values (x and y for stars, for example)
typedef int CoordValue;

// type used for temperature
typedef unsigned long Kelvins;

// item types
typedef enum
{
    IT_INVALID = 0,
    IT_ARTIFACT = 1,
    IT_RUIN = 2,
    IT_MINERAL = 4,
    IT_LIFEFORM = 8,
    IT_TRADEITEM = 16
} ItemType;

// item age
typedef enum
{
    IA_INVALID = 0,
    IA_STONE,
    IA_METAL,
    IA_INDUSTRIAL,
    IA_SPACEFARING
} ItemAge;

// an item
class Item {
  public:
    Item();
    Item(const Item &rhs);
    virtual ~Item();
    Item &operator=(const Item &rhs);

    void Reset();

    bool
    IsArtifact() {
        return itemType == IT_ARTIFACT;
    };
    bool
    IsMineral() {
        return itemType == IT_MINERAL;
    };

    ID id;                // the ID of this item
    ItemType itemType;    // the type of item
    std::string name;     // the name
    double value;         // value
    double size;          // size (m^3)
    double speed;         // speed
    double danger;        // danger
    double damage;        // damage
    ItemAge itemAge;      // age
    bool shipRepairMetal; // is this a ship repair metal?
    bool blackMarketItem; // is this a blackmarket item?
    std::string portrait; // This refers to the image file

    // for Artifact items these properties are used:
    int planetid;
    int x;
    int y;

    // additional properties for Ruins:
    std::string description;

    // helper methods for working with the enumerated types
    static ItemType ItemTypeFromString(const std::string &s);
    static ItemAge ItemAgeFromString(const std::string &s);
};

// used to represent a collection of items; the collection is comprised of
// stacks of items; for example, 100 cans of root beer, 3 slim jims, etc; you
// can request the # of stacks, and then iterate over all the stacks; each stack
// returns to you the item and the quantity in that stack
class Items {
  public:
    Items();
    Items(const Items &rhs);
    virtual ~Items();
    Items &operator=(const Items &rhs);

    void Reset();
    bool Serialize(Archive &ar);

    /**
     * initializes this object to contain a random collection of items.
     * @param maxNumStacks # of stacks will not exceed this value
     * @param maxNumItemsPerStack # of items in any stack will not exceed this
     * value
     * @param typeFilter if provided, only items of the specified type mask will
     * be generated
     */

    /**
     * returns the number of stacks, each stack contains a set of a single item
     * type
     */
    int GetNumStacks();

    /**
     * returns a single stack of items - the item info and the number of that
     * item in the stack
     */
    void GetStack(int idx, Item &item, int &numItemsInStack);

    /**
     * add the specified quantity of the item ID to this object; the item(s)
     * will be added to the stack of that item type if already present.
     */
    void AddItems(ID id, int numItemsToAdd);

    /**
     * remove the specified quantity of the item ID from this object; the stack
     * of this item type will be decremented by the specified amount; it will
     * not go below zero.
     */
    void RemoveItems(ID id, int numItemsToRemove);

    /**
     * sets the # of the specified item to the value provided, overwriting the
     * existing value if the item is already present or adding it if not.
     */
    void SetItemCount(ID id, int numItems);

    /**
     * scan the vector for the item with the given id value and set the item
     * placeholder to the item matching that id
     */
    void Get_Item_By_ID(int id, Item &item, int &num_in_stack);

  private:
    std::vector<std::pair<ID, int>>
        stacks; // holds the stacks (each stack is a set of one item type)
};

// spectral classes
// these numbers match the ordering of the images in is_tiles.bmp and are not
// in astronomical order
typedef enum
{
    SC_INVALID = -1,
    SC_A = 1,
    SC_B = 2,
    SC_F = 3,
    SC_G = 4,
    SC_K = 5,
    SC_M = 6,
    SC_O = 7
} SpectralClass;

class Planet;

// a star
class Star {
  public:
    Star();
    Star(const Star &rhs);
    virtual ~Star();
    Star &operator=(const Star &rhs);

    // the ID of this star
    ID id;

    // the name of the star
    std::string name;

    // the position on the starmap
    CoordValue x, y;

    // spectral class of the star
    SpectralClass spectralClass;

    // the color of the star
    std::string color;

    // star temperature
    Kelvins temperature;

    // mass
    unsigned long mass;

    // radius
    unsigned long radius;

    // luminosity
    unsigned long luminosity;

    // used to access the planets in this star; this class owns the
    // memory and so it is not necessary to delete the returned pointer
    int GetNumPlanets();
    Planet *GetPlanet(int idx);   // by index [0...N)
    Planet *GetPlanetByID(ID id); // by ID

    // helper methods for working with the enumerated types
    static SpectralClass SpectralClassFromString(const std::string &s);

  private:
    std::vector<Planet *> planets;
    std::map<ID, Planet *> planetsByID;

    friend class DataMgr;
};

// planet sizes
typedef enum
{
    PS_INVALID = 0,
    PS_SMALL,
    PS_MEDIUM,
    PS_LARGE,
    PS_HUGE
} PlanetSize;

// planet types, values are the tiles used for the types
typedef enum
{
    PT_INVALID = 0,
    PT_ASTEROID = 7,
    PT_ROCKY = 6,
    PT_FROZEN = 4,
    PT_OCEANIC = 5,
    PT_MOLTEN = 3,
    PT_GASGIANT = 2,
    PT_ACIDIC = 8

} PlanetType;

// planet temperatures
typedef enum
{
    PTMP_INVALID = 0,
    PTMP_SUBARCTIC,
    PTMP_ARCTIC,
    PTMP_TEMPERATE,
    PTMP_TROPICAL,
    PTMP_SEARING,
    PTMP_INFERNO
} PlanetTemperature;

// planet gravity
typedef enum
{
    PG_INVALID = 0,
    PG_NEGLIGIBLE,
    PG_VERYLOW,
    PG_LOW,
    PG_OPTIMAL,
    PG_VERYHEAVY,
    PG_CRUSHING
} PlanetGravity;

// planet atmosphere
typedef enum
{
    PA_INVALID = 0,
    PA_NONE,
    PA_TRACEGASES,
    PA_BREATHABLE,
    PA_ACIDIC,
    PA_TOXIC,
    PA_FIRESTORM
} PlanetAtmosphere;

// planet weather
typedef enum
{
    PW_INVALID = 0,
    PW_NONE,
    PW_CALM,
    PW_MODERATE,
    PW_VIOLENT,
    PW_VERYVIOLENT
} PlanetWeather;

// a planet within a star
class Planet {
  public:
    Planet();
    Planet(const Planet &rhs);
    virtual ~Planet();
    Planet &operator=(const Planet &rhs);

    // the ID of this planet
    ID id;

    // the ID of the host star containing this planet
    ID hostStarID;

    // planet name
    std::string name;

    // planet size
    PlanetSize size;

    // planet type
    PlanetType type;

    // planet color
    std::string color;

    // planet temperature
    PlanetTemperature temperature;

    // planet gravity
    PlanetGravity gravity;

    // planet atmosphere
    PlanetAtmosphere atmosphere;

    // planet weather
    PlanetWeather weather;

    // landable property
    bool landable;

    // helper methods for working with the enumerated types
    static PlanetSize PlanetSizeFromString(const std::string &size);
    static std::string PlanetSizeToString(PlanetSize size);

    static PlanetType PlanetTypeFromString(const std::string &type);
    static std::string PlanetTypeToString(PlanetType type);

    static PlanetTemperature
    PlanetTemperatureFromString(const std::string &temperature);
    static std::string PlanetTemperatureToString(PlanetTemperature temperature);

    static PlanetGravity PlanetGravityFromString(const std::string &gravity);
    static std::string PlanetGravityToString(PlanetGravity gravity);

    static PlanetAtmosphere
    PlanetAtmosphereFromString(const std::string &atmosphere);
    static std::string PlanetAtmosphereToString(PlanetAtmosphere atmosphere);

    static PlanetWeather PlanetWeatherFromString(const std::string &weather);
};

class DataMgr {
  public:
    // don't create new instances of this class, use the instance provided in
    // the Module class
    DataMgr();
    virtual ~DataMgr();

    // only called once at game initialization to load in all the data and
    // prepare it for access shouldn't need to call this.
    bool Initialize();

    // used to access available items; memory is owned by this class; you should
    // not delete any returned objects
    int GetNumItems();
    Item *GetItem(int idx);                 // by index [0..N)
    Item *GetItemByID(ID id);               // by ID
    Item *GetItem(const std::string &name); // by name

    // used to access the available stars; memory is owned by this class; you
    // should not delete any returned objects
    int GetNumStars();
    Star *GetStar(int idx);                              // by index [0...N)
    Star *GetStarByID(ID id);                            // by ID
    Star *GetStarByLocation(CoordValue x, CoordValue y); // by location

    int GetNumFlux();
    const Flux *GetFlux(int idx);
    const Flux *GetFluxByLocation(CoordValue x, CoordValue y);

    // this version does not require a star parent class
    std::vector<Planet *> allPlanets;
    std::map<ID, Planet *> allPlanetsByID;
    Planet *GetPlanetByID(ID id);

    std::string GetRandMixedName();

  private:
    bool m_initialized;
    bool LoadItems();
    bool LoadGalaxy();
    bool LoadHumanNames();

    std::vector<Item *> items;
    std::map<ID, Item *> itemsByID;
    std::vector<Star *> stars;
    std::map<ID, Star *> starsByID;
    std::map<std::pair<CoordValue, CoordValue>, Star *> starsByLocation;
    std::vector<std::pair<std::string *, std::string *> *> humanNames;
    std::vector<Flux *> flux;
    std::map<std::pair<CoordValue, CoordValue>, Flux *> fluxByLocation;
};

#endif
