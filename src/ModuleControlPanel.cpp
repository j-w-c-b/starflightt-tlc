/*
        STARFLIGHT - THE LOST COLONY
        ModuleControlPanel.cpp
        Author: coder1024
        Date: April, 07
*/

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "AudioSystem.h"
#include "Events.h"
#include "Game.h"
#include "GameState.h"
#include "ModeMgr.h"
#include "ModuleControlPanel.h"
#include "controlpanel_resources.h"

using namespace std;
using namespace controlpanel_resources;

ALLEGRO_DEBUG_CHANNEL("ModuleControlPanel")

int CMDBUTTONS_UL_X;
int CMDBUTTONS_UL_Y;
int OFFICERICON_UL_X;
int OFFICERICON_UL_Y;

#define CMDBUTTON_SPACING 0

ModuleControlPanel::ModuleControlPanel()
    : Module(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT),
      resources(CONTROLPANEL_IMAGES) {
    controlPanelBackgroundImg = NULL;
    mouseOverOfficer = NULL;
    selectedOfficer = NULL;
    mouseOverCommand = NULL;
    selectedCommand = NULL;
    bEnabled = true;
}

ModuleControlPanel::~ModuleControlPanel(void) {}

bool
ModuleControlPanel::on_init() {
    // load the control panel datafile
    if (!resources.load()) {
        g_game->message("ControlPanel: Error loading datafile");
        return false;
    }

    static int bx = (int)g_game->getGlobalNumber("GUI_CONTROLPANEL_POS_X");
    static int by = (int)g_game->getGlobalNumber("GUI_CONTROLPANEL_POS_Y");
    CMDBUTTONS_UL_X = bx + 18;
    CMDBUTTONS_UL_Y = by + 242;
    OFFICERICON_UL_X = bx + 45;
    OFFICERICON_UL_Y = by + 157;

    // load background image
    controlPanelBackgroundImg = resources[I_GUI_CONTROLPANEL];

    const int officerIconWidth = 40;
    const int officerIconHeight = 40;
    int officerIconX = OFFICERICON_UL_X;
    int officerIconY = OFFICERICON_UL_Y;

    if (!CommandButton::InitCommon(*this))
        return false;

    if (!OfficerButton::InitCommon())
        return false;

    // reusable button object
    CommandButton *cbtn;

    /*
     * CAPTAIN
     */
    OfficerButton *captainBtn;
    captainBtn = new OfficerButton(
        *this,
        OFFICER_CAPTAIN,
        I_CP_CAPTAIN_MO,
        I_CP_CAPTAIN_SELECT,
        officerIconX,
        officerIconY);

    selectedOfficer = captainBtn;
    officerButtons.push_back(captainBtn);

    int cix = CMDBUTTONS_UL_X;
    int ciy = CMDBUTTONS_UL_Y;

    // LAUNCH BUTTON
    cbtn = new CommandButton(
        *this, I_COMMANDICON_CAPTAIN_LAUNCH, "Break orbit", cix, ciy);
    cbtn->setEventID(EVENT_CAPTAIN_LAUNCH);
    captainBtn->commandButtons.push_back(cbtn);

    // DESCEND BUTTON
    cix += CommandButton::GetCommonWidth() + CMDBUTTON_SPACING;
    cbtn = new CommandButton(
        *this, I_COMMANDICON_CAPTAIN_DESCEND, "Descend to surface", cix, ciy);
    cbtn->setEventID(EVENT_CAPTAIN_DESCEND);
    captainBtn->commandButtons.push_back(cbtn);

    // CARGO HOLD BUTTON
    cix += CommandButton::GetCommonWidth() + CMDBUTTON_SPACING;
    cbtn = new CommandButton(
        *this, I_COMMANDICON_CAPTAIN_CARGO, "Cargo hold", cix, ciy);
    cbtn->setEventID(EVENT_CAPTAIN_CARGO);
    captainBtn->commandButtons.push_back(cbtn);

    // QUESTLOG BUTTON
    cix = CMDBUTTONS_UL_X;
    ciy += CommandButton::GetCommonHeight() + CMDBUTTON_SPACING;
    cbtn = new CommandButton(
        *this, I_COMMANDICON_COM_QUESTLOG, "Quest log", cix, ciy);
    cbtn->setEventID(EVENT_CAPTAIN_QUESTLOG);
    captainBtn->commandButtons.push_back(cbtn);

    /*
     * SCIENCE OFFICER
     */
    officerIconX += officerIconWidth;
    OfficerButton *scienceBtn = new OfficerButton(
        *this,
        OFFICER_SCIENCE,
        I_CP_SCIENCE_MO,
        I_CP_SCIENCE_SELECT,
        officerIconX,
        officerIconY);
    officerButtons.push_back(scienceBtn);

    cix = CMDBUTTONS_UL_X;
    ciy = CMDBUTTONS_UL_Y;

    // SCAN BUTTON
    cbtn = new CommandButton(
        *this, I_COMMANDICON_SCIENCE_SCAN, "Sensor scan", cix, ciy);
    cbtn->setEventID(EVENT_SCIENCE_SCAN);
    scienceBtn->commandButtons.push_back(cbtn);

    // ANALYSIS BUTTON
    cix += CommandButton::GetCommonWidth() + CMDBUTTON_SPACING;
    cbtn = new CommandButton(
        *this, I_COMMANDICON_SCIENCE_ANALYSIS, "Sensor analysis", cix, ciy);
    cbtn->setEventID(EVENT_SCIENCE_ANALYSIS);
    scienceBtn->commandButtons.push_back(cbtn);

    /*
     * NAVIGATOR
     */
    officerIconX += officerIconWidth;
    OfficerButton *navBtn = new OfficerButton(
        *this,
        OFFICER_NAVIGATION,
        I_CP_NAVIGATION_MO,
        I_CP_NAVIGATION_SELECT,
        officerIconX,
        officerIconY);
    officerButtons.push_back(navBtn);

    cix = CMDBUTTONS_UL_X;
    ciy = CMDBUTTONS_UL_Y;

    // ORBIT BUTTON
    cbtn = new CommandButton(
        *this, I_COMMANDICON_NAV_ORBIT, "Orbit planet", cix, ciy);
    cbtn->setEventID(EVENT_NAVIGATOR_ORBIT);
    navBtn->commandButtons.push_back(cbtn);

    // STARPORT DOCK BUTTON
    cix += CommandButton::GetCommonWidth() + CMDBUTTON_SPACING;
    cbtn = new CommandButton(
        *this, I_COMMANDICON_NAV_DOCK, "Dock with Starport", cix, ciy);
    cbtn->setEventID(EVENT_NAVIGATOR_DOCK);
    navBtn->commandButtons.push_back(cbtn);

    // HYPERSPACE ENGINE BUTTON
    cix += CommandButton::GetCommonWidth() + CMDBUTTON_SPACING;
    cbtn = new CommandButton(
        *this, I_COMMANDICON_NAV_HYPERSPACE, "Hyperspace engine", cix, ciy);
    cbtn->setEventID(EVENT_NAVIGATOR_HYPERSPACE);
    navBtn->commandButtons.push_back(cbtn);

    // STARMAP BUTTON
    cix = CMDBUTTONS_UL_X;
    ciy += CommandButton::GetCommonHeight() + CMDBUTTON_SPACING;
    cbtn = new CommandButton(
        *this, I_COMMANDICON_NAV_STARMAP, "Starmap", cix, ciy);
    cbtn->setEventID(EVENT_NAVIGATOR_STARMAP);
    navBtn->commandButtons.push_back(cbtn);

    /*
     * TACTICAL
     */
    officerIconX += officerIconWidth;
    OfficerButton *tacBtn = new OfficerButton(
        *this,
        OFFICER_TACTICAL,
        I_CP_TACTICAL_MO,
        I_CP_TACTICAL_SELECT,
        officerIconX,
        officerIconY);
    officerButtons.push_back(tacBtn);

    cix = CMDBUTTONS_UL_X;
    ciy = CMDBUTTONS_UL_Y;

    // SHIELDS BUTTON
    cbtn = new CommandButton(
        *this, I_COMMANDICON_TAC_SHIELDS, "Raise/Lower Shields", cix, ciy);
    cbtn->setEventID(EVENT_TACTICAL_SHIELDS);
    tacBtn->commandButtons.push_back(cbtn);

    // WEAPONS BUTTON
    cix += CommandButton::GetCommonWidth() + CMDBUTTON_SPACING;
    cbtn = new CommandButton(
        *this, I_COMMANDICON_TAC_WEAPONS, "Arm/Disarm Weapons", cix, ciy);
    cbtn->setEventID(EVENT_TACTICAL_WEAPONS);
    tacBtn->commandButtons.push_back(cbtn);

    /*
     * ENGINEER
     */
    officerIconX = OFFICERICON_UL_X;
    officerIconY = OFFICERICON_UL_Y + officerIconHeight;
    OfficerButton *engBtn = new OfficerButton(
        *this,
        OFFICER_ENGINEER,
        I_CP_ENGINEER_MO,
        I_CP_ENGINEER_SELECT,
        officerIconX,
        officerIconY);
    officerButtons.push_back(engBtn);

    cix = CMDBUTTONS_UL_X;
    ciy = CMDBUTTONS_UL_Y;

    // REPAIR BUTTON
    cbtn = new CommandButton(
        *this, I_COMMANDICON_ENG_REPAIR, "Repair systems", cix, ciy);
    cbtn->setEventID(EVENT_ENGINEER_REPAIR);
    engBtn->commandButtons.push_back(cbtn);

    // INJECT FUEL BUTTON
    cix += CommandButton::GetCommonWidth() + CMDBUTTON_SPACING;
    cbtn = new CommandButton(
        *this, I_COMMANDICON_COM_RESPOND, "Inject fuel", cix, ciy);
    cbtn->setEventID(EVENT_ENGINEER_INJECT);
    engBtn->commandButtons.push_back(cbtn);

    /*
     * COMMUNICATIONS
     */
    officerIconX += officerIconWidth;
    OfficerButton *comBtn = new OfficerButton(
        *this,
        OFFICER_COMMUNICATION,
        I_CP_COMM_MO,
        I_CP_COMM_SELECT,
        officerIconX,
        officerIconY);
    officerButtons.push_back(comBtn);

    cix = CMDBUTTONS_UL_X;
    ciy = CMDBUTTONS_UL_Y;

    // HAIL BUTTON
    cbtn = new CommandButton(
        *this, I_COMMANDICON_COM_HAIL, "Hail or respond", cix, ciy);
    cbtn->setEventID(EVENT_COMM_HAIL);
    comBtn->commandButtons.push_back(cbtn);

    // QUESTION BUTTON
    cix += CommandButton::GetCommonWidth() + CMDBUTTON_SPACING;
    cbtn = new CommandButton(
        *this, I_COMMANDICON_COM_QUESTION, "Ask a question", cix, ciy);
    cbtn->setEventID(EVENT_COMM_QUESTION);
    comBtn->commandButtons.push_back(cbtn);

    // POSTURE BUTTON
    cix += CommandButton::GetCommonWidth() + CMDBUTTON_SPACING;
    cbtn = new CommandButton(
        *this, I_COMMANDICON_COM_POSTURE, "Change posture", cix, ciy);
    cbtn->setEventID(EVENT_COMM_POSTURE);
    comBtn->commandButtons.push_back(cbtn);

    // TERMINATE BUTTON
    cix = CMDBUTTONS_UL_X;
    ciy += CommandButton::GetCommonHeight() + CMDBUTTON_SPACING;
    cbtn = new CommandButton(
        *this, I_COMMANDICON_COM_TERMINATE, "End communication", cix, ciy);
    cbtn->setEventID(EVENT_COMM_TERMINATE);
    comBtn->commandButtons.push_back(cbtn);

    // DISTRESS BUTTON
    cix += CommandButton::GetCommonWidth() + CMDBUTTON_SPACING;
    cbtn = new CommandButton(
        *this, I_COMMANDICON_COM_DISTRESS, "Send distress signal", cix, ciy);
    cbtn->setEventID(EVENT_COMM_DISTRESS);
    comBtn->commandButtons.push_back(cbtn);

    /*
     * MEDICAL
     */
    officerIconX += officerIconWidth;
    OfficerButton *medBtn = new OfficerButton(
        *this,
        OFFICER_MEDICAL,
        I_CP_MEDICAL_MO,
        I_CP_MEDICAL_SELECT,
        officerIconX,
        officerIconY);
    officerButtons.push_back(medBtn);

    cix = CMDBUTTONS_UL_X;
    ciy = CMDBUTTONS_UL_Y;

    // EXAMINE BUTTON
    cbtn = new CommandButton(
        *this, I_COMMANDICON_MED_EXAMINE, "Examine crew", cix, ciy);
    cbtn->setEventID(EVENT_DOCTOR_EXAMINE);
    medBtn->commandButtons.push_back(cbtn);

    // TREAT BUTTON
    cix += CommandButton::GetCommonWidth() + CMDBUTTON_SPACING;
    cbtn = new CommandButton(
        *this, I_COMMANDICON_MED_TREAT, "Treat crew", cix, ciy);
    cbtn->setEventID(EVENT_DOCTOR_TREAT);
    medBtn->commandButtons.push_back(cbtn);

    // do something with the buttons
    for (vector<OfficerButton *>::iterator i = officerButtons.begin();
         i != officerButtons.end();
         ++i) {
        OfficerButton *officerButton = *i;

        if (officerButton == NULL)
            return false;

        if (!officerButton->InitButton())
            return false;
    }

    // load audio files
    sndOfficerSelected =
        g_game->audioSystem->Load("data/controlpanel/officer_selected.ogg");
    if (!sndOfficerSelected) {
        g_game->message("ControlPanel: Error loading officer_selected");
        return false;
    }

    sndOfficerCommandSelected = g_game->audioSystem->Load(
        "data/controlpanel/officer_command_selected.ogg");
    if (!sndOfficerCommandSelected) {
        g_game->message("ControlPanel: Error loading officer_command_selected");
        return false;
    }

    return true;
}

