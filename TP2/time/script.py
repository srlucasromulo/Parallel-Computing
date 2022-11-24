import pandas as pd

df = pd.read_csv('time_elapsed', names=['function', 'time'], sep=';')

df = df.groupby(['function'], as_index=False).mean()

df.to_csv('../profile', index=False, sep=';')
