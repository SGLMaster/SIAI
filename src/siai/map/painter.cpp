#include "map/painter.hpp"

#include <wx/dc.h>
#include <wx/scrolwin.h>

PanelImage::PanelImage() = default;
PanelImage::~PanelImage() = default;

std::unique_ptr<PanelImage> PanelImage::create(const std::string& path)
{
    return std::make_unique<WxPanelImage>(path);
}

WxPanelImage::WxPanelImage() = default;
WxPanelImage::WxPanelImage(const std::string& path) : m_image{ std::make_unique<wxImage>(path, wxBITMAP_TYPE_PNG) } {}
WxPanelImage::~WxPanelImage() = default;

Painter::Painter() = default;
Painter::~Painter() = default;

std::unique_ptr<Painter> Painter::createWxPainter(wxDC& dc, const PanelData& data)
{
    return std::make_unique<WxPainter>(dc, data);
}

WxPainter::WxPainter(wxDC& dc, const PanelData& data) : m_dc{dc},
                                                        m_origin{data.origin}, m_size{data.size}, m_zoom{data.zoom}
{
    dc.Clear();
}

WxPainter::~WxPainter() = default;

void WxPainter::setBrush(PanelColor color)
{
    switch(color)
    {
    case PanelColor::WHITE:
        m_dc.SetBrush(*wxWHITE_BRUSH);
        break;
    case PanelColor::BLACK:
        m_dc.SetBrush(*wxBLACK_BRUSH);
        break;
    case PanelColor::RED:
        m_dc.SetBrush(*wxRED_BRUSH);
        break;
    default:
        m_dc.SetBrush(*wxWHITE_BRUSH);
        break;
    }
}

void WxPainter::setPen(PanelColor color)
{
    switch(color)
    {
    case PanelColor::WHITE:
        m_dc.SetPen(*wxWHITE_PEN);
        break;
    case PanelColor::BLACK:
        m_dc.SetPen(*wxBLACK_PEN);
        break;
    case PanelColor::RED:
        m_dc.SetPen(*wxRED_PEN);
        break;
    case PanelColor::SELECTED:
        m_dc.SetPen(*wxRED_PEN);
        break;
    default:
        m_dc.SetPen(*wxBLACK_PEN);
        break;
    }
}

void WxPainter::drawRectangle(const PanelPoint& origin, const PanelSize& size)
{
    m_dc.DrawRectangle(origin.x, origin.y, size.width, size.height);
}

void WxPainter::drawImage(PanelImage& panelImage, const PanelPoint& origin, const PanelSize& size)
{
    WxPanelImage& castedPanelImage = dynamic_cast<WxPanelImage&>(panelImage);

    wxImage* image = castedPanelImage.m_image.get();

    wxBitmap bitmap = wxBitmap(image->Scale(size.width, size.height));

    m_dc.DrawBitmap(bitmap, origin.x, origin.y);
}
