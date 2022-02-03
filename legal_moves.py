def move_is_legal(board, mv_row, mv_col, player):
    opponent = get_opponent_num(player)
    if is_last_in_capture(board, mv_row, mv_col, player, opponent):
        return True

def is_two_open_threes(board, mv_row, mv_col, player):
    pass

def is_last_in_capture(board, mv_row, mv_col, player, opponent):
    # Take care of sides of the board
    d = [(-1, 0), (-1, -1), (0, -1), (1, -1), (1, 0), (1, 1),  (0, 1), (-1, 1)]
    opp = [(1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0), (-1, -1), (0, -1), (1, -1)]
    opp_plus = [(2, 0), (2, 2), (0, 2), (-2, 2), (-2, 0), (-2, -2), (0, -2), (2, -2)]
    for i in range(8):
        if board[mv_row + d[i][1]][mv_col + d[i][0]]:
            if board[mv_row + opp[i][1]][mv_col + opp[i][0]] == player:
                if board[mv_row + opp_plus[i][1]][mv_col + opp_plus[i][0]] == opponent:
                    return True
    return False


def get_opponent_num(player): # TEST THIS do I write the move before it is checked? if not just pass player param
    if player == 1:
        opponent = 2
    else:
        opponent = 1
    return opponent