
int main() {
	int a[5] = {55, 20, 3, 4, 5};
	int min_val;
	int x = a[0];
	int i;
	for (i = 1; i < sizeof(a)/sizeof(a[0]); i++) {
		x = (x > a[i]) ? a[i] : x;
	}
	min_val = x;
	return min_val;
}
