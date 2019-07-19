#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../includes/distance.h"
#include "../includes/struct.h"
#include "../includes/vector.h"

void angleBetweenLines(Line *lines, int base_index, int line_count)
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
				printf("Slopes parallel? m1 %f m2%f\n", m1, m2);
				continue;
			}
			// Slopes to different to belong to a cone
			else if (abs(m2 - m1) >= 3)
			{
				printf("Slopes too different m1 %f m2%f. Difference %F \n", m1, m2, abs(m2 - m1));
				continue;
			}

			printf("slope m1 %f, at j%d m2 at i%d %f\n", m1, m2, j, i);

			//Formula tan(theta) = |(m2-m1)/(1+(m2*m1)|
			// Inversing formula to find angle
			float theta = atan(fabs((m2-m1)/(1+(m2*m1)))) * (180/PI);
			printf("angle between lines %f\n", theta);

			// Angle does not belong to top of cone
			if (theta > 45)
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
		}
	}
}

// Cone: 325mm, sensor and focal length refer to link;
float distanceToCone(int real_size, Line *lines, int sensor_size, int focal_length, int height)
{
	float cone_sides[3];
	int base;
	//angleBetweenLines(lines, base);

	int pixel_size = 1;
	// Distance in mm
	float distance = (focal_length * real_size * height) / (pixel_size * sensor_size);

	printf("Distance (mm): and (m): %f\n", distance, distance / 1000);
	
}
