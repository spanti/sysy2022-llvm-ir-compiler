; ModuleID = 'llvm-link'
source_filename = "llvm-link"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.1 = private unnamed_addr constant [3 x i8] c"%c\00", align 1
@.str.2 = private unnamed_addr constant [4 x i8] c"%d:\00", align 1
@.str.3 = private unnamed_addr constant [4 x i8] c" %d\00", align 1
@.str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %d = alloca i32, align 4
  %result = alloca i32, align 4
  store i32 5, ptr %a, align 4
  store i32 5, ptr %b, align 4
  store i32 1, ptr %c, align 4
  store i32 -2, ptr %d, align 4
  store i32 2, ptr %result, align 4
  %d2 = load i32, ptr %d, align 4
  %multmp = mul i32 %d2, 1
  %divtmp = sdiv i32 %multmp, 2
  %icmptmp = icmp slt i32 %divtmp, 0
  %zextmp = zext i1 %icmptmp to i32
  %icmptmp3 = icmp ne i32 %zextmp, 0
  br i1 %icmptmp3, label %then, label %then1

then:                                             ; preds = %then4, %entry
  %result17 = load i32, ptr %result, align 4
  call void @putint(i32 %result17)
  br label %ifcont

then1:                                            ; preds = %then1, %entry
  %a5 = load i32, ptr %a, align 4
  %b6 = load i32, ptr %b, align 4
  %subtmp = sub i32 %a5, %b6
  %icmptmp7 = icmp ne i32 %subtmp, 0
  %zextmp8 = zext i1 %icmptmp7 to i32
  %icmptmp9 = icmp ne i32 %zextmp8, 0
  br i1 %icmptmp9, label %then4, label %then1

then4:                                            ; preds = %then1
  %c10 = load i32, ptr %c, align 4
  %addtmp = add i32 %c10, 3
  %sremtmp = srem i32 %addtmp, 2
  %icmptmp11 = icmp ne i32 %sremtmp, 0
  %zextmp12 = zext i1 %icmptmp11 to i32
  %icmptmp13 = icmp ne i32 %zextmp12, 0
  %zextmp14 = zext i1 %icmptmp13 to i32
  %icmptmp15 = icmp ne i32 %zextmp14, 0
  %zextmp16 = zext i1 %icmptmp15 to i32
  %ifcond = icmp sgt i32 %zextmp16, 0
  br i1 %ifcond, label %then, label %ifcont

ifcont:                                           ; preds = %then4, %then
  %d20 = load i32, ptr %d, align 4
  %sremtmp21 = srem i32 %d20, 2
  %addtmp22 = add i32 %sremtmp21, 67
  %icmptmp23 = icmp slt i32 %addtmp22, 0
  %zextmp24 = zext i1 %icmptmp23 to i32
  %icmptmp25 = icmp ne i32 %zextmp24, 0
  br i1 %icmptmp25, label %then18, label %then19

then18:                                           ; preds = %then26, %ifcont
  store i32 4, ptr %result, align 4
  %result43 = load i32, ptr %result, align 4
  call void @putint(i32 %result43)
  br label %ifcont44

then19:                                           ; preds = %then19, %ifcont
  %a27 = load i32, ptr %a, align 4
  %b28 = load i32, ptr %b, align 4
  %subtmp29 = sub i32 %a27, %b28
  %icmptmp30 = icmp ne i32 %subtmp29, 0
  %zextmp31 = zext i1 %icmptmp30 to i32
  %icmptmp32 = icmp ne i32 %zextmp31, 0
  br i1 %icmptmp32, label %then26, label %then19

then26:                                           ; preds = %then19
  %c33 = load i32, ptr %c, align 4
  %addtmp34 = add i32 %c33, 2
  %sremtmp35 = srem i32 %addtmp34, 2
  %icmptmp36 = icmp ne i32 %sremtmp35, 0
  %zextmp37 = zext i1 %icmptmp36 to i32
  %icmptmp38 = icmp ne i32 %zextmp37, 0
  %zextmp39 = zext i1 %icmptmp38 to i32
  %icmptmp40 = icmp ne i32 %zextmp39, 0
  %zextmp41 = zext i1 %icmptmp40 to i32
  %ifcond42 = icmp sgt i32 %zextmp41, 0
  br i1 %ifcond42, label %then18, label %ifcont44

