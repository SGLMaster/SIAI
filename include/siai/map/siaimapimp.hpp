#pragma once

#include "map/entities/entities.hpp"

#include "siaimap.hpp"

#include <memory>

class CommandStream;

class SIAIMapImp : public SIAIMap
{
private:
    int m_numberOfColumns{0};
    int m_numberOfRows{0};

    Entities::Container m_entities;

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

    virtual int getLastSelectedId() const noexcept;
    virtual MapPosition getLastSelectedPosition() const noexcept override;

private:
    void generateCells();
};
