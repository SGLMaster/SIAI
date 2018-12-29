#pragma once

#include "map/entities.hpp"

#include "siaimap.hpp"

#include <memory>

class ICell;
class IAgv;

class MapCommand;

class SIAIMapImp : public SIAIMap
{
private:
    int m_numberOfColumns{0};
    int m_numberOfRows{0};

    Entities::Container m_entities;

    using CommandStream = std::vector<std::unique_ptr<MapCommand>>;
    CommandStream m_commandStream;

public:
    SIAIMapImp();
    virtual ~SIAIMapImp();

    virtual void executeCommand(const std::string& command) override;
    virtual void undoLastCommand() override;

    virtual void reset(int numberOfColumns, int numberOfRows) override;
    virtual void repaint(Painter& painter) override;

    virtual int getNumberOfColumns() const noexcept override;
    virtual int getNumberOfRows() const noexcept override;

    virtual int getLastSelectedId() const noexcept;
    virtual MapPosition getLastSelectedPosition() const noexcept override;

private:
    void generateCells();
};
