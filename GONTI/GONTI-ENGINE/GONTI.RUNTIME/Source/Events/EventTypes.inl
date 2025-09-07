#ifndef EVENTTYPES_INL
#define EVENTTYPES_INL

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/Defines.h>

        typedef struct GontiEventContext {
            union {
                i64 i64[2];
                u64 u64[2];
                f64 f64[2];

                i32 i32[4];
                u32 u32[4];
                f32 f32[4];

                i16 i16[8];
                u16 u16[8];

                i8 i8[16];
                u8 u8[16];

                char c[16];
            }data;
        }GontiEventContext;

        typedef enum GontiSystemEventCode {
            GONTI_EVENT_CODE_APPLICATION_QUIT = 0x01,
            GONTI_EVENT_CODE_KEY_PRESSED = 0x02,
            GONTI_EVENT_CODE_KEY_RELEASED = 0x03,
            GONTI_EVENT_CODE_BUTTON_PRESSED = 0x04,
            GONTI_EVENT_CODE_BUTTON_RELEASED = 0x05,
            GONTI_EVENT_CODE_MOUSE_MOVED = 0x06,
            GONTI_EVENT_CODE_MOUSE_WHEEL = 0x07,
            GONTI_EVENT_CODE_RESIZED = 0x08,
            GONTI_EVENT_CODE_APPLICATION_PAUSE = 0x09,
            GONTI_EVENT_CODE_APPLICATION_RESUME = 0x10,

            MAX_GONTI_EVENT_CODE = 0xFF
        }GontiSystemEventCode;

        typedef b8 (*PFN_gontiOnEvent)(u16 code, void* sender, void* listenerInst, GontiEventContext data);

#ifdef __cplusplus
    }
#endif

#endif