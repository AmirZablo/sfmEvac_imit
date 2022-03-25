import numpy as np
import matplotlib.pyplot as plt
import sys
import os

params=sys.argv[1:]

if(len(params)==1):
    datos = np.genfromtxt('frm_'+str("{:06d}".format(int(params[0])))+'.dat', dtype=str).transpose()
    names=datos[0][1:]
    pos=[(float(i),float(j)) for i,j in zip(datos[1][1:],datos[2][1:])]
    att=datos[3][1:]
    L=float(datos[4][1])
    H=float(datos[5][1])
    D=float(datos[6][1])
    r=float(datos[7][1])
    t=float(datos[8][1])
    verts=((-D/2,0),(-L/2,0),(-L/2,H),(L/2,H),(L/2,0),(D/2,0)) #(x,y)

    fig, ax = plt.subplots()
    ax.set_aspect( 1 )
    #draw the room
    plt.plot([verts[0][0],verts[1][0],verts[2][0],verts[3][0],verts[4][0],verts[5][0]],[verts[0][1],verts[1][1],verts[2][1],verts[3][1],verts[4][1],verts[5][1]],color='black',linewidth=1)
    #draw pedestrians

    for p in pos:
        if att[pos.index(p)]=='ego':
            ax.add_artist(plt.Circle((p[0],p[1]),r,color='red'))
        else:
            ax.add_artist(plt.Circle((p[0],p[1]),r,color='blue')) 

    plt.text(-L/2+2,H-4,"t = "+str("{:.2f}".format(t))+"s",fontsize=14)
    #red_patch = mpatches.Patch(color='red', label='Egoista')
    #blue_patch = mpatches.Patch(color='blue', label='Cooperativo')
    #plt.legend(handles=[red_patch,blue_patch],loc='upper right')
    plt.savefig("frm_"+str("{:06d}".format(int(params[0])))+".png",dpi=150,bbox_inches='tight')
    plt.close()

elif len(params)==2:
    for i in range(int(params[0]),int(params[1])+1):
        datos = np.genfromtxt('frm_'+str("{:06d}".format(i))+'.dat', dtype=str).transpose()
        names=datos[0][1:]
        pos=[(float(i),float(j)) for i,j in zip(datos[1][1:],datos[2][1:])]
        att=datos[3][1:]
        L=float(datos[4][1])
        H=float(datos[5][1])
        D=float(datos[6][1])
        r=float(datos[7][1])
        t=float(datos[8][1])
        verts=((-D/2,0),(-L/2,0),(-L/2,H),(L/2,H),(L/2,0),(D/2,0)) #(x,y)

        fig, ax = plt.subplots()
        ax.set_aspect( 1 )
        #draw the room
        plt.plot([verts[0][0],verts[1][0],verts[2][0],verts[3][0],verts[4][0],verts[5][0]],[verts[0][1],verts[1][1],verts[2][1],verts[3][1],verts[4][1],verts[5][1]],color='black',linewidth=1)
        #draw pedestrians

        for p in pos:
            if att[pos.index(p)]=='ego':
                ax.add_artist(plt.Circle((p[0],p[1]),r,color='red'))
            else:
                ax.add_artist(plt.Circle((p[0],p[1]),r,color='blue')) 

        plt.text(-L/2+2,H-4,"t = "+str("{:.2f}".format(t))+"s",fontsize=14)
        #red_patch = mpatches.Patch(color='red', label='Egoista')
        #blue_patch = mpatches.Patch(color='blue', label='Cooperativo')
        #plt.legend(handles=[red_patch,blue_patch],loc='upper right')
        plt.savefig("frm_"+str("{:06d}".format(i))+".png",dpi=150,bbox_inches='tight')
        plt.close()

else:
    for i in range(len([name for name in os.listdir('.') if os.path.isfile(name)])-1):
        datos = np.genfromtxt('frm_'+str("{:06d}".format(i))+'.dat', dtype=str).transpose()
        names=datos[0][1:]
        pos=[(float(i),float(j)) for i,j in zip(datos[1][1:],datos[2][1:])]
        att=datos[3][1:]
        L=float(datos[4][1])
        H=float(datos[5][1])
        D=float(datos[6][1])
        r=float(datos[7][1])
        t=float(datos[8][1])
        verts=((-D/2,0),(-L/2,0),(-L/2,H),(L/2,H),(L/2,0),(D/2,0)) #(x,y)

        fig, ax = plt.subplots()
        ax.set_aspect( 1 )
        #draw the room
        plt.plot([verts[0][0],verts[1][0],verts[2][0],verts[3][0],verts[4][0],verts[5][0]],[verts[0][1],verts[1][1],verts[2][1],verts[3][1],verts[4][1],verts[5][1]],color='black',linewidth=1)
        #draw pedestrians

        for p in pos:
            if att[pos.index(p)]=='ego':
                ax.add_artist(plt.Circle((p[0],p[1]),r,color='red'))
            else:
                ax.add_artist(plt.Circle((p[0],p[1]),r,color='blue')) 

        plt.text(-L/2+2,H-4,"t = "+str("{:.2f}".format(t))+"s",fontsize=14)
        #red_patch = mpatches.Patch(color='red', label='Egoista')
        #blue_patch = mpatches.Patch(color='blue', label='Cooperativo')
        #plt.legend(handles=[red_patch,blue_patch],loc='upper right')
        plt.savefig("frm_"+str("{:06d}".format(i))+".png",dpi=150,bbox_inches='tight')
        plt.close()