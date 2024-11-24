#ifndef QT_LOTTIE_WIDGET_H
#define QT_LOTTIE_WIDGET_H
#include <QWidget>

namespace tvg {
QT_FORWARD_DECLARE_CLASS(Animation)
QT_FORWARD_DECLARE_CLASS(Picture)
QT_FORWARD_DECLARE_CLASS(Canvas)
QT_FORWARD_DECLARE_CLASS(QTimer)
} // namespace tvg

namespace Pari {

class LottieWidget : public QWidget
{
public:
    LottieWidget(std::string path, QWidget *parent = nullptr);
    ~LottieWidget() = default;
    bool isValid();
    bool isPlay();
    void setPlay(bool play);

private:
    std::string _path;
    tvg::Animation *_animation;
    tvg::Canvas *_canvas;
    tvg::Picture *_picture;
    bool _valid;
    quint32 _currentFrame;
    quint32 _totalFrame;
    bool _play;
    QTimer *_frameTimer;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};

} // namespace Pari
#endif //QT_LOTTIE_WIDGET_H
