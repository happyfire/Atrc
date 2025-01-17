#pragma once

#include <agz/editor/geometry/geometry.h>
#include <agz/editor/ui/utility/elided_label.h>
#include <agz-utils/mesh.h>

AGZ_EDITOR_BEGIN

class TriangleBVHWidget : public GeometryWidget
{
    Q_OBJECT

public:

    using Vertex = mesh::vertex_t;

    struct CloneState
    {
        // vertices && tracer_object: cloning
        // vertices && !tracer_object: create tracer_object according to vertices
        // !vertices && !tracer_object completely new widget

        QString                              filename;
        RC<std::vector<Vertex>> vertices;
        RC<tracer::Geometry>    tracer_object;
    };

    explicit TriangleBVHWidget(const CloneState &clone_state);

    ResourceWidget<tracer::Geometry> *clone() override;

    void save_asset(AssetSaver &saver) override;

    void load_asset(AssetLoader &loader) override;

    RC<tracer::ConfigNode> to_config(JSONExportContext &ctx) const override;

    std::vector<EntityInterface::Vertex> get_vertices() const override;

protected:

    void update_tracer_object_impl() override;

private:

    void init_as_cube();

    void load_from_file();

    void do_update_tracer_object();

    QString                              filename_;
    RC<std::vector<Vertex>> vertices_;

    ElidedLabel *filename_label_  = nullptr;
};

class TriangleBVHWidgetCreator : public GeometryWidgetCreator
{
public:

    QString name() const override
    {
        return "Triangle Mesh";
    }

    ResourceWidget<tracer::Geometry> *create_widget(
        ObjectContext &obj_ctx) const override;
};

AGZ_EDITOR_END
