#pragma once

#include <wx/thread.h>

class ManagerThread : public wxThread
{};

class SIAIMap;
class DbConnector;

class UpdateMapThread : public ManagerThread
{
private:
    SIAIMap* m_mapControl;
    DbConnector* m_dbConnector;

public:
    UpdateMapThread(SIAIMap* mapControl, DbConnector* dbConnector);
    virtual ~UpdateMapThread();

    virtual void* Entry() override;
};