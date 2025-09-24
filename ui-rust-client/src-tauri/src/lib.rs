pub mod target_mod {
    tonic::include_proto!("target");
}

use target_mod::target_client::TargetClient;
use target_mod::Empty;

use tauri::State;
use tonic::transport::Channel;

use std::sync::Arc;

use log::{error, info, trace};

use tokio::sync::Mutex;

type GrpcClient = Arc<Mutex<Option<TargetClient<Channel>>>>;

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

            trace!("x = {}, y = {}", r.x, r.y);

            Ok(format!("{} {}", r.x, r.y))
        }

        None => Ok(format!("{} {}", 42, 27)),
    }
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
        .invoke_handler(tauri::generate_handler![target_connect, get_position])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
