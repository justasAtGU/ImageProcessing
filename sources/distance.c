#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../includes/distance.h"
#include "../includes/struct.h"
#include "../includes/vector.h"

void angleBetweenLines(Line *lines, int base_index, int line_count, int height)
{
	int count = 0;
	int cone_height1, cone_height2;

	// Go through all edges to look for angles/cones
	for (int j = 0; j < line_count; j++)
	{
		//Find slope. Check if line is vertical or horizontal
		float m1 = ((float)(lines[j].p2.y - lines[j].p1.y) / (float)(lines[j].p2.x - lines[j].p1.x));
		if (m1 == 0 || !isfinite(m1))
		{
			continue;
		}

		for (int i = 0; i < line_count; i++)
		{
			// Same line. Skip
			if (j == i)
			{
				continue;
			}

			// Find second slope
			float m2 = ((float)(lines[i].p2.y - lines[i].p1.y) / (float)(lines[i].p2.x - lines[i].p1.x));

			//  Check if line is vertical or horizontal
			if (m2 == 0 || !isfinite(m2))
			{
				continue;
			}
			// Both slopes positive/negative, so lines may be parallel
			else if ((m1 > 0 && m2 > 0) || (m1 < 0 && m2 < 0))
			{
				continue;
			}
            
			// Slopes to different to belong to a cone
            // Replace with a thresh
			else if (abs(m1) - abs(m2) >= 1.5)
			{
                float diff = abs(m1 - m2);
				printf("Slopes too different m1 %f m2 %f. Difference %f \n", m1, m2, diff);
				continue;
			}

			//Formula tan(theta) = |(m2-m1)/(1+(m2*m1)|
			// Inversing formula to find angle
			float theta = atan(fabs((m2-m1)/(1+(m2*m1)))) * (180/PI);
			printf("angle between lines %f\n", theta);

			// Angle does not belong to top of cone
			if (theta > 60)
			{
				continue;
			}

			cone_height1 = abs(lines[i].p1.y - lines[i].p2.y);
			cone_height2 = abs(lines[j].p1.y - lines[j].p2.y);
			printf("H1 %d, P1.x %d, P1.y %d, P2.x %d, P2.y %d\n", cone_height1,
				lines[j].p1.x, lines[j].p1.y, lines[j].p2.x, lines[j].p2.y);
			printf("H2 %d, P1.x %d, P1.y %d, P2.x %d, P2.y %d\n", cone_height2,
				lines[i].p1.x, lines[i].p1.y, lines[i].p2.x, lines[i].p2.y);

			vector_erase(lines, j);
			vector_erase(lines, i);
			line_count = vector_size(lines);
            
            float distance = (FOCAL_LENGTH * real_size * height) / (cone_height * SENSOR_HEIGHT);

            printf("Distance %f (mm), %f (cm) and %f (m)\n", distance, distance / 100, distance / 1000);
	
		}
	}
}
