//Code by Amir Nicolás Zablotsky (amirzablotsky@gmail.com)
//Version 1, 18/2/2022
//Units system: MKS

#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

#define PI 3.1415926



//Parameters
int iter=0;
int frameSaveRate; //Save every nth frame
int seed; //Seed for random number generator
float dt= roundf(0.001 * 1000) / 1000; //Time step
float t=0.0;
float tmax; //Max time
int NCoop; //Number of cooperative pedestrians
int NEgo; //Number of egoist pedestrians
float tau_coop; //Relaxation time (cooperatives)
float tau_ego; //Relaxation time (egoists)
float A_coop; //Intensity of social force [N] (cooperatives)
float A_ego; //Intensity of social force [N] (egoists)
float B_coop; //Characteristic distance of social force (cooperatives)
float B_ego; //Characteristic distance of social force (egoists)
float kappa; //Friction constant
float desiredVel_coop; //Desired velocity (cooperatives)
float desiredVel_ego; //Desired velocity (egoists)
float r; //Radius of the pedestrians (disks)
float m; //Mass of the pedestrians
int L; //Room's width (with room of 50x50, you can fit at most 1000 pedestrians in the room)
int H; //Room's height
float D; //Exit's width (at the center of the bottom wall)
float phi; //2phi is the effective angle of sight of the pedestrians
float cteFOV; //Factor for the interaction forces when not in the FOV
float vmax; //Maximum velocity
float r_dens; //Density to register
float r_imit; //Imitation radius
string folname="";

class Pedestrian;
vector<Pedestrian> peds;

struct pto2D
{
    float x;
    float y;
};

