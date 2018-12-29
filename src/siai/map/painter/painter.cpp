#include "map/painter/painterimp.hpp"

PanelImage::PanelImage() = default;
PanelImage::~PanelImage() = default;

std::unique_ptr<PanelImage> PanelImage::create(const std::string& path)
{
    return std::make_unique<WxPanelImage>(path);
}

Painter::Painter() = default;
Painter::~Painter() = default;

std::unique_ptr<Painter> Painter::createWxPainter(wxDC& dc, const PanelData& data)
{
    return std::make_unique<WxPainter>(dc, data);
}
