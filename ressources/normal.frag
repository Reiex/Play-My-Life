uniform sampler2D textureCourante;

vec4 getCouleurPixel()
{
	return texture2D(textureCourante, gl_TexCoord[0].xy);
}

void main()
{
	vec4 couleur = getCouleurPixel();
	
	if (couleur.r == 0.0 && couleur.g == 0.0 && couleur.b == 0.0)
	{
		couleur.a = 0.0;
	}
	
	gl_FragColor = couleur;
}
