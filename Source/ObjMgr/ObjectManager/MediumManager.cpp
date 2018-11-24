#include "../ParamParser.h"
#include "MediumManager.h"

AGZ_NS_BEG(ObjMgr)

Atrc::Medium *HomongeneousMediumCreator::Create(const ConfigGroup &params, ObjArena<> &arena) const
{
    auto sigmaA = ParamParser::ParseSpectrum(params["sigmaA"]);
    auto sigmaS = ParamParser::ParseSpectrum(params["sigmaS"]);
    auto le     = ParamParser::ParseSpectrum(params["le"]);
    auto g      = params["g"].AsValue().Parse<Atrc::Real>();

    return arena.Create<Atrc::HomogeneousMedium>(sigmaA, sigmaS, le, g);
}

AGZ_NS_END(ObjMgr)
