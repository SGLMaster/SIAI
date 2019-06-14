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

    virtual void insertToDatabase(DbConnector& connector, const std::string& mapName) const override;
    virtual void updateInDatabase(DbConnector& connector, const std::string& mapName) const override;
    virtual void loadFromDatabase(DbConnector& connector) override;

protected:
    virtual void drawDirectionMarker(Painter& painter);

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
        return "Regular";
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
        return "Blocked";
    }

    virtual void draw(Painter& painter) override;
};

class ParkingCell : public CellDefault
{
public:
    ParkingCell() = delete;
    ParkingCell(int id, const MapPosition& position);
    virtual ~ParkingCell();

    virtual std::string getEntityName() const noexcept override
    {
        return "Parking";
    }

    virtual void draw(Painter& painter) override;
};

class StorageCell : public CellDefault
{
public:
    StorageCell() = delete;
    StorageCell(int id, const MapPosition& position);
    virtual ~StorageCell();

    virtual std::string getEntityName() const noexcept override
    {
        return "Storage";
    }

    virtual void draw(Painter& painter) override;
};

class IngressCell : public CellDefault
{
public:
    IngressCell() = delete;
    IngressCell(int id, const MapPosition& position);
    virtual ~IngressCell();

    virtual std::string getEntityName() const noexcept override
    {
        return "Ingress";
    }

    virtual void draw(Painter& painter) override;
};
