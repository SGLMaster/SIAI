#pragma once

#include "map/entities/cell.hpp"
#include "painter/painter.hpp"

#include "util/id.hpp"

class CellDefault : public ICell
{
protected:
    PanelPoint m_origin;
    int m_sideLength;

public:
    CellDefault() = delete;
    CellDefault(int id, const MapPosition& position);
    virtual ~CellDefault();

    virtual void draw(Painter& painter) override;

    virtual bool isVisibleOnScreen(const PanelPoint& panelOrigin, const PanelSize& panelSize) const noexcept override;
    virtual bool hasPointInside(const PanelPoint& point) const noexcept override;

    virtual void saveToDatabase(DbConnector& connector, const std::string& tableName) const override;
    virtual void loadFromDatabase(DbConnector& connector) override;

private:
    void calculateZoomedSideLength(int zoom);
    void calculateOrigin();

    void setDifferentPenIfSelected(Painter& painter);
};

class RegularCell : public CellDefault
{
public:
    RegularCell() = delete;
    RegularCell(int id, const MapPosition& position);
    virtual ~RegularCell();

    virtual std::string getEntityName() const noexcept override
    {
        return "RegularCell";
    }

    virtual void draw(Painter& painter) override;

};

class BlockedCell : public CellDefault
{
public:
    BlockedCell() = delete;
    BlockedCell(int id, const MapPosition& position);
    virtual ~BlockedCell();

    virtual std::string getEntityName() const noexcept override
    {
        return "BlockedCell";
    }

    virtual void draw(Painter& painter) override;
};
