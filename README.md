# skkfep

from 

http://ftp.nara.wide.ad.jp/pub/Linux/gentoo-portage/app-i18n/skkfep/skkfep-0.87-r1.ebuild

- [オリジナルのREADME](./README)

## 構成

```
+-+stdin +-------+ pty +----+
|T| ==>  |skkfep | ==> |fork|
|E| <==  | romkan| <== +----+
|R|stdout| kkconv|
|M| <==  +-------+
+-+ statuline
    predit
```

## TODO

Ubuntu-22.04(WSL) で作業中・・・

- [x] meson でビルド
- [x] segmentation fault. よくわからないので プロジェクト全体を `c++` にすることで、ゆるい暗黙の cast を防止した。動くようになったがEUC-JPで読めない
- [x] euc-jp/sjis/iso-2022-jp変換を削除
- [x] utf-8 で動くように改造(ある程度動くようになってきた)
- [ ] unittest可能にする
- [ ] 文字幅の判定。`戻る・消す・戻る` の幅があっていないような気がする。utf-8 の3byteが3colになって戻りすぎ
- [ ] termcap 非依存
- [ ] Windows版
- [ ] libuv
- [ ] コメントアウトした keymap の第２引数(OkuriFirst)を復旧する

## 削除しちゃった

- server
- system dict
- candlist.okuri。`[` がJISYO.L に入っていなかったのでよくわからない
- 文字コード変換(utf-8決め打ち)
- UNIX固有コード(最近の Linux で動けばいいやという割り切り)

