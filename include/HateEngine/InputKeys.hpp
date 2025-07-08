#pragma once

namespace HateEngine {
    enum Key {
        KeyUnknown = 0,

        KeyA = 4,
        KeyB = 5,
        KeyC = 6,
        KeyD = 7,
        KeyE = 8,
        KeyF = 9,
        KeyG = 10,
        KeyH = 11,
        KeyI = 12,
        KeyJ = 13,
        KeyK = 14,
        KeyL = 15,
        KeyM = 16,
        KeyN = 17,
        KeyO = 18,
        KeyP = 19,
        KeyQ = 20,
        KeyR = 21,
        KeyS = 22,
        KeyT = 23,
        KeyU = 24,
        KeyV = 25,
        KeyW = 26,
        KeyX = 27,
        KeyY = 28,
        KeyZ = 29,

        Key1 = 30,
        Key2 = 31,
        Key3 = 32,
        Key4 = 33,
        Key5 = 34,
        Key6 = 35,
        Key7 = 36,
        Key8 = 37,
        Key9 = 38,
        Key0 = 39,

        KeyReturn = 40,
        KeyEscape = 41,
        KeyBackspace = 42,
        KeyTab = 43,
        KeySpace = 44,

        KeyMinus = 45,
        KeyEquals = 46,
        KeyLeftBracket = 47,
        KeyRightBracket = 48,
        KeyBackslash = 49, /**< Located at the lower left of the return
                            *   key on ISO keyboards and at the right end
                            *   of the QWERTY row on ANSI keyboards.
                            *   Produces REVERSE SOLIDUS (backslash) and
                            *   VERTICAL LINE in a US layout, REVERSE
                            *   SOLIDUS and VERTICAL LINE in a UK Mac
                            *   layout, NUMBER SIGN and TILDE in a UK
                            *   Windows layout, DOLLAR SIGN and POUND SIGN
                            *   in a Swiss German layout, NUMBER SIGN and
                            *   APOSTROPHE in a German layout, GRAVE
                            *   ACCENT and POUND SIGN in a French Mac
                            *   layout, and ASTERISK and MICRO SIGN in a
                            *   French Windows layout.
                            */
        KeyNonUSHash = 50, /**< ISO USB keyboards actually use this code
                            *   instead of 49 for the same key, but all
                            *   OSes I've seen treat the two codes
                            *   identically. So, as an implementor, unless
                            *   your keyboard generates both of those
                            *   codes and your OS treats them differently,
                            *   you should generate KeyBackslash
                            *   instead of this code. As a user, you
                            *   should not rely on this code because SDL
                            *   will never generate it with most (all?)
                            *   keyboards.
                            */
        KeySemicolon = 51,
        KeyApostrophe = 52,
        KeyGrave = 53, /**< Located in the top left corner (on both ANSI
                        *   and ISO keyboards). Produces GRAVE ACCENT and
                        *   TILDE in a US Windows layout and in US and UK
                        *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                        *   and NOT SIGN in a UK Windows layout, SECTION
                        *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                        *   layouts on ISO keyboards, SECTION SIGN and
                        *   DEGREE SIGN in a Swiss German layout (Mac:
                        *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                        *   DEGREE SIGN in a German layout (Mac: only on
                        *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                        *   French Windows layout, COMMERCIAL AT and
                        *   NUMBER SIGN in a French Mac layout on ISO
                        *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                        *   SIGN in a Swiss German, German, or French Mac
                        *   layout on ANSI keyboards.
                        */
        KeyComma = 54,
        KeyPeriod = 55,
        KeySlash = 56,

        KeyCapsLock = 57,

        KeyF1 = 58,
        KeyF2 = 59,
        KeyF3 = 60,
        KeyF4 = 61,
        KeyF5 = 62,
        KeyF6 = 63,
        KeyF7 = 64,
        KeyF8 = 65,
        KeyF9 = 66,
        KeyF10 = 67,
        KeyF11 = 68,
        KeyF12 = 69,

