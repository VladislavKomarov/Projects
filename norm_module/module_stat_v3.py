import numpy as np
import matplotlib.pyplot as plt
import os
import math
import copy
import json
import requests
import sys
import argparse
import shutil

#-----------------------------------------------------------------------------------------------------------------------

def createParser ():
    parser = argparse.ArgumentParser()
    parser.add_argument('--dis_id', type=int, required=True)
    parser.add_argument('--vegtype', type=str, required=True)
    parser.add_argument('--year_from', type=int, required=True)
    parser.add_argument('--year_to', type=int, required=True)
    parser.add_argument('--tnorm_year', type=int, default=0, required=False)
    return parser

parser = createParser()
namespace = parser.parse_args(sys.argv[1:])
year_stat = namespace.year_to-namespace.year_from + 1

#-----------------------------------------------------------------------------------------------------------------------

'''Расчет среднего межгодового значения накопленной температуре temp_acc по району. Несколько вариантов с учетом выбросов mean;rob;wins'''

temp_acc_interannual_mean = [0] * 366
temp_acc_interannual_rob = [0] * 366
temp_acc_interannual_wins = [0] * 366
temp_acc_data = [0] * 366
for i in range(366):
    temp_acc_data[i] = [0] * year_stat

for i in range(namespace.year_to-namespace.year_from + 1):
    response = requests.get("http://smisdev.vega.smislab.ru/api/v1/rest/mo/:" + str(namespace.dis_id) + "/mo/meteo.json?year=" + str(namespace.year_from + i) + "&param=temp_acc" + "&ukey=53616c7465645f5f88cbb8825c20d0401e1170ef0fad2fbcd855a7345a10c0207e663a68c5faeb8b")
    json_data = json.loads(response.text)
    n = 0
    for json_a in json_data["result"][0]["temp_acc"]:
        n = n + 1
        while n < int(json_a['day']):
            if n == 1:
                temp_acc_data[n - 1][i] = 0
            else:
                temp_acc_data[n - 1][i] = temp_acc_data[n - 2][i]
        temp_acc_data[n - 1][i] = float(json_a['value'])
    if n == 365:
        temp_acc_data[n][i] = temp_acc_data[n - 1][i]

temp_acc_data_copy = copy.deepcopy(temp_acc_data)

#Среднее арифметическое для накопленной температуры

for i in range(0, 366):
    summ = 0
    for j in range(0, year_stat):
        summ = summ + temp_acc_data[i][j]
    temp_acc_interannual_mean[i] = summ / year_stat


#Винзоризованное среднее для накопленной температуры

for i in range(0, 366):
    data_temp = copy.deepcopy(temp_acc_data[i])
    data_temp.sort()
    summ = 0
    for j in range(0, year_stat):
        if j < int(year_stat * 0.15):
            data_temp[j] = data_temp[int(year_stat * 0.15)]
        if j > year_stat - 1 - int(year_stat * 0.15):
            data_temp[j] = data_temp[year_stat - 1 - int(year_stat * 0.15)]
        summ = summ + data_temp[j]
    temp_acc_interannual_wins[i] = summ / year_stat

#Робастный алгоритм А для накопленной температуры

