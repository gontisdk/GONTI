#ifndef GtInputTypesINL
#define GtInputTypesINL

#ifdef __cplusplus
    extern "C" {
#endif

        typedef enum GtInputMouseBtns {
            GT_BTN_LEFT,
            GT_BTN_RIGHT,
            GT_BTN_MIDDLE,
            BUTTON_MAX_GT_BTNS
        }GtInputMouseBtns;

        //#define DEFINE_KEYBOARD_KEY(name, code) GT_KEY_##name = code

        typedef enum GtInputKeyboardKeys {
            GT_KEY_BACKSPACE = 0x08,
            GT_KEY_ENTER = 0x0D,
            GT_KEY_TAB = 0x09,
            GT_KEY_SHIFT = 0x10,
            GT_KEY_CONTROL = 0x11,

            GT_KEY_PAUSE = 0x13,
            GT_KEY_CAPITAL = 0x14,

            GT_KEY_ESCAPE = 0x1B,

            GT_KEY_CONVERT = 0x1C,
            GT_KEY_NONCONVERT = 0x1D,
            GT_KEY_ACCEPT = 0x1E,
            GT_KEY_MODECHANGE = 0x1F,

            GT_KEY_SPACE = 0x20,
            GT_KEY_PAGEUP = 0x21,
            GT_KEY_PAGEDOWN = 0x22,
            GT_KEY_END = 0x23,
            GT_KEY_HOME = 0x24,
            GT_KEY_LEFT = 0x25,
            GT_KEY_UP = 0x26,
            GT_KEY_RIGHT = 0x27,
            GT_KEY_DOWN = 0x28,
            GT_KEY_SELECT = 0x29,
            GT_KEY_PRINT = 0x2A,
            GT_KEY_EXECUTE = 0x2B, //
            GT_KEY_PRINTSCREEN = 0x2C,
            GT_KEY_INSERT = 0x2D,
            GT_KEY_DELETE = 0x2E,
            GT_KEY_HELP = 0x2F,

            GT_KEY_0 = 0x30,
            GT_KEY_1 = 0x31,
            GT_KEY_2 = 0x32,
            GT_KEY_3 = 0x33,
            GT_KEY_4 = 0x34,
            GT_KEY_5 = 0x35,
            GT_KEY_6 = 0x36,
            GT_KEY_7 = 0x37,
            GT_KEY_8 = 0x38,
            GT_KEY_9 = 0x39,

            GT_KEY_A = 0x41,
            GT_KEY_B = 0x42,
            GT_KEY_C = 0x43,
            GT_KEY_D = 0x44,
            GT_KEY_E = 0x45,
            GT_KEY_F = 0x46,
            GT_KEY_G = 0x47,
            GT_KEY_H = 0x48,
            GT_KEY_I = 0x49,
            GT_KEY_J = 0x4A,
            GT_KEY_K = 0x4B,
            GT_KEY_L = 0x4C,
            GT_KEY_M = 0x4D,
            GT_KEY_N = 0x4E,
            GT_KEY_O = 0x4F,
            GT_KEY_P = 0x50,
            GT_KEY_Q = 0x51,
            GT_KEY_R = 0x52,
            GT_KEY_S = 0x53,
            GT_KEY_T = 0x54,
            GT_KEY_U = 0x55,
            GT_KEY_V = 0x56,
            GT_KEY_W = 0x57,
            GT_KEY_X = 0x58,
            GT_KEY_Y = 0x59,
            GT_KEY_Z = 0x5A,

            GT_KEY_LSUPER = 0x5B,
            GT_KEY_RSUPER = 0x5C,
            GT_KEY_APPS = 0x5D,

            GT_KEY_SLEEP = 0x5F,

            GT_KEY_NUMPAD0 = 0x60,
            GT_KEY_NUMPAD1 = 0x61,
            GT_KEY_NUMPAD2 = 0x62,
            GT_KEY_NUMPAD3 = 0x63,
            GT_KEY_NUMPAD4 = 0x64,
            GT_KEY_NUMPAD5 = 0x65,
            GT_KEY_NUMPAD6 = 0x66,
            GT_KEY_NUMPAD7 = 0x67,
            GT_KEY_NUMPAD8 = 0x68,
            GT_KEY_NUMPAD9 = 0x69,
            GT_KEY_MULTIPLY = 0x6A,
            GT_KEY_ADD = 0x6B,
            GT_KEY_SEPARATOR = 0x6C,
            GT_KEY_SUBTRACT = 0x6D,
            GT_KEY_DECIMAL = 0x6E,
            GT_KEY_DIVIDE = 0x6F,

            GT_KEY_F1 = 0x70,
            GT_KEY_F2 = 0x71,
            GT_KEY_F3 = 0x72,
            GT_KEY_F4 = 0x73,
            GT_KEY_F5 = 0x74,
            GT_KEY_F6 = 0x75,
            GT_KEY_F7 = 0x76,
            GT_KEY_F8 = 0x77,
            GT_KEY_F9 = 0x78,
            GT_KEY_F10 = 0x79,
            GT_KEY_F11 = 0x7A,
            GT_KEY_F12 = 0x7B,
            GT_KEY_F13 = 0x7C,
            GT_KEY_F14 = 0x7D,
            GT_KEY_F15 = 0x7E,
            GT_KEY_F16 = 0x7F,
            GT_KEY_F17 = 0x80,
            GT_KEY_F18 = 0x81,
            GT_KEY_F19 = 0x82,
            GT_KEY_F20 = 0x83,
            GT_KEY_F21 = 0x84,
            GT_KEY_F22 = 0x85,
            GT_KEY_F23 = 0x86,
            GT_KEY_F24 = 0x87,

            GT_KEY_NUMLOCK = 0x90,

            GT_KEY_SCROLL = 0x91,

            GT_KEY_NUMPAD_EQUAL = 0x92,

            GT_KEY_LSHIFT = 0xA0,
            GT_KEY_RSHIFT = 0xA1,
            GT_KEY_LCONTROL = 0xA2,
            GT_KEY_RCONTROL = 0xA3,
            GT_KEY_LALT = 0xA4,
            GT_KEY_RALT = 0xA5,

            GT_KEY_SEMICOLON = 0x3B,

            GT_KEY_APOSTROPHE = 0xDE,
            GT_KEY_QUOTE = GT_KEY_APOSTROPHE,
            GT_KEY_EQUAL = 0xBB,
            GT_KEY_COMMA = 0xBC,
            GT_KEY_MINUS = 0xBD,
            GT_KEY_PERIOD = 0xBE,
            GT_KEY_SLASH = 0xBF,

            GT_KEY_GRAVE = 0xC0,

            GT_KEY_LBRACKET = 0xDB,
            GT_KEY_PIPE = 0xDC,
            GT_KEY_BACKSLASH = GT_KEY_PIPE,
            GT_KEY_RBRACKET = 0xDD,

            KEY_MAX_GONTI_KEYS = 0xFF
        }GtInputKeyboardKeys;

#ifdef __cplusplus
    }
#endif

#endif