pub mod target_mod {
    tonic::include_proto!("target");
}

use target_mod::target_client::TargetClient;
use target_mod::Empty;

use tonic::transport::Channel;
use tauri::State;

struct GrpcClient(TargetClient<Channel>);


// Learn more about Tauri commands at https://tauri.app/develop/calling-rust/
#[tauri::command]
fn greet(name: &str) -> String {
    format!("Hello, {}! You've been greeted from Rust!", name)
}

#[tauri::command]
async fn get_position(client: State<'_, GrpcClient>) -> Result<String, String> {

    println!("We are called!");

    let mut grpc_client = client.0.clone();
    let response = grpc_client
        .get_position(Empty{})
        .await
        .map_err(|e| e.to_string())?;

    let r = response.into_inner();

    Ok(format!("x = {}, y = {}", r.x, r.y))
}


#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {

let grpc_client = tauri::async_runtime::block_on(async {
        TargetClient::connect("http://192.168.1.160:50051")
        .await
        .unwrap()
    });


    tauri::Builder::default()
        .manage(GrpcClient(grpc_client))
        .plugin(tauri_plugin_opener::init())
        .invoke_handler(tauri::generate_handler![greet])
        .invoke_handler(tauri::generate_handler![get_position])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}

/*
pub fn main() {
    // Preload async resources before Tauri starts


    tauri::Builder::default()
        .manage(GrpcClient(grpc_client))
        .invoke_handler(tauri::generate_handler![fetch_data])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}

*/