for i in range(0, 366):
    data_temp = copy.deepcopy(temp_acc_data[i])
    temp_acc_data[i].sort()
    j = 0
    median = 0

    if year_stat % 2 == 1:
        median = temp_acc_data[i][int((year_stat - 1)/2)]
    else:
        median = (temp_acc_data[i][int((year_stat - 1)/2)] + temp_acc_data[i][int((year_stat - 1)/2 + 1)]) / 2

    for l in range(year_stat):
        data_temp[l] = abs(data_temp[l] - median)
    data_temp.sort()

    if year_stat % 2 == 1:
        deviation = 1.483 * data_temp[int((year_stat - 1)/2)]
    else:
        deviation = 1.483 * (data_temp[int((year_stat - 1)/2)] + data_temp[int((year_stat - 1)/2 + 1)]) / 2

    for l in range(year_stat):
        if temp_acc_data[i][l] < median - 1.5 * deviation:
            temp_acc_data[i][l] = median - 1.5 * deviation
        else:
            if temp_acc_data[i][l] > median + 1.5 * deviation:
                temp_acc_data[i][l] = median + 1.5 * deviation

    for iteration in range(3):
        summ_m = 0
        summ_d = 0
        for l in range(0, year_stat):
            summ_m = summ_m + temp_acc_data[i][l]
        median = summ_m / year_stat

        for l in range(0, year_stat):
            summ_d = summ_d + pow(temp_acc_data[i][j] - median, 2)
        deviation = 1.134 * math.sqrt(summ_d / (year_stat - 1))

        for l in range(year_stat):
            if temp_acc_data[i][l] < median - 1.5 * deviation:
                temp_acc_data[i][l] = median - 1.5 * deviation
            else:
                if temp_acc_data[i][l] > median + 1.5 * deviation:
                    temp_acc_data[i][l] = median + 1.5 * deviation
    temp_acc_interannual_rob[i] = median

#-----------------------------------------------------------------------------------------------------------------------

'''Здесь полученные данные заносятся в массив, с которым модуль будет работать.'''

print(namespace)
data_weeks = [0] * 53
first_week = 1
for i in range(53):
    data_weeks[i] = [0] * year_stat

for i in range(namespace.year_to-namespace.year_from + 1):
    n = 0
    response = requests.get("http://smisdev.vega.smislab.ru/api/v1/rest/mo/:"+str(namespace.dis_id)+"/mo/ndvi_7dc_modis_int.json?year="+str(namespace.year_to - i)+"&vegtype="+namespace.vegtype+"&ukey=53616c7465645f5f88cbb8825c20d0401e1170ef0fad2fbcd855a7345a10c0207e663a68c5faeb8b")
    json_data = json.loads(response.text)
    for json_a in json_data["result"][0]["ndvi_7dc_modis_int"]:
        n = n + 1
        while n < int(json_a['week']):
            data_weeks[n - 1][i] = 0
            n = n + 1
        data_weeks[n - 1][i] = json_a['value']

data_weeks2 = copy.deepcopy(data_weeks)
data_weeks_copy = copy.deepcopy(data_weeks)

#print(data_weeks)

#-----------------------------------------------------------------------------------------------------------------------

'''Здесь проводится нормировка на температуру одним из вариантов'''

def norm_temp (temp_acc_interannual):
    global data_weeks
    for year in range(0, year_stat):
        if namespace.year_to - year == namespace.tnorm_year:
            for i in range(0, 52):
                data_weeks2[i][year] = data_weeks[i][year]
            continue
        temp_x = [0] * 366
        temp_x_num = [0] * 366
        response = requests.get("http://smisdev.vega.smislab.ru/api/v1/rest/mo/:" + str(namespace.dis_id) + "/mo/meteo.json?year=" + str(namespace.year_to - year) + "&param=temp_acc" + "&ukey=53616c7465645f5f88cbb8825c20d0401e1170ef0fad2fbcd855a7345a10c0207e663a68c5faeb8b")
        json_data = json.loads(response.text)
        n = 0
        for json_a in json_data["result"][0]["temp_acc"]:
            n = n + 1
            while n < int(json_a['day']):
                if n == 1:
                    temp_x[n - 1] = 0
                else:
                    temp_x[n - 1] = temp_x[n - 2]
            temp_x[n - 1] = float(json_a['value'])
        if n == 365:
            temp_x[n] = temp_x[n - 1]


        i = 0
        j = 0
        #f1 = open('out//temp//' + str(namespace.year_to - year) + ".csv", 'w', encoding='utf-8')
        #f1.writelines("day;temp\n")
        ndvi_day = [0] * 366
        while (i < 363) and (j < 365):
            while temp_acc_interannual[i] <= 0:
                i = i + 1
                #f1.writelines(str(i) + ';' + str(temp_x[i]) + '\n')
            while temp_x[j] <= 0 and j < 360:
                temp_x_num[j] = j
                ndvi_day[j] = j
                j = j + 1
            if temp_x[j] > temp_acc_interannual[i]:
                i = i + 1
                #f1.writelines(str(i) + ';' + str(temp_x[j]) + '\n')
            else:
                temp_x_num[j] = i
                ndvi_day[j] = j
                j = j + 1
        if j < 366:
            while j < 365:
                temp_x_num[j] = 363
                ndvi_day[j] = ndvi_day[j - 1]
                j = j + 1

        #print(temp_acc_interannual)
        #f1.close()

        sum = 0
        k = 0
        z = 1
        i = 0
        while i in range(0, 364):
            if temp_x_num[i] < z * 7:
                sum = sum + data_weeks[int(ndvi_day[i] / 7)][year]
                k = k + 1
                i = i + 1
            else:
                if k == 0:
                    data_weeks2[z - 1][year] = 0
                else:
                    data_weeks2[z - 1][year] = sum / k
                k = 0
                sum = 0
                z = z + 1


