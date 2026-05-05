#include "ComparePanel.h"
#include <fstream>

ComparePanel::ComparePanel(wxWindow *parent) : wxPanel(parent)
{
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    
    // Top control panel
    wxBoxSizer *controlSizer = new wxBoxSizer(wxHORIZONTAL);
    
    // Method selection
    controlSizer->Add(new wxStaticText(this, wxID_ANY, "Comparison Method:"), 0, wxALIGN_CENTER | wxRIGHT, 5);
    methodChoice = new wxChoice(this, wxID_ANY);
    methodChoice->Append("Template Matching");
    methodChoice->Append("Histogram Comparison");
    methodChoice->Append("MSE/PSNR (Full Image)");
    methodChoice->Append("All Methods");
    methodChoice->SetSelection(3);
    controlSizer->Add(methodChoice, 0, wxALIGN_CENTER | wxRIGHT, 20);
    
    // Threshold slider
    controlSizer->Add(new wxStaticText(this, wxID_ANY, "Match Threshold:"), 0, wxALIGN_CENTER | wxRIGHT, 5);
    thresholdSlider = new wxSlider(this, wxID_ANY, 70, 0, 100, wxDefaultPosition, wxSize(150, -1));
    controlSizer->Add(thresholdSlider, 0, wxALIGN_CENTER | wxRIGHT, 5);
    thresholdValue = new wxStaticText(this, wxID_ANY, "70%");
    controlSizer->Add(thresholdValue, 0, wxALIGN_CENTER);
    
    mainSizer->Add(controlSizer, 0, wxALL | wxEXPAND, 10);
    
    // Image panels
    wxBoxSizer *imageSizer = new wxBoxSizer(wxHORIZONTAL);
    
    // Source image panel
    wxStaticBox *sourceBox = new wxStaticBox(this, wxID_ANY, "Source Image");
    wxStaticBoxSizer *sourceSizer = new wxStaticBoxSizer(sourceBox, wxVERTICAL);
    sourcePreview = new wxStaticBitmap(this, wxID_ANY, wxBitmap(300, 200), wxDefaultPosition, wxSize(350, 250));
    wxButton *loadSourceBtn = new wxButton(this, wxID_ANY, "Load Source Image");
    sourceSizer->Add(sourcePreview, 0, wxALIGN_CENTER | wxALL, 10);
    sourceSizer->Add(loadSourceBtn, 0, wxALIGN_CENTER | wxALL, 5);
    
    // Reference image panel
    wxStaticBox *referenceBox = new wxStaticBox(this, wxID_ANY, "Reference Image");
    wxStaticBoxSizer *referenceSizer = new wxStaticBoxSizer(referenceBox, wxVERTICAL);
    referencePreview = new wxStaticBitmap(this, wxID_ANY, wxBitmap(300, 200), wxDefaultPosition, wxSize(350, 250));
    wxButton *loadReferenceBtn = new wxButton(this, wxID_ANY, "Load Reference Image");
    referenceSizer->Add(referencePreview, 0, wxALIGN_CENTER | wxALL, 10);
    referenceSizer->Add(loadReferenceBtn, 0, wxALIGN_CENTER | wxALL, 5);
    
    // Result panel
    wxStaticBox *resultBox = new wxStaticBox(this, wxID_ANY, "Result");
    wxStaticBoxSizer *resultSizer = new wxStaticBoxSizer(resultBox, wxVERTICAL);
    resultPreview = new wxStaticBitmap(this, wxID_ANY, wxBitmap(300, 200), wxDefaultPosition, wxSize(350, 250));
    resultSizer->Add(resultPreview, 0, wxALIGN_CENTER | wxALL, 10);
    
    imageSizer->Add(sourceSizer, 1, wxALL | wxEXPAND, 5);
    imageSizer->Add(referenceSizer, 1, wxALL | wxEXPAND, 5);
    imageSizer->Add(resultSizer, 1, wxALL | wxEXPAND, 5);
    
    mainSizer->Add(imageSizer, 2, wxEXPAND | wxALL, 10);
    
    // Action buttons
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *compareBtn = new wxButton(this, wxID_ANY, "Compare Images", wxDefaultPosition, wxSize(150, 40));
    wxButton *clearBtn = new wxButton(this, wxID_ANY, "Clear All", wxDefaultPosition, wxSize(150, 40));
    wxButton *saveResultBtn = new wxButton(this, wxID_ANY, "Save Result", wxDefaultPosition, wxSize(150, 40));
    
    buttonSizer->Add(compareBtn, 0, wxALIGN_CENTER | wxALL, 10);
    buttonSizer->Add(clearBtn, 0, wxALIGN_CENTER | wxALL, 10);
    buttonSizer->Add(saveResultBtn, 0, wxALIGN_CENTER | wxALL, 10);
    
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER, 10);
    
    // Log panel
    wxStaticBox *logBox = new wxStaticBox(this, wxID_ANY, "Comparison Log");
    wxStaticBoxSizer *logSizer = new wxStaticBoxSizer(logBox, wxVERTICAL);
    txtLog = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    logSizer->Add(txtLog, 1, wxEXPAND | wxALL, 5);
    
    mainSizer->Add(logSizer, 1, wxEXPAND | wxALL, 10);
    
    SetSizer(mainSizer);
    
    // Bind events
    loadSourceBtn->Bind(wxEVT_BUTTON, &ComparePanel::OnLoadSource, this);
    loadReferenceBtn->Bind(wxEVT_BUTTON, &ComparePanel::OnLoadReference, this);
    compareBtn->Bind(wxEVT_BUTTON, &ComparePanel::OnCompareImages, this);
    clearBtn->Bind(wxEVT_BUTTON, &ComparePanel::OnClearAll, this);
    saveResultBtn->Bind(wxEVT_BUTTON, &ComparePanel::OnSaveResult, this);
    
    thresholdSlider->Bind(wxEVT_SLIDER, [this](wxCommandEvent&) {
        int value = thresholdSlider->GetValue();
        thresholdValue->SetLabel(wxString::Format("%d%%", value));
    });
}

