use tonic_prost_build;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    tonic_prost_build::configure()
        .build_server(false)
        .compile_protos(&["../proto/target.proto"], &["../proto"])?;
    tauri_build::build();
    Ok(())
}
