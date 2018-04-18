"""Helper functions to consolidate different CMSSW configs"""
import FWCore.ParameterSet.Config as cms

def addSkim(process, isData=False, doJetFilter=True, doHLT=False):
    if doHLT:
        print "Applying HLT trigger selection"
        print "triggerSelection should be verified for new datasets."
        process.triggerSelection = cms.EDFilter( "TriggerResultsFilter",
            triggerConditions = cms.vstring(
                # EXO-16-003
                'HLT_HT250_DisplacedDijet40_DisplacedTrack_v*',
                'HLT_HT350_DisplacedDijet40_DisplacedTrack_v*',
                'HLT_HT400_DisplacedDijet40_Inclusive_v*',
                'HLT_HT500_DisplacedDijet40_Inclusive_v*',
                # Data: Run2015*
                # 'HLT_PFHT475_v*',
                # 'HLT_PFHT600_v*',
                # 'HLT_PFHT750_4Jet_v*',
                # 'HLT_PFHT800_v*',
                # MC: RunIISpring15DR74
                #'HLT_PFHT900_v*',
                # Signal region
                'HLT_PFHT800_v*',
                'HLT_PFHT900_v*',
            ),
            hltResults = cms.InputTag( "TriggerResults", "", "HLT" ),
            l1tResults = cms.InputTag( "" ),
            l1tIgnoreMask = cms.bool( False ),
            l1techIgnorePrescales = cms.bool( False ),
            daqPartitions = cms.uint32( 1 ),
            throw = cms.bool( False )
        )
    doFilter = True if doJetFilter else False # doFilter must be boolean type
    process.jetFilter = cms.EDFilter("JetFilter",
        doFilter = cms.bool( doFilter ),
        srcJets = cms.InputTag("ak4PFJetsCHS"),
        # srcJets = cms.InputTag("patJets"),
        # additionalCut = cms.string(""),
        additionalCut = cms.string("abs(eta) < 2.5 && pt > 50.0"),
        jetCuts = cms.VPSet(
            cms.PSet(
                minPt = cms.double(400.0),
                maxEta = cms.double(2.5),
                stringCut = cms.string(""),
                ),
            cms.PSet(
                minPt = cms.double(200.0),
                maxEta = cms.double(2.5),
                stringCut = cms.string(""),
                ),
            cms.PSet(
                minPt = cms.double(125.0),
                maxEta = cms.double(2.5),
                stringCut = cms.string(""),
                ),
            cms.PSet(
                minPt = cms.double(50.0),
                maxEta = cms.double(2.5),
                stringCut = cms.string(""),
                ),
        )
    )
    process.eventCountPreTrigger = cms.EDAnalyzer('EventCounter')
    process.eventCountPreFilter = cms.EDAnalyzer('EventCounter')
    process.eventCountPostFilter = cms.EDAnalyzer('EventCounter')
    if doHLT: # Add triggerSelection
        return cms.Sequence(process.eventCountPreTrigger * process.triggerSelection * process.eventCountPreFilter * process.jetFilter * process.eventCountPostFilter)
    else: # Ignore triggerSelection
        return cms.Sequence(process.eventCountPreTrigger * process.eventCountPreFilter * process.jetFilter * process.eventCountPostFilter)

