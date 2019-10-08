#include <agz/tracer/core/medium.h>
#include <agz/utility/misc.h>

AGZ_TRACER_BEGIN

class VoidMedium : public Medium
{
public:

    static std::string description()
    {
        return R"___(
void [Medium]
)___";
    }

    Spectrum tr(const Vec3 &a, const Vec3 &b) const noexcept override
    {
        return Spectrum(1);
    }

    SampleMediumResult sample(const Vec3 &o, const Vec3 &d, real t_min, real t_max, const Sample1 &sam) const noexcept override
    {
        return { {}, 1 };
    }

    ShadingPoint shade(const MediumIntersection &inct, Arena &arena) const noexcept override
    {
        return { nullptr };
    }
};

std::shared_ptr<Medium> create_void()
{
    return std::make_shared<VoidMedium>();
}

AGZ_TRACER_END