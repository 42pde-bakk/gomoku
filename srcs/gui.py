from tkinter import messagebox

def congratulate_winner(player: int):
    undo = messagebox.askquestion('Congratulations', f"🎉🎉🎉 PLAYER {player} WON 🎉🎉🎉\nUndo winning move?")
    if undo == 'yes':
        return False
    res = messagebox.askquestion("Try again", "New game?")
    if res == 'no':
        exit(1)
    else:
        return True
