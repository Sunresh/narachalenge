#include <wx/wx.h>
#include <opencv2/opencv.hpp>
#include <wx/statline.h>

class MainFrame : public wxFrame
{
private:
    wxStaticBitmap *sourcePreview;
    wxStaticBitmap *referencePreview;
    wxStaticBitmap *resultPreview;
    wxTextCtrl *txtLog;
    
    cv::Mat sourceImage;
    cv::Mat referenceImage;
    cv::Mat resultImage;
    
    std::string sourcePath;
    std::string referencePath;

public:
    MainFrame() : wxFrame(NULL, wxID_ANY, "Image Compare Tool", wxDefaultPosition, wxSize(900, 800))
    {
        // Create main sizer
        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
        
        // Create top panel for images
        wxBoxSizer *imageSizer = new wxBoxSizer(wxHORIZONTAL);
        
        // Source image panel
        wxStaticBox *sourceBox = new wxStaticBox(this, wxID_ANY, "Source Image");
        wxStaticBoxSizer *sourceSizer = new wxStaticBoxSizer(sourceBox, wxVERTICAL);
        sourcePreview = new wxStaticBitmap(this, wxID_ANY, wxBitmap(300, 200), wxDefaultPosition, wxSize(300, 200));
        wxButton *loadSourceBtn = new wxButton(this, wxID_ANY, "Load Source Image");
        sourceSizer->Add(sourcePreview, 0, wxALIGN_CENTER | wxALL, 10);
        sourceSizer->Add(loadSourceBtn, 0, wxALIGN_CENTER | wxALL, 5);
        
        // Reference image panel
        wxStaticBox *referenceBox = new wxStaticBox(this, wxID_ANY, "Reference Image");
        wxStaticBoxSizer *referenceSizer = new wxStaticBoxSizer(referenceBox, wxVERTICAL);
        referencePreview = new wxStaticBitmap(this, wxID_ANY, wxBitmap(300, 200), wxDefaultPosition, wxSize(300, 200));
        wxButton *loadReferenceBtn = new wxButton(this, wxID_ANY, "Load Reference Image");
        referenceSizer->Add(referencePreview, 0, wxALIGN_CENTER | wxALL, 10);
        referenceSizer->Add(loadReferenceBtn, 0, wxALIGN_CENTER | wxALL, 5);
        
        // Result panel
        wxStaticBox *resultBox = new wxStaticBox(this, wxID_ANY, "Result");
        wxStaticBoxSizer *resultSizer = new wxStaticBoxSizer(resultBox, wxVERTICAL);
        resultPreview = new wxStaticBitmap(this, wxID_ANY, wxBitmap(300, 200), wxDefaultPosition, wxSize(300, 200));
        resultSizer->Add(resultPreview, 0, wxALIGN_CENTER | wxALL, 10);
        
        imageSizer->Add(sourceSizer, 1, wxALL | wxEXPAND, 5);
        imageSizer->Add(referenceSizer, 1, wxALL | wxEXPAND, 5);
        imageSizer->Add(resultSizer, 1, wxALL | wxEXPAND, 5);
        
        // Control panel
        wxBoxSizer *controlSizer = new wxBoxSizer(wxHORIZONTAL);
        wxButton *compareBtn = new wxButton(this, wxID_ANY, "Compare Images", wxDefaultPosition, wxSize(150, 40));
        wxButton *clearBtn = new wxButton(this, wxID_ANY, "Clear All", wxDefaultPosition, wxSize(150, 40));
        
        controlSizer->Add(compareBtn, 0, wxALIGN_CENTER | wxALL, 10);
        controlSizer->Add(clearBtn, 0, wxALIGN_CENTER | wxALL, 10);
        
        // Log panel
        wxStaticBox *logBox = new wxStaticBox(this, wxID_ANY, "Log");
        wxStaticBoxSizer *logSizer = new wxStaticBoxSizer(logBox, wxVERTICAL);
        txtLog = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
        logSizer->Add(txtLog, 1, wxEXPAND | wxALL, 5);
        
        // Add everything to main sizer
        mainSizer->Add(imageSizer, 2, wxEXPAND | wxALL, 10);
        mainSizer->Add(controlSizer, 0, wxALIGN_CENTER | wxALL, 10);
        mainSizer->Add(logSizer, 1, wxEXPAND | wxALL, 10);
        
        this->SetSizer(mainSizer);
        
        // Bind events
        loadSourceBtn->Bind(wxEVT_BUTTON, &MainFrame::OnLoadSource, this);
        loadReferenceBtn->Bind(wxEVT_BUTTON, &MainFrame::OnLoadReference, this);
        compareBtn->Bind(wxEVT_BUTTON, &MainFrame::OnCompareImages, this);
        clearBtn->Bind(wxEVT_BUTTON, &MainFrame::OnClearAll, this);
        
        // Center the window
        Centre();
    }
    
