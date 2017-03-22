/**
 * Settings dialog.
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include "gui_lib.h"
#include "config.h"

class SettingsDialog : public wxDialog
{
  private:

    Config *config;

    wxCheckBox *checkBoxShowPacketBinary;

    /**
     * Status bar for informative messages.
     */
    wxStatusBar *statusBar;

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    /**
     * Save information request.
     */
    void OnSaveRequest(wxCommandEvent& event);

    /**
     * Close interface request.
     */
    void OnCloseRequest(wxCommandEvent &event);

    void OnCharHook(wxKeyEvent& event);

    void OnCheckBoxShowPacketBinaryInfo(wxCommandEvent& event);

  public:

    /**
     * Constructor and initializer.
     */
    SettingsDialog(wxWindow *parent);
};

#endif