fn main() {
    zanbil_build::builder()
        .add_system_include_dir("/usr/include/qt6/QtCore")
        .add_system_include_dir("/usr/include/qt6/QtGui")
        .add_system_include_dir("/usr/include/qt6/QtWidgets")
        .add_system_include_dir("/usr/include/qt6")
        .add_system_include_dir("/usr/local/include")
        .build();
    build_rs::output::rustc_link_lib("Qt6Widgets");
    build_rs::output::rustc_link_lib("Qt6Gui");
    build_rs::output::rustc_link_lib("Qt6Core");
    build_rs::output::rustc_link_search("/usr/local/lib");
    build_rs::output::rustc_link_lib("thorvg");
}
