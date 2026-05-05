#include "CompressPanel.h"
#include <fstream>
#include <algorithm>
#include <cmath>

CompressPanel::CompressPanel(wxWindow *parent) : wxPanel(parent)
{
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    // ==================== COMPRESSION SETTINGS ====================
    wxStaticBox *controlBox = new wxStaticBox(this, wxID_ANY, "Compression Settings");
    wxStaticBoxSizer *controlSizer = new wxStaticBoxSizer(controlBox, wxVERTICAL);

    // Format selection
    wxBoxSizer *formatSizer = new wxBoxSizer(wxHORIZONTAL);
    formatSizer->Add(new wxStaticText(this, wxID_ANY, "Output Format:"), 0, wxALIGN_CENTER | wxRIGHT, 10);
    formatChoice = new wxChoice(this, wxID_ANY);
    formatChoice->Append("JPEG (lossy, best for photos)");
    formatChoice->Append("PNG (lossless, best for graphics)");
    formatChoice->Append("WebP (modern, good compression)");
    formatChoice->SetSelection(0);
    formatSizer->Add(formatChoice, 1, wxEXPAND);
    controlSizer->Add(formatSizer, 0, wxEXPAND | wxALL, 5);

    // Quality slider (for JPEG/WebP)
    wxBoxSizer *qualitySizer = new wxBoxSizer(wxHORIZONTAL);
    qualitySizer->Add(new wxStaticText(this, wxID_ANY, "Quality:"), 0, wxALIGN_CENTER | wxRIGHT, 10);
    qualitySlider = new wxSlider(this, wxID_ANY, 85, 1, 100, wxDefaultPosition, wxSize(300, -1));
    qualitySizer->Add(qualitySlider, 1, wxEXPAND | wxRIGHT, 10);
    qualityValue = new wxStaticText(this, wxID_ANY, "85%");
    qualitySizer->Add(qualityValue);
    controlSizer->Add(qualitySizer, 0, wxEXPAND | wxALL, 5);

    controlSizer->AddSpacer(10);
    
    // ==================== RESIZE SETTINGS ====================
    wxStaticBox *resizeBox = new wxStaticBox(this, wxID_ANY, "Resize Settings");
    wxStaticBoxSizer *resizeSizer = new wxStaticBoxSizer(resizeBox, wxVERTICAL);
    
    // Resize method selection
    scalePercentRadio = new wxRadioButton(this, wxID_ANY, "Scale by Percentage", wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    customSizeRadio = new wxRadioButton(this, wxID_ANY, "Custom Width/Height");
    presetSizeRadio = new wxRadioButton(this, wxID_ANY, "Preset Dimensions");
    
    resizeSizer->Add(scalePercentRadio, 0, wxALL, 5);
    resizeSizer->Add(customSizeRadio, 0, wxALL, 5);
    resizeSizer->Add(presetSizeRadio, 0, wxALL, 5);
    
    resizeSizer->AddSpacer(5);
    
    // Scale by percentage controls
    wxBoxSizer *scaleSizer = new wxBoxSizer(wxHORIZONTAL);
    scaleSizer->Add(new wxStaticText(this, wxID_ANY, "Scale (%):"), 0, wxALIGN_CENTER | wxRIGHT, 10);
    scaleSlider = new wxSlider(this, wxID_ANY, 100, 10, 100, wxDefaultPosition, wxSize(200, -1));
    scaleSizer->Add(scaleSlider, 1, wxEXPAND | wxRIGHT, 10);
    scaleValue = new wxStaticText(this, wxID_ANY, "100%");
    scaleSizer->Add(scaleValue);
    resizeSizer->Add(scaleSizer, 0, wxEXPAND | wxLEFT, 20);
    
    resizeSizer->AddSpacer(5);
    
    // Custom size controls
    wxFlexGridSizer *customGrid = new wxFlexGridSizer(2, 5, 5);
    customGrid->Add(new wxStaticText(this, wxID_ANY, "Width (px):"), 0, wxALIGN_CENTER);
    widthInput = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(100, -1));
    customGrid->Add(widthInput, 0, wxEXPAND);
    
    customGrid->Add(new wxStaticText(this, wxID_ANY, "Height (px):"), 0, wxALIGN_CENTER);
    heightInput = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(100, -1));
    customGrid->Add(heightInput, 0, wxEXPAND);
    
    keepAspectCheck = new wxCheckBox(this, wxID_ANY, "Keep aspect ratio");
    customGrid->Add(keepAspectCheck, 0, wxALIGN_CENTER);
    customGrid->Add(new wxStaticText(this, wxID_ANY, ""), 0); // Empty spacer
    
    resizeSizer->Add(customGrid, 0, wxEXPAND | wxLEFT, 20);
    
    resizeSizer->AddSpacer(5);
    
    // Preset size controls
    wxBoxSizer *presetSizer = new wxBoxSizer(wxHORIZONTAL);
    presetSizer->Add(new wxStaticText(this, wxID_ANY, "Preset:"), 0, wxALIGN_CENTER | wxRIGHT, 10);
    presetSizeChoice = new wxChoice(this, wxID_ANY);
    presetSizeChoice->Append("Original Size");
    presetSizeChoice->Append("1920x1080 (Full HD)");
    presetSizeChoice->Append("1280x720 (HD)");
    presetSizeChoice->Append("854x480 (480p)");
    presetSizeChoice->Append("640x360 (360p)");
    presetSizeChoice->Append("320x240 (240p)");
    presetSizeChoice->Append("Instagram Square (1080x1080)");
    presetSizeChoice->Append("Facebook Cover (820x312)");
    presetSizeChoice->Append("Twitter Post (1600x900)");
    presetSizeChoice->SetSelection(0);
    presetSizer->Add(presetSizeChoice, 1, wxEXPAND);
    resizeSizer->Add(presetSizer, 0, wxEXPAND | wxLEFT, 20);
    
    controlSizer->Add(resizeSizer, 0, wxEXPAND | wxALL, 5);
    
    // ==================== TARGET FILE SIZE ====================
    wxStaticBox *targetBox = new wxStaticBox(this, wxID_ANY, "Target File Size (Experimental)");
    wxStaticBoxSizer *targetSizer = new wxStaticBoxSizer(targetBox, wxVERTICAL);
    
    wxBoxSizer *targetGrid = new wxBoxSizer(wxHORIZONTAL);
    targetGrid->Add(new wxStaticText(this, wxID_ANY, "Target Size (KB):"), 0, wxALIGN_CENTER | wxRIGHT, 10);
    
    targetSizeChoice = new wxChoice(this, wxID_ANY);
    targetSizeChoice->Append("No target (use quality)");
    targetSizeChoice->Append("50 KB (Very Small)");
    targetSizeChoice->Append("100 KB (Thumbnail)");
    targetSizeChoice->Append("200 KB (Small)");
    targetSizeChoice->Append("500 KB (Medium)");
    targetSizeChoice->Append("1 MB (Good Quality)");
    targetSizeChoice->Append("2 MB (High Quality)");
    targetSizeChoice->Append("5 MB (Very High)");
    targetSizeChoice->SetSelection(0);
    targetGrid->Add(targetSizeChoice, 1, wxEXPAND | wxRIGHT, 10);
    
    currentSizeInfo = new wxStaticText(this, wxID_ANY, "");
    targetGrid->Add(currentSizeInfo, 0, wxALIGN_CENTER);
    
    targetSizer->Add(targetGrid, 0, wxEXPAND | wxALL, 5);
    
    targetSizeInfo = new wxStaticText(this, wxID_ANY, "ℹ️ Target size tries to achieve specific file size by adjusting quality");
    targetSizer->Add(targetSizeInfo, 0, wxALL | wxLEFT, 10);
    
    controlSizer->Add(targetBox, 0, wxEXPAND | wxALL, 5);
    
    mainSizer->Add(controlSizer, 0, wxEXPAND | wxALL, 10);

    // ==================== IMAGE PREVIEWS ====================
    wxBoxSizer *previewSizer = new wxBoxSizer(wxHORIZONTAL);

    wxStaticBox *originalBox = new wxStaticBox(this, wxID_ANY, "Original Image");
    wxStaticBoxSizer *originalSizer = new wxStaticBoxSizer(originalBox, wxVERTICAL);
    originalPreview = new wxStaticBitmap(this, wxID_ANY, wxBitmap(350, 300), wxDefaultPosition, wxSize(400, 350));
    originalSizer->Add(originalPreview, 0, wxALIGN_CENTER | wxALL, 10);

    wxStaticBox *compressedBox = new wxStaticBox(this, wxID_ANY, "Compressed Image");
    wxStaticBoxSizer *compressedSizer = new wxStaticBoxSizer(compressedBox, wxVERTICAL);
    compressedPreview = new wxStaticBitmap(this, wxID_ANY, wxBitmap(350, 300), wxDefaultPosition, wxSize(400, 350));
    compressedSizer->Add(compressedPreview, 0, wxALIGN_CENTER | wxALL, 10);

    previewSizer->Add(originalSizer, 1, wxALL | wxEXPAND, 5);
    previewSizer->Add(compressedSizer, 1, wxALL | wxEXPAND, 5);

    mainSizer->Add(previewSizer, 1, wxEXPAND | wxALL, 10);

    // ==================== IMAGE INFORMATION PANEL ====================
    wxStaticBox *infoBox = new wxStaticBox(this, wxID_ANY, "Image Information");
    wxStaticBoxSizer *infoSizer = new wxStaticBoxSizer(infoBox, wxVERTICAL);

    wxFlexGridSizer *gridSizer = new wxFlexGridSizer(2, 5, 10);

    gridSizer->Add(new wxStaticText(this, wxID_ANY, "📐 Original Dimensions:"), 0, wxALIGN_LEFT);
    originalDimensions = new wxStaticText(this, wxID_ANY, "Not loaded");
    gridSizer->Add(originalDimensions, 0, wxALIGN_LEFT);

    gridSizer->Add(new wxStaticText(this, wxID_ANY, "💾 Original File Size:"), 0, wxALIGN_LEFT);
    originalFileSize = new wxStaticText(this, wxID_ANY, "Not loaded");
    gridSizer->Add(originalFileSize, 0, wxALIGN_LEFT);

    gridSizer->Add(new wxStaticText(this, wxID_ANY, "📐 Compressed Dimensions:"), 0, wxALIGN_LEFT);
    compressedDimensions = new wxStaticText(this, wxID_ANY, "Not compressed");
    gridSizer->Add(compressedDimensions, 0, wxALIGN_LEFT);

    gridSizer->Add(new wxStaticText(this, wxID_ANY, "💾 Compressed File Size:"), 0, wxALIGN_LEFT);
    compressedFileSize = new wxStaticText(this, wxID_ANY, "Not compressed");
    gridSizer->Add(compressedFileSize, 0, wxALIGN_LEFT);

    gridSizer->Add(new wxStaticText(this, wxID_ANY, "🎯 Space Saved:"), 0, wxALIGN_LEFT);
    compressionRatio = new wxStaticText(this, wxID_ANY, "Not compressed");
    gridSizer->Add(compressionRatio, 0, wxALIGN_LEFT);

    infoSizer->Add(gridSizer, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(infoSizer, 0, wxEXPAND | wxALL, 10);

    // ==================== ACTION BUTTONS ====================
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *loadBtn = new wxButton(this, wxID_ANY, "📂 Load Image", wxDefaultPosition, wxSize(130, 40));
    wxButton *compressBtn = new wxButton(this, wxID_ANY, "🔧 Compress Image", wxDefaultPosition, wxSize(130, 40));
    wxButton *saveBtn = new wxButton(this, wxID_ANY, "💾 Save Compressed", wxDefaultPosition, wxSize(130, 40));

    buttonSizer->Add(loadBtn, 0, wxALIGN_CENTER | wxALL, 5);
    buttonSizer->Add(compressBtn, 0, wxALIGN_CENTER | wxALL, 5);
    buttonSizer->Add(saveBtn, 0, wxALIGN_CENTER | wxALL, 5);

    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER, 10);

    // ==================== LOG PANEL ====================
    wxStaticBox *logBox = new wxStaticBox(this, wxID_ANY, "Compression Log");
    wxStaticBoxSizer *logSizer = new wxStaticBoxSizer(logBox, wxVERTICAL);
    txtLog = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    logSizer->Add(txtLog, 1, wxEXPAND | wxALL, 5);

    mainSizer->Add(logSizer, 1, wxEXPAND | wxALL, 10);

    SetSizer(mainSizer);

    // ==================== BIND EVENTS ====================
    loadBtn->Bind(wxEVT_BUTTON, &CompressPanel::OnLoadImage, this);
    compressBtn->Bind(wxEVT_BUTTON, &CompressPanel::OnCompressImage, this);
    saveBtn->Bind(wxEVT_BUTTON, &CompressPanel::OnSaveImage, this);

    qualitySlider->Bind(wxEVT_SLIDER, [this](wxCommandEvent &) {
        int value = qualitySlider->GetValue();
        qualityValue->SetLabel(wxString::Format("%d%%", value));
    });

    scaleSlider->Bind(wxEVT_SLIDER, [this](wxCommandEvent &) {
        int value = scaleSlider->GetValue();
        scaleValue->SetLabel(wxString::Format("%d%%", value));
        if (scalePercentRadio->GetValue()) {
            int width = originalImage.cols * value / 100;
            int height = originalImage.rows * value / 100;
            widthInput->SetValue(wxString::Format("%d", width));
            heightInput->SetValue(wxString::Format("%d", height));
        }
    });

    scalePercentRadio->Bind(wxEVT_RADIOBUTTON, &CompressPanel::OnResizeMethodChanged, this);
    customSizeRadio->Bind(wxEVT_RADIOBUTTON, &CompressPanel::OnResizeMethodChanged, this);
    presetSizeRadio->Bind(wxEVT_RADIOBUTTON, &CompressPanel::OnResizeMethodChanged, this);
    
    presetSizeChoice->Bind(wxEVT_CHOICE, &CompressPanel::OnPresetSizeChanged, this);
    targetSizeChoice->Bind(wxEVT_CHOICE, &CompressPanel::OnTargetSizeChanged, this);
    
    widthInput->Bind(wxEVT_TEXT, [this](wxCommandEvent&) {
        if (keepAspectCheck->GetValue() && !originalImage.empty()) {
            long width;
            if (widthInput->GetValue().ToLong(&width)) {
                int newHeight = (originalImage.rows * width) / originalImage.cols;
                heightInput->SetValue(wxString::Format("%d", newHeight));
            }
        }
    });
    
    heightInput->Bind(wxEVT_TEXT, [this](wxCommandEvent&) {
        if (keepAspectCheck->GetValue() && !originalImage.empty()) {
            long height;
            if (heightInput->GetValue().ToLong(&height)) {
                int newWidth = (originalImage.cols * height) / originalImage.rows;
                widthInput->SetValue(wxString::Format("%d", newWidth));
            }
        }
    });

    formatChoice->Bind(wxEVT_CHOICE, [this](wxCommandEvent &) {
        bool isJPEG = (formatChoice->GetSelection() == 0);
        qualitySlider->Enable(isJPEG);
        qualityValue->Enable(isJPEG);
    });
    
    // Initialize controls
    OnResizeMethodChanged();
}

