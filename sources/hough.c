#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../includes/hough.h"

void houghTransform(unsigned char* image, int width, int height, int thresh, int lineLength,
 int lineGap, int linesMax, float rho, float theta, Vector lines[4])
{	
	// PPHT Step 1: Return if empty
	if ((!image)) {
		printf("Image is empty. Cannot perform PPHT\n");
		return;
	} 
	
	// Initialize rho and theta
	float irho = 1/rho;
	int num_theta = round(PI/theta); 
	int num_rho = round(((width + height) * 2 + 1) / rho);

	//printf("num theta %d", num_theta);
	// Initialize and fill accumulator with zeros
	int acc [num_rho][num_theta];
	memset(acc, 0, num_rho*num_theta*sizeof(int));

	// Create vector holding trigonometry functions
    Vector trigtab[num_theta*2];
    for(int n = 0; n < num_theta; n++)
    {
        trigtab[n*2].data = (cos((double)n*theta) * irho);
        trigtab[n*2+1].data = (sin((double)n*theta) * irho);
    }
    const float* ttab = &trigtab[0].data;
    
	// Initialize randomizer, used to get random pixel
	srand(time(NULL));
	
	// Initialize positions for pixels
	int length = width * height;
	int pixel_x, pixel_y, pos;

	int max_value = thresh - 1;
	unsigned char* mdata0 = (unsigned char*) malloc(width*height); //used to store values in

	// Collect edges
	for (int i = 0; i < length; i++)
	{
		if (image[i] == 255)
		{
			mdata0[i] = image[i];
		}
		
	}
	
	for (; length > 0; length--)
	{	
		int max_theta = 0;
		
		// PPHT Step 2: Randomly select pixel from input image.
		pos = rand() % length;
		
		// Skip black pixels
		if (image[pos] != 255) 
		{
			continue;	
		}
		
		// Get pixel postions, in terms of rows and coloumns
		pixel_x = pos % width;
		pixel_y = pos / width;

        // Check if it has been excluded already
        if (!mdata0[pos])
        {
            continue;
        }
        
        //printf("past exclussion\n"); 
        
		// PPHT Step 3: Remove pixel, by overriding with last value
		image[pos] = image[length];
		
		// Map pixel to accumulator
		for (int n = 0; n < num_theta; n++)//, adata += num_rho)
		{
			int rho_temp = round(pixel_x * ttab[n*2] + pixel_y * ttab[n*2+1]);
            rho_temp += (num_rho - 1) / 2;
            //float rho_f = (pixel_x * cos(n)) + (pixel_y * sin(n));

            int val = ++acc[rho_temp][n];

            //printf("num_rho %d temp_rho %d num_theta %d n_theta %d\n", num_rho, rho_temp, num_theta, n);
			//int val = 1;//acc[rho_temp][n]++;
			// Round sin and cos for integer operations
			//rho1 = (pixel_x * floor(A*cos(theta))) + (pixel_y * floor(A*sin(theta)));
			//int rho1 = (pixel_x * cos(n)) + (pixel_y * sin(n));
			//int val = acc[rho1][n]++;//++ before according to opencv?

			// Update max if value in accumulator is higher
			if (max_value < val)
			{
				max_value = val;
				max_theta = n;
			}
		}

		// PPHT STEP 4: Skip if max value in accumulator is lower than threshold
		if (max_value < thresh)
		{
		    continue;
		}
				
		float a = -ttab[max_theta*2+1];
        float b = ttab[max_theta*2];
        int i = pixel_y, j = pixel_x, k, x0, y0, dx0, dy0, xflag;
        int good_line;
        const int shift = 16;
        Point line_end[2];
        line_end[0].x = 0;
        line_end[1].x = 0;
        line_end[0].y = 0;
        line_end[1].y = 0;
        x0 = j;
        y0 = i;
        
		//printf("pass flaot\n");

        if( fabs(a) > fabs(b) )
        {
            xflag = 1;
            dx0 = a > 0 ? 1 : -1;
            dy0 = round( b*(1 << shift)/fabs(a) );
            y0 = (y0 << shift) + (1 << (shift-1));
        }
        else
        {
            xflag = 0;
            dy0 = b > 0 ? 1 : -1;
            dx0 = round( a*(1 << shift)/fabs(b) );
            x0 = (x0 << shift) + (1 << (shift-1));
        }
        
        //printf("pass abs if else\n");

        for (k = 0; k < 2; k++)
        {
            int gap = 0, x = x0, y = y0, dx = dx0, dy = dy0;

            if (k > 0)
            {
                dx = -dx, dy = -dy;
			}
			
			//printf("pass if k > 0 dx %d dy %d\n", dx, dy);

            // walk along the line using fixed-point arithmetics,
            // stop at the image border or in case of too big gap
            for( ;; x += dx, y += dy )
            {
                unsigned char* mdata;
                int i1, j1;

                if (xflag)
                {
                    j1 = x;
                    i1 = y >> shift;
                }
                else
                {
                    j1 = x >> shift;
                    i1 = y;

                }
				//printf("i1 %d j1 %d\n", i1, j1);

				// Check if exceeding image bounds
                if (j1 < 0 || j1 >= width || i1 < 0 || i1 >= height)
                {
                    break;
				}
				//printf("j1 %d i1 %d after bouunds? \n", j1, i1);

                mdata = mdata0 + i1*width + j1;

                // for each non-zero point:
                //    update line end,
                //    clear the mask element
                //    reset the gap
                if (*mdata)
                {
                    gap = 0;
                    line_end[k].y = i1;
                    line_end[k].x = j1;
                    //printf("macarena j1 %d i1 %d k %d\n", j1, i1, k);

                }
                else if (++gap > lineGap)
                {
					//printf("no macarena j1 %d i1 %d k %d\n", j1, i1, k);

                    break;
                }
            }
        }

        good_line = (abs(line_end[1].x - line_end[0].x) >= lineLength ||
                    abs(line_end[1].y - line_end[0].y) >= lineLength);
		
		//printf("good line = %d line ends = %d %d %d %d \n", good_line, line_end[0].x, line_end[0].y, line_end[1].x, line_end[1].y);

		for (k = 0; k < 2; k++)
        {
            int x = x0, y = y0, dx = dx0, dy = dy0;

            if (k > 0)
            {
                dx = -dx, dy = -dy;
			}
			
			//printf("pass again k > 0 k = %d\n", k);

            // walk along the line using fixed-point arithmetics,
            // stop at the image border or in case of too big gap
            for( ;; x += dx, y += dy )
            {

                unsigned char* mdata;
                int i1, j1;

                if(xflag)
                {
                    j1 = x;
                    i1 = y >> shift;
                }
                else
                {
                    j1 = x >> shift;
                    i1 = y;
                    //printf("j1 %d i1 %d\n", j1, i1);

                }

                mdata = mdata0 + i1*width + j1;
                // for each non-zero point:
                //    update line end,
                //    clear the mask element
                //    reset the gap
                if (*mdata)
                {
					//printf("pass if m data \n");
					//printf("good line\n");

                    if (good_line)
                    {
						//printf("pass if good line. \n");

                        //adata = (int*)image;
                        for (int n = 0; n < num_theta; n++)//, adata += num_rho )
                        {
                            int rho_temp = round(j1 * ttab[n*2] + i1 * ttab[n*2+1]);
                            rho_temp += (num_rho - 1) / 2;                            
                            acc[rho_temp][n]--;
                            int vall =  acc[rho_temp][n];

                        }
                        //int vally = 1;
						//printf("pass for loop acccccc\n");

                    }
					//printf("pre mdata = 0\n");
                    
                    *mdata = 0;

					//printf("pass mdata = 0\n");

                }
				//printf("pre if line end statement\n");

                if (i1 == line_end[k].y && j1 == line_end[k].x)
                {
					//printf("in line end if \n\n\n");
                    break;
                }
                //printf("i1 %d line y %d j1 %d line x %d \n", i1, line_end[k].y, j1, line_end[k].x);

            }
            //printf("post ; ; dx dy loop\n");

        }
        
        //printf("pass entire second for loop\n");

        if (good_line)
        {	
			unsigned char* out = (unsigned char*) malloc(width*height);
			out += line_end[0].x;
			out += line_end[0].y;
			out += line_end[1].x;
			out += line_end[1].y;
			//Vector lines[4];
            //Vec4i lr(line_end[0].x, line_end[0].y, line_end[1].x, line_end[1].y);
            lines[0].data =line_end[0].x;
            lines[1].data =line_end[0].y;
            lines[2].data =line_end[1].x;
            lines[3].data =line_end[1].y;

			image = out;
			printf("l0x %d l0y %d l1x %d l1y %d\n", line_end[0].x, line_end[0].y, line_end[1].x, line_end[1].y);
			printf("outty %s\n", out);
            //lines.push_back(lr);
            if (lines->current >= linesMax)
            {
				
				printf("final good line in if with return\n");
                return;
			}
        }
        
        //printf("pass final if goodline\n");

		
		// Step 5: Look along the corridor in the accumulator
		//for( int n = 0; n < numangle; n++ )
		//{
		//	trigtab[n*2] = (float)(cos((double)n*theta) * irho);
		//	trigtab[n*2+1] = (float)(sin((double)n*theta) * irho);
		//}
		//const float* ttab = &trigtab[0];
		// a = -ttab[max_n*2+1];
		// b = ttab[max_n*2];
		

	}
	//image = (unsigned char*)adata;
	     
}