def addWJetSkim(process, isData=False):
    print "Adding WJet Skim step."
    print "triggerSelection should be verified for new datasets."
    print "electronID should be modified for each global tag."
    process.triggerSelection = cms.EDFilter( "TriggerResultsFilter",
        triggerConditions = cms.vstring(
            # Data: Run2015*
            'HLT_Ele27_eta2p1_WPLoose_Gsf_v*',
            'HLT_Ele27_eta2p1_WPTight_Gsf_v*',
            'HLT_IsoMu24_eta2p1_v*',
            # MC: RunIISpring15DR74
            'HLT_Ele27_eta2p1_WP75_Gsf_v*', # Off at 1.4e34
            'HLT_Ele32_eta2p1_WP75_Gsf_v*',
            # 'HLT_IsoMu24_eta2p1_v*', # same in data
            # 'HLT_IsoMu27_v*', # Good for data and MC, turn off for now for consistency

            # Exists in MC
            # 'HLT_Ele22_eta2p1_WP75_Gsf_v*',
            # 'HLT_IsoMu17_v*',
            # Exists in data, unprescaled
            # 'HLT_IsoMu20_v*',
            #
            # 'HLT_IsoMu22_v*',
            # 'HLT_IsoMu20_eta2p1_v*',
        ),
        hltResults = cms.InputTag( "TriggerResults", "", "HLT" ),
        l1tResults = cms.InputTag( "gtDigis" ),
        l1tIgnoreMask = cms.bool( False ),
        l1techIgnorePrescales = cms.bool( False ),
        daqPartitions = cms.uint32( 1 ),
        throw = cms.bool( False )
    )
    process.wJetFilter = cms.EDFilter("WJetFilter",
        isData = cms.bool( False ),
        srcMuons = cms.InputTag("slimmedMuons"),
        srcElectrons = cms.InputTag("slimmedElectrons"),
        srcJets = cms.InputTag("slimmedJets"),
        srcMET = cms.InputTag("slimmedMETs"),
        minPtMuon = cms.double(20.0),
        minPtElectron = cms.double(20.0),
        minPtMET = cms.double(20.0),
        minMt = cms.double(50.0),
        maxMt = cms.double(100.0),
        minDeltaR = cms.double(0.4),
        maxDeltaPhi = cms.double(0.4), # Doesn't do anything
        minPtSelectedJet = cms.double(20.0),
        maxPtAdditionalJets = cms.double(20.0), # Doesn't do anything
        electronID = cms.string('cutBasedElectronID-Spring15-25ns-V1-standalone-medium'),
        # electronID = cms.string('cutBasedElectronID-CSA14-50ns-V1-standalone-medium'),
    )
    if isData: process.wJetFilter.isData = cms.bool(True)
    process.eventCountPreTrigger = cms.EDAnalyzer('EventCounter')
    process.eventCountPreFilter = cms.EDAnalyzer('EventCounter')
    process.eventCountPostFilter = cms.EDAnalyzer('EventCounter')
    ############################################################
    # Recreate miniAOD
    ############################################################
    if isData:
        # customisation of the process.
        process.load('Configuration.StandardSequences.PAT_cff')
        # Automatic addition of the customisation function from PhysicsTools.PatAlgos.slimming.miniAOD_tools
        from PhysicsTools.PatAlgos.slimming.miniAOD_tools import miniAOD_customizeAllData
        #call to customisation function miniAOD_customizeAllData imported from PhysicsTools.PatAlgos.slimming.miniAOD_tools
        process = miniAOD_customizeAllData(process)
    else:
        # customisation of the process.
        process.load('Configuration.StandardSequences.PATMC_cff')
        # Automatic addition of the customisation function from PhysicsTools.PatAlgos.slimming.miniAOD_tools
        from PhysicsTools.PatAlgos.slimming.miniAOD_tools import miniAOD_customizeAllMC
        #call to customisation function miniAOD_customizeAllMC imported from PhysicsTools.PatAlgos.slimming.miniAOD_tools
        process = miniAOD_customizeAllMC(process)
    ############################################################
    # Ignore trigger selection if MC
    if isData:
        return cms.Sequence(process.eventCountPreTrigger * (process.triggerSelection) * process.eventCountPreFilter * process.wJetFilter * process.eventCountPostFilter)
    else:
        return cms.Sequence(process.eventCountPreTrigger * cms.ignore(process.triggerSelection) * process.eventCountPreFilter * process.wJetFilter * process.eventCountPostFilter)

def addAnalyze(process, isData=False, sample=''):
    addPFHT(process, isData=False, sample='')
    from TrackingTools.TrackAssociator.default_cfi import TrackAssociatorParameterBlock
    process.emergingJetAnalyzer = cms.EDAnalyzer('EmergingJetAnalyzer',
        TrackAssociatorParameterBlock,
        srcJets = cms.InputTag("jetFilter", "selectedJets"),
        isData = cms.untracked.bool(False),
        srcHtMht = cms.InputTag("offlinePFHT"),
    )
    if isData: process.emergingJetAnalyzer.isData = cms.bool( True )
    if sample=='wjet': process.emergingJetAnalyzer.srcJets = cms.InputTag("wJetFilter")
    # Load jet correctors
    process.load('JetMETCorrections.Configuration.JetCorrectors_cff')
    process.emJetAnalyzer = cms.EDFilter('EmJetAnalyzer',
        TrackAssociatorParameterBlock,
        srcJets = cms.InputTag("jetFilter", "selectedJets"),
        isData = cms.bool(False),
        vertexreco = cms.PSet(
            primcut = cms.double( 3.0 ),
            seccut = cms.double( 5.0 ),
            smoothing = cms.bool( True ),
            # weightthreshold = cms.double( 0.0010 ),
            minweight = cms.double( 0.5 ),
            finder = cms.string( "avr" )
        ),
        hlTriggerResults = cms.InputTag("TriggerResults", "", "HLT"),
        jets = cms.untracked.InputTag("ak4CaloJets"),
        associatorVTX = cms.untracked.InputTag("ak4JTAatVX"),
        associatorCALO = cms.untracked.InputTag("ak4JTAatCAL"),
        scanMode = cms.bool(False),
        scanRandomJet = cms.bool(False),
        debug = cms.untracked.bool(False),
        saveTracks = cms.bool(True),
    )
    if isData: process.emJetAnalyzer.isData = cms.bool( True )
    if sample=='wjet'     : process.emJetAnalyzer.srcJets = cms.InputTag("wJetFilter")
    if sample=='recotest' : process.emJetAnalyzer.srcJets = cms.InputTag("ak4PFJetsCHS")

    # return cms.Sequence(process.emergingJetAnalyzer+process.emJetAnalyzer)
    return cms.Sequence(process.emJetAnalyzer)

