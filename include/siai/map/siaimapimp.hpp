#pragma once

#include "siaimap.hpp"

#include <memory>
#include <vector>

class MapEntity;

class SIAIMapImp : public SIAIMap
{
private:
    int m_numberOfColumns{0};
    int m_numberOfRows{0};

    using EntityPointer = std::unique_ptr<MapEntity>;

    std::vector<EntityPointer> m_entities;

public:
    SIAIMapImp();
    virtual ~SIAIMapImp();

    virtual int getNumberOfColumns() const noexcept override;
    virtual int getNumberOfRows() const noexcept override;

    virtual void reset(int numberOfColumns, int numberOfRows) override;
    virtual void repaint(PainterContainer& painter) override;

    virtual void selectEntityUnderMouse(const PanelPoint& mousePosition) override;
    virtual void diselectAllEntities() override;

    virtual void placeBlockedCell(const PanelPoint& position) override;

private:
    virtual void eraseEntityUnderMouse(const PanelPoint& position);

    void fillEntitiesVectorWithCells();
};
