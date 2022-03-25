import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import sys
import os

params=sys.argv[1:]

if(len(params)==1):
    datos = np.genfromtxt('frm_'+str("{:06d}".format(int(params[0])))+'.dat', dtype=str).transpose()
    names=datos[0][1:]
    pos=[(float(i),float(j)) for i,j in zip(datos[1][1:],datos[2][1:])]
    x=[float(i) for i in datos[1][1:]]
    y=[float(i) for i in datos[2][1:]]
    att=datos[3][1:]
    L=float(datos[4][1])
    H=float(datos[5][1])
    D=float(datos[6][1])
    r=float(datos[7][1])
    t=float(datos[8][1])
    verts=((-D/2,0),(-L/2,0),(-L/2,H),(L/2,H),(L/2,0),(D/2,0)) #(x,y)

    fig, ax = plt.subplots(1,2)
    ax[0].set_aspect( 1 )
    ax[1].set_aspect( 1 )
    #draw the room
    ax[0].plot([verts[0][0],verts[1][0],verts[2][0],verts[3][0],verts[4][0],verts[5][0]],[verts[0][1],verts[1][1],verts[2][1],verts[3][1],verts[4][1],verts[5][1]],color='black',linewidth=1)
    #draw pedestrians

    ax[0].title.set_text('Simulación')
    ax[1].title.set_text('Densidad de peatones')

    for p in pos:
        if att[pos.index(p)]=='ego':
            ax[0].add_artist(plt.Circle((p[0],p[1]),r,color='red'))
        else:
            ax[0].add_artist(plt.Circle((p[0],p[1]),r,color='blue')) 

    ax[0].text(-L/2+2,L-4,"t = "+str("{:.2f}".format(t))+"s",fontsize=14)

    my_cmap = mpl.cm.get_cmap('viridis',10).copy()
    my_cmap.set_over('red')

    ax[1].text(-L/2+2,L-4,"t = "+str("{:.2f}".format(t))+"s",fontsize=14,color="white")
    hist=ax[1].hist2d(x,y,bins=[[i for i in range(int(-L/2),int(L/2 +1))],[i for i in range(int(H+1))]],cmap=my_cmap,vmin=0,vmax=9.999)
    #fig.colorbar(hist[3],ax=ax[1],label="$\\rho$ [#peatones/m$^2$]",ticks=[0,1,2,3,4,5,6,7,8,9,10],extend='max')
    # Make a new Axes instance
    cbar_ax = plt.gcf().add_axes([0.91, 0.261, 0.02, 0.47])
    plt.colorbar(hist[3],cax=cbar_ax,label="$\\rho$ [#peatones/m$^2$]",ticks=[0,1,2,3,4,5,6,7,8,9,10],extend='max')


    plt.savefig("frm+dns_"+str("{:06d}".format(int(params[0])))+".png",dpi=150,bbox_inches='tight')
    plt.close()

