typedef enum Boolean {
    NO=0, YES=1, FALSE=0, TRUE=1
} boolean;

/* for getstring */
typedef enum Strtype {
    THAIENG, ENGLISH, ENGUPCASE, NUMBER, ONEORTWO
} strtype;

typedef enum Search_file_mode {
    AUTO_FIND, CUR_DIR, CW_DIR, NOT_FOUND = -1
} search_file_mode;

typedef enum Thai_level {
    TOPEST=3, UPPER=2, MIDDLE=0, BELOW=1
} thai_level;

typedef enum Font_attr {
    REVERSEATTR=0x02,
    ONELINEATTR=0x01,
    BOLDATTR=0x04,
    ENLARGEATTR=0x40,
    TWOLINEATTR=0x80,
    ITALICATTR=0x20,
    SUPERATTR=0x08,
    SUBATTR=0x10,
    NORMALATTR=0x00
} font_attr;

typedef enum Font_code {
    ONELINECODE=19,           /* ^S */
    BOLDCODE=2,               /* ^B */
    ENLARGECODE=5,            /* ^E */
    TWOLINECODE=18,           /* ^R */
    ITALICCODE=23,            /* ^W */
    SUPERCODE=20,             /* ^T */
    SUBCODE=22,               /* ^V */
    ITALIC=23,
    UNDERLINE=19,
    SUBSCRIPT=22,
    SUPER=20,
    ENLARGE=5,
    BOLD=2,
    DOUBLEUNDERLINE=18
} font_code;
