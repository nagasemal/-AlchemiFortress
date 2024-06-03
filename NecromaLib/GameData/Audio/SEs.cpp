#include "pch.h"
#include "SEs.h"

XACT_WAVEBANK_SES ConvertToElement(ELEMENT element)
{
        switch (element)
        {
        case NOMAL:
            return XACT_WAVEBANK_SES_EARTHMACHINEATTACK;
        case FLAME:
            return XACT_WAVEBANK_SES_FIREMACHINEATTACK;
        case AQUA:
            return XACT_WAVEBANK_SES_WATERMACHINEATTACK;
        case WIND:
            return XACT_WAVEBANK_SES_WINDEARTHMACHINEATTACK;
        case EARTH:
            return XACT_WAVEBANK_SES_EARTHMACHINEATTACK;
        default:
            break;
        }

        return XACT_WAVEBANK_SES_EARTHMACHINEATTACK;
}
