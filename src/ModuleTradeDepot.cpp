#include <sstream>

#include "AudioSystem.h"
#include "Button.h"
#include "Events.h"
#include "Game.h"
#include "GameState.h"
#include "ModeMgr.h"
#include "ModuleTradeDepot.h"
#include "QuestMgr.h"
#include "ScrollBox.h"
#include "Util.h"
#include "tradedepot_resources.h"

using namespace std;
using namespace tradedepot_resources;

#define PLAYERLIST_X 10
#define PLAYERLIST_Y 240
#define PLAYERLIST_WIDTH 437
#define PLAYERLIST_NUMITEMS_WIDTH 53
#define PLAYERLIST_VALUE_WIDTH 84
#define PLAYERLIST_HEIGHT 166

#define SELLLIST_X 10
#define SELLLIST_Y 580
#define SELLLIST_WIDTH 437
#define SELLLIST_NUMITEMS_WIDTH 53
#define SELLLIST_VALUE_WIDTH 84
#define SELLLIST_HEIGHT 97

#define DEPOTLIST_X 572
#define DEPOTLIST_Y 240
#define DEPOTLIST_WIDTH 437
#define DEPOTLIST_NUMITEMS_WIDTH 53
#define DEPOTLIST_VALUE_WIDTH 84
#define DEPOTLIST_HEIGHT 166

#define BUYLIST_X 580
#define BUYLIST_Y 580
#define BUYLIST_WIDTH 437
#define BUYLIST_NUMITEMS_WIDTH 53
#define BUYLIST_VALUE_WIDTH 84
#define BUYLIST_HEIGHT 97

#define LIST_TEXTHEIGHT 29

#define PLAYER_BALANCE_X 435
#define PLAYER_BALANCE_Y 188
#define PLAYER_BALANCE_TEXTHEIGHT 48
#define PLAYER_BALANCE_TEXTCOL GREEN

#define SELLTOTAL_X 430
#define SELLTOTAL_Y 524
#define SELLTOTAL_TEXTHEIGHT 48
#define SELLTOTAL_TEXTCOL GREEN

#define BUYTOTAL_X 593
#define BUYTOTAL_Y 524
#define BUYTOTAL_TEXTHEIGHT 48
#define BUYTOTAL_TEXTCOL RED

#define EXITBTN_X 20
#define EXITBTN_Y 698

#define BUTTONS_X 460
#define SELLBUYBTN_Y 330
#define CHECKOUTBTN_Y 386
#define CLEARBTN_Y 623
#define BTNTEXTCOLOR BLACK
#define BTN_TEXTHEIGHT 30

#define FILTERBTN_Y 719
#define FILTERBTN_START_X 304
#define FILTERBTN_DELTA_X 147
#define FILTERBTN_TEXTHEIGHT 20

#define PROMPTBG_X 303
#define PROMPTBG_Y 69
#define SPINUPBTN_X 369
#define SPINUPBTN_Y 22
#define SPINDOWNBTN_X 369
#define SPINDOWNBTN_Y 42
#define ALLBTN_X 20
#define ALLBTN_Y 71
#define OKBTN_X 270
#define OKBTN_Y 71
#define CANCELBTN_X 270
#define CANCELBTN_Y 117
#define QTYTEXT_X 198
#define QTYTEXT_Y 25
#define PRICE_X 20
#define PRICE_Y 137

#define PROMPTBTN_TEXT_COLOR BLACK
#define PROMPT_TEXT_COLOR ORANGE

#define PROMPT_MAX_CHARS 8
#define PROMPT_VAL_TEXTHEIGHT 30
#define CURSOR_DELAY 10
#define CURSOR_Y QTYTEXT_Y

ALLEGRO_DEBUG_CHANNEL("ModuleTradeDepot")

const int ITEM_ENDURIUM = 54;

ModuleTradeDepot::ModuleTradeDepot(void)
    : m_tradeMode(TM_TRADING), m_clearListSelOnUpdate(false),
      m_background(NULL), m_playerList(NULL), m_playerListNumItems(NULL),
      m_playerListValue(NULL), m_sellList(NULL), m_sellListNumItems(NULL),
      m_sellListValue(NULL), m_depotList(NULL), m_depotListNumItems(NULL),
      m_depotListValue(NULL), m_buyList(NULL), m_buyListNumItems(NULL),
      m_buyListValue(NULL), m_sellbuyBtn(NULL), m_exitBtn(NULL),
      m_checkoutBtn(NULL), m_clearBtn(NULL), m_filterAllBtn(NULL),
      m_filterSpecialtyGoodBtn(NULL), m_filterMineralBtn(NULL),
      m_filterLifeformBtn(NULL), m_filterTradeItemBtn(NULL),
      exitToStarportCommons(false), m_filterType(IT_INVALID),
      m_promptBackground(NULL), m_spinUpBtn(NULL), m_spinDownBtn(NULL),
      m_allBtn(NULL), m_okBtn(NULL), m_cancelBtn(NULL), m_cursorIdx(0),
      m_cursorIdxDelay(0), m_resources(TRADEDEPOT_IMAGES) {
    for (int i = 0; i < TRADEDEPOT_NUMBUTTONS; i++) {
        m_buttons[i] = NULL;
    }
}

ModuleTradeDepot::~ModuleTradeDepot(void) {}

