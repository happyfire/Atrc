#pragma once

#include <Atrc/Atrc/ResourceInterface/FilmFilter.h>

#include "ui_Box.h"

class BoxCore : public QWidget
{
    Q_OBJECT

public:

    static const char *GetTypeName();

    BoxCore();

    std::string Serialize() const;

    void Deserialize(const AGZ::ConfigNode &node);

    bool IsMultiline() const noexcept;

private:

    Ui::BoxWidget ui_;
};

using Box = WidgetCore2FilmFilterInstance<BoxCore>;
