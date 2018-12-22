#include "map/cellimp.hpp"

#include <wx/dcclient.h>

RegularCell::RegularCell() = default;
RegularCell::RegularCell(int column, int row) : Cell(column, row) {}
RegularCell::~RegularCell() = default;

void RegularCell::draw(PainterContainer& painter)
{
    painter.drawClient.SetBrush(*wxWHITE_BRUSH);

    Cell::draw(painter);
}

BlockedCell::BlockedCell() = default;
BlockedCell::BlockedCell(int column, int row) : Cell(column, row) {}
BlockedCell::~BlockedCell() = default;

void BlockedCell::draw(PainterContainer& painter)
{
    painter.drawClient.SetBrush(*wxBLACK_BRUSH);

    Cell::draw(painter);
}
