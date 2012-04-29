define(`source',`syscmd(`cat $1 | sed "s/    /\t/g"')')dnl
define(`header',`学籍番号: 201111365
名前: 坂口和彦
課題番号: $1
練習問題番号: $2
題名: $3
')dnl