bool
ModuleControlPanel::on_update() {
    /**
     * Set gameState variable to keep track of currently selected officer
     * this is needed by the Status Window module, among other places.
     **/
    if (selectedOfficer != NULL)
        g_game->gameState->setCurrentSelectedOfficer(
            selectedOfficer->GetOfficerType());

    return true;
}

bool
ModuleControlPanel::on_draw(ALLEGRO_BITMAP *target) {
    if (g_game->gameState->getCurrentModule() == MODULE_ENCOUNTER
        && !g_game->doShowControls())
        return false;

    static int lastMode = 0;

    // set CP buttons when mode change takes place
    if (g_game->gameState->player->controlPanelMode != lastMode) {
        lastMode = g_game->gameState->player->controlPanelMode;
    }

    // render CP background with transparency
    static int gcpx = (int)g_game->getGlobalNumber("GUI_CONTROLPANEL_POS_X");
    static int gcpy = (int)g_game->getGlobalNumber("GUI_CONTROLPANEL_POS_Y");
    al_set_target_bitmap(target);

    if (controlPanelBackgroundImg)
        al_draw_bitmap(controlPanelBackgroundImg, gcpx, gcpy, 0);

    // render command buttons for the selected officer
    if (selectedOfficer != NULL) {
        for (vector<CommandButton *>::iterator i =
                 selectedOfficer->commandButtons.begin();
             i != selectedOfficer->commandButtons.end();
             ++i) {
            CommandButton *commandButton = *i;

            if (commandButton->GetEnabled()) {
                if (selectedCommand == commandButton) {
                    commandButton->RenderSelected(target);
                } else if (mouseOverCommand == commandButton) {
                    commandButton->RenderMouseOver(target);
                } else {
                    commandButton->RenderPlain(target);
                }
            } else {
                commandButton->RenderDisabled(target);
            }
        }
    }

    // render officer buttons
    for (vector<OfficerButton *>::iterator i = officerButtons.begin();
         i != officerButtons.end();
         ++i) {
        OfficerButton *officerButton = *i;

        if (officerButton == selectedOfficer) {
            officerButton->RenderSelected(target);
        } else if (officerButton == mouseOverOfficer) {
            officerButton->RenderMouseOver(target);
        } else {
            if (officerButton->imgMouseOver)
                al_draw_bitmap(
                    officerButton->imgMouseOver,
                    officerButton->posX,
                    officerButton->posY,
                    0);
        }
    }
    return true;
}

