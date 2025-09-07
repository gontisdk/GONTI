#ifndef EVENTS_H
#define EVENTS_H

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "../__GONTI_INCLUDES.h"

            #ifdef Events_ON

                typedef struct EventContext {
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
                }EventContext;

                typedef enum SystemEventCode {
                    EVENT_CODE_APPLICATION_QUIT = 0x01,
                    EVENT_CODE_KEY_PRESSED = 0x02,
                    EVENT_CODE_KEY_RELEASED = 0x03,
                    EVENT_CODE_BUTTON_PRESSED = 0x04,
                    EVENT_CODE_BUTTON_RELEASED = 0x05,
                    EVENT_CODE_MOUSE_MOVED = 0x06,
                    EVENT_CODE_MOUSE_WHEEL = 0x07,
                    EVENT_CODE_RESIZED = 0x08,
                    EVENT_CODE_APPLICATION_PAUSE = 0x09,
                    EVENT_CODE_APPLICATION_RESUME = 0x10,

                    MAX_EVENT_CODE = 0xFF
                }SystemEventCode;

                typedef b8 (*PFN_onEvent)(u16 code, void* sender, void* listenerInst, EventContext data);

                /*BOOL*/
                KAPI b8 eventInitialize();
                KAPI b8 eventRegister(u16 code, void* listener, PFN_onEvent onEvent);
                KAPI b8 eventUnregister(u16 code, void* listener, PFN_onEvent onEvent);
                KAPI b8 eventFire(u16 code, void* sender, EventContext context);

                /*VOID*/
                KAPI void eventShutdown();

            #endif

    #ifdef __cplusplus
        }
    #endif

#endif