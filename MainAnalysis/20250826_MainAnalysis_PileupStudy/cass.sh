#!/bin/bash

INPUT_DIR="/eos/cms/store/group/phys_heavyions/aholterm/OO2025/skim_250805_OO_IonPhysics0_LowPtV2"
BATCH_SIZE=500
OUTPUT_PREFIX="${INPUT_DIR}/intermediate_batch"
FINAL_OUTPUT="${INPUT_DIR}/MergedOutput.root"

# Get all .root files from the directory
echo "Scanning directory: $INPUT_DIR"
FILES=($(find "$INPUT_DIR" -name "*.root" -type f ! -name "MergedOutput.root" | sort))
TOTAL_FILES=${#FILES[@]}

echo "Total files found: $TOTAL_FILES"

if [ $TOTAL_FILES -eq 0 ]; then
    echo "No .root files found in $INPUT_DIR"
    exit 1
fi

# Calculate number of batches needed
BATCHES=$(( (TOTAL_FILES + BATCH_SIZE - 1) / BATCH_SIZE ))
echo "Number of batches: $BATCHES"

# Process each batch
for i in $(seq 1 $BATCHES); do
    START=$(( (i-1) * BATCH_SIZE ))
    END=$(( i * BATCH_SIZE ))
    
    if [ $END -gt $TOTAL_FILES ]; then
        END=$TOTAL_FILES
    fi
    
    echo "Processing batch $i: files $(($START + 1)) to $END"
    
    # Extract files for this batch
    BATCH_FILES=()
    for j in $(seq $START $(($END - 1))); do
        BATCH_FILES+=("${FILES[$j]}")
    done
    
    # Run hadd for this batch
    echo "Running: hadd -f ${OUTPUT_PREFIX}_$(printf "%02d" $i).root ${BATCH_FILES[@]}"
    hadd -f "${OUTPUT_PREFIX}_$(printf "%02d" $i).root" "${BATCH_FILES[@]}"
    
    if [ $? -eq 0 ]; then
        echo "Batch $i completed successfully"
    else
        echo "Error in batch $i"
        exit 1
    fi
done

# All batches completed - keeping intermediate files
echo "All batches completed successfully!"
echo "Intermediate files created:"
ls -la ${OUTPUT_PREFIX}_*.root
echo "Done!"