// ==================== RESIZE METHOD HANDLER ====================
// Parameterless version for initialization
void CompressPanel::OnResizeMethodChanged()
{
    bool scaleMode = scalePercentRadio->GetValue();
    bool customMode = customSizeRadio->GetValue();
    bool presetMode = presetSizeRadio->GetValue();
    
    // Enable/disable appropriate controls
    scaleSlider->Enable(scaleMode);
    scaleValue->Enable(scaleMode);
    
    widthInput->Enable(customMode);
    heightInput->Enable(customMode);
    keepAspectCheck->Enable(customMode);
    
    presetSizeChoice->Enable(presetMode);
}

// Event version for radio button events
void CompressPanel::OnResizeMethodChanged(wxCommandEvent &event)
{
    OnResizeMethodChanged();  // Call the parameterless version
}

void CompressPanel::OnPresetSizeChanged(wxCommandEvent &event)
{
    if (!presetSizeRadio->GetValue() || originalImage.empty())
        return;
    
    int selection = presetSizeChoice->GetSelection();
    int newWidth = 0, newHeight = 0;
    
    switch(selection) {
        case 0: // Original
            newWidth = originalImage.cols;
            newHeight = originalImage.rows;
            break;
        case 1: // 1920x1080
            newWidth = 1920;
            newHeight = 1080;
            break;
        case 2: // 1280x720
            newWidth = 1280;
            newHeight = 720;
            break;
        case 3: // 854x480
            newWidth = 854;
            newHeight = 480;
            break;
        case 4: // 640x360
            newWidth = 640;
            newHeight = 360;
            break;
        case 5: // 320x240
            newWidth = 320;
            newHeight = 240;
            break;
        case 6: // Instagram Square
            newWidth = 1080;
            newHeight = 1080;
            break;
        case 7: // Facebook Cover
            newWidth = 820;
            newHeight = 312;
            break;
        case 8: // Twitter Post
            newWidth = 1600;
            newHeight = 900;
            break;
    }
    
    widthInput->SetValue(wxString::Format("%d", newWidth));
    heightInput->SetValue(wxString::Format("%d", newHeight));
    
    // Update scale slider to match
    double scaleX = (double)newWidth / originalImage.cols;
    double scaleY = (double)newHeight / originalImage.rows;
    double scalePercent = (std::min(scaleX, scaleY)) * 100;
    scaleSlider->SetValue((int)scalePercent);
    scaleValue->SetLabel(wxString::Format("%d%%", (int)scalePercent));
}

