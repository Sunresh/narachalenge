#include <wx/wx.h>
#include <opencv2/opencv.hpp>

// 1. Define the Application Class
class NaraApp : public wxApp {
public:
    virtual bool OnInit();
};

// 2. Define the Main Window (Frame)
class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);
private:
    void OnQuit(wxCommandEvent& event);
};
