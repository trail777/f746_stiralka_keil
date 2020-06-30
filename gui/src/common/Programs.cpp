#include <gui/common/Programs.hpp>

const char* getProgramName(WasherProgram p)
{
    switch (p)
    {
    case PRG_ACTIVE_WEAR:
        return "ACTIVE WEAR";
    case PRG_DELICATES:
        return "DELICATES";
    case PRG_ECOWASH:
        return "ECO WASH";
    case PRG_HEAVYDUTY:
        return "HEAVY DUTY";
    case PRG_NORMAL:
        return "NORMAL";
    case PRG_QUICK_WASH:
        return "QUICK WASH";
    case PRG_SMARTWASH:
        return "SMART WASH";
    default:
        return "UNKNOWN";
    }
}

int getProgramTime(WasherProgram p)
{
    switch (p)
    {
    case PRG_ACTIVE_WEAR:
        return 160;
    case PRG_DELICATES:
        return 120;
    case PRG_ECOWASH:
        return 190;
    case PRG_HEAVYDUTY:
        return 165;
    case PRG_NORMAL:
        return 145;
    case PRG_QUICK_WASH:
        return 65;
    case PRG_SMARTWASH:
        return 135;
    default:
        return 0;
    }
}
