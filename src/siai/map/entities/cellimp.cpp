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

void CellDefault::drawDirectionMarker(Painter& painter)
{
	bool isVisible = isVisibleOnScreen(painter.getOrigin(), painter.getSize());

	if(isVisible)
	{
		switch(m_direction)
		{
		case MapDirection::RIGHT:
		{
			static auto normalImage = PanelImage::create("resources/map/direction-marker-right.png");
			painter.drawImage(*normalImage, m_origin, PanelSize{m_sideLength, m_sideLength});
			break;
		}
		case MapDirection::DOWN:
		{
			static auto normalImage = PanelImage::create("resources/map/direction-marker-down.png");
			painter.drawImage(*normalImage, m_origin, PanelSize{m_sideLength, m_sideLength});
			break;
		}
		case MapDirection::LEFT:
		{
			static auto normalImage = PanelImage::create("resources/map/direction-marker-left.png");
			painter.drawImage(*normalImage, m_origin, PanelSize{m_sideLength, m_sideLength});
			break;
		}
		case MapDirection::UP:
		{
			static auto normalImage = PanelImage::create("resources/map/direction-marker-up.png");
			painter.drawImage(*normalImage, m_origin, PanelSize{m_sideLength, m_sideLength});
			break;
		}
		case MapDirection::INVALID:
			break;
		}
	}
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

void CellDefault::insertToDatabase(DbConnector& connector, const std::string& mapName) const
{
	;
}

void CellDefault::updateInDatabase(DbConnector& connector, const std::string& mapName) const
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

	drawDirectionMarker(painter);
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

ParkingCell::ParkingCell(int id, const MapPosition& position) : CellDefault(id, position) {}
ParkingCell::~ParkingCell() = default;

void ParkingCell::draw(Painter& painter)
{
	calculateZoomedSideLength(painter.getZoom());
	calculateOrigin();

	bool isVisible = isVisibleOnScreen(painter.getOrigin(), painter.getSize());

	if(isVisible)
	{
		if(!m_selected)
		{
			static auto normalImage = PanelImage::create("resources/map/parking-cell.png");
			painter.drawImage(*normalImage, m_origin, PanelSize{m_sideLength, m_sideLength});
		}
		else
		{
			static auto normalImage = PanelImage::create("resources/map/parking-cell-selected.png");
			painter.drawImage(*normalImage, m_origin, PanelSize{m_sideLength, m_sideLength});
		}
	}

	drawDirectionMarker(painter);
}

StorageCell::StorageCell(int id, const MapPosition& position) : CellDefault(id, position) {}
StorageCell::~StorageCell() = default;

void StorageCell::draw(Painter& painter)
{
	calculateZoomedSideLength(painter.getZoom());
	calculateOrigin();

	bool isVisible = isVisibleOnScreen(painter.getOrigin(), painter.getSize());

	if(isVisible)
	{
		if(!m_selected)
		{
			static auto normalImage = PanelImage::create("resources/map/storage-cell.png");
			painter.drawImage(*normalImage, m_origin, PanelSize{m_sideLength, m_sideLength});
		}
		else
		{
			static auto normalImage = PanelImage::create("resources/map/storage-cell-selected.png");
			painter.drawImage(*normalImage, m_origin, PanelSize{m_sideLength, m_sideLength});
		}
	}

	drawDirectionMarker(painter);
}

IngressCell::IngressCell(int id, const MapPosition& position) : CellDefault(id, position) {}
IngressCell::~IngressCell() = default;

void IngressCell::draw(Painter& painter)
{
	calculateZoomedSideLength(painter.getZoom());
	calculateOrigin();

	bool isVisible = isVisibleOnScreen(painter.getOrigin(), painter.getSize());

	if(isVisible)
	{
		if(!m_selected)
		{
			static auto normalImage = PanelImage::create("resources/map/ingress-cell.png");
			painter.drawImage(*normalImage, m_origin, PanelSize{m_sideLength, m_sideLength});
		}
		else
		{
			static auto normalImage = PanelImage::create("resources/map/ingress-cell-selected.png");
			painter.drawImage(*normalImage, m_origin, PanelSize{m_sideLength, m_sideLength});
		}
	}

	drawDirectionMarker(painter);
}