#pragma once

#include "siaimap.hpp"

#include <memory>
#include <vector>

class IMapEntity;
class ICell;
class IAgv;

using EntityPointer = std::unique_ptr<IMapEntity>;
using EntityIterator = std::vector<EntityPointer>::iterator;

class SIAIMapImp : public SIAIMap
{
private:
    int m_numberOfColumns{0};
    int m_numberOfRows{0};

    std::vector<EntityPointer> m_entities;

public:
    SIAIMapImp();
    virtual ~SIAIMapImp();

    virtual void testDrawAll(Painter& painter) override;

    virtual void reset(int numberOfColumns, int numberOfRows) override;
    virtual void repaint(Painter& painter) override;

    virtual int getNumberOfColumns() const noexcept override;
    virtual int getNumberOfRows() const noexcept override;

    virtual int getLastSelectedId() const noexcept;
    virtual MapPosition getLastSelectedPosition() const noexcept override;

    virtual void selectEntity(const PanelPoint& point) override;
    virtual void diselectAllEntities() override;

    virtual void replaceCell(const std::string& type, const PanelPoint& position) override;

private:
    bool selectOrDiselectIfHasPointInside(IMapEntity* entity, const PanelPoint& point) noexcept;

    void createCopyWithDifferentTypeAndEraseOriginal(EntityIterator& entityIterator, const std::string& type);
    EntityIterator findCellIteratorWithPoint(const PanelPoint& point);

    bool isCellOccupied(const MapPosition& position) const;

    void sortEntitiesByDrawOrder();

    void generateCells();
};
