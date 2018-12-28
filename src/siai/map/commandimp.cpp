#include "map/commandimp.hpp"
#include "map/mapentity.hpp"

CommandDrawAll::CommandDrawAll(EntitiesContainer& entities, Painter& painter) : m_entities{entities},
                                                                                m_painter{painter} {}
CommandDrawAll::~CommandDrawAll() = default;

void CommandDrawAll::execute()
{
    for(const auto& entity : m_entities)
    {
        entity->draw(m_painter);
    }
}

void CommandDrawAll::undo()
{
    ;
}
