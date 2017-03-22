/**
 * About dialog.
 */

#include "dlg_about.h"
#include "gui_controls.h"
#include "app_version.h"
#include "pixmaps/close_16x16.xpm"
#include "pixmaps/info_16x16.xpm"

AboutDialog::AboutDialog(wxWindow *parent):
  wxDialog(parent, wxID_ANY, "")
{
  SetTitle(wxT("Application information"));

  SetSize(wxSize(440, 300));

  SetIcon(wxIcon(info_16x16_xpm));

  wxBoxSizer *szMargin = new wxBoxSizer(wxVERTICAL);

  SetSizer(szMargin);

  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);
  szMargin->Add(szTop, 1, wxALL | wxEXPAND | wxGROW, 10);


  // Application description

  wxStaticText *label = new wxStaticText(this, wxID_STATIC, wxT(APP_BRIEF) wxT("."));
  szTop->Add(label, 0, wxBOTTOM, 15);

  wxFlexGridSizer *szFields = new wxFlexGridSizer(0, 2, 5, 5);
  szTop->Add(szFields, 1, wxALL, 0);

  // Application description

  label = new wxStaticText(this, wxID_STATIC, wxT("Homepage"));
  szFields->Add(label, 0, wxALL, 1);

  label = new wxStaticText(this, wxID_STATIC, wxT(APP_URL));
  szFields->Add(label, 0, wxALL, 1);

  // Application version

  label = new wxStaticText(this, wxID_STATIC, wxT("Versão"));
  szFields->Add(label, 0, wxALL, 1);

  label = new wxStaticText(this, wxID_STATIC, wxT(APP_VERSION));
  szFields->Add(label, 0, wxALL, 1);

  // Build information

  label = new wxStaticText(this, wxID_STATIC, wxT("Build"));
  szFields->Add(label, 0, wxALL, 1);

  label = new wxStaticText(this, wxID_STATIC, wxT(__DATE__ " " __TIME__));
  szFields->Add(label, 0, wxALL, 1);

  // Owner

  wxBoxSizer *szRow = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szRow, 0, wxALL | wxALIGN_CENTER, 5);

  label = new wxStaticText(this, wxID_STATIC, wxString(wxT(APP_OWNER) wxT(" ®")));
  szRow->Add(label, 0, wxALL, 0);

  szTop->AddSpacer(3);

  szRow = new wxBoxSizer(wxHORIZONTAL);
  szTop->Add(szRow, 0, wxALL | wxALIGN_RIGHT, 3);

  wxBitmapButton *btnClose = new wxBitmapButton(this,
                                                wxID_CANCEL, wxBitmap(close_16x16_xpm),
                                                wxDefaultPosition,
                                                wxDefaultSize,
                                                wxBU_AUTODRAW);
  szRow->Add(btnClose, 0, wxALL | wxALIGN_BOTTOM, 1);


  //Fit();

  Centre();
}


void AboutDialog::Terminate()
{
  Close();

  //statusBar->Destroy();

  Destroy();
}


void AboutDialog::OnRightClick(wxMouseEvent& event)
{
  Terminate();
}


BEGIN_EVENT_TABLE(AboutDialog, wxDialog)
  EVT_RIGHT_UP(AboutDialog::OnRightClick)
  EVT_LEFT_UP(AboutDialog::OnRightClick)
END_EVENT_TABLE()