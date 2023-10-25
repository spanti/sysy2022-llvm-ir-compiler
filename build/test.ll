; ModuleID = 'test jit'
source_filename = "test jit"

declare i32 @getint()

declare i32 @getch()

declare i32 @getarray(i32*)

declare void @putint(i32)

declare void @putch(i32)

declare void @putarray(i32, i32*)

define i32 @func1() {
entry:
  ret i32 555
}

define i32 @func2() {
entry:
  ret i32 111
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %calltmp = call i32 @func1()
  store i32 %calltmp, i32* %a, align 4
  %a1 = load i32, i32* %a, align 4
  %calltmp2 = call i32 @func2()
  %subtmp = sub i32 %a1, %calltmp2
  call void @putint(i32 %subtmp)
  ret i32 0
}
