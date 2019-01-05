#ifndef DISPLAYHISTOGRAM_H
#define DISPLAYHISTOGRAM_H

#include <QWidget>
#include <opencv2/opencv.hpp>

namespace Ui {
  class displayHistogram;
}

class displayHistogram : public QWidget
{
  Q_OBJECT

public:
  explicit displayHistogram(QWidget *parent = nullptr);
  ~displayHistogram();
public slots:
    void updateHistogramFront(cv::Mat frame);
    void updateHistogramTop(cv::Mat frame);
    void updateHistogramSide(cv::Mat frame);

private slots:
    void on_pushButton_clicked();

private:
  Ui::displayHistogram *ui;
  cv::Mat histogramFront;
  cv::Mat histogramTop;
  cv::Mat histogramSide;
  cv::Mat displayFrame;
  QString cameraSelection;
};

#endif // DISPLAYHISTOGRAM_H
