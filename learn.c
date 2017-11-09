// MACHINE LEARNING FOR HOUSE PRICES

// Sophia Dong
// Rutgers University CS211

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//function to multiply 2 matrices, returns matrix object result (pointer)
void m_multiply(int m1, int n1, int m2, int n2, double matrix1[m1][n1], double matrix2[m2][n2], double result[m1][n2]){
	
	for (int a=0; a<m1; a++){
		for (int b=0; b< n2; b++){
			//initialize matrix
			result[a][b] = 0;
		}
	}
	
	for (int a=0; a<m1; ++a){
		for (int b=0; b<n2; ++b){
			for (int c=0; c<n1; c++){
				//yay pointer arithmetic!!
				*(*(result + a) + b) += *(*(matrix1 + a) + c) *
				*(*(matrix2 + c) + b);
			}
		}
	}
	
}


//function to compute inverse of matrix function
//using Gauss Jordan elimination, returns matrix object result (pointer)
void m_inverse(int m1, int n1, int m2, int n2, double matrix1[m1][n1], double matrix2[m2][n2]){
	
	int dim = 0;
	int rows = m1;
	int cols = n1;
	
	
	//check to see which dimension is larger
	if (rows < cols){
		//rows is smaller 
		dim = rows;
	}
	else{
		//columns is smaller or they're the same
		dim = cols;
	}
	
	//generate identity matrix
	for (int a=0; a<dim; a++){
		matrix2[a][a]=1;
	}
	
	//as long as first element is not 0 or 1, divide all to make first element 1
	if (matrix1[0][0] == 0 || matrix1[0][0] == 1){
		//do nothing
	}
	else{
		
		//not 0 or 1
		
		double temp = matrix1[0][0];
		for (int a=0; a<cols; a++){
			matrix1[0][a] = matrix1[0][a]/temp;
			matrix2[0][a] = matrix2[0][a]/temp;
		}
	}
	
	//now need to make everything under [0][0] a 0
	for (int a=1; a<rows; a++){
		
		if (matrix1[a][0] == 0){
			//do nothing
		}
		else if (matrix1[a][0]==1){
			//go through columns and subtract
			for (int b=0; b<cols; b++){
				matrix1[a][b] = matrix1[a][b] - matrix1[0][b];
				matrix2[a][b] = matrix2[a][b] - matrix2[0][b];
			}
		}
		else{; 
			//not 0 or 1, need to subtract a multiple
			double mult = matrix1[a][0];
			for (int c=0; c<cols; c++){
				matrix1[a][c] = matrix1[a][c] - (matrix1[0][c] * mult);
				matrix2[a][c] = matrix2[a][c] - (matrix2[0][c] * mult);
			}
		}
	}
	
	//now goes through the rest of the diagonals to make everything 1
	for (int a=1; a<dim; a++){
		
		if (matrix1[a][a] == 0 || matrix1[a][a] == 1){
			//do nothing
		}
		else{
			//not 0 or 1
			double div = matrix1[a][a];
			for (int b=0; b<cols; b++){
				matrix1[a][b] = (matrix1[a][b]/div);
				matrix2[a][b] = (matrix2[a][b]/div);
			}
		}
		
		if (rows > a+1){
			//adjusts
			
			for (int z=a+1; z<rows; z++){
				if (matrix1[z][a] == 0){
					//do nothing
				}
				else if (matrix1[z][a] == 1){
					//subtract exact amount
					for (int zz=0; zz<cols; zz++){
						matrix1[z][zz] = matrix1[z][zz] - matrix1[a][zz];
						matrix2[z][zz] = matrix2[z][zz] - matrix2[a][zz];
					}
				}
				else{
					//subtract a multiple
					double mult = matrix1[z][a];
					for (int zz=0; zz<cols; zz++){
						matrix1[z][zz] = matrix1[z][zz] - (matrix1[a][zz] * mult);
						matrix2[z][zz] = matrix2[z][zz] - (matrix2[a][zz] * mult);
					}
				}
			}
		}
		
		//these for loops are killing me
		//now we need to go from the bottom row upwards
		for (int x=a-1; x>=0; x--){
			if (matrix1[x][a] == 0){
				//do nothing
			}
			else if (matrix1[x][a] == 1){
				//subtract exact amount
				for (int y=0; y<cols; y++){
					matrix1[x][y] = matrix1[x][y] - matrix1[a][y];
					matrix2[x][y] = matrix2[x][y] - matrix2[a][y];
				}
			}
			else{
				//need to find a mult
				double mult = matrix1[x][a];
				for (int y=0; y<cols; y++){
					matrix1[x][y] = matrix1[x][y] - (matrix1[a][y] * mult);
					matrix2[x][y] = matrix2[x][y] - (matrix2[a][y] * mult);
				}
			}
		}
		
		
	}
	
	
}

