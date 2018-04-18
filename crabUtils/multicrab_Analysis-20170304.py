"""MultiCRAB script template for EmergingJet analysis"""
MC=0
DATA=1

jobname = 'Analysis-20170304'        # Jobname
psetname = 'Configuration/test/test_cfg.py'      # Path to pset
postfix = 'v1'                # Postfix to job, increment for each major version
dryrun = 0

from datasets import dataset
from wrappers import submit, submit_newthread

# Import short name for datasets
from datasets_AODSIM_2017_02_20 import *

QCD_HT50to100    = "/QCD_HT50to100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM"
QCD_HT100to200   = "/QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM"
QCD_HT200to300   = "/QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM"
QCD_HT300to500   = "/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM"
QCD_HT500to700   = "/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM"
QCD_HT700to1000  = "/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM"
QCD_HT1000to1500 = "/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM"
QCD_HT1500to2000 = "/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM"
QCD_HT2000toInf  = "/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM"

TTbar            = "/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM"
WJet             = "/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16DR80Premix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/AODSIM"

datasets = [
    # def __init__(self, alias, fullpath, isData, unitsPerJob=1, totalUnits=1, splitting='FileBased', priority=1, inputDBS='global', label='', doHLT=1, doJetFilter=0):
    # dataset( "Dummy" , Dummy , MC , 1 , 10000 , splitting='FileBased' , priority=99 , label='signal' ) ,
    dataset( "mass_pi_d_1_tau_pi_d_0p001"  , mass_pi_d_1_tau_pi_d_0p001  , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_1_tau_pi_d_0p1"    , mass_pi_d_1_tau_pi_d_0p1    , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_1_tau_pi_d_1"      , mass_pi_d_1_tau_pi_d_1      , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_1_tau_pi_d_5"      , mass_pi_d_1_tau_pi_d_5      , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_1_tau_pi_d_25"     , mass_pi_d_1_tau_pi_d_25     , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_1_tau_pi_d_60"     , mass_pi_d_1_tau_pi_d_60     , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_1_tau_pi_d_100"    , mass_pi_d_1_tau_pi_d_100    , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_1_tau_pi_d_150"    , mass_pi_d_1_tau_pi_d_150    , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_1_tau_pi_d_300"    , mass_pi_d_1_tau_pi_d_300    , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_2_tau_pi_d_0p001"  , mass_pi_d_2_tau_pi_d_0p001  , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_2_tau_pi_d_0p1"    , mass_pi_d_2_tau_pi_d_0p1    , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_2_tau_pi_d_1"      , mass_pi_d_2_tau_pi_d_1      , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_2_tau_pi_d_5"      , mass_pi_d_2_tau_pi_d_5      , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_2_tau_pi_d_25"     , mass_pi_d_2_tau_pi_d_25     , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_2_tau_pi_d_60"     , mass_pi_d_2_tau_pi_d_60     , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_2_tau_pi_d_100"    , mass_pi_d_2_tau_pi_d_100    , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_2_tau_pi_d_150"    , mass_pi_d_2_tau_pi_d_150    , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_2_tau_pi_d_300"    , mass_pi_d_2_tau_pi_d_300    , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_5_tau_pi_d_0p001"  , mass_pi_d_5_tau_pi_d_0p001  , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_5_tau_pi_d_0p1"    , mass_pi_d_5_tau_pi_d_0p1    , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_5_tau_pi_d_1"      , mass_pi_d_5_tau_pi_d_1      , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_5_tau_pi_d_5"      , mass_pi_d_5_tau_pi_d_5      , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_5_tau_pi_d_25"     , mass_pi_d_5_tau_pi_d_25     , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_5_tau_pi_d_60"     , mass_pi_d_5_tau_pi_d_60     , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_5_tau_pi_d_100"    , mass_pi_d_5_tau_pi_d_100    , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_5_tau_pi_d_150"    , mass_pi_d_5_tau_pi_d_150    , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_5_tau_pi_d_300"    , mass_pi_d_5_tau_pi_d_300    , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_10_tau_pi_d_0p001" , mass_pi_d_10_tau_pi_d_0p001 , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_10_tau_pi_d_0p1"   , mass_pi_d_10_tau_pi_d_0p1   , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_10_tau_pi_d_1"     , mass_pi_d_10_tau_pi_d_1     , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_10_tau_pi_d_5"     , mass_pi_d_10_tau_pi_d_5     , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_10_tau_pi_d_25"    , mass_pi_d_10_tau_pi_d_25    , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_10_tau_pi_d_60"    , mass_pi_d_10_tau_pi_d_60    , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_10_tau_pi_d_100"   , mass_pi_d_10_tau_pi_d_100   , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_10_tau_pi_d_150"   , mass_pi_d_10_tau_pi_d_150   , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "mass_pi_d_10_tau_pi_d_300"   , mass_pi_d_10_tau_pi_d_300   , MC , 1 , 10000 , splitting='FileBased' , priority=99 , inputDBS='phys03' , label='signal' , doHLT=0 ) ,
    dataset( "QCD_HT50to100"               , QCD_HT50to100               , MC , 1 , 10000 , splitting='FileBased' , priority=10 , label='signal' )  ,
    dataset( "QCD_HT100to200"              , QCD_HT100to200              , MC , 1 , 10000 , splitting='FileBased' , priority=10 , label='signal' )  ,
    dataset( "QCD_HT200to300"              , QCD_HT200to300              , MC , 1 , 10000 , splitting='FileBased' , priority=10 , label='signal' )  ,
    dataset( "QCD_HT300to500"              , QCD_HT300to500              , MC , 1 , 10000 , splitting='FileBased' , priority=10 , label='signal' )  ,
    dataset( "QCD_HT500to700"              , QCD_HT500to700              , MC , 1 , 10000 , splitting='FileBased' , priority=10 , label='signal' )  ,
    dataset( "QCD_HT700to1000"             , QCD_HT700to1000             , MC , 1 , 10000 , splitting='FileBased' , priority=80 , label='signal' )  ,
    dataset( "QCD_HT1000to1500"            , QCD_HT1000to1500            , MC , 1 , 10000 , splitting='FileBased' , priority=90 , label='signal' )  ,
    dataset( "QCD_HT1500to2000"            , QCD_HT1500to2000            , MC , 1 , 10000 , splitting='FileBased' , priority=80 , label='signal' )  ,
    dataset( "QCD_HT2000toInf"             , QCD_HT2000toInf             , MC , 1 , 10000 , splitting='FileBased' , priority=70 , label='signal' )  ,
]

