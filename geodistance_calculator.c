/*
RUID: 212002524
Author: Marlon Dominguez
Class: Data_Organization
Date: September 29, 2023
*/

#include <math.h>
#include <stdio.h>
#include <string.h>

/*
Input:
- Latitude 1, Latitude 2. {Convert to RADIANS} 
- Longitude 1, Longitude 2. {Convert to RADIANS}
- Earth Radius. {Convert to RADIANS}
*/

//File name constant
#define INPUT_FILE "cities.dat" 

//Struct to be used in cities array
struct cityType {

   char name[20];    /* name of city */

   double lat;            /* latitude of city */

   double lon;          /* longitude of city */

};


/*
Return type: double
Calculates sigma, a decimal value, to be used for the computation of distance between two locations.  
*/
double calc_sigma(double lat_one, double lat_two, double lon_one, double lon_two){
	double sigma = pow( sin((lat_two - lat_one)/2), 2 )  + 
			cos(lat_one) * cos(lat_two) * pow( sin((lon_two - lon_one)/2), 2 );
	
	return sigma;
	

}

/*
Return type: double
Calculates geodistance between two areas. 
Uses calc_sigma(lat_one, lat_two, lon_one, lon_two) to calclulate sigma to be used in geodistance calulcation.
*/
double geodistance(double lat_one, double lon_one, double lat_two, double lon_two){
	double geo_distance = -1;
	 
	double sigma = calc_sigma(lat_one*(M_PI/180), lat_two*(M_PI/180), lon_one*(M_PI/180), lon_two*(M_PI/180));
	int Earth_Radius = 3959;
	//Calculate the distance between two locations with sigma
        geo_distance = 2 * Earth_Radius * atan2(sqrt(sigma), sqrt(1-sigma));
	
	return geo_distance;
}

/*
Return type: integer 
Searches database to see if a given city is in the cities array of structs.
*/

int search_database(char target[20], struct cityType cities[]){
	//Search for target
	for(int x = 0; x < 100; x++){
		//If target is in database, return True. 
		if(strcmp(target, cities[x].name) == 0){
			return 0;
		}
	}
	//Target is not in database, return False.
	return 1;
}

/*
Return type: Void
Loops through source_data struct and prints data if city is within given distance.
*/
void cities_withinSource(struct cityType source_data, double maxDist, struct cityType cities[]){
	/*
	Iterate through cityType[] and use str.cmp(const char *s1, const char *s2)
	to confirm that source_city is in the database.	
	*/
	
	for(int i = 0; i < 100; i++){
		double geo_distance = geodistance(source_data.lat, source_data.lon, cities[i].lat, cities[i].lon);

		//Print the city if geodistance is less than inputted max distance
		if(geo_distance <= maxDist && strcmp(source_data.name, cities[i].name) != 0){
			printf("%s Latitude: %lf Longitude: %lf Geodistance: %lf \n", cities[i].name, cities[i].lat, cities[i].lon, geo_distance); 
		}

	}
	return;

}


int main(void){
	//Read and store city information from cities.dat file into an array, cities. 
	struct cityType cities[100];
	FILE *data = fopen(INPUT_FILE, "r");
	
	char city[20];
	double latitude, longitude; 

	//Iterate over cities.dat, parse, and store information into cities array of structs.
	int count = 0; 
	while(!feof(data)){
		
		fscanf(data, "%s %lf %lf", city, &latitude, &longitude);
		strcpy(cities[count].name, city); 
		cities[count].lat = latitude;
		cities[count].lon = longitude;
		
		count+=1;
	}
	
	//Request city from client and store in source_city
	printf("Enter City...");
	char source_city[20];
	scanf("%s", source_city);

	//Catches a city that is not in the database and prompts client to insert again.
	while(search_database(source_city, cities) == 1){
		printf("City is not in database, please try again.");
		printf("Enter City... ");
		scanf("%s", source_city);
	}

	//close file since we are done processing it.
	fclose(data);	

	//Request max distance, convert to absolute value in the case of a negative number.
	double maxdist;
	printf("Enter max distance from %s: ", source_city);
	scanf("%lf", &maxdist);

	//Retrieve source data struct to compute geodistance with other cities.
	struct cityType source_data;
	for(int i = 0; i < 100; i++){
		if(strcmp(source_city, cities[i].name) == 0){
			source_data = cities[i];
		}
	}
	//Print given city data
	printf("\n%s: Latitude: %lf Longitude: %lf \n \n", source_data.name, source_data.lat, source_data.lon);
	printf("Cities within 100 miles of %s:\n", source_city);

	//Print name, latitude, longitude, and geodistance if geodistance is less than 100.
        cities_withinSource(source_data, maxdist, cities);	

		
	return 0;
}
