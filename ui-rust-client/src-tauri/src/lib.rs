pub mod target_mod {
    tonic::include_proto!("target");
}

use target_mod::target_client::TargetClient;
use target_mod::Empty;

use tauri::State;
use tonic::transport::Channel;

struct GrpcClient(Option<TargetClient<Channel>>);

#[tauri::command]
async fn get_position(client: State<'_, GrpcClient>) -> Result<String, String> {
    println!("We are called!");

    let grpc_client = client.0.clone();

    match grpc_client {
        Some(client) => {
            let response = client
                .clone()
                .get_position(Empty {})
                .await
                .map_err(|e| e.to_string())?;

            let r = response.into_inner();

            println!("x = {}, y = {}", r.x, r.y);

            Ok(format!("x = {}, y = {}", r.x, r.y))
        }

        None => Ok(format!("{} {}", 42, 27)),
    }
}

#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {
    let grpc_client = match tauri::async_runtime::block_on(async {
        TargetClient::connect("http://192.168.1.160:50051").await
    }) {
        Ok(c) => Some(c),
        Err(e) => {
            println!("ERROR: Oops. Connection not possible: {}", e.to_string());
            None
        }
    };

    tauri::Builder::default()
        .manage(GrpcClient(grpc_client))
        .plugin(tauri_plugin_opener::init())
        .invoke_handler(tauri::generate_handler![get_position])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
