#pragma once

#include "map/command.hpp"

#include <vector>

class Painter;

class IMapEntity;

using EntityPointer = std::unique_ptr<IMapEntity>;
using EntitiesContainer = std::vector<EntityPointer>;

class CommandDrawAll : public MapCommand
{
private:
    EntitiesContainer& m_entities;
    Painter& m_painter;

public:
    CommandDrawAll() = delete;
    CommandDrawAll(EntitiesContainer& entities, Painter& painter);
    virtual ~CommandDrawAll();

    virtual void execute() override;
    virtual void undo() override;
};
