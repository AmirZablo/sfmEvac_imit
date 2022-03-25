primerIndice=1
ultimoIndice=2
indice=primerIndice # (Incrementa dentro del loop)

frameSaveRate= 999999
seed= 10
dt= 0.001
tmax= 500.0
NCoop= 10
NEgo= 250
tau_coop= 0.5
tau_ego= 0.5
A_coop= 10000.0
A_ego= 2000.0
B_coop= 0.08
B_ego= 0.08
kappa= 240000.0
desiredVel_coop= 3.5
desiredVel_ego= 3.5
r= 0.25
m= 70.0
L= 30
H= 30
D=1.0
phi= 3.1415926
cteFOV= 0.5
vmax= 8.0
r_dens=1.5*D

while indice <= ultimoIndice:
    with open('params.'+str(indice), 'w') as f:
        f.write('frameSaveRate: '+str(frameSaveRate)+'\n')
        f.write('seed: '+str(seed)+'\n')
        f.write('dt: '+str(dt)+'\n')
        f.write('tmax: '+str(tmax)+'\n')
        f.write('NCoop: '+str(NCoop)+'\n')
        f.write('NEgo: '+str(NEgo)+'\n')
        f.write('tau_coop: '+str(tau_coop)+'\n')
        f.write('tau_ego: '+str(tau_ego)+'\n')
        f.write('A_coop: '+str(A_coop)+'\n')
        f.write('A_ego: '+str(A_ego)+'\n')
        f.write('B_coop: '+str(B_coop)+'\n')
        f.write('B_ego: '+str(B_ego)+'\n')
        f.write('kappa: '+str(kappa)+'\n')
        f.write('desiredVel_coop: '+str(desiredVel_coop)+'\n')
        f.write('desiredVel_ego: '+str(desiredVel_ego)+'\n')
        f.write('r: '+str(r)+'\n')
        f.write('m: '+str(m)+'\n')
        f.write('L: '+str(L)+'\n')
        f.write('H: '+str(H)+'\n')
        f.write('D: '+str(D)+'\n')
        f.write('phi: '+str(phi)+'\n')
        f.write('cteFOV: '+str(cteFOV)+'\n')
        f.write('vmax: '+str(vmax)+'\n')
        f.write('r_dens: '+str(r_dens))

    seed+=10
    indice+=1 
