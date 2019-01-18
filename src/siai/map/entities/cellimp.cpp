#include "map/entities/cellimp.hpp"

#include "database/database.hpp"
#include "database/sqlquery.hpp"

#include "globals.hpp"

CellDefault::CellDefault(int id, const MapPosition& position) : ICell(id, position), m_sideLength{0} {}
CellDefault::~CellDefault() = default;

void CellDefault::draw(Painter& painter)
{
    calculateZoomedSideLength(painter.getZoom());
    calculateOrigin();

    bool isVisible = isVisibleOnScreen(painter.getOrigin(), painter.getSize());

    if(isVisible)
    {
        setDifferentPenIfSelected(painter);

        painter.drawRectangle(m_origin, PanelSize{m_sideLength, m_sideLength});
    }
}

bool CellDefault::isVisibleOnScreen(const PanelPoint& panelOrigin, const PanelSize& panelSize) const noexcept
{
    int panelLimitX = panelOrigin.x + panelSize.width;
    int panelLimitY = panelOrigin.y + panelSize.height;

    int minValidCellOriginX = panelOrigin.x - m_sideLength;
    int minValidCellOriginY = panelOrigin.y - m_sideLength;

    return m_origin.x < panelLimitX && m_origin.y < panelLimitY && m_origin.x >= minValidCellOriginX
    		&& m_origin.y >= minValidCellOriginY;
}

bool CellDefault::hasPointInside(const PanelPoint& point) const noexcept
{
    return point.x > m_origin.x && point.y > m_origin.y && point.x <= ( m_origin.x + m_sideLength )
            && point.y <= ( m_origin.y + m_sideLength );
}

void CellDefault::calculateZoomedSideLength(int zoom)
{
    m_sideLength = SIAIGlobals::CELLS_DEFAULT_WIDTH_PX*zoom;
}

void CellDefault::calculateOrigin()
{
    m_origin = PanelPoint{m_position.column*m_sideLength, m_position.row*m_sideLength};
}

void CellDefault::setDifferentPenIfSelected(Painter& painter)
{
    if(!m_selected)
    {
        painter.setPen(PanelColor::BLACK);
    }
    else
    {
        painter.setPen(PanelColor::SELECTED);
    }
}

void CellDefault::saveToDatabase(DbConnector& connector, const std::string& mapName) const
{
	int direction = static_cast<int>(m_direction);

	std::vector<std::string> valuesToSave{std::to_string(m_id), std::to_string(m_position.column),
		std::to_string(m_position.row), std::to_string(direction), getEntityName()};

	std::string tableName = SIAIGlobals::DB_CELLS_TABLE_PREFIX + mapName;

	SqlQueryData dataForUpdate{tableName, ICell::dbColumnNames, valuesToSave};
	SqlWhereCondition whereCondition( SqlQueryData{tableName, {"id"}, {std::to_string(m_id)} } );

	SqlUpdateQuery updateQuery(dataForUpdate, whereCondition.generateString());

	connector.executeQueryWithoutResults(updateQuery);
}

void CellDefault::loadFromDatabase(DbConnector& connector)
{

}

RegularCell::RegularCell(int id, const MapPosition& position) : CellDefault(id, position) {}
RegularCell::~RegularCell() = default;

void RegularCell::draw(Painter& painter)
{
    painter.setBrush(PanelColor::WHITE);

    CellDefault::draw(painter);
}

BlockedCell::BlockedCell(int id, const MapPosition& position) : CellDefault(id, position) {}
BlockedCell::~BlockedCell() = default;

void BlockedCell::draw(Painter& painter)
{
	calculateZoomedSideLength(painter.getZoom());
	calculateOrigin();

	bool isVisible = isVisibleOnScreen(painter.getOrigin(), painter.getSize());

	if(isVisible)
	{
		if(!m_selected)
		{
			static auto normalImage = PanelImage::create("resources/map/blocked-cell.png");
			painter.drawImage(*normalImage, m_origin, PanelSize{m_sideLength, m_sideLength});
		}
		else
		{
			static auto normalImage = PanelImage::create("resources/map/blocked-cell-selected.png");
			painter.drawImage(*normalImage, m_origin, PanelSize{m_sideLength, m_sideLength});
		}
	}
}
