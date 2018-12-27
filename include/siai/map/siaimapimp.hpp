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

    virtual int getNumberOfColumns() const noexcept override;
    virtual int getNumberOfRows() const noexcept override;

    virtual void reset(int numberOfColumns, int numberOfRows) override;
    virtual void repaint(Painter& painter) override;

    virtual void selectEntity(const PanelPoint& point) override;
    virtual void diselectAllEntities() override;

    virtual int getLastSelectedId() const noexcept;
    virtual MapPosition getLastSelectedPosition() const noexcept override;

    virtual void replaceCell(const std::string& type, const PanelPoint& position) override;

private:
    bool selectOrDiselectIfHasPointInside(IMapEntity* entity, const PanelPoint& point) noexcept;

    void eraseEntity(const PanelPoint& point);

    EntityIterator findCellIteratorWithPoint(const PanelPoint& point);
    void createCopyWithDifferentTypeAndEraseOriginal(EntityIterator& entityIterator, const std::string& type);

    void sortEntitiesByDrawOrder();

    void generateCells();
};