float Randomfloat(float a, float b) //Random number generator between a and b
{
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

pto2D Rotate(pto2D vec, float theta) //Rotate a vector by theta
{
    pto2D rot;
    rot.x=cos(theta)*vec.x-sin(theta)*vec.y;
    rot.y=sin(theta)*vec.x+cos(theta)*vec.y;
    return(rot);
}

float distance(float x1, float y1, float x2, float y2) //Distance between (x1,y1) and (x2,y2)
{
    return sqrt(pow((x1-x2),2)+pow((y1-y2),2));
}

class Pedestrian
{
    public:
    int name;
    string att;
    float x;
    float y;
    float xPrev;
    float yPrev;
    float vx;
    float vy;
    float tau;
    float desiredVel;
    float A;
    float B;
    float dirx;
    float diry;
    float prev_ax;
    float new_ax;
    float prev_ay;
    float new_ay;
    bool imit;

    //Pedestrian class
    Pedestrian(int name_, string att_, float x_, float y_, float vx_, float vy_, float tau_, float desiredVel_, float B_, float A_) 
    {
        name=name_;
        att=att_;
        x=x_;
        y=y_;
        vx=vx_;
        vy=vy;
        tau=tau_;
        desiredVel=desiredVel_;
        A=A_;
        B=B_;
        dirx=1.0;
        diry=0.0;
        prev_ax=0.0;
        new_ax=0.0;
        prev_ay=0.0;
        new_ay=0.0;
        xPrev=0;
        yPrev=0;
        imit=false;
    }

    void update_e() //Update the direction of the pedestrian
    {
        if(x<(-D/2 + r)) //x coordinate is to the left of the door
        {
            dirx=((-D/2 + r)-x)/distance(-(D/2)+r,0.0,x,y);
            diry=(-y)/distance(-(D/2)+r,0.0,x,y);
        }    
        else if(x>(D/2 - r)) //x coordinate is to the right of the door
        {
            dirx=((D/2 - r)-x)/distance((D/2)-r,0,x,y);
            diry=(-y)/distance((D/2)-r,0.0,x,y);
        }   
        else  //x coordinate is inside the door
        {
            dirx=0.0;
            diry=-1.0;
        }     
    }
};

void saveFrame(bool error) //Save the current state of the simulation
{
    ofstream out;
    if(!error)
    {
        switch (to_string((int)(iter/frameSaveRate)).length())
        {
            case 1:
                out.open("sim_"+folname+"/frames/frm_00000"+to_string((int)(iter/frameSaveRate))+".dat");
                break;
            case 2:
                out.open("sim_"+folname+"/frames/frm_0000"+to_string((int)(iter/frameSaveRate))+".dat");
                break;
            case 3:
                out.open("sim_"+folname+"/frames/frm_000"+to_string((int)(iter/frameSaveRate))+".dat");
                break;
            case 4:
                out.open("sim_"+folname+"/frames/frm_00"+to_string((int)(iter/frameSaveRate))+".dat");
                break;
            case 5:
                out.open("sim_"+folname+"/frames/frm_0"+to_string((int)(iter/frameSaveRate))+".dat");
                break;
            default:
                out.open("sim_"+folname+"/frames/frm_"+to_string((int)(iter/frameSaveRate))+".dat");
                break;
        }
        out<<"name x y attitude L H D r t imitando\n";
        for(int p=0;p<peds.size();p++)
        {
            out<<to_string(peds[p].name)+" "+to_string(peds[p].x)+" "+to_string(peds[p].y)+" "+peds[p].att+" "+to_string(L)+" "+to_string(H)+" "+to_string(D)+" "+to_string(r)+" "+to_string(t)+" "+to_string(peds[p].imit)+'\n';
        }
    }
    else
    {
        out.open("sim_"+to_string(seed)+"/frames/error.dat");
        out<<"Error, pedestrian went quantum."; //Peaton atraviesa pared
    }
    
    out.close();
}

pto2D F0(float vx, float vy, float desiredVel, float dirx, float diry, float tau, float mass) //Desire force
{
    pto2D F;
    F.x=(mass/tau)*(desiredVel*dirx-vx);
    F.y=(mass/tau)*(desiredVel*diry-vy);
    return F;
}

pto2D F_inter(float x1, float y1, float x2, float y2, float r1, float r2, float B, float A) //Social force
{
    pto2D F;
    F.x=A*exp(((r1+r2)-distance(x1,y1,x2,y2))/B)*((x1-x2)/distance(x1,y1,x2,y2));
    F.y=A*exp(((r1+r2)-distance(x1,y1,x2,y2))/B)*((y1-y2)/distance(x1,y1,x2,y2));
    return F;
}

pto2D F_roz(float dist, Pedestrian p1, Pedestrian p2, float r1, float r2) //Friction force
{
    pto2D norm;
    norm.x=((p1.x-p2.x)/dist);
    norm.y=((p1.y-p2.y)/dist);
    pto2D tang=Rotate(norm,PI/2);
    pto2D F;
    F.x=kappa*((r1+r2)-dist)*((p2.vx-p1.vx)*tang.x+(p2.vy-p1.vy)*tang.y)*tang.x;
    F.y=kappa*((r1+r2)-dist)*((p2.vx-p1.vx)*tang.x+(p2.vy-p1.vy)*tang.y)*tang.y;
    return(F);
}

bool isInFOV(float dirx, float diry,float fx,float fy) //Check if the second pedestrian is in the FOV
{
    float prod=dirx*fx+diry*fy;
    return(prod>=sqrt(pow(fx,2)+pow(fy,2))*cos(phi));
}

void loadParams(string ifile) //Load the parameters from the params file
{
    ifstream params(ifile.c_str());
    string ident;
    if ( !params.is_open())
    {
        cout<<"Could not open file\n";
    }
    else 
    {
        while (!params.eof()) 
        {
            params>>ident;
            if(ident=="frameSaveRate:")
            {
                params>>frameSaveRate;
            }
            if(ident=="seed:")
            {
                params>>seed;
            }
            if(ident=="dt:")
            {
                params>>dt;
                dt=roundf(dt * 1000) / 1000;
            }
            if(ident=="tmax:")
            {
                params>>tmax;
            }
            if(ident=="NCoop:")
            {
                params>>NCoop;
            }
            if(ident=="NEgo:")
            {
                params>>NEgo;
            }
            if(ident=="tau_coop:")
            {
                params>>tau_coop;
            }
            if(ident=="tau_ego:")
            {
                params>>tau_ego;
            }
            if(ident=="A_coop:")
            {
                params>>A_coop;
            }
            if(ident=="A_ego:")
            {
                params>>A_ego;
            }
            if(ident=="B_coop:")
            {
                params>>B_coop;
            }
            if(ident=="B_ego:")
            {
                params>>B_ego;
            }
            if(ident=="kappa:")
            {
                params>>kappa;
            }
            if(ident=="desiredVel_coop:")
            {
                params>>desiredVel_coop;
            }
            if(ident=="desiredVel_ego:")
            {
                params>>desiredVel_ego;
            }
            if(ident=="r:")
            {
                params>>r;
            }
            if(ident=="m:")
            {
                params>>m;
            }
            if(ident=="L:")
            {
                params>>L;
            }
            if(ident=="H:")
            {
                params>>H;
            }
            if(ident=="D:")
            {
                params>>D;
            }
            if(ident=="phi:")
            {
                params>>phi;
            }
            if(ident=="cteFOV:")
            {
                params>>cteFOV;
            }
            if(ident=="vmax:")
            {
                params>>vmax;
            }
            if(ident=="r_dens:")
            {
                params>>r_dens;
            }
            if(ident=="r_imit:")
            {
                params>>r_imit;
            }

        }
        params.close();
    }
}

int main(int argc, char *argv[]) //Main function
{
    if ( argc != 2 ) //If the user doesn't include the params file
    {
        cout<<"usage: "<< argv[0] <<" <filename>\n";
        goto error;
    }
    else
    {
        string params(argv[1]);
        loadParams(params); //Load the parameters
        bool startrec=false;
       
        for(int i=0;i<params.size();i++)
        {
            if(startrec)
            {
                folname+=params[i];
            }
            if(params[i]=='.')
            {
               startrec=true;
            }
        }
        srand(seed);

        stringstream dir; //Create the simulation directory
        dir<<"mkdir sim_"<<folname;
        system(dir.str().c_str());

        stringstream subdir;
        subdir<<"mkdir sim_"<<folname<<"/frames"; //Create the frames subdirectory
        system(subdir.str().c_str());

        ofstream salida;
        ofstream dens_exit;
        salida.open("sim_"+folname+"/salida.txt");
        dens_exit.open("sim_"+folname+"/dens_exit.txt");
        salida<<"# PARAMETROS DE LA SIMULACIÓN\n";
        salida<<"\n";
        salida<<"# frameSaveRate: "<<frameSaveRate<<"\n";
        salida<<"# seed: "<<seed<<"\n";
        salida<<"# dt: "<<dt<<"\n";
        salida<<"# tmax: "<<tmax<<"\n";
        salida<<"# NCoop: "<<NCoop<<"\n";
        salida<<"# NEgo: "<<NEgo<<"\n";
        salida<<"# tau_coop: "<<tau_coop<<"\n";
        salida<<"# tau_ego: "<<tau_ego<<"\n";
        salida<<"# A_coop: "<<A_coop<<"\n";
        salida<<"# A_ego: "<<A_ego<<"\n";
        salida<<"# B_coop: "<<B_coop<<"\n";
        salida<<"# B_ego: "<<B_ego<<"\n";
        salida<<"# kappa: "<<kappa<<"\n";
        salida<<"# desiredVel_coop: "<<desiredVel_coop<<"\n";
        salida<<"# desiredVel_ego: "<<desiredVel_ego<<"\n";
        salida<<"# r: "<<r<<"\n";
        salida<<"# m: "<<m<<"\n";
        salida<<"# L: "<<L<<"\n";
        salida<<"# H: "<<H<<"\n";
        salida<<"# D: "<<D<<"\n";
        salida<<"# phi: "<<phi<<"\n";
        salida<<"# cteFOV: "<<cteFOV<<"\n";
        salida<<"# vmax: "<<vmax<<"\n";
        salida<<"# r_dens: "<<r_dens<<"\n";
        salida<<"# r_imit: "<<r_imit<<"\n";
        salida<<"\n";
        salida<<"# Inicializando peatones...\n";
        for(int i=0; i<NEgo+NCoop; i++) //Initialize the pedestrians
        {
            bool trying=true;
            float x_cand=0.0;
            float y_cand=0.0;
            while(trying) //Generate random positions for the i-th pedestrian until the condtions are met
            {
                x_cand=Randomfloat(-L/2 +r,L/2 -r);
                y_cand=Randomfloat(r,H-r);
                trying=false;
                for(Pedestrian &p: peds)
                {
                    if((distance(x_cand,y_cand,p.x,p.y)<(4*r))) //  || (distance(x_cand,y_cand,-L/2,y_cand)<(3*r)) || (distance(x_cand,y_cand,L/2,y_cand)<(3*r)) || (distance(x_cand,y_cand,x_cand,H)<(3*r)) || (distance(x_cand,y_cand,x_cand,0)<(3*r))
                    {
                        trying=true;
                        break;
                    }
                }
            }
        
            if(i<NEgo)  //Add the pedestrian to the group
            {
                Pedestrian p(i,"ego",x_cand,y_cand,0.0,0.0,tau_ego,desiredVel_ego,B_ego,A_ego);
                peds.push_back(p);
            }
            else
            {
                Pedestrian p(i,"coop",x_cand,y_cand,0.0,0.0,tau_coop,desiredVel_coop,B_coop,A_coop);
                peds.push_back(p);
            }
        }

        salida<<"# Inicialización finalizada, "<<NEgo<<" peatones egoistas y "<<NCoop<<" peatones cooperativos.\n";
        int pedsLeft=NEgo+NCoop;
        int egoEv=0;
        int coopEv=0;
        saveFrame(false);

        salida<<"# Comienza la simulación...\n";
        salida<<"\n";
        salida<<"# t de salida | actitud | egoistas evacuados | cooperativos evacuados | total evacuados\n";
        dens_exit<<"# t de salida | densidad de peatones | r area\n";

        while(pedsLeft>0.2*(NEgo+NCoop)) //Main loop, end simulation when 80% left the room 
        {
            int p_in_r=0;
            for(int p=0; p<peds.size();) //Update the position of each pedestrian
            {
                peds[p].update_e();
                peds[p].xPrev=peds[p].x;
                peds[p].yPrev=peds[p].y;
                peds[p].x=peds[p].x+peds[p].vx*dt+0.5*peds[p].prev_ax*dt*dt;
                peds[p].y=peds[p].y+peds[p].vy*dt+0.5*peds[p].prev_ay*dt*dt;
                if(peds[p].x<-L/2 || peds[p].x>L/2 || peds[p].y>H)
                {
                    saveFrame(true);
                    salida<<"# ERROR: Un chabon salió volando.\n";
                    salida<<"# Simulación finalizada.\n";
                    salida.close();
                    goto error;
                }
                if(peds[p].y<=0.0) //Check if the pedestrian exited the room
                {
                    if(peds[p].att=="ego")
                    {
                        egoEv++;
                    }
                    else
                    {
                        coopEv++;
                    }
                    salida<<t<<" "<<peds[p].att<<" "<<egoEv<<" "<<coopEv<<" "<<egoEv+coopEv<<"\n";
                    peds.erase(peds.begin()+p);
                    pedsLeft-=1;
                }
                else
                {
                    if(distance(peds[p].x,peds[p].y,0,0)<r_dens) //Check if the pedestrian is in the density registration region
                    {
                        p_in_r++;
                    }
                    p++;
                }
            }

            for(int p=0;p<peds.size();p++) //Compute the total social force and friction force for each pedestrian
            {
                int policias_vecinos=0;

                for(int p2=p+1;p2<peds.size();p2++)
                {
                    if(distance(peds[p].x,peds[p].y,peds[p2].x,peds[p2].y)<5.0)
                    {
                        if(distance(peds[p].x,peds[p].y,peds[p2].x,peds[p2].y)<r_imit && peds[p].att=="ego" && peds[p2].att=="coop")
                        {
                            policias_vecinos++;
                        }

                        float f12x=F_inter(peds[p].x,peds[p].y,peds[p2].x,peds[p2].y,r,r,peds[p].B,peds[p].A).x;
                        float f12y=F_inter(peds[p].x,peds[p].y,peds[p2].x,peds[p2].y,r,r,peds[p].B,peds[p].A).y;
                        
                        if(isInFOV(peds[p].dirx,peds[p].diry,-f12x,-f12y)) //EC 8 PAPER HELBING
                        {
                            peds[p].new_ax+=f12x/m;
                            peds[p].new_ay+=f12y/m;
                        }
                        else
                        {
                            peds[p].new_ax+=cteFOV*f12x/m;
                            peds[p].new_ay+=cteFOV*f12y/m;
                        }


                        float f21x=F_inter(peds[p2].x,peds[p2].y,peds[p].x,peds[p].y,r,r,peds[p2].B,peds[p2].A).x;
                        float f21y=F_inter(peds[p2].x,peds[p2].y,peds[p].x,peds[p].y,r,r,peds[p2].B,peds[p2].A).y;
                        
                        if(isInFOV(peds[p2].dirx,peds[p2].diry,-f21x,-f21y)) //EC 8 PAPER HELBING
                        {
                            peds[p2].new_ax+=f21x/m;
                            peds[p2].new_ay+=f21y/m;
                        }
                        else
                        {
                            peds[p2].new_ax+=cteFOV*f21x/m;
                            peds[p2].new_ay+=cteFOV*f21y/m;
                        }

                        float dist=distance(peds[p].x,peds[p].y,peds[p2].x,peds[p2].y);
                        if(dist<2.0*r)
                        {
                            pto2D froz=F_roz(dist,peds[p],peds[p2],r,r);
                            peds[p].new_ax+=froz.x/m;
                            peds[p].new_ay+=froz.y/m;
                            peds[p2].new_ax+=-froz.x/m;
                            peds[p2].new_ay+=-froz.y/m;
                        }
                    }
                }

                if(peds[p].att=="ego")
                    {
                        if(policias_vecinos>0) //Imitación
                        {
                            peds[p].desiredVel=desiredVel_coop;
                            peds[p].A=A_coop;
                            peds[p].B=B_coop;
                            peds[p].tau=tau_coop;
                            peds[p].imit=true;
                        }
                        else
                        {
                            peds[p].desiredVel=desiredVel_ego;
                            peds[p].A=A_ego;
                            peds[p].B=B_ego;
                            peds[p].tau=tau_ego;
                            peds[p].imit=false;
                        }
                    }

                peds[p].new_ax+=F_inter(peds[p].x,peds[p].y,-L/2,peds[p].y,r,0,peds[p].B,peds[p].A).x/m; //Interaction with the left wall
                peds[p].new_ay+=F_inter(peds[p].x,peds[p].y,-L/2,peds[p].y,r,0,peds[p].B,peds[p].A).y/m;

                peds[p].new_ax+=F_inter(peds[p].x,peds[p].y,L/2,peds[p].y,r,0,peds[p].B,peds[p].A).x/m; //Interaction with the right wall
                peds[p].new_ay+=F_inter(peds[p].x,peds[p].y,L/2,peds[p].y,r,0,peds[p].B,peds[p].A).y/m;

                peds[p].new_ax+=F_inter(peds[p].x,peds[p].y,peds[p].x,H,r,0,peds[p].B,peds[p].A).x/m; //Interaction with the top wall
                peds[p].new_ay+=F_inter(peds[p].x,peds[p].y,peds[p].x,H,r,0,peds[p].B,peds[p].A).y/m;

                if(peds[p].x<-(L/2)+1.0*r) //Friction with left wall
                    {
                        Pedestrian pared(-1,"pared",-(L/2),peds[p].y,0.0,0.0,0.0,0.0,0.0,0.0);
                        pto2D rozWall=F_roz(peds[p].x+(L/2),peds[p],pared,r,0);
                        peds[p].new_ax+=rozWall.x/m;
                        peds[p].new_ay+=rozWall.y/m;
                    }
                if(peds[p].x>(L/2)-1.0*r) //Friction with right wall
                    {
                        Pedestrian pared(-1,"pared",L/2,peds[p].y,0.0,0.0,0.0,0.0,0.0,0.0);
                        pto2D rozWall=F_roz((L/2)-peds[p].x,peds[p],pared,r,0);
                        peds[p].new_ax+=rozWall.x/m;
                        peds[p].new_ay+=rozWall.y/m;
                    }

                if(peds[p].x<(-D/2) || peds[p].x>(D/2)) //The pedestrian's x coordinate is outside of the door
                {
                    peds[p].new_ax+=F_inter(peds[p].x,peds[p].y,peds[p].x,0,r,0,peds[p].B,peds[p].A).x/m; //Interaction with the bottom walls
                    peds[p].new_ay+=F_inter(peds[p].x,peds[p].y,peds[p].x,0,r,0,peds[p].B,peds[p].A).y/m;
                    if(peds[p].y<1.0*r) //Friction with bottom wall
                    {
                        Pedestrian pared(-1,"pared",peds[p].x,0.0,0.0,0.0,0.0,0.0,0.0,0.0);
                        pto2D rozWall=F_roz(peds[p].y,peds[p],pared,r,0);
                        peds[p].new_ax+=rozWall.x/m;
                        peds[p].new_ay+=rozWall.y/m;
                    }
                }
                else if(peds[p].x>(-D/2) && peds[p].x<(-D/2 + r)) //Part of the pedestrian's x coordinate is inside the door
                {
                    peds[p].new_ax+=F_inter(peds[p].x,peds[p].y,-D/2,0,r,0,peds[p].B,peds[p].A).x/m; //Interaction with the bottom walls
                    peds[p].new_ay+=F_inter(peds[p].x,peds[p].y,-D/2,0,r,0,peds[p].B,peds[p].A).y/m;
                }
                else if(peds[p].x<(D/2) && peds[p].x>(D/2 - r)) //Part of the pedestrian's x coordinate is inside the door
                {
                    peds[p].new_ax+=F_inter(peds[p].x,peds[p].y,D/2,0,r,0,peds[p].B,peds[p].A).x/m; //Interaction with the bottom walls
                    peds[p].new_ay+=F_inter(peds[p].x,peds[p].y,D/2,0,r,0,peds[p].B,peds[p].A).y/m;
                }

                float interVx=(peds[p].x-peds[p].xPrev)/dt; //Intermediate velocity (modified Verlet for velocity dependent forces)
                float interVy=(peds[p].y-peds[p].yPrev)/dt;

                peds[p].new_ax+=F0(interVx,interVy,peds[p].desiredVel,peds[p].dirx,peds[p].diry,peds[p].tau,m).x/m; //Desire force
                peds[p].new_ay+=F0(interVx,interVy,peds[p].desiredVel,peds[p].dirx,peds[p].diry,peds[p].tau,m).y/m;
            }

            for(int p=0;p<peds.size();p++) //Update the velocity and acceleration of each pedestrian
            {
                peds[p].vx=peds[p].vx+(peds[p].prev_ax+peds[p].new_ax)/2 *dt;
                peds[p].vy=peds[p].vy+(peds[p].prev_ay+peds[p].new_ay)/2 *dt;

                if(sqrt(pow(peds[p].vx,2)+pow(peds[p].vy,2))>vmax) //Check if the velocity is over vmax
                {
                    peds[p].vx=vmax*(peds[p].vx/sqrt(pow(peds[p].vx,2)+pow(peds[p].vy,2)));
                    peds[p].vy=vmax*(peds[p].vy/sqrt(pow(peds[p].vx,2)+pow(peds[p].vy,2)));
                }

                peds[p].prev_ax=peds[p].new_ax;
                peds[p].prev_ay=peds[p].new_ay;
                peds[p].new_ax=0.0;
                peds[p].new_ay=0.0;   
            }

            dens_exit<<t<<" "<<2*p_in_r/(PI*r_dens*r_dens)<<" "<<r_dens<<endl; //Save density inside the area
            t=roundf((t+dt) * 1000) / 1000; //Update the time
            iter+=1;
            if(iter%frameSaveRate==0) //Save frame if corresponds
            {
                saveFrame(false);
            }
            if(t>tmax) //If the simulation went over the time limit, end it
            {
                break;
            }
        }
        salida<<"\n";
        salida<<"# Simulación finalizada. (0.8 de los peatones evacuados)\n";
        salida<<"Tiempo de evacuación: "<<t<<"\n";
        salida<<"Peatones egoistas evacuados: "<<egoEv<<"\n";
        salida<<"Peatones cooperativos evacuados: "<<coopEv<<"\n";
        salida.close();
    }

    error:
        ;
    return 0;
}