#ifndef __SIAIMAP_H__
#define __SIAIMAP_H__

#include <windows.h>

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif

#include <string>

class Painter;
class PanelPoint;

class MapPosition;

class DLL_EXPORT SIAIMap
{
public:
    static SIAIMap* createMap();

    virtual int getNumberOfColumns() const noexcept = 0;
    virtual int getNumberOfRows() const noexcept = 0;

    virtual void reset(int numberOfColumns, int numberOfRows) = 0;
    virtual void repaint(Painter& painter) = 0;

    virtual void selectEntityWithPoint(const PanelPoint& point) = 0;
    virtual void diselectAllEntities() = 0;

    virtual int getLastSelectedId() const noexcept = 0;
    virtual MapPosition getLastSelectedPosition() const noexcept = 0;

    virtual void replaceCell(const std::string& type, const PanelPoint& position) = 0;

protected:
    SIAIMap();
    virtual ~SIAIMap();
};

#endif
