#include "map/siaimapimp.hpp"
#include "map/entities/mapentity.hpp"
#include "map/cmd/cmdstream.hpp"

#include "util/reversion.hpp"

#include <algorithm>

SIAIMapImp::SIAIMapImp() : m_commandStream{CommandStream::create()} {}
SIAIMapImp::~SIAIMapImp() = default;

void SIAIMapImp::executeCommand(const std::string& command)
{
    m_commandStream->executeAndLog(m_entities, command);

    diselectAll();
}

void SIAIMapImp::undo()
{
    m_commandStream->undo(m_entities);

    diselectAll();
}

void SIAIMapImp::redo()
{
    m_commandStream->redo(m_entities);

    diselectAll();
}

int SIAIMapImp::getNumberOfColumns() const noexcept
{
    return m_numberOfColumns;
}

int SIAIMapImp::getNumberOfRows() const noexcept
{
    return m_numberOfRows;
}

void SIAIMapImp::reset(int numberOfColumns, int numberOfRows)
{
    m_numberOfColumns = numberOfColumns;
    m_numberOfRows = numberOfRows;

    m_selectedEntities.resize(0);
    m_entities.resize(0);

    Entities::generateMapCells(m_entities, m_numberOfColumns, m_numberOfRows);
}

void SIAIMapImp::repaint(Painter& painter)
{
    for(const auto& entity : m_entities)
    {
        entity->draw(painter);
    }
}

void SIAIMapImp::selectEntity(const PanelPoint& point)
{
    for(const auto& entity : Util::reverse(m_entities))
    {
        bool someEntityChanged = Entities::selectOrDiselectIfHasPointInside(*entity, point);

        if(someEntityChanged)
        {
            m_selectedEntities.push_back(entity);
            break;
        }
    }
}

void SIAIMapImp::diselectAll()
{
    for(const auto& entity : m_selectedEntities)
    {
        entity->diselect();
    }

    m_selectedEntities.resize(0);
}

int SIAIMapImp::getSelectedEntityColumn() const noexcept
{
    MapPosition lastSelectedPosition{0, 0};

    for(const auto& entity : Util::reverse(m_selectedEntities))
    {
        if(entity->isSelected())
        {
            lastSelectedPosition = entity->getPosition();
        }
    }

    return lastSelectedPosition.column;
}

int SIAIMapImp::getSelectedEntityRow() const noexcept
{
    MapPosition lastSelectedPosition{0, 0};

    for(const auto& entity : Util::reverse(m_selectedEntities))
    {
        if(entity->isSelected())
            lastSelectedPosition = entity->getPosition();
    }

    return lastSelectedPosition.row;
}

int SIAIMapImp::getSelectedId() const noexcept
{
    int lastSelectedId{0};

    for(const auto& entity : m_selectedEntities)
    {
        if(entity->isSelected())
            lastSelectedId = entity->getId();
    }

    return lastSelectedId;
}

