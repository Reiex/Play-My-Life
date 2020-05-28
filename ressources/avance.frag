uniform sampler2D textureCourrante;
uniform int nbObstacles;
uniform vec2 obstacles[];
uniform int nbBonus;
uniform vec2 bonus[];

vec4 getCouleurPixel()
{
	return texture2D(textureCourrante, gl_TexCoord[0].xy);
}

void main()
{
	vec4 couleur = getCouleurPixel();
	
	if (couleur.r == 0.0 && couleur.g == 0.0 && couleur.b == 0.0)
	{
		couleur.a = 0.0;
	}
	else
	{
		couleur.a = sin(1.57*gl_TexCoord[0].y);
	}
	
	if (couleur.g == 1.0)
	{
		couleur.r = 0.5 + 0.5*abs(sin(10.0*gl_TexCoord[0].y));
		couleur.g = 0.5 + 0.5*abs(cos(10.0*gl_TexCoord[0].y));
		couleur.b = 0.5 + 0.5*abs(sin(20.0*gl_TexCoord[0].x));
	}
	
	gl_FragColor = couleur;
}