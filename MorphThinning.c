/*
Morphological Thinning in c
Author: Shruty Janakiraman
Based on the algorithm for bwmorph in matlab
reference: Journal and matlab
Input: binary image
The thinned image is a result of the input image being passed through two conditional checks that are primarily logical operations
Output: thinned binary Image of 1 pixel thickness


*/


#include		<stdlib.h>
#include		<stdio.h>
#include		<string.h>
#include		<math.h>
#include		<assert.h>
#include        <conio.h>
#include		<time.h>



#include		"mainheader.h"

/////////////


unsigned char **Condition1(unsigned char**,unsigned char **,int,int);
unsigned char **Condition2(unsigned char** ,int,int);
int m = 1;
int n = 1;
int b, b1, b2, b3, b4 = 0;
int N1, N2 = 0;
int black_pixels = 0;
int** Index = NULL;
//int f = 0;
int No_Pixels(unsigned char**, int, int,int );
int **Indices(unsigned char**, int , int );
unsigned char** MorphThinning(unsigned char **InputImg, int width, int height)
{
	int x = 0, y = 0, p = 0;
	int converged = 0;
	int iterthinning = 100;
	unsigned char **OriginalImage = NULL;
	unsigned char **CondtionalImage = NULL;
	unsigned char **EndImage = NULL;
	int new_width = 0;
	int new_height = 0;
	new_width = new_width + 2;
	new_height = new_height + 2;
	
	OriginalImage = (unsigned char**)malloc(height* sizeof(unsigned char*));	
	for (int i = 0; i < height; i++)
	{
		OriginalImage[i] =(unsigned char*)malloc(width*sizeof(unsigned char));
	}
	if (OriginalImage == NULL)
	{
		fprintf(stderr, "\n Memory allocation failed");
		exit(1);
	}
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			OriginalImage[y][x] = 0;
		}
	}
	int difference = 0;
	int flag = 0;
			
	//copying the thresholded image into original image
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			OriginalImage[y][x] = InputImg[y][x];
		}
	}	

	int g = 0;
	while (converged!=1)
	{
		CondtionalImage = Condition1(OriginalImage,InputImg, height, width);
		EndImage = Condition2(CondtionalImage, height, width);
		for (y = 0; y < height; y++)
		{
			for (x = 0; x < width; x++)
			{							
				difference = abs(InputImg[y][x] - EndImage[y][x]);
				
			}
		}//end of y	
		if (difference == 0)
			converged = 1;
	}//end of while

	//free((void*)OriginalImage);
	//free((void*)CondtionalImage);
			
	return(EndImage);		
	

}//end of BWMorph funtion

unsigned char **Condition1(unsigned char **OriginalImage, unsigned char **InputImg,int height, int width)
{		
	
	int Mask[3][3];
	//unsigned char **ppConditionalImage = NULL;
	black_pixels = No_Pixels(InputImg, height, width,1);//re define
	Index = Indices(InputImg,height, width);	//re define

	

	for (int j = 0; j < black_pixels; j++)
	{   
		int indx, indy = 0;
		indx = Index[j][1];
		indy = Index[j][0];	
		if ((indx <= width - 1 && indx >= 1) && (indy <= height - 1 && indy >= 1))
		{
			Mask[1][1] = OriginalImage[indy][indx];
			Mask[0][0] = OriginalImage[indy - 1][indx - 1];
			Mask[0][1] = OriginalImage[indy - 1][indx];
			Mask[0][2] = OriginalImage[indy - 1][indx + 1];
			Mask[1][0] = OriginalImage[indy][indx - 1];
			Mask[1][2] = OriginalImage[indy][indx + 1];
			Mask[2][0] = OriginalImage[indy + 1][indx];
			Mask[2][1] = OriginalImage[indy + 1][indx];
			Mask[2][2] = OriginalImage[indy + 1][indx + 1];
		}

		//**********Start Condition check***********************************************

		if (Mask[1][2] == 0 && (Mask[0][2] == 1 || Mask[0][1]) == 1)

			b1 = 1;
		else
			b1 = 0;
		if (Mask[0][1] == 0 && (Mask[0][0] == 1 || Mask[1][0] == 1))
			b2 = 1;
		else


		if (Mask[1][0] == 0 && (Mask[2][0] == 1 || Mask[2][1] == 1))
			b3 = 1;
		else
			b3 = 0;
		if (Mask[2][1] == 0 && (Mask[2][2] == 1 || Mask[1][2] == 1))
			b4 = 1;
		else
			b4 = 0;
		b = b1 + b2 + b3 + b4;

		//*****************Condition check 2***************************************************

		//mask = logical(mask)
		
		for (int i = 0; i <= 2; i++)
		{
			for (int j = 0; j <= 2; j++)
			{
				if (Mask[i][j] == 0)
					Mask[i][j] = 0;
				else if (Mask[i][j] == 1)
					Mask[i][j] = 0;							
			}
		}
		int Ncondition2;
		N1 = (double)(Mask[1][2] | Mask[0][2]) +(double)(Mask[0][1] | Mask[0][0]) + (double)(Mask[1][0] | Mask[2][0]) + (double)(Mask[2][1] | Mask[2][2]);
		N2 = (double)(Mask[0][2] | Mask[0][1]) + (double)(Mask[0][0] | Mask[1][0]) + (double)(Mask[2][0] | Mask[2][1]) + (double)(Mask[2][2] | Mask[1][2]);
		Ncondition2 = (N1 < N2 ? N1 : N2);
		//printf("%d \n", Ncondition2);

		//*******************************Condition 3***************************************************

		int Ncondition3;
		for (int i = 0; i <= 2; i++)
		{
			for (int j = 0; j <= 2; j++)
			{
				if (Mask[i][j] == 0)
					Mask[i][j] = 0;
				else if (Mask[i][j] == 1)
					Mask[i][j] = 0;
			}
		}
		
		Mask[2][2] = !Mask[2][2];		
		Ncondition3 = (Mask[0][2] | Mask[0][1] | Mask[2][2]) & Mask[1][2];

		//**********************************************************************************************

		if (Ncondition3 == 0 && Ncondition2 >= 2 && Ncondition2 <= 3 && b == 1)
			OriginalImage[indy][indx] = 0;

  }// for j= num of black pixels loop
  return(OriginalImage);

}// end of function condition



