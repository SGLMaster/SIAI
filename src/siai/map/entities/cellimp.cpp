#include "map/entities/cellimp.hpp"

#include "database/database.hpp"
#include "database/sqlquery.hpp"

#include "globals.hpp"

std::unique_ptr<PanelImage> CellDefault::m_directionMarkerImage = nullptr;

CellDefault::CellDefault(int id, const MapPosition& position) : ICell(id, position), m_sideLength{0} {}
CellDefault::~CellDefault() = default;

void CellDefault::draw(Painter& painter)
{
    calculateZoomedSideLength(painter.getZoom());
    calculateOrigin();

    bool isVisible = isVisibleOnScreen(painter.getOrigin(), painter.getSize());

    if(isVisible)
    {
		loadImages();
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

void CellDefault::drawWithImages(Painter& painter, const std::unique_ptr<PanelImage>& normalImg, 
                            		const std::unique_ptr<PanelImage>& selectedImg)
{
	calculateZoomedSideLength(painter.getZoom());
	calculateOrigin();

	bool isVisible = isVisibleOnScreen(painter.getOrigin(), painter.getSize());

	if(isVisible)
	{
		if(!m_selected)
			painter.drawImage(*normalImg, m_origin, PanelSize{m_sideLength, m_sideLength});
		else
			painter.drawImage(*selectedImg, m_origin, PanelSize{m_sideLength, m_sideLength});
	}
}

void CellDefault::drawDirectionMarker(Painter& painter) const noexcept
{
	if(!ICell::isDirectionMarkersEnabled())
		return;

	bool isVisible = isVisibleOnScreen(painter.getOrigin(), painter.getSize());

	if(isVisible)
	{
		doDrawDirectionMarker(painter);
	}
}

void CellDefault::doDrawDirectionMarker(Painter& painter) const noexcept
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

	painter.drawImageRotatedAroundCenter(*m_directionMarkerImage, m_origin, PanelSize{m_sideLength, m_sideLength}, 
											rotation);
}

void CellDefault::loadImages()
{
	//Load the images if they aren't loaded already
	if(!m_directionMarkerImage)
	{
		m_directionMarkerImage = PanelImage::create("resources/map/direction-marker.png");
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

std::unique_ptr<PanelImage> BlockedCell::m_normalImage = nullptr;
std::unique_ptr<PanelImage> BlockedCell::m_selectedImage = nullptr;

BlockedCell::BlockedCell(int id, const MapPosition& position) : CellDefault(id, position) {}
BlockedCell::~BlockedCell() = default;

void BlockedCell::draw(Painter& painter)
{
	loadImages();
	CellDefault::drawWithImages(painter, m_normalImage, m_selectedImage);
}

void BlockedCell::loadImages()
{
	//Load the images if they aren't loaded already
	if(!m_normalImage || !m_selectedImage)
	{
		m_normalImage = PanelImage::create("resources/map/blocked-cell.png");
		m_selectedImage = PanelImage::create("resources/map/blocked-cell-selected.png");
	}
}

std::unique_ptr<PanelImage> ParkingCell::m_normalImage = nullptr;
std::unique_ptr<PanelImage> ParkingCell::m_selectedImage = nullptr;

ParkingCell::ParkingCell(int id, const MapPosition& position) : CellDefault(id, position) {}
ParkingCell::~ParkingCell() = default;

void ParkingCell::draw(Painter& painter)
{
	loadImages();
	CellDefault::drawWithImages(painter, m_normalImage, m_selectedImage);
	drawDirectionMarker(painter);
}

void ParkingCell::loadImages()
{
	//Load the images if they aren't loaded already
	if(!m_normalImage || !m_selectedImage)
	{
		m_normalImage = PanelImage::create("resources/map/parking-cell.png");
		m_selectedImage = PanelImage::create("resources/map/parking-cell-selected.png");
	}
}

std::unique_ptr<PanelImage> StorageCell::m_normalImage = nullptr;
std::unique_ptr<PanelImage> StorageCell::m_selectedImage = nullptr;

StorageCell::StorageCell(int id, const MapPosition& position) : CellDefault(id, position) {}
StorageCell::~StorageCell() = default;

void StorageCell::draw(Painter& painter)
{
	loadImages();
	CellDefault::drawWithImages(painter, m_normalImage, m_selectedImage);
	drawDirectionMarker(painter);
}

void StorageCell::loadImages()
{
	//Load the images if they aren't loaded already
	if(!m_normalImage || !m_selectedImage)
	{
		m_normalImage = PanelImage::create("resources/map/storage-cell.png");
		m_selectedImage = PanelImage::create("resources/map/storage-cell-selected.png");
	}
}

std::unique_ptr<PanelImage> IngressCell::m_normalImage = nullptr;
std::unique_ptr<PanelImage> IngressCell::m_selectedImage = nullptr;

IngressCell::IngressCell(int id, const MapPosition& position) : CellDefault(id, position) {}
IngressCell::~IngressCell() = default;

void IngressCell::draw(Painter& painter)
{
	loadImages();
	CellDefault::drawWithImages(painter, m_normalImage, m_selectedImage);
	drawDirectionMarker(painter);
}

void IngressCell::loadImages()
{
	//Load the images if they aren't loaded already
	if(!m_normalImage || !m_selectedImage)
	{
		m_normalImage = PanelImage::create("resources/map/ingress-cell.png");
		m_selectedImage = PanelImage::create("resources/map/ingress-cell-selected.png");
	}
}