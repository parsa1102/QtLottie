use zanbil_build::pkg_config;

fn main() {
    zanbil_build::builder()
        .add_pkg_config_lib(pkg_config::probe_library("Qt6Core").unwrap())
        .add_pkg_config_lib(pkg_config::probe_library("Qt6Widgets").unwrap())
        .add_pkg_config_lib(pkg_config::probe_library("Qt6Gui").unwrap())
        .build();
}
