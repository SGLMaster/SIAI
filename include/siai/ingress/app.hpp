#pragma once

#include <wx/app.h>

class IngressFrame;

class IngressApp : public wxApp
{
public:
    IngressApp();
    virtual ~IngressApp();

private:
    IngressFrame* m_frame;

    virtual bool OnInit() override;
};

DECLARE_APP(IngressApp)