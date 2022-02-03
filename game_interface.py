import tkinter as tk
from tkinter import ttk
from legal_moves import is_last_in_capture, move_is_legal

class Game(tk.Frame):
    def __init__(self, size, master=None):
        super().__init__(master)
        self.pack()
        # self.geometry('300x200') DOEST WORK
        self.board = [[0 for i in range(size)] for i in range(size)]
        self.buttons = []
        self.frm_position = []
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
        self.new_game_bt()
        self.display_board()
        self.mainloop()

    def handle_click(self, args):
        i, j = args
        print(f"Clicked on row: {i}, col: {j}")
        self.play_game(i, j)
        # if self.board[i][j] == 0:
        #     self.buttons[i * self.size + j].destroy()

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
                self.frm_position.append(ttk.Frame(
                    master=self.frm_board
                    , relief=tk.RIDGE
                    , borderwidth=1
                    ))
                button_img = self.pick_color(i, j)
                self.frm_position[i * self.size + j].grid(row=i, column=j, padx=5, pady=5)
                self.buttons.append(tk.Button(
                    master=self.frm_position[i * self.size + j]\
                    , image=button_img\
                    , command=lambda row=i, column=j: self.handle_click((row, column))
                    , height=26
                    , width=26
                    ))
                self.buttons[i * self.size + j].pack()

    def make_move(self, i, j):
        # self.board[i][j] = self.player
        # move_is_legal(self.board, i, j)
        button_img = self.pick_color(i, j)
        self.buttons[i * self.size + j] = tk.Button(
            master=self.frm_position[i * self.size + j]\
            , image=button_img\
            , command=lambda row=i, column=j: self.handle_click((row, column))
            , height=26
            , width=26
            )
        self.buttons[i * self.size + j].pack()
        self.print_board()

    def change_player(self):
        if self.player == 1:
            self.player = 2
        else:
            self.player = 1

    def play_game(self, i, j):
        if self.board[i][j] == 0:
            if move_is_legal(self.board, i, j, self.player):
                print("Illegal move")
                return
            self.board[i][j] = self.player
            self.buttons[i * self.size + j].destroy()
            self.make_move(i, j)
        else:
            print("Position taken")
            return
        # Check for captures/win
        self.change_player()
        # self.display_board()

    def delete_buttons(self):
        for i in range(len(self.buttons)):
            self.buttons[i].destroy()
            self.frm_position[i].destroy() # Could there be different lengths here? Empty positions
        self.buttons = []
        self.frm_position = []

    def reset_board(self):
        self.player = 1
        self.board = [[0 for i in range(self.size)] for i in range(self.size)]
        self.delete_buttons()
        self.display_board()

    def new_game_bt(self):
        bt_new_game = tk.Button(
            text="NEW GAME"
            , command=self.reset_board
            , width=25
            , height=5
            , bg="gray"
            , fg="red"
            )
        bt_new_game.pack()

    def update_board(self, i, j):
        pass
