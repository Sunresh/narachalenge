#include <wx/wx.h>
#include "MainFrame.h"

class ImageSuiteApp : public wxApp
{
public:
    virtual bool OnInit()
    {
        (new MainFrame())->Show();
        return true;
    }
};

wxIMPLEMENT_APP(ImageSuiteApp);