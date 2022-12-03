#ifndef HAS_UI
#define HAS_UI

enum Menu {
    REBOOT,
    
    TEST_BTNS,
    SERIAL_MON,
};
enum DisplayMode {
COORD,
SYNC_MSG,
MENU_MAIN,
DEBUG
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
        void show();
        void showMenu(String name, String options[MENU_SIZE] );
        void showMessage(String msg, uint8_t duration);
        void showAutoManState();
        void showTrackState();
        void showCoords(double ra, double dec);
        void showSyncHome();


        private:
        bool menuState;
        bool trackState;
        bool autoManState;
        bool syncState;
        bool homeState;
        int8_t menuIdx;
        void reboot();
        void testButtons(HandheldController& hhc);
        void serialDebug();
        void home();

        String double2RaStr(double raDegrees);
        String double2DecStr(double decDegrees);

    };



    
}
#endif //HAS_UI
