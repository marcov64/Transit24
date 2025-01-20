/********************************************
arguments:
- object *p: object of the model containing the relevant data
- string target: label of the LSD element to be tracked
- string x: label of the LSD element tracking (typically, whose equation contains the call to sadapt(...))
- real value s: value expressing the (inverse) speed of adjustment of x to target. It should be a positive value
sadapt is a function implementing a logistical-like adaptive function.
Be "target" a variable with any dynamics and "x" a variable meant to adapt to target following its dynamics.
The logistic-like adaptation means that x is constrained in the rate of change it can provide. If target takes a constant value different from x, also at constant value, the sadapt funtion generate through time a sigma-like pattern for x reaching value of target.
The speed of adjustment of x approaching target is determined by parameter "s", the larger the slower the adaptation.

/********************************************/


double sign(double X) {
 if(X!=0)
  return X/abs(X);
  return 0;
  }
  
double sadapt(object *p, char *target, char *x, double s)
{
double lv[100];



lv[0] = V(target);

lv[1] = VL(x, 1);
lv[2] = VL(x, 2);
lv[6] = s;
lv[17]=lv[7]=lv[1]-lv[2]; //last step
if(lv[17]==0)//last step was 0
 {if(lv[0]!=lv[1])//target not reached
   lv[7]=(lv[0]-lv[1])/(lv[6]*lv[6]);//kickstart
  else
   return(lv[1]);//target reached
 }
if(lv[0]==lv[1])//target reaced
   return(lv[1]);


lv[4]=(lv[6]+1)*(lv[1]-lv[2]); //projection after s steps from here
if(abs(lv[1]-lv[0]) < abs(lv[4]) || sign(lv[7])!=sign(lv[0]-lv[1]) )
 {//projection overshoot target, or x is going in the wrong direction
 lv[5]=lv[7]/(1+1/lv[6]); //slow down
 //LOG("Over: x1=%lf prjct=%lf step=%lf\n", lv[1],lv[1]+lv[4], lv[5]);
 }
else
 {//projection undershoot while going in the right direction
  lv[5]=lv[7]*(1+1/lv[6]); //accellerate
  //LOG("Under: x1=%lf prjct=%lf step=%lf\n", lv[1], lv[1]+lv[4], lv[5]);
 }


if(abs(lv[5])<abs(lv[1]-lv[0])/(lv[6]*lv[6]) )
 {//the step is smaller than the smallest step
  lv[8]=(lv[0]-lv[1])/(lv[6]*lv[6]); //boost the step to the minimum
  //LOG("Boost: x1=%lf prjct=%lf step=%lf boost=%lf\n", lv[1], lv[1]+lv[4], lv[5],lv[8]);
 }
 else
  lv[8]=lv[5];

lv[12]=(lv[6]/10)*(lv[1]-lv[2]); //short term projection, defined as s/10 steps from now
if(abs(lv[1]-lv[0]) < abs(lv[12]) )
 {//short-term projection overshoot target
   lv[18]=lv[8]/(1+1/lv[6]); //further reduce the approaching speed
   //LOG("Reduce: x1=%lf s.t. prj=%lf step=%lf boost=%lf\n", lv[1], lv[1]+lv[12], lv[5],lv[8]);
 }
 else
  {
   lv[18]=lv[8];
  }

 if(abs(lv[1]+lv[18]-lv[0])<abs(lv[18]) )
  {
  lv[28]=lv[0]-lv[1];//arrived there, just fix the arrival
  //LOG("Gotcha: %lf\n", lv[1]+lv[18]);
  }
 else
  lv[28]=lv[18];

if(abs(lv[1]-lv[0]) <abs(lv[1]+lv[28]-lv[0]) )
 {//if the step brings x further away from target
  if(abs(lv[28]) <abs(lv[1]-lv[0])/(lv[6]*lv[6]) )
    {//step very small, real inversion
     lv[38]=lv[28]-(lv[1]-lv[0])/(lv[6]*lv[6]);
    //LOG("Inversion: step=%lf news=%lf\n", lv[28], lv[18]);
    }
   else
    lv[38]=lv[28];

 }
else
 lv[38]=lv[28];

lv[9]=lv[1]+lv[38];
return(lv[9] );

}
