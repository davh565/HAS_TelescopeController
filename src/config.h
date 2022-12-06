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
// #define DI_DEC_ENC_A    11
// #define DI_DEC_ENC_B    12
#define DO_LCD_E        11
#define DO_LCD_RS       12
// #define <LED>        13 // Arduino board LED.

// #define TXD3_DEBUG      14
// #define RXD3_DEBUG      15
// #define TXD2_R_PI       16
// #define RXD2_R_PI       17

#define DO_LCD_D4       18 // Can be used as an INT intterupt, or for serial comms.
#define DO_LCD_D5       19 // Can be used as an INT intterupt, or for serial comms.
#define DO_LCD_D6       20
#define DO_LCD_D7       21
// #define TXD1_DEBUG      18 // Can be used as an INT intterupt, or for serial comms.
// #define RXD1_DEBUG      19 // Can be used as an INT intterupt, or for serial comms.
// #define SDA_LCD         20
// #define SCL_LCD         21

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


////////////////////////////////////////////////////////////////////////////////
#define BEEP_TIME 125


#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978