def addEdmOutput(process, isData=False, sample=''):
    from Configuration.EventContent.EventContent_cff import AODSIMEventContent
    from Configuration.EventContent.EventContent_cff import AODEventContent
    process.out = cms.OutputModule("PoolOutputModule",
        compressionAlgorithm = cms.untracked.string('LZMA'),
        compressionLevel = cms.untracked.int32(4),
        dataset = cms.untracked.PSet(
            dataTier = cms.untracked.string('AODSIM'),
            filterName = cms.untracked.string('')
        ),
        eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
        fileName = cms.untracked.string('output.root'),
        outputCommands = cms.untracked.vstring(),
        SelectEvents = cms.untracked.PSet(
            SelectEvents = cms.vstring('p')
        )
    )
    if isData : process.out.outputCommands.extend(AODEventContent.outputCommands)
    else      : process.out.outputCommands.extend(AODSIMEventContent.outputCommands)
    if sample=='wjet' : process.out.outputCommands.extend(cms.untracked.vstring('keep *_wJetFilter_*_*',))
    else              : process.out.outputCommands.extend(cms.untracked.vstring('keep *_jetFilter_*_*',))

def addPFHT(process, isData=False, sample=''):
    """Copied from http://cmslxr.fnal.gov/dxr/CMSSW/source/HLTrigger/Configuration/python/HLT_25ns14e33_v1_cff.py#27446"""
    process.offlinePFHT = cms.EDProducer( "HLTHtMhtProducer",
        usePt = cms.bool( True ),
        minPtJetHt = cms.double( 40.0 ),
        maxEtaJetMht = cms.double( 999.0 ),
        minNJetMht = cms.int32( 0 ),
        jetsLabel = cms.InputTag( "ak4PFJetsCHS" ), # Changed from "hltAK4PFJetsCorrected"
        maxEtaJetHt = cms.double( 3.0 ),
        minPtJetMht = cms.double( 0.0 ),
        minNJetHt = cms.int32( 0 ),
        pfCandidatesLabel = cms.InputTag( "particleFlow" ), # Changed from "hltParticleFlow"
        excludePFMuons = cms.bool( False )
    )

def addTesting(process, isData=False, sample=''):
    # Jet Track Association
    process.load('RecoJets.JetAssociationProducers.ak4JTA_cff')
    process.emJetAnalyzer.jets = cms.untracked.InputTag('ak4CaloJets')
    process.emJetAnalyzer.associatorVTX = cms.untracked.InputTag("ak4JTAatVX")
    process.emJetAnalyzer.associatorCALO = cms.untracked.InputTag("ak4JTAatCAL")
    process.ak4JTAatVX = cms.EDProducer("JetTracksAssociatorAtVertex",
        tracks = cms.InputTag("generalTracks"),
        coneSize = cms.double(0.4),
        useAssigned = cms.bool(False),
        pvSrc = cms.InputTag("offlinePrimaryVertices"),
        jets = cms.InputTag("ak4CaloJets")
    )
    process.ak4JTAatCAL = cms.EDProducer("JetTracksAssociatorAtCaloFace",
        tracks = cms.InputTag("generalTracks"),
        extrapolations = cms.InputTag("trackExtrapolator"),
        trackQuality = cms.string("goodIterative"),
        coneSize = cms.double(0.4),
        useAssigned = cms.bool(False),
        jets = cms.InputTag("ak4CaloJets")
    )
    return cms.Sequence(process.ak4JTAatVX*process.ak4JTAatCAL)
