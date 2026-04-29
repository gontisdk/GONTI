#ifndef GtEventTypesINL
#define GtEventTypesINL

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.inl>

        typedef struct GtEventContext {
            union {
                GtI64 GtI64[2];
                GtU64 GtU64[2];
                GtF64 GtF64[2];

                GtI32 GtI32[4];
                GtU32 GtU32[4];
                GtF32 GtF32[4];

                GtI16 GtI16[8];
                GtU16 GtU16[8];

                GtI8 GtI8[16];
                GtU8 GtU8[16];

                char c[16];
            }data;
        }GtEventContext;

        typedef enum GtSystemEventCode {
            GT_EVENT_CODE_APPLICATION_QUIT = 0x01,
            GT_EVENT_CODE_KEY_PRESSED = 0x02,
            GT_EVENT_CODE_KEY_RELEASED = 0x03,
            GT_EVENT_CODE_BUTTON_PRESSED = 0x04,
            GT_EVENT_CODE_BUTTON_RELEASED = 0x05,
            GT_EVENT_CODE_MOUSE_MOVED = 0x06,
            GT_EVENT_CODE_MOUSE_WHEEL = 0x07,
            GT_EVENT_CODE_RESIZED = 0x08,
            GT_EVENT_CODE_APPLICATION_PAUSE = 0x09,
            GT_EVENT_CODE_APPLICATION_RESUME = 0x10,

            MAX_GT_EVENT_CODE = 0xFF
        }GtSystemEventCode;

        typedef GtB8 (*GtOnEventFN)(GtU16 code, void* sender, void* listenerInst, GtEventContext data);

#ifdef __cplusplus
    }
#endif

#endif