bool
ModuleTradeDepot::on_init() {
    item_to_display = IT_INVALID;
    portrait_string = "";

    m_item_portraits[IT_ARTIFACT] = m_resources[I_T_ALIENARTIFACT];
    m_item_portraits[IT_MINERAL] = m_resources[I_T_GEMS];
    m_item_portraits[IT_LIFEFORM] = m_resources[I_T_RABID_VERTRUK];
    m_item_portraits[IT_TRADEITEM] = m_resources[I_T_SEEDS];

    // load the datafile
    if (!m_resources.load()) {
        g_game->message("TradeDepot: Error loading resources");
        return false;
    }

    exitToStarportCommons = false;

    g_game->gameState->m_credits = g_game->gameState->getCredits();

    // empty the trade depot
    m_depotItems.Reset();

    // there must always be a supply of endurium!
    m_depotItems.SetItemCount(ITEM_ENDURIUM, Util::Random(1, 6) + 6);

    // add ship repair metals
    m_depotItems.SetItemCount(32, Util::Random(1, 6) + 6); // cobalt
    m_depotItems.SetItemCount(36, Util::Random(1, 6) + 6); // molybdenum
    m_depotItems.SetItemCount(39, Util::Random(1, 6) + 6); // aluminum
    m_depotItems.SetItemCount(40, Util::Random(1, 6) + 6); // titanium
    m_depotItems.SetItemCount(44, Util::Random(1, 6) + 6); // silica

    // load gui images
    m_background = m_resources[I_TRADEDEPOT_BACKGROUND];

    m_playerListValue = new ScrollBox::ScrollBox(
        g_game->font24,
        ScrollBox::SB_LIST,
        PLAYERLIST_X + PLAYERLIST_WIDTH - PLAYERLIST_VALUE_WIDTH,
        PLAYERLIST_Y,
        PLAYERLIST_VALUE_WIDTH,
        PLAYERLIST_HEIGHT,
        EVENT_TRADE_PLAYER_LIST);
    if (m_playerListValue == NULL)
        return false;

    m_playerListNumItems = new ScrollBox::ScrollBox(
        g_game->font24,
        ScrollBox::SB_LIST,
        PLAYERLIST_X + PLAYERLIST_WIDTH - PLAYERLIST_VALUE_WIDTH
            - PLAYERLIST_NUMITEMS_WIDTH,
        PLAYERLIST_Y,
        PLAYERLIST_NUMITEMS_WIDTH,
        PLAYERLIST_HEIGHT,
        EVENT_TRADE_PLAYER_LIST);
    if (m_playerListNumItems == NULL)
        return false;

    m_playerList = new ScrollBox::ScrollBox(
        g_game->font24,
        ScrollBox::SB_LIST,
        PLAYERLIST_X,
        PLAYERLIST_Y,
        PLAYERLIST_WIDTH,
        PLAYERLIST_HEIGHT,
        EVENT_TRADE_PLAYER_LIST);
    if (m_playerList == NULL)
        return false;

    m_playerListNumItems->LinkBox(m_playerList);
    m_playerListValue->LinkBox(m_playerListNumItems);

    m_sellListValue = new ScrollBox::ScrollBox(
        g_game->font24,
        ScrollBox::SB_LIST,
        SELLLIST_X + SELLLIST_WIDTH - SELLLIST_VALUE_WIDTH,
        SELLLIST_Y,
        SELLLIST_VALUE_WIDTH,
        SELLLIST_HEIGHT,
        EVENT_TRADE_SELL_LIST);
    if (m_sellListValue == NULL)
        return false;

    m_sellListNumItems = new ScrollBox::ScrollBox(
        g_game->font24,
        ScrollBox::SB_LIST,
        SELLLIST_X + SELLLIST_WIDTH - SELLLIST_VALUE_WIDTH
            - SELLLIST_NUMITEMS_WIDTH,
        SELLLIST_Y,
        SELLLIST_NUMITEMS_WIDTH,
        SELLLIST_HEIGHT,
        EVENT_TRADE_SELL_LIST);
    if (m_sellListNumItems == NULL)
        return false;

    m_sellList = new ScrollBox::ScrollBox(
        g_game->font24,
        ScrollBox::SB_LIST,
        SELLLIST_X,
        SELLLIST_Y,
        SELLLIST_WIDTH,
        SELLLIST_HEIGHT,
        EVENT_TRADE_SELL_LIST);

    m_sellListNumItems->LinkBox(m_sellList);
    m_sellListValue->LinkBox(m_sellListNumItems);

    m_depotListValue = new ScrollBox::ScrollBox(
        g_game->font24,
        ScrollBox::SB_LIST,
        DEPOTLIST_X + DEPOTLIST_WIDTH - DEPOTLIST_VALUE_WIDTH,
        DEPOTLIST_Y,
        DEPOTLIST_VALUE_WIDTH,
        DEPOTLIST_HEIGHT,
        EVENT_TRADE_DEPOT_LIST);

    m_depotListNumItems = new ScrollBox::ScrollBox(
        g_game->font24,
        ScrollBox::SB_LIST,
        DEPOTLIST_X + DEPOTLIST_WIDTH - DEPOTLIST_VALUE_WIDTH
            - DEPOTLIST_NUMITEMS_WIDTH,
        DEPOTLIST_Y,
        DEPOTLIST_NUMITEMS_WIDTH,
        DEPOTLIST_HEIGHT,
        EVENT_TRADE_DEPOT_LIST);
    if (m_depotListNumItems == NULL)
        return false;

    m_depotList = new ScrollBox::ScrollBox(
        g_game->font24,
        ScrollBox::SB_LIST,
        DEPOTLIST_X,
        DEPOTLIST_Y,
        DEPOTLIST_WIDTH,
        DEPOTLIST_HEIGHT,
        EVENT_TRADE_DEPOT_LIST);
    if (m_depotList == NULL)
        return false;

    m_depotListNumItems->LinkBox(m_depotList);
    m_depotListValue->LinkBox(m_depotListNumItems);

    m_buyListValue = new ScrollBox::ScrollBox(
        g_game->font24,
        ScrollBox::SB_LIST,
        BUYLIST_X + BUYLIST_WIDTH - BUYLIST_VALUE_WIDTH,
        BUYLIST_Y,
        BUYLIST_VALUE_WIDTH,
        BUYLIST_HEIGHT,
        EVENT_TRADE_BUY_LIST);
    if (m_buyListValue == NULL)
        return false;

    m_buyListNumItems = new ScrollBox::ScrollBox(
        g_game->font24,
        ScrollBox::SB_LIST,
        BUYLIST_X + BUYLIST_WIDTH - BUYLIST_VALUE_WIDTH
            - BUYLIST_NUMITEMS_WIDTH,
        BUYLIST_Y,
        BUYLIST_NUMITEMS_WIDTH,
        BUYLIST_HEIGHT,
        EVENT_TRADE_BUY_LIST);
    if (m_buyListNumItems == NULL)
        return false;

    m_buyList = new ScrollBox::ScrollBox(
        g_game->font24,
        ScrollBox::SB_LIST,
        BUYLIST_X,
        BUYLIST_Y,
        BUYLIST_WIDTH,
        BUYLIST_HEIGHT,
        EVENT_TRADE_BUY_LIST);
    if (m_buyList == NULL)
        return false;

    m_buyListNumItems->LinkBox(m_buyList);
    m_buyListValue->LinkBox(m_buyListNumItems);

    // create exit button
    m_exitBtn = new Button(
        m_resources[I_GENERIC_EXIT_BTN_NORM],
        m_resources[I_GENERIC_EXIT_BTN_OVER],
        NULL,
        EXITBTN_X,
        EXITBTN_Y,
        0,
        EVENT_TRADE_EXIT,
        g_game->font24,
        "Exit",
        BLACK);
    if (m_exitBtn == NULL)
        return false;
    if (!m_exitBtn->IsInitialized())
        return false;
    m_buttons[1] = m_exitBtn;
    m_modeBtnMap[m_exitBtn] = TM_TRADING;

    // create buy/sell button
    m_sellbuyBtn = new Button(
        m_resources[I_TRADEDEPOT_BTN],
        m_resources[I_TRADEDEPOT_BTN_MO],
        NULL,
        BUTTONS_X,
        SELLBUYBTN_Y,
        0,
        EVENT_TRADE_SELL_BUY,
        g_game->font24,
        "SellBuy",
        BTNTEXTCOLOR);
    if (m_sellbuyBtn == NULL)
        return false;
    if (!m_sellbuyBtn->IsInitialized())
        return false;
    m_buttons[0] = m_sellbuyBtn;
    m_modeBtnMap[m_sellbuyBtn] = TM_TRADING;

    // create confirm button
    m_checkoutBtn = new Button(
        m_resources[I_TRADEDEPOT_BTN],
        m_resources[I_TRADEDEPOT_BTN_MO],
        NULL,
        BUTTONS_X,
        CHECKOUTBTN_Y,
        0,
        EVENT_TRADE_CHECKOUT,
        g_game->font24,
        "Confirm",
        BTNTEXTCOLOR);
    if (m_checkoutBtn == NULL)
        return false;
    if (!m_checkoutBtn->IsInitialized())
        return false;
    m_buttons[2] = m_checkoutBtn;
    m_modeBtnMap[m_checkoutBtn] = TM_TRADING;

    // create clear button
    m_clearBtn = new Button(
        m_resources[I_TRADEDEPOT_BTN],
        m_resources[I_TRADEDEPOT_BTN_MO],
        NULL,
        BUTTONS_X,
        CLEARBTN_Y,
        0,
        EVENT_TRADE_CLEAR,
        g_game->font24,
        "Clear",
        BTNTEXTCOLOR);
    if (m_clearBtn == NULL)
        return false;
    if (!m_clearBtn->IsInitialized())
        return false;
    m_buttons[3] = m_clearBtn;
    m_modeBtnMap[m_clearBtn] = TM_TRADING;

    // create all button
    int x = FILTERBTN_START_X;
    m_filterAllBtn = new Button(
        m_resources[I_TRADEDEPOT_FILTERBTN],
        m_resources[I_TRADEDEPOT_FILTERBTN_MO],
        NULL,
        x,
        FILTERBTN_Y,
        0,
        EVENT_TRADE_FILTER_ALL,
        g_game->font18,
        "All",
        BTNTEXTCOLOR);
    if (m_filterAllBtn == NULL)
        return false;
    if (!m_filterAllBtn->IsInitialized())
        return false;
    m_buttons[4] = m_filterAllBtn;
    m_filterBtnMap[m_filterAllBtn] = true;
    m_modeBtnMap[m_filterAllBtn] = TM_TRADING;

    // create artifacts button
    x += FILTERBTN_DELTA_X;
    m_filterArtifactBtn = new Button(
        m_resources[I_TRADEDEPOT_FILTERBTN],
        m_resources[I_TRADEDEPOT_FILTERBTN_MO],
        NULL,
        x,
        FILTERBTN_Y,
        0,
        EVENT_TRADE_FILTER_ARTIFACT,
        g_game->font18,
        "Artifacts",
        BTNTEXTCOLOR);
    if (m_filterArtifactBtn == NULL)
        return false;
    if (!m_filterArtifactBtn->IsInitialized())
        return false;
    m_buttons[5] = m_filterArtifactBtn;
    m_filterBtnMap[m_filterArtifactBtn] = true;
    m_modeBtnMap[m_filterArtifactBtn] = TM_TRADING;

    // create spec goods button
    x += FILTERBTN_DELTA_X;
    m_filterSpecialtyGoodBtn = new Button(
        m_resources[I_TRADEDEPOT_FILTERBTN],
        m_resources[I_TRADEDEPOT_FILTERBTN_MO],
        NULL,
        x,
        FILTERBTN_Y,
        0,
        EVENT_TRADE_FILTER_SPECIALTY_GOOD,
        g_game->font18,
        "Spec Goods",
        BTNTEXTCOLOR);
    if (m_filterSpecialtyGoodBtn == NULL)
        return false;
    if (!m_filterSpecialtyGoodBtn->IsInitialized())
        return false;
    m_buttons[6] = m_filterSpecialtyGoodBtn;
    m_filterBtnMap[m_filterSpecialtyGoodBtn] = true;
    m_modeBtnMap[m_filterSpecialtyGoodBtn] = TM_TRADING;

    // create minerals button
    x += FILTERBTN_DELTA_X;
    m_filterMineralBtn = new Button(
        m_resources[I_TRADEDEPOT_FILTERBTN],
        m_resources[I_TRADEDEPOT_FILTERBTN_MO],
        NULL,
        x,
        FILTERBTN_Y,
        0,
        EVENT_TRADE_FILTER_MINERAL,
        g_game->font18,
        "Minerals",
        BTNTEXTCOLOR);
    if (m_filterMineralBtn == NULL)
        return false;
    if (!m_filterMineralBtn->IsInitialized())
        return false;
    m_buttons[7] = m_filterMineralBtn;
    m_filterBtnMap[m_filterMineralBtn] = true;
    m_modeBtnMap[m_filterMineralBtn] = TM_TRADING;

    // create lifeforms button
    x += FILTERBTN_DELTA_X;
    m_filterLifeformBtn = new Button(
        m_resources[I_TRADEDEPOT_FILTERBTN],
        m_resources[I_TRADEDEPOT_FILTERBTN_MO],
        NULL,
        x,
        FILTERBTN_Y,
        0,
        EVENT_TRADE_FILTER_LIFEFORM,
        g_game->font18,
        "Lifeforms",
        BTNTEXTCOLOR);
    if (m_filterLifeformBtn == NULL)
        return false;
    if (!m_filterLifeformBtn->IsInitialized())
        return false;
    m_buttons[8] = m_filterLifeformBtn;
    m_filterBtnMap[m_filterLifeformBtn] = true;
    m_modeBtnMap[m_filterLifeformBtn] = TM_TRADING;

    // create trade items button
    x += FILTERBTN_DELTA_X;
    m_filterTradeItemBtn = new Button(
        m_resources[I_TRADEDEPOT_FILTERBTN],
        m_resources[I_TRADEDEPOT_FILTERBTN_MO],
        NULL,
        x,
        FILTERBTN_Y,
        0,
        EVENT_TRADE_FILTER_TRADE_ITEM,
        g_game->font24,
        "Trade Items",
        BTNTEXTCOLOR);
    if (m_filterTradeItemBtn == NULL)
        return false;
    if (!m_filterTradeItemBtn->IsInitialized())
        return false;
    m_buttons[9] = m_filterTradeItemBtn;
    m_filterBtnMap[m_filterTradeItemBtn] = true;
    m_modeBtnMap[m_filterTradeItemBtn] = TM_TRADING;

    // create ship upgrades button
    x += FILTERBTN_DELTA_X;
    m_filterShipUpgradeBtn = new Button(
        m_resources[I_TRADEDEPOT_FILTERBTN],
        m_resources[I_TRADEDEPOT_FILTERBTN_MO],
        NULL,
        x,
        FILTERBTN_Y,
        0,
        EVENT_TRADE_FILTER_SHIP_UPGRADE,
        g_game->font24,
        "Ship Upgrades",
        BTNTEXTCOLOR);
    if (m_filterShipUpgradeBtn == NULL)
        return false;
    if (!m_filterShipUpgradeBtn->IsInitialized())
        return false;
    m_buttons[10] = m_filterShipUpgradeBtn;
    m_filterBtnMap[m_filterShipUpgradeBtn] = true;
    m_modeBtnMap[m_filterShipUpgradeBtn] = TM_TRADING;

    // load quantity prompt ALLEGRO_BITMAP
    m_promptBackground = m_resources[I_TRADEDEPOT_QUANTITY_PROMPT];

    // create spinup buttons
    m_spinUpBtn = new Button(
        m_resources[I_TRADEDEPOT_SPINUPBTN],
        m_resources[I_TRADEDEPOT_SPINUPBTN_MO],
        NULL,
        SPINUPBTN_X + PROMPTBG_X,
        SPINUPBTN_Y + PROMPTBG_Y,
        0,
        EVENT_TRADE_SPIN_UP);
    if (!m_spinUpBtn->IsInitialized())
        return false;
    m_buttons[11] = m_spinUpBtn;
    m_modeBtnMap[m_spinUpBtn] = TM_PROMPTING;

    // create spindown buttons
    m_spinDownBtn = new Button(
        m_resources[I_TRADEDEPOT_SPINDOWNBTN],
        m_resources[I_TRADEDEPOT_SPINDOWNBTN_MO],
        NULL,
        SPINDOWNBTN_X + PROMPTBG_X,
        SPINDOWNBTN_Y + PROMPTBG_Y,
        0,
        EVENT_TRADE_SPIN_DOWN);
    if (m_spinDownBtn == NULL)
        return false;
    if (!m_spinDownBtn->IsInitialized())
        return false;
    m_buttons[12] = m_spinDownBtn;
    m_modeBtnMap[m_spinDownBtn] = TM_PROMPTING;

    // create all button
    m_allBtn = new Button(
        m_resources[I_TRADEDEPOT_PROMPTBTN],
        m_resources[I_TRADEDEPOT_PROMPTBTN_MO],
        NULL,
        ALLBTN_X + PROMPTBG_X,
        ALLBTN_Y + PROMPTBG_Y,
        0,
        EVENT_TRADE_ALL,
        g_game->font24,
        "All",
        PROMPTBTN_TEXT_COLOR);
    if (m_allBtn == NULL)
        return false;
    if (!m_allBtn->IsInitialized())
        return false;
    m_buttons[13] = m_allBtn;
    m_modeBtnMap[m_allBtn] = TM_PROMPTING;

    // create ok button
    m_okBtn = new Button(
        m_resources[I_TRADEDEPOT_PROMPTBTN],
        m_resources[I_TRADEDEPOT_PROMPTBTN_MO],
        NULL,
        OKBTN_X + PROMPTBG_X,
        OKBTN_Y + PROMPTBG_Y,
        0,
        EVENT_TRADE_OK,
        g_game->font24,
        "OK",
        PROMPTBTN_TEXT_COLOR);
    if (m_okBtn == NULL)
        return false;
    if (!m_okBtn->IsInitialized())
        return false;
    m_buttons[14] = m_okBtn;
    m_modeBtnMap[m_okBtn] = TM_PROMPTING;

    // create cancel button
    m_cancelBtn = new Button(
        m_resources[I_TRADEDEPOT_PROMPTBTN],
        m_resources[I_TRADEDEPOT_PROMPTBTN_MO],
        NULL,
        CANCELBTN_X + PROMPTBG_X,
        CANCELBTN_Y + PROMPTBG_Y,
        0,
        EVENT_TRADE_CANCEL,
        g_game->font24,
        "Cancel",
        PROMPTBTN_TEXT_COLOR);
    if (m_cancelBtn == NULL)
        return false;
    if (!m_cancelBtn->IsInitialized())
        return false;
    m_buttons[15] = m_cancelBtn;
    m_modeBtnMap[m_cancelBtn] = TM_PROMPTING;

    m_cursor[0] = m_resources[I_TRADEDEPOT_CURSOR0];
    m_cursor[1] = m_resources[I_TRADEDEPOT_CURSOR1];

    m_sellTotal = 0;
    m_buyTotal = 0;

    m_tradeMode = TM_TRADING;

    // load sound file
    if (!g_game->audioSystem->SampleExists(
            "buttonclick")) { // switched to named sound: see debug log.
        if (!g_game->audioSystem->Load(
                "data/tradedepot/buttonclick.ogg", "buttonclick")) {
            g_game->message("ButtonClick: Error loading music");
            return false;
        }
    }

    // reset transactions
    m_sellItems.Reset();
    m_buyItems.Reset();

    // refresh lists
    UpdateLists();
    UpdateButtonStates();

    // tell questmgr that this module has been visited
    g_game->questMgr->raiseEvent(20);

    return true;
}

