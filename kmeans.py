import sys
import math
##return euclidean distance
def d(a,b):
    sum=0
    for i in range(len(a)):
        sum=sum+(float(a[i])-float(b[i]))**2
    return math.sqrt(sum) 

##checks if Eclidean Distance between the updated centroid to the previous one is less then epsilon for all centroids
def epsilon_check(a,b):
    e=0.001
    for i in range(len(a)):
        tmp=d(a[i],b[i])
        if tmp>=e:
            return False
    return True 

##return the index of the closest centroid to x       
def min_centrum(u,x):
    min=d(u[0],x)
    min_index=0
    for i in range(1,len(u)):
        tmp=d(u[i],x)
        if tmp<=min:
            min=tmp
            min_index=i
    return min_index  

##return the mean vector of a list of vectors  
def mean_vector(a):
    sums=[0.0] * len(a[0])
    for i in range(len(a[0])):
        for j in range(len(a)):
            sums[i]=sums[i]+float(a[j][i])
    for i in range(len(sums)):
        sums[i]=sums[i]/len(a)    
    return sums       


args=sys.argv[1:]
if len(args)==2:
    iter=200
    check_txt=args[1][-4:]
    if check_txt!=".txt":
        sys.exit("NA")
    
    else:
        file1=open(args[1],'r')
    
else:
    iter=args[1]
    iter=int(iter)
    if iter<=1 or iter>=1000:
            sys.exit("Invalid maximum clusters!")
    check_txt=args[2][-4:]
    if check_txt!=".txt":
        sys.exit("NA")
    else:
        file1=open(args[2],'r')
    
    

first=file1.readline()

##count the num of vectors in the input
n=0
while first!="":
    n=n+1
    first=file1.readline()
    if(n>100000):
        break

k=int(args[0])
if k<=1 or k>n:
    sys.exit("Invalid number of clusters!")

##putting all vectors in a list
x=[ [] for _ in range(n) ]
file1.seek(0)
tmp=file1.readline()
i=0
while tmp != "":
    x[i]=tmp.split()
    a=x[i][0].split(',')
    x[i]=a
    i=i+1
    tmp=file1.readline()
file1.seek(0)

## initialize centroids as first k datapoints
u_old=[ [] for _ in range(k) ]
u=[ [] for _ in range(k) ]
for i in range(k):
    u_old[i]=x[i]
    u[i]=x[i]

counter=0
while counter<iter:
    counter=counter+1
    clusters=[ [] for _ in range(k) ]
    for i in range(n):
        min=min_centrum(u,x[i])
        clusters[min].append(x[i])
    tmp=u.copy()    
    for i in range(k):
        u[i]=mean_vector(clusters[i])
       
    if(epsilon_check(u,u_old)):
        break
    u_old=tmp

for i in range(0,k):
    for j in range(0,len(u[0]))  :
        u[i][j]="{0:.4f}".format(u[i][j])
        if j==len(u[0])-1:
            print(u[i][j])
        else:
            print(u[i][j]+ ",",end="")    
