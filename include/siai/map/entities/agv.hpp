#pragma once

#include "map/entities/mapentity.hpp"

#include "util/id.hpp"

#include <memory>
#include <vector>
#include <string>

class IAgv : public IMapEntity, public INetEntity
{
public:
	static const std::vector<std::string> dbColumnNames;
	static const std::vector<std::string> dbColumnTypes;
	static const std::string primaryKeyName;

    static constexpr int MIN_AGV_ID{1};
    static constexpr int MAX_AGV_ID{9999};

    static Util::IdManager AgvsIdManager;

private:
    static constexpr int SERVER_PORT_OFFSET{20000};
    static constexpr int SOCKET_PORT_OFFSET{30000};

public:
    IAgv() = delete;
    IAgv(int id, const MapPosition& position);
    virtual ~IAgv();

    static std::unique_ptr<IAgv> create(std::string type, int id, const MapPosition& position);

    virtual int getDrawOrder() const noexcept
    {
        return 1;
    }
};