bool
ModuleTradeDepot::on_update() {
    if (m_clearListSelOnUpdate) {
        m_playerListValue->SetSelectedIndex(-1);
        m_depotListValue->SetSelectedIndex(-1);
        m_sellListValue->SetSelectedIndex(-1);
        m_buyListValue->SetSelectedIndex(-1);

        m_clearListSelOnUpdate = false;
    }

    if (exitToStarportCommons) {
        if (g_game->audioSystem->IsPlaying("buttonclick")
            == false) { // switched to named sound: see debug log.
            g_game->LoadModule(MODULE_STARPORT);
            return false;
        }
    }
    return true;
}

bool
ModuleTradeDepot::on_draw(ALLEGRO_BITMAP *target) {
    al_set_target_bitmap(target);
    al_draw_bitmap(m_background, 0, 0, 0);

    m_playerListValue->Draw(target);
    m_sellListValue->Draw(target);
    m_depotListValue->Draw(target);
    m_buyListValue->Draw(target);

    ostringstream balStr;
    balStr << g_game->gameState->getCredits();
    al_draw_text(
        g_game->font48,
        PLAYER_BALANCE_TEXTCOL,
        PLAYER_BALANCE_X,
        PLAYER_BALANCE_Y,
        ALLEGRO_ALIGN_RIGHT,
        balStr.str().c_str());

    if (m_sellTotal > 0) {
        ostringstream sellStr;
        sellStr << m_sellTotal;
        al_draw_text(
            g_game->font48,
            SELLTOTAL_TEXTCOL,
            SELLTOTAL_X,
            SELLTOTAL_Y,
            ALLEGRO_ALIGN_RIGHT,
            sellStr.str().c_str());
    }

    if (m_buyTotal > 0) {
        ostringstream buyStr;
        buyStr << m_buyTotal;
        al_draw_text(
            g_game->font48,
            BUYTOTAL_TEXTCOL,
            BUYTOTAL_X,
            BUYTOTAL_Y,
            0,
            buyStr.str().c_str());
    }

    if (m_tradeMode == TM_PROMPTING) {
        al_draw_bitmap(m_promptBackground, PROMPTBG_X, PROMPTBG_Y, 0);

        al_draw_bitmap(m_promptBackground, PROMPTBG_X, PROMPTBG_Y, 0);
        al_draw_text(
            g_game->font24,
            PROMPT_TEXT_COLOR,
            QTYTEXT_X + PROMPTBG_X,
            QTYTEXT_Y + PROMPTBG_Y,
            0,
            m_promptText.c_str());

        int nlen = al_get_text_width(g_game->font24, m_promptText.c_str());
        al_draw_bitmap(
            m_cursor[m_cursorIdx],
            QTYTEXT_X + PROMPTBG_X + nlen + 2,
            CURSOR_Y + PROMPTBG_Y,
            0);

        if (++m_cursorIdxDelay > CURSOR_DELAY) {
            m_cursorIdxDelay = 0;
            m_cursorIdx++;
            if (m_cursorIdx > 1)
                m_cursorIdx = 0;
        }

        int qty = atoi(m_promptText.c_str());
        al_draw_textf(
            g_game->font24,
            PROMPTBTN_TEXT_COLOR,
            PRICE_X + PROMPTBG_X,
            PRICE_Y + PROMPTBG_Y,
            0,
            "Price: %d",
            static_cast<int>(qty * m_promptItem.value));
    }

    for (int i = 0; i < TRADEDEPOT_NUMBUTTONS; i++) {
        if (m_modeBtnMap[m_buttons[i]] == m_tradeMode)
            m_buttons[i]->Run(target);
    }

    // draw portrait
    // case logic
    // 447,443
    if (portrait_string == "") {
        ALLEGRO_BITMAP *portrait = m_item_portraits[item_to_display];
        if (portrait) {
            al_draw_bitmap(portrait, 447, 443, 0);
        }
    } else {
        ALLEGRO_BITMAP *temp_bmp;
        std::string temp_string =
            Util::resource_path("data/tradedepot/" + portrait_string);
        temp_bmp = al_load_bitmap(temp_string.c_str());
        if (temp_bmp) {
            al_draw_bitmap(temp_bmp, 447, 443, 0);
            al_destroy_bitmap(temp_bmp);
        } else {
            string s = "TradeDepot: ERROR! " + temp_string + " not found!";
            ALLEGRO_DEBUG("%s\n", s.c_str());
        }
    }
    return true;
}

