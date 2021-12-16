import tkinter as tk
from tkinter import ttk

class Game(tk.Frame):
    def __init__(self, size, master=None):
        super().__init__(master)
        self.pack()
        self.board = [[0 for i in range(size)] for i in range(size)]
        self.board[10][10] = 2    # COM OUT
        self.board[9][10] = 2    # COM OUT
        self.board[10][11] = 2    # COM OUT
        self.board[11][11] = 1    # COM OUT
        self.size = size
        self.player = 1
        self.white = tk.PhotoImage(file='white.png')
        self.black = tk.PhotoImage(file='black.png')
        self.gray = tk.PhotoImage(file='gray.png')

    def print_board(self):
        for line in self.board:
            print(line)

    def create_window(self):
        lbl_name = ttk.Label(self, text="Go Go Gomoku")
        lbl_name.pack()
        self.frm_board = ttk.Frame(self, relief=tk.RAISED, borderwidth=10, height=500, width=500)
        self.frm_board.pack(padx=25, pady=25)
        self.display_board()
        self.mainloop()

    def handle_click(self, args):
        i, j = args
        print(f"Clicked on row: {i}, col: {j}")
        # How to destroy the button before creating a new one?
        self.play_game(i, j)

    def pick_color(self, i, j):
        if self.board[i][j] == 0:
            button_img=self.gray
        elif self.board[i][j] == 1:
            button_img=self.white
        else:
            button_img=self.black
        return button_img

    def display_board(self):
        for i in range(self.size):
            for j in range(self.size):
                frm_position = ttk.Frame(
                    master=self.frm_board,
                    relief=tk.RIDGE,
                    borderwidth=1
                    )
                button_img = self.pick_color(i, j)
                frm_position.grid(row=i, column=j, padx=5, pady=5)
                button = tk.Button(
                    master=frm_position\
                    , image=button_img\
                    , command=lambda row=i, column=j: self.handle_click((row, column))
                    , height=26
                    , width=26
                    )
                button.pack()

    def make_move(self, i, j):
        # delete before add a new one
        self.board[i][j] = self.player
        # https://stackoverflow.com/questions/12364981/how-to-delete-tkinter-widgets-from-a-window
        # if self.board[i][j] == 1:
        #     button_img=self.white
        # else:
        #     button_img=self.black
        # frm_position = ttk.Frame(
        #         master=self.frm_board,
        #         relief=tk.RIDGE,
        #         borderwidth=1
        #     ).grid(row=i, column=j, padx=5, pady=5)
        # button = tk.Button(
        #     master=frm_position\
        #     , image=button_img\
        #     , command=lambda row=i, column=j: self.handle_click((row, column))
        #     , height=26
        #     , width=26
        #     )
        # button.pack()

    def change_player(self):
        if self.player == 1:
            self.player = 2
        else:
            self.player = 1

    def play_game(self, i, j):
        if self.board[i][j] == 0:
           self.make_move(i, j)
        else:
            print("Position taken")
            return
        self.change_player()
        # self.display_board()


    def update_board(self, i, j):
        pass
