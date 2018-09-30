#include <Atrc/Atrc.h>

using namespace std;
using namespace Atrc;

constexpr uint32_t SCR_W = 640;
constexpr uint32_t SCR_H = 480;

constexpr Real SCR_ASPECT_RATIO = static_cast<Real>(SCR_W) / SCR_H;

int main()
{
    PerspectiveCamera camera(
        { -5.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 0.0, 0.0, 1.0 },
        Degr(90.0), SCR_ASPECT_RATIO, 1.0);

    ColoredSky sky({ 0.4f, 0.7f, 0.9f }, { 1.0f, 1.0f, 1.0f });
    DiffuseSphere ground(500.0, { 0.4f, 0.8f, 0.4f }, Transform::Translate({ 0.0, 0.0, -500.0 - 1.0 }));
    DiffuseSphere centreBall(1.0, { 0.7f, 0.7f, 0.7f }, TRANSFORM_IDENTITY);

    Scene scene;
    scene.camera = &camera;
    scene.entities = { &ground, &centreBall, &sky };

    RenderTarget<Color3f> renderTarget(SCR_W, SCR_H);

    //AmbientIntegrator integrator;
    PathTracer integrator(4);

    // ParallelRenderer<Native1sppSubareaRenderer> renderer(-1);
    ParallelRenderer<JitteredSubareaRenderer> renderer(-1, 1000);

    renderer.Render(scene, integrator, renderTarget);

    auto gammaCorrectedImage = renderTarget.Map(
    [](const Color3f &c)
    {
        return c.Map<float>([](float f) { return Pow(f, 1.0f / 2.2f); });
    });

    AGZ::Tex::TextureFile::WriteRGBToPNG(
        "Output/SinglePathTracedDiffuseSphere.png", AGZ::Tex::ClampedF2B(gammaCorrectedImage));
}