bool
ModuleControlPanel::on_close() {
    for (vector<OfficerButton *>::iterator i = officerButtons.begin();
         i != officerButtons.end();
         ++i) {
        if (*i != NULL) {
            (*i)->DestroyButton();
            delete *i;
        }
    }
    officerButtons.clear();

    CommandButton::DestroyCommon();
    OfficerButton::DestroyCommon();

    if (sndOfficerSelected != NULL) {
        sndOfficerSelected = NULL;
    }

    if (sndOfficerCommandSelected != NULL) {
        sndOfficerCommandSelected = NULL;
    }

    selectedOfficer = NULL;

    // unload the data file (thus freeing all resources at once)
    resources.unload();

    return true;
}

#pragma region INPUT

bool
ModuleControlPanel::on_key_pressed(ALLEGRO_KEYBOARD_EVENT *event) {
    switch (event->keycode) {
    case ALLEGRO_KEY_F1: // select the captain
        g_game->gameState->setCurrentSelectedOfficer(OFFICER_CAPTAIN);
        for (vector<OfficerButton *>::iterator i = officerButtons.begin();
             i != officerButtons.end();
             ++i) {
            OfficerButton *officerButton = *i;
            if (officerButton->GetOfficerType() == OFFICER_CAPTAIN) {
                // change the officer
                selectedOfficer = officerButton;
                break;
            }
        }
        break;
    case ALLEGRO_KEY_F2: // select the science officer
        g_game->gameState->setCurrentSelectedOfficer(OFFICER_SCIENCE);
        for (vector<OfficerButton *>::iterator i = officerButtons.begin();
             i != officerButtons.end();
             ++i) {
            OfficerButton *officerButton = *i;
            if (officerButton->GetOfficerType() == OFFICER_SCIENCE) {
                // change the officer
                selectedOfficer = officerButton;
                break;
            }
        }
        break;
    case ALLEGRO_KEY_F3: // select the navigator
        g_game->gameState->setCurrentSelectedOfficer(OFFICER_NAVIGATION);
        for (vector<OfficerButton *>::iterator i = officerButtons.begin();
             i != officerButtons.end();
             ++i) {
            OfficerButton *officerButton = *i;
            if (officerButton->GetOfficerType() == OFFICER_NAVIGATION) {
                // change the officer
                selectedOfficer = officerButton;
                break;
            }
        }
        break;
    case ALLEGRO_KEY_F4: // select the tactician
        g_game->gameState->setCurrentSelectedOfficer(OFFICER_TACTICAL);
        for (vector<OfficerButton *>::iterator i = officerButtons.begin();
             i != officerButtons.end();
             ++i) {
            OfficerButton *officerButton = *i;
            if (officerButton->GetOfficerType() == OFFICER_TACTICAL) {
                // change the officer
                selectedOfficer = officerButton;
                break;
            }
        }
        break;
    case ALLEGRO_KEY_F5: // select the engineer
        g_game->gameState->setCurrentSelectedOfficer(OFFICER_ENGINEER);
        for (vector<OfficerButton *>::iterator i = officerButtons.begin();
             i != officerButtons.end();
             ++i) {
            OfficerButton *officerButton = *i;
            if (officerButton->GetOfficerType() == OFFICER_ENGINEER) {
                // change the officer
                selectedOfficer = officerButton;
                break;
            }
        }
        break;
    case ALLEGRO_KEY_F6: // select the comms officer
        g_game->gameState->setCurrentSelectedOfficer(OFFICER_COMMUNICATION);
        for (vector<OfficerButton *>::iterator i = officerButtons.begin();
             i != officerButtons.end();
             ++i) {
            OfficerButton *officerButton = *i;
            if (officerButton->GetOfficerType() == OFFICER_COMMUNICATION) {
                // change the officer
                selectedOfficer = officerButton;
                break;
            }
        }
        break;
    case ALLEGRO_KEY_F7: // select the doctor
        g_game->gameState->setCurrentSelectedOfficer(OFFICER_MEDICAL);
        for (vector<OfficerButton *>::iterator i = officerButtons.begin();
             i != officerButtons.end();
             ++i) {
            OfficerButton *officerButton = *i;
            if (officerButton->GetOfficerType() == OFFICER_MEDICAL) {
                // change the officer
                selectedOfficer = officerButton;
                break;
            }
        }
        break;

    case ALLEGRO_KEY_M: // "map" button
        g_game->gameState->setCurrentSelectedOfficer(OFFICER_NAVIGATION);
        for (vector<OfficerButton *>::iterator i = officerButtons.begin();
             i != officerButtons.end();
             ++i) {
            OfficerButton *officerButton = *i;
            if (officerButton->GetOfficerType() == OFFICER_NAVIGATION) {
                // change the officer
                selectedOfficer = officerButton;
                break;
            }
        }
        ALLEGRO_EVENT e = {
            .type = static_cast<unsigned int>(EVENT_NAVIGATOR_STARMAP)};
        g_game->broadcast_event(&e);
        break;
    }
    return true;
}

