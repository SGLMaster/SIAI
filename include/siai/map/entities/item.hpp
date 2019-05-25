#pragma once

#include "util/id.hpp"

#include <memory>
#include <string>

class AItem
{
public:
    static constexpr int MIN_ID{1};
    static constexpr int MAX_ID{2000000000};

    static Util::IdManager IdManager;

protected:
    int m_id;
    int m_rackId;

public:
    AItem() = delete;
    AItem(int id, int rackId);
    virtual ~AItem();

    static std::unique_ptr<AItem> create(std::string type, int id, int rackId);
};