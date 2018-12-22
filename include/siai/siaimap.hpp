#ifndef __SIAIMAP_H__
#define __SIAIMAP_H__

#include <windows.h>

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif

class PainterContainer;
class PanelPoint;

class DLL_EXPORT SIAIMap
{
public:
    static SIAIMap* createMap();

    virtual int getNumberOfColumns() const noexcept = 0;
    virtual int getNumberOfRows() const noexcept = 0;

    virtual void reset(int numberOfColumns, int numberOfRows) = 0;
    virtual void repaint(PainterContainer& painter) = 0;

    virtual void selectEntityUnderMouse(const PanelPoint& mousePosition) = 0;
    virtual void diselectAllEntities() = 0;

    virtual void placeBlockedCell(const PanelPoint& mousePosition) = 0;

protected:
    SIAIMap();
    virtual ~SIAIMap();
};

#endif
