from cProfile import label
from turtle import color
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import sys
import os

params=sys.argv[1:]

if(len(params)==1):
    datos = np.genfromtxt('frm_'+str("{:06d}".format(int(params[0])))+'.dat', dtype=str).transpose()
    names=datos[0][1:]
    x=[float(i) for i in datos[1][1:]]
    y=[float(i) for i in datos[2][1:]]
    L=float(datos[4][1])
    H=float(datos[5][1])
    t=float(datos[7][1])

    fig, ax = plt.subplots()
    ax.set_aspect( 1 )

    my_cmap = mpl.cm.get_cmap('viridis',10).copy()
    my_cmap.set_over('red')

    plt.text(-L/2+2,H-4,"t = "+str("{:.2f}".format(t))+"s",fontsize=14,color="white")
    plt.hist2d(x,y,bins=[[i for i in range(int(-L/2),int(L/2 +1))],[i for i in range(int(H+1))]],cmap=my_cmap,vmin=0,vmax=9.999)
    cbar=plt.colorbar(label="$\\rho$ [#peatones/m$^2$]",ticks=[0,1,2,3,4,5,6,7,8,9,10],extend='max',location='right')
    plt.savefig("dns_"+str("{:06d}".format(int(params[0])))+".png",dpi=150,bbox_inches='tight')
    plt.close()

elif len(params)==2:
    for i in range(int(params[0]),int(params[1])+1):
        datos = np.genfromtxt('frm_'+str("{:06d}".format(i))+'.dat', dtype=str).transpose()
        names=datos[0][1:]
        x=[float(i) for i in datos[1][1:]]
        y=[float(i) for i in datos[2][1:]]
        L=float(datos[4][1])
        H=float(datos[5][1])
        t=float(datos[7][1])

        fig, ax = plt.subplots()
        ax.set_aspect( 1 )
    
        my_cmap = mpl.cm.get_cmap('viridis',10).copy()
        my_cmap.set_over('red')

        plt.text(-L/2+2,H-4,"t = "+str("{:.2f}".format(t))+"s",fontsize=14,color="white")
        plt.hist2d(x,y,bins=[[i for i in range(int(-L/2),int(L/2 +1))],[i for i in range(int(H+1))]],cmap=my_cmap,vmin=0,vmax=9.999)
        cbar=plt.colorbar(label="$\\rho$ [#peatones/m$^2$]",ticks=[0,1,2,3,4,5,6,7,8,9,10],extend='max',location='right')
        plt.savefig("dns_"+str("{:06d}".format(i))+".png",dpi=150,bbox_inches='tight')
        plt.close()

else:
    for i in range(len([name for name in os.listdir('.') if os.path.isfile(name)])-1):
        datos = np.genfromtxt('frm_'+str("{:06d}".format(i))+'.dat', dtype=str).transpose()
        names=datos[0][1:]
        x=[float(i) for i in datos[1][1:]]
        y=[float(i) for i in datos[2][1:]]
        L=float(datos[4][1])
        H=float(datos[5][1])
        t=float(datos[7][1])

        fig, ax = plt.subplots()
        ax.set_aspect( 1 )
    
        my_cmap = mpl.cm.get_cmap('viridis',10).copy()
        my_cmap.set_over('red')

        plt.text(-L/2+2,H-4,"t = "+str("{:.2f}".format(t))+"s",fontsize=14,color="white")
        plt.hist2d(x,y,bins=[[i for i in range(int(-L/2),int(L/2 +1))],[i for i in range(int(H+1))]],cmap=my_cmap,vmin=0,vmax=9.999)
        cbar=plt.colorbar(label="$\\rho$ [#peatones/m$^2$]",ticks=[0,1,2,3,4,5,6,7,8,9,10],extend='max',location='right')
        plt.savefig("dns_"+str("{:06d}".format(i))+".png",dpi=150,bbox_inches='tight')
        plt.close()