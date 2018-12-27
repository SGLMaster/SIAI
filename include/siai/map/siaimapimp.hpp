#pragma once

#include "siaimap.hpp"

#include <memory>
#include <vector>

class IMapEntity;
class ICell;
class IAgv;

class SIAIMapImp : public SIAIMap
{
private:
    int m_numberOfColumns{0};
    int m_numberOfRows{0};

    using EntityPointer = std::shared_ptr<IMapEntity>;
    std::vector<EntityPointer> m_entities;

    using CellPointer = std::shared_ptr<ICell>;
    std::vector<CellPointer> m_cells;

    using AgvPointer = std::shared_ptr<IAgv>;
    std::vector<AgvPointer> m_agvs;

public:
    SIAIMapImp();
    virtual ~SIAIMapImp();

    virtual int getNumberOfColumns() const noexcept override;
    virtual int getNumberOfRows() const noexcept override;

    virtual void reset(int numberOfColumns, int numberOfRows) override;
    virtual void repaint(Painter& painter) override;

    virtual void selectEntityWithPoint(const PanelPoint& point) override;
    virtual void diselectAllEntities() override;

    virtual int getLastSelectedId() const noexcept;
    virtual MapPosition getLastSelectedPosition() const noexcept override;

    virtual void replaceCell(const std::string& type, const PanelPoint& position) override;

private:
    bool selectOrDiselectIfHasPointInside(IMapEntity* entity, const PanelPoint& point) noexcept;

    void eraseEntityWithPoint(const PanelPoint& point);

    void fillCellsVector();
};
