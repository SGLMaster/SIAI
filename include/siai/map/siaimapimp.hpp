#pragma once

#include "siaimap.hpp"

#include <memory>
#include <vector>

class ICell;

class SIAIMapImp : public SIAIMap
{
private:
    int m_numberOfColumns{0};
    int m_numberOfRows{0};

    using CellPointer = std::unique_ptr<ICell>;
    std::vector<CellPointer> m_cells;

public:
    SIAIMapImp();
    virtual ~SIAIMapImp();

    virtual int getNumberOfColumns() const noexcept override;
    virtual int getNumberOfRows() const noexcept override;

    virtual void reset(int numberOfColumns, int numberOfRows) override;
    virtual void repaint(Painter& painter) override;

    virtual void selectEntityWithPoint(const PanelPoint& point) override;
    virtual void diselectAllEntities() override;

    virtual void replaceCell(const std::string& type, const PanelPoint& position) override;

private:
    virtual void eraseEntityWithPoint(const PanelPoint& point);

    void fillEntitiesVectorWithCells();
};