#-----------------------------------------------------------------------------------------------------------------------

def norm_temp_year(temp_acc_interannual, year_plan):
    for year in range(0, 1):
        temp_x = [0] * 366
        temp_x_num = [0] * 366
        response = requests.get("http://smisdev.vega.smislab.ru/api/v1/rest/mo/:" + str(namespace.dis_id) + "/mo/meteo.json?year=" + str(year_plan) + "&param=temp_acc" + "&ukey=53616c7465645f5f88cbb8825c20d0401e1170ef0fad2fbcd855a7345a10c0207e663a68c5faeb8b")
        json_data = json.loads(response.text)
        n = 0
        for json_a in json_data["result"][0]["temp_acc"]:
            n = n + 1
            while n < int(json_a['day']):
                if n == 1:
                    temp_x[n - 1] = 0
                else:
                    temp_x[n - 1] = temp_x[n - 2]
            temp_x[n - 1] = float(json_a['value'])
        if n == 365:
            temp_x[n] = temp_x[n - 1]


        for t in range(0, 3):
            if t == 0:
                temp_acc_interannual = temp_acc_interannual_mean
            if t == 1:
                temp_acc_interannual = temp_acc_interannual_rob
            if t == 2:
                temp_acc_interannual = temp_acc_interannual_wins
            i = 0
            j = 0
            temp_x_num = [0] * 366
            while (i < 363) and (j < 365):
                while temp_acc_interannual[i] <= 0:
                    i = i + 1
                while temp_x[j] <= 0 and j < 360:
                    temp_x_num[j] = j
                    j = j + 1
                if temp_x[j] > temp_acc_interannual[i]:
                    i = i + 1
                else:
                    temp_x_num[j] = i
                    j = j + 1
            if j < 366:
                while j < 365:
                    temp_x_num[j] = 363
                    j = j + 1
            global data_weeks

            for i in range(0, 52):
                sum = 0
                k = 7
                for j in range(0, 7):
                    if data_weeks[int(temp_x_num[i * 7 + j] / 7)][namespace.year_to - year_plan] == 0:
                        sum = sum + 0
                        k = k - 1
                    else:
                        sum = sum + data_weeks[int(temp_x_num[i * 7 + j] / 7)][namespace.year_to - year_plan]
                if k != 0:
                    temp_acc_year[i][t] = sum / k

#-----------------------------------------------------------------------------------------------------------------------

'''Создается массив данных, по которым строятся графики'''

value = [0] * 5
for i in range(0, 5):
    value[i] = [0] * 52

#-----------------------------------------------------------------------------------------------------------------------

#Среднее арифметическое
def mean():
    for i in range(0, 52):
        summ = 0
        count = 0
        for j in range(0, year_stat):
            if data_weeks2[i][j] != 0:
                summ = summ + data_weeks2[i][j]
                count = count + 1
        if (flag == 1) and (count > 0):
            value[0][i] = summ / count
        else:
            if count > 2:
                value[0][i] = summ / count

