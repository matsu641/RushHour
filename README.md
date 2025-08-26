## Rush Hour（駐車場パズル）

このプロジェクトは、有名なパズルゲーム「Rush Hour」にインスパイアされて作ったゲームで、赤い車を他の車を動かして出口まで導くことでゲームクリアとなる。

---

### 作った理由

トロント大学の**ECE243（Computer Organization）**の課題として制作した。  
組み込みシステムや低レベルのハードウェア制御を学ぶ一環として、**VGA描画**や**PS/2デバイスからの入力処理**をC言語で実装した。

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

### 動画

![Demo](video.gif)

---
### 使い方
1.  Enterを押す。
2.   動かしたい車に対応するアルファベットをキーで打つ。
3.   赤い車を出口に達するようにパズルを解く。
---
### 実装のポイント

1. **表示とグラフィック**
   - VGA出力（仮想）を使って2Dグリッドを描画。
   - 各車は異なる色の長方形で表示。

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
- **入力デバイス**：PS/2キーボード（IRQ line 22から割り込み）
- **出力デバイス**：VGAディスプレイ（double buffering使用）

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
-「SUCCESS!」の勝利メッセージ表示

---

### Rush Hour Puzzle Game – FPGA-Based Implementation

---

**Project Overview**  
Inspired by the classic *Rush Hour* puzzle game, this project challenges players to move surrounding vehicles and clear a path to guide the red car to the exit. The game was developed as part of the ECE243 (Computer Organization) course at the University of Toronto, with the goal of gaining hands-on experience in embedded systems and low-level hardware control.

---
**Why I Built It**  
The project served as a practical exercise in system-level C programming, memory-mapped I/O, and interrupt-driven input/output. It was implemented entirely in C and designed to run on a simulated DE1-SoC FPGA board using the CPUlator platform.

---
### Demonstration
![image](https://github.com/user-attachments/assets/59ec1de5-9b02-49c9-a887-f154ea2b9718)

![image](https://github.com/user-attachments/assets/99e93eae-c625-41dc-bc4a-36dbe19ee9d6)
---
**Gameplay Summary**  
- Players move cars within a parking lot grid to free the red car and reach the exit.
- Vehicles can only move along their orientation (horizontal → left/right, vertical → up/down).
- The game tracks the elapsed time using a count-up timer displayed on the HEX display.
- The puzzle is cleared once the red car reaches the designated exit area.
---
**How It Works**  
1. Press `Enter` to begin.  
2. Select a car using its assigned letter (e.g., `A`, `B`, etc.).  
3. Move the car using arrow keys to solve the puzzle.
---
**Technical Highlights**  
- **Graphics & Display**:  
  - Simulated VGA output is used to render a colorful 2D grid.  
  - Each vehicle is displayed as a colored rectangle with distinct identifiers.  
  - Double buffering ensures flicker-free visual updates.  
- **User Input**:  
  - PS/2 keyboard input is handled via interrupt (IRQ line 22).  
  - Keyboard controls include car selection (A, B...) and movement via arrow keys.  
- **Game Logic**:  
  - Vehicle states (position, orientation) are managed via C structures.  
  - Validations prevent overlaps and boundary violations.  
  - Beep sounds are triggered on selection, movement, and puzzle completion.  
- **Testing & Demonstration**:  
  - Collision detection and movement were tested in various scenarios.  
  - Full gameplay and puzzle completion were demonstrated successfully on CPUlator.
---
**Development Environment**  
- **Platform**: [CPUlator](https://cpulator.01xz.net/)  
- **Architecture**: RISC-V (DE1-SoC simulation mode)  
- **Language**: C  
- **Input**: Simulated PS/2 Keyboard  
- **Output**: VGA display with double buffering  
---
**Deliverables**  
- Complete source code (modularized: drawing, input, logic)  
- Design documentation detailing FSMs, data structures, and rendering logic  
- Final report including implementation strategy, testing procedures, and future plans  
---
**Planned Enhancements**  
- Smooth car animation in 22-pixel steps  
- Car selection via PS/2 mouse  
- Enhanced sound effects during interaction and a "SUCCESS!" victory message with celebratory audio  

---
