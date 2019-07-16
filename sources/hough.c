#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../includes/hough.h"
#include "../includes/vector.h"

void houghTransform(unsigned char* image, int width, int height, int thresh, int lineLength,
 int lineGap, int linesMax, float rho, float theta, Line *lines)
{	
	// PPHT Step 1: Return if empty
	if ((!image)) {
		printf("Image is empty. Cannot perform PPHT\n");
		return;
	} 
	unsigned char* out = (unsigned char*) malloc(width*height);
	int line_count = 0;
	
	// Initialize rho and theta
	float irho = 1/rho;
	int num_theta = round(PI/theta); 
	int num_rho = round(((width + height) * 2 + 1) / rho);
	//printf("num theta %d", num_theta);
	// Initialize and fill accumulator with zeros
	int acc [num_rho][num_theta];
	memset(acc, 0, num_rho*num_theta*sizeof(int));

	// Create vector holding trigonometry functions
    Trig trigtab[num_theta*2];
    for(int n = 0; n < num_theta; n++)
    {
        trigtab[n*2].data = (cos((double)n*theta) * irho);
        trigtab[n*2+1].data = (sin((double)n*theta) * irho);
    }
    const float* ttab = &trigtab[0].data;
    
	// Initialize randomizer, used to get random pixel
	srand(time(NULL));
	int pos;

	int max_value = thresh - 1;
	unsigned char* mdata0 = (unsigned char*) malloc(width*height); //used to store values in

	// Collect edges	
	Point *nzloc = NULL;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (image[j*width + i] > thresh)
			{
				mdata0[j*width + i] = 255;
				Point temp;
				temp.x = i;
				temp.y = j;
				vector_push_back(nzloc, temp);
			}
			else
			{
				mdata0[j*width + i] = 0;
			}
			out[j*width + i] = 0;
		}
	}
	int t;
			FILE *file;
			file = fopen("mdata.pgm", "wb");
			fprintf(file, "P5 # %dus\n%d %d\n255\n", t, width, height);
			fwrite(mdata0, width*height, 1, file);
	/*
	for (int i = 0; i < width * height; i++)
	{
		if (image[i] > 100)
		{
			mdata0[i] = 255;
			Point temp;
			temp.x = i % width;
			temp.y = i / width;
			vector_push_back(nzloc, temp);
		}
		else
		{
			mdata0[i] = 0;
		}
		out[i] = 0;
	}*/
	int count = vector_size(nzloc);

    printf("NZLOC count %d\n", count);
	for (; count > 0; count--)
	{	
		int max_theta = 0;
		
		// PPHT Step 2: Randomly select pixel from input image.
		pos = rand() % count;
		Point point = nzloc[pos];
		int j = point.x;
		int i = point.y;
		//printf("Point.x %d, Point.y %d\n", point.x, point.y);
		// PPHT Step 3: Remove pixel, by overriding with last value
		nzloc[pos] = nzloc[count - 1];
		
		// Skip black pixels
		if (mdata0[i*width + j] <= 0) 
		{
			continue;	
		}
	

        // Check if it has been excluded already
        if (!mdata0[i*width + j])
        {
            continue;
        }
                
		
		
		// Map pixel to accumulator
		for (int n = 0; n < num_theta; n++)//, adata += num_rho)
		{
			int rho_temp = round(j * ttab[n*2] + i * ttab[n*2+1]);
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
        int k, x0, y0, dx0, dy0, xflag;
        int good_line;
        const int shift = 16;
        Point line_end[2];
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
			out[width * line_end[0].y + line_end[0].x] = 255;
			out[width * line_end[1].y + line_end[1].x] = 255;
			Line line;
			line.p1.x = line_end[0].x;
			line.p1.y = line_end[0].y;
			line.p2.x = line_end[1].x;
			line.p2.y = line_end[1].y;
			vector_push_back(lines, line);
            if (vector_size(lines) >= linesMax)
            {
				drawLines(out, lines, width, height);
                return;
			}
        }
	}	     
}

void drawLines(unsigned char* image, Line *lines, int width, int height)
{
	printf("vector size %d", vector_size(lines));

	for (int i = 0; i < vector_size(lines); i++)
	{
		int pos1 = lines[i].p1.y*width + lines[i].p1.x;
		int pos2 = lines[i].p2.y*width + lines[i].p2.x;
		while(pos1 != pos2)
		{
			image[pos1] = 155;
				
			if (lines[i].p1.x == lines[i].p2.x)
			{
				//Skip
			}
			else if (lines[i].p1.x > lines[i].p2.x)
			{
				lines[i].p1.x--;
			}
			else if (lines[i].p1.x < lines[i].p2.x){
				lines[i].p1.x++;
			}

			if (lines[i].p1.y == lines[i].p2.y)
			{
				//Skip
			}
			else if (lines[i].p1.y > lines[i].p2.y)
			{
				lines[i].p1.y--;
			}
			else if (lines[i].p1.y < lines[i].p2.y){
				lines[i].p1.y++;
			}

			pos1 = lines[i].p1.y*width + lines[i].p1.x;
		}
	}

	

	int t;
	FILE *file;
	file = fopen("out.pgm", "wb");
	fprintf(file, "P5 # %dus\n%d %d\n255\n", t, width, height);
	fwrite(image, width*height, 1, file);

}

