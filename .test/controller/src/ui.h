#ifndef HAS_UI
#define HAS_UI

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
        uint16_t getPotValue(){return potValue;}
        void updateButtons();
        void updateStates(bool& autoMan, bool& track);


        private:
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
    };

    class Display{
        public:
        Display(){;}
        void updateAutoManState(bool autoMan);
        void updateTrackState(bool track);
        void updateCoords(double ra, double dec);
        void init();

        private:
        String double2RaStr(double raDegrees);
        String double2DecStr(double decDegrees);

    };



    
}
#endif //HAS_UI