bool
ModuleTradeDepot::on_close() {
    if (m_playerListValue != NULL) {
        delete m_playerListValue;
        m_playerListValue = NULL;
    }

    if (m_sellListValue != NULL) {
        delete m_sellListValue;
        m_sellListValue = NULL;
    }

    if (m_depotListValue != NULL) {
        delete m_depotListValue;
        m_depotListValue = NULL;
    }

    if (m_buyListValue != NULL) {
        delete m_buyListValue;
        m_buyListValue = NULL;
    }

    for (int i = 0; i < TRADEDEPOT_NUMBUTTONS; i++) {
        m_buttons[i]->Destroy();
        delete m_buttons[i];
        m_buttons[i] = NULL;
    }

    m_resources.unload();
    return true;
}

bool
ModuleTradeDepot::on_key_pressed(ALLEGRO_KEYBOARD_EVENT *event) {
    if (m_tradeMode == TM_PROMPTING) {
        if (isdigit(event->unichar)) {
            char c = event->unichar;

            if (m_promptText.size() < PROMPT_MAX_CHARS) {
                m_promptText += c;
                return false;
            }
        } else if (event->keycode == ALLEGRO_KEY_BACKSPACE) {
            if (m_promptText.size() > 0) {
                string newText =
                    m_promptText.substr(0, m_promptText.size() - 1);
                m_promptText = newText;
                return false;
            }
        }
    }
    return true;
}

