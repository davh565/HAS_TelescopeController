#ifndef HAS_ENUMS
#define HAS_ENUMS



enum command{ 
    GET_RA,
    GET_DEC,
    SYNC_POSITION,
    SLEW_TO_TARGET,
    STOP_SLEW,
    SET_TARGET_RA,
    SET_TARGET_DEC,
    NO_CMD 
};

enum axis{RA, DECL};
enum {COORD1, COORD2};
enum {LHA};
enum {AZ, ALT};
enum {DEG_RA, DEG_DEC};

enum direction{
  FORWARD,
  REVERSE
};

#endif