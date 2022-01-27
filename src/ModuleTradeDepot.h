#ifndef MODULETRADEDEPOT_H
#define MODULETRADEDEPOT_H
//#pragma once

#include "AudioSystem.h"
#include "DataMgr.h"
#include "Module.h"
#include "ResourceManager.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <map>

#define TRADEDEPOT_NUMBUTTONS 16

class Button;
namespace ScrollBox {
class ScrollBox;
}

class ModuleTradeDepot : public Module {
  public:
    ModuleTradeDepot();
    virtual ~ModuleTradeDepot();
    virtual bool on_init() override;
    virtual bool on_update() override;
    virtual bool on_draw(ALLEGRO_BITMAP *target) override;
    virtual bool on_key_pressed(ALLEGRO_KEYBOARD_EVENT *event) override;
    virtual bool on_mouse_move(ALLEGRO_MOUSE_EVENT *event) override;
    virtual bool on_mouse_button_down(ALLEGRO_MOUSE_EVENT *event) override;
    virtual bool on_mouse_button_up(ALLEGRO_MOUSE_EVENT *event) override;
    virtual bool on_event(ALLEGRO_EVENT *event) override;
    virtual bool on_close() override;

  private:
    ItemType item_to_display;

    // bug fix: needs 0-5, was capped at 5
    std::map<ItemType, ALLEGRO_BITMAP *> m_item_portraits;
    std::string portrait_string;

    typedef enum
    {
        TM_INVALID = 0,
        TM_TRADING,
        TM_PROMPTING
    } TradeModeType;
    TradeModeType m_tradeMode;

    typedef enum
    {
        PT_INVALID = 0,
        PT_SELL,
        PT_BUY
    } PromptType;
    PromptType m_promptType;
    Item m_promptItem;
    int m_promptNumItems;
    std::string m_promptText;

    bool m_clearListSelOnUpdate;

    void DoBuySell();
    void DoFinalizeTransaction();

    ALLEGRO_BITMAP *m_background;

    ScrollBox::ScrollBox *m_playerList;
    ScrollBox::ScrollBox *m_playerListNumItems;
    ScrollBox::ScrollBox *m_playerListValue;
    ScrollBox::ScrollBox *m_sellList;
    ScrollBox::ScrollBox *m_sellListNumItems;
    ScrollBox::ScrollBox *m_sellListValue;
    ScrollBox::ScrollBox *m_depotList;
    ScrollBox::ScrollBox *m_depotListNumItems;
    ScrollBox::ScrollBox *m_depotListValue;
    ScrollBox::ScrollBox *m_buyList;
    ScrollBox::ScrollBox *m_buyListNumItems;
    ScrollBox::ScrollBox *m_buyListValue;

    Button *m_buttons[TRADEDEPOT_NUMBUTTONS];
    std::map<Button *, bool> m_filterBtnMap;
    std::map<Button *, TradeModeType> m_modeBtnMap;
    Button *m_sellbuyBtn;
    Button *m_exitBtn;
    Button *m_checkoutBtn;
    Button *m_clearBtn;

    Button *m_filterAllBtn;
    Button *m_filterArtifactBtn;
    Button *m_filterSpecialtyGoodBtn;
    Button *m_filterMineralBtn;
    Button *m_filterLifeformBtn;
    Button *m_filterTradeItemBtn;
    Button *m_filterShipUpgradeBtn;

    bool exitToStarportCommons;

    ItemType m_filterType;
    Items m_playerItemsFiltered;
    Items m_depotItems;
    Items m_depotItemsFiltered;
    Items m_sellItems;
    Items m_buyItems;

    ALLEGRO_BITMAP *m_promptBackground;
    Button *m_spinUpBtn;
    Button *m_spinDownBtn;
    Button *m_allBtn;
    Button *m_okBtn;
    Button *m_cancelBtn;

    ALLEGRO_BITMAP *m_cursor[2];
    int m_cursorIdx;
    int m_cursorIdxDelay;

    void UpdateButtonStates();
    void UpdateLists();

    int m_sellTotal;
    int m_buyTotal;

    ResourceManager<ALLEGRO_BITMAP> m_resources;
};

#endif