bool
ModuleTradeDepot::on_mouse_move(ALLEGRO_MOUSE_EVENT *event) {
    int x = event->x;
    int y = event->y;

    if (m_tradeMode == TM_TRADING) {
        m_playerListValue->OnMouseMove(x, y);
        m_sellListValue->OnMouseMove(x, y);
        m_depotListValue->OnMouseMove(x, y);
        m_buyListValue->OnMouseMove(x, y);
    }

    for (int i = 0; i < TRADEDEPOT_NUMBUTTONS; i++) {
        if (m_modeBtnMap[m_buttons[i]] == m_tradeMode)
            m_buttons[i]->OnMouseMove(x, y);
    }
    return true;
}

bool
ModuleTradeDepot::on_mouse_button_down(ALLEGRO_MOUSE_EVENT *event) {
    int button = event->button - 1;
    int x = event->x;
    int y = event->y;

    if (m_tradeMode == TM_TRADING) {
        m_playerListValue->OnMousePressed(button, x, y);
        m_sellListValue->OnMousePressed(button, x, y);
        m_depotListValue->OnMousePressed(button, x, y);
        m_buyListValue->OnMousePressed(button, x, y);
    }

    return true;
}

bool
ModuleTradeDepot::on_mouse_button_up(ALLEGRO_MOUSE_EVENT *event) {
    int button = event->button - 1;
    int x = event->x;
    int y = event->y;

    if (m_tradeMode == TM_TRADING) {
        m_playerListValue->OnMouseReleased(button, x, y);
        m_sellListValue->OnMouseReleased(button, x, y);
        m_depotListValue->OnMouseReleased(button, x, y);
        m_buyListValue->OnMouseReleased(button, x, y);

        if (is_mouse_click(event)) {
            m_playerListValue->OnMouseClick(button, x, y);
            m_sellListValue->OnMouseClick(button, x, y);
            m_depotListValue->OnMouseClick(button, x, y);
            m_buyListValue->OnMouseClick(button, x, y);
        }
    }

    for (int i = 0; i < TRADEDEPOT_NUMBUTTONS; i++) {
        if (m_modeBtnMap[m_buttons[i]] == m_tradeMode)
            m_buttons[i]->OnMouseReleased(button, x, y);
    }
    return true;
}

