#pragma once

//event sent by workers to server class
//after client is served
const wxEventType wxEVT_WORKER = wxNewEventType();
#define EVT_WORKER(func) DECLARE_EVENT_TABLE_ENTRY( wxEVT_WORKER, -1, -1, (wxObjectEventFunction) (wxEventFunction) (WorkerEventFunction) & func, (wxObject *) NULL ),

class WorkerEvent : public wxEvent
{
public:
    WorkerEvent(void* pSender)
    {
        SetId(-1);
        SetEventType(wxEVT_WORKER);
        m_sender = pSender;
        m_exit = false;
        m_workerFailed = false;
    }

    virtual wxEvent* Clone() const
    {
        return new WorkerEvent(*this);
    }

    void* m_sender;
    bool m_exit;
    bool m_workerFailed;
};

typedef void (wxEvtHandler::*WorkerEventFunction)(WorkerEvent&);