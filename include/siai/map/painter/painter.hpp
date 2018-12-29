#pragma once

#include <memory>
#include <string>

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
    RED,
    SELECTED
};

class wxDC;

class PanelImage
{
public:
    static std::unique_ptr<PanelImage> create(const std::string& path);

    PanelImage();
    virtual ~PanelImage();
};

class Painter
{
public:
    static std::unique_ptr<Painter> createWxPainter(wxDC& dc, const PanelData& data);

    Painter();
    virtual ~Painter();

    virtual PanelPoint getOrigin() const noexcept = 0;
    virtual PanelSize getSize() const noexcept = 0;
    virtual int getZoom() const noexcept = 0;

    virtual void setBrush(PanelColor color) = 0;
    virtual void setPen(PanelColor color) = 0;

    virtual void drawRectangle(const PanelPoint& origin, const PanelSize& size) = 0;
    virtual void drawImage(PanelImage& panelImage, const PanelPoint& origin, const PanelSize& size) = 0;
};
