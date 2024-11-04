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

int	get_object_type(int object_idx)
{
	return (int(texelFetch(objects, object_idx).r));
}

int	next_object_index(int current_idx)
{
	int	next_idx = int(texelFetch(objects, current_idx + 1).r);
	if (get_object_type(next_idx) == OBJ_NONE)
		return (-1);
	return (next_idx);
}
