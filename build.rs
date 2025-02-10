use std::env::current_dir;

fn main() {
    let curr_dir = current_dir().unwrap().to_str().unwrap().to_owned();
    println!("cargo:rustc-link-search=native={}", curr_dir);
    println!("cargo:rustc-link-lib=static=asm");
}