bool
ModuleControlPanel::on_mouse_move(ALLEGRO_MOUSE_EVENT *event) {
    int x = event->x;
    int y = event->y;

    // look for officer button mouse over
    mouseOverOfficer = NULL;
    for (vector<OfficerButton *>::iterator i = officerButtons.begin();
         (i != officerButtons.end()) && (mouseOverOfficer == NULL);
         ++i) {
        OfficerButton *officerButton = *i;

        if (officerButton->IsInButton(x, y)) {
            mouseOverOfficer = officerButton;
        }
    }

    // look for command button mouse over
    mouseOverCommand = NULL;
    if (selectedOfficer != NULL) {
        for (vector<CommandButton *>::iterator i =
                 selectedOfficer->commandButtons.begin();
             i != selectedOfficer->commandButtons.end();
             ++i) {
            CommandButton *commandButton = *i;

            if (commandButton->IsInButton(x, y)) {
                mouseOverCommand = commandButton;
            }
        }
    }
    return true;
}

bool
ModuleControlPanel::on_mouse_button_click(ALLEGRO_MOUSE_EVENT *event) {
    int button = event->button - 1;
    int x = event->x;
    int y = event->y;
    bool capture_press = false;

    if (button != 0) {
        return true;
    }

    // select officer
    for (auto &i : officerButtons) {
        OfficerButton *officerButton = i;

        if (officerButton->IsInButton(x, y)) {
            // change the officer
            if (selectedOfficer != officerButton
                && selectedCommand != nullptr) {
                ALLEGRO_EVENT e = {
                    .type = static_cast<unsigned int>(
                        selectedCommand->getEventID())};
                g_game->broadcast_event(&e);
                selectedCommand = nullptr;
            }
            selectedOfficer = officerButton;

            // play sound
            g_game->audioSystem->Play(sndOfficerSelected);

            capture_press = true;
            break;
        }
    }
    if (selectedOfficer != nullptr) {
        for (auto &i : selectedOfficer->commandButtons) {
            CommandButton *commandButton = i;

            if (commandButton->IsInButton(x, y)
                && commandButton->GetEnabled()) {
                if (selectedCommand != commandButton
                    && selectedCommand != nullptr) {
                    ALLEGRO_EVENT e = {
                        .type = static_cast<unsigned int>(
                            selectedCommand->getEventID())};
                    g_game->broadcast_event(&e);
                }
                selectedCommand = commandButton;

                g_game->audioSystem->Play(sndOfficerCommandSelected);
                capture_press = true;
            }
        }
    }

    // launch event based on button ID so all modules in this mode will be
    // notified
    if (selectedCommand && capture_press) {
        ALLEGRO_EVENT e = {
            .type = static_cast<unsigned int>(selectedCommand->getEventID())};
        g_game->broadcast_event(&e);
    }

    return !capture_press;
}

