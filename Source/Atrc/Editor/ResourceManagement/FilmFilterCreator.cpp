#include <Atrc/Editor/ResourceManagement/FilmFilterCreator.h>

namespace
{
    class BoxFilterInstance : public FilmFilterInstance
    {
        float sidelen_ = 1;

    protected:

        void Export(const ResourceManager &rscMgr, LauncherScriptExportingContext &ctx) const override
        {
            ctx.AddLine("type = Box;");
            ctx.AddLine("sidelen = ", sidelen_, ";");
        }

    public:

        using FilmFilterInstance::FilmFilterInstance;

        void Display(ResourceManager&) override
        {
            ImGui::InputFloat("sidelen", &sidelen_);
        }
    };

    class GaussianFilterInstance : public FilmFilterInstance
    {
        float radius_ = 0.7f;
        float alpha_ = 2;

    protected:

        void Export(const ResourceManager &rscMgr, LauncherScriptExportingContext &ctx) const override
        {
            ctx.AddLine("type = Gaussian;");
            ctx.AddLine("radius = ", std::to_string(radius_), ";");
            ctx.AddLine("alpha = ", std::to_string(alpha_), ";");
        }

    public:

        using FilmFilterInstance::FilmFilterInstance;

        void Display(ResourceManager&) override
        {
            ImGui::InputFloat("radius.x", &radius_);
            ImGui::InputFloat("alpha", &alpha_);
        }
    };
}

void RegisterFilmFilterCreators(ResourceManager &rscMgr)
{
    static const BoxFilterCreator iBoxFilterCreator;
    static const GaussianFilterCreator iGaussianFilterCreator;
    rscMgr.AddCreator(&iBoxFilterCreator);
    rscMgr.AddCreator(&iGaussianFilterCreator);
}

std::shared_ptr<FilmFilterInstance> BoxFilterCreator::Create(ResourceManager &rscMgr, std::string name) const
{
    return std::make_shared<BoxFilterInstance>(GetName(), std::move(name));
}

std::shared_ptr<FilmFilterInstance> GaussianFilterCreator::Create(ResourceManager &rscMgr, std::string name) const
{
    return std::make_shared<GaussianFilterInstance>(GetName(), std::move(name));
}