        KeyPrintScreen = 70,
        KeyScrollLock = 71,
        KeyPause = 72,
        KeyInsert = 73, /**< insert on PC, help on some Mac keyboards (but
                                       does send code 73, not 117) */
        KeyHome = 74,
        KeyPageUp = 75,
        KeyDelete = 76,
        KeyEnd = 77,
        KeyPageDown = 78,
        KeyRight = 79,
        KeyLeft = 80,
        KeyDown = 81,
        KeyUp = 82,

        KeyNumLockClear = 83, /**< num lock on PC, clear on Mac keyboards
                               */
        KeyNumPadDivide = 84,
        KeyNumPadMultiply = 85,
        KeyNumPadMinus = 86,
        KeyNumPadPlus = 87,
        KeyNumPadEnter = 88,
        KeyNumPad1 = 89,
        KeyNumPad2 = 90,
        KeyNumPad3 = 91,
        KeyNumPad4 = 92,
        KeyNumPad5 = 93,
        KeyNumPad6 = 94,
        KeyNumPad7 = 95,
        KeyNumPad8 = 96,
        KeyNumPad9 = 97,
        KeyNumPad0 = 98,
        KeyNumPadPeriod = 99,

        KeyNonUSBackslash = 100, /**< This is the additional key that ISO
                                  *   keyboards have over ANSI ones,
                                  *   located between left shift and Y.
                                  *   Produces GRAVE ACCENT and TILDE in a
                                  *   US or UK Mac layout, REVERSE SOLIDUS
                                  *   (backslash) and VERTICAL LINE in a
                                  *   US or UK Windows layout, and
                                  *   LESS-THAN SIGN and GREATER-THAN SIGN
                                  *   in a Swiss German, German, or French
                                  *   layout. */
        KeyApplication = 101, /**< windows contextual menu, compose */
        KeyPower = 102, /**< The USB document says this is a status flag,
                         *   not a physical key - but some Mac keyboards
                         *   do have a power key. */
        KeyNumPadEquals = 103,
        KeyF13 = 104,
        KeyF14 = 105,
        KeyF15 = 106,
        KeyF16 = 107,
        KeyF17 = 108,
        KeyF18 = 109,
        KeyF19 = 110,
        KeyF20 = 111,
        KeyF21 = 112,
        KeyF22 = 113,
        KeyF23 = 114,
        KeyF24 = 115,
        KeyExecute = 116,
        KeyHelp = 117, /**< AL Integrated Help Center */
        KeyMenu = 118, /**< Menu (show menu) */
        KeySelect = 119,
        KeyStop = 120, /**< AC Stop */
        KeyAgain = 121, /**< AC Redo/Repeat */
        KeyUndo = 122, /**< AC Undo */
        KeyCut = 123, /**< AC Cut */
        KeyCopy = 124, /**< AC Copy */
        KeyPaste = 125, /**< AC Paste */
        KeyFind = 126, /**< AC Find */
        KeyMute = 127,
        KeyVolumeUp = 128,
        KeyVolumeDown = 129,
        /* not sure whether there's a reason to enable these */
        /*     KeyLockingcapslock = 130,  */
        /*     KeyLockingnumlock = 131, */
        /*     KeyLockingscrolllock = 132, */
        KeyNumPadComma = 133,
        KeyNumPadEqualsAs400 = 134,

        KeyInternational1 = 135, /**< used on Asian keyboards, see
                                                footnotes in USB doc */
        KeyInternational2 = 136,
        KeyInternational3 = 137, /**< Yen */
        KeyInternational4 = 138,
        KeyInternational5 = 139,
        KeyInternational6 = 140,
        KeyInternational7 = 141,
        KeyInternational8 = 142,
        KeyInternational9 = 143,
        KeyLang1 = 144, /**< Hangul/English toggle */
        KeyLang2 = 145, /**< Hanja conversion */
        KeyLang3 = 146, /**< Katakana */
        KeyLang4 = 147, /**< Hiragana */
        KeyLang5 = 148, /**< Zenkaku/Hankaku */
        KeyLang6 = 149, /**< reserved */
        KeyLang7 = 150, /**< reserved */
        KeyLang8 = 151, /**< reserved */
        KeyLang9 = 152, /**< reserved */