#pragma endregion

#pragma region COMMANDBUTTON
//******************************************************************************
// CommandButton
//******************************************************************************

#define CMDBUTTON_LABEL_CLR al_map_rgb(0, 0, 0)

ALLEGRO_BITMAP *ModuleControlPanel::CommandButton::imgBackground = NULL;
ALLEGRO_BITMAP *ModuleControlPanel::CommandButton::imgBackgroundDisabled = NULL;
ALLEGRO_BITMAP *ModuleControlPanel::CommandButton::imgBackgroundMouseOver =
    NULL;
ALLEGRO_BITMAP *ModuleControlPanel::CommandButton::imgBackgroundSelected = NULL;

ModuleControlPanel::CommandButton::CommandButton(
    ModuleControlPanel &outer,
    const std::string &datFileCmdIcon,
    const std::string &cmdName,
    int posX,
    int posY)
    : outer(outer) {
    this->datFileCmdIcon = datFileCmdIcon;
    this->cmdName = cmdName;
    this->posX = posX;
    this->posY = posY;
    this->imgCmdIcon = NULL;
    this->enabled = true;
}

ModuleControlPanel::CommandButton::~CommandButton() {}

bool
ModuleControlPanel::CommandButton::InitCommon(ModuleControlPanel &outer) {
    imgBackground = outer.resources[I_COMMAND_BUTTON_BG];
    imgBackgroundDisabled = outer.resources[I_COMMAND_BUTTON_BG_DISABLED];
    imgBackgroundMouseOver = outer.resources[I_COMMAND_BUTTON_BG_MO];
    imgBackgroundSelected = outer.resources[I_COMMAND_BUTTON_BG_SELECT];

    return true;
}