cv::Mat CompressPanel::ResizeImage(const cv::Mat& input)
{
    if (input.empty()) return input;
    
    int newWidth = input.cols;
    int newHeight = input.rows;
    
    if (scalePercentRadio->GetValue()) {
        double scale = scaleSlider->GetValue() / 100.0;
        newWidth = input.cols * scale;
        newHeight = input.rows * scale;
    }
    else if (customSizeRadio->GetValue()) {
        long width, height;
        if (widthInput->GetValue().ToLong(&width)) newWidth = width;
        if (heightInput->GetValue().ToLong(&height)) newHeight = height;
    }
    else if (presetSizeRadio->GetValue()) {
        long width, height;
        if (widthInput->GetValue().ToLong(&width)) newWidth = width;
        if (heightInput->GetValue().ToLong(&height)) newHeight = height;
    }
    
    if (newWidth <= 0) newWidth = input.cols;
    if (newHeight <= 0) newHeight = input.rows;
    
    cv::Mat resized;
    cv::resize(input, resized, cv::Size(newWidth, newHeight));
    return resized;
}

void CompressPanel::OnTargetSizeChanged(wxCommandEvent &event)
{
    CalculateAndDisplayTargetSize();
}

void CompressPanel::CalculateAndDisplayTargetSize()
{
    int selection = targetSizeChoice->GetSelection();
    if (selection == 0) {
        currentSizeInfo->SetLabel("");
        qualitySlider->Enable(true);
        qualityValue->Enable(true);
        return;
    }
    
    int targetKB[] = {0, 50, 100, 200, 500, 1024, 2048, 5120};
    int target = targetKB[selection];
    
    currentSizeInfo->SetLabel(wxString::Format("🎯 Target: %d KB", target));
    
    // Auto-adjust quality based on target size (simplified estimation)
    if (!originalImage.empty()) {
        // Estimate required quality
        double originalSizeKB = 0;
        std::ifstream file(currentImagePath, std::ifstream::ate | std::ifstream::binary);
        if (file.is_open()) {
            originalSizeKB = file.tellg() / 1024.0;
            file.close();
        }
        
        int estimatedQuality = (int)((double)target / originalSizeKB * 100);
        estimatedQuality = std::max(1, std::min(100, estimatedQuality));
        
        qualitySlider->SetValue(estimatedQuality);
        qualityValue->SetLabel(wxString::Format("%d%%", estimatedQuality));
        
        txtLog->AppendText(wxString::Format("Auto-adjusted quality to %d%% for target size %d KB\n", 
                          estimatedQuality, target));
    }
}

