"""MultiCRAB script template for EmergingJet analysis"""
MC=0
DATA=1

jobname = 'Skim-20170303'        # Jobname
psetname = 'Configuration/test/test_cfg.py'      # Path to pset
postfix = 'v1'                # Postfix to job, increment for each major version
dryrun = 0

from datasets import dataset
from wrappers import submit, submit_newthread
datasets = [
    # dataset( "Run2016B-v1" , "/JetHT/Run2016B-23Sep2016-v1/AOD" , DATA , 10 , 1000000 , splitting='LumiBased' , priority=10 , label='signal' ) ,
    dataset( "Run2016B-v3" , "/JetHT/Run2016B-23Sep2016-v3/AOD" , DATA ,  10 , 1000000 , splitting='LumiBased' , priority=10 , label='signal' ) ,
    # dataset( "Run2016C"    , "/JetHT/Run2016C-23Sep2016-v1/AOD" , DATA , 10 , 1000000 , splitting='LumiBased' , priority=10 , label='signal' ) ,
    # dataset( "Run2016D"    , "/JetHT/Run2016D-23Sep2016-v1/AOD" , DATA , 10 , 1000000 , splitting='LumiBased' , priority=10 , label='signal' ) ,
    # dataset( "Run2016E"    , "/JetHT/Run2016E-23Sep2016-v1/AOD" , DATA , 10 , 1000000 , splitting='LumiBased' , priority=10 , label='signal' ) ,
    # dataset( "Run2016F"    , "/JetHT/Run2016F-23Sep2016-v1/AOD" , DATA , 10 , 1000000 , splitting='LumiBased' , priority=10 , label='signal' ) ,
    # dataset( "Run2016G"    , "/JetHT/Run2016G-23Sep2016-v1/AOD" , DATA , 10 , 1000000 , splitting='LumiBased' , priority=10 , label='signal' ) ,
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
    config.Site.blacklist = ['T2_US_Purdue']
    # config.Site.whitelist = ['T2_CH_CERN']
    # config.Site.ignoreGlobalBlacklist = True

    ############################################################
    ## Dataset specific settings
    ############################################################
    for dataset in datasets:
        alias = dataset.alias
        pyCfgParams = ['crab=1'] # Temporary list, must be written to config.JobType.pyCfgParams before submitting
        config.General.requestName   = ('%s-%s' + time.strftime("-%Y-%m%d-%H%M%S")) % (jobname, alias)
        config.Data.outLFNDirBase = '/store/user/yoshin/EmJetSkim/%s-%s/%s/' % (jobname, postfix, alias)
        config.Data.inputDataset = dataset.fullpath
        config.Data.unitsPerJob  = dataset.unitsPerJob
        config.Data.totalUnits   = dataset.totalUnits
        isData                   = dataset.isData
        config.Data.splitting    = dataset.splitting
        config.JobType.priority  = dataset.priority
        config.Data.inputDBS     = dataset.inputDBS
        label                    = dataset.label
        # MC specific settings:
        if not isData:
            pyCfgParams.append('data=0')
        # Data specific settings:
        if isData:
            pyCfgParams.append('data=1')
            config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt'
        pyCfgParams.append('steps=skim')
        # Label specific settings
        pyCfgParams.append('sample='+label)
        pyCfgParams.append('doHLT=1')
        pyCfgParams.append('doJetFilter=0')
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

