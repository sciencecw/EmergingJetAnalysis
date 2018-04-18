import FWCore.ParameterSet.Config as cms

########################################
# Command line argument parsing
########################################
import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('analysis')
options.register ('crab',
                  0, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.int,          # string, int, or float
                  "Set to 1 to run on CRAB.")
options.register ('data',
                  0, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.int,          # string, int, or float
                  "Set to 1 for data.")
sample_options = ['signal', 'background', 'wjet'] # Valid options for sample
options.register ('sample',
                  'signal', # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.string,          # string, int, or float
                  "Specify type of sample. Valid values: %s" % sample_options)
steps_options = ['skim', 'analyze'] # Valid options for steps
options.register ('steps',
                  [],
                  VarParsing.VarParsing.multiplicity.list, # singleton or list
                  VarParsing.VarParsing.varType.string,          # string, int, or float
                  "Steps to execute. Possible values: skim, analyze.")
options.steps = ['skim', 'analyze'] # default value
options.register ('doHLT',
                  0, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.int,          # string, int, or float
                  "Set to 1 to turn on HLT selection for skim step.")
options.register ('doJetFilter',
                  1, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.int,          # string, int, or float
                  "Set to 1 to turn on JetFilter for skim step.")
# options.register ('ntupleFile',
#                   'ntuple.root', # default value
#                   VarParsing.VarParsing.multiplicity.singleton, # singleton or list
#                   VarParsing.VarParsing.varType.string,          # string, int, or float
#                   "Specify plain root output file created by TFileService")
options.register ('outputLabel',
                  '', # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.string,          # string, int, or float
                  "Specify label for both PoolOutputModule and TFileService output files.")
# Get and parse the command line arguments
options.parseArguments()
print ''
print 'Printing options:'
print options
print 'Only the following options are used: crab, data, sample, steps, doHLT, doJetFilter'
print ''

# Check validity of command line arguments
if options.sample not in sample_options:
    print 'Invalid sample type. Setting to sample type to signal.'
    options.sample = 'signal'
for step in options.steps:
    if step not in steps_options:
        print "Skipping invalid steps: %s" % step
        options.steps.remove(step)

print options.steps

from EmergingJetAnalysis.Configuration.emjetTools import *

process = cms.Process('TEST')
if 'skim' in options.steps and len(options.steps)==1:
    # If only running skim, change process name
    process.setName_('SKIM')

########################################
# Stable configuration
########################################
# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')


## Geometry and Detector Conditions (needed for a few patTuple production steps)
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

## Options and Output Report
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False),
        # SkipEvent = cms.untracked.vstring('ProductNotFound')
)

# Unscheduled execution
# process.options.allowUnscheduled = cms.untracked.bool(False)
process.options.allowUnscheduled = cms.untracked.bool(True)

########################################
# Skim
########################################
import os
cmssw_version = os.environ['CMSSW_VERSION']
skimStep = cms.Sequence()
if 'skim' in options.steps:
    print ''
    print '####################'
    print 'Adding Skim step'
    print '####################'
    print ''
    if options.sample=='wjet':
        skimStep = addWJetSkim(process, options.data)
        if 'CMSSW_7_4_12' in cmssw_version:
            process.wJetFilter.electronID = cms.string('cutBasedElectronID-Spring15-25ns-V1-standalone-medium')
        elif 'CMSSW_7_4_1_patch4' in cmssw_version:
            process.wJetFilter.electronID = cms.string('cutBasedElectronID-CSA14-50ns-V1-standalone-medium')
    else:
        skimStep = addSkim(process, options.data, doJetFilter=options.doJetFilter, doHLT=options.doHLT)

########################################
# Analyze
########################################
analyzeStep = cms.Sequence()
if 'analyze' in options.steps:
    print ''
    print '####################'
    print 'Adding Analyze step'
    print '####################'
    print ''
    analyzeStep = addAnalyze(process, options.data, options.sample)

########################################
# Testing step
########################################
testing = 0
testingStep = cms.Sequence()
if testing:
    testingStep = addTesting(process, options.data, options.sample)

