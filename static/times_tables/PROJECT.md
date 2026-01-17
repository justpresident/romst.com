# Multiplication Tables Game

## Core Mechanics

- 10x10 grid displays all multiplication questions (Row × Col)
- Cells start as unsolved (unless pre-solved by level config)
- Questions auto-pop in sequence: cell expands to screen center, kid enters answer, cell shrinks back
- Correct answer: cell marked solved
- Wrong answer: cell returns unsolved, game continues to next question
- Timer tracks elapsed time
- Level complete when all unsolved cells are solved

## Animation Flow

1. Select next cell (sequential or random based on level config)
2. Cell expands from grid position to screen center
3. Show question, focus input
4. Kid submits answer
5. Cell shrinks back to grid position
6. Update cell state (solved/unsolved)
7. Repeat until done

## Level Configuration

Each level defines:

| Property | Type | Description |
|----------|------|-------------|
| `gridSize` | number | Grid dimensions (e.g., 5 = 5×5) |
| `preSolved` | (a, b) => bool | Returns true if cell [a,b] is pre-solved |
| `flowType` | "sequential" \| "random" | Order questions appear |
| `secondsPerCell` | number | Countdown timer seconds for each question |
| `lives` | number | Number of mistakes allowed before game over |

## Progression

- Levels unlock sequentially
- Progress saved to localStorage
