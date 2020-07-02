import seaborn as sns
import sys
import csv
from statistics import stdev
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick

pd.set_option('display.max_rows', None)

files = [
    {'file': 'b000', 'bonus': '0.00'},
    {'file': 'b001', 'bonus': '0.01'},
    {'file': 'b002', 'bonus': '0.02'},
    {'file': 'b003', 'bonus': '0.03'},
    {'file': 'b004', 'bonus': '0.04'},
    {'file': 'b005', 'bonus': '0.05'},
    {'file': 'b006', 'bonus': '0.06'},
    {'file': 'b007', 'bonus': '0.07'},
    {'file': 'b008', 'bonus': '0.08'},
    {'file': 'b009', 'bonus': '0.09'},
    {'file': 'b010', 'bonus': '0.10'},
    {'file': 'b011', 'bonus': '0.11'},
    {'file': 'b012', 'bonus': '0.12'},
    {'file': 'b013', 'bonus': '0.13'},
    {'file': 'b014', 'bonus': '0.14'},
    {'file': 'b015', 'bonus': '0.15'},

]

all = []
for f in files:
    with open(f['file']+'/'+f['file']+'.txt') as csv_file_r:
    #with open(f['file']+'.txt') as csv_file_r:
        csv_reader = csv.reader(csv_file_r, delimiter=';')
        qtas = []
        qtbs = []
        results = []
        e00 = []
        for row in csv_reader:
            if(row[0]!='partial'):            
                qta = int(row[0])
                qtb = int(row[1])
                result = 'Undef.'
                if qta == 500:
                    result = 'A fix. simulations'
                elif qta == 0:
                    result = 'B fix. simulations'
                else:
                    #print('appended '+str(qta))
                    #qtas.append(qta)
                    #qtbs.append(qtb)
                    #results.append(result)
                    #e00 = pd.DataFrame(list(zip(qtas,qtbs,results)), columns=['qta', 'qtb', 'type'])
                    #e00["bonus"]=f['bonus']
                    e00.append([qta,qtb,result,f['bonus']])
        all += e00

all = pd.DataFrame(all, columns=['qta', 'qtb', 'type', 'bonus'])
#print(all)
resumo = all.groupby(["bonus", "type"])["qta"].count().unstack(fill_value=0).stack().reset_index(name="sum")

resumo2 = resumo
resumo3 = pd.DataFrame([],columns=['bonus', 'type', 'sum'])
for x in resumo["bonus"].unique():
    u_x = all.loc[(all["type"]=="Undef.") & (all["bonus"]==x)]    
    u_x_a = u_x["qta"].sum()
    u_x_b = u_x["qtb"].sum()
    sumab = u_x_a + u_x_b
    col_a = {"bonus":x, "type":"A", "sum":u_x_a/sumab}
    col_b = {"bonus":x, "type":"B", "sum":u_x_b/sumab}
    resumo3 = resumo3.append(col_a, ignore_index=True)
    resumo3 = resumo3.append(col_b, ignore_index=True)
print(resumo3)

'''fig_dims = (6, 4)
fig, ax = plt.subplots(figsize=fig_dims)
fig = sns.barplot(data=resumo3, x="bonus", y="sum", hue="type")
ax.yaxis.set_major_formatter(mtick.PercentFormatter(xmax=1))
plt.legend(bbox_to_anchor=(1.05, 1.0), loc='upper left')
ax.set(xlabel="α", ylabel="Nodes in undefined simulations" )
plt.setp(ax.get_xticklabels(), rotation=90, horizontalalignment='center')
plt.tight_layout()'''

#ct = pd.crosstab(resumo3.bonus, resumo3.type, normalize="index" )
#ct = pd.crosstab(resumo3.bonus, resumo3.type, resumo3.sum , aggfunc="sum",normalize="index" )
#print(ct)
ct = resumo3.pivot(index="bonus", columns="type", values="sum")


fig_dims = (6, 4)
#fig, ax = plt.subplots(figsize=fig_dims)
fig = ct.plot.bar(stacked=True)
fig.yaxis.set_major_formatter(mtick.PercentFormatter(xmax=1))
plt.legend(bbox_to_anchor=(1.05, 1.0), loc='upper left')
fig.set(xlabel="α", ylabel="Nodes in undefined simulations" )
plt.setp(fig.get_xticklabels(), rotation=90, horizontalalignment='center')
plt.tight_layout()

#plt.show()

plt.savefig("barplot.svg")
plt.savefig("barplot.png", dpi=200)
'''fig_dims = (6, 4)
fig, ax = plt.subplots(figsize=fig_dims)

fig = sns.lineplot(data=resumo2, x="bonus", y="sum", hue="type")
ax.yaxis.set_major_formatter(mtick.PercentFormatter(xmax=6242))
plt.legend(bbox_to_anchor=(1.05, 1.0), loc='upper left')

plt.tight_layout()
ax.set(xlabel="α", ylabel="Nodes % (All simulations)" )
plt.setp(ax.get_xticklabels(), rotation=90, horizontalalignment='center')

plt.show()'''
