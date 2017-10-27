#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <memory>
#include "perlin.h"

namespace Ui {
class MainWindow;
}

enum GenOp {
  NOISE,
  SMOOTHNOISE,
  TURBULENCE,
  CLOUD,
  MARBLE
};

class MainWindow : public QMainWindow
{
  Q_OBJECT

  protected:
    void resizeEvent(QResizeEvent *event) override;

  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

  private slots:
    void on_buttonGenerate_clicked();

    void on_radioNoise_toggled(bool checked);

    void on_radioSmooth_toggled(bool checked);

    void on_radioTurbulence_toggled(bool checked);

    void on_radioCloud_toggled(bool checked);

    void on_radioMarble_toggled(bool checked);

  private:
    GenOp op;
    Ui::MainWindow *ui;
    std::shared_ptr<Noise> noise;
    QGraphicsScene *scene;
};

#endif // MAINWINDOW_H
