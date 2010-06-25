
int grid_h = 100;                   //The height of the world
int grid_w = 100;                   //The width of the world
float soil_temp_exchange = 0.001;       //The rate of heat exchange between the soil of adjacent squares
float air_temp_exchange = 0.05;         //The rate of heat exchange between the air of adjacent squares
float airground_heat_exchange = 0.002;   //The rate of heat exchange between the soil and the air
float air_ground_thermal_ratio = 0.1;   //The relative heat capacity of air as compared to soil
float sunlight_to_heat = 1;         //Calculated based on what is required to heat air (not soil) by 1C
float irradiance_to_space = 0.001;  //The rate at which the air loses its heat
float soil_waterflow_rate = 0.1;    //Waterflow also depends heavily on organic matter
float O2_diffusion = 0.001; //The rate that O2travels between the air in adjacent grids
float CO2_diffusion = 0.001; //The rate that CO2 travels between the air in adjacent grids
float H2O_diffusion = 0.001; //The rate that water vapor travels between the air in adjacent grids
float evapoation_rate = 0.0001; //The of evaporation of water from soil to air
float unit_width = 100; //The width of each unit grid (in meters)
int water_log = 10; //The base of the logarithm used to determine water retention by organic matter
//int N_log = 10; //The base of the logarithm used to determine nitrogen retention by organic matter 
//int P_log = 10; //The base of the logarithm used to determine phosphorous retention by organic matter
float N_leaching = 0.8; //Additional coefficient applied to the waterflow that governs the rate of nitrogen leaching relative to the water flow
float P_leaching = 0.8; //Additional coefficient applied to the waterflow that governs the rate of phosphorous leaching relative to the water flow
float decomposition_rate = 0.0001;
float energy_soil_temp_ratio = 1000000 //The amount of energy required to raise the temperature of soil by 1C
float sunlight_energy = 1000 //The amount of energy in 1 unit of sunlight
float organic_stored_heat = 0.0001 //The amount of energy in a unit of organic matter relative to the specific heat of soil

//SOIL
float z[grid_w][grid_h];
float organic[grid_w][grid_h];
float N[grid_w][grid_h];
float P[grid_w][grid_h];
float soil_H2O[grid_w][grid_h];
float detritus[grid_w][grid_h];     //currently unused
float soil_temp[grid_w][grid_h];
float sunlight[grid_w][grid_h];

//AIR

float O2[grid_w][grid_h];
float CO2[grid_w][grid_h];
float N2[grid_w][grid_h];
float air_H2O[grid_w][grid_h];
float air_temp[grid_w][grid_h];

//SOIL DIFFERENCE
float organic_difference[grid_w][grid_h];
float N_difference[grid_w][grid_h];
float P_difference[grid_w][grid_h];
float soil_H2O_difference[grid_w][grid_h];
float detritus_difference[grid_w][grid_h];      //currently unused
float soil_temp_difference[grid_w][grid_h];

//AIR DIFFERENCE
float O2_difference[grid_w][grid_h];
float CO2_difference[grid_w][grid_h];
float N2_difference[grid_w][grid_h];
float air_H2O_difference[grid_w][grid_h];
float air_temp_difference[grid_w][grid_h];




float org[3]; //Log10 value of organic matter. First value for the square being calculated, second for the right square, third for the bottom square
float waterflow[2]; //Holds the waterflow calculations between cells to save computation 1 is right cell, 2 is bottom cell
float z_coeff[2]; //Holds the gravity coefficient between cells to save computation


//Initialize vales
for(i=0;i<grid_w;i++){
    for(j=0;j<grid_h;j++){
        //SOIL
        z[grid_w][grid_h] = 1;           //in meters
        organic[grid_w][grid_h] = 50;    //value
        N[grid_w][grid_h] = 0.5;         //From 0 to 1   (or value)
        P[grid_w][grid_h] = 0.5;         //From 0 to 1   (or value)
        soil_H2O[grid_w][grid_h] = 0.5;  //From 0 to 1
        detritus[grid_w][grid_h] = 0;    //value
        soil_temp[grid_w][grid_h] = 25;  //in C
        sunlight[grid_w][grid_h] = 25;   //25,000 "energy" per tick, equilibrates to 25C
        O2[grid_w][grid_h] = 20;         //out of 100
        CO2[grid_w][grid_h] = 2;         //out of 100
        N2[grid_w][grid_h] = 78;         //out of 100
        air_H2O[grid_w][grid_h] = 0.5;   //From 0 to 1, form clouds at 1
        air_temp[grid_w][grid_h] = 25;   //in C
    }
}

