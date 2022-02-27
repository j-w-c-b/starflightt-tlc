#include "ModuleCaptainsLounge.h"
#include "AudioSystem.h"
#include "Button.h"
#include "Events.h"
#include "Game.h"
#include "GameState.h"
#include "ModeMgr.h"
#include "Util.h"
#include "captainslounge_resources.h"

#include <sstream>
#include <string>

using namespace std;
using namespace captainslounge;

#define TEXTHEIGHT_TITLES 60
#define TEXTHEIGHT_GAME_NAME 24
#define TEXTHEIGHT_GAME_PROFESSION 20
#define TEXTHEIGHT_BTN_TITLES 30
#define TEXTCOL al_map_rgb(0, 255, 255)

#define BACKBTN_X 52
#define BACKBTN_Y 698

#define BTN_BASE_Y 200
#define BTN_DELTA_Y 96
#
#define TEXT_BOX_X_LEFT 580
#define TEXT_BOX_WIDTH 440

#define BTN_NEWCAPTAIN_X 361
#define BTN_DELCAPTAIN_X 425
#define BTN_SELCAPTAIN_X 489
#define BTN_SAVECAPTAIN_X TEXT_BOX_X_LEFT + 20

#define GAME_BASE_Y 192
#define GAME_DELTA_Y 97
#define GAME_SLOT_X 36
#define GAME_SLOT_W 600
#define GAME_SLOT_H 50

#define GAMES_TITLE_X 64
#define GAMES_TITLE_Y 100
#define TEXTHEIGHT_GAMES_TITLE 50

#define CURGAME_TITLE_X BTN_SAVECAPTAIN_X + 37 + 20
#define CURGAME_TITLE_Y 182

#define YES_X 317
#define YES_Y 533
#define NO_X 620
#define NO_Y 533
#define TEXTHEIGHT_MODALPROMPT 40
#define MODALPROMPT_START_Y 230

#define MODALPROMPT_BG_X 69
#define MODALPROMPT_BG_Y 157
static const int MODALPROMPT_BG_W = 885;
static const int MODALPROMPT_BG_H = 454;
static const int NAME_W = 300;
static const int NAME_H = 28;
static const int PROF_W = 300;
static const int BTN_W = 40;
static const int BTN_H = 40;

static const int LAUNCHBTN_X = BACKBTN_X + 180;
static const int LAUNCHBTN_Y = BACKBTN_Y;

static const int DETAILS_X = 580;
static const int DETAILS_Y = 175;
static const int DETAILS_W = 420;
static const int DETAILS_H = 470;

ALLEGRO_DEBUG_CHANNEL("ModuleCaptainsLounge")