        KeyAltErase = 153, /**< Erase-Eaze */
        KeySysReq = 154,
        KeyCancel = 155, /**< AC Cancel */
        KeyClear = 156,
        KeyPrior = 157,
        KeyReturn2 = 158,
        KeySeparator = 159,
        KeyOut = 160,
        KeyOper = 161,
        KeyClearAgain = 162,
        KeyCrsel = 163,
        KeyExsel = 164,

        KeyNumPad00 = 176,
        KeyNumPad000 = 177,
        KeyThousandsSeparator = 178,
        KeyDecimalSeparator = 179,
        KeyCurrencyUnit = 180,
        KeyCurrencySubunit = 181,
        KeyNumPadLeftParen = 182,
        KeyNumPadRightParen = 183,
        KeyNumPadLeftBrace = 184,
        KeyNumPadRightBrace = 185,
        KeyNumPadTab = 186,
        KeyNumPadBackspace = 187,
        KeyNumPadA = 188,
        KeyNumPadB = 189,
        KeyNumPadC = 190,
        KeyNumPadD = 191,
        KeyNumPadE = 192,
        KeyNumPadF = 193,
        KeyNumPadXor = 194,
        KeyNumPadPower = 195,
        KeyNumPadPercent = 196,
        KeyNumPadLess = 197,
        KeyNumPadGreater = 198,
        KeyNumPadAmpersand = 199,
        KeyNumPadDblampersand = 200,
        KeyNumPadVerticalbar = 201,
        KeyNumPadDblverticalbar = 202,
        KeyNumPadColon = 203,
        KeyNumPadHash = 204,
        KeyNumPadSpace = 205,
        KeyNumPadAt = 206,
        KeyNumPadExclam = 207,
        KeyNumPadMemStore = 208,
        KeyNumPadMemRecall = 209,
        KeyNumPadMemClear = 210,
        KeyNumPadMemAdd = 211,
        KeyNumPadMemSubtract = 212,
        KeyNumPadMemMultiply = 213,
        KeyNumPadMemDivide = 214,
        KeyNumPadPlusMinus = 215,
        KeyNumPadClear = 216,
        KeyNumPadClearEntry = 217,
        KeyNumPadBinary = 218,
        KeyNumPadOctal = 219,
        KeyNumPadDecimal = 220,
        KeyNumPadHexadecimal = 221,

        KeyLeftCtrl = 224,
        KeyLeftShift = 225,
        KeyLeftAlt = 226, /**< alt, option */
        KeyLeftMeta = 227, /**< windows, command (apple), meta */
        KeyRightCtrl = 228,
        KeyRightShift = 229,
        KeyRightAlt = 230, /**< alt gr, option */
        KeyRightMeta = 231, /**< windows, command (apple), meta */

        KeyMode = 257, /**< I'm not sure if this is really not covered
                        *   by any of the above, but since there's a
                        *   special SDL_KMOD_MODE for it I'm adding it here
                        */

        /* @} */ /* Usage page 0x07 */

        /**
         *
         *  name Usage page 0x0C
         *
         *  These values are mapped from usage page 0x0C (USB consumer page).
         *
         *  There are way more keys in the spec than we can represent in the
         *  current scancode range, so pick the ones that commonly come up in
         *  real world usage.
         */
        /* @{ */

        KeySleep = 258, /**< Sleep */
        KeyWake = 259, /**< Wake */

        KeyChannelIncrement = 260, /**< Channel Increment */
        KeyChannelDecrement = 261, /**< Channel Decrement */

