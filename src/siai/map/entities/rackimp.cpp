#include "map/entities/rackimp.hpp"

#include "database/database.hpp"
#include "database/sqlquery.hpp"

#include "globals.hpp"

RegularRack::RegularRack(int id, const MapPosition& position) : IRack(id, position){}
RegularRack::~RegularRack() = default;

void RegularRack::draw(Painter& painter)
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

bool RegularRack::isVisibleOnScreen(const PanelPoint& panelOrigin, const PanelSize& panelSize) const noexcept
{
	int panelLimitX = panelOrigin.x + panelSize.width;
    int panelLimitY = panelOrigin.y + panelSize.height;

    int minValidCellOriginX = panelOrigin.x - m_radio;
    int minValidCellOriginY = panelOrigin.y - m_radio;

    return m_cellOrigin.x < panelLimitX && m_cellOrigin.y < panelLimitY && m_cellOrigin.x >= minValidCellOriginX 
						&& m_cellOrigin.y >= minValidCellOriginY;
}

bool RegularRack::hasPointInside(const PanelPoint& point) const noexcept
{
	return point.x > m_cellOrigin.x && point.y > m_cellOrigin.y && point.x <= ( m_cellOrigin.x + m_radio )
            && point.y <= ( m_cellOrigin.y + m_radio );
}

void RegularRack::insertToDatabase(DbConnector& connector, const std::string& mapName) const
{
	int direction = static_cast<int>(m_direction);
	int liftedValue = static_cast<int>(m_lifted);

	std::vector<std::string> valuesToInsert{std::to_string(m_id), std::to_string(m_position.column),
		std::to_string(m_position.row), std::to_string(direction), std::to_string(liftedValue)};

	std::string tableName = SIAIGlobals::DB_RACKS_TABLE_PREFIX + mapName;

	SqlInsertQuery insertRackQuery(SqlQueryData{tableName, IRack::dbColumnNames, valuesToInsert});

	connector.executeQueryWithoutResults(insertRackQuery);
}

void RegularRack::updateInDatabase(DbConnector& connector, const std::string& mapName) const
{
	int direction = static_cast<int>(m_direction);
	int liftedValue = static_cast<int>(m_lifted);

	std::vector<std::string> valuesToUpdate{std::to_string(m_id), std::to_string(m_position.column),
		std::to_string(m_position.row), std::to_string(direction), std::to_string(liftedValue)};

	std::string tableName = SIAIGlobals::DB_RACKS_TABLE_PREFIX + mapName;

	SqlQueryData dataToUpdate{tableName, IRack::dbColumnNames, valuesToUpdate};
	SqlWhereCondition whereCondition( SqlQueryData{tableName, {"id"}, {std::to_string(m_id)} } );

	SqlUpdateQuery updateQuery(dataToUpdate, whereCondition.generateString());

	connector.executeQueryWithoutResults(updateQuery);
}

void RegularRack::loadFromDatabase(DbConnector& connector)
{
	;
}

void RegularRack::calculateDrawingData(int zoom)
{
    calculateZoomedRadio(zoom);
    calculateCellOrigin(zoom);
	calculateCenter(zoom);
}

void RegularRack::calculateZoomedRadio(int zoom)
{
    m_radio = SIAIGlobals::RACK_DEFAULT_LENGTH*zoom;
}

void RegularRack::calculateCellOrigin(int zoom)
{
    int cellWidth = SIAIGlobals::CELLS_DEFAULT_WIDTH_PX*zoom;

    m_cellOrigin = PanelPoint{m_position.column*cellWidth, m_position.row*cellWidth};
}

void RegularRack::calculateCenter(int zoom)
{
    int cellWidth = SIAIGlobals::CELLS_DEFAULT_WIDTH_PX*zoom;

    int centerX = m_position.column*cellWidth + (cellWidth/2);
    int centerY = m_position.row*cellWidth + (cellWidth/2);

    m_center = PanelPoint{centerX, centerY};
}

void RegularRack::drawNormalImage(Painter& painter)
{
	PanelSize rackSize{m_radio, m_radio};

	static auto regularImage = PanelImage::create("resources/map/regular-rack.png");
	static auto liftedImage = PanelImage::create("resources/map/regular-rack-lifted.png");

	if(m_lifted)
		painter.drawImage(*liftedImage, m_cellOrigin, rackSize);
	else
		painter.drawImage(*regularImage, m_cellOrigin, rackSize);
}

void RegularRack::drawSelectedImage(Painter& painter)
{
	PanelSize rackSize{m_radio, m_radio};

	static auto selectedImage = PanelImage::create("resources/map/regular-rack-selected.png");
	painter.drawImage(*selectedImage, m_cellOrigin, rackSize);
}