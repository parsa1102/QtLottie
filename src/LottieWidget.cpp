#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPainter>
#include <QResizeEvent>
#include <QTimer>
#include <LottieWidget.h>
#include <thorvg.h>
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
{
    _animation = tvg::Animation::gen();
    _picture = _animation->picture();
    if (_picture->load(path.c_str()) == tvg::Result::Success) {
        _valid = true;
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
        _buffer = new uint32_t[width() * height()]{0};
        _canvas = tvg::SwCanvas::gen();
        _canvas->target(_buffer, width(), width(), height(), tvg::ColorSpace::ARGB8888);
        _totalFrame = _animation->totalFrame();
        _currentFrame = 0;
        _duration = _animation->duration(); // in seconds
        _canvas->push(_picture);
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

void LottieWidget::paintEvent(QPaintEvent *event)
{
    qDebug() << "paint event called on frame : " << _currentFrame;
    _animation->frame(_currentFrame);
    _picture = _animation->picture();
    _canvas->clear();
    _canvas->push(_picture);
    _canvas->draw();
    _canvas->sync();
    QPainter painter(this);
    QImage img(reinterpret_cast<const uchar *>(_buffer), width(), height(), QImage::Format_ARGB32);
    painter.drawImage(0, 0, img);
    return;
}

void LottieWidget::resizeEvent(QResizeEvent *event)
{
    if (_valid) {
        _width = event->size().width();
        _height = event->size().height();
        if (nullptr != _buffer) {
            delete[] _buffer;
        }
        // do this better implicit casting like this is not a good idea
        _buffer = new uint32_t[quint32(_width) * quint32(_height)]{0};
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
