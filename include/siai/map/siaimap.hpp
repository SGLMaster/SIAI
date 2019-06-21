#ifndef __SIAIMAP_H__
#define __SIAIMAP_H__

#ifndef CONSOLE_APP
    #include <windows.h>
#endif

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif

#include "map/entities/entities.hpp"

#include <string>

class Painter;
class PanelPoint;

class MapPosition;

class DbConnector;

class DLL_EXPORT SIAIMap
{
public:
    static SIAIMap* createMap(bool forConsole=false);

    virtual void executeCommand(const std::string& command, DbConnector& connector) = 0;
    virtual void undo(DbConnector& connector) = 0;
    virtual void redo(DbConnector& connector) = 0;

    virtual void setName(const std::string& name) = 0;
    virtual std::string getName() = 0;

    virtual void loadFromDb(DbConnector& connector) = 0;
    virtual void updateFromDb(DbConnector& connector) = 0;

    virtual void reset(int numberOfColumns, int numberOfRows) = 0;
    virtual void repaint(Painter& painter) = 0;

    virtual int getNumberOfColumns() const noexcept = 0;
    virtual int getNumberOfRows() const noexcept = 0;

    virtual void selectEntity(const PanelPoint& point) = 0;
    virtual void diselectAll() = 0;

    virtual Entities::AgvPtr getAgvPtr(int id) = 0;

    virtual int getSelectedEntityColumn() const noexcept = 0;
    virtual int getSelectedEntityRow() const noexcept = 0;
    virtual int getSelectedId() const noexcept = 0;

    virtual void createDatabaseTables(DbConnector& connector) = 0;

    virtual void uploadChanges(DbConnector& connector) = 0;

    virtual bool moveAgvToCellWithId(DbConnector& connector, Entities::AgvPtr& agv, int idCell) = 0;
    virtual bool assignNewTaskToAgv(DbConnector& connector, Entities::AgvPtr& agv) = 0;
    virtual bool updateTaskForAgv(Entities::AgvPtr& agv) = 0;
    virtual bool liftRackInPosition(DbConnector& connector, const MapPosition& rackPosition) = 0;

    virtual void enableDirectionMarkers(bool value) = 0;

    SIAIMap();
    virtual ~SIAIMap();
};

#endif
