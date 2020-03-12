#include <agz/tracer/core/entity.h>
#include <agz/tracer/core/geometry.h>
#include <agz/tracer/core/light.h>
#include <agz/tracer/core/medium.h>
#include <agz/tracer/create/material.h>

AGZ_TRACER_BEGIN

namespace
{
    class DiffuseLight : public AreaLight
    {
        RC<const Geometry> geometry_;
        Spectrum radiance_;
        MediumInterface medium_interface_;

    public:

        DiffuseLight(
            RC<const Geometry> geometry,
            const Spectrum &radiance,
            const MediumInterface &med)
        {
            geometry_ = std::move(geometry);
            radiance_ = radiance;
            medium_interface_ = med;
        }

        LightSampleResult sample(
            const Vec3 &ref, const Sample5 &sam) const noexcept override
        {
            real pdf_area;
            const auto spt = geometry_->sample(ref, &pdf_area, { sam.u, sam.v, sam.r });

            if(dot(spt.geometry_coord.z, ref - spt.pos) <= 0)
                return LIGHT_SAMPLE_RESULT_NULL;

            const Vec3 spt_to_ref = ref - spt.pos;
            const real dist2 = spt_to_ref.length_square();

            LightSampleResult ret;
            ret.pos      = spt.pos;
            ret.nor      = spt.geometry_coord.z;
            ret.ref      = ref;
            ret.radiance = radiance_;
            ret.pdf      = pdf_area * dist2
                         / std::abs(cos(spt.geometry_coord.z, spt_to_ref));

            return ret;
        }

        real pdf(
            const Vec3 &ref, const SurfacePoint &spt) const noexcept override
        {
            if(dot(spt.geometry_coord.z, ref - spt.pos) <= 0)
                return 0;

            const real area_pdf = geometry_->pdf(ref, spt.pos);
            const Vec3 spt_to_ref = ref - spt.pos;
            const real dist2 = spt_to_ref.length_square();
            const real abscos = std::abs(cos(spt.geometry_coord.z, spt_to_ref));
            const real area_to_solid_angle_factor = dist2 / abscos;

            return area_pdf * area_to_solid_angle_factor;
        }

        LightEmitResult sample_emit(const Sample5 &sam) const noexcept override
        {
            real pdf_pos;
            const auto surface_point = geometry_->sample(
                &pdf_pos, { sam.u, sam.v, sam.w });

            const auto [local_dir, pdf_dir] = math::distribution
                    ::zweighted_on_hemisphere(sam.r, sam.s);

            const Vec3 global_dir = surface_point
                .geometry_coord
                .local_to_global(local_dir);

            LightEmitResult ret;
            ret.pos  = surface_point.eps_offset(global_dir);
            ret.dir = global_dir;
            ret.nor    = surface_point.geometry_coord.z;
            ret.radiance  = radiance_;
            ret.pdf_pos   = pdf_pos;
            ret.pdf_dir   = pdf_dir;

            return ret;
        }

        LightEmitPDFResult emit_pdf(
            const Vec3 &pos, const Vec3 &dir, const Vec3 &nor) const noexcept override
        {
            const real local_dir_z = cos(dir, nor);
            const real pdf_pos = geometry_->pdf(pos);

            const real pdf_dir = math::distribution
                ::zweighted_on_hemisphere_pdf(local_dir_z);

            return { pdf_pos, pdf_dir };
        }

        Spectrum power() const noexcept override
        {
            return PI_r * radiance_ * geometry_->surface_area();
        }

        Spectrum radiance(
            const Vec3 &pos, const Vec3 &nor, const Vec2 &uv,
            const Vec3 &light_to_out) const noexcept override
        {
            return dot(nor, light_to_out) > 0 ? radiance_ : Spectrum();
        }

        const Geometry *geometry() const noexcept
        {
            return geometry_.get();
        }

        const MediumInterface &medium_interface() const noexcept
        {
            return medium_interface_;
        }
    };
}

class DiffuseLightEntity : public Entity
{
    Box<DiffuseLight> light_;
    RC<Material> material_;

public:

    void initialize(
        RC<const Geometry> geometry,
        const Spectrum &radiance,
        const MediumInterface &med, bool no_denoise)
    {
        light_ = newBox<DiffuseLight>(std::move(geometry), radiance, med);
        material_ = create_ideal_black();
        set_no_denoise_flag(no_denoise);
    }

    bool has_intersection(const Ray &r) const noexcept override
    {
        return light_->geometry()->has_intersection(r);
    }

    bool closest_intersection(
        const Ray &r, EntityIntersection *inct) const noexcept override
    {
        if(!light_->geometry()->closest_intersection(r, inct))
            return false;
        inct->entity     = this;
        inct->material   = material_.get();
        inct->medium_in  = light_->medium_interface().in.get();
        inct->medium_out = light_->medium_interface().out.get();
        return true;
    }

    AABB world_bound() const noexcept override
    {
        return light_->geometry()->world_bound();
    }

    const AreaLight *as_light() const noexcept override
    {
        return light_.get();
    }

    AreaLight *as_light() noexcept override
    {
        return light_.get();
    }
};

RC<Entity> create_diffuse_light(
    RC<const Geometry> geometry,
    const Spectrum &radiance,
    const MediumInterface &med,
    bool no_denoise)
{
    auto ret = newRC<DiffuseLightEntity>();
    ret->initialize(geometry, radiance, med, no_denoise);
    return ret;
}

AGZ_TRACER_END