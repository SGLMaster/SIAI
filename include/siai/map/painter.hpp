#pragma once

#include <memory>

struct PanelPoint
{
    int x;
    int y;
};

struct PanelSize
{
    int width;
    int height;
};

struct PanelData
{
    PanelPoint origin;
    PanelSize size;
    int zoom;
};

enum class PanelColor
{
    WHITE,
    BLACK,
    RED
};

class wxDC;

class Painter
{
public:
    Painter();
    virtual ~Painter();

    virtual PanelPoint getOrigin() const noexcept = 0;
    virtual PanelSize getSize() const noexcept = 0;
    virtual int getZoom() const noexcept = 0;

    virtual void setBrush(PanelColor color) = 0;
    virtual void setPen(PanelColor color) = 0;

    virtual void drawRectangle(const PanelPoint& origin, const PanelSize& size) = 0;

    static std::unique_ptr<Painter> createWxPainter(wxDC& dc, const PanelData& data);
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

private:
    WxPainter() = delete;
};
