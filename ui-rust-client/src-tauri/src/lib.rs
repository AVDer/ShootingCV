pub mod target_mod {
    tonic::include_proto!("target");
}

use target_mod::target_client::TargetClient;
use target_mod::Empty;
use target_mod::GetImageRequest;
use target_mod::ImageType;

use tauri::State;
use tonic::transport::Channel;

use std::sync::Arc;

use log::{debug, error, info, trace};

use tokio::sync::Mutex;

use base64::engine::general_purpose::STANDARD;
use base64::Engine;

type GrpcClient = Arc<Mutex<Option<TargetClient<Channel>>>>;

#[derive(serde::Serialize)]
struct ImageResponse {
    base64: String,
    mime_type: String,
}

#[tauri::command]
async fn get_position(client: State<'_, GrpcClient>) -> Result<String, String> {
    trace!("get_position function call");

    let grpc_client = client.lock().await;

    match grpc_client.as_ref() {
        Some(c) => {
            let response = c
                .clone()
                .get_position(Empty {})
                .await
                .map_err(|e| e.to_string())?;

            let r = response.into_inner();

            debug!("x = {}, y = {}", r.x, r.y);

            Ok(format!("{} {}", r.x, r.y))
        }

        None => Ok(format!("{} {}", 42, 27)),
    }
}

#[tauri::command]
async fn calibrate_target(client: State<'_, GrpcClient>) -> Result<(), String> {
    trace!("calibrate_target function call");

    let grpc_client = client.lock().await;

    match grpc_client.as_ref() {
        Some(c) => {
            let _ = c
                .clone()
                .adjust_image(Empty {})
                .await
                .map_err(|e| e.to_string())?;
            Ok(())
        }

        None => Ok(()),
    }
}

fn map_image_type(image_type: &str) -> Result<ImageType, String> {
    match image_type {
        "original" => Ok(ImageType::Original),
        "transformed" => Ok(ImageType::Transform),
        "marked" => Ok(ImageType::Mark),
        _ => Err(format!("Unknown image type: {}", image_type)),
    }
}

#[tauri::command]
async fn read_calibration_image(
    client: State<'_, GrpcClient>,
    image_type: String,
) -> Result<ImageResponse, String> {
    // image_type will be: "original", "transformed", or "marked"
    debug!("Requested image type: {}", image_type);

    let image_type = map_image_type(&image_type)?;

    let image_request = tonic::Request::new(GetImageRequest {
        image_type: image_type as i32,
    });

    let grpc_client = client.lock().await;

    let image_response = grpc_client
        .as_ref()
        .unwrap()
        .clone()
        .get_image(image_request)
        .await
        .map_err(|e| e.to_string())?
        .into_inner();

    // Encode bytes to Base64
    let base64_data = STANDARD.encode(&image_response.image_data);

    Ok(ImageResponse {
        base64: base64_data,
        mime_type: image_response.mime_type,
    })
}

#[tauri::command]
async fn target_connect(client: State<'_, GrpcClient>, host: String) -> Result<(), String> {
    trace!("connect function call");

    let mut grpc_client = client.lock().await;

    let server_url = format!("http://{}:50051", host);
    info!("Connecting to: {server_url}");

    match TargetClient::connect(server_url).await {
        Ok(c) => {
            info!("Successfully connected to the server");
            *grpc_client = Some(c);
            Ok(())
        }
        Err(e) => {
            error!("Connection not possible: {}", e.to_string());
            *grpc_client = None;
            Err(String::from("Connection not possible"))
        }
    }
}

#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {
    let grpc_client: GrpcClient = Arc::new(Mutex::new(None));

    tauri::Builder::default()
        .manage(grpc_client)
        .plugin(tauri_plugin_opener::init())
        .invoke_handler(tauri::generate_handler![
            target_connect,
            get_position,
            read_calibration_image,
            calibrate_target,
        ])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