bool
ModuleTradeDepot::on_event(ALLEGRO_EVENT *event) {
    bool playBtnClick = false;

    int eventType = event->type;

    if (eventType == EVENT_TRADE_PLAYER_LIST) {
        // clear other list selections
        m_depotListValue->SetSelectedIndex(-1);
        m_sellListValue->SetSelectedIndex(-1);
        m_buyListValue->SetSelectedIndex(-1);
        playBtnClick = true;

        // determine item type of selected item so that we can show the correct
        // portrait
        Item item;
        int numItems;
        m_playerItemsFiltered.GetStack(
            m_playerListValue->GetSelectedIndex(), item, numItems);
        item_to_display = item.itemType;
        portrait_string = item.portrait;
    } else if (eventType == EVENT_TRADE_DEPOT_LIST) {
        // clear other list selections
        m_playerListValue->SetSelectedIndex(-1);
        m_sellListValue->SetSelectedIndex(-1);
        m_buyListValue->SetSelectedIndex(-1);
        playBtnClick = true;

        // determine item type of selected item so that we can show the correct
        // portrait
        Item item;
        int numItems;
        m_depotItemsFiltered.GetStack(
            m_depotListValue->GetSelectedIndex(), item, numItems);
        item_to_display = item.itemType;
        portrait_string = item.portrait;
    } else if (eventType == EVENT_TRADE_SELL_LIST) {
        m_sellListValue->SetSelectedIndex(-1);
    } else if (eventType == EVENT_TRADE_BUY_LIST) {
        m_buyListValue->SetSelectedIndex(-1);
    } else if (eventType == EVENT_TRADE_SELL_BUY) {
        if (m_playerListValue->GetSelectedIndex() >= 0) {
            Item item;
            int numItems;
            m_playerItemsFiltered.GetStack(
                m_playerListValue->GetSelectedIndex(), item, numItems);
            m_promptItem = item;
            m_promptNumItems = numItems;
            m_promptType = PT_SELL;
            m_tradeMode = TM_PROMPTING;
            m_promptText = "1";
        } else if (m_depotListValue->GetSelectedIndex() >= 0) {
            Item item;
            int numItems;
            m_depotItemsFiltered.GetStack(
                m_depotListValue->GetSelectedIndex(), item, numItems);
            m_promptItem = item;
            m_promptNumItems = numItems;
            m_promptType = PT_BUY;
            m_tradeMode = TM_PROMPTING;
            m_promptText = "1";
        }

        playBtnClick = true;
    } else if (eventType == EVENT_TRADE_EXIT) {
        exitToStarportCommons = true;
        playBtnClick = true;
    } else if (eventType == EVENT_TRADE_CHECKOUT) {
        DoFinalizeTransaction();
        playBtnClick = true;
    } else if (eventType == EVENT_TRADE_CLEAR) {
        m_sellItems.Reset();
        m_buyItems.Reset();
        UpdateLists();

        playBtnClick = true;
    } else if (eventType == EVENT_TRADE_FILTER_ALL) {
        m_filterType = IT_INVALID;
        playBtnClick = true;
        UpdateLists();
        m_playerListValue->ScrollToTop();
        m_depotListValue->ScrollToTop();
    } else if (eventType == EVENT_TRADE_FILTER_ARTIFACT) {
        m_filterType = IT_ARTIFACT;
        playBtnClick = true;
        UpdateLists();
        m_playerListValue->ScrollToTop();
        m_depotListValue->ScrollToTop();
    } else if (eventType == EVENT_TRADE_FILTER_MINERAL) {
        m_filterType = IT_MINERAL;
        playBtnClick = true;
        UpdateLists();
        m_playerListValue->ScrollToTop();
        m_depotListValue->ScrollToTop();
    } else if (eventType == EVENT_TRADE_FILTER_LIFEFORM) {
        m_filterType = IT_LIFEFORM;
        playBtnClick = true;
        UpdateLists();
        m_playerListValue->ScrollToTop();
        m_depotListValue->ScrollToTop();
    } else if (eventType == EVENT_TRADE_FILTER_TRADE_ITEM) {
        m_filterType = IT_TRADEITEM;
        playBtnClick = true;
        UpdateLists();
        m_playerListValue->ScrollToTop();
        m_depotListValue->ScrollToTop();
    } else if (eventType == EVENT_TRADE_SPIN_UP) {
        int val = atoi(m_promptText.c_str());
        val++;
        if (val > m_promptNumItems)
            val = m_promptNumItems;
        if (val > 99999999)
            val = 99999999;
        ostringstream str;
        str << val;
        m_promptText = str.str();

        playBtnClick = true;
    } else if (eventType == EVENT_TRADE_SPIN_DOWN) {
        int val = atoi(m_promptText.c_str());
        val--;
        if (val < 0)
            val = 0;
        if (val > m_promptNumItems)
            val = m_promptNumItems;
        if (val > 99999999)
            val = 99999999;
        ostringstream str;
        str << val;
        m_promptText = str.str();

        playBtnClick = true;
    } else if (eventType == EVENT_TRADE_ALL) {
        ostringstream str;
        str << m_promptNumItems;
        m_promptText = str.str();

        playBtnClick = true;
    } else if (eventType == EVENT_TRADE_OK) {
        DoBuySell();
        m_tradeMode = TM_TRADING;
        playBtnClick = true;
        m_clearListSelOnUpdate = true;
    } else if (eventType == EVENT_TRADE_CANCEL) {
        m_tradeMode = TM_TRADING;
        playBtnClick = true;
        m_clearListSelOnUpdate = true;
    }

    if (playBtnClick) {
        g_game->audioSystem->Play(
            "buttonclick"); // switched to named sound: see debug log.
    }

    UpdateButtonStates();
    return true;
}

