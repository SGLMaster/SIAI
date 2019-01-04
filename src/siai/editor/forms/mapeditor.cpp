///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>

#include "editor/forms/mapeditor.h"

///////////////////////////////////////////////////////////////////////////
using namespace Forms;

MapEditorFrame::MapEditorFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 800,600 ), wxDefaultSize );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	m_toolBar1 = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_HORIZONTAL );
	m_toolSelect = m_toolBar1->AddTool( wxID_ANY, _("Seleccionar"), wxBitmap( wxT("resources/tools/select.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_RADIO, _("Seleccionar"), _("Seleccionar"), NULL );

	m_toolRegularCell = m_toolBar1->AddTool( wxID_ANY, _("Celda Libre"), wxBitmap( wxT("resources/tools/white-cell.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_RADIO, _("Celda Libre"), _("Celda Libre"), NULL );

	m_toolBlockedCell = m_toolBar1->AddTool( wxID_ANY, _("Celda Bloqueada"), wxBitmap( wxT("resources/tools/black-cell.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_RADIO, _("Celda Bloqueada"), _("Celda Bloqueada"), NULL );

	m_toolAddAgv = m_toolBar1->AddTool( wxID_ANY, _("Agregar Agv"), wxBitmap( wxT("resources/tools/regular-agv.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_RADIO, _("Agregar Agv"), _("Agregar Agv"), NULL );

	m_toolTurnLeft = m_toolBar1->AddTool( wxID_ANY, _("Girar a la izquierda"), wxBitmap( wxT("resources/tools/turn-left.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_RADIO, _("Girar a la izquierda"), _("Girar a la izquierda"), NULL );

	m_toolTurnRight = m_toolBar1->AddTool( wxID_ANY, _("Girar a la derecha"), wxBitmap( wxT("resources/tools/turn-right.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_RADIO, _("Girar a la derecha"), _("Girar a la derecha"), NULL );

	m_toolBar1->AddSeparator();

	m_toolUndo = m_toolBar1->AddTool( wxID_ANY, _("Deshacer"), wxBitmap( wxT("resources/tools/undo.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, _("Deshacer"), _("Deshacer"), NULL );

	m_toolRedo = m_toolBar1->AddTool( wxID_ANY, _("Rehacer"), wxBitmap( wxT("resources/tools/redo.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, _("Rehacer"), _("Rehacer"), NULL );

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

	m_menuDatabase = new wxMenu();
	wxMenuItem* m_menuItemDbSettings;
	m_menuItemDbSettings = new wxMenuItem( m_menuDatabase, wxID_ANY, wxString( _("Configuración") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuDatabase->Append( m_menuItemDbSettings );

	m_menubar->Append( m_menuDatabase, _("Base de Datos") );

	this->SetMenuBar( m_menubar );

	m_statusBar = this->CreateStatusBar( 3, wxSTB_ELLIPSIZE_END|wxSTB_SIZEGRIP, wxID_ANY );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( m_toolSelect->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapEditorFrame::OnToolSelect ) );
	this->Connect( m_toolRegularCell->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapEditorFrame::OnToolRegularCell ) );
	this->Connect( m_toolBlockedCell->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapEditorFrame::OnToolBlockedCell ) );
	this->Connect( m_toolAddAgv->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapEditorFrame::OnToolAddAgv ) );
	this->Connect( m_toolTurnLeft->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapEditorFrame::OnToolTurnLeft ) );
	this->Connect( m_toolTurnRight->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapEditorFrame::OnToolTurnRight ) );
	this->Connect( m_toolUndo->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapEditorFrame::OnToolUndo ) );
	this->Connect( m_toolRedo->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapEditorFrame::OnToolRedo ) );
	m_sliderZoom->Connect( wxEVT_SLIDER, wxCommandEventHandler( MapEditorFrame::OnSliderZoom ), NULL, this );
	m_scrolledMapPanel->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( MapEditorFrame::OnEnterMapPanel ), NULL, this );
	m_scrolledMapPanel->Connect( wxEVT_LEAVE_WINDOW, wxMouseEventHandler( MapEditorFrame::OnLeaveMapPanel ), NULL, this );
	m_scrolledMapPanel->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MapEditorFrame::OnLeftClickMapPanel ), NULL, this );
	m_scrolledMapPanel->Connect( wxEVT_PAINT, wxPaintEventHandler( MapEditorFrame::OnPaintMapPanel ), NULL, this );
	m_menuMap->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapEditorFrame::OnSelectionNewMap ), this, m_menuItemNewMap->GetId());
	m_menuDatabase->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapEditorFrame::OnSelectionDbSettings ), this, m_menuItemDbSettings->GetId());
}

MapEditorFrame::~MapEditorFrame()
{
	// Disconnect Events
	this->Disconnect( m_toolSelect->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapEditorFrame::OnToolSelect ) );
	this->Disconnect( m_toolRegularCell->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapEditorFrame::OnToolRegularCell ) );
	this->Disconnect( m_toolBlockedCell->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapEditorFrame::OnToolBlockedCell ) );
	this->Disconnect( m_toolAddAgv->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapEditorFrame::OnToolAddAgv ) );
	this->Disconnect( m_toolTurnLeft->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapEditorFrame::OnToolTurnLeft ) );
	this->Disconnect( m_toolTurnRight->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapEditorFrame::OnToolTurnRight ) );
	this->Disconnect( m_toolUndo->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapEditorFrame::OnToolUndo ) );
	this->Disconnect( m_toolRedo->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapEditorFrame::OnToolRedo ) );
	m_sliderZoom->Disconnect( wxEVT_SLIDER, wxCommandEventHandler( MapEditorFrame::OnSliderZoom ), NULL, this );
	m_scrolledMapPanel->Disconnect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( MapEditorFrame::OnEnterMapPanel ), NULL, this );
	m_scrolledMapPanel->Disconnect( wxEVT_LEAVE_WINDOW, wxMouseEventHandler( MapEditorFrame::OnLeaveMapPanel ), NULL, this );
	m_scrolledMapPanel->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MapEditorFrame::OnLeftClickMapPanel ), NULL, this );
	m_scrolledMapPanel->Disconnect( wxEVT_PAINT, wxPaintEventHandler( MapEditorFrame::OnPaintMapPanel ), NULL, this );

}

