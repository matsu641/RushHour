## Car Park Puzzle（駐車場パズル）

このプロジェクトは、CPUlator（DE1-SoC Simulator）上で動作するスライディングパズルゲーム「Car Park Puzzle（駐車場パズル）」です。  
有名なパズルゲーム「Rush Hour」にインスパイアされており、赤い車を他の車を動かして出口まで導くことが目的です。

---

### 作った理由

本プロジェクトは、トロント大学の**ECE243（Computer Organization）**の課題として制作されました。  
組み込みシステムや低レベルのハードウェア制御を学ぶ一環として、**VGA描画**や**PS/2デバイスからの入力処理**をC言語で実装しています。

---

### ゲーム概要

- **CPUlator**を使って、仮想的にDE1-SoCボードの動作を再現。
- 駐車場のグリッド上にランダムに配置された車を動かして、赤い車を出口へと導きます。
- 車は向いている方向にしか動かせません（横向き＝左右、縦向き＝上下）。
- 赤い車の先端が出口に達するとパズルクリアとなります。
- ゲーム中はカウントアップタイマーがHEXディスプレイ上に映され、ゲームクリアに何秒かかったか見ることができます。

---
### デモンストレーション
![image](https://github.com/user-attachments/assets/59ec1de5-9b02-49c9-a887-f154ea2b9718)

![image](https://github.com/user-attachments/assets/99e93eae-c625-41dc-bc4a-36dbe19ee9d6)

---
### 使い方
1.  Enterを押す。
2.   動かしたい車に対応するアルファベットをキーで打つ。
3.   赤い車を出口に達するようにパズルを解く。
---
### 実装のポイント

1. **表示とグラフィック**
   - VGA出力（仮想）を使って2Dグリッドを描画。
   - 各車は異なる色の長方形で表示されます。

2. **ユーザー入力**
   - PS/2キーボード（CPUlator上でシミュレート）を使用。
   - キーボード：`A`, `B`などのキーで車選択、矢印キーで移動。

3. **ゲームロジック**
   - 車の位置と向きは内部の構造体で管理。
   - 他の車との衝突や枠外への移動を防ぐバリデーションを実装。
   - 車の選択・移動時、クリア時にビープ音が鳴るよう実装。

4. **テストとデモ**
   - 正しく移動できるか、衝突判定が機能しているかを確認。
   - ゲームプレイとクリアの一例をシミュレーション上で動作確認済み。

---

### 実行環境・開発環境

- **プラットフォーム**：CPUlator (https://cpulator.01xz.net/)
- **対象アーキテクチャ**：RISC-V (DE1-SoC Simulator モード)
- **開発言語**：C言語
- **入力デバイス**：PS/2キーボード、PS/2マウス（CPUlatorによりシミュレート）
- **出力デバイス**：VGAディスプレイ（CPUlator上の仮想画面）

---

### 成果物

- **ゲーム本体**：C言語で記述されたパズルゲーム（CPUlator上で動作）
- **レポート**：設計方針、データ構造、FSMや描画処理の詳細、テスト内容
- **コード**：描画、入力、ロジックを実現するすべてのCコードファイル

---

### 実装予定の機能
- 車の移動アニメーション（滑らかに22ピクセル単位で移動）
- PS/2マウスによる車の選択
- 車の選択時／移動時のビープ音
- 勝利時のサウンド再生
-「YOU WIN!」の勝利メッセージ表示

