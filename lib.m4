define(`source',`syscmd(`cat $1 | sed "s/    /\t/g"')')dnl
define(`header',
`学籍番号: 201111365
名前: 坂口和彦
課題番号: $1
練習問題番号: $2
題名: $3
')dnl
define(`footer',
`このレポートと付属するソースコードの最新版は、以下の URI から参照できる。

https://github.com/coins11/system-program/tree/pi8027/dnl
syscmd(`pwd | sed "s/^.*\/\([^\/]*\)$/\1/"')dnl
')dnl
