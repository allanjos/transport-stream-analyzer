/**
 * About dialog.
 */

#ifndef DLG_ABOUT_H
#define DLG_ABOUT_H

#include "gui_lib.h"

class AboutDialog: public wxDialog
{
  private:

    void OnEraseBackground(wxEraseEvent &event);

    /**
     * Event table declaration - wxWidgets
     */
    DECLARE_EVENT_TABLE()

  public:
    /**
     * Constructor and initializer.
     */
    AboutDialog(wxWindow *parent);

    /**
     * Closes and terminates the dialog.
     */
    void Terminate();

    void OnRightClick(wxMouseEvent& event);
};

#endif