void
ModuleTradeDepot::DoBuySell() {
    int qty = atoi(m_promptText.c_str());

    if (qty > m_promptNumItems)
        qty = m_promptNumItems;
    if (qty > 99999999)
        qty = 99999999;
    if (qty <= 0)
        return;

    if (m_promptType == PT_SELL) {
        m_sellItems.SetItemCount(m_promptItem.id, qty);
        UpdateLists();
    } else if (m_promptType == PT_BUY) {
        // verify the ship has cargo pods
        if (g_game->gameState->m_ship.getCargoPodCount() == 0) {
            g_game->ShowMessageBoxWindow(
                "",
                "Your ship has no Cargo Pods! You must purchase one before "
                "taking on cargo.",
                300,
                300,
                WHITE);
            return;
        }

        m_buyItems.SetItemCount(m_promptItem.id, qty);
        UpdateLists();
    }
}

void
ModuleTradeDepot::DoFinalizeTransaction() {
    Item item;
    int numItems;

    // we sell first since it will bring both credits and cargo slots
    for (int i = 0; i < m_sellItems.GetNumStacks(); i++) {
        m_sellItems.GetStack(i, item, numItems);
        g_game->gameState->m_credits += (int)(item.value * numItems);
        g_game->gameState->m_items.RemoveItems(item.id, numItems);
        m_depotItems.AddItems(item.id, numItems);
    }
    m_sellItems.Reset();
    m_playerListValue->ScrollToTop();
    UpdateLists();

    // verify credit balance & cargo space before buying
    int numstacks = m_buyItems.GetNumStacks();
    if (numstacks == 0)
        return;
    int buyValue = 0, neededSpace = 0;

    for (int i = 0; i < numstacks; i++) {
        m_buyItems.GetStack(i, item, numItems);
        buyValue += (int)(item.value * numItems);
        if (!item.IsArtifact())
            neededSpace += numItems;
    }

    if (buyValue > g_game->gameState->m_credits) {
        g_game->ShowMessageBoxWindow("", "You can't afford it!", 250, 150);
        return;
    }

    if (neededSpace > g_game->gameState->m_ship.getAvailableSpace()) {
        g_game->ShowMessageBoxWindow(
            "",
            "You don't have enough free space in your cargo hold!",
            300,
            300);
        return;
    }

    // ok, we have enough money and cargo space: we pay
    g_game->gameState->m_credits -= buyValue;

    // the transaction took place; clear the list
    for (int i = 0; i < numstacks; i++) {
        m_buyItems.GetStack(i, item, numItems);
        m_depotItems.RemoveItems(item.id, numItems);
        g_game->gameState->m_items.AddItems(item.id, numItems);
    }
    m_buyItems.Reset();
    m_depotListValue->ScrollToTop();
    UpdateLists();
}

