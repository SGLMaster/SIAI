#pragma once

#include "map/entities/mapentity.hpp"

#include "util/id.hpp"

#include <memory>
#include <vector>
#include <string>

class ICell : public IMapEntity
{
public:
    static constexpr int MIN_ID{1};
    static constexpr int MAX_ID{999999};

    static Util::IdManager IdManager;

    static const std::vector<std::string> dbColumnNames;
	static const std::vector<std::string> dbColumnTypes;
	static const std::string primaryKeyName;

private:
    static bool directionMarkersEnabled;

public:
    ICell() = delete;
    ICell(int id, const MapPosition& position);
    virtual ~ICell();

    static std::unique_ptr<ICell> create(std::string type, int id, const MapPosition& position);

    static void enableDirectionMarkers(bool value)
    {
        directionMarkersEnabled = value;
    }
    static bool isDirectionMarkersEnabled()
    {
        return directionMarkersEnabled;
    }
};