NewMapDialog::NewMapDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxHORIZONTAL );

	m_staticMapName = new wxStaticText( this, wxID_ANY, _("Nombre del Mapa:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticMapName->Wrap( -1 );
	bSizer14->Add( m_staticMapName, 0, wxALL, 5 );


	bSizer14->Add( 0, 0, 1, wxEXPAND, 5 );

	m_textCtrlMapName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer14->Add( m_textCtrlMapName, 0, wxALL, 5 );


	bSizer13->Add( bSizer14, 1, wxEXPAND, 5 );


	bSizer2->Add( bSizer13, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );

	m_staticTextNumberOfCols = new wxStaticText( this, wxID_ANY, _("Número de Columnas:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextNumberOfCols->Wrap( -1 );
	bSizer3->Add( m_staticTextNumberOfCols, 0, wxALL, 5 );


	bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );

	m_textCtrlNumberOfCols = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
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

	m_textCtrlNumberOfRows = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
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
	bSizer2->Fit( this );

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

DbSettingsDialog::DbSettingsDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );

	m_staticHost = new wxStaticText( this, wxID_ANY, _("Host:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticHost->Wrap( -1 );
	bSizer7->Add( m_staticHost, 0, wxALL, 5 );


	bSizer7->Add( 0, 0, 1, wxEXPAND, 5 );

	m_textCtrlHost = new wxTextCtrl( this, wxID_ANY, _("localhost"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_textCtrlHost, 0, wxALL, 5 );


	bSizer6->Add( bSizer7, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );

	m_staticPort = new wxStaticText( this, wxID_ANY, _("Puerto:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticPort->Wrap( -1 );
	bSizer10->Add( m_staticPort, 0, wxALL, 5 );


	bSizer10->Add( 0, 0, 1, wxEXPAND, 5 );

	m_textCtrlPort = new wxTextCtrl( this, wxID_ANY, _("3306"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer10->Add( m_textCtrlPort, 0, wxALL, 5 );


	bSizer6->Add( bSizer10, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );

	m_staticUser = new wxStaticText( this, wxID_ANY, _("Usuario:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticUser->Wrap( -1 );
	bSizer8->Add( m_staticUser, 0, wxALL, 5 );


	bSizer8->Add( 0, 0, 1, wxEXPAND, 5 );

	m_textCtrlUser = new wxTextCtrl( this, wxID_ANY, _("test_user"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_textCtrlUser, 0, wxALL, 5 );


	bSizer6->Add( bSizer8, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );

	m_staticPassword = new wxStaticText( this, wxID_ANY, _("Contraseña:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticPassword->Wrap( -1 );
	bSizer9->Add( m_staticPassword, 0, wxALL, 5 );


	bSizer9->Add( 0, 0, 1, wxEXPAND, 5 );

	m_textCtrlPassword = new wxTextCtrl( this, wxID_ANY, _("easypass"), wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	bSizer9->Add( m_textCtrlPassword, 0, wxALL, 5 );


	bSizer6->Add( bSizer9, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxHORIZONTAL );

	m_buttonAccept = new wxButton( this, wxID_ANY, _("Aceptar"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer12->Add( m_buttonAccept, 0, wxALL, 5 );

	m_button4 = new wxButton( this, wxID_ANY, _("Cancelar"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer12->Add( m_button4, 0, wxALL, 5 );


	bSizer6->Add( bSizer12, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer6 );
	this->Layout();
	bSizer6->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( DbSettingsDialog::OnClose ) );
	m_buttonAccept->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSettingsDialog::OnButtonClickAccept ), NULL, this );
	m_button4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSettingsDialog::OnButtonClickCancel ), NULL, this );
}

DbSettingsDialog::~DbSettingsDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( DbSettingsDialog::OnClose ) );
	m_buttonAccept->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSettingsDialog::OnButtonClickAccept ), NULL, this );
	m_button4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DbSettingsDialog::OnButtonClickCancel ), NULL, this );

}
