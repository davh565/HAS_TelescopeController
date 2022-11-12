#ifndef HAS_IO
#define HAS_IO

#include "pos.h"



namespace io{
    pos::Position getMotorPositions();
    void stopMotors();
}
#endif