void CompressPanel::OnLoadImage(wxCommandEvent &event)
{
    wxFileDialog openDialog(this, "Select Image to Compress", "", "",
                            "Image files (*.jpg;*.png;*.bmp;*.webp)|*.jpg;*.png;*.bmp;*.webp",
                            wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openDialog.ShowModal() == wxID_CANCEL)
        return;

    std::string path = std::string(openDialog.GetPath().mb_str());
    originalImage = cv::imread(path);

    if (originalImage.empty())
    {
        txtLog->AppendText("Error: Could not load image.\n");
        return;
    }

    currentImagePath = path;

    // Display dimensions
    int width = originalImage.cols;
    int height = originalImage.rows;
    originalDimensions->SetLabel(wxString::Format("%d x %d px", width, height));
    
    widthInput->SetValue(wxString::Format("%d", width));
    heightInput->SetValue(wxString::Format("%d", height));

    // Get file size
    std::ifstream file(path, std::ifstream::ate | std::ifstream::binary);
    long long fileSizeBytes = file.tellg();
    file.close();

    double fileSizeKB = fileSizeBytes / 1024.0;
    double fileSizeMB = fileSizeBytes / (1024.0 * 1024.0);
    
    if (fileSizeMB >= 1.0)
        originalFileSize->SetLabel(wxString::Format("%.2f KB (%.2f MB)", fileSizeKB, fileSizeMB));
    else
        originalFileSize->SetLabel(wxString::Format("%.2f KB", fileSizeKB));

    // Clear compressed info
    compressedDimensions->SetLabel("Not compressed");
    compressedFileSize->SetLabel("Not compressed");
    compressionRatio->SetLabel("Not compressed");

    UpdatePreview(originalPreview, originalImage);
    
    double sizeMB = fileSizeBytes / (1024.0 * 1024.0);
    txtLog->AppendText(wxString::Format("✅ Image loaded: %dx%d pixels, %.2f MB\n",
                                        originalImage.cols, originalImage.rows, sizeMB));
    
    CalculateAndDisplayTargetSize();
}

