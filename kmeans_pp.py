import pandas as pd
import numpy as np
import sys
import math
import mykmeanssp
def d(a,b):
    sum=0
    for i in range(len(a)):
        sum=sum+(float(a[i])-float(b[i]))**2
    return math.sqrt(sum) 
args=sys.argv[1:]
k=int(args[0])
if len(args)==5:
    iter=int(args[1])
    check_txt1=args[3][-4:]
    check_txt2=args[4][-4:]

else:
    iter=300
    check_txt1=args[2][-4:]
    check_txt2=args[3][-4:]
if check_txt1!=".txt" and check_txt1!=".csv":
    sys.exit("NA")
if check_txt2!=".txt" and check_txt2!=".csv":
    sys.exit("NA")    
eps=float(args[len(args)-3])
input1=pd.read_csv(args[len(args)-2],header=None)
input2=pd.read_csv(args[len(args)-1],header=None)
input_combine=pd.merge(input1,input2,on=0)
input_sorted=input_combine.sort_values(by=0,ascending=True)
n=input_sorted.shape[0]
if k<=1 or k>n:
    sys.exit("Invalid number of clusters!")
if eps<0:
    sys.exit("Invalid epsilon!")
if iter<=1 or iter>=1000:
        sys.exit("Invalid maximum clusters!")    
num_columns=input_sorted.shape[1] -1
data_points=np.zeros((n,num_columns))
column_name=[]
for i in range(0,num_columns +1):
    column_name.append(i)
input_pd=pd.DataFrame(input_sorted)
input_pd.columns=column_name
input_pd.set_index(0,inplace=True)
for i in range(0,n):
    for j in range(0,num_columns):
        data_points[i][j]=input_pd[j+1][i]
np.random.seed(1234)
centroid=np.zeros((k,num_columns))

propabilities=np.zeros((n))
first_index=np.random.randint(0,n)
for i in range(0,num_columns):
    centroid[0][i]=data_points[first_index][i]



num_centroid=1
index_centroid=[]
index_centroid.append(first_index)
for i in range(0,k-1):
    for j in range(0,n):
        min_d=math.inf
        if j in index_centroid:
            continue
        for p in range(0,num_centroid):
            dis=d(data_points[j],centroid[p])
            if dis<min_d:
                min_d=dis
        propabilities[j]=min_d
    sum_d=np.sum(propabilities)
    propabilities=propabilities/sum_d            
 
    new_centroid=np.random.choice(n,p=propabilities)
    index_centroid.append(new_centroid)
    propabilities[new_centroid]=0    
    for j in range(0,num_columns):
        centroid[num_centroid][j]=data_points[new_centroid][j]
    num_centroid=num_centroid+1

centroid_list=centroid.tolist()
data_points_list=data_points.tolist()
a=mykmeanssp.fit(k,iter,n,num_columns,eps,data_points_list,centroid_list)



print(*index_centroid, sep=",")
for i in range(k):
    print(",".join([f"{a[i][j]:.4f}"for j in range(num_columns)]))