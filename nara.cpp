#include <wx/wx.h>
#include <opencv2/opencv.hpp>

class MainFrame : public wxFrame
{
public:
    wxTextCtrl *txtLog;
    wxStaticBitmap *previewArea;
    void DoWork()
    {
        SetStatusText("Processing...");
        txtLog->AppendText("Starting OpenCV Task...\n");

        // Simple OpenCV Task: Create a dummy image and check dimensions
        cv::Mat test = cv::Mat::zeros(100, 100, CV_8UC3);

        txtLog->AppendText("OpenCV Matrix Created: " + std::to_string(test.cols) + "px wide\n");
        SetStatusText("Task Complete");
    }
    void Coppy() {
        // Create a dummy OpenCV image (BGR)
        cv::Mat mat = cv::Mat::zeros(240, 320, CV_8UC3);
        // Draw a random color circle to show it's "live"
        cv::circle(mat, cv::Point(160, 120), 50, cv::Scalar(rand()%255, rand()%255, rand()%255), -1);

        // --- CONVERSION: Mat (BGR) -> wxImage (RGB) -> wxBitmap ---
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB); // wxWidgets uses RGB
        
        wxImage img(rgb.cols, rgb.rows, rgb.data, true);
        previewArea->SetBitmap(wxBitmap(img.Copy())); // Copy avoids memory issues

        txtLog->AppendText("Preview updated with new circle.\n");
    }
    MainFrame() : wxFrame(NULL, wxID_ANY, "Nara UI Control", wxDefaultPosition, wxSize(400, 300))
    {
        // 1. Create a layout container (Vertical Sizer)
        wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

        previewArea = new wxStaticBitmap(this, wxID_ANY, wxBitmap(320, 240));
        sizer->Add(previewArea, 0, wxALIGN_CENTER | wxALL, 10);
        // 2. Add UI Elements
        txtLog = new wxTextCtrl(this, wxID_ANY, "System Ready...\n", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
        wxButton *btnAction = new wxButton(this, wxID_ANY, "Run OpenCV Task");

        // 3. Arrange elements
        sizer->Add(txtLog, 1, wxEXPAND | wxALL, 10); // '1' makes it stretch
        sizer->Add(btnAction, 0, wxALIGN_CENTER | wxBOTTOM, 10);
        this->SetSizer(sizer);

        // 4. Create a status bar at the bottom
        CreateStatusBar();
        SetStatusText("Idle");

        // 5. Task Logic: When button is clicked
        btnAction->Bind(wxEVT_BUTTON, [this](wxCommandEvent &){ DoWork(); Coppy(); });
    }
};

class NaraApp : public wxApp
{
public:
    virtual bool OnInit()
    {
        (new MainFrame())->Show();
        return true;
    }
};

wxIMPLEMENT_APP(NaraApp);
