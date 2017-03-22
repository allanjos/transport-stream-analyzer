/**
 *
 * Settings dialog
 */

#include "gui_controls.h"
#include "dlg_settings.h"
#include "dlg_main.h"
#include "pixmaps/config_16x16.xpm"
#include "pixmaps/save_16x16.xpm"
#include "pixmaps/close_16x16.xpm"

SettingsDialog::SettingsDialog(wxWindow *parent):
  wxDialog(parent,
           wxID_ANY,
           wxT("Configurações"),
           wxDefaultPosition,
           wxDefaultSize,
           wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER) {
  config = Config::getInstance();

  SetAutoLayout(true);

  SetSize(wxSize(600, 550));

  SetIcon(wxIcon(config_16x16_xpm));

  wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

  SetSizer(szTop);

  // Toolbar

  wxToolBar *toolBar = new wxToolBar(this,
                                     wxID_ANY,
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxTB_DEFAULT_STYLE | wxTB_HORZ_TEXT);

  toolBar->SetToolBitmapSize(wxSize(16, 16));

  toolBar->AddTool(DLG_CONFIG_TOOL_SAVE_ID, "Salvar", wxBitmap(save_16x16_xpm));

  toolBar->AddSeparator();

  toolBar->AddTool(DLG_CONFIG_TOOL_CLOSE_ID, "Fechar", wxBitmap(close_16x16_xpm));

  toolBar->Realize();

  szTop->Add(toolBar, 0, wxALL | wxEXPAND | wxGROW, 0);

  szTop->AddSpacer(3);

  // Notebook

  wxNotebook *notebook = new wxNotebook(this, wxID_ANY);

  szTop->Add(notebook, 1, wxALL|wxGROW|wxEXPAND, 0);

  // Geral configuration panel

  wxScrolledWindow *panelConfigGeneral = new wxScrolledWindow(notebook,
                                                              wxID_ANY,
                                                              wxDefaultPosition,
                                                              wxDefaultSize,
                                                              wxTAB_TRAVERSAL | wxHSCROLL | wxVSCROLL);

  notebook->AddPage(panelConfigGeneral, wxT("Geral"), false);

  wxBoxSizer *szPanelGeneralMargin = new wxBoxSizer(wxVERTICAL);
  panelConfigGeneral->SetSizer(szPanelGeneralMargin);

  wxBoxSizer *szPanelGeneral = new wxBoxSizer(wxVERTICAL);
  szPanelGeneralMargin->Add(szPanelGeneral, 1, wxALL | wxEXPAND | wxGROW, 5);

  wxFlexGridSizer *sizerGeneralFields = new wxFlexGridSizer(2, 5, 5);
  szPanelGeneral->Add(sizerGeneralFields, 1, wxALL | wxGROW, 5);

  checkBoxShowPacketBinary = new wxCheckBox(panelConfigGeneral, wxID_ANY, wxT(""));
  sizerGeneralFields->Add(checkBoxShowPacketBinary, 0, wxALL, 0);

  wxStaticText *label = new wxStaticText(panelConfigGeneral, wxID_ANY, wxT("Mostrar informação binária de todo o pacote TS."));
  sizerGeneralFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 0);

  // Database configuration panel

  wxScrolledWindow *panelConfigTS = new wxScrolledWindow(notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxHSCROLL|wxVSCROLL);

  notebook->AddPage(panelConfigTS, wxT("Transport Stream"), false);

  wxBoxSizer *szPanelTsMargin = new wxBoxSizer(wxVERTICAL);

  panelConfigTS->SetSizer(szPanelTsMargin);
  panelConfigTS->SetScrollbars(1, 1, 20, 20);

  wxBoxSizer *szPanelTs = new wxBoxSizer(wxVERTICAL);
  szPanelTsMargin->Add(szPanelTs, 1, wxALL | wxEXPAND | wxGROW, 5);

  // Margin

  wxBoxSizer *szTsInfo = new wxBoxSizer(wxVERTICAL);
  szPanelTs->Add(szTsInfo, 1, wxALL | wxEXPAND | wxGROW, 0);

  // Status bar

  statusBar = new wxStatusBar(this, wxID_ANY, wxST_SIZEGRIP);
  szTop->Add(statusBar, 0, wxALL | wxGROW | wxEXPAND, 0);

  // Top sizer ending configuration.

  this->CenterOnParent();

  // Load data

  if (config->hasKey("show-packet-binary-info") && config->getValueBool("show-packet-binary-info")) {
    checkBoxShowPacketBinary->SetValue(true);
  }
  else {
    checkBoxShowPacketBinary->SetValue(false);
  }

  // Event handlers

  checkBoxShowPacketBinary->Bind(wxEVT_CHECKBOX, &SettingsDialog::OnCheckBoxShowPacketBinaryInfo, this);
  Bind(wxEVT_TOOL, &SettingsDialog::OnSaveRequest, this, DLG_CONFIG_TOOL_SAVE_ID);
  Bind(wxEVT_TOOL, &SettingsDialog::OnCloseRequest, this, DLG_CONFIG_TOOL_CLOSE_ID);
  Bind(wxEVT_CHAR_HOOK, &SettingsDialog::OnCharHook, this);
}

void SettingsDialog::Terminate()
{
  Close();

  Destroy();
}

void SettingsDialog::OnSaveRequest(wxCommandEvent& WXUNUSED(event)) {

}

void SettingsDialog::OnCloseRequest(wxCommandEvent& WXUNUSED(event)) {
  Terminate();
}

void SettingsDialog::OnCharHook(wxKeyEvent& event) {
  //wxMessageBox(wxString::Format(wxT("Key code: %d"), event.GetKeyCode()));

  // Exit <ESC>
  if (event.GetKeyCode() == WXK_ESCAPE) {
    Terminate();
  }

  event.Skip();
}

void SettingsDialog::OnCheckBoxShowPacketBinaryInfo(wxCommandEvent& WXUNUSED(event)) {
  if (checkBoxShowPacketBinary->IsChecked()) {
    config->setKey("show-packet-binary-info", true);
  }
  else {
    config->setKey("show-packet-binary-info", false);
  }
}