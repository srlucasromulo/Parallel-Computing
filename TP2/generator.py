import random

gen_prey = 1
gen_predator = 3
gen_food = 2
n_gen = 25
r = 1000
c = 1000
n = 0

buffer = ''

for x in range(r):
	for y in range(c):
		rnd = random.random()
		if rnd > 0.2:
			n += 1
		if 0.2 < rnd < 0.3:
			buffer += f'ROCHA {x} {y}\n'
		if 0.3 < rnd < 0.65:
			buffer += f'COELHO {x} {y}\n'
		if 0.65 < rnd < 1.:
			buffer += f'RAPOSA {x} {y}\n'

buffer = f'{gen_prey} {gen_predator} {gen_food} ' + \
			f'{n_gen} {r} {c} {n}\n' + buffer

with open('input', 'w') as fp:
	fp.write(buffer)