elif len(params)==2:
    for i in range(int(params[0]),int(params[1])+1):
        datos = np.genfromtxt('frm_'+str("{:06d}".format(i))+'.dat', dtype=str).transpose()
        names=datos[0][1:]
        pos=[(float(i),float(j)) for i,j in zip(datos[1][1:],datos[2][1:])]
        x=[float(i) for i in datos[1][1:]]
        y=[float(i) for i in datos[2][1:]]
        att=datos[3][1:]
        L=float(datos[4][1])
        H=float(datos[5][1])
        D=float(datos[6][1])
        r=float(datos[7][1])
        t=float(datos[8][1])
        verts=((-D/2,0),(-L/2,0),(-L/2,H),(L/2,H),(L/2,0),(D/2,0)) #(x,y)

        fig, ax = plt.subplots(1,2)
        ax[0].set_aspect( 1 )
        ax[1].set_aspect( 1 )
        #draw the room
        ax[0].plot([verts[0][0],verts[1][0],verts[2][0],verts[3][0],verts[4][0],verts[5][0]],[verts[0][1],verts[1][1],verts[2][1],verts[3][1],verts[4][1],verts[5][1]],color='black',linewidth=1)
        #draw pedestrians

        ax[0].title.set_text('Simulación')
        ax[1].title.set_text('Densidad de peatones')

        for p in pos:
            if att[pos.index(p)]=='ego':
                ax[0].add_artist(plt.Circle((p[0],p[1]),r,color='red'))
            else:
                ax[0].add_artist(plt.Circle((p[0],p[1]),r,color='blue')) 

        ax[0].text(-L/2+2,L-4,"t = "+str("{:.2f}".format(t))+"s",fontsize=14)

        my_cmap = mpl.cm.get_cmap('viridis',10).copy()
        my_cmap.set_over('red')

        ax[1].text(-L/2+2,L-4,"t = "+str("{:.2f}".format(t))+"s",fontsize=14,color="white")
        hist=ax[1].hist2d(x,y,bins=[[i for i in range(int(-L/2),int(L/2 +1))],[i for i in range(int(H+1))]],cmap=my_cmap,vmin=0,vmax=9.999)
        #fig.colorbar(hist[3],ax=ax[1],label="$\\rho$ [#peatones/m$^2$]",ticks=[0,1,2,3,4,5,6,7,8,9,10],extend='max')
        # Make a new Axes instance
        cbar_ax = plt.gcf().add_axes([0.91, 0.261, 0.02, 0.47])
        plt.colorbar(hist[3],cax=cbar_ax,label="$\\rho$ [#peatones/m$^2$]",ticks=[0,1,2,3,4,5,6,7,8,9,10],extend='max')
        
        plt.savefig("frm_"+str("{:06d}".format(i))+".png",dpi=150,bbox_inches='tight')
        plt.close()

else:
    for i in range(len([name for name in os.listdir('.') if os.path.isfile(name)])-1):
        datos = np.genfromtxt('frm_'+str("{:06d}".format(i))+'.dat', dtype=str).transpose()
        names=datos[0][1:]
        pos=[(float(i),float(j)) for i,j in zip(datos[1][1:],datos[2][1:])]
        x=[float(i) for i in datos[1][1:]]
        y=[float(i) for i in datos[2][1:]]
        att=datos[3][1:]
        L=float(datos[4][1])
        H=float(datos[5][1])
        D=float(datos[6][1])
        r=float(datos[7][1])
        t=float(datos[8][1])
        verts=((-D/2,0),(-L/2,0),(-L/2,H),(L/2,H),(L/2,0),(D/2,0)) #(x,y)

        fig, ax = plt.subplots(1,2)
        ax[0].set_aspect( 1 )
        ax[1].set_aspect( 1 )
        #draw the room
        ax[0].plot([verts[0][0],verts[1][0],verts[2][0],verts[3][0],verts[4][0],verts[5][0]],[verts[0][1],verts[1][1],verts[2][1],verts[3][1],verts[4][1],verts[5][1]],color='black',linewidth=1)
        #draw pedestrians

        ax[0].title.set_text('Simulación')
        ax[1].title.set_text('Densidad de peatones')

        for p in pos:
            if att[pos.index(p)]=='ego':
                ax[0].add_artist(plt.Circle((p[0],p[1]),r,color='red'))
            else:
                ax[0].add_artist(plt.Circle((p[0],p[1]),r,color='blue')) 

        ax[0].text(-L/2+2,L-4,"t = "+str("{:.2f}".format(t))+"s",fontsize=14)

        my_cmap = mpl.cm.get_cmap('viridis',10).copy()
        my_cmap.set_over('red')

        ax[1].text(-L/2+2,L-4,"t = "+str("{:.2f}".format(t))+"s",fontsize=14,color="white")
        hist=ax[1].hist2d(x,y,bins=[[i for i in range(int(-L/2),int(L/2 +1))],[i for i in range(int(H+1))]],cmap=my_cmap,vmin=0,vmax=9.999)
        #fig.colorbar(hist[3],ax=ax[1],label="$\\rho$ [#peatones/m$^2$]",ticks=[0,1,2,3,4,5,6,7,8,9,10],extend='max')
        # Make a new Axes instance
        cbar_ax = plt.gcf().add_axes([0.91, 0.261, 0.02, 0.47])
        plt.colorbar(hist[3],cax=cbar_ax,label="$\\rho$ [#peatones/m$^2$]",ticks=[0,1,2,3,4,5,6,7,8,9,10],extend='max')
        
        plt.savefig("frm_"+str("{:06d}".format(i))+".png",dpi=150,bbox_inches='tight')
        plt.close()