#ifndef INPUTTYPES_INL
#define INPUTTYPES_INL

#ifdef __cplusplus
    extern "C" {
#endif

        typedef enum GontiInputMouseButtons {
            GONTI_BUTTON_LEFT,
            GONTI_BUTTON_RIGHT,
            GONTI_BUTTON_MIDDLE,
            BUTTON_MAX_GONTI_BUTTONS
        }GontiInputMouseButtons;

        //#define DEFINE_KEYBOARD_KEY(name, code) GONTI_KEY_##name = code

        // Undef using keys from Windows.h for no colisions
        //#undef GONTI_KEY_EXECUTE

        typedef enum GontiInputKeyboardKeys {
            GONTI_KEY_BACKSPACE = 0x08,
            GONTI_KEY_ENTER = 0x0D,
            GONTI_KEY_TAB = 0x09,
            GONTI_KEY_SHIFT = 0x10,
            GONTI_KEY_CONTROL = 0x11,

            GONTI_KEY_PAUSE = 0x13,
            GONTI_KEY_CAPITAL = 0x14,

            GONTI_KEY_ESCAPE = 0x1B,

            GONTI_KEY_CONVERT = 0x1C,
            GONTI_KEY_NONCONVERT = 0x1D,
            GONTI_KEY_ACCEPT = 0x1E,
            GONTI_KEY_MODECHANGE = 0x1F,

            GONTI_KEY_SPACE = 0x20,
            GONTI_KEY_PAGEUP = 0x21,
            GONTI_KEY_PAGEDOWN = 0x22,
            GONTI_KEY_END = 0x23,
            GONTI_KEY_HOME = 0x24,
            GONTI_KEY_LEFT = 0x25,
            GONTI_KEY_UP = 0x26,
            GONTI_KEY_RIGHT = 0x27,
            GONTI_KEY_DOWN = 0x28,
            GONTI_KEY_SELECT = 0x29,
            GONTI_KEY_PRINT = 0x2A,
            //GONTI_KEY_EXECUTE = 0x2B,
            GONTI_KEY_PRINTSCREEN = 0x2C,
            GONTI_KEY_INSERT = 0x2D,
            GONTI_KEY_DELETE = 0x2E,
            GONTI_KEY_HELP = 0x2F,

            GONTI_KEY_0 = 0x30,
            GONTI_KEY_1 = 0x31,
            GONTI_KEY_2 = 0x32,
            GONTI_KEY_3 = 0x33,
            GONTI_KEY_4 = 0x34,
            GONTI_KEY_5 = 0x35,
            GONTI_KEY_6 = 0x36,
            GONTI_KEY_7 = 0x37,
            GONTI_KEY_8 = 0x38,
            GONTI_KEY_9 = 0x39,

            GONTI_KEY_A = 0x41,
            GONTI_KEY_B = 0x42,
            GONTI_KEY_C = 0x43,
            GONTI_KEY_D = 0x44,
            GONTI_KEY_E = 0x45,
            GONTI_KEY_F = 0x46,
            GONTI_KEY_G = 0x47,
            GONTI_KEY_H = 0x48,
            GONTI_KEY_I = 0x49,
            GONTI_KEY_J = 0x4A,
            GONTI_KEY_K = 0x4B,
            GONTI_KEY_L = 0x4C,
            GONTI_KEY_M = 0x4D,
            GONTI_KEY_N = 0x4E,
            GONTI_KEY_O = 0x4F,
            GONTI_KEY_P = 0x50,
            GONTI_KEY_Q = 0x51,
            GONTI_KEY_R = 0x52,
            GONTI_KEY_S = 0x53,
            GONTI_KEY_T = 0x54,
            GONTI_KEY_U = 0x55,
            GONTI_KEY_V = 0x56,
            GONTI_KEY_W = 0x57,
            GONTI_KEY_X = 0x58,
            GONTI_KEY_Y = 0x59,
            GONTI_KEY_Z = 0x5A,

            GONTI_KEY_LSUPER = 0x5B,
            GONTI_KEY_RSUPER = 0x5C,
            GONTI_KEY_APPS = 0x5D,

            GONTI_KEY_SLEEP = 0x5F,

            GONTI_KEY_NUMPAD0 = 0x60,
            GONTI_KEY_NUMPAD1 = 0x61,
            GONTI_KEY_NUMPAD2 = 0x62,
            GONTI_KEY_NUMPAD3 = 0x63,
            GONTI_KEY_NUMPAD4 = 0x64,
            GONTI_KEY_NUMPAD5 = 0x65,
            GONTI_KEY_NUMPAD6 = 0x66,
            GONTI_KEY_NUMPAD7 = 0x67,
            GONTI_KEY_NUMPAD8 = 0x68,
            GONTI_KEY_NUMPAD9 = 0x69,
            GONTI_KEY_MULTIPLY = 0x6A,
            GONTI_KEY_ADD = 0x6B,
            GONTI_KEY_SEPARATOR = 0x6C,
            GONTI_KEY_SUBTRACT = 0x6D,
            GONTI_KEY_DECIMAL = 0x6E,
            GONTI_KEY_DIVIDE = 0x6F,

            GONTI_KEY_F1 = 0x70,
            GONTI_KEY_F2 = 0x71,
            GONTI_KEY_F3 = 0x72,
            GONTI_KEY_F4 = 0x73,
            GONTI_KEY_F5 = 0x74,
            GONTI_KEY_F6 = 0x75,
            GONTI_KEY_F7 = 0x76,
            GONTI_KEY_F8 = 0x77,
            GONTI_KEY_F9 = 0x78,
            GONTI_KEY_F10 = 0x79,
            GONTI_KEY_F11 = 0x7A,
            GONTI_KEY_F12 = 0x7B,
            GONTI_KEY_F13 = 0x7C,
            GONTI_KEY_F14 = 0x7D,
            GONTI_KEY_F15 = 0x7E,
            GONTI_KEY_F16 = 0x7F,
            GONTI_KEY_F17 = 0x80,
            GONTI_KEY_F18 = 0x81,
            GONTI_KEY_F19 = 0x82,
            GONTI_KEY_F20 = 0x83,
            GONTI_KEY_F21 = 0x84,
            GONTI_KEY_F22 = 0x85,
            GONTI_KEY_F23 = 0x86,
            GONTI_KEY_F24 = 0x87,

            GONTI_KEY_NUMLOCK = 0x90,

            GONTI_KEY_SCROLL = 0x91,

            GONTI_KEY_NUMPAD_EQUAL = 0x92,

            GONTI_KEY_LSHIFT = 0xA0,
            GONTI_KEY_RSHIFT = 0xA1,
            GONTI_KEY_LCONTROL = 0xA2,
            GONTI_KEY_RCONTROL = 0xA3,
            GONTI_KEY_LALT = 0xA4,
            GONTI_KEY_RALT = 0xA5,

            GONTI_KEY_SEMICOLON = 0x3B,

            GONTI_KEY_APOSTROPHE = 0xDE,
            GONTI_KEY_QUOTE = GONTI_KEY_APOSTROPHE,
            GONTI_KEY_EQUAL = 0xBB,
            GONTI_KEY_COMMA = 0xBC,
            GONTI_KEY_MINUS = 0xBD,
            GONTI_KEY_PERIOD = 0xBE,
            GONTI_KEY_SLASH = 0xBF,

            GONTI_KEY_GRAVE = 0xC0,

            GONTI_KEY_LBRACKET = 0xDB,
            GONTI_KEY_PIPE = 0xDC,
            GONTI_KEY_BACKSLASH = GONTI_KEY_PIPE,
            GONTI_KEY_RBRACKET = 0xDD,

            KEY_MAX_GONTI_KEYS = 0xFF
        }GontiInputKeyboardKeys;

#ifdef __cplusplus
    }
#endif

#endif