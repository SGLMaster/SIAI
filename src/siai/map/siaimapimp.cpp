#include "map/siaimapimp.hpp"
#include "map/mapentity.hpp"
#include "map/painter.hpp"
#include "map/command.hpp"

#include "util/reversion.hpp"
#include "util/string.hpp"

#include <algorithm>
#include <deque>

SIAIMapImp::SIAIMapImp() = default;
SIAIMapImp::~SIAIMapImp() = default;

void SIAIMapImp::executeCommand(const std::string& command)
{
    std::deque<std::string> arguments;
    Util::String::splitIntoContainer(command, arguments, ' ');

    std::string commandName = arguments.front();
    arguments.pop_front();

    auto mapCommand = MapCommand::create(commandName, arguments);
    mapCommand->execute(m_entities);

    m_commandStream.push_back(std::move(mapCommand));
}

void SIAIMapImp::undoLastCommand()
{
    auto& lastCommand = m_commandStream.back();
    lastCommand->undo(m_entities);
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

    m_entities.resize(0);

    generateCells();
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
        bool someEntityChanged = selectOrDiselectIfHasPointInside(entity.get(), point);

        if(someEntityChanged) return;
    }
}

MapPosition SIAIMapImp::getLastSelectedPosition() const noexcept
{
    MapPosition lastSelectedPosition{0, 0};

    for(const auto& entity : Util::reverse(m_entities))
    {
        if(entity->isSelected())
            lastSelectedPosition = entity->getPosition();
    }

    return lastSelectedPosition;
}

int SIAIMapImp::getLastSelectedId() const noexcept
{
    int lastSelectedId{0};

    for(const auto& entity : m_entities)
    {
        if(entity->isSelected())
            lastSelectedId = entity->getId();
    }

    return lastSelectedId;
}

bool SIAIMapImp::selectOrDiselectIfHasPointInside(IMapEntity* entity, const PanelPoint& point) noexcept
{
    if(entity->hasPointInside(point))
    {
        if(!entity->isSelected())
            entity->select();
        else
            entity->diselect();
        return true;
    }

    return false;
}

void SIAIMapImp::generateCells()
{
    int idGenerator{0};

    for(int column = 0; column < m_numberOfColumns; ++column)
    {
        for(int row = 0; row < m_numberOfRows; ++row)
        {
            int id = ++idGenerator;
            MapPosition position{column, row, MapDirection::RIGHT};

            Entities::Pointer tmpCell = ICell::create("RegularCell", id, position);

            m_entities.push_back(std::move(tmpCell));

            ///////////////////////////TEST//////////////////////////
            if(position.column < 20)
            {
                Entities::Pointer tmpAgv = IAgv::create("RegularAgv", id, position);
                m_entities.push_back(std::move(tmpAgv));
            }

        }
    }
}