unsigned char **Condition2(unsigned char **ConditionalImage,int height,int width)
{
	
	int Mask[3][3];
	for (int j = 0; j <black_pixels; j++)
	{
		int indx, indy = 0;
		indx = Index[j][1];
		indy = Index[j][0];
		if ((indx <= width - 1 && indx >= 1) && (indy <= height - 1 && indy >= 1))
		{
			Mask[1][1] = ConditionalImage[indy][indx];
			Mask[0][0] = ConditionalImage[indy - 1][indx - 1];
			Mask[0][1] = ConditionalImage[indy - 1][indx];
			Mask[0][2] = ConditionalImage[indy - 1][indx + 1];
			Mask[1][0] = ConditionalImage[indy][indx - 1];
			Mask[1][2] = ConditionalImage[indy][indx + 1];
			Mask[2][0] = ConditionalImage[indy + 1][indx];
			Mask[2][1] = ConditionalImage[indy + 1][indx];
			Mask[2][2] = ConditionalImage[indy + 1][indx + 1];
		}
		//**********Start Condition check***************

		if (Mask[1][2] == 0 && (Mask[0][2] == 1 || Mask[0][1]) == 1)

			b1 = 1;
		else
			b1 = 0;
		if (Mask[0][1] == 0 && (Mask[0][0] == 1 || Mask[1][0] == 1))
			b2 = 1;
		else
			b2 = 0;
		if (Mask[1][0] == 0 && (Mask[2][0] == 1 || Mask[2][1] == 1))
			b3 = 1;
		else
			b3 = 0;
		if (Mask[2][1] == 0 && (Mask[2][2] == 1 || Mask[1][2] == 1))
			b4 = 1;
		else
			b4 = 0;
		b = b1 + b2 + b3 + b4;

		//*****************Condition check 2***************************

		//mask = logical(mask)

		for (int i = 0; i <= 2; i++)
		{
			for (int j = 0; j <= 2; j++)
			{
				if (Mask[i][j] == 0)
					Mask[i][j] = 0;
				else if (Mask[i][j] == 1)

					Mask[i][j] = 0;
			}
		}

		int Ncondition2;
		N1 = (double)(Mask[1][2] | Mask[0][2]) + (double)(Mask[0][1] | Mask[0][0]) + (double)(Mask[1][0] | Mask[2][0]) + (double)(Mask[2][1] | Mask[2][2]);
		N2 = (double)(Mask[0][2] | Mask[0][1]) + (double)(Mask[0][0] | Mask[1][0]) + (double)(Mask[2][0] | Mask[2][1]) + (double)(Mask[2][2] | Mask[1][2]);
		Ncondition2 = (N1 < N2 ? N1 : N2);

		//*******************************Condition 3***************************************************

		int Ncondition3;
		for (int i = 0; i <= 2; i++)
		{
			for (int j = 0; j <= 2; j++)
			{
				if (Mask[i][j] == 0)
					Mask[i][j] = 0;
				else if (Mask[i][j] == 1)
					Mask[i][j] = 0;
			}
		}

		// only change is in here
		Mask[0][0] = !Mask[0][0];		
		Ncondition3 = (Mask[2][0] | Mask[2][1] | Mask[0][0]) & Mask[1][0];
		//**********************************************************************************************

		if (Ncondition3 == 0 && Ncondition2 >= 2 && Ncondition2 <= 3 && b == 1)
			ConditionalImage[indy][indx] = 0;

	}// for j= num of black pixels loop
	
	
	return(ConditionalImage);
	
}//end of function condition2


int No_Pixels(unsigned char**InputImg, int height, int width,int find)
{
	int total = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (InputImg[i][j] == 1)
				total++;
		}
	}
	return total;

}
int **Indices(unsigned char**InputImg, int height, int width)
{ 
	int ind = 0;
	Index = (int**)malloc(sizeof(int*)*black_pixels);
	if (Index == NULL)
	{
		fprintf(stderr, "\n Memory allocation failed");
		exit(1);
	}
	for (int i = 0; i < black_pixels; i++)
	{
		Index[i] = (int*)malloc(2 * sizeof(int));
	}
	for (int k = 0; k < height; k++)
	{
		for (int l = 0; l < width; l++)
		{
			if (InputImg[k][l] == 1)
			{
				Index[ind][0] = k;
				Index[ind][1] = l;// x cord
				ind++;
			}
			
		}
	}
	return(Index);

}