int main() {
	static const int stksz = 10000000;
	static int stk[stksz], espbak;
	__asm__ __volatile__ ( "movl %%esp, %0\n\tmovl %1, %%esp\n\t" : "=g"(espbak) : "g"(stk+stksz-1)  );
	solve();
	exit(0);
}
