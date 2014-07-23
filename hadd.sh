#!/bin/bash

for i in {0..9}; do hadd out_${i}.root job_*${i}/ak7PFCHS.root; done
hadd ak7PFCHS.root out_*.root
rm out_*.root


for i in {0..9}; do hadd out_${i}.root job_*${i}/ak5PFCHS.root; done
hadd ak5PFCHS.root out_*.root
rm out_*.root