import os
crabTaskDir = 'crabTasks'
if not os.path.exists(crabTaskDir):
    os.makedirs(crabTaskDir)

if __name__ == '__main__':

    ############################################################
    ## Common settings
    ############################################################
    from WMCore.Configuration import Configuration
    import time
    config = Configuration()

    config.section_("General")
    config.General.workArea = crabTaskDir + '/' + 'crab_' + jobname + time.strftime("-%Y-%m%d") + '-' + postfix
    tasklistFileName = config.General.workArea + '.txt'
    if not dryrun: tasklistFile = open(tasklistFileName, 'a')

    config.section_("JobType")
    config.JobType.pluginName = 'Analysis'
    config.JobType.psetName = psetname
    # config.JobType.scriptExe = 'crab_script.sh'                                # Script to run instead of cmsRun
    # config.JobType.inputFiles = ['emjet-basicJetAnalyzer.py','crab_script.py'] # Additional input files
    # config.JobType.outputFiles = ['histo.root']                                # Collect non-EDM outputs if any

    config.section_("Data")
    config.Data.splitting = 'FileBased'
    config.Data.publication = True
    config.Data.outputDatasetTag = jobname
    # config.Data.ignoreLocality = True

    config.section_("Site")
    config.Site.storageSite = "T3_US_UMD"
    # config.Site.blacklist = ['T2_US_Purdue']
    # config.Site.whitelist = ['T2_CH_CERN']
    # config.Site.ignoreGlobalBlacklist = True

    ############################################################
    ## Dataset specific settings
    ############################################################
    for dataset in datasets:
        alias = dataset.alias
        pyCfgParams = ['crab=1'] # Temporary list, must be written to config.JobType.pyCfgParams before submitting
        config.General.requestName   = ('%s-%s' + time.strftime("-%Y-%m%d-%H%M%S")) % (jobname, alias)
        config.Data.outLFNDirBase = '/store/user/yoshin/EmJetAnalysis/%s-%s/%s/' % (jobname, postfix, alias)
        config.Data.inputDataset = dataset.fullpath
        config.Data.unitsPerJob  = dataset.unitsPerJob
        config.Data.totalUnits   = dataset.totalUnits
        isData                   = dataset.isData
        config.Data.splitting    = dataset.splitting
        config.JobType.priority  = dataset.priority
        config.Data.inputDBS     = dataset.inputDBS
        label                    = dataset.label
        doHLT                    = dataset.doHLT
        doJetFilter              = dataset.doJetFilter
        # MC specific settings:
        if not isData:
            pyCfgParams.append('data=0')
        # Data specific settings:
        if isData:
            pyCfgParams.append('data=1')
            config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt'
        pyCfgParams.append('steps=skim,analyze')
        # Label specific settings
        pyCfgParams.append('sample='+label)
        pyCfgParams.append('doHLT=%d' % doHLT)
        pyCfgParams.append('doJetFilter=%d' % doJetFilter)
        config.JobType.pyCfgParams = pyCfgParams

        if not dryrun:
            res = submit_newthread(config, dryrun=dryrun)
            taskId = res['uniquerequestname'].split(':')[0]
            filePath = '%s%s/%s/%s/' % ( config.Data.outLFNDirBase, config.Data.inputDataset.split('/')[1], config.Data.outputDatasetTag, taskId )
            print 'filePath:'
            print filePath
            tasklistFile.write(filePath)
            tasklistFile.write('\n')

    if not dryrun: tasklistFile.close()

