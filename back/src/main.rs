use std::fs::File;
use std::io::{Read, Write};
use std::net::{TcpListener, TcpStream};

fn handle_client(mut stream: TcpStream) {
    let mut buffer = [0; 1024];
    let mut file = File::create("uploaded_file.txt").unwrap();
    let mut header_read = false;

    loop {
        match stream.read(&mut buffer) {
            Ok(0) => break,
            Ok(bytes_read) => {
                if !header_read {
                    if let Some(index) = buffer.windows(4).position(|w| w == b"\r\n\r\n") {
                        file.write_all(&buffer[(index + 4)..bytes_read]).unwrap();
                        header_read = true;
                    }
                } else {
                    file.write_all(&buffer[..bytes_read]).unwrap();
                }
            }
            Err(_) => break, // Error
        }
    }

    println!("File uploaded successfully");
}

fn main() {
    let listener = TcpListener::bind("0.0.0.0:8080").unwrap();

    for stream in listener.incoming() {
        match stream {
            Ok(stream) => {
                println!("New client connected");
                handle_client(stream);
            }
            Err(e) => {
                eprintln!("Error: {}", e);
            }
        }
    }
}
