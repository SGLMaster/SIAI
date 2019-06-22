#pragma once

#include "map/entities/agv.hpp"
#include "painter/painter.hpp"

#include <memory>
#include <string>

class PanelImage;

class AgvDefault : public IAgv
{
protected:
    PanelPoint m_origin;
    PanelSize m_size;

    std::shared_ptr<MapTask> m_currentTask;

public:
    AgvDefault() = delete;
    AgvDefault(int id, const MapPosition& position);
    virtual ~AgvDefault();

    virtual void draw(Painter& painter) override;

    virtual bool isVisibleOnScreen(const PanelPoint& panelOrigin, const PanelSize& panelSize) const noexcept override;
    virtual bool hasPointInside(const PanelPoint& point) const noexcept override;

    virtual void insertToDatabase(DbConnector& connector, const std::string& mapName) const override;
    virtual void updateInDatabase(DbConnector& connector, const std::string& mapName) const override;
    virtual void loadFromDatabase(DbConnector& connector) override;

    virtual bool assignTask(DbConnector& connector, const std::string& mapName, 
                            std::shared_ptr<MapTask> newTask) override;
    virtual bool dropTask(DbConnector& connector, const std::string& mapName) override;
    virtual void updateTaskPath(const MapPath& newPath) noexcept override
    {
        m_currentTask->setPath(newPath);
    }

    virtual bool isAtDestination() const noexcept
    {
        if(m_currentTask)
        {
            MapPosition destination = m_currentTask->getDestination();

            return (m_position.column == destination.column && m_position.row == destination.row);
        }

        return false;
    }
    virtual MapDirection getNextDirection() const noexcept override
    {
	    if(m_currentTask)
		    return m_currentTask->getNextDirection(m_position);

        return MapDirection::INVALID;
    }
    
    virtual int getLiftedRackId() const noexcept override;

protected:
    void calculateDrawingData(int zoom);
    void calculateZoomedSize(int zoom);
    void calculateOrigin(int zoom);

private:
    bool updateIngressTaskAssignmentInDb(DbConnector& connector,  const std::string& mapName, int valueToAssign) const;
};

class RegularAgv : public AgvDefault
{
private:
    static std::unique_ptr<PanelImage> m_regularImage;
    static std::unique_ptr<PanelImage> m_selectedImage;

public:
    RegularAgv() = delete;
    RegularAgv(int id, const MapPosition& position);
    virtual ~RegularAgv();

    virtual std::string getEntityName() const noexcept override
    {
        return "RegularAgv";
    }

    virtual void draw(Painter& painter) override;

private:
    void doDrawImage(Painter& painter);
};
