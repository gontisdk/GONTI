#ifndef INPUTS_H
#define INPUTS_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/Defines.h>
        #include "InputTypes.inl"

        KAPI void gontiInputInitialize();
        KAPI void gontiInputShutdown();
        KAPI void gontiInputUpdate(f64 deltaTime);

        KAPI void gontiInputProcessKey(GontiInputKeyboardKeys key, b8 pressed);
        KAPI void gontiInputProcessButton(GontiInputMouseButtons button, b8 pressed);
        KAPI void gontiInputProcessMouseMove(i16 x, i16 y);
        KAPI void gontiInputProcessMouseWheel(i8 zDelta);

        KAPI void gontiInputGetMousePosition(i32* x, i32* y);
        KAPI void gontiInputGetPreviousMousePosition(i32* x, i32* y);

        KAPI b8 gontiInputIsKeyDown(GontiInputKeyboardKeys key);
        KAPI b8 gontiInputIsKeyUp(GontiInputKeyboardKeys key);
        KAPI b8 gontiInputWasKeyDown(GontiInputKeyboardKeys key);
        KAPI b8 gontiInputWasKeyUp(GontiInputKeyboardKeys key);

        KAPI b8 gontiInputIsButtonDown(GontiInputMouseButtons button);
        KAPI b8 gontiInputIsButtonUp(GontiInputMouseButtons button);
        KAPI b8 gontiInputWasButtonDown(GontiInputMouseButtons button);
        KAPI b8 gontiInputWasButtonUp(GontiInputMouseButtons button);

#ifdef __cplusplus
    }
#endif

#endif