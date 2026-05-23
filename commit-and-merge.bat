@echo off
cd /d "c:\Users\jula_\OneDrive\Pulpit\Super mario game.worktrees\agents-fix-and-launch-game"

echo ===== Staging changes =====
git add -A

echo.
echo ===== Committing changes =====
git commit -m "Fix syntax errors in game code" -m "- Fixed underscores in MapManager.cpp constructor and if statements^
- Fixed asterisks in Student.cpp constructor initialization^
- Added missing view_x parameter to map_manager.update() in main.cpp" -m "Co-authored-by: Copilot <223556219+Copilot@users.noreply.github.com>"

echo.
echo ===== Git worktree list =====
for /f "tokens=1" %%i in ('git worktree list ^| findstr /V agents-fix-and-launch-game ^| findstr /V bare ^| head -1') do set MAIN_WORKTREE=%%i

if "!MAIN_WORKTREE!"=="" (
    set MAIN_WORKTREE=c:\Users\jula_\OneDrive\Pulpit\Super mario game.worktrees
)

echo Main worktree detected at: !MAIN_WORKTREE!

echo.
echo ===== Merging to main branch =====
git -C "!MAIN_WORKTREE!" merge agents-fix-and-launch-game

echo.
echo ===== Merge complete! =====
git log --oneline -3

pause
