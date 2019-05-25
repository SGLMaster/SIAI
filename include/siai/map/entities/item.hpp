#pragma once

#include "util/id.hpp"

#include <memory>
#include <vector>
#include <string>

class AItem
{
public:
    static constexpr int MIN_ID{1};
    static constexpr int MAX_ID{2000000000};

    static Util::IdManager IdManager;

    static const std::vector<std::string> dbColumnNames;
	static const std::vector<std::string> dbColumnTypes;
	static const std::string primaryKeyName;

    struct Data
    {
        int id;
        int rackId;

        std::string name;
        std::string code;

        int weight;
    };

protected:
    Data m_data;

public:
    AItem() = delete;
    AItem(const Data& data);
    virtual ~AItem();

    static std::unique_ptr<AItem> create(std::string type, const Data& data);
};