ifcont44:                                         ; preds = %then26, %then18
  ret i32 0
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @getint() #0 {
  %1 = alloca i32, align 4
  %2 = call i32 (ptr, ...) @__isoc99_scanf(ptr noundef @.str, ptr noundef %1)
  %3 = load i32, ptr %1, align 4
  ret i32 %3
}

declare i32 @__isoc99_scanf(ptr noundef, ...) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @getch() #0 {
  %1 = alloca i8, align 1
  %2 = call i32 (ptr, ...) @__isoc99_scanf(ptr noundef @.str.1, ptr noundef %1)
  %3 = load i8, ptr %1, align 1
  %4 = sext i8 %3 to i32
  ret i32 %4
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @getarray(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store ptr %0, ptr %2, align 8
  %5 = call i32 (ptr, ...) @__isoc99_scanf(ptr noundef @.str, ptr noundef %3)
  store i32 0, ptr %4, align 4
  br label %6

6:                                                ; preds = %16, %1
  %7 = load i32, ptr %4, align 4
  %8 = load i32, ptr %3, align 4
  %9 = icmp slt i32 %7, %8
  br i1 %9, label %10, label %19

10:                                               ; preds = %6
  %11 = load ptr, ptr %2, align 8
  %12 = load i32, ptr %4, align 4
  %13 = sext i32 %12 to i64
  %14 = getelementptr inbounds i32, ptr %11, i64 %13
  %15 = call i32 (ptr, ...) @__isoc99_scanf(ptr noundef @.str, ptr noundef %14)
  br label %16

16:                                               ; preds = %10
  %17 = load i32, ptr %4, align 4
  %18 = add nsw i32 %17, 1
  store i32 %18, ptr %4, align 4
  br label %6, !llvm.loop !6

19:                                               ; preds = %6
  %20 = load i32, ptr %3, align 4
  ret i32 %20
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @putint(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %3 = load i32, ptr %2, align 4
  %4 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %3)
  ret void
}

declare i32 @printf(ptr noundef, ...) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @putch(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %3 = load i32, ptr %2, align 4
  %4 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %3)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @putarray(i32 noundef %0, ptr noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  store i32 %0, ptr %3, align 4
  store ptr %1, ptr %4, align 8
  %6 = load i32, ptr %3, align 4
  %7 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, i32 noundef %6)
  store i32 0, ptr %5, align 4
  br label %8

8:                                                ; preds = %19, %2
  %9 = load i32, ptr %5, align 4
  %10 = load i32, ptr %3, align 4
  %11 = icmp slt i32 %9, %10
  br i1 %11, label %12, label %22

12:                                               ; preds = %8
  %13 = load ptr, ptr %4, align 8
  %14 = load i32, ptr %5, align 4
  %15 = sext i32 %14 to i64
  %16 = getelementptr inbounds i32, ptr %13, i64 %15
  %17 = load i32, ptr %16, align 4
  %18 = call i32 (ptr, ...) @printf(ptr noundef @.str.3, i32 noundef %17)
  br label %19

19:                                               ; preds = %12
  %20 = load i32, ptr %5, align 4
  %21 = add nsw i32 %20, 1
  store i32 %21, ptr %5, align 4
  br label %8, !llvm.loop !8

22:                                               ; preds = %8
  %23 = call i32 (ptr, ...) @printf(ptr noundef @.str.4)
  ret void
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.ident = !{!0}
!llvm.module.flags = !{!1, !2, !3, !4, !5}

!0 = !{!"Ubuntu clang version 16.0.6 (++20230610113324+7cbf1a259152-1~exp1~20230610233415.100)"}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 8, !"PIC Level", i32 2}
!3 = !{i32 7, !"PIE Level", i32 2}
!4 = !{i32 7, !"uwtable", i32 2}
!5 = !{i32 7, !"frame-pointer", i32 2}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