CaptainsLoungeSlot::CaptainsLoungeSlot(
    GameState::GameSaveSlot slot,
    EventType new_event,
    EventType del_event,
    EventType load_event,
    EventType save_event)
    : Module(
        GAME_SLOT_X,
        GAME_BASE_Y + GAME_DELTA_Y * static_cast<int>(slot),
        GAME_SLOT_W,
        GAME_SLOT_H),
      m_name_label(make_shared<Label>(
          "",
          GAME_SLOT_X,
          GAME_BASE_Y + GAME_DELTA_Y * static_cast<int>(slot),
          NAME_W,
          NAME_H,
          false,
          0,
          g_game->font24,
          TEXTCOL)),
      m_profession_label(make_shared<Label>(
          "",
          GAME_SLOT_X,
          GAME_BASE_Y + GAME_DELTA_Y * static_cast<int>(slot)
              + TEXTHEIGHT_GAME_NAME + 2,
          PROF_W,
          TEXTHEIGHT_GAME_PROFESSION,
          false,
          0,
          g_game->font18,
          TEXTCOL)),
      m_new_button(make_shared<Button>(
          BTN_NEWCAPTAIN_X,
          BTN_BASE_Y + BTN_DELTA_Y * static_cast<int>(slot),
          BTN_W,
          BTN_H,
          EVENT_NONE,
          new_event,
          images[I_CAPTAINSLOUNGE_PLUS],
          images[I_CAPTAINSLOUNGE_PLUS_MOUSEOVER],
          images[I_CAPTAINSLOUNGE_PLUS_DISABLED])),
      m_del_button(make_shared<Button>(
          BTN_DELCAPTAIN_X,
          BTN_BASE_Y + BTN_DELTA_Y * static_cast<int>(slot),
          BTN_W,
          BTN_H,
          EVENT_NONE,
          del_event,
          images[I_CAPTAINSLOUNGE_DEL],
          images[I_CAPTAINSLOUNGE_DEL_MOUSEOVER],
          images[I_CAPTAINSLOUNGE_DEL_DISABLED])),
      m_load_button(make_shared<Button>(
          BTN_SELCAPTAIN_X,
          BTN_BASE_Y + BTN_DELTA_Y * static_cast<int>(slot),
          BTN_W,
          BTN_H,
          EVENT_NONE,
          load_event,
          images[I_CAPTAINSLOUNGE_SEL],
          images[I_CAPTAINSLOUNGE_SEL_MOUSEOVER],
          images[I_CAPTAINSLOUNGE_SEL_DISABLED])),
      m_save_button(make_shared<Button>(
          BTN_SAVECAPTAIN_X,
          BTN_BASE_Y + BTN_DELTA_Y * static_cast<int>(slot),
          BTN_W,
          BTN_H,
          EVENT_NONE,
          save_event,
          images[I_CAPTAINSLOUNGE_SAVE],
          images[I_CAPTAINSLOUNGE_SAVE_MOUSEOVER])),
      m_game_state(nullptr), m_slot(slot) {
    add_child_module(m_name_label);
    add_child_module(m_profession_label);
    add_child_module(m_new_button);
    add_child_module(m_del_button);
    add_child_module(m_load_button);
    add_child_module(m_save_button);

    m_save_button->set_active(false);

    CaptainsLoungeSlot::on_init();
}

CaptainsLoungeSlot::~CaptainsLoungeSlot() {}

bool
CaptainsLoungeSlot::on_init() {
    auto gs = GameState::ReadGame(m_slot);

    if (gs) {
        m_name_label->set_text(gs->get_officer(OFFICER_CAPTAIN)->get_name());
        switch (gs->m_profession) {
        case PROFESSION_SCIENTIFIC:
            m_profession_label->set_text("Scientific");
            break;
        case PROFESSION_FREELANCE:
            m_profession_label->set_text("Freelance");
            break;
        case PROFESSION_MILITARY:
            m_profession_label->set_text("Military");
            break;
        default:
            m_profession_label->set_text("");
            break;
        }
        m_new_button->set_enabled(false);
        m_del_button->set_enabled(true);
        m_load_button->set_enabled(true);
        m_save_button->set_active(true);
    } else {
        m_name_label->set_text("");
        m_profession_label->set_text("");
        m_new_button->set_enabled(true);
        m_del_button->set_enabled(false);
        m_load_button->set_enabled(false);
        m_save_button->set_active(true);
    }
    m_game_state.reset(gs);

    m_save_button->set_active(g_game->gameState->m_captainSelected);

    return true;
}

bool
CaptainsLoungeSlot::on_event(ALLEGRO_EVENT *event) {
    EventType t = static_cast<EventType>(event->type);

    switch (t) {
    case EVENT_CAPTAINSLOUNGE_CONFIRM_SEL_YES:
        m_save_button->set_active(true);
        break;
    default:
        break;
    }
    return true;
}

bool
CaptainsLoungeSlot::on_close() {
    m_game_state.reset();

    return true;
}

string
CaptainsLoungeSlot::get_captain_name() const {
    return m_game_state->get_officer(OFFICER_CAPTAIN)->get_name();
}

