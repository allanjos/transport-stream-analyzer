#include "app.h"
#include "dlg_main.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    wxFileSystem::AddHandler(new wxZipFSHandler);

    MainFrame *frame = new MainFrame();

    frame->Show(true);

    return true;
}