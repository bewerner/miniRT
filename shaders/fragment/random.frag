float	rand()
{
	g_seed = (214013 * g_seed + 2531011);
	return(float(((g_seed >> 16) & 0x7FFF) / 32768.0));
	// g_seed++;
	// return(sin(g_seed) * 0.5 + 0.5);
}

vec4	dither(vec4 col)
{
	vec4 newcol;
	float	r = col.r * 255.0;
	float	g = col.g * 255.0;
	float	b = col.b * 255.0;

	if (rand() < r - floor(r))
		r++;
	if (rand() < g - floor(g))
		g++;
	if (rand() < b - floor(b))
		b++;

	r = floor(r);
	g = floor(g);
	b = floor(b);

	newcol.r = r / 255;
	newcol.g = g / 255;
	newcol.b = b / 255;
	newcol.a = 1;

	return (newcol);
}

// vec4	dither(vec4 col)
// {
// 	vec4	outcol = col;
// 	float	r = col.r * 255.0;
// 	float	g = col.g * 255.0;
// 	float	b = col.b * 255.0;

// 	if (rand() < r - floor(r))
// 		outcol.r = col.r + 1.0 / 255.0;
// 	if (rand() < g - floor(g))
// 		outcol.g = col.g + 1.0 / 255.0;
// 	if (rand() < b - floor(b))
// 		outcol.b = col.b + 1.0 / 255.0;
// 	outcol.a = col.a;
// 	return (outcol);
// }
