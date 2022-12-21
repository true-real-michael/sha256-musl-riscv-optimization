from random import seed, randint

seed(0)

N_BYTES = 1024 * 16

with open('../input/input.txt', 'wb') as file:
    file.write(
        bytes(randint(1, 255) for i in range(N_BYTES))
    )
