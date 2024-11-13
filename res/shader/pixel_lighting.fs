#version 330
uniform float shade;
in vec3 cshade;
in vec3 v_o;
in vec3 locallight_o;
in vec3 n;
in float shadescale_o;
out vec4 pixelcolour;
void main( )
{
    float shadescale;
    shadescale=shadescale_o;
    vec3 vn,l,ln,nn,h,hh;
    vec3 ambient = vec3(0.4,0.4,0.4);       // ambient light    

   vn=normalize(v_o);
    ln=normalize(locallight_o);
    if (dot(ln,n)<0)
    {
        h=vn-ln;//light direction shines from source to object
        hh=h;
        h=normalize(h);
        nn=normalize(n);
        shadescale= dot(h,nn);//inward normal
        if (shadescale<0)
            shadescale=0;
        shadescale*=shadescale;
        shadescale*=shadescale;
    }
    else
    shadescale=0;

    // Get pixel colour from input shade
    if (shade>=0.5)
    {       
        pixelcolour = vec4( (cshade * shadescale) + (ambient * cshade), 1.0 );  // ambient lighting     

        //pixelcolour = vec4( (cshade*ambient)+ambient, 1.0 );
        //pixelcolour += vec4(ambient, 1.0);
    }
    else
    {
        pixelcolour = vec4( (cshade * shadescale_o) + (ambient * cshade), 1.0 );

        //pixelcolour = vec4( (cshade*ambient)+ambient, 1.0 );      
        //pixelcolour += vec4(ambient, 1.0);

    }               

}
