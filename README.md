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
