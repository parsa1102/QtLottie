
# QtLottie

A custom Qt widget for rendering and controlling Lottie animations using the ThorVG library. This project provides a convenient way to integrate Lottie animations into Qt applications, leveraging the ThorVG engine for efficient vector graphics rendering.

## Features

- **Lottie Animation Support**: Load and display Lottie animations from JSON files.
- **Frame Control**: Set and get the current frame of the animation.
- **Playback Control**: Start and stop playback with customizable frame intervals.
- **Auto-Resizing**: Automatically adjusts widget size based on animation dimensions.

## Requirements

- **Qt**
- **ThorVG**

## Installation

1. Clone the repository:

    ```bash
    git clone https://github.com/parsa1102/QtLottie.git
    cd QtLottie
    ```

2. Build the project using CMake:

    ```bash
    mkdir build && cd build
    cmake ..
    make
    ```

3. Install the library:

    ```bash
    sudo make install
    ```

## Usage

### Example

```cpp
#include "LottieWidget.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    Pari::LottieWidget widget("/path/to/animation.json");
    widget.setPlay(true);
    widget.show();
    
    return app.exec();
}
```


## Development

To contribute:

1. Fork the repository.
2. Create a new feature branch.
3. Submit a pull request with your changes.

## License

This project is licensed under the MIT License.

## Acknowledgements

- **ThorVG**
- **Qt**
