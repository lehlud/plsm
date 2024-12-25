# plsm

Toy general purpose systems programming lanugage

## Hello World Example (+ unicode, yeah)

This example just looks like C without its syntax when in reality, plsm aims to be different. It shall have support for classes, lambdas, interfaces/traits and easy modularization.

```plsm
fun write(fd : i64, msg : &u8, len : u64) i64 {
  inline asm (
    "mov $0, %rax"         // syscall: write
    "mov $1, %rdi"         // file descriptor: stdout
    "mov $2, %rsi"         // message to write
    "mov $3, %rdx"         // length of message
    "syscall"              // make the syscall
    :
    : "r"(1 as i64), "r"(1 as i64), "r"(msg), "r"(len)  // input: different params
    : "rax", "rdi", "rsi", "rdx"                        // clobbered registers
  );

  ret 0;
}

fun exit(code : u8) i64 {
  inline asm (
    "mov $0, %rax"
    "mov $1, %rdi"
    "syscall"
    :
    : "r"(60 as i64), "r"(code as i64)
    : "rax", "rdi"
  );

  ret 0;
}

fun main(argc : i32) u8 {
  write(1, "Hello World!\n", 14);
  write(1, "Ə Ɛ Ƒ ƒ Ɠ Ɣ ƕ Ɩ Ɨ Ƙ ƙ ƚ ƛ Ɯ Ɲ ƞ Ɵ Ơ ơ Ƣ ƣ Ƥ ƥ\n", 69);
  exit(10);

  ret 0;
}
```