#-----------------------------------------------------------------------------------------------------------------------

#Винзоризованное среднее
def wins():
    for i in range(0, 52):
        data_week = copy.deepcopy(data_weeks2[i])
        data_week.sort()
        j = 0
        summ = 0
        while (j != year_stat) and (data_week[j] == 0):
            j = j + 1
        for l in range(j, year_stat):
            if l < j + int((year_stat - j) * 0.15):
                data_week[l] = data_week[int(j + (year_stat - j) * 0.15)]
            if l > year_stat - 1 - int((year_stat - j) * 0.15):
                data_week[l] = data_week[year_stat - 1 - int((year_stat - j) * 0.15)]
            summ = summ + data_week[l]
        if (year_stat - j > 1):
            value[2][i] = summ / (year_stat - j)

#-----------------------------------------------------------------------------------------------------------------------

#Робастный алгоритм А
def rob():
    for i in range(0, 52):
        data_weeks2[i].sort()
        data_week = copy.deepcopy(data_weeks2[i])
        j = 0
        medianD = 0
        while (j != year_stat) and (data_weeks2[i][j] == 0):
            j = j + 1
        if j > year_stat - 2:
            continue

        if (year_stat - j) % 2 == 1:
            medianD = data_weeks2[i][int((year_stat - 1 + j)/2)]
        else:
            medianD = (data_weeks2[i][int((year_stat - 1 + j)/2)] + data_weeks2[i][int((year_stat - 1 + j)/2 + 1)]) / 2

        for l in range(year_stat):
            if data_week[l] != 0:
                data_week[l] = abs(data_week[l] - medianD)
        data_week.sort()
        if (year_stat - j) % 2 == 1:
            deviation = 1.483 * data_week[int((year_stat - 1 + j)/2)]
        else:
            deviation = 1.483 * (data_week[int((year_stat - 1 + j)/2)] + data_week[int((year_stat - 1 + j)/2 + 1)]) / 2

        for l in range(year_stat):
            if data_weeks2[i][l] != 0:
                if data_weeks2[i][l] < medianD - 1.5 * deviation:
                    data_weeks2[i][l] = medianD - 1.5 * deviation
                else:
                    if data_weeks2[i][l] > medianD + 1.5 * deviation:
                        data_weeks2[i][l] = medianD + 1.5 * deviation

        for iteration in range(3):
            summ_m = 0
            summ_d = 0
            count = 0
            for l in range(j, year_stat):
                summ_m = summ_m + data_weeks2[i][l]
                count = count + 1
            if count > 0:
                medianD = summ_m / count

            count = 0
            for l in range(j, year_stat):
                summ_d = summ_d + pow(data_weeks[i][j] - medianD, 2)
                count = count + 1
            deviation = 1.134 * math.sqrt(summ_d / (count - 1))

            for l in range(year_stat):
                if data_weeks2[i][l] != -1:
                    if data_weeks2[i][l] < medianD - 1.5 * deviation:
                        data_weeks2[i][l] = medianD - 1.5 * deviation
                    else:
                        if data_weeks2[i][l] > medianD + 1.5 * deviation:
                            data_weeks2[i][l] = medianD + 1.5 * deviation
        value[1][i] = medianD

#cd C:\Users\essir\Desktop\module
#venv\scripts\python.exe module_stat_v3.py --dis_id 571 --vegtype agro --year_from 2001 --year_to 2017 --tnorm_year 2009
#http://smisdev.vega.smislab.ru/api/v1/rest/mo/:571/mo/ndvi_7dc_modis_int.json?year=2007&vegtype=agro&ukey=53616c7465645f5f88cbb8825c20d0401e1170ef0fad2fbcd855a7345a10c0207e663a68c5faeb8b
#http://smisdev.vega.smislab.ru/api/v1/rest/mo/:571/mo/meteo.json?year=2007&param=temp_acc&ukey=53616c7465645f5f88cbb8825c20d0401e1170ef0fad2fbcd855a7345a10c0207e663a68c5faeb8b