        KeyMediaPlay = 262, /**< Play */
        KeyMediaPause = 263, /**< Pause */
        KeyMediaRecord = 264, /**< Record */
        KeyMediaFastForward = 265, /**< Fast Forward */
        KeyMediaRewind = 266, /**< Rewind */
        KeyMediaNextTrack = 267, /**< Next Track */
        KeyMediaPreviousTrack = 268, /**< Previous Track */
        KeyMediaStop = 269, /**< Stop */
        KeyMediaEject = 270, /**< Eject */
        KeyMediaPlayPause = 271, /**< Play / Pause */
        KeyMediaSelect = 272, /* Media Select */

        KeyAcNew = 273, /**< AC New */
        KeyAcOpen = 274, /**< AC Open */
        KeyAcClose = 275, /**< AC Close */
        KeyAcExit = 276, /**< AC Exit */
        KeyAcSave = 277, /**< AC Save */
        KeyAcPrint = 278, /**< AC Print */
        KeyAcProperties = 279, /**< AC Properties */

        KeyAcSearch = 280, /**< AC Search */
        KeyAcHome = 281, /**< AC Home */
        KeyAcBack = 282, /**< AC Back */
        KeyAcForward = 283, /**< AC Forward */
        KeyAcStop = 284, /**< AC Stop */
        KeyAcRefresh = 285, /**< AC Refresh */
        KeyAcBookmarks = 286, /**< AC Bookmarks */

        /* @} */ /* Usage page 0x0C */


        /**
         *
ame Mobile keys
         *
         *  These are values that are often used on mobile phones.
         */
        /* @{ */

        KeySoftLeft = 287, /**< Usually situated below the display on phones and
                                          used as a multi-function feature key for selecting
                                          a software defined function shown on the bottom left
                                          of the display. */
        KeySoftRight = 288, /**< Usually situated below the display on phones and
                                           used as a multi-function feature key for selecting
                                           a software defined function shown on the bottom right
                                           of the display. */
        KeyCall = 289, /**< Used for accepting phone calls. */
        KeyEndCall = 290, /**< Used for rejecting phone calls. */

        /* @} */ /* Mobile keys */

        /* Add any other keys here. */

        KeyReserved = 400, /**< 400-500 reserved for dynamic keycodes */

        KeyCount = 512 /**< not a key, just marks the number of scancodes for array bounds */


    };

    enum MouseButton {
        MouseButton1,
        MouseButton2,
        MouseButton3,
        MouseButton4,
        MouseButton5,
        MouseButton6,
        MouseButton7,
        MouseButton8,
        MouseButtonLeft = MouseButton1,
        MouseButtonRight = MouseButton2,
        MouseButtonMiddle = MouseButton3,
    };

    enum GamepadButtons {
        GamepadA,
        GamepadB,
        GamepadX,
        GamepadY,
        GamepadLeftBumper,
        GamepadRightBumper,
        GamepadBack,
        GamepadStart,
        GamepadGuide,
        GamepadLeftThumb,
        GamepadRightThumb,
        GamepadDPadUp,
        GamepadDPadRight,
        GamepadDPadDown,
        GamepadDPadLeft,
        GamepadCross = GamepadA,
        GamepadCircle = GamepadB,
        GamepadSquare = GamepadX,
        GamepadTriangle = GamepadY,
    };

    enum GamepadAxis {
        GamepadAxisLeftXLeft,
        GamepadAxisLeftXRight,
        GamepadAxisLeftYUp,
        GamepadAxisLeftYDown,
        GamepadAxisRightXLeft,
        GamepadAxisRightXRight,
        GamepadAxisRightYUp,
        GamepadAxisRightYDown,
        GamepadAxisLeftX,
        GamepadAxisLeftY,
        GamepadAxisRightX,
        GamepadAxisRightY,
        GamepadAxisLeftTrigger,
        GamepadAxisRightTrigger,
    };
} // namespace HateEngine
