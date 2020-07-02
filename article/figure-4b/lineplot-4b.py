import seaborn as sns
import sys
import csv
from statistics import stdev
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick
import gc

files = [
    {'file': 'a1g9', 'bonus': '0.01'},
    {'file': 'a3g7', 'bonus': '0.03'},
    {'file': 'a5g5', 'bonus': '0.05'},
    {'file': 'a7g3', 'bonus': '0.07'},
    {'file': 'a9g1', 'bonus': '0.09'}
]

al = []

for f in files:
    with open(f['file']+'/'+f['file']+'.txt') as csv_file_r:
        print(f['file'])
        csv_reader = csv.reader(csv_file_r, delimiter=';')
        e00 = []
        for row in csv_reader:
            if(row[0]!='partial'):
                qta = int(row[0])
                qtb = int(row[1])
                result = 'Undef.'
                if qta == 500:
                    result = 'A'
                elif qta == 0:
                    result = 'B'
                e00.append([qta,qtb,result,f['bonus']])
        al += e00

all = pd.DataFrame(al, columns=['qta', 'qtb', 'type', 'bonus'])
print(all)
resumo = all.groupby(["bonus", "type"])["qta"].count().unstack(fill_value=0).stack().reset_index(name="sum")

fig_dims = (6, 4)
fig, ax = plt.subplots(figsize=fig_dims)

print(resumo)

fig = sns.lineplot(data=resumo, x="bonus", y="sum", hue="type")
plt.legend(bbox_to_anchor=(1.05, 1.0), loc='upper left')

ax.set(xlabel="alpha A", ylabel="Fixation %" )
ax.yaxis.set_major_formatter(mtick.PercentFormatter(xmax=5000000))
#ax.xaxis.set_major_formatter(mtick.ScalarFormatter())
#ax.set_xticks(resumo['bonus'].unique())
#plt.setp(ax.get_xticklabels(), rotation=90, horizontalalignment='center')
plt.ylim(1200000,2000000)
plt.tight_layout()

plt.show()
plt.savefig("lineplot-4b.svg")
plt.savefig("lineplot-4b.png", dpi=200)
