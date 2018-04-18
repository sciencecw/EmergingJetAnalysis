<div id="table-of-contents">
<h2>Table of Contents</h2>
<div id="text-table-of-contents">
<ul>
<li><a href="#orga9e1c2b">1. EmergingJetAnalysis</a>
<ul>
<li><a href="#org9b41fc8">1.1. Sub-packages</a></li>
<li><a href="#orgaa411af">1.2. Dependencies</a>
<ul>
<li><a href="#orge9ea96c">1.2.1. Cog - </a></li>
</ul>
</li>
<li><a href="#orgd1305a8">1.3. Quick instructions</a>
<ul>
<li><a href="#org7224d53">1.3.1. To checkout latest code</a></li>
<li><a href="#orgc3b2b06">1.3.2. To setup environment and compile</a></li>
<li><a href="#org27a34ee">1.3.3. To run main example config file</a></li>
</ul>
</li>
<li><a href="#org0dbe4df">1.4. General steps</a></li>
<li><a href="#org7e5a8be">1.5. Example config file</a></li>
</ul>
</li>
</ul>
</div>
</div>

<a id="orga9e1c2b"></a>

# EmergingJetAnalysis

Main repository for Emerging Jet analysis


<a id="org9b41fc8"></a>

## Sub-packages

-   EmJetAnalyzer: Main analyzer - analyzes input jets and outputs flat NTuple
-   JetFilter: Applies basic kinematic selection for the signal region and outputs selected jets for analysis
-   WJetFilter: Applies W+jet selection and outputs selected jets for analysis


<a id="orgaa411af"></a>

## Dependencies


<a id="orge9ea96c"></a>

### Cog - <http://nedbatchelder.com/code/cog/index.html>

Cog is used for code generation of classes with repetitive structure, e.g. `EmergingJetAnalyzer/interface/OutputTree.h`
To install, simply execute:

    pip install --user cogapp


<a id="orgd1305a8"></a>

## Quick instructions


<a id="org7224d53"></a>

### To checkout latest code

    cmsrel CMSSW_8_0_26_patch1
    cd CMSSW_8_0_26_patch1/src
    cmsenv
    git cms-merge-topic yhshin11:fix-GetTrackTrajInfo # Needed for track trajectory info.
    git clone https://github.com/yhshin11/EmergingJetAnalysis.git


<a id="orgc3b2b06"></a>

### To setup environment and compile

    cmsenv
    cd EmergingJetAnalysis
    ./buildAll.sh


<a id="org27a34ee"></a>

### To run main example config file

    cmsRun Configuration/test/test_cfg.py


<a id="org0dbe4df"></a>

## General steps

For our analysis involving Emerging Jets, we follow the following steps:

1.  Skim the data for useful events and select jets (JetFilter/WJetFilter)
2.  Run the analyzer over the selected events/jets and produce ROOT files containing simple TTree objects (EmJetAnalyzer)
3.  Make histograms/plots from the flat ROOT files


<a id="org7e5a8be"></a>

## Example config file

An example config file is provided at `Configuration/test/test_cfg.py`. Make a copy of this file and edit `process.source` to run over an appropriate EDM file (AOD/AODSIM). You can now run the config file using cmsRun.

The config file comes with some command line options (defined at the top of the file):

-   `data` (default: `0`) - Must be set to 1 when running over real data and not MC.
-   `steps` (default: `skim,analyze`) - Determines which of the provided steps to execute. Setting `skim` turns on the basic selection step. Setting `analyze` turns on the analyzer.
-   `sample` (default: `signal`) - This sets the module parameters to be appropriate for different samples, e.g. if `sample=wjet` is set, the W+jet kinematic selection (WJetFilter) is used instead of the signal region kinematic selection (JetFilter). The details of the configuration are defined in `Configuration/python/emJetTools.py`. (Warning: The logic here may change while the analysis is ongoing.)

For example to run both the skim and analyze steps over signal MC, simply do:

    cmsRun Configuration/test/test_cfg.py data=0 steps=skim,analyze sample=signal

Since the provided options are exactly the default, you could also simply do:

    cmsRun Configuration/test/test_cfg.py

Sometimes you may want to run the two steps separately. For instance since the skimming step is more stable while the analyze step is more in flux, you may want to run only the skimming step over a large dataset, so that you can run different versions of the analyze step later on. To run only the skimming step over data events, you can do:

    cmsRun Configuration/test/test_cfg.py data=1 steps=skim

Note: By default, EmJetAnalyzer expects as input, jets that were selected by WJetFilter or JetFilter. This means that you cannot only run the analyze step over a bare AOD/AODSIM file.

