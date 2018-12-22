#pragma once

class wxDC;

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

struct PainterContainer
{
    wxDC& drawClient;

    int zoom;

    const PanelPoint& panelOrigin;
    const PanelSize& panelSize;
};
