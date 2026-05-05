#ifndef COMPAREPANEL_H
#define COMPAREPANEL_H

#include <wx/wx.h>
#include <opencv2/opencv.hpp>

class ComparePanel : public wxPanel
{
private:
    wxStaticBitmap *sourcePreview;
    wxStaticBitmap *referencePreview;
    wxStaticBitmap *resultPreview;
    wxTextCtrl *txtLog;
    
    cv::Mat sourceImage;
    cv::Mat referenceImage;
    cv::Mat resultImage;
    
    wxChoice *methodChoice;
    wxSlider *thresholdSlider;
    wxStaticText *thresholdValue;

public:
    ComparePanel(wxWindow *parent);
    
    void OnLoadSource(wxCommandEvent &event);
    void OnLoadReference(wxCommandEvent &event);
    void OnCompareImages(wxCommandEvent &event);
    void OnClearAll(wxCommandEvent &event);
    void OnSaveResult(wxCommandEvent &event);
    
    void PerformTemplateMatching(double threshold);
    void PerformHistogramComparison();
    void PerformMSEComparison();
    double CalculateMSE(const cv::Mat& img1, const cv::Mat& img2);
    double CalculatePSNR(const cv::Mat& img1, const cv::Mat& img2);
    void UpdatePreview(wxStaticBitmap* preview, cv::Mat mat);
};

#endif