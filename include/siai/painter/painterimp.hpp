#pragma once

#include "painter/painter.hpp"

class wxImage;

class WxPanelImage : public PanelImage
{
private:
    std::unique_ptr<wxImage> m_image;

public:
    WxPanelImage(const std::string& path);
    virtual ~WxPanelImage();

    friend class WxPainter;

private:
    WxPanelImage() = delete;
};

class WxPainter : public Painter
{
private:
    wxDC& m_dc;

    PanelPoint m_origin;
    PanelSize m_size;
    int m_zoom;

public:
    WxPainter(wxDC& dc, const PanelData& data);
    virtual ~WxPainter();

    virtual PanelPoint getOrigin() const noexcept override
    {
        return m_origin;
    }
    virtual PanelSize getSize() const noexcept override
    {
        return m_size;
    }
    virtual int getZoom() const noexcept override
    {
        return m_zoom;
    }

    virtual void setBrush(PanelColor color) override;
    virtual void setPen(PanelColor color) override;

    virtual void drawRectangle(const PanelPoint& origin, const PanelSize& size) override;
    virtual void drawCircle(const PanelPoint& origin, int radio) override;
    virtual void drawImage(PanelImage& panelImage, const PanelPoint& origin, const PanelSize& size) override;

private:
    WxPainter() = delete;
};
