#include <Atrc/Material/BxDF/PerfectSpecularReflection.h>
#include <Atrc/Material/BxDF/PerfectSpecularTransmission.h>
#include <Atrc/Material/BxDF/PerfectSpecular.h>
#include <Atrc/Material/BxDFAggregate.h>
#include <Atrc/Material/FresnelSpecular.h>

AGZ_NS_BEG(Atrc)

FresnelSpecular::FresnelSpecular(const Spectrum &rc, RC<const FresnelDielectric> fresnel)
    : rc_(rc), fresnel_(std::move(fresnel))
{
    AGZ_ASSERT(fresnel_);
}

void FresnelSpecular::Shade(const SurfacePoint &sp, ShadingPoint *dst) const
{
    AGZ_ASSERT(dst);

    auto bsdf = MakeRC<BxDFAggregate>(sp.geoLocal, sp.geoLocal);
    //bsdf->AddBxDF(MakeRC<PerfectSpecularReflection>(rc_, fresnel_));
    //bsdf->AddBxDF(MakeRC<PerfectSpecularTransmission>(rc_, fresnel_));
    bsdf->AddBxDF(MakeRC<PerfectSpecular>(rc_, fresnel_));

    dst->bsdf = std::move(bsdf);
    dst->shdLocal = sp.geoLocal;
}

AGZ_NS_END(Atrc)
