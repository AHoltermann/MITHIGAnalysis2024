#!/bin/bash
MAXCORES=40

NAME="2024117_ForestDfinderData"
OUTPUT="output"
counter=0
filelist="/data/NewSkims23_24/InputLists/2024117_ForestDfinder.txt"
MERGEDOUTPUT="/data/NewSkims23_24/$NAME.root"
rm $MERGEDOUTPUT

# Function to monitor active processes
wait_for_slot() {
    while (( $(jobs -r | wc -l) >= MAXCORES )); do
        # Wait a bit before checking again
        sleep 1
    done
}


# Check if the filelist is empty
if [[ ! -s "$filelist" ]]; then
    echo "No matching files found in Samples directory."
    exit 1
fi

echo "File list created successfully: $filelist"
rm -rf $OUTPUT
mkdir $OUTPUT
# Loop through each file in the file list
while IFS= read -r file; do
            echo "Processing $file"
            ./Execute --Input "$file" \
            --Output "$OUTPUT/output_$counter.root" \
            --Year 2024 \
            --ZDCTree zdcanalyzer/zdcrechit \
            --ApplyTriggerRejection false \
            --ApplyEventRejection true \
            --ApplyZDCGapRejection true \
            --ApplyDRejection false \
            --ZDCMinus1nThreshold 900 \
            --ZDCPlus1nThreshold 900 \
            --IsData true \
            --PFTree particleFlowAnalyser/pftree &
 #           --DGenTree Dfinder/ntGen &
    ((counter++))
    wait_for_slot
    #if (( counter % $MAXCORES == 0 )); then
    #    wait
    #fi
    #if (( counter % $MAXCORES == 0 )); then
    #    wait
    #fi
done < "$filelist"
wait 

hadd $MERGEDOUTPUT $OUTPUT/output_*.root
echo "All done!"
echo "Merged output file: $MERGEDOUTPUT"
