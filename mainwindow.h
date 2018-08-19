#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGroupBox>

#include "noise.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

  protected:
    void resizeEvent(QResizeEvent *event) override;

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

  private slots:
    void on_buttonGenerate_clicked();

    void on_buttonSave_clicked();

    void on_radioNoise_toggled(bool checked);

    void on_radioSmooth_toggled(bool checked);

    void on_radioTurbulence_toggled(bool checked);

    void on_radioCloud_toggled(bool checked);

    void on_radioMarble_toggled(bool checked);

  private:
    Ui::MainWindow* ui;
    QGraphicsScene* scene;
    QGroupBox* noiseOptions;

    // noise generator
    NoiseType op;
    std::shared_ptr<Noise> noise;

    // object with generated noise
    QPixmap p;

    void clearNoiseOptions();
};

#endif // MAINWINDOW_H