while(1){                   //This is the main simulation

    //Reset values for difference matrix:
    for(i=0;i<100;i++){
        for(j=0;j<100;j++){
            //SOIL DIFFERENCE
            organic_difference[i][j] = 0;
            N_difference[i][j] = 0;
            P_difference[i][j] = 0;
            soil_H2O_difference[i][j] = 0;
            detritus_difference[i][j] = 0;      //currently unused
            soil_temp_difference[i][j] = 0;
            //AIR DIFFERENCE
            O2_difference[i][j] = 0;
            CO2_difference[i][j] = 0;
            N2_difference[i][j] = 0;
            air_H2O_difference[i][j] = 0;
            air_temp_difference[i][j] = 0;
        }
    }
    

    for(i=0;i<100;i++){
        for(j=0;j<100;j++){
            //soil temperature exchange
            soil_temp_difference[i][j] -= soil_temp_exchange*(soil_temp[i][j] - soil_temp[i+1][j]);
            soil_temp_difference[i][j] -= soil_temp_exchange*(soil_temp[i][j] - soil_temp[i][j+1]);
            soil_temp_difference[i+1][j] -= soil_temp_exchange*(soil_temp[i+1][j] - soil_temp[i][j]);
            soil_temp_difference[i][j+1] -= soil_temp_exchange*(soil_temp[i][j+1] - soil_temp[i][j]);
            //air temperature exchange
            air_temp_difference[i][j] -= air_temp_exchange*(air_temp[i][j] - air_temp[i+1][j]);
            air_temp_difference[i][j] -= air_temp_exchange*(air_temp[i][j] - air_temp[i][j+1]);
            air_temp_difference[i+1][j] -= air_temp_exchange*(air_temp[i+1][j] - air_temp[i][j]);
            air_temp_difference[i][j+1] -= air_temp_exchange*(air_temp[i][j+1] - air_temp[i][j]);
            //air-soil heating
            soil_temp_difference[i][j] -= air_ground_thermal_ratio*airground_heat_exhcange*(soil_temp[i][j] - air_temp[i][j]);
            air_temp_difference[i][j] -= airground_heat_exhcange*(air_temp[i][j] - soil_temp[i][j])/air_ground_thermal_ratio;
            //solar heating
            soil_temp_difference[i][j] += sunlight_to_heat*air_ground_thermal_ratio*sunlight[i][j];
            air_temp_difference[i][j] += sunlight_to_heat*(1-air_ground_thermal_ratio)*sunlight[i][j];
            
            
            //irradiance to space
            air_temp_difference[i][j] -= irradiance_to_space*air_temp[i][j];
            
            //calculate coefficients related to water and nutrient flow in soil
            org[0] = 1/log(organic[i][j], water_log);
            org[1] = 1/log(organic[i+1][j], water_log);
            org[2] = 1/log(organic[i][j+1], water_log);
            z_coeff[0] = 1 + ((z[i][j] - z[i+1][j])/unit_width);
            z_coeff[1] = 1 + ((z[i][j] - z[i][j+1])/unit_width);
            
            //water flow soil
            waterflow[0] = soil_waterflow_rate*(org[0]*soil_H2O[i][j]-org[1]*soil_H2O[i+1][j])*z_coeff[0];
            soil_H2O_difference[i][j] -= waterflow[0];
            soil_H2O_difference[i+1][j] += waterflow[0];
            waterflow[1] = soil_waterflow_rate*(org[0]*soil_H2O[i][j]-org[2]*soil_H2O[i][j+1])*z_coeff[1];
            soil_H2O_difference[i][j] -= waterflow[1];
            soil_H2O_difference[i][j+1] += waterflow[1];
            
            //nutrient_flow
            N_difference[i][j] -= N_leaching*waterflow[0]*(org[0]*N[i][j] - org[1]*N[i+1][j]);
            N_difference[i+1][j] += N_leaching*waterflow[0]*(org[0]*N[i][j] - org[1]*N[i+1][j]);
            N_difference[i][j] -= N_leaching*waterflow[0]*(org[0]*N[i][j] - org[2]*N[i][j+1]);
            N_difference[i][j+1] += N_leaching*waterflow[0]*(org[0]*N[i][j] - org[2]*N[i][j+1]);
            
            P_difference[i][j] -= P_leaching*waterflow[0]*(org[0]*P[i][j] - org[1]*P[i+1][j]);
            P_difference[i+1][j] += P_leaching*waterflow[0]*(org[0]*P[i][j] - org[1]*P[i+1][j]);
            P_difference[i][j] -= P_leaching*waterflow[0]*(org[0]*P[i][j] - org[2]*P[i][j+1]);
            P_difference[i][j+1] += P_leaching*waterflow[0]*(org[0]*P[i][j] - org[2]*P[i][j+1]);
            
            //evaporation
            //faster at higher ground T, air_T, lower humidity
            soil_H2O_difference[i][j] -= soil_H2O[i][j]*evaporation_rate*soil_temp[i][j]*(1-air_H2O[i][j]);
            air_H2O_difference[i][j] += soil_H2O[i][j]*evaporation_rate*soil_temp[i][j]*(1-air_H2O[i][j]);
            
            //water_flow_air
            air_H2O_difference[i][j] -= H2O_diffusion*(air_H2O[i][j] - air_H2O[i+1][j]);
            air_H2O_difference[i][j] -= H2O_diffusion*(air_H2O[i][j] - air_H2O[i][j+1]);        
            air_H2O_difference[i+1][j] -= H2O_diffusion*(air_H2O[i][j] - air_H2O[i][j+1]);
            air_H2O_difference[i][j+1] -= H2O_diffusion*(air_H2O[i][j] - air_H2O[i+1][j]);
            
            //air flow
            O2_difference[i][j] -= O2_diffusion*(O2[i][j] - O2[i+1][j]);
            O2_difference[i][j] -= O2_diffusion*(O2[i][j] - O2[i][j+1]);
            O2_difference[i+1][j] -= O2_diffusion*(O2[i+1][j] - O2[i][j]);
            O2_difference[i][j+1] -= O2_diffusion*(O2[i][j+1] - O2[i][j]);
            CO2_difference[i][j] -= CO2_diffusion*(CO2[i][j] - CO2[i+1][j]);
            CO2_difference[i][j] -= CO2_diffusion*(CO2[i][j] - CO2[i][j+1]);
            CO2_difference[i+1][j] -= CO2_diffusion*(CO2[i+1][j] - CO2[i][j]);
            CO2_difference[i][j+1] -= CO2_diffusion*(CO2[i][j+1] - CO2[i][j]);
            
            //decomposition
            organic_difference[i][j] -= decomposition_rate*soil_temp[i][j]*soil_H2O[i][j]
            soil_temp_difference[i][j] += organic_stored_heat*decomposition_rate*soil_temp[i][j]*soil_H2O[i][j]
        }
    }
    for(i=0;i<grid_w;i++){
        for(j=0;j<grid_h;j++){
            //SOIL
            organic[i][j]+= organic_difference[i][j];
            N[i][j]+= N_difference[i][j];
            P[i][j]+= P_difference[i][j];
            soil_H2O[i][j]+= soil_H2O_difference[i][j];
            detritus[i][j]+= detritus_difference[i][j];   //currently unused
            soil_temp[i][j]+= soil_temp_difference[i][j];
            //AIR
            O2[i][j]+= O2_difference[i][j];
            CO2[i][j]+= CO2_difference[i][j];
            N2[i][j]+= N2_difference[i][j]; 
            air_H2O[i][j]+= air_H2O_difference[i][j];
            air_temp[i][j]+= air_temp_difference[i][j];
        }
    }
}

/* conversions for organic matter
  measure organic matter in terms of fixed carbon
  1 sugar = 6 CO2 + 6H2O - 6O2
  1 organic = 6CO2
  1 sunlight captured = 1000 energy (1/6000 of the amount to fix a unit of CO2)
  1,000,000 energy is enough to raise T of soil 1C (1000 sunlight)
  1 organic = 1000 energy
  


for equilibrium at 25C
irradiance to space = 1/1000 air temp if 100,000 units energy is 1C then 25,000 energy is lost per tick.
That would be sunlight=25 on average, averaged over day/night
*/

/* other things that this does not yet do

erosion
reflectivity


