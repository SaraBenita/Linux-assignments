#!/bin/bash
for arg in "$@"
do

#putting the input in a variable
inputApi="${arg^^}"

#creating folder for all api data
mkdir -p "Api Data"

cd "Api Data"

#creating links for arrival
#to check if it is a valid aircraft
if [[ ${#inputApi} -eq 6 && $inputApi =~ ^[a-zA-Z0-9]+$ ]]; then
	isAircraft="true"
	rm -rf "$inputApi" 2>/dev/null
	mkdir -p $inputApi
	cd $inputApi
	echo "https://opensky-network.org/api/flights/aircraft?icao24=ABCD&begin=BEGINTIME&end=ENDTIME" > arrival_link
#to check if it is a valid airport
elif [[ ${#inputApi} -eq 4 && $inputApi =~ ^[a-zA-Z]+$ ]]; then
	isAircraft="false"
	rm -rf "$inputApi" 2>/dev/null
	mkdir -p $inputApi
	cd $inputApi
 	echo "https://opensky-network.org/api/flights/arrival?airport=ABCD&begin=BEGINTIME&end=ENDTIME" > arrival_link
#case of invalid input
else
	echo "Wrong input! Failed to fetch data for $inputApi"
	continue
fi

#Creating files to store the begin and end time for checking the flights
date -d "24 hours ago" +%s > begin_time
date +%s >> end_time

#modifying the link: setting the data to the link
sed -i "s/BEGINTIME/$(cat begin_time)/" arrival_link 
sed -i "s/ENDTIME/$(cat end_time)/" arrival_link
sed -i "s/ABCD/$inputApi/" arrival_link 

#creating link for departure
sed "s/arrival/departure/" arrival_link > departure_link

#fetching the jsons from the api
curl -s --compressed -L $(cat arrival_link)  -o arrival_json
curl -s --compressed -L $(cat departure_link) -o departure_json

errorFile="error"

if [ -s "$errorFile" ]; then
    echo "The api request came back with an error."
	cd ..
	rm -rf $inputApi
	cd ..
	continue
else
    rm -f "$errorFile" 
fi




#check if the json returned empty - assuming because the airport/aircraft does not exist or no flights
if [[ $(cat arrival_json) == "[]" ]]; then
	echo "No flights found for $inputApi"
	cd ..
	rm -r $inputApi
	cd ..
	continue
else

#To make all the ending } switch to new lines
sed -i 's/}/\n/g' arrival_json
sed -i 's/}/\n/g' departure_json

# To delete all the starting {
sed -i 's/{//g' arrival_json
sed -i 's/{//g' departure_json


#Delete 3 characters: " [ ] from all the file
sed -i 's/"//g' arrival_json
sed -i 's/\[//g' arrival_json
sed -i 's/\]//g' arrival_json
sed -i 's/"//g' departure_json
sed -i 's/\[//g' departure_json
sed -i 's/\]//g' departure_json


#Delete every first comma in a line
sed -i 's/^,//g' arrival_json
sed -i 's/^,//g' departure_json

#Switch every comma with :
sed -i 's/,/:/g' arrival_json
sed -i 's/,/:/g' departure_json


#create headers part in file from the current json
awk -F: '{print $1","$3 ","$5"," $7 ","$9"," $11}' arrival_json > $inputApi.arv
awk -F: '{print $1","$3 ","$5"," $7 ","$9"," $11}' departure_json > $inputApi.dpt

#keep just the first row
sed -i '2,$d' $inputApi.arv
sed -i '2,$d' $inputApi.dpt

#inserting the json wanted content to a new file
awk -F: '{print $2"," $4 ","$6 ","$8"," $10"," $12}' arrival_json > mod_arrival_json
awk -F: '{print $2"," $4 ","$6 ","$8"," $10"," $12}' departure_json > mod_departure_json

#merging the content after the header part
cat mod_arrival_json >> $inputApi.arv
cat mod_departure_json >> $inputApi.dpt


#deleting the files that are no longer in use
rm begin_time end_time arrival_json mod_arrival_json arrival_link departure_link departure_json mod_departure_json

if [[ "$isAircraft" == "true" ]]; then
	rm $inputApi.dpt
	mv $inputApi.arv $inputApi.flt
	echo "Data for aircraft $inputApi fetched successfully!"
else
	echo "Data for airport $inputApi fetched successfully!"
fi

cd ..
cd ..

fi


done