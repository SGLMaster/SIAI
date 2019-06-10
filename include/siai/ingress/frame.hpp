#pragma once

#include "ingress/ingress.h"

#include "database/database.hpp"

#include <memory>

class IngressFrame : public Forms::IngressFrame
{
private:
    std::string m_originalFrameTitle;

    DbConnectionOptions m_dbConnectionOptions;

    std::unique_ptr<DbConnector> m_dbConnector;

public:
    IngressFrame(wxWindow* parent);
    virtual ~IngressFrame();

private:
    virtual void OnSelectionConnect(wxCommandEvent& event) override;

    void tryToConnectDb();
    bool isDbConnected();

    void updateFrameTitle();
};