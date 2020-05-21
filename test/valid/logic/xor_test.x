int main():
	int one = 1;
	int zero = 0;

	int r = 1;

	if one >< zero:
		r = r + 2;

		if zero >< one:
			r = r + 4;

			if !(one >< one):
				r = r + 8;

				if !(zero >< zero):
					r = r + 16;

	return r;
