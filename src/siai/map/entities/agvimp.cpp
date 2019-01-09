#include "map/entities/agvimp.hpp"

#include "database/database.hpp"
#include "database/sqlquery.hpp"

#include "globals.hpp"

AgvDefault::AgvDefault(const MapPosition& position) : IAgv(position) {}
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

void AgvDefault::saveToDatabase(DbConnector& connector, const std::string& mapName) const
{
	int direction = static_cast<int>(m_direction);

	std::vector<std::string> valuesToSave{std::to_string(m_id), std::to_string(m_position.column),
		std::to_string(m_position.row), std::to_string(direction)};

	std::string tableName = SIAIGlobals::DB_AGVS_TABLE_PREFIX + mapName;

	tryToInsertToDb(connector, tableName, valuesToSave);
}

void AgvDefault::loadFromDatabase(DbConnector& connector)
{

}

void AgvDefault::tryToInsertToDb(DbConnector& connector, const std::string& tableName,
		const std::vector<std::string>& valuesToInsert) const
{
	SqlInsertQuery insertCellQuery(SqlQueryData{tableName, IAgv::dbColumnNames, valuesToInsert});

	try
	{
		connector.executeQueryWithoutResults(insertCellQuery);
	}
	catch(const std::exception& e)
	{
		updateOnDatabase(connector, tableName, valuesToInsert);
	}
}

void AgvDefault::updateOnDatabase(DbConnector& connector, const std::string& tableName,
		const std::vector<std::string>& valuesToUpdate) const
{
	SqlQueryData dataForUpdate{tableName, IAgv::dbColumnNames, valuesToUpdate};
	SqlWhereCondition whereCondition( SqlQueryData{tableName, {"id"}, {std::to_string(m_id)} } );

	SqlUpdateQuery updateQuery(dataForUpdate, whereCondition.generateString());

	connector.executeQueryWithoutResults(updateQuery);
}

RegularAgv::RegularAgv(const MapPosition& position) : AgvDefault(position){}
RegularAgv::~RegularAgv() = default;

void RegularAgv::draw(Painter& painter)
{
    calculateDrawingData(painter.getZoom());

    bool isVisible = isVisibleOnScreen(painter.getOrigin(), painter.getSize());

    if(isVisible)
    {
        if(!m_selected)
        {
        	drawNormalImage(painter);
        }
        else
        {
            drawSelectedImage(painter);
        }
    }
}

void RegularAgv::drawNormalImage(Painter& painter)
{
	switch(m_direction)
	{
	case MapDirection::RIGHT:
	{
		static auto normalImage = PanelImage::create("resources/map/regular-agv-right.png");
		painter.drawImage(*normalImage, m_origin, m_size);
		break;
	}
	case MapDirection::DOWN:
	{
		static auto normalImage = PanelImage::create("resources/map/regular-agv-down.png");
		painter.drawImage(*normalImage, m_origin, m_size);
		break;
	}
	case MapDirection::LEFT:
	{
		static auto normalImage = PanelImage::create("resources/map/regular-agv-left.png");
		painter.drawImage(*normalImage, m_origin, m_size);
		break;
	}
	case MapDirection::UP:
	{
		static auto normalImage = PanelImage::create("resources/map/regular-agv-up.png");
		painter.drawImage(*normalImage, m_origin, m_size);
		break;
	}
	}
}

void RegularAgv::drawSelectedImage(Painter& painter)
{
	switch(m_direction)
	{
	case MapDirection::RIGHT:
	{
		static auto selectedImage = PanelImage::create("resources/map/regular-agv-right-selected.png");
		painter.drawImage(*selectedImage, m_origin, m_size);
		break;
	}
	case MapDirection::DOWN:
	{
		static auto selectedImage = PanelImage::create("resources/map/regular-agv-down-selected.png");
		painter.drawImage(*selectedImage, m_origin, m_size);
		break;
	}
	case MapDirection::LEFT:
	{
		static auto selectedImage = PanelImage::create("resources/map/regular-agv-left-selected.png");
		painter.drawImage(*selectedImage, m_origin, m_size);
		break;
	}
	case MapDirection::UP:
	{
		static auto selectedImage = PanelImage::create("resources/map/regular-agv-up-selected.png");
		painter.drawImage(*selectedImage, m_origin, m_size);
		break;
	}
	}
}
