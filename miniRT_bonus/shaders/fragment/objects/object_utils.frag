int	next_light_type(inout int i)
{
	if (i == -1)
	{
		i++;
		return (int(texelFetch(lights, i).r));
	}
	i = int(texelFetch(lights, i + 1).r);
	return (int(texelFetch(lights, i).r));
}

int	next_object_type(inout int i)
{
	if (i == -1)
	{
		i++;
		return (int(texelFetch(objects, i).r));
	}
	i = int(texelFetch(objects, i + 1).r);
	return (int(texelFetch(objects, i).r));
}