process.p = cms.Path( skimStep * testingStep * analyzeStep )

########################################
# Configure EDM Output
########################################
if 'skim' in options.steps and len(options.steps)==1:
    # If only running skim, add AOD/AODSIM and jetFilter/wJetFilter to output
    print ''
    print '####################'
    print 'Adding EDM output'
    print '####################'
    print ''
    addEdmOutput(process, options.data, options.sample)
else:
    # Otherwise only save EDM output of jetFilter and wJetFilter
    process.out = cms.OutputModule("PoolOutputModule",
        fileName = cms.untracked.string('output.root'),
        outputCommands = cms.untracked.vstring('drop *'),
    )
    if options.sample=='wjet' : process.out.outputCommands.extend(cms.untracked.vstring('keep *_wJetFilter_*_*',))
    else                      : process.out.outputCommands.extend(cms.untracked.vstring('keep *_jetFilter_*_*',))

########################################
# Generic configuration
########################################
if 'CMSSW_7_4_12' in cmssw_version:
    globalTags=['74X_mcRun2_design_v2','74X_dataRun2_Prompt_v3']
elif 'CMSSW_7_4_1_patch4' in cmssw_version:
    globalTags=['MCRUN2_74_V9','74X_dataRun2_Prompt_v0']
elif 'CMSSW_7_6_3' in cmssw_version:
    globalTags=['76X_mcRun2_asymptotic_RunIIFall15DR76_v1','76X_dataRun2_16Dec2015_v0']
elif 'CMSSW_8_0_26_patch1' in cmssw_version:
    # globalTags=['80X_mcRun2_asymptotic_2016_miniAODv2_v1','80X_dataRun2_2016SeptRepro_v7']
    globalTags=['80X_mcRun2_asymptotic_2016_TrancheIV_v8','80X_dataRun2_2016SeptRepro_v7']
