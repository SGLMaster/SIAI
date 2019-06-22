#pragma once

#include "map/entities/cell.hpp"
#include "painter/painter.hpp"

#include "util/id.hpp"

class CellDefault : public ICell
{
protected:
    PanelPoint m_origin;
    int m_sideLength;

    static std::unique_ptr<PanelImage> m_directionMarkerImage;

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
    void loadImages();
    void drawWithImages(Painter& painter, const std::unique_ptr<PanelImage>& normalImg, 
                                const std::unique_ptr<PanelImage>& selectedImg);

    void drawDirectionMarker(Painter& painter) const noexcept;
    void doDrawDirectionMarker(Painter& painter) const noexcept;

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
private:
    static std::unique_ptr<PanelImage> m_normalImage;
    static std::unique_ptr<PanelImage> m_selectedImage;

public:
    BlockedCell() = delete;
    BlockedCell(int id, const MapPosition& position);
    virtual ~BlockedCell();

    virtual std::string getEntityName() const noexcept override
    {
        return "Blocked";
    }

    virtual void draw(Painter& painter) override;

private:
    void loadImages();
};

class ParkingCell : public CellDefault
{
private:
    static std::unique_ptr<PanelImage> m_normalImage;
    static std::unique_ptr<PanelImage> m_selectedImage;

public:
    ParkingCell() = delete;
    ParkingCell(int id, const MapPosition& position);
    virtual ~ParkingCell();

    virtual std::string getEntityName() const noexcept override
    {
        return "Parking";
    }

    virtual void draw(Painter& painter) override;

private:
    void loadImages();
};

class StorageCell : public CellDefault
{
private:
    static std::unique_ptr<PanelImage> m_normalImage;
    static std::unique_ptr<PanelImage> m_selectedImage;

public:
    StorageCell() = delete;
    StorageCell(int id, const MapPosition& position);
    virtual ~StorageCell();

    virtual std::string getEntityName() const noexcept override
    {
        return "Storage";
    }

    virtual void draw(Painter& painter) override;

private:
    void loadImages();
};

class IngressCell : public CellDefault
{
private:
    static std::unique_ptr<PanelImage> m_normalImage;
    static std::unique_ptr<PanelImage> m_selectedImage;

public:
    IngressCell() = delete;
    IngressCell(int id, const MapPosition& position);
    virtual ~IngressCell();

    virtual std::string getEntityName() const noexcept override
    {
        return "Ingress";
    }

    virtual void draw(Painter& painter) override;

private:
    void loadImages();
};