int
ModuleControlPanel::CommandButton::GetCommonWidth() {
    return al_get_bitmap_width(imgBackground);
}

int
ModuleControlPanel::CommandButton::GetCommonHeight() {
    return al_get_bitmap_height(imgBackground);
}

bool
ModuleControlPanel::CommandButton::InitButton() {
    imgCmdIcon = outer.resources[datFileCmdIcon];

    return true;
}

void
ModuleControlPanel::CommandButton::DestroyButton() {

    if (imgCmdIcon != NULL) {
        imgCmdIcon = NULL;
    }
}

void
ModuleControlPanel::CommandButton::DestroyCommon() {
    // now handled by the datafile

    /*if (imgBackground != NULL)
    {
            al_destroy_bitmap(imgBackground);
            imgBackground = NULL;
    }

    if (imgBackgroundDisabled != NULL)
    {
            al_destroy_bitmap(imgBackgroundDisabled);
            imgBackgroundDisabled = NULL;
    }

    if (imgBackgroundMouseOver != NULL)
    {
            al_destroy_bitmap(imgBackgroundMouseOver);
            imgBackgroundMouseOver = NULL;
    }

    if (imgBackgroundSelected != NULL)
    {
            al_destroy_bitmap(imgBackgroundSelected);
            imgBackgroundSelected = NULL;
    }*/
}

