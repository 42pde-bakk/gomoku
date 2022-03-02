from tkinter import messagebox


def congratulate_winner(player: int):
    messagebox.showinfo('Congratulations', f"🎉🎉🎉 PLAYER {player} WON 🎉🎉🎉")
    res = messagebox.askquestion("Try again", "New game?")
    if res == 'no':
        exit(1)
    else:
        return True
