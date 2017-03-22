/**
 * Application main window.
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

#include <sys/types.h>
#include <sys/stat.h>

#include "gui_controls.h"
#include "gui.h"
#include "app.h"
#include "logger.h"
#include "dlg_main.h"
#include "dlg_about.h"
#include "dlg_settings.h"
#include "app_version.h"

#include "pixmaps/close_16x16.xpm"
#include "pixmaps/help_16x16.xpm"
#include "pixmaps/config_16x16.xpm"
#include "pixmaps/folder_16x16.xpm"
#include "pixmaps/icon.xpm"
#include "pixmaps/list_allbackward.xpm"
#include "pixmaps/list_allforward.xpm"
#include "pixmaps/list_forward.xpm"
#include "pixmaps/list_backward.xpm"
#include "pixmaps/save_16x16.xpm"
#include "pixmaps/info_16x16.xpm"

using namespace std;

MainFrame::MainFrame(): wxFrame(NULL, wxID_ANY, wxT("Editor de arquivos de Transport Stream"))
{
    ts = NULL;
    currentTsPacket = 0;
    packet = NULL;

    Freeze();

    SetIcon(wxIcon(icon_xpm));

    // Initial configuration

    SetAutoLayout(true);

    // Size and position

    int x, y, width, height;

    wxClientDisplayRect(&x, &y, &width, &height);

    if (width > 1024) {
        width = 1024;
    }
    else {
        width -= 100;
    }

    if (height >= 900) {
        height = 850;
    }
    else {
        height -= 100;
    }

    SetSize(wxSize(width, height));

    Centre();

    // Menu

    wxMenu *menuFile = new wxMenu;

    // Open file
    wxMenuItem *menuItemFileOpen = new wxMenuItem(menuFile,
                                                  DLG_MAIN_MENU_TS_FILE_OPEN_ID,
                                                  wxT("&Abrir arquivo de transport stream\tCtrl-O"),
                                                  wxT("Abrir e carregar arquivo de transport stream armazenado no computador local."));
    menuItemFileOpen->SetBitmap(wxBitmap(folder_16x16_xpm));
    menuFile->Append(menuItemFileOpen);

    // Save file
    wxMenuItem *menuItemFileSave = new wxMenuItem(menuFile,
                                                  DLG_MAIN_MENU_TS_FILE_SAVE_ID,
                                                  wxT("&Salvar modificações\tCtrl-H"),
                                                  wxT("Salvar modificações no arquivo de transport stream carregado."));
    menuItemFileSave->SetBitmap(wxBitmap(save_16x16_xpm));
    menuFile->Append(menuItemFileSave);

    // Settings
    wxMenuItem *menuItemSettings = new wxMenuItem(menuFile,
                                                  DLG_MAIN_MENU_SETTINGS_ID,
                                                  wxT("&Configurações\tCtrl-H"),
                                                  wxT("Abrir diálogo de configurações."));
    menuItemSettings->SetBitmap(wxBitmap(config_16x16_xpm));
    menuFile->Append(menuItemSettings);

    // Close
    wxMenuItem *menuItemClose = new wxMenuItem(menuFile,
                                               wxID_EXIT,
                                               wxT("&Fechar aplicação\tALT-F4"),
                                               wxT("Finalizar aplicação."));
    menuItemClose->SetBitmap(wxBitmap(close_16x16_xpm));
    menuFile->Append(menuItemClose);

    wxMenu *menuHelp = new wxMenu;

    wxMenuItem *menuItemDoc = new wxMenuItem(menuFile,
                                             DLG_MAIN_MENU_DOCUMENTATION_ID,
                                             wxT("&Documentação\tF1"),
                                             wxT("Documentação com informações de uso da aplicação."));
    menuItemDoc->SetBitmap(wxBitmap(help_16x16_xpm));
    menuHelp->Append(menuItemDoc);

    wxMenuItem *menuItemAbout = new wxMenuItem(menuFile,
                                               wxID_ABOUT,
                                               wxT("&Sobre a aplicação\tCTRL-F1"),
                                               wxT("Informações gerais sobre a aplicação."));
    menuItemAbout->SetBitmap(wxBitmap(info_16x16_xpm));
    menuHelp->Append(menuItemAbout);

    wxMenuBar *menuBar = new wxMenuBar;

    menuBar->Append(menuFile, "&Geral");
    menuBar->Append(menuHelp, wxT("&Informações"));

    SetMenuBar(menuBar);

    // Status bar

    CreateStatusBar();

    SetStatusText("");

    // Top sizer

    wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

    SetSizer(szTop);

    // Toolbar

    wxBoxSizer *szToolbars = new wxBoxSizer(wxHORIZONTAL);

    szTop->Add(szToolbars, 0, wxALL | wxEXPAND, 0);

    wxAuiToolBar *toolBar = new wxAuiToolBar(this,
                                             wxID_ANY,
                                             wxDefaultPosition,
                                             wxDefaultSize,
                                             wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_TEXT | wxAUI_TB_HORZ_TEXT | wxAUI_TB_PLAIN_BACKGROUND);
    szToolbars->Add(toolBar, 1, wxALL | wxEXPAND, 0);


    toolBar->SetToolBitmapSize(wxSize(16, 16));

    toolBar->AddTool((int) DLG_MAIN_TOOLBAR_CONFIG_MAIN_ID,
                     wxT(""),
                     wxBitmap(config_16x16_xpm));

    toolBar->AddTool((int) DLG_MAIN_TOOLBAR_TS_SAVE_ID,
                     wxT(""),
                     wxBitmap(save_16x16_xpm));

    toolBar->Realize();

    // Main panel

    wxPanel *panelMain = new wxPanel(this, wxID_ANY);

    panelMain->SetAutoLayout(true);

    szTop->Add(panelMain, 1, wxALL | wxGROW, 0);

    wxBoxSizer *szMain = new wxBoxSizer(wxVERTICAL);

    panelMain->SetSizer(szMain);

    // File information

    wxStaticBoxSizer *sizerFileInfo = new wxStaticBoxSizer(wxVERTICAL, panelMain, wxT("Informação gerais sobre o arquivo:"));

    szMain->Add(sizerFileInfo, 0, wxALL | wxGROW, 5);

    wxFlexGridSizer *sizerFileInfoFields = new wxFlexGridSizer(2, 5, 5);

    sizerFileInfo->Add(sizerFileInfoFields, 1, wxALL | wxGROW, 5);

    // File path

    wxStaticText *label = new wxStaticText(panelMain, wxID_ANY, wxT("Arquivo de transport stream: "));
    sizerFileInfoFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 0);

    wxBoxSizer *szTsEditControls = new wxBoxSizer(wxHORIZONTAL);
    sizerFileInfoFields->Add(szTsEditControls, 0, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 0);

    textCtrlFilePath = new wxTextCtrl(panelMain, wxID_ANY);
    szTsEditControls->Add(textCtrlFilePath, 1, wxALL | wxEXPAND, 0);

    wxBitmapButton *buttonOpenTsFile = new wxBitmapButton(panelMain, wxID_ANY, wxBitmap(folder_16x16_xpm), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    szTsEditControls->Add(buttonOpenTsFile, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);

    sizerFileInfoFields->AddGrowableCol(1);

    // General information

    label = new wxStaticText(panelMain, wxID_ANY, wxT("Informações gerais: "));
    sizerFileInfoFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 0);

    labelFileGeneralInformation = new wxStaticText(panelMain, wxID_ANY, wxT(""));
    sizerFileInfoFields->Add(labelFileGeneralInformation, 0, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 0);

    // Add UI with TS information controls

    AddPacketInformationPanel(panelMain, szMain);

    Thaw();

    // Event handlers

    Bind(wxEVT_MENU, &MainFrame::OnTsFileOpen, this, DLG_MAIN_MENU_TS_FILE_OPEN_ID);
    Bind(wxEVT_MENU, &MainFrame::OnOpenSettingsDialog, this, DLG_MAIN_MENU_SETTINGS_ID);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);

    Bind(wxEVT_TOOL, &MainFrame::OnOpenSettingsDialog, this, DLG_MAIN_TOOLBAR_CONFIG_MAIN_ID);

    buttonOpenTsFile->Bind(wxEVT_BUTTON, &MainFrame::OnTsFileOpen, this);

    //Bind(wxEVT_TOOL, &MainFrame::OnTsFileOpen, this, DLG_MAIN_TOOLBAR_TS_OPEN_MAIN_ID);

    Logger::registerListener(this);

    // Progress dialog

    progressDialog = NULL;
}

MainFrame::~MainFrame() {
    if (ts != NULL) {
        delete ts;
    }
}

void MainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void MainFrame::OnOpenSettingsDialog(wxCommandEvent& WXUNUSED(event)) {
  SettingsDialog settingsDialog(this);

  settingsDialog.ShowModal();
}

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    AboutDialog aboutDialog(this);

    aboutDialog.ShowModal();
}

void MainFrame::AddPacketInformationPanel(wxWindow *parent, wxSizer *sizer) {
    sizerContentTop = new wxStaticBoxSizer(wxVERTICAL, parent, wxT("Pacotes TS contidos no arquivo:"));
    sizer->Add(sizerContentTop, 1, (wxALL & ~wxTOP) | wxGROW, 5);

    wxBoxSizer *sizerFileContentToolbar = new wxBoxSizer(wxHORIZONTAL);
    sizerContentTop->Add(sizerFileContentToolbar, 0, (wxALL & ~wxTOP) | wxALIGN_RIGHT, 0);

    // Packets list step information

    wxStaticText *label = new wxStaticText(parent, wxID_ANY, wxT("Pacote "));
    sizerFileContentToolbar->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);

    textCtrlPacketCurrent = new wxTextCtrl(parent, DLG_MAIN_TEXTCTRL_PACKET_CURRENT_ID, wxT("0"), wxDefaultPosition, wxSize(55, -1), wxTE_RIGHT | wxTE_PROCESS_ENTER);
    sizerFileContentToolbar->Add(textCtrlPacketCurrent, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);

    label = new wxStaticText(parent, wxID_ANY, wxT(" de "));
    sizerFileContentToolbar->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);

    labelPacketsCount = new wxStaticText(parent, DLG_MAIN_TEXTCTRL_PACKETS_COUNT_ID, wxT("0"));
    sizerFileContentToolbar->Add(labelPacketsCount, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);

    // Toolbar

    wxAuiToolBar *toolBar = new wxAuiToolBar(parent,
                                             wxID_ANY,
                                             wxDefaultPosition,
                                             wxDefaultSize,
                                             wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_TEXT | wxAUI_TB_HORZ_TEXT | wxAUI_TB_PLAIN_BACKGROUND);
    sizerFileContentToolbar->Add(toolBar, 0, wxALL | wxGROW, 0);

    toolBar->SetToolBitmapSize(wxSize(16, 16));

    toolBar->AddTool((int) DLG_MAIN_TOOLBAR_LIST_ALL_BACKWARD_ID,
                     wxT(""),
                     wxBitmap(list_allbackward_xpm));

    toolBar->AddTool((int) DLG_MAIN_TOOLBAR_LIST_BACKWARD_ID,
                     wxT(""),
                     wxBitmap(list_backward_xpm));

    toolBar->AddTool((int) DLG_MAIN_TOOLBAR_LIST_FORWARD_ID,
                     wxT(""),
                     wxBitmap(list_forward_xpm));

    toolBar->AddTool((int) DLG_MAIN_TOOLBAR_LIST_ALL_FORWARD_ID,
                     wxT(""),
                     wxBitmap(list_allforward_xpm));

    toolBar->Realize();

    // Content

    wxBoxSizer *sizerFileContent = new wxBoxSizer(wxHORIZONTAL);
    sizerContentTop->Add(sizerFileContent, 1, (wxALL & ~wxTOP) | wxGROW, 0);

    // Packet header fields

    wxStaticBoxSizer *sizerTsHeader = new wxStaticBoxSizer(wxVERTICAL, parent, wxT("Cabeçalho do pacote TS:"));
    sizerFileContent->Add(sizerTsHeader, 0, wxALL, 0);

    wxFlexGridSizer *sizerPacketHeaderFields = new wxFlexGridSizer(2, 1, 1);
    sizerTsHeader->Add(sizerPacketHeaderFields, 0, wxALL | wxGROW, 0);

    // Sync byte

    label = new wxStaticText(parent, wxID_ANY, wxT("Sync byte: "));
    sizerPacketHeaderFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 0);

    textCtrlSyncByte = new wxTextCtrl(parent, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(Gui::TEXTCTRL_NUMBER_HEIGHT, -1));
    sizerPacketHeaderFields->Add(textCtrlSyncByte, 0, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 0);

    // Transport error indicator

    label = new wxStaticText(parent, wxID_ANY, wxT("Transport error indicator: "));
    sizerPacketHeaderFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 0);

    textTransportErrorIndicator = new wxTextCtrl(parent, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(Gui::TEXTCTRL_NUMBER_HEIGHT, -1));
    sizerPacketHeaderFields->Add(textTransportErrorIndicator, 0, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 0);

    // Payload unit start indicator

    label = new wxStaticText(parent, wxID_ANY, wxT("Payload unit start indicator: "));
    sizerPacketHeaderFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 0);

    textCtrlPayloadUnitStartIndicator = new wxTextCtrl(parent, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(Gui::TEXTCTRL_NUMBER_HEIGHT, -1));
    sizerPacketHeaderFields->Add(textCtrlPayloadUnitStartIndicator, 0, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 0);

    // Transport priority

    label = new wxStaticText(parent, wxID_ANY, wxT("Transport priority: "));
    sizerPacketHeaderFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 0);

    textCtrlTransportPriority = new wxTextCtrl(parent, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(Gui::TEXTCTRL_NUMBER_HEIGHT, -1));
    sizerPacketHeaderFields->Add(textCtrlTransportPriority, 0, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 0);

    // PID

    label = new wxStaticText(parent, wxID_ANY, wxT("PID: "));
    sizerPacketHeaderFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 0);

    textCtrlPID = new wxTextCtrl(parent, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(Gui::TEXTCTRL_NUMBER_HEIGHT, -1));
    sizerPacketHeaderFields->Add(textCtrlPID, 0, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 0);

    // Transport scrambling control

    label = new wxStaticText(parent, wxID_ANY, wxT("Transport scrambling control: "));
    sizerPacketHeaderFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 0);

    textCtrlTransportScramblingControl = new wxTextCtrl(parent, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(Gui::TEXTCTRL_NUMBER_HEIGHT, -1));
    sizerPacketHeaderFields->Add(textCtrlTransportScramblingControl, 0, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 0);

    // Adaptation field control

    label = new wxStaticText(parent, wxID_ANY, wxT("Adaptation field control: "));
    sizerPacketHeaderFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 0);

    textCtrlAdaptationFieldControl = new wxTextCtrl(parent, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(Gui::TEXTCTRL_NUMBER_HEIGHT, -1));
    sizerPacketHeaderFields->Add(textCtrlAdaptationFieldControl, 0, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 0);

    // Continuity counter

    label = new wxStaticText(parent, wxID_ANY, wxT("Continuity counter: "));
    sizerPacketHeaderFields->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 0);

    textCtrlContinuityCounter = new wxTextCtrl(parent, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(Gui::TEXTCTRL_NUMBER_HEIGHT, -1));
    sizerPacketHeaderFields->Add(textCtrlContinuityCounter, 0, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 0);

    // Packet raw content

    wxBoxSizer *sizerPacketPayload = new wxBoxSizer(wxVERTICAL);
    sizerFileContent->Add(sizerPacketPayload, 1, wxLEFT | wxGROW, 3);

    textCtrlPacketRaw = new wxTextCtrl(parent, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    wxFont font;
    font = textCtrlPacketRaw->GetFont();
    font.SetFamily(wxFONTFAMILY_TELETYPE);
    textCtrlPacketRaw->SetFont(font);
    sizerPacketPayload->Add(textCtrlPacketRaw, 1, wxALL | wxGROW, 0);

    // Grid

    wxStaticBoxSizer *sizerRawBytes = new wxStaticBoxSizer(wxVERTICAL, parent, wxT("Bytes do pacote TS:"));
    sizerFileContent->Add(sizerRawBytes, 0, wxALL & ~wxTOP, 3);

    gridBytes = new wxGrid(parent, wxID_ANY);
    sizerRawBytes->Add(gridBytes, 0, wxALL, 0);


    gridBytes->CreateGrid(15, 13); // rows, cols

    gridBytes->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);

    //gridBytes->SetRowSize(0, 60);
    gridBytes->SetColSize(0, 50);

    gridBytes->HideColLabels();
    gridBytes->HideRowLabels();

    gridBytes->SetDefaultColSize(25, true);

    wxFont fontGrid(8, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    gridBytes->SetDefaultCellFont(fontGrid);

    // Event handlers

    Bind(wxEVT_TOOL, &MainFrame::OnLoadFirstPacket, this, DLG_MAIN_TOOLBAR_LIST_ALL_BACKWARD_ID);
    Bind(wxEVT_TOOL, &MainFrame::OnLoadLastPacket, this, DLG_MAIN_TOOLBAR_LIST_ALL_FORWARD_ID);
    Bind(wxEVT_TOOL, &MainFrame::OnLoadPrevPacket, this, DLG_MAIN_TOOLBAR_LIST_BACKWARD_ID);
    Bind(wxEVT_TOOL, &MainFrame::OnLoadNextPacket, this, DLG_MAIN_TOOLBAR_LIST_FORWARD_ID);

    textCtrlPacketCurrent->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnPacketCurrentPressEnter, this);
}

void MainFrame::OnTsFileOpen(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog *dialog = new wxFileDialog(this,
                                            wxT("Escolha o arquivo de transport stream"), wxEmptyString, wxEmptyString,
                                            wxT("Arquivos de Tranport Stream (*.ts)|*.ts|Outas extensões (*.*)|*.*"),
                                            wxFD_OPEN);

    if (dialog->ShowModal() != wxID_OK) {
        return;
    }

    filePath = dialog->GetPath();

    dialog->Destroy();

    // Show file path

    textCtrlFilePath->SetValue(filePath);

    // Transport stream loader

    if (ts == NULL) {
        ts = new TransportStream();
    }

    ts->setListener(this);

    // Progress dialog

    progressDialog = new wxProgressDialog(wxT("Aguarde"),
                                          wxT("Carregando arquivo de transport stream"),
                                          0,
                                          this,
                                          wxPD_APP_MODAL);

    std::string path = std::string(filePath.mb_str());

    ts->setPath(path);

    SetStatusText(wxT("Carregando arquivo de transport stream..."));

    currentTsPacket = 0;

    ts->load();

    if (ts->isLoaded()) {
        SetStatusText(wxT("Arquivo carregado. Carregando lista de pacotes..."));

        ts->loadPacketOffsets();

        labelPacketsCount->SetLabelText(wxString::Format("%ld", ts->getPacketOffsets().size()));
        labelPacketsCount->Layout();
        labelPacketsCount->GetContainingSizer()->Layout();
        sizerContentTop->Layout();

        SetStatusText(wxT("Lista de pacotes carregada."));

        delete progressDialog;

        loadCurrentTsPacket();
    }
    else {
        delete progressDialog;
    }
}

void MainFrame::callbackOnLoad(TransportStream *ts) {
    labelFileGeneralInformation->SetLabelText(wxString::Format(wxT("%ld byte(s)"), ts->getFileSize()));
    labelFileGeneralInformation->Layout();
    labelFileGeneralInformation->GetContainingSizer()->Layout();
}

void MainFrame::callbackOnLoadPacketOffsets(TransportStream *) {
}

void MainFrame::loadCurrentTsPacket() {
    Logger::debug("Carregando informações do pacote TS corrente");

    resetPacketInformation();

    if (!ts->isLoaded()) {
        Logger::error("Arquivo TS não está carregado.");

        return;
    }

    // Parse TS Packet

    if (packet != NULL) {
        delete packet;
        packet = NULL;
    }

    packet = new TsPacket();

    int rc = ts->parsePacket(currentTsPacket, packet);

    if (rc != 0) {
        Logger::error("Erro no processamento do pacote TS %d. Código de execução: %d", currentTsPacket, rc);

        return;
    }

    textCtrlPacketCurrent->SetValue(wxString::Format("%ld", currentTsPacket + 1));

    textCtrlSyncByte->SetValue(wxString(wxT("0x47")));
    textTransportErrorIndicator->SetValue(wxString::Format("%d", packet->getHeader()->getErrorIndicator()));
    textCtrlPayloadUnitStartIndicator->SetValue(wxString::Format("%d", packet->getHeader()->getUnitStartIndicator()));
    textCtrlTransportPriority->SetValue(wxString::Format("%d", packet->getHeader()->getTransportPriority()));
    textCtrlPID->SetValue(wxString::Format("%d", packet->getHeader()->getPid()));
    textCtrlTransportScramblingControl->SetValue(wxString::Format("%d", packet->getHeader()->getTransportScramblingControl()));
    textCtrlAdaptationFieldControl->SetValue(wxString::Format("%d", packet->getHeader()->getAdaptationFieldControl()));
    textCtrlContinuityCounter->SetValue(wxString::Format("%d", packet->getHeader()->getContinuityCounter()));

    SetStatusText(wxString::Format(wxT("Informações do pacote TS %ld de %ld carregadas."), currentTsPacket + 1, ts->getPacketOffsets().size()));

    int row = 0;
    int col = 0;
    int cols = 13;

    gridBytes->Freeze();

    gridBytes->ClearGrid();

    for (unsigned int index = 0; index < packet->getBytes().size(); index++) {
        col = (int) (index % cols);
        row = ceil(index / cols);

        wxString str = wxString::Format("%.2x", packet->getBytes().at(index));

        str = str.Upper();

        if (index == 0 && str.Cmp("47") == 0) {
            gridBytes->SetCellTextColour(row, col, wxT("#548364"));
        }
        else if (str.Cmp("FF") == 0) {
            gridBytes->SetCellTextColour(row, col, wxT("#1647e7"));
        }
        else if (str.Cmp("00") == 0) {
            gridBytes->SetCellTextColour(row, col, wxT("#555555"));
        }
        else {
            gridBytes->SetCellTextColour(row, col, *wxBLACK);
        }

        gridBytes->SetCellValue(row, col, str);
    }

    gridBytes->Thaw();
}

void MainFrame::loadPrevTsPacket() {
    if (ts == NULL) {
        // TODO Handle this
        return;
    }

    if (ts->getPacketOffsets().size() <= 0)  {
        // TODO Handle this
        return;
    }

    if (currentTsPacket <= 0) {
        // TODO Handle this
        return;
    }

    --currentTsPacket;

    loadCurrentTsPacket();
}

void MainFrame::loadNextTsPacket() {
    if (ts == NULL) {
        // TODO Handle this
        return;
    }

    if (ts->getPacketOffsets().size() <= 0)  {
        // TODO Handle this
        return;
    }

    if (currentTsPacket > ts->getPacketOffsets().size() - 1) {
        // TODO Handle this
        return;
    }

    ++currentTsPacket;

    loadCurrentTsPacket();
}

void MainFrame::loadFirstTsPacket() {
    if (ts->getPacketOffsets().size() <= 0)  {
        // TODO Handle this
        return;
    }

    currentTsPacket = 0;

    loadCurrentTsPacket();
}

void MainFrame::loadLastTsPacket() {
    if (ts->getPacketOffsets().size() <= 0)  {
        // TODO Handle this
        return;
    }

    currentTsPacket = ts->getPacketOffsets().size() - 1;

    loadCurrentTsPacket();
}

void MainFrame::OnLoadFirstPacket(wxCommandEvent& WXUNUSED(event)) {
    loadFirstTsPacket();
}

void MainFrame::OnLoadLastPacket(wxCommandEvent& WXUNUSED(event)) {
    loadLastTsPacket();
}

void MainFrame::OnLoadPrevPacket(wxCommandEvent& WXUNUSED(event)) {
    loadPrevTsPacket();
}

void MainFrame::OnLoadNextPacket(wxCommandEvent& WXUNUSED(event)) {
    loadNextTsPacket();
}

void MainFrame::OnPacketCurrentPressEnter(wxCommandEvent& WXUNUSED(event)) {
    if (textCtrlPacketCurrent->GetValue().Length() <= 0) {
        return;
    }

    if (!ts->isLoaded()) {
        SetStatusText(wxT("É necessário carregar um arquivo de TS para navegar entre os pacotes do mesmo."));
        return;
    }

    long current;

    textCtrlPacketCurrent->GetValue().ToLong(&current);

    if (current <= 0) {
        SetStatusText(wxT("Número de pacote informado é inválido."));
        return;
    }

    if ((unsigned long) current >= ts->getPacketOffsets().size()) {
        SetStatusText(wxT("Número de pacote informado é inválido."));
        return;
    }

    currentTsPacket = current - 1;

    loadCurrentTsPacket();
}

void MainFrame::onLogMessage(const char *message, LogType mode) {
    textCtrlPacketRaw->Freeze();

    if (textCtrlPacketRaw->GetValue().Length() > 0) {
        textCtrlPacketRaw->SetValue(textCtrlPacketRaw->GetValue() + wxT("\n"));
    }

    wxString str;

    switch (mode) {
        case LOG_INFO:
            str = wxT("[INFO ] ");
            break;
        case LOG_ERROR:
            str = wxT("[ERROR] ");
            break;
        default:
            str = wxT("[DEBUG] ");
            break;
    }

    textCtrlPacketRaw->SetValue(textCtrlPacketRaw->GetValue() + str + wxString(message, wxConvUTF8));

    textCtrlPacketRaw->Thaw();
}

void MainFrame::resetFileInformation() {
    textCtrlFilePath->SetValue(wxT(""));

    MainFrame::resetPacketInformation();
}

void MainFrame::resetPacketInformation() {
    textCtrlPacketRaw->SetValue(wxT(""));

    textCtrlPacketCurrent->SetValue(wxT("0"));

    labelFileGeneralInformation->SetLabelText(wxT(""));
    labelFileGeneralInformation->Layout();
    labelFileGeneralInformation->GetContainingSizer()->Layout();

    textCtrlSyncByte->SetValue(wxT(""));
    textTransportErrorIndicator->SetValue(wxT(""));
    textCtrlPayloadUnitStartIndicator->SetValue(wxT(""));
    textCtrlTransportPriority->SetValue(wxT(""));
    textCtrlPID->SetValue(wxT(""));
    textCtrlTransportScramblingControl->SetValue(wxT(""));
    textCtrlAdaptationFieldControl->SetValue(wxT(""));
    textCtrlContinuityCounter->SetValue(wxT(""));

    SetStatusText(wxT(""));
}