else: print 'No global tag specified for CMSSW_VERSION: %s' % cmssw_version
print 'CMSSW_VERSION is %s' % cmssw_version
print 'Using the following global tags [MC, DATA]:'
print globalTags
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, globalTags[options.data], '')

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)
process.MessageLogger.cerr.FwkReport.limit = 20
process.MessageLogger.cerr.default.limit = 100

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # eventsToProcess = cms.untracked.VEventRange("1:36:3523-1:36:3523"),
    fileNames = cms.untracked.vstring(
        # File with single dark pions
        # 'file:/afs/cern.ch/user/y/yoshin/work/public/temp/step2_dark.root'
        # 'file:/home/yhshin/data/testfiles/temp/step2_dark.root'
        # Model A
        # '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-v1/160201_201550/0000/aodsim_1.root',
        # '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-v1/160201_201550/0000/aodsim_100.root',
        # '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-v1/160201_201550/0000/aodsim_101.root',
        # '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-v1/160201_201550/0000/aodsim_102.root',
        # '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-v1/160201_201550/0000/aodsim_104.root',
        # '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-v1/160201_201550/0000/aodsim_105.root',
        # '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-v1/160201_201550/0000/aodsim_106.root',
        # '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-v1/160201_201550/0000/aodsim_107.root',
        # '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-v1/160201_201550/0000/aodsim_108.root',
        # '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-v1/160201_201550/0000/aodsim_109.root',
        # Model B
        # '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-v1/160202_073524/0000/aodsim_106.root',
        # '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-v1/160202_073524/0000/aodsim_107.root',
        # '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-v1/160202_073524/0000/aodsim_109.root',
        # '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-v1/160202_073524/0000/aodsim_11.root',
        # '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-v1/160202_073524/0000/aodsim_110.root',
        # '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-v1/160202_073524/0000/aodsim_111.root',
        # '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-v1/160202_073524/0000/aodsim_113.root',
        # '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-v1/160202_073524/0000/aodsim_114.root',
        # '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-v1/160202_073524/0000/aodsim_115.root',
        # '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelB_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-v1/160202_073524/0000/aodsim_116.root',
        # signal
        # '/store/group/phys_exotica/EmergingJets/EmergingJets_ModelA_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM/150717_090102/0000/aodsim_1.root'
        # QCD MC 74X
        # '/store/mc/RunIISpring15DR74/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/AODSIM/Asympt25ns_MCRUN2_74_V9-v1/00000/10198812-0816-E511-A2B5-AC853D9DAC1D.root'
        # '/store/mc/RunIISpring15DR74/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/AODSIM/Asympt25ns_MCRUN2_74_V9-v1/00000/025B6100-A217-E511-AF6F-0002C92DB46C.root'
        # data skim
        # '/store/group/phys_exotica/EmergingJets/DataSkim-20160302-v0/Run2015D/JetHT/DataSkim-20160302/160303_061653/0000/output_1.root'
        # wjet
        # '/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_1.root'
        # 'file:/afs/cern.ch/user/y/yoshin/eos/cms/store/group/phys_exotica/EmergingJets/wjetskim-v0/SingleMuonD-PRv3/SingleMuon/WJetSkim/151028_030342/0000/output_1.root'
        # wjet MC
        # '/store/mc/RunIISpring15DR74/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/AODSIM/Asympt25ns_MCRUN2_74_V9-v1/00000/006D71A7-73FC-E411-8C41-6CC2173BBE60.root'
        # pickevents from data skim with alphaMax==0
        # 'file:/afs/cern.ch/user/y/yoshin/CMSSW_7_6_3/src/EmergingJetAnalysis/scans/pickevents_alphaMax_0.root'
        # pickevents from data skim with alphaMax>0.9
        # 'file:/afs/cern.ch/user/y/yoshin/CMSSW_7_6_3/src/EmergingJetAnalysis/scans/pickevents_alphaMax_0p9.root'
        # pickevents from QCD MC with alphaMax==0
        # 'file:/afs/cern.ch/user/y/yoshin/CMSSW_7_6_3/src/EmergingJetAnalysis/scans/pickevents_alphaMax_0_QCDMCSkim_HT1500to2000.root'
        # QCD MC 80X (RunIISpring16DR80)
        # '/store/mc/RunIISpring16DR80/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/AODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/00000/005A737D-5919-E611-910A-02163E0148F1.root',
        # 'file:/home/yhshin/data/testfiles/80X/005A737D-5919-E611-910A-02163E0148F1.root',
        # '/store/mc/RunIISpring16DR80/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/AODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_v3-v1/20000/001CC242-4002-E611-A527-0025905A610A.root',
        # 'file:/home/yhshin/data/testfiles/80X/001CC242-4002-E611-A527-0025905A610A.root',
        # Jet HT Data
        # 'file:/home/yhshin/data/testfiles/80X/003EC773-5797-E611-A173-002590E7D7C2.root'
        # SingleMuon Data
        # 'file:/data/users/yhshin/testfiles/80X/00F3CB69-2998-E611-9CE8-0CC47A7E68AA.root',
        # 'file:/data/users/yhshin/testfiles/80X/04C2A309-589C-E611-95A4-3417EBE64B9D.root',
        '/store/data/Run2016G/SingleMuon/AOD/23Sep2016-v1/1110000/04C2A309-589C-E611-95A4-3417EBE64B9D.root',
        '/store/data/Run2016G/SingleMuon/AOD/23Sep2016-v1/1110000/0A53761A-5B9C-E611-ABC8-7845C4FC3B8A.root',
        '/store/data/Run2016G/SingleMuon/AOD/23Sep2016-v1/1110000/281277D6-B19C-E611-9033-848F69FD46E8.root',
        '/store/data/Run2016G/SingleMuon/AOD/23Sep2016-v1/1110000/3EA40A4B-619C-E611-B28C-008CFA0027B4.root',
        '/store/data/Run2016G/SingleMuon/AOD/23Sep2016-v1/1110000/645DFEE0-B19C-E611-9A70-001E67F12486.root',
        # 80XDRtest
        # 'file:/home/yhshin/EmJetMCProd/CMSSW_8_0_21/src/EmJetDigiReco/aodsim_modelB_old_80XDRtest.root'
        # 'file:/home/yhshin/EmJetMCProd/CMSSW_8_0_21/src/EmJetDigiReco/aodsim_QCD_80XDRtest.root'
        # 80X DRtest
        # '/store/user/yoshin/EmJetMC/DRtest/mass_X_d_1000_mass_pi_d_2_tau_pi_d_5/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/DRtest-80X/170405_110038/0000/aodsim_1.root',
        # '/store/user/yoshin/EmJetMC/DRtest/mass_X_d_1000_mass_pi_d_2_tau_pi_d_5/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/DRtest-80X/170405_110038/0000/aodsim_10.root',
        # '/store/user/yoshin/EmJetMC/DRtest/mass_X_d_1000_mass_pi_d_2_tau_pi_d_5/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/DRtest-80X/170405_110038/0000/aodsim_14.root',
        # '/store/user/yoshin/EmJetMC/DRtest/mass_X_d_1000_mass_pi_d_2_tau_pi_d_5/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/DRtest-80X/170405_110038/0000/aodsim_15.root',
        # '/store/user/yoshin/EmJetMC/DRtest/mass_X_d_1000_mass_pi_d_2_tau_pi_d_5/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/DRtest-80X/170405_110038/0000/aodsim_16.root',
        # '/store/user/yoshin/EmJetMC/DRtest/mass_X_d_1000_mass_pi_d_2_tau_pi_d_5/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/DRtest-80X/170405_110038/0000/aodsim_17.root',
        # '/store/user/yoshin/EmJetMC/DRtest/mass_X_d_1000_mass_pi_d_2_tau_pi_d_5/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/DRtest-80X/170405_110038/0000/aodsim_18.root',
        # '/store/user/yoshin/EmJetMC/DRtest/mass_X_d_1000_mass_pi_d_2_tau_pi_d_5/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/DRtest-80X/170405_110038/0000/aodsim_19.root',
        # '/store/user/yoshin/EmJetMC/DRtest/mass_X_d_1000_mass_pi_d_2_tau_pi_d_5/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/DRtest-80X/170405_110038/0000/aodsim_2.root',
        # '/store/user/yoshin/EmJetMC/DRtest/mass_X_d_1000_mass_pi_d_2_tau_pi_d_5/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/DRtest-80X/170405_110038/0000/aodsim_20.root',
        # 74X DR test
        # '/store/user/yoshin/EmJetMC/DRtest/mass_X_d_1000_mass_pi_d_2_tau_pi_d_5/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/DRtest-74X/170405_104435/0000/aodsim_1.root',
        # '/store/user/yoshin/EmJetMC/DRtest/mass_X_d_1000_mass_pi_d_2_tau_pi_d_5/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/DRtest-74X/170405_104435/0000/aodsim_10.root',
        # '/store/user/yoshin/EmJetMC/DRtest/mass_X_d_1000_mass_pi_d_2_tau_pi_d_5/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/DRtest-74X/170405_104435/0000/aodsim_14.root',
        # '/store/user/yoshin/EmJetMC/DRtest/mass_X_d_1000_mass_pi_d_2_tau_pi_d_5/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/DRtest-74X/170405_104435/0000/aodsim_15.root',
        # '/store/user/yoshin/EmJetMC/DRtest/mass_X_d_1000_mass_pi_d_2_tau_pi_d_5/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/DRtest-74X/170405_104435/0000/aodsim_16.root',
        # '/store/user/yoshin/EmJetMC/DRtest/mass_X_d_1000_mass_pi_d_2_tau_pi_d_5/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/DRtest-74X/170405_104435/0000/aodsim_17.root',
        # '/store/user/yoshin/EmJetMC/DRtest/mass_X_d_1000_mass_pi_d_2_tau_pi_d_5/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/DRtest-74X/170405_104435/0000/aodsim_18.root',
        # '/store/user/yoshin/EmJetMC/DRtest/mass_X_d_1000_mass_pi_d_2_tau_pi_d_5/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/DRtest-74X/170405_104435/0000/aodsim_19.root',
        # '/store/user/yoshin/EmJetMC/DRtest/mass_X_d_1000_mass_pi_d_2_tau_pi_d_5/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/DRtest-74X/170405_104435/0000/aodsim_2.root',
        # '/store/user/yoshin/EmJetMC/DRtest/mass_X_d_1000_mass_pi_d_2_tau_pi_d_5/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/DRtest-74X/170405_104435/0000/aodsim_20.root',
        # 74X QCD
        # '/store/mc/RunIISpring15DR74/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/AODSIM/Asympt25ns_MCRUN2_74_V9-v2/20000/0466B78B-1B28-E511-9D25-002590D9D984.root',
        # '/store/mc/RunIISpring15DR74/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/AODSIM/Asympt25ns_MCRUN2_74_V9-v2/20000/069D0214-2427-E511-9CCE-0025905B859E.root',
        # '/store/mc/RunIISpring15DR74/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/AODSIM/Asympt25ns_MCRUN2_74_V9-v2/20000/0C27A159-3C27-E511-A843-A0040420FE80.root',
        # '/store/mc/RunIISpring15DR74/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/AODSIM/Asympt25ns_MCRUN2_74_V9-v2/20000/0E369CA3-4428-E511-9020-00266CF9C244.root',
        # '/store/mc/RunIISpring15DR74/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/AODSIM/Asympt25ns_MCRUN2_74_V9-v2/20000/100BD79F-E627-E511-AC61-20CF305B0519.root',
        # '/store/mc/RunIISpring15DR74/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/AODSIM/Asympt25ns_MCRUN2_74_V9-v2/20000/1C8A6770-3C27-E511-8BA0-A0040420FE80.root',
        # '/store/mc/RunIISpring15DR74/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/AODSIM/Asympt25ns_MCRUN2_74_V9-v2/20000/2061CC9A-4428-E511-8123-00266CF9AEA4.root',
        # '/store/mc/RunIISpring15DR74/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/AODSIM/Asympt25ns_MCRUN2_74_V9-v2/20000/265F7597-0F27-E511-BA02-0025905A60B8.root',
        # '/store/mc/RunIISpring15DR74/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/AODSIM/Asympt25ns_MCRUN2_74_V9-v2/20000/2E309214-2427-E511-951E-0025905B859E.root',
        # '/store/mc/RunIISpring15DR74/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/AODSIM/Asympt25ns_MCRUN2_74_V9-v2/20000/38A47F4C-4427-E511-9E70-0002C90B7480.root',
        # 80X test with step1 and step2
        # 'file:/home/yhshin/EmJetMCProd/CMSSW_8_0_21/src/EmJetDigiReco/step2.root',
        # 80X test with RunIISummer15GS
        # 'file:/home/yhshin/EmJetMCProd/CMSSW_8_0_21/src/EmJetDigiReco/aodsim-20170430.root'
        # 'file:/store/user/yoshin/EmJetMC/AODSIM/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-2017-04-30-test/170430_233938/0000/aodsim_13.root',
        # 'file:/store/user/yoshin/EmJetMC/AODSIM/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-2017-04-30-test/170430_233938/0000/aodsim_16.root',
        # 'file:/store/user/yoshin/EmJetMC/AODSIM/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-2017-04-30-test/170430_233938/0000/aodsim_17.root',
        # 'file:/store/user/yoshin/EmJetMC/AODSIM/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-2017-04-30-test/170430_233938/0000/aodsim_18.root',
        # 'file:/store/user/yoshin/EmJetMC/AODSIM/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-2017-04-30-test/170430_233938/0000/aodsim_19.root',
        # 'file:/store/user/yoshin/EmJetMC/AODSIM/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-2017-04-30-test/170430_233938/0000/aodsim_20.root',
        # 'file:/store/user/yoshin/EmJetMC/AODSIM/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-2017-04-30-test/170430_233938/0000/aodsim_21.root',
        # 'file:/store/user/yoshin/EmJetMC/AODSIM/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-2017-04-30-test/170430_233938/0000/aodsim_22.root',
        # 'file:/store/user/yoshin/EmJetMC/AODSIM/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-2017-04-30-test/170430_233938/0000/aodsim_23.root',
        # 'file:/store/user/yoshin/EmJetMC/AODSIM/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-2017-04-30-test/170430_233938/0000/aodsim_24.root',
        # 'file:/store/user/yoshin/EmJetMC/AODSIM/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-2017-04-30-test/170430_233938/0000/aodsim_26.root',
        # 'file:/store/user/yoshin/EmJetMC/AODSIM/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-2017-04-30-test/170430_233938/0000/aodsim_27.root',
        # 'file:/store/user/yoshin/EmJetMC/AODSIM/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-2017-04-30-test/170430_233938/0000/aodsim_29.root',
        # 'file:/store/user/yoshin/EmJetMC/AODSIM/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-2017-04-30-test/170430_233938/0000/aodsim_30.root',
        # 'file:/store/user/yoshin/EmJetMC/AODSIM/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-2017-04-30-test/170430_233938/0000/aodsim_4.root',
        # 'file:/store/user/yoshin/EmJetMC/AODSIM/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/AODSIM-2017-04-30-test/170430_233938/0000/aodsim_8.root',
        # 80X WJet MC
        # '/store/mc/RunIISummer16DR80Premix/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/AODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/120000/023963AD-CBBE-E611-AC12-D4AE526A048B.root',
        # 80X Data
        # '/store/data/Run2016B/JetHT/AOD/23Sep2016-v1/90000/0024BC8F-AC82-E611-9019-001E675817A4.root'
        # 80X QCD
        # '/store/mc/RunIISummer16DR80Premix/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/AODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/110000/0075291F-AAB4-E611-A618-A0000420FE80.root'
        # '/store/mc/RunIISummer16DR80Premix/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/AODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/110000/007BFA96-C3B0-E611-90E0-047D7BD6DD64.root'
        # '/store/mc/RunIISummer16DR80Premix/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/AODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/110000/007BFA96-C3B0-E611-90E0-047D7BD6DD64.root'
        # ModelA AODSIM-v2017-05-02
        # 'file:/store/user/yoshin/EmJetMC/AODSIM/v2017-05-02/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/RunIISummer16DR80Premix_private-AODSIM-v2017-05-02/170503_203019/0000/aodsim_1.root',
        # 'file:/store/user/yoshin/EmJetMC/AODSIM/v2017-05-02/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/RunIISummer16DR80Premix_private-AODSIM-v2017-05-02/170503_203019/0000/aodsim_10.root',
        # 'file:/store/user/yoshin/EmJetMC/AODSIM/v2017-05-02/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/RunIISummer16DR80Premix_private-AODSIM-v2017-05-02/170503_203019/0000/aodsim_11.root',
        # 'file:/store/user/yoshin/EmJetMC/AODSIM/v2017-05-02/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/RunIISummer16DR80Premix_private-AODSIM-v2017-05-02/170503_203019/0000/aodsim_12.root',
        # 'file:/store/user/yoshin/EmJetMC/AODSIM/v2017-05-02/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/RunIISummer16DR80Premix_private-AODSIM-v2017-05-02/170503_203019/0000/aodsim_13.root',
        # 'file:/store/user/yoshin/EmJetMC/AODSIM/v2017-05-02/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/RunIISummer16DR80Premix_private-AODSIM-v2017-05-02/170503_203019/0000/aodsim_14.root',
        # 'file:/store/user/yoshin/EmJetMC/AODSIM/v2017-05-02/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/RunIISummer16DR80Premix_private-AODSIM-v2017-05-02/170503_203019/0000/aodsim_15.root',
        # 'file:/store/user/yoshin/EmJetMC/AODSIM/v2017-05-02/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/RunIISummer16DR80Premix_private-AODSIM-v2017-05-02/170503_203019/0000/aodsim_16.root',
        # 'file:/store/user/yoshin/EmJetMC/AODSIM/v2017-05-02/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/RunIISummer16DR80Premix_private-AODSIM-v2017-05-02/170503_203019/0000/aodsim_17.root',
        # 'file:/store/user/yoshin/EmJetMC/AODSIM/v2017-05-02/EmergingJets_mass_X_d_1000_mass_pi_d_2_tau_pi_d_5_TuneCUETP8M1_13TeV_pythia8Mod/RunIISummer16DR80Premix_private-AODSIM-v2017-05-02/170503_203019/0000/aodsim_18.root',
        # DarkPionGun events with pileup
        # 'file:/home/yhshin/EmJetMCProd/CMSSW_8_0_21/src/EmJetDigiReco/aodsim-DarkPionGun.root'
        # 'file:/home/yhshin/EmJetMCProd/CMSSW_8_0_21/src/EmJetDigiReco/aodsim-DarkPionGun-1k.root'
        # DarkPionGun events with no pileup
        # 'file:/home/yhshin/EmJetMCProd/CMSSW_8_0_21/src/EmJetDigiReco/aodsim-DarkPionGun-NoPileUp.root'
    ),
)