    void OnLoadSource(wxCommandEvent &event)
    {
        wxFileDialog openDialog(this, "Select Source Image", "", "", 
                                "Image files (*.jpg;*.png;*.bmp)|*.jpg;*.png;*.bmp", 
                                wxFD_OPEN | wxFD_FILE_MUST_EXIST);
        
        if (openDialog.ShowModal() == wxID_CANCEL)
            return;
        
        sourcePath = std::string(openDialog.GetPath().mb_str());
        sourceImage = cv::imread(sourcePath);
        
        if (sourceImage.empty())
        {
            txtLog->AppendText("Error: Could not load source image.\n");
            return;
        }
        
        // Display the image
        UpdatePreview(sourcePreview, sourceImage);
        txtLog->AppendText("Source image loaded successfully.\n");
    }
    
    void OnLoadReference(wxCommandEvent &event)
    {
        wxFileDialog openDialog(this, "Select Reference Image", "", "", 
                                "Image files (*.jpg;*.png;*.bmp)|*.jpg;*.png;*.bmp", 
                                wxFD_OPEN | wxFD_FILE_MUST_EXIST);
        
        if (openDialog.ShowModal() == wxID_CANCEL)
            return;
        
        referencePath = std::string(openDialog.GetPath().mb_str());
        referenceImage = cv::imread(referencePath);
        
        if (referenceImage.empty())
        {
            txtLog->AppendText("Error: Could not load reference image.\n");
            return;
        }
        
        // Display the image
        UpdatePreview(referencePreview, referenceImage);
        txtLog->AppendText("Reference image loaded successfully.\n");
    }
    
    void OnCompareImages(wxCommandEvent &event)
    {
        if (sourceImage.empty() || referenceImage.empty())
        {
            txtLog->AppendText("Error: Please load both source and reference images first.\n");
            return;
        }
        
        txtLog->AppendText("\n=== Starting Comparison ===\n");
        
        // Method 1: Template Matching
        if (referenceImage.cols <= sourceImage.cols && referenceImage.rows <= sourceImage.rows)
        {
            PerformTemplateMatching();
        }
        else
        {
            txtLog->AppendText("Warning: Reference image is larger than source image. Template matching skipped.\n");
        }
        
        // Method 2: Histogram Comparison
        PerformHistogramComparison();
        
        // Method 3: Structural Similarity (MSE)
        PerformMSEComparison();
        
        txtLog->AppendText("=== Comparison Complete ===\n\n");
    }
    
    void PerformTemplateMatching()
    {
        cv::Mat result;
        cv::matchTemplate(sourceImage, referenceImage, result, cv::TM_CCOEFF_NORMED);
        
        double minVal, maxVal;
        cv::Point minLoc, maxLoc;
        cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
        
        txtLog->AppendText(wxString::Format("Template Matching Score: %.4f\n", maxVal));
        
        if (maxVal >= 0.7) // 70% match threshold
        {
            txtLog->AppendText(wxString::Format("✓ Match found at position (%d, %d)\n", maxLoc.x, maxLoc.y));
            
            // Draw rectangle on source image
            resultImage = sourceImage.clone();
            cv::rectangle(resultImage, maxLoc, 
                         cv::Point(maxLoc.x + referenceImage.cols, maxLoc.y + referenceImage.rows), 
                         cv::Scalar(0, 0, 255), 2);
            
            UpdatePreview(resultPreview, resultImage);
        }
        else
        {
            txtLog->AppendText("✗ No significant match found.\n");
            resultImage = sourceImage.clone();
            UpdatePreview(resultPreview, resultImage);
        }
    }
    
