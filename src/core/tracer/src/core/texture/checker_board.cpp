#include <agz/tracer/core/texture.h>
#include <agz/utility/misc.h>

AGZ_TRACER_BEGIN

class CheckerBoard : public Texture
{
    real grid_size_ = 1;

    Spectrum color_1_ = Spectrum(0, 0, 0);
    Spectrum color_2_ = Spectrum(1, 1, 1);

protected:

    Spectrum sample_spectrum_impl(const Vec2& uv) const noexcept override
    {
        int u_idx = static_cast<int>(math::saturate(uv.x) / grid_size_);
        int v_idx = static_cast<int>(math::saturate(uv.y) / grid_size_);
        if((u_idx + v_idx) & 1)
            return color_1_;
        return color_2_;
    }

public:

    void initialize(
        const TextureCommonParams &common_params,
        int grid_count, const Spectrum &color1, const Spectrum &color2)
    {
        init_common_params(common_params);

        if(grid_count <= 0)
            throw ObjectConstructionException("invalid grid_count value: " + std::to_string(grid_count));
        grid_size_ = real(1) / grid_count;

        color_1_ = color1;
        color_2_ = color2;
    }

    int width() const noexcept override
    {
        return 1;
    }

    int height() const noexcept override
    {
        return 1;
    }
};

std::shared_ptr<Texture> create_checker_board(
    const TextureCommonParams &common_params,
    int grid_count, const Spectrum &color1, const Spectrum &color2)
{
    auto ret = std::make_shared<CheckerBoard>();
    ret->initialize(common_params, grid_count, color1, color2);
    return ret;
}

AGZ_TRACER_END