#include "MainFrame.h"
#include "ComparePanel.h"
#include "CompressPanel.h"

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, "Image Processing Suite", wxDefaultPosition, wxSize(1000, 800))
{
    // Create menu bar
    wxMenuBar *menuBar = new wxMenuBar();
    
    // File menu
    wxMenu *fileMenu = new wxMenu();
    fileMenu->Append(wxID_EXIT, "&Exit\tCtrl+Q", "Exit the application");
    menuBar->Append(fileMenu, "&File");
    
    // Help menu
    wxMenu *helpMenu = new wxMenu();
    helpMenu->Append(wxID_ABOUT, "&About\tF1", "Show about dialog");
    menuBar->Append(helpMenu, "&Help");
    
    SetMenuBar(menuBar);
    
    // Bind menu events
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    
    // Create notebook for tabs
    notebook = new wxNotebook(this, wxID_ANY);
    
    // Create panels
    comparePanel = new ComparePanel(notebook);
    compressPanel = new CompressPanel(notebook);
    
    // Add tabs
    notebook->AddPage(comparePanel, "Image Compare");
    notebook->AddPage(compressPanel, "Image Compressor");
    
    // Main sizer
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(notebook, 1, wxEXPAND | wxALL, 5);
    
    SetSizer(mainSizer);
    
    // Create status bar
    CreateStatusBar();
    SetStatusText("Ready");
    
    Centre();
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("Image Processing Suite\nVersion 1.0\n\nFeatures:\n- Image Comparison (Template Matching, Histogram, MSE/PSNR)\n- Image Compression (JPEG quality adjustment, PNG compression)\n- Image Resizing\n\nBuilt with wxWidgets and OpenCV",
                 "About Image Processing Suite", wxOK | wxICON_INFORMATION);
}