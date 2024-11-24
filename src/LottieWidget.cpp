#include <QDebug>
#include <QTimer>
#include <LottieWidget.h>
#include <thorvg.h>
namespace Pari {

LottieWidget::LottieWidget(std::string path, QWidget *parent)
    : QWidget(parent)
    , _path{path}
    , _valid{false}
    , _play{false}
{
    _animation = tvg::Animation::gen();
    _canvas = tvg::SwCanvas::gen();
    _picture = _animation->picture();
    if (_picture->load(path.c_str()) == tvg::Result::Success) {
        _valid = true;
        _totalFrame = _animation->totalFrame();
    } else {
        _valid = false;
        qWarning() << "failed to load lottie animation from file path : "
                   << QString::fromStdString(path);
    }
}

bool LottieWidget::isValid()
{
    return _valid;
}

bool LottieWidget::isPlay()
{
    return _play;
}

void LottieWidget::setPlay(bool play)
{
    _play = play;
    if (_valid) {
        double dur = 1000.0 / double(_totalFrame); //time in between frames in msecs
        if (_frameTimer) {
            _frameTimer->deleteLater();
        }
        _frameTimer = new QTimer();
        _frameTimer->setInterval(dur);
        connect(_frameTimer, &QTimer::timeout, this, [this]() { update(); });
        _frameTimer->start();
    }
}

void LottieWidget::paintEvent(QPaintEvent *event) {}

} // namespace Pari
