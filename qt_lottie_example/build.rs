fn main() {
    zanbil_build::builder()
        .add_system_include_dir("/usr/include/qt6/QtCore")
        .add_system_include_dir("/usr/include/qt6/QtGui")
        .add_system_include_dir("/usr/include/qt6/QtWidgets")
        .add_system_include_dir("/usr/include/qt6")
        .add_system_include_dir("/usr/local/include")
        .build();
}
