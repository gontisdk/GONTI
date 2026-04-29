#ifndef GtInputsH
#define GtInputsH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.inl>
        #include <Inputs/GtInputTypes.inl>

        GTAPI void gontiInputInitialize(GtU64* memoryRequirement, void* state);
        GTAPI void gontiInputShutdown(void* state);
        GTAPI void gontiInputUpdate(GtF64 deltaTime);

        GTAPI void gontiInputProcessKey(GtInputKeyboardKeys key, GtB8 pressed);
        GTAPI void gontiInputProcessButton(GtInputMouseBtns button, GtB8 pressed);
        GTAPI void gontiInputProcessMouseMove(GtI16 x, GtI16 y);
        GTAPI void gontiInputProcessMouseWheel(GtI8 zDelta);

        GTAPI void gontiInputGetMousePosition(GtI32* x, GtI32* y);
        GTAPI void gontiInputGetPreviousMousePosition(GtI32* x, GtI32* y);

        GTAPI GtB8 gontiInputIsKeyDown(GtInputKeyboardKeys key);
        GTAPI GtB8 gontiInputIsKeyUp(GtInputKeyboardKeys key);
        GTAPI GtB8 gontiInputWasKeyDown(GtInputKeyboardKeys key);
        GTAPI GtB8 gontiInputWasKeyUp(GtInputKeyboardKeys key);

        GTAPI GtB8 gontiInputIsButtonDown(GtInputMouseBtns button);
        GTAPI GtB8 gontiInputIsButtonUp(GtInputMouseBtns button);
        GTAPI GtB8 gontiInputWasButtonDown(GtInputMouseBtns button);
        GTAPI GtB8 gontiInputWasButtonUp(GtInputMouseBtns button);

#ifdef __cplusplus
    }
#endif

#endif