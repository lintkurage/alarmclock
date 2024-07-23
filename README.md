ESP 32 Alarm Clock Program

このプログラムはESP32を用いて目覚まし時計を作成した時のプログラムです.  
回路図は以下の通りである.  
<img src="https://github.com/lintkurage/alarmclock/issues/1#issue-2405144886">

使用言語:Arudino言語(C++)  

1. 時計機能(esp32clock)  
   7segmentLEDをダイナミック点灯させて時間を表示する物となっている.
   ntpサーバにアクセスして時間を取得する.
3. アラーム機能(esp32alarm)  
   ブザーとスイッチを使った回路の際に使用します
