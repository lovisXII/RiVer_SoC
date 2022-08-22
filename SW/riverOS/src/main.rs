#![no_std] // don't link the Rust standard library
#![no_main] // disable all Rust-level entry points
#![feature(once_cell)]
use core::arch::asm;
use core::ffi::c_void;
use core::panic::PanicInfo;
use core::ptr::null;


pub fn instruction_address_fault()  {unsafe {asm!("nop")}}
pub fn illegal_instruction()  {unsafe {asm!("nop")}}
pub fn instruction_address_misagligned()  {unsafe {asm!("nop")}}
pub fn env_call_u_mode()  {unsafe {asm!("nop")}}
pub fn env_call_s_mode() {unsafe {asm!("nop")}}
pub fn env_call_m_mode() {unsafe {asm!("nop")}}
pub fn load_adress_missaligned() {unsafe {asm!("nop")}}
pub fn store_adress_missaligned() {unsafe {asm!("nop")}}
pub fn load_access_fault() {unsafe {asm!("nop")}}
pub fn store_access_fault() {unsafe {asm!("nop")}}
pub fn env_call_wrong_mode() {unsafe {asm!("nop")}}
pub fn breakpoint() {unsafe {asm!("nop")}}

pub unsafe fn interrupt_handler() -> ! {
    let mut mcause : u32;
    asm!(
        "csrrwi  x10, mcause,   0x0",
        "csrrci  x11, mscratch, 0x01",
        "sw      x10, 0(x11)",
        out("x10") mcause
    );
    match mcause {
        0 => instruction_address_misagligned(),
        1 => instruction_address_fault(),
        2 => illegal_instruction(),
        3 => breakpoint(),  
        4 => load_adress_missaligned(),
        5 => load_access_fault(),
        6 => store_adress_missaligned(),
        7 => store_access_fault(),
        8 => env_call_u_mode(),
        9 => env_call_s_mode(),
        11 => env_call_m_mode(),
        _ => ()
    };
    asm!("mret");
    unreachable!()
}



pub unsafe fn init() {
    asm!(
        "la    x28, _start",
        "li    x30, 0x90000000",
        "srli  x29, x29,      2",
        "sll   x29, x29,      2",
        "addi  x29, x29,      0",
        "csrrw x0,  mepc,     x28",
        "csrrw x0,  mtvec,    x29",
        "csrrw x0,  mscratch, x30",
        "li    x2,  0x10000",
        in("x29") interrupt_handler,
    );
}

#[no_mangle] // don't mangle the name of this function
pub extern "C" fn _start() -> ! {
    // this function is the entry point, since the linker looks for a function
    // named `_start` by default
    unsafe {
        init();
    }
    loop {}
}




/// This function is called on panic.
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}