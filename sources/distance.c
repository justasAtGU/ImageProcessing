#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../includes/distance.h"
#include "../includes/struct.h"
#include "../includes/vector.h"

void findCone(Line *lines, float angle, int line_count, int height, unsigned char* image)
{
	// Go through all edges to look for angles/cones
	for (int j = 0; j < line_count; j++)
	{
		//Find slope. Check if line is vertical or horizontal
		float m1 = ((float)(lines[j].p2.y - lines[j].p1.y) / (float)(lines[j].p2.x - lines[j].p1.x));
		if (m1 == 0 || !isfinite(m1))
		{
			//printf("Slope m1 %f, is vertical/horizontal\n", m1);
			continue;
		}

		for (int i = 0; i < line_count; i++)
		{
			// Same line. Skip
			if (j == i)
			{
				//printf("AM I STUPID, slope i %d and j %d\n", j, i);
				continue;
			}

			// Find second slope
			float m2 = ((float)(lines[i].p2.y - lines[i].p1.y) / (float)(lines[i].p2.x - lines[i].p1.x));

			//  Check if line is vertical or horizontal
			if (m2 == 0 || !isfinite(m2))
			{
				//printf("Slope m2 %f, is vertical/horizontal\n", m2);
				continue;
			}
			// Both slopes positive/negative, so lines may be parallel
			else if ((m1 > 0 && m2 > 0) || (m1 < 0 && m2 < 0))
			{
                		//printf("Lines paralled, m1 %f, m2 %f\n", m1, m2);
				continue;
			}
			// Slopes to different to belong to a cone
           		// Replace with a thresh
			else if (abs(m1) - abs(m2) >= 2)
			{
                		//printf("Slopes too different m1 %f and m2 %f\n", m1, m2);
				continue;
			}

			//Formula tan(theta) = |(m2-m1)/(1+(m2*m1)|
			// Inversing formula to find angle
			float theta = atan(fabs((m2-m1)/(1+(m2*m1)))) * (180/PI);

			// Angle does not belong to top of cone
			if (theta > angle)
			{
                printf("Angle %f, too large for cone. thesh = %f\n", theta, angle);
				continue;
			}
            
            // Find cone top and possible base
            int top = findIntersectionPoint(lines[j], lines[i], m1, m2);
	    //int top = fmin(fmin(lines[i].p1.y, lines[i].p2.y), fmin(lines[j].p1.y, lines[j].p2.y));
            int base = fmax(fmax(lines[i].p1.y, lines[i].p2.y), fmax(lines[j].p1.y, lines[j].p2.y));
            //printf("DUMMY p1y %d p2 %d p21y %d p22 y %d \n",lines[i].p1.y, lines[i].p2.y, lines[j].p1.y, lines[j].p2.y);

            findDistance(abs(top - base));
            
            // Cone found, remove lines
            line_count -= 2;
            vector_erase(lines, j);
			vector_erase(lines, i);
		}
	}
}

int findIntersectionPoint(Line line1, Line line2, float m1, float m2)
{
    // Line equation: y = mx + b
    //float b1 = -((m1 * line1.p1.x) - line1.p1.y);
    //float b2 = -((m2 * line2.p1.x) - line2.p1.y);
    float b1 = (line1.p1.y - (m1 * line1.p1.x));
    float b2 = (line2.p1.y - (m2 * line2.p1.x));
    //printf("b1 %f, b2 %f, b3 %f, b4 %f\n", b1, b2, b3, b4);
    
    // Intersection point for x: m1x1 + b1 = m2x2 + b2
    int point_x = (b2 - b1)/(m1 - m2);
    int point_y = round((m1 * point_x) + b1);
    point_y += 20;
 //printf("intersection px %d, py %d, b1 %f, b2 %f, m1 %f, m2 %f\n", point_x, point_y, b1, b2, m1, m2);
    return point_y;
}

void findDistance(int cone_height)
{
    float distance = ((FOCAL_LENGTH * CONE_SIZE * IMAGE_HEIGHT) / (cone_height * SENSOR_HEIGHT));
    printf("Distance %f (mm), Cone height (px) %d\n", distance, cone_height);
    return;
}

