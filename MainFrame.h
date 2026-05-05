#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>
#include <wx/notebook.h>

class ComparePanel;
class CompressPanel;

class MainFrame : public wxFrame
{
private:
    wxNotebook *notebook;
    ComparePanel *comparePanel;
    CompressPanel *compressPanel;

public:
    MainFrame();
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};

#endif