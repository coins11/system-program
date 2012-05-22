include(`../lib.m4')dnl
define(`run210',`dnl
syscmd(`echo "> $1"')dnl
syscmd(`echo "$1" | ./210 2>&1 | sed "s/^/< /"')dnl
')dnl
header(`2', `210', `文字、文字列、標準入出力')dnl

今回の課題では練習問題210を選択した。以下にソースコードを示す。

source(`210.c.m4')dnl

実行時に計算したくない部分を C のプリプロセッサで記述できなかったため、GNU M4 を
使用した。このプログラムをコンパイルするには、GNU M4 を通した上で C コンパイラに
渡す必要がある。

このプログラムの実装は、再帰下降パーサを素直に実装したものである。ただし、|, <,
> を prefix としてコマンドやファイル名を読むために、ungetc を用いて入力ストリー
ムの先頭に | を追加した。

このプログラムは以下の文法 input を受理する。

space       ::= "\t" | " "

spaces      ::= { spaces }

command     ::= alphabet { alphabet | digit | space }

file        ::= alphabet { alphabet | digit }

input1      ::= "|" spaces command spaces input1
             |  spaces

input2      ::= "|" spaces command spaces input2
             |  "<" spaces file    spaces input1
             |  spaces

input3      ::= "|" spaces command spaces input3
             |  ">" spaces file    spaces input1
             |  spaces

input4      ::= "|" spaces command spaces input
             |  "<" spaces file    spaces input3
             |  ">" spaces file    spaces input2
             |  spaces

input       ::= spaces command spaces input4

< の直後にあるファイル名は最初のコマンドの入力にリダイレクトされ、> の直後にある
ファイル名は最後のコマンドの出力のリダイレクト先となり、コマンド列は先頭から順に
パイプで繋がれることを意味する。

以下に、いくつかの入出力例を示す。"> "で始まる行は入力を示し、"< "で始まる行は出
力を示す。

run210(`cat file | grep hoge')dnl

run210(`grep hoge < file')dnl

run210(`date > file')dnl

run210(`a | b |c d | e| f g')dnl

run210(`a < input| b c| d > output')dnl

run210(`a | b c> output |d < input')dnl

run210(`a < input1| b c |d < input2> output')dnl

6番目の実行例より、課題で求められている1つ目の要件「異なるリダイレクション記号の
出現順序が変わっても対応できるように」を満たしていることが分かる。

7番目の実行例より、課題で求められている2つ目の要件「同じリダイレクション記号が複
数出てきたらエラーメッセージが出力されるように」を満たしていることが分かる。

4番目から6番目までの実行例より、課題で求められている3つ目の要件「スペースがコマ
ンド，リダイレクション記号，ファイル名の周りに入っていなくても，または複数個入っ
ていても，同じように処理できるように」を満たしていることが分かる。

footer()dnl
