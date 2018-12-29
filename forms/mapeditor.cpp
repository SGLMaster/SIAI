///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>

#include "mapeditor.h"

///////////////////////////////////////////////////////////////////////////
using namespace Forms;

MapEditorFrame::MapEditorFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 800,600 ), wxDefaultSize );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	m_toolBar1 = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_HORIZONTAL );
	m_toolSelect = m_toolBar1->AddTool( wxID_ANY, _("Seleccionar"), wxBitmap( wxT("resources/tools/select.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, _("Seleccionar"), _("Seleccionar"), NULL );

	m_toolRegularCell = m_toolBar1->AddTool( wxID_ANY, _("Celda Libre"), wxBitmap( wxT("resources/tools/white-cell.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, _("Celda Libre"), _("Celda Libre"), NULL );

	m_toolBlockedCell = m_toolBar1->AddTool( wxID_ANY, _("Celda Bloqueada"), wxBitmap( wxT("resources/tools/black-cell.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, _("Celda Bloqueada"), _("Celda Bloqueada"), NULL );

	m_toolBar1->AddSeparator();

	m_toolUndo = m_toolBar1->AddTool( wxID_ANY, _("Deshacer"), wxBitmap( wxT("resources/tools/undo.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, _("Deshacer"), _("Deshacer"), NULL );

	m_toolBar1->AddSeparator();

	m_staticTextZoom = new wxStaticText( m_toolBar1, wxID_ANY, _("Zoom:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextZoom->Wrap( -1 );
	m_toolBar1->AddControl( m_staticTextZoom );
	m_sliderZoom = new wxSlider( m_toolBar1, wxID_ANY, 1, 1, 10, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL );
	m_toolBar1->AddControl( m_sliderZoom );
	m_toolBar1->Realize();

	bSizer1->Add( m_toolBar1, 1, wxEXPAND, 5 );

	m_scrolledMapPanel = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_scrolledMapPanel->SetScrollRate( 5, 5 );
	bSizer1->Add( m_scrolledMapPanel, 20, wxEXPAND | wxALL, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();
	m_menubar = new wxMenuBar( 0 );
	m_menuMap = new wxMenu();
	wxMenuItem* m_menuItemNewMap;
	m_menuItemNewMap = new wxMenuItem( m_menuMap, wxID_ANY, wxString( _("Nuevo Mapa...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuMap->Append( m_menuItemNewMap );

	m_menubar->Append( m_menuMap, _("Mapa") );

	this->SetMenuBar( m_menubar );

	m_statusBar = this->CreateStatusBar( 3, wxSTB_ELLIPSIZE_END|wxSTB_SIZEGRIP, wxID_ANY );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( m_toolSelect->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapEditorFrame::OnToolSelect ) );
	this->Connect( m_toolRegularCell->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapEditorFrame::OnToolRegularCell ) );
	this->Connect( m_toolBlockedCell->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapEditorFrame::OnToolBlockedCell ) );
	this->Connect( m_toolUndo->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapEditorFrame::OnToolUndo ) );
	m_sliderZoom->Connect( wxEVT_SLIDER, wxCommandEventHandler( MapEditorFrame::OnSliderZoom ), NULL, this );
	m_scrolledMapPanel->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MapEditorFrame::OnLeftClickMapPanel ), NULL, this );
	m_scrolledMapPanel->Connect( wxEVT_PAINT, wxPaintEventHandler( MapEditorFrame::OnPaintMapPanel ), NULL, this );
	m_menuMap->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapEditorFrame::OnSelectionNewMap ), this, m_menuItemNewMap->GetId());
}

MapEditorFrame::~MapEditorFrame()
{
	// Disconnect Events
	this->Disconnect( m_toolSelect->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapEditorFrame::OnToolSelect ) );
	this->Disconnect( m_toolRegularCell->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapEditorFrame::OnToolRegularCell ) );
	this->Disconnect( m_toolBlockedCell->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapEditorFrame::OnToolBlockedCell ) );
	this->Disconnect( m_toolUndo->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapEditorFrame::OnToolUndo ) );
	m_sliderZoom->Disconnect( wxEVT_SLIDER, wxCommandEventHandler( MapEditorFrame::OnSliderZoom ), NULL, this );
	m_scrolledMapPanel->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MapEditorFrame::OnLeftClickMapPanel ), NULL, this );
	m_scrolledMapPanel->Disconnect( wxEVT_PAINT, wxPaintEventHandler( MapEditorFrame::OnPaintMapPanel ), NULL, this );

}

NewMapDialog::NewMapDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );

	m_staticTextNumberOfCols = new wxStaticText( this, wxID_ANY, _("Número de Columnas:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextNumberOfCols->Wrap( -1 );
	bSizer3->Add( m_staticTextNumberOfCols, 0, wxALL, 5 );


	bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );

	m_textCtrlNumberOfCols = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !m_textCtrlNumberOfCols->HasFlag( wxTE_MULTILINE ) )
	{
	m_textCtrlNumberOfCols->SetMaxLength( 3 );
	}
	#else
	m_textCtrlNumberOfCols->SetMaxLength( 3 );
	#endif
	bSizer3->Add( m_textCtrlNumberOfCols, 0, wxALL, 5 );


	bSizer2->Add( bSizer3, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );

	m_staticTextNumberOfRows = new wxStaticText( this, wxID_ANY, _("Número de Filas:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextNumberOfRows->Wrap( -1 );
	bSizer4->Add( m_staticTextNumberOfRows, 0, wxALL, 5 );


	bSizer4->Add( 0, 0, 1, wxEXPAND, 5 );

	m_textCtrlNumberOfRows = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !m_textCtrlNumberOfRows->HasFlag( wxTE_MULTILINE ) )
	{
	m_textCtrlNumberOfRows->SetMaxLength( 3 );
	}
	#else
	m_textCtrlNumberOfRows->SetMaxLength( 3 );
	#endif
	bSizer4->Add( m_textCtrlNumberOfRows, 0, wxALL, 5 );


	bSizer2->Add( bSizer4, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );


	bSizer5->Add( 0, 0, 1, wxEXPAND, 5 );

	m_buttonAccept = new wxButton( this, wxID_ANY, _("Aceptar"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_buttonAccept, 0, wxALL, 5 );

	m_buttonCancel = new wxButton( this, wxID_ANY, _("Cancelar"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_buttonCancel, 0, wxALL, 5 );


	bSizer2->Add( bSizer5, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer2 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( NewMapDialog::OnClose ) );
	m_buttonAccept->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NewMapDialog::OnButtonClickAccept ), NULL, this );
	m_buttonCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NewMapDialog::OnButtonClickCancel ), NULL, this );
}

NewMapDialog::~NewMapDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( NewMapDialog::OnClose ) );
	m_buttonAccept->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NewMapDialog::OnButtonClickAccept ), NULL, this );
	m_buttonCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NewMapDialog::OnButtonClickCancel ), NULL, this );

}
