#include "map/entities/agvimp.hpp"

#include "database/database.hpp"
#include "database/sqlquery.hpp"

#include "globals.hpp"

#include <mysql++.h>

AgvDefault::AgvDefault(int id, const MapPosition& position) : IAgv(id, position) {}
AgvDefault::~AgvDefault() = default;

void AgvDefault::draw(Painter& painter)
{
    calculateDrawingData(painter.getZoom());

    bool isVisible = isVisibleOnScreen(painter.getOrigin(), painter.getSize());

    if(isVisible)
    {
        painter.setBrush(PanelColor::RED);
        painter.drawRectangle(m_origin, m_size);
    }
}

bool AgvDefault::isVisibleOnScreen(const PanelPoint& panelOrigin, const PanelSize& panelSize) const noexcept
{
    int panelLimitX = panelOrigin.x + panelSize.width;
    int panelLimitY = panelOrigin.y + panelSize.height;

    int minValidCellOriginX = panelOrigin.x - m_size.width;
    int minValidCellOriginY = panelOrigin.y - m_size.height;

    return m_origin.x < panelLimitX && m_origin.y < panelLimitY
            && m_origin.x >= minValidCellOriginX && m_origin.y >= minValidCellOriginY;
}

bool AgvDefault::hasPointInside(const PanelPoint& point) const noexcept
{
    return point.x > m_origin.x && point.y > m_origin.y && point.x <= ( m_origin.x + m_size.width )
            && point.y <= ( m_origin.y + m_size.height );
}

void AgvDefault::calculateDrawingData(int zoom)
{
    calculateZoomedSize(zoom);
    calculateOrigin(zoom);
}

void AgvDefault::calculateZoomedSize(int zoom)
{
    int sideLength = SIAIGlobals::AGVS_DEFAULT_WIDTH_PX*zoom;

    m_size = PanelSize{sideLength, sideLength};
}

void AgvDefault::calculateOrigin(int zoom)
{
    int cellWidth = SIAIGlobals::CELLS_DEFAULT_WIDTH_PX*zoom;

    int offsetRelativeToCellOriginX = (cellWidth-m_size.width)/2;
    int offsetRelativeToCellOriginY = (cellWidth-m_size.height)/2;

    int originX = m_position.column*cellWidth + offsetRelativeToCellOriginX;
    int originY = m_position.row*cellWidth + offsetRelativeToCellOriginY;

    m_origin = PanelPoint{originX, originY};
}

void AgvDefault::insertToDatabase(DbConnector& connector, const std::string& mapName) const
{
	int direction = static_cast<int>(m_direction);

	std::vector<std::string> valuesToInsert{std::to_string(m_id), std::to_string(m_position.column),
		std::to_string(m_position.row), std::to_string(direction)};

	std::string tableName = SIAIGlobals::DB_AGVS_TABLE_PREFIX + mapName;

	SqlInsertQuery insertCellQuery(SqlQueryData{tableName, IAgv::dbColumnNames, valuesToInsert});

	connector.executeQueryWithoutResults(insertCellQuery);
}

void AgvDefault::updateInDatabase(DbConnector& connector, const std::string& mapName) const
{
	int direction = static_cast<int>(m_direction);

	std::vector<std::string> valuesToUpdate{std::to_string(m_id), std::to_string(m_position.column),
		std::to_string(m_position.row), std::to_string(direction)};

	std::string tableName = SIAIGlobals::DB_AGVS_TABLE_PREFIX + mapName;

	SqlQueryData dataToUpdate{tableName, IAgv::dbColumnNames, valuesToUpdate};
	SqlWhereCondition whereCondition( SqlQueryData{tableName, {"id"}, {std::to_string(m_id)} } );

	SqlUpdateQuery updateQuery(dataToUpdate, whereCondition.generateString());

	connector.executeQueryWithoutResults(updateQuery);
}

void AgvDefault::loadFromDatabase(DbConnector& connector)
{
	;
}

bool AgvDefault::assignTask(DbConnector& connector, const std::string& mapName, std::shared_ptr<MapTask> newTask)
{
	m_currentTask.reset();
	m_currentTask = std::move(newTask);

	if(m_currentTask->isIngressTask())
	{
		bool taskAssigned = updateIngressTaskAssignmentInDb(connector, mapName, m_id);

		// If we couldn't save to Db we erase the task
		if(!taskAssigned)
		{
			m_currentTask.reset();
			return false;
		}
		else
		{
			return true;
		}
	}

	m_currentTask.reset();
    return true;
}

bool AgvDefault::dropTask(DbConnector& connector, const std::string& mapName)
{
	if(!m_currentTask)
		return true;

	if(m_currentTask->isIngressTask())
	{
		//We update with a value of 0 which means "unassigned"
		bool taskDropped = updateIngressTaskAssignmentInDb(connector, mapName, 0);
	
		// If we were able to drop the task on the db, now we drop it locally
		if(taskDropped)
			m_currentTask.reset();
		else
			return false;
	}

	return true;
}

bool AgvDefault::updateIngressTaskAssignmentInDb(DbConnector& connector, const std::string& mapName, 
												int valueToAssign) const
{
	std::string tableName = SIAIGlobals::DB_INGRESS_TABLE_PREFIX + mapName;

	int taskId = m_currentTask->getId();

	SqlQueryData dataToUpdate{ tableName, {"agvid"}, { std::to_string(valueToAssign) } };
	SqlWhereCondition whereCondition(SqlQueryData{tableName, {"id"}, {std::to_string(taskId)}});
	SqlUpdateQuery updateQuery(dataToUpdate, whereCondition.generateString());

	try
	{
		connector.executeQueryWithoutResults(updateQuery);
	}
	catch (const mysqlpp::Exception &e)
	{
		return false;
	}

	return true;
}

int AgvDefault::getLiftedRackId() const noexcept
{
	if(m_currentTask)
	{
		return m_currentTask->getRackId();
	}

	return -1;
}

std::unique_ptr<PanelImage> RegularAgv::m_regularImage = nullptr;
std::unique_ptr<PanelImage> RegularAgv::m_selectedImage = nullptr;

RegularAgv::RegularAgv(int id, const MapPosition& position) : AgvDefault(id, position)
{
	m_regularImage = PanelImage::create("resources/map/regular-agv.png");
	m_selectedImage = PanelImage::create("resources/map/regular-agv-selected.png");
}
RegularAgv::~RegularAgv() = default;

void RegularAgv::draw(Painter& painter)
{
    calculateDrawingData(painter.getZoom());

    bool isVisible = isVisibleOnScreen(painter.getOrigin(), painter.getSize());

    if(isVisible)
    {
        doDrawImage(painter);
    }
}

void RegularAgv::doDrawImage(Painter& painter)
{
	double rotation = 0.0;

	switch(m_direction)
	{
	case MapDirection::RIGHT:
		break;
	case MapDirection::DOWN:
		rotation = Painter::ROTATION_270;
		break;
	case MapDirection::LEFT:
		rotation = Painter::ROTATION_180;
		break;
	case MapDirection::UP:
		rotation = Painter::ROTATION_90;
		break;
	case MapDirection::INVALID:
		break;
	}

	if(!m_selected)
		painter.drawImageRotatedAroundCenter(*m_regularImage, m_origin, m_size, rotation);
	else
		painter.drawImageRotatedAroundCenter(*m_selectedImage, m_origin, m_size, rotation);
}
