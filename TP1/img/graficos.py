import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

## plot cpu x time
# filename = './ctan/time'

# df = pd.read_csv(filename, sep=';')
# df.columns = ['cpu', 'time']

# fig, ax = plt.subplots(figsize=(10, 5))

# fig = sns.barplot(
# 	data=df, 
# 	x='cpu', 
# 	y='time',
# 	ax=ax
# )

# fig.set(
# 	xlabel='processo',
# 	ylabel='tempo(s)',
# 	title='tempo por processo'
# )

# plt.show()


## plot ambiente x time
dados = [
	{
		'ambiente': 'pessoal',
		'tempo' : 32.9
	},
	{
		'ambiente': 'laboratório',
		'tempo' : 7.4
	}
]

df = pd.DataFrame(dados)

fig, ax = plt.subplots()

fig = sns.barplot(
	data=df, 
	x='ambiente', 
	y='tempo',
	ax=ax
)

fig.set(
	xlabel='ambiente',
	ylabel='tempo(s)',
	title='tempo por ambiente'
)

plt.show()