CaptainsLoungeDetails::CaptainsLoungeDetails(const GameState *state)
    : Module(DETAILS_X, DETAILS_Y, DETAILS_W, DETAILS_H),
      m_name_label(new Label(
          "",
          CURGAME_TITLE_X,
          CURGAME_TITLE_Y,
          DETAILS_X + DETAILS_W - CURGAME_TITLE_X,
          al_get_font_line_height(g_game->font24.get()),
          false,
          0,
          g_game->font24,
          TEXTCOL)),
      m_profession_label(new Label(
          "",
          CURGAME_TITLE_X,
          CURGAME_TITLE_Y + TEXTHEIGHT_GAME_NAME + 2,
          DETAILS_X + DETAILS_W - CURGAME_TITLE_X,
          al_get_font_line_height(g_game->font18.get()),
          false,
          0,
          g_game->font18,
          TEXTCOL)),
      m_stardate_label(new Label(
          "",
          CURGAME_TITLE_X,
          CURGAME_TITLE_Y + 3 * (TEXTHEIGHT_GAME_NAME + 2),
          DETAILS_X + DETAILS_W - CURGAME_TITLE_X,
          al_get_font_line_height(g_game->font18.get()),
          false,
          0,
          g_game->font18,
          TEXTCOL)),
      m_credits_label(new Label(
          "",
          CURGAME_TITLE_X,
          CURGAME_TITLE_Y + 4 * (TEXTHEIGHT_GAME_NAME + 2),
          DETAILS_X + DETAILS_W - CURGAME_TITLE_X,
          al_get_font_line_height(g_game->font18.get()),
          false,
          0,
          g_game->font18,
          TEXTCOL)),
      m_location_label(new Label(
          "",
          CURGAME_TITLE_X,
          CURGAME_TITLE_Y + 5 * (TEXTHEIGHT_GAME_NAME + 2),
          DETAILS_X + DETAILS_W - CURGAME_TITLE_X,
          al_get_font_line_height(g_game->font18.get()),
          false,
          0,
          g_game->font18,
          TEXTCOL)),
      m_ship_label(new Label(
          "",
          CURGAME_TITLE_X,
          CURGAME_TITLE_Y + 7 * (TEXTHEIGHT_GAME_NAME + 2),
          DETAILS_X + DETAILS_W - CURGAME_TITLE_X,
          al_get_font_line_height(g_game->font20.get()),
          false,
          0,
          g_game->font20,
          TEXTCOL)),
      m_ship_left_column(new Label(
          "",
          CURGAME_TITLE_X,
          CURGAME_TITLE_Y + 8 * (TEXTHEIGHT_GAME_NAME + 2),
          DETAILS_X + DETAILS_W - CURGAME_TITLE_X,
          al_get_font_line_height(g_game->font18.get()) * 3,
          true,
          0,
          g_game->font18,
          TEXTCOL)),
      m_ship_right_column(new Label(
          "",
          CURGAME_TITLE_X + 150,
          CURGAME_TITLE_Y + 8 * (TEXTHEIGHT_GAME_NAME + 2),
          DETAILS_X + DETAILS_W - CURGAME_TITLE_X,
          al_get_font_line_height(g_game->font18.get()) * 3,
          true,
          0,
          g_game->font18,
          TEXTCOL)),
      m_officer_title_label(new Label(
          "",
          CURGAME_TITLE_X,
          CURGAME_TITLE_Y + 12 * (TEXTHEIGHT_GAME_NAME + 2),
          DETAILS_X + DETAILS_W - CURGAME_TITLE_X,
          al_get_font_line_height(g_game->font18.get()) * 6,
          true,
          0,
          g_game->font18,
          TEXTCOL)),
      m_officer_name_label(new Label(
          "",
          CURGAME_TITLE_X + 150,
          CURGAME_TITLE_Y + 12 * (TEXTHEIGHT_GAME_NAME + 2),
          DETAILS_X + DETAILS_W - CURGAME_TITLE_X,
          al_get_font_line_height(g_game->font18.get()) * 6,
          true,
          0,
          g_game->font18,
          TEXTCOL)) {
    add_child_module(m_name_label);
    add_child_module(m_profession_label);
    add_child_module(m_stardate_label);
    add_child_module(m_credits_label);
    add_child_module(m_location_label);

    add_child_module(m_ship_label);
    add_child_module(m_ship_left_column);
    add_child_module(m_ship_right_column);

    add_child_module(m_officer_title_label);
    add_child_module(m_officer_name_label);

    reset_labels(state);
}

bool
CaptainsLoungeDetails::on_init() {
    reset_labels(g_game->gameState);
    return true;
}

bool
CaptainsLoungeDetails::on_event(ALLEGRO_EVENT *event) {
    EventType t = static_cast<EventType>(event->type);

    switch (t) {
    case EVENT_CAPTAINSLOUNGE_CONFIRM_SEL_YES:
        reset_labels(g_game->gameState);
        break;
    default:
        break;
    }
    return true;
}

