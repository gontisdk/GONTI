#ifndef GtClockH
#define GtClockH

#ifdef __cplusplus
    extern "C" {
#endif

        #include "GtClockTypes.inl"

        GTAPI void gontiClockUpdate(GtClock* clock);
        GTAPI void gontiClockStart(GtClock* clock);
        GTAPI void gontiClockStop(GtClock* clock);

#ifdef __cplusplus
    }
#endif

#endif