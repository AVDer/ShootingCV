// Prevents additional console window on Windows in release, DO NOT REMOVE!!
#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]

use env_logger::Builder;

fn main() {
    log::set_max_level(log::LevelFilter::Trace);

    Builder::new()
        .filter_level(log::LevelFilter::Info)
        .filter(Some("ui_rust_client_lib"), log::LevelFilter::Trace)
        .init();

    ui_rust_client_lib::run()
}