//function to print results of a matrix
void printresult(int m, int n, double result1[m][n]){
	for (int a=0; a<m; a++){
		for (int b=0; b<n; b++){
			printf("%0.0f\n", result1[a][b]);
		}
	}
}

int main(int argc, char **argv){
	
	//check for valid inputs
	/*if (argc != 3){
		printf("error\n");
		return 0;
	}*/
	
	//printf("arg1: %s", argv[1]);
	
	char *word; //why is there a null character at the end of my second argument
	
	char *ftrain; //name of training file
	char *ftest; //name of testing file
	
	for (int a=1; a<=2; a++){
		//goes through both files
		//f1 = training file
		//f2 = testing file
		word = argv[a]; //assigns word to variable
		
		if (a==1){
			//training file
			ftrain = word;
		}
		else{
			//a=2
			//testing file
			ftest = word;
		}
		
	}
	
	//printf("ftrain: %s\n", ftrain);
	//printf("ftest: %s\n", ftest);
	
	//open training file
	FILE *f = fopen(ftrain, "r");
	
	//first 2 lines of training file:
	int k; //attributes
	int n; //train lines
	
	//read first 2 lines
	fscanf(f, "%d\n", &k);
	fscanf(f, "%d\n", &n);
	
	//printf("k value: %d\n", k);
	//printf("n value: %d\n", n);
	
	double X[n][k+1]; //matrix for training data: X
	int mcounter = 0;
	int ncounter = 0;
	
	double data; //used for storing in matrix
	for (int a=1; a<=n*(k+1); a++){
		
		if(a%(k+1) == 0){
			//read with\n
			fscanf(f, "%lf\n", &data);
			X[ncounter][mcounter] = data;
			ncounter++;
			mcounter=0;
		}
		else{
			//read with ,
			fscanf(f, "%lf,", &data);
			X[ncounter][mcounter] = data;
			mcounter++;	
		}
		
	}
		
		
	//print out matrix for testing purposes
	/*for (int a=0; a<n; a++){
			for (int b=0; b<k+1; b++){
				printf("%lf", X[a][b]);
				if (b < k){
					//put space
					printf(" ");
				}
			}
			printf("\n");
		}*/
		
	//separate it into a separate matrix:
	//Y = size nx1 = first column
	double Y[n][1]; //initialize Y matrix
	for (int a=0; a<n; a++){
		Y[a][0] = X[a][k];
		//printf("%lf\n", Y[a][0]);
	}
	
	//now need to transpose X and store it as transposed
	//X has size n x (k+1) so X^T will be size (k+1) x n
	/*double Xt[k+1][n];
	for (int a=0; a<k+1; a++){
		for (int b=0; b<n; b++){
			//every row is a column, every column is a row
			Xt[a][b] = X[b][a];
		}
	}*/
	
	//create unaugmented X size n x k (get rid of result line)
	//use this for inverse computation
	double unaugX[n][k];
	for (int a=0; a<n; a++){
		for (int b=0; b<k; b++){
			unaugX[a][b] = X[a][b];
		}
	}
	
	FILE *g = fopen(ftest, "r");
	
	if (f == 0){
		//doesn't exist, couldn't open
		perror("error fopen\n");
		exit(1);
	}
	
	int testnum; //number of rows to predict
	
	//read first line to get rows
	fscanf(g, "%d\n", &testnum);
	
	double testdata[testnum][k]; //matrix for training data: X
	int testcounter = 0;
	int kcounter = 0;
	
	//can reuse the data variable as a temp
	
	for (int a=1; a<=testnum*k; a++){
		
		if(a%(k) == 0){
			//read with\n
			fscanf(g, "%lf\n", &data);
			testdata[testcounter][kcounter] = data;
			testcounter++;
			kcounter=0;
		}
		else{
			//read with ,
			fscanf(g, "%lf,", &data);
			testdata[testcounter][kcounter] = data;
			kcounter++;	
		}
		
	}
	
	double Xshift[testnum][k+1]; //with 1 as first column for testing data
	for (int a=0; a<testnum; a++){
		Xshift[a][0] = 1;
		for (int b=1; b<k+1; b++){
			Xshift[a][b] = testdata[a][b-1];
		}
	}
	
	
	
	//printf("\n\n");
	//print out tranposed matrix
	/*for (int a=0; a<k+1; a++){
		for (int b=0; b<n; b++){
			printf("%lf", Xt[a][b]);
			if (b < n-1){
				//put space
				printf(" ");
			}
		}
		printf("\n");
	}*/
	
	//now multiply Xt * X
	
	//Xt has size (k+1) x n??
	//X has size n x (k+1)??
	
	//create new matrix (k+1) x (k+1) or (k+2)x(k+2)??
	double XtmX[k+1][k+1];
	
	//initialize XtmX
	for (int a=0; a<k+1; a++){
		for (int b=0; b<k+1; b++){
			XtmX[a][b]=0.0;
		}
	}
	
	//for training data
	double XX[n][k+1];
	for (int a=0; a<n; a++){
		XX[a][0] = 1;
		for (int b=1; b<k+1; b++){
			XX[a][b] = X[a][b-1];
		}
	}
	
	double XXt[k+1][n];
	for (int a=0; a<k+1; a++){
		for (int b=0; b<n; b++){
			XXt[a][b] = XX[b][a];
		}
	}
	
	//multiply XXt by XX to get XtmX
	m_multiply(k+1,n,n,k+1,XXt,XX, XtmX);
	

	
	//do the inverse of the result
	double XtmXinv[k+1][k+1];
	for (int a=0; a<k+1; a++){
		for (int b=0; b<k+1; b++){
			XtmXinv[a][b] = 0.0;
		}
	}
	m_inverse(k+1,k+1,k+1,k+1,XtmX, XtmXinv);
	
	
	
	
	
	
		
	
	//now compute result (besides the multiply by Y part)
	double RminY[k+1][n];
	for (int a=0; a<k+1; a++){
		for (int b=0; b<n; b++){
			RminY[a][b] = 0;
		}
	}
	m_multiply(k+1,k+1,k+1,n,XtmXinv, XXt, RminY);
	
	
	
	
	
	
	
	
	//now multiply the entire thing by Y to get Result
	double W[k+1][1];
	for (int a=0; a<k+1; a++){
		W[a][0] = 0.0;
	}
	m_multiply(k+1, n, n, 1, RminY, Y, W);
	
	/*printf("Y:\n");
	for (int a=0; a<n; a++){
			for (int b=0; b<1; b++){
				printf("%lf", Y[a][b]);
				if (b < 1){
					//put space
					printf(" ");
				}
			}
			printf("\n");
		}
	*/
	
	//compute final result from testing data 
	double result[testnum][1];
	for (int a=0; a<testnum; a++){
		result[a][0]=0.0;
	}
	m_multiply(testnum, k+1, k+1, 1, Xshift, W, result);
	
	
	
	
	
	
	
	
	
	
	
	printresult(testnum, 1, result);
	
	
	
	
	
	
	
	
	
	
	return 0;

	
}
