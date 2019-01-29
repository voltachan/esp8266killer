#ifndef DisplayUI_h
#define DisplayUI_h

#include "language.h"
#include "A_config.h"
#include "Settings.h"
#include "Names.h"
#include "SSIDs.h"
#include "Scan.h"
#include "Attack.h"

#include <SimpleButton.h>

using namespace simplebutton;

extern Settings settings;
extern Names    names;
extern SSIDs    ssids;
extern Accesspoints accesspoints;
extern Stations     stations;
extern Scan     scan;
extern Attack   attack;
extern uint32_t currentTime;

extern String leftRight(String a, String b, int len);
extern String center(String a, int len);
extern String left(String a, int len);
extern String right(String a, int len);
extern String leftRight(String a, String b, int len);
extern String replaceUtf8(String str, String r);

const char D_INTRO_0[] PROGMEM = "";
const char D_INTRO_1[] PROGMEM = "ESP8266 Deauther";
const char D_INTRO_2[] PROGMEM = "by @Spacehuhn";
const char D_INTRO_3[] PROGMEM = "";

// fallback for the buttons
#ifndef BUTTON_UP
  #define BUTTON_UP 255
#endif // ifndef BUTTON_UP

#ifndef BUTTON_DOWN
  #define BUTTON_DOWN 255
#endif // ifndef BUTTON_DOWN

#ifndef BUTTON_A
  #define BUTTON_A 255
#endif // ifndef BUTTON_A

#ifndef BUTTON_B
  #define BUTTON_B 255
#endif // ifndef BUTTON_B

struct MenuNode {
    std::function<String()>getStr; // function used to create the displayed string
    std::function<void()>  click; // function that is executed when node is clicked
    std::function<void()>  hold;  // function that is executed when node is pressed for > 800ms
};

struct Menu {
    SimpleList<MenuNode>* list;
    Menu                * parentMenu;
    uint8_t               selected;
    std::function<void()> build; // function that is executed when button is clicked
};

class DisplayUI {
    public:
        enum DISPLAY_MODE { OFF = 0, BUTTON_TEST = 1, MENU = 2, LOADSCAN = 3, PACKETMONITOR = 4, INTRO = 5, CLOCK = 6 };

        uint8_t mode      = DISPLAY_MODE::MENU;
        bool highlightLED = false;

        Button* up   = NULL;
        Button* down = NULL;
        Button* a    = NULL;
        Button* b    = NULL;

        // ===== adjustable ===== //
        DEAUTHER_DISPLAY // see config.h

        const uint8_t maxLen           = 18;
        const uint8_t lineHeight       = 12;
        const uint8_t buttonDelay      = 250;
        const uint8_t drawInterval     = 100; // 100ms = 10 FPS
        const uint16_t scrollSpeed     = 500; // time interval in ms
        const uint16_t screenIntroTime = 2500;
        const uint16_t screenWidth = 128;
        const uint16_t sreenHeight = 64;

        void configInit();
        void configOn();
        void configOff();
        void updatePrefix();
        void updateSuffix();
        void drawString(int x, int y, String str);
        void drawString(int row, String str);
        void drawLine(int x1, int y1, int x2, int y2);
        // ====================== //

        DisplayUI();
        ~DisplayUI();

        void setup();
#ifdef HIGHLIGHT_LED
        void setupLED();
#endif // ifdef HIGHLIGHT_LED

        void update();
        void on();
        void off();

    private:
        int16_t selectedID     = 0; // i.e. access point ID to draw the apMenu
        uint8_t scrollCounter = 0; // for horizontal scrolling

        uint32_t scrollTime    = 0; // last time a character was moved
        uint32_t drawTime      = 0; // last time a frame was drawn
        uint32_t startTime     = 0; // when the screen was enabled
        uint32_t buttonTime    = 0; // last time a button was pressed

        bool enabled = false;       // display enabled
        bool tempOff = false;

        // selected attack modes
        bool beaconSelected = false;
        bool deauthSelected = false;
        bool probeSelected  = false;

        // menus
        Menu* currentMenu;

        Menu mainMenu;

        Menu scanMenu;
        Menu showMenu;
        Menu attackMenu;

        Menu apListMenu;
        Menu stationListMenu;
        Menu nameListMenu;
        Menu ssidListMenu;

        Menu apMenu;
        Menu stationMenu;
        Menu nameMenu;
        Menu ssidMenu;

        void setupButtons();

        String getChannel();

        // draw functions
        void draw();
        void drawButtonTest();
        void drawMenu();
        void drawLoadingScan();
        void drawPacketMonitor();
        void drawIntro();
        void clearMenu(Menu* menu);

        // menu functions
        void changeMenu(Menu* menu);
        void goBack();
        void createMenu(Menu* menu, Menu* parent, std::function<void()>build);

        void addMenuNode(Menu* menu, std::function<String()>getStr, std::function<void()>click, std::function<void()>hold);
        void addMenuNode(Menu* menu, std::function<String()>getStr, std::function<void()>click);
        void addMenuNode(Menu* menu, std::function<String()>getStr, Menu* next);
        void addMenuNode(Menu* menu, const char* ptr, std::function<void()>click);
        void addMenuNode(Menu* menu, const char* ptr, Menu* next);

        // fake clock
        void drawClock();
        void setTime(int h, int m, int s);

        int clockHour   = 6;
        int clockMinute = 0;
        int clockSecond = 0;

        uint32_t clockTime = 0;
};

#endif // ifndef DisplayUI_h
