#include <LottieWidget.h>
#include <QApplication>
#include <thorvg.h>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    tvg::Initializer::init(4);
    Pari::LottieWidget widget("Animation.json");
    widget.setPlay(true);
    widget.show();
    return app.exec();
}
