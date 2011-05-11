int main ()                                                                                 
{                                                                                       
//  int a = 5, b;
//  float af=0.0, bf=1.0;
    int i;
    int x=0+2, y=1, z=2, w=3, v[9];
    float xf=1.0 , yf=1.1, zf=0;
    double yd = 1.1, xd[10], wd, ud, vd;
    int xx = x+y+w;
    x = y;
    x = -y;
    x = +y;
    x = y+z;
    y = x*z;
    y = x/z;
    y = x<<z;
    y = x>>1;

    y = (x);
    y = (x + z); 
    x = y+z+w;
    x = y*z+w;
    x = y- z/w;
    x = y+ -z;

    y=yd;

    xf = yf + -zf;

    xd[0] =1.1;
    yd = xd[1];

    xd[1] = 1+2+ yd;
        
    x --;
    y= x + (x++);

    xd[0]--;
    xd[1]++;

    ++x;
    ++v[9];
    ++v[y+z+w];


    xd[1]+=1;

    xd[1]+= ud+vd;

    x += 1;

    for(i=0;i<10;i++)
    {
        x = y;
    }

    
    for(i=x+y;i<x+y+z;i+=w+z)
        x = y;
    
    //    y = x ++ ;
//    z = ++x[y];

  return x+y;                                                                           
}
