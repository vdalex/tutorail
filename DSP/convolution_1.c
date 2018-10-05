#include <stdio.h> 
#include <graphics.h> 

int m, n, x[30], h[30], y[30], i, j, k, x2[30], a[30], p; 

void main() 
{ 
	int gd = DETECT, gm, option; 
	int Y = 0, X = 200, A = 0; 
	printf(" enter the length of the first sequence\n"); 
	scanf("%d",&m); 
	printf(" enter the length of the second sequence\n"); 
	scanf("%d",&n); 

	printf(" enter the first sequence\n"); 
	for(i = 0; i < m; i++) scanf("%d",&x[i]); 

	printf(" enter the second sequence\n"); 
	for(j = 0; j < n; j++) scanf("%d",&h[j]); 

	/* If length of both sequences are not equal */ 
	/* Pad the smaller sequence with zero */ 
		       
	p = m+n-1; 
	for(i = n; i < p; i++) 
	{ 
		h[i] = 0; 
	}            
		               
	for(i = m; i < p; i++) 
	{ 
		x[i] = 0; 
	}       
		   

	y[0] = 0; 
	a[0] = h[0]; 

	for(j = 1; j < p; j++) a[j] = h[p-j]; /*folding h(n) to h(-n)*/ 
	 

	/*Circular convolution*/ 
	for(i = 0; i < p; i++) y[0] += x[i]*a[i]; 

	for(k = 1;k < p; k++) 
	{ 
		y[k] = 0; 
		/* circular shift */ 
		for(j = 1; j < p; j++) x2[j] = a[j-1]; 
		x2[0] = a[p-1]; 
		for(i = 0; i < p; i++) 
		{ 
			a[i] = x2[i]; 
			y[k] += x[i]*x2[i]; 
		} 
	} 

	/*displaying the result*/ 
	printf(" the circular convolution is\n"); 
	for(i = 0; i < p; i++) printf("%d \t", y[i]); 
	initgraph(&gd, &gm, NULL); 
	line(100, 200, 500, 200); 
	for(i = 0; i < p; i++) 
	{ 
		A = y[i]*10; 
		if(A >= 0) 
		{ 
			for(j = 0; j <= A; j++) 
			{ 
				Y = (200+j)-y[i]*10; 
				putpixel(X, Y, WHITE); 
			} 
		} 
		else 
		{ 
			for(j = 0; j >= A; j--) 
			{ 
				Y = (200+j)-y[i]*10; 
				putpixel(X, Y, WHITE);
			}
		} 
		X = X+20; 
	} 

	delay(50000); 
	closegraph(); 
}
