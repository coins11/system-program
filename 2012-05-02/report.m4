include(`../lib.m4')dnl
header(`3', `304', `ファイルアクセス')dnl

今回の課題では練習問題304を選択した。以下にソースコードを示す。

source(`304.c')dnl

このプログラムは、講義資料に示されている例を元に、主に read_utmp などを変更して作成した。

以下に acacia01 での実行例を示す。

        |                |        |Mon May  7 09:30:39 2012
s1011426|                | console|Mon May  7 22:04:11 2012
s1011426|                | ttys000|Mon May  7 22:52:14 2012
s1111365|   130.158.199.1| ttys001|Mon May  7 23:42:22 2012
s1011426|                | ttys002|Mon May  7 22:29:27 2012
s1011426|                | ttys003|Mon May  7 22:29:15 2012
================================================================================
        |                |        |Mon May  7 09:30:39 2012
s1011426|                | console|Mon May  7 22:04:11 2012
s1011426|                | ttys000|Mon May  7 22:52:14 2012
s1111365|   130.158.199.1| ttys001|Mon May  7 23:42:22 2012
s1111365|   130.158.199.1| ttys002|Mon May  7 23:42:36 2012
s1011426|                | ttys003|Mon May  7 22:29:15 2012
================================================================================
        |                |        |Mon May  7 09:30:39 2012
s1011426|                | console|Mon May  7 22:04:11 2012
s1011426|                | ttys000|Mon May  7 22:52:14 2012
s1111365|   130.158.199.1| ttys001|Mon May  7 23:42:22 2012
s1111365|   130.158.199.1| ttys002|Mon May  7 23:42:36 2012
s1111365|   130.158.199.1| ttys003|Mon May  7 23:42:57 2012
================================================================================

端末を開いた分だけ増えていっていることが分かる。

footer()dnl