#Вывод данных в csv

i = 0
N1 = 366
N2 = 52
flag = 0
temp_acc_year = [0] * 53
for i in range(53):
    temp_acc_year[i] = [0] * 3
i = 0
start = 0
finish = 52

mean()
for i in range(0, 51):
    if value[0][i] == 0 and value[0][i + 1] > 0:
        start = i + 1
    if value[0][i] > 0 and value[0][i + 1] == 0:
        finish = i + 1

if os.path.exists("out"):
    if os.path.exists("out//dis_" + str(namespace.dis_id)):
        print("")
    else:
        os.mkdir("out//dis_" + str(namespace.dis_id))
else:
    os.mkdir("out")
    os.mkdir("out//dis_" + str(namespace.dis_id))

i = 0
f = open('out//dis_' + str(namespace.dis_id) + '//dis_' + str(namespace.dis_id) + '_' + str(namespace.vegtype) + '_' + str(namespace.year_from) + '-' + str(namespace.year_to) +'_TEMP_ACC.csv', 'w', encoding='utf-8')
f.writelines("day;")
for i in range(year_stat):
    f.writelines("t_acc_" + str(namespace.year_from + i) + ";")
f.writelines("t_acc_mean;t_acc_mean_rob;t_acc_mean_wins" + "\n")
i = 0
j = 0
while i < N1:
    f.writelines(str(i+1) + ";")
    for j in range(year_stat):
        f.writelines(str(temp_acc_data_copy[i][j]) + ";")
    f.writelines(str(temp_acc_interannual_mean[i]) + ";" + str(temp_acc_interannual_rob[i]) + ";" + str(temp_acc_interannual_wins[i]) + '\n')
    i = i + 1
f.close()

i = 0
j = 0
data_weeks2_copy = copy.deepcopy(data_weeks2)
mean()
rob()
data_weeks2 = data_weeks2_copy
wins()
value1 = copy.deepcopy(value)
data_weeks2 = copy.deepcopy(data_weeks)

value = [0] * 5
for i in range(0, 5):
    value[i] = [0] * 52
flag = 1

if namespace.tnorm_year > 0:
    response = requests.get("http://smisdev.vega.smislab.ru/api/v1/rest/mo/:" + str(namespace.dis_id) + "/mo/meteo.json?year=" + str(namespace.tnorm_year) + "&param=temp_acc" + "&ukey=53616c7465645f5f88cbb8825c20d0401e1170ef0fad2fbcd855a7345a10c0207e663a68c5faeb8b")
    json_data = json.loads(response.text)
    n = 0
    for json_a in json_data["result"][0]["temp_acc"]:
        n = n + 1
        while n < int(json_a['day']):
            if n == 1:
                temp_acc_interannual_mean[n - 1] = 0
            else:
                temp_acc_interannual_mean[n - 1] = temp_acc_interannual_mean[n - 2]
        temp_acc_interannual_mean[n - 1] = float(json_a['value'])
    if n == 365:
        temp_acc_interannual_mean[n] = temp_acc_interannual_mean[n - 1]

'''f = open('out//dis_' + str(namespace.dis_id) + '//dis_' + str(namespace.dis_id) + '_file.csv', 'w', encoding='utf-8')
for i in range(0, 366):
    f.writelines(str(temp_acc_interannual_mean[i]) + "\n")'''

data_weeks2 = [0] * 53
for i in range(53):
    data_weeks2[i] = [0] * year_stat
norm_temp(temp_acc_interannual_mean)
data_weeks2_copy = copy.deepcopy(data_weeks2)
mean()
rob()
data_weeks2 = data_weeks2_copy
wins()
if namespace.tnorm_year > 0:
    f = open('out//dis_' + str(namespace.dis_id) + '//dis_' + str(namespace.dis_id) + '_' + str(namespace.vegtype) + '_' + str(namespace.year_from) + '-' + str(namespace.year_to) + '_tnorm_' + str(namespace.tnorm_year) + '_NDVI.csv', 'w', encoding='utf-8')
