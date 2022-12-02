////////////////////////////////////////////////////////////////////////////////
// PARAMETERS
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// PIN CONFIGURATION
////////////////////////////////////////////////////////////////////////////////

// #define <RXD0>       0
// #define <TXD0>       1
#define DI_DEC_LIM_LO   2
#define DI_DEC_LIM_HI   3
#define DO_DEC_STP_DIR  4
#define PWM_DEC_STP_PUL 5
#define PWM_RA_STP_PUL  6
#define DO_RA_STP_DIR   7

#define DI_RA_LIM_IDX   8
#define PWM_BZR         9
// #define <RESERVED>   10
// #define <TIMER1>     11 // If additional hardware Pulse Gen required, use this pin. Otherwise, use as PCINT for encoders.
#define DI_DEC_ENC_A    11
#define DI_DEC_ENC_B    12
// #define <LED>        13 // Arduino board LED.

#define TXD3_DEBUG      14
#define RXD3_DEBUG      15
#define TXD2_R_PI       16
#define RXD2_R_PI       17

// #define <INT3/TXD1>  18 // Can be used as an INT intterupt, or for serial comms.
// #define <INT2/RXD1>  19 // Can be used as an INT intterupt, or for serial comms.
#define SDA_LCD         20
#define SCL_LCD         21

// #define <TIMER5>     46 // If additional hardware Pulse Gen required, use this pin.
// #define <T5>         47 // Can be used as counter input for Timer5. See AtMega2560 datasheheet Chp. 17 for more info.
// #define <ICP5>       48 // Can be used to connect external clock source to Timer5. See AtMega2560 datasheheet Chp. 17 for more info.
// #define <ICP4>       49 // Can be used to connect external clock source to Timer4. See AtMega2560 datasheheet Chp. 17 for more info.

// #define <MISO>       50 // Can be used as SPI MISO.
// #define <MOSI>       51 // Can be used as SPI MOSI.
// #define <SCK>        52 // Can be used as SPI SCK.
// #define <SS>         53 // Can be used as SPI SS.

#define DI_MUX_SIG     A6
#define DO_MUX_ADDR_A  A7
#define DO_MUX_ADDR_B  A8
#define DO_MUX_ADDR_C  A9
#define AI_POT_SPEED   A10

#define DI_RA_ENC_A    A11
#define DI_RA_ENC_B    A12
// #define <PCINT21>   A13 // Can be used as a PCINT intterupt.
// #define <PCINT22>   A14 // Can be used as a PCINT intterupt.
// #define <PCINT23>   A15 // Can be used as a PCINT intterupt.




// TEMPORARY - OLD CONTROLLER
// #define DI_MODE 21
// #define DI_DEC_DIR 24
// #define DI_DEC_PUL 25
// #define DI_RA_DIR 22
// #define DI_RA_PUL 23
// #define DI_TRACK A9
// #define AI_POT A8