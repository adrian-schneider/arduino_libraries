#define LED_RED                  1
#define LED_GREEN                2
#define LED_YELLOW               3

#define DSKY_DEBUG
#define DSKY_CANCEL_IS_BACKSPACE
#define DSKY_NOUN_UNDEFINES_VERB
//#define DSKY_VERB_UNDEFINES_NOUN
#ifdef DSKY_DEBUG
  #define SERIAL_SPEED           115200
#else // DSKY_DEBUG
  #define SERIAL_SPEED           9600
  #define DSKY_USE_TRM425
#endif // DSKY_DEBUG

#ifdef DSKY_DEBUG
  #define DSKY_C_ENTER           'e'
  #define DSKY_C_CLEAR           'c'
  #define DSKY_C_VERB            'v'
  #define DSKY_C_NOUN            'n'
  #define DSKY_C_MINUS           '-'
#else // DSKY_DEBUG
  #define DSKY_C_ENTER           '#'
  #define DSKY_C_CLEAR           '*'
  #define DSKY_C_VERB            'B'
  #define DSKY_C_NOUN            'A'
  #define DSKY_C_MINUS           'C'
#endif // DSKY_DEBUG

// TRM425 position cursor: 020, pos+0100 (octal).
//             1       2
// pos 01234567012345670123 Input mask
// 100 NOUN 99  VERB 99
// 124 P1 +99999
// 150 P2 +99999
// 174 P3 +99999
#define DSKY_TRM425_FF           "\014"           // Clear screen
#define DSKY_TRM425_XY_NOUN      "\020\105"
#define DSKY_TRM425_XY_TNOUN     "\020\100NOUN"
#define DSKY_TRM425_XY_VERB      "\020\116"
#define DSKY_TRM425_XY_TVERB     "\020\111VERB"
#define DSKY_TRM425_XY_P1        "\020\127"
#define DSKY_TRM425_XY_TP1       "\020\124P1"
#define DSKY_TRM425_XY_P2        "\020\153"
#define DSKY_TRM425_XY_TP2       "\020\150P2"
#define DSKY_TRM425_XY_P3        "\020\177"
#define DSKY_TRM425_XY_TP3       "\020\174P3"

#define COMPUTE_LIMITS

#ifdef COMPUTE_LIMITS
  #define TYPE_MININT(_t)        ((_t)(1 << (sizeof(_t) * 8 - 1)))
  #define TYPE_MAXINT(_t)        ((_t)~TYPE_MININT(_t))

  #define MININT                 (TYPE_MININT(int16_t))
  #define MAXINT                 (TYPE_MAXINT(int16_t))
  #define MAXINTD10              ((int16_t)(MAXINT / 10))
#else // COMPUTE_LIMITS
  #define MININT                 (int16_t)-32768
  #define MAXINT                 (int16_t)32767
  #define MAXINTD10              (int16_t)3276
#endif // COMPUTE_LIMITS

#define UNDEFINED                MININT
#define MAXORDER                 (int16_t)10000

#define IO_LED_RED_PM            DDRD |= (1<<7)    // P7 red status LED
#define IO_LED_RED_ON            PORTD |= (1<<7)   // P7
#define IO_LED_RED_OFF           PORTD &= ~(1<<7)  // P7

#define IO_LED_GREEN_PM          DDRB |= (1<<0)    // P8 green status LED
#define IO_LED_GREEN_ON          PORTB |=  (1<<0)  // P8
#define IO_LED_GREEN_OFF         PORTB &= ~(1<<0)  // P8