void
CaptainsLoungeDetails::reset_labels(const GameState *state) {
    if (state && state->m_captainSelected) {
        m_name_label->set_text(state->get_officer(OFFICER_CAPTAIN)->get_name());
        m_profession_label->set_text(
            "Profession: " + state->getProfessionString());

        m_stardate_label->set_text(
            "Stardate: " + state->stardate.GetFullDateString());
        m_credits_label->set_text(
            "Credits: " + to_string(g_game->gameState->m_credits));
        m_location_label->set_text(
            "Location: " + state->get_saved_module_name());

        m_ship_label->set_text("MSS " + state->m_ship.getName());

        m_ship_left_column->set_text(
            "Cargo Pods: " + to_string(state->m_ship.getCargoPodCount())
            + "\n"
              "Armor: "
            + state->m_ship.getArmorClassString()
            + "\n"
              "Lasers: "
            + state->m_ship.getLaserClassString());

        m_ship_right_column->set_text(
            "Engine: " + state->m_ship.getEngineClassString() + "\n"
            + "Shields: " + state->m_ship.getShieldClassString() + "\n"
            + "Missiles: " + state->m_ship.getMissileLauncherClassString());

        string titles = "";
        for (auto i : OfficerIterator) {
            titles += to_string(i) + ":\n";
        }
        m_officer_title_label->set_text(titles);

        string names = "";
        for (auto i : OfficerIterator) {
            auto o = g_game->gameState->get_officer(i);
            if (o) {
                names += o->get_last_name() + "\n";
            } else {
                names += "\n";
            }
        }
        m_officer_name_label->set_text(names);
    } else {
        m_name_label->set_text("");
        m_profession_label->set_text("");
        m_stardate_label->set_text("");
        m_credits_label->set_text("");
        m_location_label->set_text("");

        m_ship_label->set_text("");
        m_ship_left_column->set_text("");
        m_ship_right_column->set_text("");

        m_officer_title_label->set_text("");
        m_officer_name_label->set_text("");
    }
}

CaptainsLoungeConfirmationBox::CaptainsLoungeConfirmationBox(
    const string &text,
    GameState::GameSaveSlot slot,
    EventType yes_event,
    EventType no_event)
    : Module(
        MODALPROMPT_BG_X,
        MODALPROMPT_BG_Y,
        MODALPROMPT_BG_W,
        MODALPROMPT_BG_H),
      m_background(new Bitmap(
          images[I_CAPTAINSLOUNGE_MODALPROMPT_BACKGROUND],
          MODALPROMPT_BG_X,
          MODALPROMPT_BG_Y,
          MODALPROMPT_BG_W,
          MODALPROMPT_BG_H)),
      m_details(new Label(
          text,
          MODALPROMPT_BG_X + 55,
          MODALPROMPT_BG_Y + 120,
          770,
          256,
          true,
          ALLEGRO_ALIGN_CENTER,
          g_game->font32,
          TEXTCOL)),
      m_yes_button(new Button(
          YES_X,
          YES_Y,
          -1,
          -1,
          EVENT_NONE,
          yes_event,
          images[I_CAPTAINSLOUNGE_YES],
          images[I_CAPTAINSLOUNGE_YES_MOUSEOVER])),
      m_no_button(new Button(
          NO_X,
          NO_Y,
          -1,
          -1,
          EVENT_NONE,
          no_event,
          images[I_CAPTAINSLOUNGE_NO],
          images[I_CAPTAINSLOUNGE_NO_MOUSEOVER])),
      m_slot(slot) {
    add_child_module(m_background);
    add_child_module(m_details);
    add_child_module(m_yes_button);
    add_child_module(m_no_button);
}

CaptainsLoungeConfirmationBox::~CaptainsLoungeConfirmationBox() {}

