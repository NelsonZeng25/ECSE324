extern int MIN_2(int x, int y);

int main() {
/*
	int a, b, c;
	a = 1;
	b = 2;
	return MIN_2(a,b);
*/

	int a[5] = {1, 2, 3, 420, 5};
	
	int max_val = a[0];			// Initially set max_val to first number of array
	int i;
	for (i = 1; i < sizeof(a)/sizeof(a[0]); i++) {						// Iterate through list
		max_val = (MIN_2(a[i], max_val) == max_val) ? a[i] : max_val;	// If the min_val between a[i] and max_val is max_val, set max_val to a[i]
	}
	return max_val;
}
