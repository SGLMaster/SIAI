///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/slider.h>
#include <wx/toolbar.h>
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace Forms
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class MapEditorFrame
	///////////////////////////////////////////////////////////////////////////////
	class MapEditorFrame : public wxFrame
	{
		private:

		protected:
			wxToolBar* m_toolBar1;
			wxToolBarToolBase* m_toolSelect;
			wxToolBarToolBase* m_toolRegularCell;
			wxToolBarToolBase* m_toolBlockedCell;
			wxToolBarToolBase* m_toolAddAgv;
			wxToolBarToolBase* m_toolTurnLeft;
			wxToolBarToolBase* m_toolTurnRight;
			wxToolBarToolBase* m_toolUndo;
			wxToolBarToolBase* m_toolRedo;
			wxStaticText* m_staticTextZoom;
			wxSlider* m_sliderZoom;
			wxScrolledWindow* m_scrolledMapPanel;
			wxMenuBar* m_menubar;
			wxMenu* m_menuMap;
			wxStatusBar* m_statusBar;

			// Virtual event handlers, overide them in your derived class
			virtual void OnToolSelect( wxCommandEvent& event ) = 0;
			virtual void OnToolRegularCell( wxCommandEvent& event ) = 0;
			virtual void OnToolBlockedCell( wxCommandEvent& event ) = 0;
			virtual void OnToolAddAgv( wxCommandEvent& event ) = 0;
			virtual void OnToolTurnLeft( wxCommandEvent& event ) = 0;
			virtual void OnToolTurnRight( wxCommandEvent& event ) = 0;
			virtual void OnToolUndo( wxCommandEvent& event ) = 0;
			virtual void OnToolRedo( wxCommandEvent& event ) = 0;
			virtual void OnSliderZoom( wxCommandEvent& event ) = 0;
			virtual void OnLeftClickMapPanel( wxMouseEvent& event ) = 0;
			virtual void OnPaintMapPanel( wxPaintEvent& event ) = 0;
			virtual void OnSelectionNewMap( wxCommandEvent& event ) = 0;


		public:

			MapEditorFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Editor de Mapas SIAI"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

			~MapEditorFrame();

	};

	///////////////////////////////////////////////////////////////////////////////
	/// Class NewMapDialog
	///////////////////////////////////////////////////////////////////////////////
	class NewMapDialog : public wxDialog
	{
		private:

		protected:
			wxStaticText* m_staticTextNumberOfCols;
			wxTextCtrl* m_textCtrlNumberOfCols;
			wxStaticText* m_staticTextNumberOfRows;
			wxTextCtrl* m_textCtrlNumberOfRows;
			wxButton* m_buttonAccept;
			wxButton* m_buttonCancel;

			// Virtual event handlers, overide them in your derived class
			virtual void OnClose( wxCloseEvent& event ) = 0;
			virtual void OnButtonClickAccept( wxCommandEvent& event ) = 0;
			virtual void OnButtonClickCancel( wxCommandEvent& event ) = 0;


		public:

			NewMapDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Nuevo Mapa..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 275,150 ), long style = wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP );
			~NewMapDialog();

	};

} // namespace Forms

