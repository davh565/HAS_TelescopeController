#ifndef HAS_UI
#define HAS_UI
#include "pos.h"

enum MenuMain {
    ITM_COORDS,
    ITM_SYNC_STATUS,
    ITM_DEBUG
};
enum MenuDebug {
    ITM_TEST_BTNS,
    ITM_HORZ_LIM,
    ITM_REBOOT
};

enum DisplayMode {
COORDS,
SYNC,
MENU_MAIN,
MENU_DEBUG,
BTN_TST
// DEBUG
};

namespace ui{

    class HandheldController{
        public:
        HandheldController(){;}
        void initButtons(uint8_t pinPot, uint8_t pinSig, uint8_t addrA, uint8_t addrB, uint8_t addrC);
        bool getBtnRaMinus(){return btnRaMinus;}
        bool getBtnRaPlus(){return btnRaPlus;}
        bool getBtnDecMinus(){return btnDecMinus;}
        bool getBtnDecPlus(){return btnDecPlus;}
        bool getBtnMenu(){return btnMenu;}
        bool getBtnAutoMan(){return btnAutoMan;}
        bool getBtnGoTo(){return btnGoTo;}
        bool getBtnTrack(){return btnTrack;}
        bool getBtnRaMinusRise(){return btnRaMinusEdgeR;}
        bool getBtnRaPlusRise(){return btnRaPlusEdgeR;}
        bool getBtnDecMinusRise(){return btnDecMinusEdgeR;}
        bool getBtnDecPlusRise(){return btnDecPlusEdgeR;}
        bool getBtnMenuRise(){return btnMenuEdgeR;}
        bool getBtnAutoManRise(){return btnAutoManEdgeR;}
        bool getBtnGoToRise(){return btnGoToEdgeR;}
        bool getBtnTrackRise(){return btnTrackEdgeR;}

        uint16_t getPotValue(){return potValue;}
        void updateButtons();


        private:
        void updateEdges();
        const uint8_t debounceMicros = 50;
        uint8_t pinPot;
        uint8_t pinSig;
        uint8_t addrA;
        uint8_t addrB;
        uint8_t addrC;
        uint16_t potValue;
        bool btnRaMinus;
        bool btnRaPlus;
        bool btnDecMinus;
        bool btnDecPlus;
        bool btnMenu;
        bool btnAutoMan;
        bool btnGoTo;
        bool btnTrack;
        bool btnRaMinusEdgeR;
        bool btnRaPlusEdgeR;
        bool btnDecMinusEdgeR;
        bool btnDecPlusEdgeR;
        bool btnMenuEdgeR;
        bool btnAutoManEdgeR;
        bool btnGoToEdgeR;
        bool btnTrackEdgeR;
    };

    class Display{
        #define MENU_SIZE 5
        public:
        Display(){;}
        void init();
        void updateStates(HandheldController hhc, bool sync, bool home);
        void show(HandheldController hhc, pos::Position pos);
        void showMenu();
        void showMenuDebug();
        void showMessage(String msg, uint8_t duration);
        void showAutoManState();
        void showTrackState();
        void showCoords(double ra, double dec);
        void showSyncHome();
        bool getTrackState(){return trackState;}
        bool getAutoManState(){return autoManState;}


        private:
        bool trackState;
        bool autoManState;
        bool syncState;
        bool homeState;
        int8_t menuIdx;
        int8_t prevMenuIdx;
        int8_t dbgMenuIdx;
        int8_t prevDbgMenuIdx;
        void reboot();
        void testButtons(HandheldController hhc);
        void serialDebug();
        void home();
        DisplayMode mode;
        DisplayMode prevMode;
        DisplayMode lastMode;
        // HandheldController ctrlr;
        String double2RaStr(double raDegrees);
        String double2DecStr(double decDegrees);

    };



    
}
#endif //HAS_UI
