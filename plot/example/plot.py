import seaborn as sns
import sys
import csv
from statistics import stdev
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

file = sys.argv[1]

e00 = pd.read_csv(file)

alla = e00.loc[(e00["type"]=='A')]
allb = e00.loc[(e00["type"]=='B')]

lp = sns.boxplot(data=e00, x="cyc", y="nodes", hue="type")
lp.set(xlabel="Simulation Cycle", ylabel="Node count")

#lp.set_title("Regular A 0.08 x 0.01 B")
 

box = lp.get_position()
lp.set_position([box.x0, box.y0, box.width * 0.85, box.height])
lp.legend(loc='center right', bbox_to_anchor=(1.25, 0.5), ncol=1)
plt.savefig("boxplot-"+file+".png", dpi=200)

alla5k = alla.loc[alla["cyc"]==5000,"nodes"]
allb5k = allb.loc[allb["cyc"]==5000,"nodes"]

perc_file = open("perc-"+file+".txt", "a+")
txt = str(alla5k.describe().apply(lambda x: format(x, 'f'))) + "\n" + str(allb5k.describe().apply(lambda x: format(x, 'f'))) + "\n"
perc_file.write(txt)
perc_file.close()

winc_file = open("fixc-"+file+".txt", "a+")
total = len(alla5k)
awin = len(alla5k.loc[alla5k==500,])
bwin = len(allb5k.loc[allb5k==500,])
undef = total - awin - bwin
awperc = awin / total
bwperc = bwin / total
unperc = undef / total

undefa = alla5k.loc[alla5k<500]
undefa = undefa.loc[undefa>0]

undefb = allb5k.loc[allb5k<500]
undefb = undefb.loc[undefb>0]

lundefa = sum(undefa)/500
lundefb = sum(undefb)/500

winc = str(awin)+","+str(bwin)+","+str(undef)+"\n"+str(awperc)+","+str(bwperc)+","+str(unperc)+","+str(lundefa/total)+","+str(lundefb/total)+"\n"


winc_file.write(winc)
winc_file.close()