producePdfWeights = 0
if producePdfWeights:
# if options.data==0:
    # Produce PDF weights (maximum is 3)
    process.pdfWeights = cms.EDProducer("PdfWeightProducer",
        # Fix POWHEG if buggy (this PDF set will also appear on output,
        # so only two more PDF sets can be added in PdfSetNames if not "")
        #FixPOWHEG = cms.untracked.string("cteq66.LHgrid"),
        #GenTag = cms.untracked.InputTag("genParticles"),
        PdfInfoTag = cms.untracked.InputTag("generator"),
        PdfSetNames = cms.untracked.vstring(
                "CT14nlo.LHgrid",
                "NNPDF30_nlo_as_0118.LHgrid",
                "NNPDF23_lo_as_0130_qed.LHgrid",
                # , "MRST2006nnlo.LHgrid"
                # , "NNPDF10_100.LHgrid"
        )
    )


process.TFileService = cms.Service("TFileService", fileName = cms.string('ntuple.root') )
# process.TFileService = cms.Service("TFileService", fileName = cms.string("ntuple-74X-DRtest-20170425.root") )
# process.TFileService = cms.Service("TFileService", fileName = cms.string("ntuple-74X-QCD-20170425.root") )

testVertexReco = 0
if testVertexReco:
    # addEdmOutput(process, options.data, options.sample)
    # Keep all objects created by emJetAnalyzer
    process.out.outputCommands.extend(cms.untracked.vstring('keep *_emJetAnalyzer_*_*',))
    # Keep genParticles
    process.out.outputCommands.extend(cms.untracked.vstring('keep *_genParticles_*_*',))

doSkimHlt = 1
if doSkimHlt:
    process.emJetAnalyzer.saveTracks = cms.bool(False)
    process.triggerSelection.triggerConditions = cms.vstring(
        # 'HLT_PFHT475_v*', # Old
        'HLT_IsoMu24_v*',
    )

if options.outputLabel:
    process.out.fileName = cms.untracked.string('output-%s.root' % options.outputLabel)
    process.TFileService.fileName = cms.string('ntuple-%s.root' % options.outputLabel)

# storage
process.outpath = cms.EndPath(process.out)

process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
# # Needed for GetTrackTrajInfo
# process.load("RecoTracker.Configuration.RecoTracker_cff")

# process.load('Configuration.StandardSequences.Reconstruction_cff') #new for navigation
# process.load('Configuration.StandardSequences.GeometryExtended_cff') #new for navigation
# # process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff') #new for navigation
# # process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
# # process.load('JetMETCorrections.Configuration.CorrectedJetProducers_cff')
# # # #get the jet energy corrections from the db file
# # process.load("CondCore.CondDB.CondDB_cfi") 