void CompressPanel::OnCompressImage(wxCommandEvent &event)
{
    if (originalImage.empty())
    {
        txtLog->AppendText("Error: Please load an image first.\n");
        return;
    }

    txtLog->AppendText("\n=== Starting Compression ===\n");

    // Resize image
    cv::Mat resizedImage = ResizeImage(originalImage);
    
    if (resizedImage.empty())
    {
        txtLog->AppendText("Error: Resize failed!\n");
        return;
    }
    
    txtLog->AppendText(wxString::Format("Resized to %dx%d pixels\n", resizedImage.cols, resizedImage.rows));

    // Compress based on format
    int format = formatChoice->GetSelection();
    std::vector<int> params;
    
    int targetSelection = targetSizeChoice->GetSelection();
    int targetKB = 0;
    if (targetSelection > 0) {
        int targetValues[] = {0, 50, 100, 200, 500, 1024, 2048, 5120};
        targetKB = targetValues[targetSelection];
    }

    if (format == 0) // JPEG
    {
        int quality = qualitySlider->GetValue();
        
        // If target size is set, try to achieve it by adjusting quality
        if (targetKB > 0) {
            int bestQuality = quality;
            int minQuality = 1, maxQuality = 100;
            
            // Binary search for quality that achieves target size
            for (int i = 0; i < 7; i++) { // 7 iterations for binary search
                int midQuality = (minQuality + maxQuality) / 2;
                std::vector<int> testParams;
                testParams.push_back(cv::IMWRITE_JPEG_QUALITY);
                testParams.push_back(midQuality);
                
                std::vector<uchar> testBuffer;
                cv::imencode(".jpg", resizedImage, testBuffer, testParams);
                double testSizeKB = testBuffer.size() / 1024.0;
                
                if (testSizeKB > targetKB)
                    maxQuality = midQuality;
                else
                    minQuality = midQuality;
                
                bestQuality = midQuality;
            }
            quality = bestQuality;
            qualitySlider->SetValue(quality);
            qualityValue->SetLabel(wxString::Format("%d%%", quality));
            txtLog->AppendText(wxString::Format("Target size %d KB: Auto-adjusted quality to %d%%\n", targetKB, quality));
        }
        
        params.push_back(cv::IMWRITE_JPEG_QUALITY);
        params.push_back(quality);
        cv::imencode(".jpg", resizedImage, compressedBuffer, params);
        compressedImage = cv::imdecode(compressedBuffer, cv::IMREAD_COLOR);
        txtLog->AppendText(wxString::Format("JPEG Compression with quality %d%%\n", quality));
    }
    else if (format == 1) // PNG
    {
        int compression = 9;
        params.push_back(cv::IMWRITE_PNG_COMPRESSION);
        params.push_back(compression);
        cv::imencode(".png", resizedImage, compressedBuffer, params);
        compressedImage = cv::imdecode(compressedBuffer, cv::IMREAD_COLOR);
        txtLog->AppendText("PNG Lossless Compression (level 9)\n");
    }
    else if (format == 2) // WebP
    {
        int quality = qualitySlider->GetValue();
        params.push_back(cv::IMWRITE_WEBP_QUALITY);
        params.push_back(quality);
        cv::imencode(".webp", resizedImage, compressedBuffer, params);
        compressedImage = cv::imdecode(compressedBuffer, cv::IMREAD_COLOR);
        txtLog->AppendText(wxString::Format("WebP Compression with quality %d%%\n", quality));
    }

    if (compressedImage.empty())
    {
        txtLog->AppendText("Error: Compression failed!\n");
        return;
    }

    // Display compressed dimensions
    compressedDimensions->SetLabel(wxString::Format("%d x %d px", compressedImage.cols, compressedImage.rows));

    // Display compressed file size
    double compressedSizeKB = compressedBuffer.size() / 1024.0;
    double compressedSizeMB = compressedBuffer.size() / (1024.0 * 1024.0);
    
    if (compressedSizeMB >= 1.0)
        compressedFileSize->SetLabel(wxString::Format("%.2f KB (%.2f MB)", compressedSizeKB, compressedSizeMB));
    else
        compressedFileSize->SetLabel(wxString::Format("%.2f KB", compressedSizeKB));

    // Calculate compression ratio
    long long originalFileSize = 0;
    std::ifstream file(currentImagePath, std::ifstream::ate | std::ifstream::binary);
    if (file.is_open()) {
        originalFileSize = file.tellg();
        file.close();
    }
    
    double savedPercentage = (1 - (double)compressedBuffer.size() / originalFileSize) * 100;
    double savedKB = (originalFileSize - compressedBuffer.size()) / 1024.0;
    
    compressionRatio->SetLabel(wxString::Format("%.1f%% saved (%.2f KB)", savedPercentage, savedKB));

    // Log statistics
    double originalSizeKB = originalFileSize / 1024.0;
    txtLog->AppendText(wxString::Format("📊 Original size: %.2f KB\n", originalSizeKB));
    txtLog->AppendText(wxString::Format("📊 Compressed size: %.2f KB\n", compressedSizeKB));
    txtLog->AppendText(wxString::Format("📊 Space saved: %.1f%% (%.2f KB)\n", savedPercentage, savedKB));
    
    // Check if target was achieved
    if (targetKB > 0) {
        if (compressedSizeKB <= targetKB)
            txtLog->AppendText(wxString::Format("✅ Target size achieved! (%d KB target)\n", targetKB));
        else
            txtLog->AppendText(wxString::Format("⚠️ Target size not achieved. (%d KB target, got %.2f KB)\n", targetKB, compressedSizeKB));
    }

    UpdatePreview(compressedPreview, compressedImage);
    txtLog->AppendText("=== Compression Complete ===\n\n");
}