    void PerformHistogramComparison()
    {
        // Convert to HSV for better color comparison
        cv::Mat sourceHSV, referenceHSV;
        cv::cvtColor(sourceImage, sourceHSV, cv::COLOR_BGR2HSV);
        cv::cvtColor(referenceImage, referenceHSV, cv::COLOR_BGR2HSV);
        
        int histSize[] = {50, 60};
        float hRanges[] = {0, 180};
        float sRanges[] = {0, 256};
        const float* ranges[] = {hRanges, sRanges};
        int channels[] = {0, 1};
        
        cv::Mat sourceHist, referenceHist;
        cv::calcHist(&sourceHSV, 1, channels, cv::Mat(), sourceHist, 2, histSize, ranges, true, false);
        cv::calcHist(&referenceHSV, 1, channels, cv::Mat(), referenceHist, 2, histSize, ranges, true, false);
        
        cv::normalize(sourceHist, sourceHist, 0, 1, cv::NORM_MINMAX);
        cv::normalize(referenceHist, referenceHist, 0, 1, cv::NORM_MINMAX);
        
        double similarity = cv::compareHist(sourceHist, referenceHist, cv::HISTCMP_CORREL);
        
        txtLog->AppendText(wxString::Format("Histogram Similarity: %.4f\n", similarity));
        
        if (similarity > 0.7)
            txtLog->AppendText("✓ Images have similar color distribution.\n");
        else if (similarity > 0.4)
            txtLog->AppendText("⚠ Images have moderately similar color distribution.\n");
        else
            txtLog->AppendText("✗ Images have different color distribution.\n");
    }
    
    void PerformMSEComparison()
    {
        if (sourceImage.size() != referenceImage.size())
        {
            txtLog->AppendText("MSE Comparison: Images have different sizes. Resizing reference to match source.\n");
            cv::Mat resizedReference;
            cv::resize(referenceImage, resizedReference, sourceImage.size());
            double mse = CalculateMSE(sourceImage, resizedReference);
            double psnr = CalculatePSNR(sourceImage, resizedReference);
            txtLog->AppendText(wxString::Format("MSE (Mean Squared Error): %.4f\n", mse));
            txtLog->AppendText(wxString::Format("PSNR (Peak Signal-to-Noise Ratio): %.2f dB\n", psnr));
        }
        else
        {
            double mse = CalculateMSE(sourceImage, referenceImage);
            double psnr = CalculatePSNR(sourceImage, referenceImage);
            txtLog->AppendText(wxString::Format("MSE (Mean Squared Error): %.4f\n", mse));
            txtLog->AppendText(wxString::Format("PSNR (Peak Signal-to-Noise Ratio): %.2f dB\n", psnr));
        }
    }
    
    double CalculateMSE(const cv::Mat& img1, const cv::Mat& img2)
    {
        cv::Mat diff;
        cv::absdiff(img1, img2, diff);
        diff.convertTo(diff, CV_32F);
        diff = diff.mul(diff);
        cv::Scalar s = cv::sum(diff);
        double mse = (s[0] + s[1] + s[2]) / (img1.total() * 3);
        return mse;
    }
    
    double CalculatePSNR(const cv::Mat& img1, const cv::Mat& img2)
    {
        double mse = CalculateMSE(img1, img2);
        if (mse == 0)
            return 100; // Identical images
        
        double psnr = 10 * log10(255 * 255 / mse);
        return psnr;
    }
    
    void OnClearAll(wxCommandEvent &event)
    {
        sourceImage.release();
        referenceImage.release();
        resultImage.release();
        
        sourcePath.clear();
        referencePath.clear();
        
        // Clear previews
        sourcePreview->SetBitmap(wxBitmap(300, 200));
        referencePreview->SetBitmap(wxBitmap(300, 200));
        resultPreview->SetBitmap(wxBitmap(300, 200));
        
        txtLog->Clear();
        txtLog->AppendText("All images and logs cleared.\n");
    }
    
    void UpdatePreview(wxStaticBitmap* preview, cv::Mat mat)
    {
        if (mat.empty())
            return;
        
        cv::Mat display;
        cv::Mat resized;
        
        // Resize image to fit preview area while maintaining aspect ratio
        int maxWidth = 280;
        int maxHeight = 180;
        
        double aspect = (double)mat.cols / mat.rows;
        int newWidth = maxWidth;
        int newHeight = (int)(maxWidth / aspect);
        
        if (newHeight > maxHeight)
        {
            newHeight = maxHeight;
            newWidth = (int)(maxHeight * aspect);
        }
        
        cv::resize(mat, resized, cv::Size(newWidth, newHeight));
        
        // Convert BGR to RGB for wxWidgets
        cv::cvtColor(resized, display, cv::COLOR_BGR2RGB);
        
        wxImage img(display.cols, display.rows, display.data, true);
        preview->SetBitmap(wxBitmap(img));
        
        // Force refresh
        preview->Refresh();
    }
};

class CompareApp : public wxApp
{
public:
    virtual bool OnInit()
    {
        (new MainFrame())->Show();
        return true;
    }
};

wxIMPLEMENT_APP(CompareApp);