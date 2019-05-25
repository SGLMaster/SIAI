#pragma once

#include "map/entities/item.hpp"

#include "database/dbobject.hpp"

class RegularItem : public AItem, public DbObject
{
public:
    RegularItem() = delete;
    RegularItem(const AItem::Data& data);
    virtual ~RegularItem();

    virtual void insertToDatabase(DbConnector& connector, const std::string& mapName) const override;
    virtual void updateInDatabase(DbConnector& connector, const std::string& mapName) const override;
    virtual void loadFromDatabase(DbConnector& connector) override;
};