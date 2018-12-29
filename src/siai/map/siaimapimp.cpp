#include "map/siaimapimp.hpp"
#include "map/mapentity.hpp"
#include "map/painter.hpp"
#include "map/command.hpp"

#include "util/reversion.hpp"
#include "util/string.hpp"

#include <algorithm>

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

    if(m_commandStream.size() > 0 && m_commandIterator != m_commandStream.end())
    {
        int lastCommandIndex = std::distance(m_commandStream.begin(), m_commandIterator);
        int newCommandStreamSize = lastCommandIndex;

        m_commandStream.resize(newCommandStreamSize);
    }

    m_commandStream.push_back(std::move(mapCommand));

    m_commandIterator = m_commandStream.end();
}

void SIAIMapImp::undo()
{
    if(m_commandIterator != m_commandStream.begin() && m_commandStream.size() > 0)
    {
        --m_commandIterator;
    }

    if(m_commandStream.size() > 0)
    {
        auto& lastCommand = *m_commandIterator;
        lastCommand->undo(m_entities);
    }
}

void SIAIMapImp::redo()
{
    if(m_commandIterator != m_commandStream.end() && m_commandStream.size() > 0)
    {
        auto& command = *m_commandIterator;
        command->execute(m_entities);

        ++m_commandIterator;
    }
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

    Entities::generateMapCells(m_entities, m_numberOfColumns, m_numberOfRows);
}

void SIAIMapImp::repaint(Painter& painter)
{
    for(const auto& entity : m_entities)
    {
        entity->draw(painter);
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
