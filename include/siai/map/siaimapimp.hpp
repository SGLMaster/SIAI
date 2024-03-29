#pragma once

#include "map/entities/entities.hpp"

#include "map/siaimap.hpp"

#include "database/database.hpp"

#include <memory>

class CommandStream;

class DbQuery;

class SIAIMapImp : public SIAIMap
{
private:
    bool m_createdForConsole;

	std::string m_name;

    std::string m_cellsDbTableName;
    std::string m_agvsDbTableName;
    std::string m_racksDbTableName;
    std::string m_itemsDbTableName;
    std::string m_ingressDbTableName;

    int m_numberOfColumns{0};
    int m_numberOfRows{0};

    Entities::Stock m_entities;
    Entities::Container m_selectedEntities;

    std::unique_ptr<CommandStream> m_commandStream;

public:
    SIAIMapImp(bool forConsole);
    virtual ~SIAIMapImp();

    virtual void executeCommand(const std::string& command, DbConnector& connector) override;
    virtual void undo(DbConnector& connector) override;
    virtual void redo(DbConnector& connector) override;

    virtual void setName(const std::string& name) override;
    virtual std::string getName() override;

    virtual void loadFromDb(DbConnector& connector) override;
    virtual void updateFromDb(DbConnector& connector) override;

    virtual void reset(int numberOfColumns, int numberOfRows) override;
    virtual void repaint(Painter& painter) override;

    virtual int getNumberOfColumns() const noexcept override;
    virtual int getNumberOfRows() const noexcept override;

    virtual void selectEntity(const PanelPoint& point) override;
    virtual void diselectAll() override;

    virtual Entities::AgvPtr getAgvPtr(int id) override;

    virtual int getSelectedEntityColumn() const noexcept override;
    virtual int getSelectedEntityRow() const noexcept override;
    virtual int getSelectedId() const noexcept;

    virtual void createDatabaseTables(DbConnector& connector) override;

    virtual void uploadChanges(DbConnector& connector) override;

    virtual bool moveAgvToCellWithId(DbConnector& connector, Entities::AgvPtr& agv, int idCell) override;
    virtual bool assignNewTaskToAgv(DbConnector& connector, Entities::AgvPtr& agv) override;
    virtual bool updateTaskForAgv(Entities::AgvPtr& agv) override;
    virtual bool liftRackInPosition(DbConnector& connector, const MapPosition& rackPosition) override;
    virtual bool dropRackInPosition(DbConnector& connector, const MapPosition& rackPosition) override;
    virtual bool hasLiftedRack(Entities::AgvPtr& agv) override;

    virtual void enableDirectionMarkers(bool value) override;

private:
    void updateDbTableNames();

    void loadCellsFromDb(DbConnector& connector);
    void loadAgvsFromDb(DbConnector& connector);
    void loadRacksFromDb(DbConnector& connector);

    void updateAgvsFromDb(DbConnector& connector);
    void updateRacksFromDb(DbConnector& connector);

    void createCellsDbTable(DbConnector& connector);
    void fillCellsDbTable(DbConnector& connector);

    void createAgvsDbTable(DbConnector& connector);
    void createRacksDbTable(DbConnector& connector);
    void createItemsDbTable(DbConnector& connector);
    void createIngressDbTable(DbConnector& connector);

    bool tryQueryWithoutResults(DbConnector& connector, const DbQuery& query);
    bool tryQueryAndStore(DbConnector& connector, const DbQuery& query, std::vector<DbRow>& vector);
};