void
ModuleControlPanel::CommandButton::RenderPlain(ALLEGRO_BITMAP *canvas) {
    Render(canvas, imgBackground);
}

void
ModuleControlPanel::CommandButton::RenderDisabled(ALLEGRO_BITMAP *canvas) {
    Render(canvas, imgBackgroundDisabled);
}

void
ModuleControlPanel::CommandButton::RenderMouseOver(ALLEGRO_BITMAP *canvas) {
    Render(canvas, imgBackgroundMouseOver);

    static int x = 40 + (int)g_game->getGlobalNumber("GUI_CONTROLPANEL_POS_X");
    static int y = 115 + (int)g_game->getGlobalNumber("GUI_CONTROLPANEL_POS_Y");
    g_game->Print18(canvas, x + 5, y, cmdName.c_str(), WHITE);
}

void
ModuleControlPanel::CommandButton::RenderSelected(ALLEGRO_BITMAP *canvas) {
    Render(canvas, imgBackgroundSelected, true);
}

bool
ModuleControlPanel::CommandButton::IsInButton(int x, int y) {
    if ((x >= posX) && (x < posX + al_get_bitmap_width(imgBackground))
        && (y >= posY) && (y < posY + al_get_bitmap_height(imgBackground))) {
        return true;
    }

    return false;
}

void
ModuleControlPanel::CommandButton::SetEnabled(bool enabled) {
    this->enabled = enabled;
}

bool
ModuleControlPanel::CommandButton::GetEnabled() {
    return enabled;
}

void
ModuleControlPanel::CommandButton::Render(
    ALLEGRO_BITMAP *canvas,
    ALLEGRO_BITMAP *imgBackground,
    bool down) {
    al_set_target_bitmap(canvas);
    // draw button background and command icon image
    al_draw_bitmap(imgBackground, posX, posY, 0);

    if (down)
        al_draw_bitmap(imgCmdIcon, posX, posY, 0);
    else
        al_draw_bitmap_region(
            imgCmdIcon,
            0,
            2,
            al_get_bitmap_width(imgCmdIcon),
            al_get_bitmap_height(imgCmdIcon),
            posX,
            posY,
            0);
}

#pragma endregion

#pragma region OFFICERBUTTON

//******************************************************************************
// OfficerButton
//******************************************************************************

