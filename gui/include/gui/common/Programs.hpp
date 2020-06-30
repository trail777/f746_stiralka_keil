#ifndef PROGRAMS_HPP
#define PROGRAMS_HPP

enum WasherProgram
{
    PRG_ACTIVE_WEAR,
    PRG_DELICATES,
    PRG_ECOWASH,
    PRG_HEAVYDUTY,
    PRG_NORMAL,
    PRG_QUICK_WASH,
    PRG_SMARTWASH
};

const char* getProgramName(WasherProgram p);
int getProgramTime(WasherProgram p);

#endif
