# SORACOM UG Explorer 2019

## WioLTE初級 WioLTEと絶対圧センサーを触って体験するハンズオン

## 本日の機材
各自の机の上に、本日必要な機材を用意しています。
* パソコン(お持ち込み品)
    * 事前に案内済みの [セットアップ](setup) が完了していること
* 有効なSORACOMアカウント（すでにソラコム側でアカウントを作成済みです）
* SORACOMアカウントに登録済みの SORACOM Air SIM （すでにWio LTEの中に入っています）
* [Grove IoT スターターキット for SORACOM](https://soracom.jp/products/#grovestarter_kit)
* [絶対圧センサ](https://www.switch-science.com/catalog/5329/)

![機材](https://docs.google.com/drawings/d/e/2PACX-1vQDtAOALHo8MhG_Hr1LUfVJvOfrVJjOslUvKhTvGKmcQ1KH849J-RsXl3VXsuTCytJJceyVkG3Rjlbl/pub?w=757&h=540)

## DFUモードへの切り替え方

Wio LTE は２つのモードを持っています。  
**この操作は Wio LTE の開発で何度も行うことになりますので、必ず覚えてください**

* 書き込まれたプログラムを実行する「通常モード」
* プログラムを書き込むことができる「DFUモード」（Device Firmware Upgradeモード）

![フロー](https://docs.google.com/drawings/d/e/2PACX-1vQAcnymqWTTneRwnc9EFz21YvrmfCsIuV33yfqf1ODC_LKQR-6762CJDMclRIWC8BfUeDDLpC6KKs-2/pub?w=581&h=253)

「通常モード」の Wio LTE に対して、下記操作を行います

1. *BOOTボタン* を押し、 **押し続けてください**
2. *RSTボタン* を押し、離します
3. 押し続けていた *BOOTボタン* を離します

DFUモードへの移行方法  
※動画ではmicroUSB接続をしていませんが、実際は接続した状態で行ってください

外観からはどちらのモードで動いているのか判別できません。PC のデバイスマネージャ（もしくはシステム情報）で確認することができます。  
確認方法は[DFU モードの確認の仕方](setup#dfu2)を参照してください。

![DFUモードへの移行](http://drive.google.com/uc?export=view&id=1447mCTbYS7iMTtVWaTkXJzHD8vJ8lprJ)
![Wio LTE を横からみた図](https://docs.google.com/drawings/d/e/2PACX-1vRnhRiZC7-jRCqLaxJO6E7Bmq0_8BxornXgP1y6UHdYXhr6iBm_RNoV148oSzJKeHBYXRjYai9msQoz/pub?w=480&h=249)

<h2 id="handson2">絶対圧センサの値をSORACOM Harvestで可視化しよう！</h2>

1. 配線
I2C 端子に 絶対圧センサーを取りつけます。

![配線](media/1-4.png)

2. Wio LTEにスケッチ例を書き込む

下記の「スケッチ書き込みページをページをクリックし、手順に沿って書き込みを行ってください。

[スケッチ書き込みページ](webdfu/)   
[https://masato-ka.github.com/explorer2019-handson/handson1/webdfu](webdfu/)

Connect ボタンを押し、**STM32 BOOTLOADER**を選択して「接続」をクリックします。

「対応デバイスが見つかりませんでした」 が一覧に表示された場合は Wio LTE が DFU モードになっていません。 DFU モードへの移行操作をしてください。

![フロー](https://docs.google.com/drawings/d/e/2PACX-1vTxepoPYMnANUwLFFHmhc6I6vFyM_aHL-eD-VFIuzAzk1ND5HLFxw093f7Qy3ccZoEc82NHvyoVaaYB/pub?w=924&h=469)

![フロー](https://docs.google.com/drawings/d/e/2PACX-1vRqDKcttYBSi-f9uHFkOa5f-DKWIwafpvZExjS_SKInhhK4L4Rv3cHmwuHtZAZoeKfGn9iD-NzYm1mg/pub?w=480&h=360)

「【絶対圧センサ】を書き込む」をクリックして、スケッチ例を書き込みます。

![フロー](https://docs.google.com/drawings/d/e/2PACX-1vQPJgqVL27iP9fttdDO9tneBADtHXc6bd1oEcfWo0BDIu29fcvKw3V632ttvzg3VIU0Van47iQgXn7D/pub?w=927&h=467)

書き込んだコードを知りたい方はこちらをクリックください。

[絶対圧センサ読み取りコード](/sketch/baro-harvest.ino)

3. SORACOM Harvest で駐車時間を可視化します。

ソラコムのアカウントにログインします。

[アカウントログインページ](https://console.soracom.io/#/?coverage_type=jp)  
[https://console.soracom.io/#/?coverage_type=jp](https://console.soracom.io/#/?coverage_type=jp)

ログイン情報はPC横の紙をご覧ください。
メールアドレス、パスワードを入力し、ログインをクリックします。

![フロー](https://docs.google.com/drawings/d/e/2PACX-1vTy5KORqQjieOg-ijF5CLyfhALMq-CmE8G9822NahydZSw5zIYqcz4efiru6R5n1RjAEaotdmfNmXsN/pub?w=444&h=250)

SIMを選択し、「操作」からデータを確認を選びます。

![フロー](https://docs.google.com/drawings/d/e/2PACX-1vTyI9zm46Q4RYmDi6wYD0_Q0sFUfmJu-XH_KYR_eDmR7u0ydc1nqIj0jhV_bf1fB5NNn2N_OUEcYHY-/pub?w=455&h=522)

SORACOM Harvet Dataの画面が開きます。しばらく待つと絶対圧センサーの値がグラフになって表示されます。
絶対圧センサの高さを変えて、気圧の変化を計測しましょう。

![parking](https://docs.google.com/drawings/d/e/2PACX-1vSSa_HTsUnGurBhtTo0UTDry01HDujPAx1CNybl2nkvOsdGtqNmYYSpOI8dsMtNxl8w0yEoVDb4XYe_/pub?w=937&h=441)

## 片付け
これでハンズオンは終了です。最後まで終わった方は、センサーを外し元あった場所にお戻しください。