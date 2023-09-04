source_filename = "Module"
target triple = "x86_64-apple-darwin18.7.0"

define i32 @factorial(i32) {
entry:
  %eq = icmp eq i32 %0, 0   ; n == 0
  br i1 %eq, label %then, label %else

then:                                             ; preds = %entry
  br label %ifcont

else:                                             ; preds = %entry
  %sub = sub i32 %0, 1   ; n - 1
  %1 = call i32 @factorial(i32 %sub) ; factorial(n-1)
  %mult = mul i32 %0, %1  ; n * factorial(n-1)
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  %iftmp = phi i32 [ 1, %then ], [ %mult, %else ]
  ret i32 %iftmp
}
define i32 @main(){
entry:
  %0 = call i32 @factorial(i32 10)
  ret i32 0
}