ModuleCaptainsLounge::ModuleCaptainsLounge()
    : Module(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT),
      m_background(make_shared<Bitmap>(images[I_CAPTAINSLOUNGE_BACKGROUND])),
      m_back_button(make_shared<TextButton>(
          make_shared<Label>(
              "Exit",
              BACKBTN_X,
              BACKBTN_Y
                  + (64 - al_get_font_line_height(g_game->font32.get())) / 2,
              176,
              64,
              false,
              ALLEGRO_ALIGN_CENTER,
              g_game->font32,
              BLACK),
          BACKBTN_X,
          BACKBTN_Y,
          -1,
          -1,
          EVENT_NONE,
          EVENT_CAPTAINSLOUNGE_BACK,
          images[I_GENERIC_EXIT_BTN_NORM],
          images[I_GENERIC_EXIT_BTN_OVER])),
      m_launch_button(make_shared<TextButton>(
          make_shared<Label>(
              "Launch",
              LAUNCHBTN_X,
              LAUNCHBTN_Y
                  + (64 - al_get_font_line_height(g_game->font32.get())) / 2,
              176,
              64,
              false,
              ALLEGRO_ALIGN_CENTER,
              g_game->font32,
              BLACK),
          LAUNCHBTN_X,
          LAUNCHBTN_Y,
          -1,
          -1,
          EVENT_NONE,
          EVENT_CAPTAINSLOUNGE_LAUNCH,
          images[I_GENERIC_EXIT_BTN_NORM],
          images[I_GENERIC_EXIT_BTN_OVER])),
      m_captain_details(make_shared<CaptainsLoungeDetails>(g_game->gameState)),
      m_detail_ui_help(make_shared<Label>(
          "You may load an existing captain by "
          "clicking on the target (load) button beside a slot which "
          "contains an existing captain.\n\n"
          "Or, you may create a new captain by "
          "clicking on the + (new) button beside "
          "an empty slot.",
          TEXT_BOX_X_LEFT + 20,
          CURGAME_TITLE_Y + 20,
          TEXT_BOX_WIDTH - 40,
          DETAILS_H - 20,
          true,
          0,
          g_game->font24,
          TEXTCOL)),
      m_selected_slot(GameState::GAME_SAVE_SLOT_UNKNOWN), m_display_help(true) {
    add_child_module(m_background);
    add_child_module(m_back_button);
    add_child_module(m_launch_button);
    for (int i = 0; i < CAPTAINSLOUNGE_NUMSLOTS; i++) {
        m_captain_slots[i] = make_shared<CaptainsLoungeSlot>(
            static_cast<GameState::GameSaveSlot>(
                GameState::GAME_SAVE_SLOT0 + i),
            static_cast<EventType>(EVENT_CAPTAINSLOUNGE_NEW0 + i),
            static_cast<EventType>(EVENT_CAPTAINSLOUNGE_DEL0 + i),
            static_cast<EventType>(EVENT_CAPTAINSLOUNGE_SEL0 + i),
            static_cast<EventType>(EVENT_CAPTAINSLOUNGE_SAVE0 + i));
        if (!m_captain_slots[i]->is_empty()) {
            m_display_help = false;
        }

        add_child_module(m_captain_slots[i]);
    }
    add_child_module(m_captain_details);
    add_child_module(m_detail_ui_help);
    m_captain_details->set_active(false);
    m_detail_ui_help->set_active(true);
}

bool
ModuleCaptainsLounge::on_init() {
    g_game->SetTimePaused(true); // game-time frozen in this module.

    if (m_selected_slot != GameState::GAME_SAVE_SLOT_UNKNOWN
        && g_game->gameState->m_captainSelected) {
        m_captain_details->set_active(true);
        m_detail_ui_help->set_active(false);
        m_launch_button->set_enabled(true);
        g_game->gameState->SaveGame(m_selected_slot);
        m_back_button->set_enabled(false);
    } else if (g_game->gameState->m_captainSelected) {
        m_selected_slot = g_game->gameState->get_current_game_save_slot();
        m_captain_details->set_active(true);
        m_detail_ui_help->set_active(false);
        m_launch_button->set_enabled(true);
    } else {
        m_selected_slot = GameState::GAME_SAVE_SLOT_UNKNOWN;
        m_captain_details->set_active(false);
        m_detail_ui_help->set_active(true);
        m_launch_button->set_enabled(false);
    }
    return true;
}