void CompressPanel::OnSaveImage(wxCommandEvent &event)
{
    if (compressedImage.empty())
    {
        txtLog->AppendText("No compressed image to save. Please compress an image first.\n");
        return;
    }

    wxFileDialog saveDialog(this, "Save Compressed Image", "", "compressed.jpg",
                            "JPEG files (*.jpg)|*.jpg|PNG files (*.png)|*.png|WebP files (*.webp)|*.webp",
                            wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveDialog.ShowModal() == wxID_CANCEL)
        return;

    std::string path = std::string(saveDialog.GetPath().mb_str());

    std::string ext = path.substr(path.find_last_of(".") + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    std::vector<int> params;
    if (ext == "jpg" || ext == "jpeg")
    {
        params.push_back(cv::IMWRITE_JPEG_QUALITY);
        params.push_back(qualitySlider->GetValue());
    }
    else if (ext == "png")
    {
        params.push_back(cv::IMWRITE_PNG_COMPRESSION);
        params.push_back(9);
    }
    else if (ext == "webp")
    {
        params.push_back(cv::IMWRITE_WEBP_QUALITY);
        params.push_back(qualitySlider->GetValue());
    }

    bool success = cv::imwrite(path, compressedImage, params);
    if (success)
    {
        txtLog->AppendText(wxString::Format("💾 Compressed image saved to: %s\n", path));
        
        std::ifstream file(path, std::ifstream::ate | std::ifstream::binary);
        long long savedSize = file.tellg();
        file.close();
        txtLog->AppendText(wxString::Format("📊 Saved file size: %.2f KB\n", savedSize / 1024.0));
    }
    else
    {
        txtLog->AppendText("Error: Failed to save image!\n");
    }
}

void CompressPanel::UpdatePreview(wxStaticBitmap *preview, cv::Mat mat)
{
    if (mat.empty())
        return;

    cv::Mat display;
    cv::Mat resized;

    int maxWidth = 350;
    int maxHeight = 320;

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