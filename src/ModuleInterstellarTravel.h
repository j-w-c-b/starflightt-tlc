/*
        STARFLIGHT - THE LOST COLONY
        ModuleInterstellarTravel.h - Handles interstellar travel on the main
   viewscreen Author: J.Harbour Date: January, 2007

        Flux system: Keith Patch
*/

#ifndef INTERSTELLARTRAVEL_H
#define INTERSTELLARTRAVEL_H

#include <string>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "DataMgr.h"
#include "Flux.h"
#include "GameState.h"
#include "Module.h"
#include "PlayerShipSprite.h"
#include "RichTextLabel.h"
#include "ScrolledModule.h"
#include "Sprite.h"
#include "TileScroller.h"
#include "Timer.h"

class ModuleInterstellarTravel : public Module {
  private:
    std::shared_ptr<TileScroller> m_scroller;

    int starFound;

    const Flux *flux;

    bool flag_Weapons;
    Timer timerEngaged;
    std::string alienRaceText, alienRaceTextPlural, depth;
    AlienRaces alienRace;
    double roll, proximity, odds;
    int movement_counter;

    int currentStar;
    Star *starSystem;
    PlayerShipSprite *ship;
    bool flag_DoNormalSpace;
    bool flag_FoundFlux;
    bool flag_nav;
    bool flag_thrusting;
    int flag_rotation;
    bool flag_launchEncounter;

    ALLEGRO_BITMAP *img_gui;

    void loadGalaxyData();
    void createGalaxy();
    void identifyStar();
    void calculateEnemyFleetSize();
    int getFleetSizeByRace(bool small_fleet);
    void load_flux();
    bool enter_flux();
    void identify_flux();
    void AugmentFuel(float percentage);
    bool RollEncounter(AlienRaces forceThisRace = ALIEN_NONE);
    void EnterStarSystem();
    double getPlayerGalacticX();
    double getPlayerGalacticY();
    double Distance(double x1, double y1, double x2, double y2);
    std::shared_ptr<Sample> m_klaxon;

  public:
    virtual bool on_init() override;
    virtual bool on_update() override;
    virtual bool on_draw(ALLEGRO_BITMAP *target) override;
    virtual bool on_key_down(ALLEGRO_KEYBOARD_EVENT *event) override;
    virtual bool on_key_up(ALLEGRO_KEYBOARD_EVENT *event) override;
    virtual bool on_event(ALLEGRO_EVENT *event) override;
    virtual bool on_close() override;
};

#endif
