#ifndef QT_LOTTIE_WIDGET_H
#define QT_LOTTIE_WIDGET_H
#include <QWidget>

namespace tvg {
QT_FORWARD_DECLARE_CLASS(Animation)
QT_FORWARD_DECLARE_CLASS(Picture)
QT_FORWARD_DECLARE_CLASS(SwCanvas)
QT_FORWARD_DECLARE_CLASS(QTimer)
} // namespace tvg

namespace Pari {

struct Result
{
    bool succeed = false;
    QString errMsg;
};

class LottieWidget : public QWidget
{
public:
    LottieWidget(std::string path, QWidget *parent = nullptr);
    ~LottieWidget() = default;
    bool isValid();
    bool isPlay();
    void setPlay(bool play);
    Result setCurrentFrame(quint32 currentFrame);
    quint32 getCurrentFrame();

private:
    std::string _path;
    tvg::Animation *_animation;
    tvg::SwCanvas *_canvas;
    tvg::Picture *_picture;
    bool _valid;
    quint32 _currentFrame;
    quint32 _totalFrame;
    double _duration;
    bool _play;
    QTimer *_frameTimer;
    double _width;
    double _height;
    uint32_t *_buffer;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};

} // namespace Pari
#endif //QT_LOTTIE_WIDGET_H
