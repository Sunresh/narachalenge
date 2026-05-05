#ifndef COMPRESSPANEL_H
#define COMPRESSPANEL_H

#include <wx/wx.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

class CompressPanel : public wxPanel
{
private:
    // Image preview controls
    wxStaticBitmap *originalPreview;
    wxStaticBitmap *compressedPreview;
    wxTextCtrl *txtLog;

    // Compression controls
    wxSlider *qualitySlider;
    wxStaticText *qualityValue;
    wxChoice *formatChoice;

    // Resize controls - Radio buttons
    wxRadioButton *scalePercentRadio;
    wxRadioButton *customSizeRadio;
    wxRadioButton *presetSizeRadio;

    // Scale by percentage controls
    wxSlider *scaleSlider;
    wxStaticText *scaleValue;

    // Custom size controls
    wxTextCtrl *widthInput;
    wxTextCtrl *heightInput;
    wxCheckBox *keepAspectCheck;

    // Preset dimensions controls
    wxChoice *presetSizeChoice;

    // Info display members
    wxStaticText *originalDimensions;
    wxStaticText *originalFileSize;
    wxStaticText *compressedDimensions;
    wxStaticText *compressedFileSize;
    wxStaticText *compressionRatio;
    wxStaticText *targetSizeInfo;

    // Target file size controls
    wxChoice *targetSizeChoice;
    wxStaticText *currentSizeInfo;

    // Image data
    cv::Mat originalImage;
    cv::Mat compressedImage;
    std::vector<uchar> compressedBuffer;
    std::string currentImagePath;

    // Helper methods
    cv::Mat ResizeImage(const cv::Mat &input);
    void UpdateResizeControls();
    void CalculateAndDisplayTargetSize();

public:
    CompressPanel(wxWindow *parent);

    // Event handlers
    void OnLoadImage(wxCommandEvent &event);
    void OnCompressImage(wxCommandEvent &event);
    void OnSaveImage(wxCommandEvent &event);
    // Add this in the public section
    void OnResizeMethodChanged();                      // Parameterless version
    void OnResizeMethodChanged(wxCommandEvent &event); // Event version
    void OnPresetSizeChanged(wxCommandEvent &event);
    void OnTargetSizeChanged(wxCommandEvent &event);
    void UpdatePreview(wxStaticBitmap *preview, cv::Mat mat);
};

#endif