else:
    f = open('out//dis_' + str(namespace.dis_id) + '//dis_' + str(namespace.dis_id) + '_' + str(namespace.vegtype) + '_' + str(namespace.year_from) + '-' + str(namespace.year_to) + '_tnorm_mean_NDVI.csv', 'w', encoding='utf-8')
f.writelines("week;")
for i in range(year_stat):
    f.writelines("ndvi_" + str(namespace.year_from + i) + ";")
f.writelines("ndvi_mean;ndvi_mean_rob;ndvi_mean_wins;")
i = 0
for i in range(year_stat):
    f.writelines("ndvi_t_" + str(namespace.year_from + i) + ";")
f.writelines("ndvi_t_mean;ndvi_t_mean_rob;ndvi_t_mean_wins" + "\n")
i = 0
while i < N2:
    f.writelines(str(i+1) + ";")
    #print(str(value[0][i]) + ' ' + str(value[1][i]) + ' ' + str(value[2][i]) +'\n')
    for j in range(year_stat):
        if data_weeks_copy[i][year_stat - j - 1] > 0:
            f.writelines(str(data_weeks_copy[i][year_stat - j - 1]) + ";")
        else:
            f.writelines(";")
    j = 0
    if (i > start - 1) and (i < finish):
        f.writelines(str(value1[0][i]) + ";" + str(value1[1][i]) + ";" + str(value1[2][i]) + ";")
    else:
        f.writelines(";;;")
    for j in range(year_stat):
        if data_weeks2_copy[i][year_stat - j - 1] > 0:
            f.writelines(str(data_weeks2_copy[i][year_stat - j - 1]) + ";")
        else:
            f.writelines(";")
    if (i > start - 1) and (i < finish):
        if value[0][i] > 0:
            f.writelines(str(value[0][i]) + ";")
        else:
            f.writelines(";")
        if value[1][i] > 0:
            f.writelines(str(value[1][i]) + ";")
        else:
            f.writelines(";")
        if value[2][i] > 0:
            f.writelines(str(value[2][i]) + "\n")
        else:
            f.writelines("\n")
        #f.writelines(str(value[0][i]) + ";" + str(value[1][i]) + ";" + str(value[2][i]) + "\n")
    else:
        f.writelines(";;" + "\n")
    i = i + 1
f.close()


'''N = 52
i = 0
start = 0
value[2][49] = value[0][49]
value[1][49] = value[0][49]

while i < N:
    if value[0][i] == 0:
        del(value[0][i])
        del(value[1][i])
        del(value[2][i])
        N = N - 1
    else:
        value[0][i] = round(value[0][i], 3)
        value[1][i] = round(value[1][i], 3)
        value[2][i] = round(value[2][i], 3)
        if i == 0:
            start = 52 - N + 1
        i = i + 1

x = np.arange(start, start + N, 1.0)
plt.plot(x, value[0], 'r', x, value[1], 'b', x, value[2], 'g')

plt.xticks(np.arange(1., 53., 2.0, ), size='xx-large')
plt.yticks(np.arange(0., 1.1, 0.1), size='xx-large')
plt.title("Средний многолетний показатель", size='xx-large')
plt.xlabel("Номер недели в году", size='xx-large')
plt.ylabel("Вегетационный индекс NDVI", size='xx-large')
plt.legend(("Среднее арифметическое", "Робастный алгоритм А", "Винзоризованное среднее"))
plt.grid()
plt.show()'''
#-----------------------------------------------------------------------------------------------------------------------

#Вывод данных в json формате

'''print(temp_acc_interannual_mean[365])
print(temp_acc_interannual_wins[365])
print(temp_acc_interannual_rob[365])

x = np.arange(0, 366, 1.0)
plt.plot(x, temp_acc_interannual_mean, 'r', x, temp_acc_interannual_wins, 'b', x, temp_acc_interannual_rob, 'g')
plt.show()'''
