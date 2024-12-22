#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPainter>
#include <QResizeEvent>
#include <QTimer>
#include <QtMath>
#include <LottieWidget.h>
#include <thorvg.h>
#include <QtMath>

static bool logLoadResult(tvg::Result const &result)
{
    bool ret = false;
    switch (result) {
    case tvg::Result::Success:
        qInfo() << "Lottie File parsed and loaded successfully";
        ret = true;
        break;
    case tvg::Result::InvalidArguments:
        qWarning() << "Invalid Arguments error while loading lottie file e.g. empty paths or null "
                      "pointers";
        break;
    case tvg::Result::InsufficientCondition:
        qWarning() << "InsufficientCondition error while loading lottie file e.g. asking for "
                      "properties of an object, which does not exist";
        break;
    case tvg::Result::FailedAllocation:
        qWarning() << "Failed Allocation error while loading lottie file";
        break;
    case tvg::Result::MemoryCorruption:
        qWarning() << "memory corruption error while loading lottie file";
        break;
    case tvg::Result::NonSupport:
        qWarning() << "Non Support error while loading lottie file, make sure your thorvg build "
                      "supports Lottie files";
        break;
    case tvg::Result::Unknown:
        qWarning() << "Unknown error while loading lottie file";
        break;
    default:
        qWarning() << "Unknown error while loading lottie file";
    }
    return ret;
}

namespace Pari {

LottieWidget::LottieWidget(std::string path, QWidget *parent)
    : QWidget(parent)
    , _path{path}
    , _valid{false}
    , _play{false}
    , _currentFrame{0}
    , _totalFrame{0}
    , _duration{0.0}
    , _frameTimer{nullptr}
    , _buffer{nullptr}
    , _animation{nullptr}
    , _canvas{nullptr}
    , _picture{nullptr}
{
    setSource(path);
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
        double dur = 1000.0 * _duration / double(_totalFrame); //time in between frames in msecs
        if (_frameTimer) {
            _frameTimer->deleteLater();
        }
        _frameTimer = new QTimer();
        _frameTimer->setInterval(dur);
        connect(_frameTimer, &QTimer::timeout, this, [this]() {
            _currentFrame++;
            if (_currentFrame > _totalFrame) {
                _currentFrame = 0;
            }
            update();
        });
        _frameTimer->start();
    }
    return;
}

bool LottieWidget::isPaused()
{
    return !_play;
}

void LottieWidget::setPaused(bool paused)
{
    _play = !paused;
}

Result LottieWidget::setCurrentFrame(quint32 currentFrame)
{
    Result res = {false, ""};
    if (currentFrame <= _totalFrame) {
        _currentFrame = currentFrame;
        res.succeed = true;
    } else {
        res.succeed = false;
        res.errMsg = "can not set currentFrame since input is bigger than total frame count";
    }
    return res;
}

quint32 LottieWidget::getCurrentFrame()
{
    return _currentFrame;
}

double LottieWidget::getDuration()
{
    return _duration;
}

quint32 LottieWidget::getTotalFrame()
{
    return _totalFrame;
}

double LottieWidget::getFrameRate()
{
    return double(_totalFrame) / _duration;
}

bool LottieWidget::setSource(std::string path)
{
    cleanUpMemory();
    _animation = tvg::Animation::gen();
    _picture = _animation->picture();
    auto const &res = _picture->load(path.c_str());
    if (logLoadResult(res)) {
        _valid = true;
        _width = width();
        _height = height();
        QFile file(QString::fromStdString(path));
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray fileData = file.readAll();
            file.close();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData);
            QJsonObject object = jsonDoc.object();
            if (object.contains("w") && object.contains("h")) {
                _width = object["w"].toDouble();
                _height = object["h"].toDouble();
                setFixedSize(_width, _height);
            } else {
                qWarning() << "json file did not contain valid w or h keys so autosizing failed";
            }
        } else {
            qWarning() << "autosizing failed due to QFile failing to open Lottie json File";
        }
        allocateBuffer();
        _canvas = tvg::SwCanvas::gen();
        _canvas->target(_buffer, width(), width(), height(), tvg::ColorSpace::ARGB8888);
        _totalFrame = _animation->totalFrame();
        _currentFrame = 0;
        _duration = _animation->duration(); // in seconds
        _canvas->push(_picture);
    } else {
        _valid = false;
    }
    return _valid;
}

void LottieWidget::allocateBuffer()
{
    auto const &size = qFloor(_width * _height);
    _buffer = (uint32_t *) realloc(_buffer, size * sizeof(uint32_t));
    memset(_buffer, 0, size * sizeof(uint32_t));
    return;
}

void LottieWidget::cleanUpMemory()
{
    if (nullptr != _animation) {
        delete _animation;
        //must leave deleting the picture to this
    }
    if (nullptr != _canvas) {
        delete _canvas;
    }
}

void LottieWidget::paintEvent(QPaintEvent *event)
{
    if (_valid) {
        qDebug() << "paint event called on frame : " << _currentFrame;
        _animation->frame(_currentFrame);
        _picture = _animation->picture();
        _canvas->clear();
        _canvas->push(_picture);
        _canvas->draw();
        _canvas->sync();
        QPainter painter(this);
        QImage img(reinterpret_cast<const uchar *>(_buffer),
                   width(),
                   height(),
                   QImage::Format_ARGB32);
        painter.drawImage(0, 0, img);
        return;
    }
}

void LottieWidget::resizeEvent(QResizeEvent *event)
{
    if (_valid) {
        _width = event->size().width();
        _height = event->size().height();
        allocateBuffer();
        _buffer = new uint32_t[qCeil(_width) * qCeil(_height)]{0};
        _picture = _animation->picture();
        _picture->size(_width, _height);
        delete _canvas;
        _canvas = tvg::SwCanvas::gen();
        _canvas->clear();
        _canvas->target(_buffer, _width, _width, _height, tvg::ColorSpace::ARGB8888);
        _canvas->push(_picture);
    }
}

} // namespace Pari
