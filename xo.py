import numpy as np
from tqdm import tqdm

state_board = np.ones((20000), dtype=int) * 0.5

def id_from_state(state):
    num = 0
    for i in range(3):
        for j in range(3):
            num *= 3
            num += state[i][j]
    return num

def state_from_id(num):
    state = np.zeros((3, 3), dtype=int)
    for i in range(3):
        for j in range(3):
            state[i][j] = num % 3
            num //= 3
    return state

def is_winning(state, player):
    # diagonals 
    if state[0][0] == player and state[1][1] == player and state[2][2] == player:
        return True
    if state[0][2] == player and state[1][1] == player and state[2][0] == player:
        return True
    
    for i in range(3):
        # rows
        if state[i][0] == player and state[i][1] == player and state[i][2] == player:
            return True
        # columns
        if state[0][i] == player and state[1][i] == player and state[2][i] == player:
            return True
            
    return False

def is_terminal(state):
    if is_winning(state, 1) or is_winning(state, 2):
        return True
    
    for i in range(3):
        for j in range(3):
            if state[i][j] == 0:
                return False
            elif state[i][j] == 1:
                continue
            elif state[i][j] == 2:
                continue
    return True

def update_state_value(state, next_state, alpha=0.1):
    state_id = id_from_state(state)
    next_state_id = id_from_state(next_state)
    state_board[state_id] = state_board[state_id] + alpha * (state_board[next_state_id] - state_board[state_id])

def render_state(state, mark=False):
    print("Current state: ==================")
    for i in range(3):
        row = "" if not mark else "| "
        for j in range(3):
            if state[i][j] == 0:
                row += ". "
            elif state[i][j] == 1:
                row += "X "
            elif state[i][j] == 2:
                row += "O "
        print(row)
    print("Current state value:", state_board[id_from_state(state)])
    print('=================================')

def initialize_state_board(main_player=1):
    global state_board
    state_board = np.ones((19700), dtype=int) * 0.5

    for state_id in range(19700):
        if state_id >= 19683:  # 3^9 = 19683 possible states
            break
        state = state_from_id(state_id)
        if is_winning(state, main_player) and not is_winning(state, 2 if main_player == 1 else 1):
            state_board[state_id] = 1.0
        elif is_winning(state, 2 if main_player == 1 else 1):
            state_board[state_id] = 0.0

state_buffer = []
def get_state_buffer():
    global state_buffer

    ini_state = 0
    end_state = 19682

    if len(state_buffer) == 0:
        return range(ini_state, end_state + 1)
    else:
        return state_buffer
    
def one_run_table_update():
    list_states = get_state_buffer()
    first_run = len(state_buffer) == 0

    for state_id in list_states:
        state = state_from_id(state_id)

        # check if the state is valid
        # amount of X's and O's must be equal or X's must be one more than O's
        x_count = np.sum(state == 1)
        o_count = np.sum(state == 2)
        if not (x_count == o_count or x_count == o_count + 1):
            continue
        
        # check if the state is terminal
        if is_terminal(state):
            continue

        if first_run:
            state_buffer.append(state_id)

        # player one update
        for i in range(3):
            for j in range(3):
                if state[i][j] == 0:
                    next_state = np.copy(state)
                    next_state[i][j] = 1
                    update_state_value(state, next_state, alpha=0.1)

        # player two update
        for i in range(3):
            for j in range(3):
                if state[i][j] == 0:
                    next_state = np.copy(state)
                    next_state[i][j] = 2
                    update_state_value(state, next_state, alpha=0.1)
                    

if __name__ == "__main__":
    initialize_state_board()
    iterations = 5000
    
    for i in tqdm(range(iterations), desc="Updating state board"):
        one_run_table_update()

    print('Exporting state board...')
    state_board.astype(np.float32).tofile("w_xo_array.bin")

    # copy data into game data
    import shutil
    shutil.copy("w_xo_array.bin", "../../sdl/XOGame/assets/data/w_xo_array.bin")
    print('State board exported successfully!')


