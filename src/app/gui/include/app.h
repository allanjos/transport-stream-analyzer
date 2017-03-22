#ifndef APP_H
#define APP_H

#include "gui_lib.h"

class App: public wxApp
{
    public:
        virtual bool OnInit();
};

#endif // APP_H