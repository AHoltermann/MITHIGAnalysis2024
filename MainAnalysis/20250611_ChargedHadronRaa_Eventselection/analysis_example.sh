
#!/bin/bash
PATHSKIM=/data00/OOsamples/Skims
#source clean.sh
# rm *.root
TRIGGER=0
ISDATA=0
SCALEFACTOR=1.0

VZMAX=15.0
NVTXMIN=1
CCFILTER=1
PVFILTER=1
ISHIJING=1
HFE_min_1=4.0
HFE_min_2=4.0
source clean.sh

INPUT=$PATHSKIM/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root
OUTPUTANALYSIS=output.root
./ExecuteChargedHadronRAA \
  --Input $INPUT \
  --TriggerChoice $TRIGGER \
  --ScaleFactor $SCALEFACTOR \
  --Output $OUTPUTANALYSIS \
  --IsData $ISDATA \
  --VzMax $VZMAX \
  --NvtxMin $NVTXMIN \
  --CCFilter $CCFILTER \
  --PVFilter $PVFILTER \
  --IsHijing $ISHIJING \
  --HFE_min_1 $HFE_MIN_1 \
  --HFE_min_2 $HFE_MIN_2 \
  