#define OFFICER_MOUSEOVERTIP_SPACEFROMBTN_X 3
#define OFFICER_MOUSEOVERTIP_SPACEFROMBTN_Y 0
#define OFFICER_MOUSEOVERTIP_BORDER_THICKNESS 2
#define OFFICER_MOUSEOVERTIP_BORDER_CLR al_map_rgb(0, 0, 0)
#define OFFICER_MOUSEOVERTIP_BACKGROUND_CLR al_map_rgb(200, 200, 200)
#define OFFICER_MOUSEOVERTIP_TEXT_CLR al_map_rgb(255, 255, 0)
#define OFFICER_MOUSEOVERTIP_INNER_SPACING 5
#define OFFICER_MOUSEOVERTIP_BAR_HEIGHT 10
#define OFFICER_MOUSEOVERTIP_HEALTH_CLR al_map_rgb(255, 0, 0)
#define OFFICER_MOUSEOVERTIP_LABEL_CLR al_map_rgb(255, 255, 255)
#define OFFICER_MOUSEOVERTIP_TEXTOFFSET_X 6
#define OFFICER_MOUSEOVERTIP_TEXTOFFSET_Y 6

ModuleControlPanel::OfficerButton::OfficerButton(
    ModuleControlPanel &outer,
    OfficerType officerType,
    const string &datFileMouseOver,
    const string &datFileSelected,
    int posX,
    int posY)
    : outer(outer) {
    this->officerType = officerType;
    this->datFileMouseOver = datFileMouseOver;
    this->datFileSelected = datFileSelected;
    this->posX = posX;
    this->posY = posY;
    this->imgMouseOver = NULL;
    this->imgSelected = NULL;
}

ModuleControlPanel::OfficerButton::~OfficerButton() {}

bool
ModuleControlPanel::OfficerButton::InitCommon() {
    return true;
}

bool
ModuleControlPanel::OfficerButton::InitButton() {
    imgMouseOver = outer.resources[datFileMouseOver];
    imgSelected = outer.resources[datFileSelected];

    for (vector<CommandButton *>::iterator i = commandButtons.begin();
         i != commandButtons.end();
         ++i) {
        CommandButton *commandButton = *i;

        if (commandButton == NULL)
            return false;

        if (!commandButton->InitButton())
            return false;
    }

    return true;
}

void
ModuleControlPanel::OfficerButton::RenderMouseOver(ALLEGRO_BITMAP *canvas) {
    std::string name;
    Officer *officer;

    // mouse-over button image
    al_set_target_bitmap(canvas);
    al_draw_bitmap(imgMouseOver, posX, posY, 0);

    // get the officer associated with this button
    officer = g_game->gameState->getOfficer(this->officerType);
    name = officer->name;

    // determine location for tip window
    static int cpx = (int)g_game->getGlobalNumber("GUI_CONTROLPANEL_POS_X");
    static int cpy = (int)g_game->getGlobalNumber("GUI_CONTROLPANEL_POS_Y");
    static int x = cpx + 40;
    static int y = cpy + 115;

    // background image
    al_draw_filled_rectangle(x, y, x + 165, y + 32, al_map_rgb(57, 59, 134));

    // draw tooltip of crew position/name
    g_game->Print18(
        canvas,
        x + 5,
        y,
        officer->GetTitle().c_str(),
        OFFICER_MOUSEOVERTIP_TEXT_CLR);
    g_game->Print18(
        canvas, x + 5, y + 13, name.c_str(), OFFICER_MOUSEOVERTIP_LABEL_CLR);
}

void
ModuleControlPanel::OfficerButton::RenderSelected(ALLEGRO_BITMAP *canvas) {
    al_set_target_bitmap(canvas);
    al_draw_bitmap(imgSelected, posX, posY, 0);
}

bool
ModuleControlPanel::OfficerButton::IsInButton(int x, int y) {
    if ((x >= posX) && (x < posX + al_get_bitmap_width(imgMouseOver))
        && (y >= posY) && (y < posY + al_get_bitmap_height(imgMouseOver))) {
        return true;
    }

    return false;
}

void
ModuleControlPanel::OfficerButton::DestroyButton() {
    for (vector<CommandButton *>::iterator i = commandButtons.begin();
         i != commandButtons.end();
         ++i) {
        delete *i;
    }
    commandButtons.clear();
}

void
ModuleControlPanel::OfficerButton::DestroyCommon() {}

#pragma endregion
