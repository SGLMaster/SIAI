#pragma once

#include "map/entities/entities.hpp"

#include "map/siaimap.hpp"

#include <memory>

class CommandStream;

class SIAIMapImp : public SIAIMap
{
private:
    int m_numberOfColumns{0};
    int m_numberOfRows{0};

    Entities::Container m_entities;
    Entities::Container m_selectedEntities;

    std::unique_ptr<CommandStream> m_commandStream;

public:
    SIAIMapImp();
    virtual ~SIAIMapImp();

    virtual void executeCommand(const std::string& command) override;
    virtual void undo() override;
    virtual void redo() override;

    virtual void reset(int numberOfColumns, int numberOfRows) override;
    virtual void repaint(Painter& painter) override;

    virtual int getNumberOfColumns() const noexcept override;
    virtual int getNumberOfRows() const noexcept override;

    virtual void selectEntity(const PanelPoint& point) override;
    virtual void diselectAll() override;

    virtual int getSelectedEntityColumn() const noexcept override;
    virtual int getSelectedEntityRow() const noexcept override;
    virtual int getSelectedId() const noexcept;

private:
    void generateCells();
};