void ComparePanel::OnLoadSource(wxCommandEvent &event)
{
    wxFileDialog openDialog(this, "Select Source Image", "", "", 
                            "Image files (*.jpg;*.png;*.bmp)|*.jpg;*.png;*.bmp", 
                            wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    
    if (openDialog.ShowModal() == wxID_CANCEL)
        return;
    
    std::string path = std::string(openDialog.GetPath().mb_str());
    sourceImage = cv::imread(path);
    
    if (sourceImage.empty())
    {
        txtLog->AppendText("Error: Could not load source image.\n");
        return;
    }
    
    UpdatePreview(sourcePreview, sourceImage);
    txtLog->AppendText("Source image loaded successfully.\n");
}

void ComparePanel::OnLoadReference(wxCommandEvent &event)
{
    wxFileDialog openDialog(this, "Select Reference Image", "", "", 
                            "Image files (*.jpg;*.png;*.bmp)|*.jpg;*.png;*.bmp", 
                            wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    
    if (openDialog.ShowModal() == wxID_CANCEL)
        return;
    
    std::string path = std::string(openDialog.GetPath().mb_str());
    referenceImage = cv::imread(path);
    
    if (referenceImage.empty())
    {
        txtLog->AppendText("Error: Could not load reference image.\n");
        return;
    }
    
    UpdatePreview(referencePreview, referenceImage);
    txtLog->AppendText("Reference image loaded successfully.\n");
}

void ComparePanel::OnCompareImages(wxCommandEvent &event)
{
    if (sourceImage.empty() || referenceImage.empty())
    {
        txtLog->AppendText("Error: Please load both source and reference images first.\n");
        return;
    }
    
    txtLog->AppendText("\n=== Starting Comparison ===\n");
    
    int method = methodChoice->GetSelection();
    double threshold = thresholdSlider->GetValue() / 100.0;
    
    switch(method)
    {
        case 0: // Template Matching only
            PerformTemplateMatching(threshold);
            break;
        case 1: // Histogram only
            PerformHistogramComparison();
            break;
        case 2: // MSE/PSNR only
            PerformMSEComparison();
            break;
        case 3: // All methods
            PerformTemplateMatching(threshold);
            PerformHistogramComparison();
            PerformMSEComparison();
            break;
    }
    
    txtLog->AppendText("=== Comparison Complete ===\n\n");
}

void ComparePanel::PerformTemplateMatching(double threshold)
{
    if (referenceImage.cols > sourceImage.cols || referenceImage.rows > sourceImage.rows)
    {
        txtLog->AppendText("Warning: Reference image is larger than source image. Template matching skipped.\n");
        return;
    }
    
    cv::Mat result;
    cv::matchTemplate(sourceImage, referenceImage, result, cv::TM_CCOEFF_NORMED);
    
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
    
    txtLog->AppendText(wxString::Format("\n[Template Matching] Score: %.4f (Threshold: %.2f)\n", maxVal, threshold));
    
    if (maxVal >= threshold)
    {
        txtLog->AppendText(wxString::Format("✓ Match found at position (%d, %d)\n", maxLoc.x, maxLoc.y));
        
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

void ComparePanel::PerformHistogramComparison()
{
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
    
    txtLog->AppendText(wxString::Format("\n[Histogram Comparison] Similarity: %.4f\n", similarity));
    
    if (similarity > 0.7)
        txtLog->AppendText("✓ Images have very similar color distribution.\n");
    else if (similarity > 0.4)
        txtLog->AppendText("⚠ Images have moderately similar color distribution.\n");
    else
        txtLog->AppendText("✗ Images have different color distribution.\n");
}

void ComparePanel::PerformMSEComparison()
{
    if (sourceImage.size() != referenceImage.size())
    {
        txtLog->AppendText("\n[MSE/PSNR] Images have different sizes. Resizing reference to match source.\n");
        cv::Mat resizedReference;
        cv::resize(referenceImage, resizedReference, sourceImage.size());
        double mse = CalculateMSE(sourceImage, resizedReference);
        double psnr = CalculatePSNR(sourceImage, resizedReference);
        txtLog->AppendText(wxString::Format("MSE: %.4f\n", mse));
        txtLog->AppendText(wxString::Format("PSNR: %.2f dB\n", psnr));
    }
    else
    {
        double mse = CalculateMSE(sourceImage, referenceImage);
        double psnr = CalculatePSNR(sourceImage, referenceImage);
        txtLog->AppendText(wxString::Format("\n[MSE/PSNR] MSE: %.4f\n", mse));
        txtLog->AppendText(wxString::Format("PSNR: %.2f dB\n", psnr));
    }
}

double ComparePanel::CalculateMSE(const cv::Mat& img1, const cv::Mat& img2)
{
    cv::Mat diff;
    cv::absdiff(img1, img2, diff);
    diff.convertTo(diff, CV_32F);
    diff = diff.mul(diff);
    cv::Scalar s = cv::sum(diff);
    double mse = (s[0] + s[1] + s[2]) / (img1.total() * 3);
    return mse;
}

double ComparePanel::CalculatePSNR(const cv::Mat& img1, const cv::Mat& img2)
{
    double mse = CalculateMSE(img1, img2);
    if (mse == 0)
        return 100;
    
    double psnr = 10 * log10(255 * 255 / mse);
    return psnr;
}

void ComparePanel::OnClearAll(wxCommandEvent &event)
{
    sourceImage.release();
    referenceImage.release();
    resultImage.release();
    
    sourcePreview->SetBitmap(wxBitmap(350, 250));
    referencePreview->SetBitmap(wxBitmap(350, 250));
    resultPreview->SetBitmap(wxBitmap(350, 250));
    
    txtLog->Clear();
    txtLog->AppendText("All images and logs cleared.\n");
}

void ComparePanel::OnSaveResult(wxCommandEvent &event)
{
    if (resultImage.empty())
    {
        txtLog->AppendText("No result image to save. Please run comparison first.\n");
        return;
    }
    
    wxFileDialog saveDialog(this, "Save Result Image", "", "result.jpg", 
                            "JPEG files (*.jpg)|*.jpg|PNG files (*.png)|*.png", 
                            wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    
    if (saveDialog.ShowModal() == wxID_CANCEL)
        return;
    
    std::string path = std::string(saveDialog.GetPath().mb_str());
    cv::imwrite(path, resultImage);
    txtLog->AppendText(wxString::Format("Result saved to: %s\n", path));
}

void ComparePanel::UpdatePreview(wxStaticBitmap* preview, cv::Mat mat)
{
    if (mat.empty())
        return;
    
    cv::Mat display;
    cv::Mat resized;
    
    int maxWidth = 320;
    int maxHeight = 230;
    
    double aspect = (double)mat.cols / mat.rows;
    int newWidth = maxWidth;
    int newHeight = (int)(maxWidth / aspect);
    
    if (newHeight > maxHeight)
    {
        newHeight = maxHeight;
        newWidth = (int)(maxHeight * aspect);
    }
    
    cv::resize(mat, resized, cv::Size(newWidth, newHeight));
    cv::cvtColor(resized, display, cv::COLOR_BGR2RGB);
    
    wxImage img(display.cols, display.rows, display.data, true);
    preview->SetBitmap(wxBitmap(img));
    preview->Refresh();
}