bool
ModuleCaptainsLounge::on_event(ALLEGRO_EVENT *event) {
    bool playBtnClick = false;
    bool exitToStarportCommons = false;
    bool launchSavedModule = false;
    bool exitToCaptCreation = false;
    bool show_help = false;

    switch (event->type) {
    case EVENT_CAPTAINSLOUNGE_BACK:
        playBtnClick = true;
        exitToStarportCommons = true;
        break;
    case EVENT_CAPTAINSLOUNGE_LAUNCH:
        playBtnClick = true;
        launchSavedModule = true;
        break;
    case EVENT_CAPTAINSLOUNGE_NEW0:
    case EVENT_CAPTAINSLOUNGE_NEW1:
    case EVENT_CAPTAINSLOUNGE_NEW2:
    case EVENT_CAPTAINSLOUNGE_NEW3:
    case EVENT_CAPTAINSLOUNGE_NEW4:
        // NEW captain
        m_selected_slot = static_cast<GameState::GameSaveSlot>(
            event->type - EVENT_CAPTAINSLOUNGE_NEW0);
        g_game->gameState->setCaptainSelected(false);

        playBtnClick = true;
        exitToCaptCreation = true;
        break;
    case EVENT_CAPTAINSLOUNGE_DEL0:
    case EVENT_CAPTAINSLOUNGE_DEL1:
    case EVENT_CAPTAINSLOUNGE_DEL2:
    case EVENT_CAPTAINSLOUNGE_DEL3:
    case EVENT_CAPTAINSLOUNGE_DEL4:
        // DEL captain
        {
            playBtnClick = true;

            auto slot = static_cast<GameState::GameSaveSlot>(
                event->type - EVENT_CAPTAINSLOUNGE_DEL0);
            show_help =
                (slot == g_game->gameState->get_current_game_save_slot());

            if (!m_captain_slots[slot]->is_empty()) {
                auto captain_name = m_captain_slots[slot]->get_captain_name();

                // activate the confirmation modal prompt
                auto confirmation_box =
                    make_shared<CaptainsLoungeConfirmationBox>(
                        "Are you sure you want to delete " + captain_name + "?",
                        slot,
                        EVENT_CAPTAINSLOUNGE_CONFIRM_DEL_YES,
                        EVENT_CAPTAINSLOUNGE_CONFIRM_DEL_NO);

                set_modal_child(confirmation_box);
            } else {
                GameState::DeleteGame(slot);
                m_captain_slots[slot]->on_init();
            }
        }
        break;
    case EVENT_CAPTAINSLOUNGE_CONFIRM_DEL_YES:
        {
            auto modal_box =
                dynamic_pointer_cast<CaptainsLoungeConfirmationBox>(
                    set_modal_child(nullptr));
            auto slot = modal_box->get_slot();
            show_help =
                (slot == g_game->gameState->get_current_game_save_slot());

            GameState::DeleteGame(slot);
            m_captain_slots[slot]->on_init();
        }
        break;

    case EVENT_CAPTAINSLOUNGE_CONFIRM_DEL_NO:
        // user cancelled captain deletion request
        set_modal_child(nullptr);
        break;
    case EVENT_CAPTAINSLOUNGE_SEL0:
    case EVENT_CAPTAINSLOUNGE_SEL1:
    case EVENT_CAPTAINSLOUNGE_SEL2:
    case EVENT_CAPTAINSLOUNGE_SEL3:
    case EVENT_CAPTAINSLOUNGE_SEL4:
        // SEL captain
        {
            playBtnClick = true;
            auto slot = static_cast<GameState::GameSaveSlot>(
                event->type - EVENT_CAPTAINSLOUNGE_SEL0);

            m_captain_details->set_active(true);
            m_detail_ui_help->set_active(false);

            // activate the confirmation modal prompt if needed:
            // game state becomes dirty when we leave the captain's lounge.
            if (g_game->gameState->m_captainSelected
                && g_game->gameState->dirty == true) {
                // activate the confirmation modal prompt
                auto confirmation_box = make_shared<
                    CaptainsLoungeConfirmationBox>(
                    "Select this Captain?\n\nProgress in the current game will "
                    "be lost.",
                    slot,
                    EVENT_CAPTAINSLOUNGE_CONFIRM_SEL_YES,
                    EVENT_CAPTAINSLOUNGE_CONFIRM_SEL_NO);

                set_modal_child(confirmation_box);
            } else {
                ALLEGRO_EVENT e = {
                    .type = EVENT_CAPTAINSLOUNGE_CONFIRM_SEL_YES};
                g_game->broadcast_event(&e);
                m_selected_slot = slot;
            }
        }
        break;
    case EVENT_CAPTAINSLOUNGE_CONFIRM_SEL_YES:
        {
            GameState::GameSaveSlot slot = m_selected_slot;

            auto modal = dynamic_pointer_cast<CaptainsLoungeConfirmationBox>(
                set_modal_child(nullptr));

            if (modal) {
                slot = modal->get_slot();
            }
            GameState::LoadGame(slot);
            m_captain_slots[slot]->on_init();

            if (m_captain_slots[slot]->is_empty()) {
                g_game->message(
                    "CaptainsLounge: Error loading game save file.");
            } else {
                m_selected_slot = slot;
            }
        }
        break;
    case EVENT_CAPTAINSLOUNGE_SAVE0:
    case EVENT_CAPTAINSLOUNGE_SAVE1:
    case EVENT_CAPTAINSLOUNGE_SAVE2:
    case EVENT_CAPTAINSLOUNGE_SAVE3:
    case EVENT_CAPTAINSLOUNGE_SAVE4:
        {
            // SAVE captain
            playBtnClick = true;

            auto slot = static_cast<GameState::GameSaveSlot>(
                event->type - EVENT_CAPTAINSLOUNGE_SAVE0);

            if (m_captain_slots[slot]->is_empty()) {
                g_game->gameState->SaveGame(slot);
                m_captain_slots[slot]->on_init();
            } else {
                auto cap_name = m_captain_slots[slot]->get_captain_name();
                auto confirmation_box =
                    make_shared<CaptainsLoungeConfirmationBox>(
                        "Save this Captain?\n\n"
                        "Existing Captain "
                            + cap_name + " will be overwritten.",
                        slot,
                        EVENT_CAPTAINSLOUNGE_CONFIRM_SAVE_YES,
                        EVENT_CAPTAINSLOUNGE_CONFIRM_SAVE_NO);
                set_modal_child(confirmation_box);
            }
        }
        break;
    case EVENT_CAPTAINSLOUNGE_CONFIRM_SAVE_YES:
        {
            auto modal = dynamic_pointer_cast<CaptainsLoungeConfirmationBox>(
                set_modal_child(nullptr));
            ALLEGRO_ASSERT(modal != nullptr);
            auto slot = modal->get_slot();
            GameState::DeleteGame(slot);
            g_game->gameState->SaveGame(slot);
            m_captain_slots[slot]->on_init();
        }
        break;
    case EVENT_CAPTAINSLOUNGE_CONFIRM_SAVE_NO:
        // user cancelled captain save request
        set_modal_child(nullptr);
        break;
    default:
        break;
    }

    if (playBtnClick) {
        g_game->audioSystem->Play(samples[S_BUTTONCLICK]);
    }

    if (show_help) {
        m_captain_details->set_active(false);
        m_detail_ui_help->set_active(true);
    }

    if (launchSavedModule) {
        GameState::LoadGame(m_selected_slot);
        if (g_game->gameState->m_captainSelected == true) {
            g_game->gameState->dirty = true;
            if (g_game->gameState->getSavedModule() == MODULE_CAPTAINSLOUNGE)
                g_game->LoadModule(MODULE_STARPORT);
            else
                g_game->LoadModule(g_game->gameState->getSavedModule());
            m_selected_slot = GameState::GAME_SAVE_SLOT_UNKNOWN;
        }
    } else if (exitToStarportCommons) {
        if (m_selected_slot != GameState::GAME_SAVE_SLOT_UNKNOWN) {
            GameState::LoadGame(m_selected_slot);
        }
        if (g_game->gameState->m_captainSelected == true) {
            g_game->gameState->dirty = true;
            g_game->LoadModule(MODULE_STARPORT);
            m_selected_slot = GameState::GAME_SAVE_SLOT_UNKNOWN;
        } else {
            g_game->LoadModule(MODULE_TITLESCREEN);
            m_selected_slot = GameState::GAME_SAVE_SLOT_UNKNOWN;
        }

        return false;
    } else if (exitToCaptCreation) {
        g_game->LoadModule(MODULE_CAPTAINCREATION);
        return false;
    } else {
        m_launch_button->set_enabled(g_game->gameState->m_captainSelected);
    }
    return true;
}
// vi: ft=cpp
