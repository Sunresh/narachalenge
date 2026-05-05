#include "nara.h"

// Macro that generates the main() function for you
wxIMPLEMENT_APP(NaraApp);

bool NaraApp::OnInit() {
    MainFrame *frame = new MainFrame("Nara Project UI");
    frame->Show(true);
    return true;
}

MainFrame::MainFrame(const wxString& title) 
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)) {
    
    // Example: Create a button
    wxButton* btn = new wxButton(this, wxID_EXIT, "Close App", wxPoint(50, 50));
    Bind(wxEVT_BUTTON, &MainFrame::OnQuit, this, wxID_EXIT);
}

void MainFrame::OnQuit(wxCommandEvent& event) {
    Close(true);
}
