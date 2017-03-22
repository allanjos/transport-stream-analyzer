#ifndef DLG_MAIN_H
#define DLG_MAIN_H


// Utility libraries
#include "gui_lib.h"
#include "ts/ts.h"
#include "logger.h"

class MainFrame: public wxFrame, public TransportStreamListener, public LoggerListener
{
    private:

        unsigned long currentTsPacket;

        wxString filePath;

        wxTextCtrl *textCtrlFilePath;
        wxStaticText *labelFileGeneralInformation;

        wxBoxSizer *sizerContentTop;

        wxTextCtrl *textCtrlPacketCurrent;
        wxStaticText *labelPacketsCount;

        wxTextCtrl *textCtrlSyncByte;
        wxTextCtrl *textTransportErrorIndicator;
        wxTextCtrl *textCtrlPayloadUnitStartIndicator;
        wxTextCtrl *textCtrlTransportPriority;
        wxTextCtrl *textCtrlPID;
        wxTextCtrl *textCtrlTransportScramblingControl;
        wxTextCtrl *textCtrlAdaptationFieldControl;
        wxTextCtrl *textCtrlContinuityCounter;

        wxTextCtrl *textCtrlPacketRaw;

        wxGrid *gridBytes;

        wxProgressDialog *progressDialog;

        TransportStream *ts;

        TsPacket *packet;

        void OnOpenSettingsDialog(wxCommandEvent& event);
        void OnTsFileOpen(wxCommandEvent& event);
        void OnExit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);

        // Transport stream listener callbacks

        void callbackOnLoad(TransportStream *ts);

        void callbackOnLoadPacketOffsets(TransportStream *ts);

        void onLogMessage(const char *message, LogType mode);

        void loadCurrentTsPacket();
        void loadFirstTsPacket();
        void loadLastTsPacket();
        void loadPrevTsPacket();
        void loadNextTsPacket();

        void OnLoadFirstPacket(wxCommandEvent& event);
        void OnLoadLastPacket(wxCommandEvent& event);
        void OnLoadPrevPacket(wxCommandEvent& event);
        void OnLoadNextPacket(wxCommandEvent& event);
        void OnPacketCurrentPressEnter(wxCommandEvent& event);

        void resetFileInformation();
        void resetPacketInformation();

    public:

        MainFrame();

        ~MainFrame();

        void AddPacketInformationPanel(wxWindow *parent, wxSizer *sizer);
};


#endif // DLG_MAIN_H