void
ModuleTradeDepot::UpdateButtonStates() {
    m_sellbuyBtn->SetVisible(false);
    m_checkoutBtn->SetVisible(false);
    m_cancelBtn->SetVisible(true);
    m_clearBtn->SetVisible(false);

    if (m_playerList->GetSelectedIndex() >= 0) {
        m_sellbuyBtn->SetButtonText("Sell");
        m_sellbuyBtn->SetVisible(true);
    }

    if (m_depotList->GetSelectedIndex() >= 0) {
        m_sellbuyBtn->SetButtonText("Buy");
        m_sellbuyBtn->SetVisible(true);
    }

    if (m_sellList->getLines() > 0) {
        m_checkoutBtn->SetVisible(true);
        m_clearBtn->SetVisible(true);
    }

    if (m_buyList->getLines() > 0) {
        m_checkoutBtn->SetVisible(true);
        m_clearBtn->SetVisible(true);
    }

    m_filterAllBtn->SetHighlight(false);
    m_filterSpecialtyGoodBtn->SetHighlight(false);
    m_filterMineralBtn->SetHighlight(false);
    m_filterLifeformBtn->SetHighlight(false);
    m_filterTradeItemBtn->SetHighlight(false);

    switch (m_filterType) {
    case IT_ARTIFACT:
        break;
    case IT_MINERAL:
        m_filterMineralBtn->SetHighlight(true);
        break;
    case IT_LIFEFORM:
        m_filterLifeformBtn->SetHighlight(true);
        break;
    case IT_TRADEITEM:
        m_filterTradeItemBtn->SetHighlight(true);
        break;
    case IT_INVALID:
    default:
        m_filterAllBtn->SetHighlight(true);
        break;
    }
}

void
ModuleTradeDepot::UpdateLists() {
    // player items

    m_playerItemsFiltered.Reset();
    for (int i = 0; i < g_game->gameState->m_items.GetNumStacks(); i++) {
        Item item;
        int numItems;
        g_game->gameState->m_items.GetStack(i, item, numItems);

        if ((m_filterType == IT_INVALID) || (item.itemType == m_filterType)) {
            m_playerItemsFiltered.AddItems(item.id, numItems);
        }
    }

    m_playerListValue->Clear();
    m_playerListValue->setLines(m_playerItemsFiltered.GetNumStacks());
    for (int i = 0; i < m_playerItemsFiltered.GetNumStacks(); i++) {
        Item item;
        int numItems;
        m_playerItemsFiltered.GetStack(i, item, numItems);

        m_playerList->Write(item.name);
        {
            ostringstream str;
            str << numItems;
            m_playerListNumItems->Write(str.str());
        }
        {
            ostringstream str;
            str << item.value;
            m_playerListValue->Write(str.str());
        }
    }

    // depot items

    m_depotItemsFiltered.Reset();
    for (int i = 0; i < m_depotItems.GetNumStacks(); i++) {
        Item item;
        int numItems;
        m_depotItems.GetStack(i, item, numItems);

        if ((m_filterType == IT_INVALID) || (item.itemType == m_filterType)) {
            m_depotItemsFiltered.AddItems(item.id, numItems);
        }
    }

    m_depotListValue->Clear();
    m_depotListValue->setLines(m_depotItemsFiltered.GetNumStacks());
    for (int i = 0; i < m_depotItemsFiltered.GetNumStacks(); i++) {
        Item item;
        int numItems;
        m_depotItemsFiltered.GetStack(i, item, numItems);

        m_depotList->Write(item.name);
        {
            ostringstream str;
            str << numItems;
            m_depotListNumItems->Write(str.str());
        }
        {
            ostringstream str;
            str << item.value;
            m_depotListValue->Write(str.str());
        }
    }

    // sell items

    int sellValue = 0;
    m_sellListValue->Clear();
    m_sellListValue->setLines(m_sellItems.GetNumStacks());
    for (int i = 0; i < m_sellItems.GetNumStacks(); i++) {
        Item item;
        int numItems;
        m_sellItems.GetStack(i, item, numItems);
        sellValue += (int)(item.value * numItems);

        m_sellList->Write(item.name);
        {
            ostringstream str;
            str << numItems;
            m_sellListNumItems->Write(str.str());
        }
        {
            ostringstream str;
            str << item.value;
            m_sellListValue->Write(str.str());
        }
    }

    // buy items

    int buyValue = 0;
    m_buyListValue->Clear();
    m_buyListValue->setLines(m_buyItems.GetNumStacks());
    for (int i = 0; i < m_buyItems.GetNumStacks(); i++) {
        Item item;
        int numItems;
        m_buyItems.GetStack(i, item, numItems);
        buyValue += (int)(item.value * numItems);

        m_buyList->Write(item.name);
        {
            ostringstream str;
            str << numItems;
            m_buyListNumItems->Write(str.str());
        }
        {
            ostringstream str;
            str << item.value;
            m_buyListValue->Write(str.str());
        }
    }

    // update net total values
    m_sellTotal = 0;
    m_buyTotal = 0;
    if (sellValue > buyValue) {
        m_sellTotal = sellValue - buyValue;
    } else {
        m_buyTotal = buyValue - sellValue;
    }
}
