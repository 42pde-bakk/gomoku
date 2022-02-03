from game_interface import Game

if __name__ == "__main__":
    board = Game(size=19)
    board.print_board()
    board.create_window()
