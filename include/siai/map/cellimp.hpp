#pragma once

#include "map/cell.hpp"

class RegularCell : public Cell
{
public:
    RegularCell();
    RegularCell(int column, int row);
    virtual ~RegularCell();

    virtual void draw(PainterContainer& painter) override;

};

class BlockedCell : public Cell
{
public:
    BlockedCell();
    BlockedCell(int column, int row);
    virtual ~BlockedCell();

    virtual void draw(PainterContainer& painter) override;
};
