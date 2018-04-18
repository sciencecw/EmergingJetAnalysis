// -*- C++ -*-
//
// Package:    EmergingJetAnalysis/EmJetAnalyzer
// Class:      EmJetAnalyzer
//
/**\class EmJetAnalyzer EmJetAnalyzer.cc EmergingJetAnalysis/EmJetAnalyzer/plugins/EmJetAnalyzer.cc

 Description: Analyzer for Emerging Jet analysis, supercedes EmergingJetAnalyzer

 Implementation:
     Re-write of EmergingJetAnalyzer to be more modular.
*/
//
// Original Author:  Young Ho Shin
//         Created:  Tue, 05 Apr 2016 19:37:25 GMT
//
//

// Useful keywords
// :MCONLY: Code that applies only when processing MC events
// :CUT: Code that applies cuts to objects/events
// :EVENTLEVEL: Code that calculates event-level quantities
// :JETLEVEL: Code that calculates jet-level quantities
// :JETTRACKLEVEL: Code that calculates jet-track-level quantities
// :JETSOURCE: Code that assigns "source" variable for a jet
// :TRACKSOURCE: Code that assigns "source" variable for a track
// :VERTEXSOURCE:
// :VERTEXTESTING: Testing for vertex reconstruction
// :GENTRACKMATCHTESTING: Testing for GenParticle-Track matching
// :FIXTRACKHITPATTERNTEST: Testing FixTrackHitPattern


// system include files
#include <memory>
#include <cassert> // For assert()
#include <stdlib.h> // For rand()
#include <math.h> // For asin()
#include <tuple>
#include <iomanip> // std::setprecision

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment4DCollection.h"
#include "DataFormats/CSCRecHit/interface/CSCSegmentCollection.h"
#include "DataFormats/RPCRecHit/interface/RPCRecHitCollection.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/BTauReco/interface/JetTag.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"

#include "TrackingTools/IPTools/interface/IPTools.h"
#include "DataFormats/GeometrySurface/interface/Line.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "MagneticField/Engine/interface/MagneticField.h"

#include "RecoVertex/ConfigurableVertexReco/interface/ConfigurableVertexReconstructor.h"
#include "RecoVertex/TrimmedKalmanVertexFinder/interface/KalmanTrimmedVertexFinder.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexSmoother.h"
#include "RecoVertex/PrimaryVertexProducer/interface/VertexHigherPtSquared.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "TrackingTools/PatternTools/interface/ClosestApproachInRPhi.h"

#include "DataFormats/JetReco/interface/GenJet.h"

// Hit pattern
#include "PhysicsTools/RecoUtils/interface/CheckHitPattern.h"
#include "RecoTracker/DebugTools/interface/FixTrackHitPattern.h"


// track association
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "TrackingTools/TrackAssociator/interface/TrackAssociatorParameters.h"

// Jet Tracks Association
#include "DataFormats/JetReco/interface/JetTracksAssociation.h"

// Track trajectory information
#include "RecoTracker/DebugTools/interface/GetTrackTrajInfo.h"

// HLT
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "TTree.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TStopwatch.h"
#include "TParameter.h"

#include "EmergingJetAnalysis/EmJetAnalyzer/interface/OutputTree.h"
#include "EmergingJetAnalysis/EmJetAnalyzer/interface/EmJetEvent.h"
#include "EmergingJetAnalysis/EmJetAnalyzer/interface/EmJetAlgos.h"
#include "EmergingJetAnalysis/GenParticleAnalyzer/plugins/GenParticleAnalyzer.cc"

// JEC corrections
#include "JetMETCorrections/JetCorrector/interface/JetCorrector.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

// Testing
#include "RecoVertex/PrimaryVertexProducer/interface/TrackFilterForPVFinding.h"

#ifndef OUTPUT
#define OUTPUT(x) std::cout<<#x << ": " << x << std::endl
#endif

#ifndef STDOUT
#define STDOUT(x) std::cout<< x << std::endl
#endif

//
// constants, enums and typedefs
//
typedef std::tuple< std::vector<double>, std::vector<double>, std::vector<double>, std::vector<double>  > DistanceResults;
// bool scanMode_ = true;
// bool scanRandomJet_ = true;
bool jetdump_ = false;

// typedef std::vector<TransientVertex> TransientVertexCollection;

//
// class declaration
//

using namespace emjet;

class GenParticleAnalyzer;

class EmJetAnalyzer : public edm::EDFilter {
  public:
    explicit EmJetAnalyzer(const edm::ParameterSet&);
    ~EmJetAnalyzer();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:
    virtual void beginJob() override;
    virtual bool filter(edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;

    virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
    virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
    //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
    //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

    // n-tuple filling
    void prepareJet(const reco::PFJet& ijet, Jet& ojet, int source, const edm::EventSetup& iSetup);
    void prepareJetTrack(const reco::TransientTrack& itrack, const Jet& ojet, Track& otrack, int source);
    void prepareJetVertex(const TransientVertex& ivertex, const Jet& ojet, Vertex& overtex, int source);
    void prepareJetVertexTrack(const reco::TransientTrack& itrack, const Jet& ojet, Track& otrack, const TransientVertex& ivertex, int source, const edm::EventSetup& iSetup);
    void fillJet(const reco::PFJet& ijet, Jet& ojet);
    void fillJetTrack(const reco::TransientTrack& itrack, const Jet& ojet, Track& otrack);
    void fillJetVertex(const TransientVertex& ivertex, const Jet& ojet, Vertex& overtex);
    bool selectTrack(const reco::TransientTrack& itrack) const;
    bool selectJetTrack(const reco::TransientTrack& itrack, const Jet& ojet, const Track& otrack) const;
    bool selectJetTrackDeltaR(const reco::TransientTrack& itrack, const Jet& ojet) const;
    bool selectJetTrackInnerHit(const reco::TransientTrack& itrack, const Jet& ojet, const edm::EventSetup& iSetup) const;
    bool selectJetTrackForVertexing(const reco::TransientTrack& itrack, const Jet& ojet, const Track& otrack) const;
    bool selectJetVertex(const TransientVertex& ivertex, const Jet& ojet, const Vertex& overtex) const;
    void fillGenParticles () ;
    void fillPrimaryVertices () ;
    vector<reco::TransientTrack> getJetTrackVectorDeltaR() const;


    // EDM output
    void findDarkPionVertices () ;

    // Computation functions
    double compute_alphaMax(const reco::PFJet& ijet, reco::TrackRefVector& trackRefs) const; // To be obsolete
    double compute_alphaMax(reco::TrackRefVector& trackRefs) const;
    double compute_alphaMax(vector<reco::TransientTrack> tracks) const;
    double compute_alpha(reco::TrackRefVector& trackRefs) const;
    double compute_alpha(vector<reco::TransientTrack> tracks) const;
    double compute_alpha_gen(const reco::PFJet& ijet) const;
    double compute_alphaMax_dz(reco::TrackRefVector& trackRefs, double max_dz, double max_dxy) const;
    double compute_alphaMax_dz(vector<reco::TransientTrack> tracks, double max_dz, double max_dxy) const;
    double compute_theta2D(const edm::EventSetup& iSetup) const;
    int    compute_nDarkPions(const reco::PFJet& ijet) const;
    int    compute_nDarkGluons(const reco::PFJet& ijet) const;
    double compute_pt2Sum (const TransientVertex& ivertex) const;
    double compute_alpha_global () const;
    double compute_track_minVertexDz (const reco::TransientTrack& itrack) const;
    double compute_btag(const reco::PFJet& ijet) const;

    // Utility functions
    reco::TrackRefVector MergeTracks(reco::TrackRefVector trks1,  reco::TrackRefVector trks2);
    template <class T>
    T get_median(const vector<T>& input) const;

    // :VERTEXTESTING:
    void vertexdump(DistanceResults) const;

    // Scanning functions (Called for specific events/objects)
    void jetdump(reco::TrackRefVector& trackRefs) const;
    void jetscan(const reco::PFJet& ijet);

    bool triggerfired(const edm::Event& ev, edm::Handle<edm::TriggerResults> triggerResultsHandle_, TString trigname);

    // ----------member data ---------------------------
    bool isData_;
    bool scanMode_;
    bool scanRandomJet_;
    bool debug_;
    bool saveTracks_;

    const edm::EventSetup* eventSetup_; // Pointer to current EventSetup object

    edm::Service<TFileService> fs;
    edm::EDGetTokenT< reco::PFJetCollection > jetCollectionToken_;
    edm::EDGetTokenT<reco::JetCorrector> jetCorrectorToken_;
    edm::EDGetTokenT<edm::View<reco::CaloJet> > jet_collT_;
    edm::EDGetTokenT<reco::JetTracksAssociationCollection> assocVTXToken_;
    edm::EDGetTokenT<reco::JetTracksAssociationCollection> assocCALOToken_;
    edm::EDGetTokenT<edm::TriggerResults> hlTriggerResultsToken_;
    edm::EDGetTokenT<LHERunInfoProduct> lheRunToken_;


    edm::ParameterSet         m_trackParameterSet;
    TrackDetectorAssociator   m_trackAssociator;
    TrackAssociatorParameters m_trackParameters;

    edm::ParameterSet         vtxconfig_;
    ConfigurableVertexReconstructor vtxmaker_;

    emjet::OutputTree otree_ ; // OutputTree object
    TTree* tree_;
    // Histogram objects
    // :GENTRACKMATCHTESTING:
    TH1F* hist_minDistance_RecoToGen_;
    TH1F* hist_minDistance_GenToReco_;
    // :VERTEXTESTING:
    TH1F* hist_LogVertexDistance_GenToReco_;
    TH1F* hist_LogVertexDistance_RecoToGen_;
    TH1F* hist_LogVertexDistance2D_GenToReco_;
    TH1F* hist_LogVertexDistance2D_RecoToGen_;
    // TH1F* hist_VertexEfficiency_;
    // TH1F* hist_VertexPurity_;

    std::auto_ptr< reco::PFJetCollection > scanJet_;
    std::auto_ptr< reco::TrackCollection > scanJetTracks_;
    std::auto_ptr< reco::TrackCollection > scanJetSelectedTracks_;
    std::auto_ptr< reco::VertexCollection > avrVerticesGlobalOutput_;
    std::auto_ptr< reco::VertexCollection > avrVerticesLocalOutput_;
    std::auto_ptr< reco::TrackCollection > avrVerticesRFTracksGlobalOutput_;
    std::auto_ptr< reco::TrackCollection > avrVerticesRFTracksLocalOutput_;
    std::auto_ptr< reco::VertexCollection > darkPionVertices_;
    std::auto_ptr< reco::VertexCollection > tkvfGlobalOutput_;
    std::auto_ptr< reco::VertexCollection > tkvfLocalOutput_;

    emjet:: Event  event_            ; // Current event
    emjet:: Jet    jet_              ; // Current jet
    emjet:: Track  track_            ; // Current track
    emjet:: Vertex vertex_           ; // Current vertex
    emjet:: GenParticle genparticle_ ; // Current genparticle
    emjet:: PrimaryVertex pv_        ; // Current genparticle
    int jet_index_         ; // Current jet index
    int track_index_       ; // Current track index
    int vertex_index_      ; // Current vertex index
    int genparticle_index_ ; // Current genparticle index

    // Retrieve once per event
    // Intermediate objects used for calculations
    edm::Handle<reco::VertexCollection> primary_verticesH_;
    edm::Handle<reco::VertexCollection> primary_vertices_withBS_;
    const reco::Vertex* primary_vertex_;
    edm::Handle<reco::PFJetCollection> selectedJets_;
    edm::Handle<reco::JetCorrector> jetCorrector_;
    edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl_;
    JetCorrectionUncertainty *jecUnc_;
    edm::ESHandle<TransientTrackBuilder> transienttrackbuilderH_;
    std::vector<reco::TransientTrack> generalTracks_;
    edm::Handle<reco::GenParticleCollection> genParticlesH_;
    const reco::BeamSpot* theBeamSpot_;
    reco::VertexCollection selectedSecondaryVertices_;
    std::vector<TransientVertex> avrVertices_;
    edm::Handle<reco::GenJetCollection> genJets_;
    std::vector<GlobalPoint> dt_points_;
    std::vector<GlobalPoint> csc_points_;
    edm::Handle<reco::JetTagCollection> bTagH_;


    // Testing counters
    int pfjet;
    int pfjet_notracks;
    int pfjet_nopv;
    int pfjet_alphazero;
    int pfjet_alphaneg;
    int pfjet_alphazero_total;
    int calojet;
    int calojet_notracks;
    int calojet_nopv;
    int calojet_alphazero;
    int calojet_alphaneg;
    int calojet_alphazero_total;
};

// Declare LHAPDF functions
namespace LHAPDF {
  void initPDFSet(int nset, const std::string& filename, int member=0);
  int numberPDF(int nset);
  void usePDFMember(int nset, int member);
  double xfx(int nset, double x, double Q, int fl);
  double getXmin(int nset, int member);
  double getXmax(int nset, int member);
  double getQ2min(int nset, int member);
  double getQ2max(int nset, int member);
  void extrapolate(bool extrapolate=true);
}

//
// static data member definitions
//

//
// constructors and destructor
//
EmJetAnalyzer::EmJetAnalyzer(const edm::ParameterSet& iConfig):
  // event_  (new Event  ()),
  // jet_    (new Jet    ()),
  // track_  (new Track  ()),
  // vertex_ (new Vertex ())
  jet_collT_ (consumes<edm::View<reco::CaloJet> >(iConfig.getUntrackedParameter<edm::InputTag>("jets"))),
  assocVTXToken_ (consumes<reco::JetTracksAssociationCollection>(iConfig.getUntrackedParameter<edm::InputTag>("associatorVTX"))),
  assocCALOToken_ (consumes<reco::JetTracksAssociationCollection>(iConfig.getUntrackedParameter<edm::InputTag>("associatorCALO"))),
  vtxconfig_(iConfig.getParameter<edm::ParameterSet>("vertexreco")),
  vtxmaker_(vtxconfig_),
  event_       (),
  jet_         (),
  track_       (),
  vertex_      (),
  genparticle_ (),
  pv_ ()
{
  // Config-independent initialization
  {
    // Initialize tree
    std::string modulename = iConfig.getParameter<std::string>("@module_label");
    tree_           = fs->make<TTree>("emJetTree","emJetTree");
    otree_.Branch(tree_);

    // :GENTRACKMATCHTESTING:
    {
      hist_minDistance_RecoToGen_ = fs->make<TH1F> ("RecoToGenTrackDistance", "RecoToGenTrackDistance", 100, -2., 3.);
      hist_minDistance_GenToReco_ = fs->make<TH1F> ("GenToRecoTrackDistance", "GenToRecoTrackDistance", 100, -2., 3.);
    }

    // Secondary vertex reco performance testing :VERTEXTESTING:
    {
      hist_LogVertexDistance_GenToReco_ = fs->make<TH1F>("GenToRecoVertexDistance", "GenToRecoVertexDistance", 100, -4., 4.);
      hist_LogVertexDistance_RecoToGen_ = fs->make<TH1F>("RecoToGenVertexDistance", "RecoToGenVertexDistance", 100, -4., 4.);
      hist_LogVertexDistance2D_GenToReco_ = fs->make<TH1F>("GenToRecoVertexDistance2D", "GenToRecoVertexDistance2D", 100, -4., 4.);
      hist_LogVertexDistance2D_RecoToGen_ = fs->make<TH1F>("RecoToGenVertexDistance2D", "RecoToGenVertexDistance2D", 100, -4., 4.);
      // hist_VertexEfficiency_            = fs->make<TH1F>("VertexEfficiency", "VertexEfficiency", 100, 0., 1.);
      // hist_VertexPurity_                = fs->make<TH1F>("VertexPurity", "VertexPurity", 100, -3., 2.);
    }
  }

  // Config-dependent initialization
  {
    // Important execution switches
    isData_ = iConfig.getParameter<bool>("isData");
    scanMode_ = iConfig.getParameter<bool>("scanMode");
    scanRandomJet_ = iConfig.getParameter<bool>("scanRandomJet");
    debug_ = iConfig.getUntrackedParameter<bool>("debug",false);
    saveTracks_ = iConfig.getParameter<bool>("saveTracks"); // Flag to enable saving of track info in ntuple

    // Save Adaptive Vertex Reco config parameters to tree_->GetUserInfo()
    {
      double primcut = vtxconfig_.getParameter<double>("primcut");
      tree_->GetUserInfo()->AddLast( new TParameter<double> ("primcut", primcut) );
      double seccut = vtxconfig_.getParameter<double>("seccut");
      tree_->GetUserInfo()->AddLast( new TParameter<double> ("seccut", seccut) );
      bool smoothing = vtxconfig_.getParameter<bool>("smoothing");
      tree_->GetUserInfo()->AddLast( new TParameter<bool> ("smoothing", smoothing) );
      double minweight = vtxconfig_.getParameter<double>("minweight");
      tree_->GetUserInfo()->AddLast( new TParameter<double> ("minweight", minweight) );
    }

    m_trackParameterSet = iConfig.getParameter<edm::ParameterSet>("TrackAssociatorParameters");
    if (isData_) {
      std::cout << "running on data" << std::endl;
    } else {
      std::cout << "running on MC"   << std::endl;
    }

    edm::ConsumesCollector iC = consumesCollector();
    m_trackParameters.loadParameters( m_trackParameterSet, iC );
    m_trackAssociator.useDefaultPropagator();

    jetCollectionToken_ = consumes< reco::PFJetCollection > (iConfig.getParameter<edm::InputTag>("srcJets"));
    if (isData_) {
      jetCorrectorToken_  = consumes<reco::JetCorrector>(edm::InputTag("ak4PFCHSL1FastL2L3ResidualCorrector"));
    }
    else {
      jetCorrectorToken_  = consumes<reco::JetCorrector>(edm::InputTag("ak4PFCHSL1FastL2L3Corrector"));
    }

    hlTriggerResultsToken_ = consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag> ("hlTriggerResults"));

    // Register hard-coded inputs
    consumes<reco::BeamSpot> (edm::InputTag("offlineBeamSpot"));
    consumes<reco::TrackCollection> (edm::InputTag("generalTracks"));
    consumes<reco::TrackCollection> (edm::InputTag("displacedStandAloneMuons"));
    consumes<reco::PFMETCollection> (edm::InputTag("pfMet"));
    consumes<reco::VertexCollection> (edm::InputTag("offlinePrimaryVerticesWithBS"));
    consumes<reco::VertexCollection> (edm::InputTag("offlinePrimaryVertices"));
    consumes<reco::VertexCollection> (edm::InputTag("inclusiveSecondaryVertices"));

    consumes<DTRecSegment4DCollection> (edm::InputTag("dt4DSegments"));
    consumes<CSCSegmentCollection> (edm::InputTag("cscSegments"));
    consumes<RPCRecHitCollection> (edm::InputTag("rpcRecHits"));

    consumes<reco::PFCandidateCollection> (edm::InputTag("particleFlow"));
    // B-tag
    consumes<reco::JetTagCollection> (edm::InputTag("pfCombinedInclusiveSecondaryVertexV2BJetTags"));
    consumes<reco::PFJetCollection> (edm::InputTag("ak4PFJetsCHS"));

    if (!isData_) { // :MCONLY:
      consumes<std::vector<PileupSummaryInfo> > (edm::InputTag("addPileupInfo"));
      consumes<std::vector<double> > (edm::InputTag("pdfWeights:CT14nlo"));
      consumes<std::vector<double> > (edm::InputTag("pdfWeights:NNPDF30"));
      consumes<std::vector<double> > (edm::InputTag("pdfWeights:NNPDF23"));
      consumes<GenEventInfoProduct> (edm::InputTag("generator"));
      // // lheRunToken_ = consumes<LHERunInfoProduct> (edm::InputTag("externalLHEProducer"));
      // // consumes<LHERunInfoProduct> (edm::InputTag("externalLHEProducer"));
      // consumes<LHERunInfoProduct> (edm::InputTag("externalLHEProducer"));
      // consumes<LHERunInfoProduct, edm::InRun>({"externalLHEProducer"});
      // consumes<LHEEventProduct> (edm::InputTag("externalLHEProducer"));
      consumes<std::vector<reco::GenMET> > (edm::InputTag("genMetTrue"));
      consumes<reco::GenParticleCollection> (edm::InputTag("genParticles"));
      consumes<reco::GenJetCollection> (edm::InputTag("ak4GenJets"));

    }

    // For scanning jets with alphaMax==0
    {
      produces< reco::PFJetCollection > ("scanJet"). setBranchAlias( "scanJet" ); // scanJet_
      produces< reco::TrackCollection > ("scanJetTracks"). setBranchAlias( "scanJetTracks" ); // scanJetTracks_
      produces< reco::TrackCollection > ("scanJetSelectedTracks"). setBranchAlias( "scanJetSelectedTracks" ); // scanJetSelectedTracks_
      // produces< TransientVertexCollection > ("avrVerticesGlobalOutput"). setBranchAlias( "avrVerticesGlobalOutput" ); // avrVerticesGlobalOutput_
      produces< reco::VertexCollection > ("avrVerticesGlobalOutput"). setBranchAlias( "avrVerticesGlobalOutput" ); // avrVerticesGlobalOutput_
      produces< reco::VertexCollection > ("avrVerticesLocalOutput"). setBranchAlias( "avrVerticesLocalOutput" ); // avrVerticesLocalOutput_
      produces< reco::TrackCollection > ("avrVerticesRFTracksGlobalOutput"). setBranchAlias( "avrVerticesRFTracksGlobalOutput" ); // avrVerticesRFTracksGlobalOutput_
      produces< reco::TrackCollection > ("avrVerticesRFTracksLocalOutput"). setBranchAlias( "avrVerticesRFTracksLocalOutput" ); // avrVerticesRFTracksLocalOutput_
      produces< reco::VertexCollection > ("darkPionVertices"). setBranchAlias( "darkPionVertices" ); // darkPionVertices_
      produces< reco::VertexCollection > ("tkvfGlobalOutput"). setBranchAlias( "tkvfGlobalOutput" ); // tkvfGlobalOutput_
    }

  }
}


EmJetAnalyzer::~EmJetAnalyzer()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}

// HLT trig path acceptance function to protect from out of range issue if trigger name is not found in TriggerResults
// Taken from https://twiki.cern.ch/twiki/pub/CMS/SWGuideCMSDataAnalysisSchool2015HLTExerciseFNAL/TriggerMuMuAnalysis.cc
bool EmJetAnalyzer::triggerfired(const edm::Event& ev, edm::Handle<edm::TriggerResults> TRHandle_, TString trigname){
  const edm::TriggerNames TrigNames_ = ev.triggerNames(*TRHandle_);
  const unsigned int ntrigs = TRHandle_->size();
  for (unsigned int itr=0; itr<ntrigs; itr++){
    TString trigName=TrigNames_.triggerName(itr);
    if (!TRHandle_->accept(itr)) continue;
    if(trigName.Contains(trigname))      return true;
  }
  return false;
}

//
// member functions
//

// ------------ method called on each new Event  ------------
bool
EmJetAnalyzer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // Copy EventSetup pointer to member variable
  eventSetup_ = &iSetup;
  assert(eventSetup_ == &iSetup);
  // Reset output tree to default values
  otree_.Init();
  // Reset output collections
  // Initialize output collections
  scanJet_ = std::auto_ptr< reco::PFJetCollection > ( new reco::PFJetCollection() );
  scanJetTracks_ = std::auto_ptr< reco::TrackCollection > ( new reco::TrackCollection() );
  scanJetSelectedTracks_ = std::auto_ptr< reco::TrackCollection > ( new reco::TrackCollection() );
  avrVerticesGlobalOutput_ = std::auto_ptr< reco::VertexCollection > ( new reco::VertexCollection() );
  avrVerticesLocalOutput_ = std::auto_ptr< reco::VertexCollection > ( new reco::VertexCollection() );
  avrVerticesRFTracksGlobalOutput_ = std::auto_ptr< reco::TrackCollection > ( new reco::TrackCollection() );
  avrVerticesRFTracksLocalOutput_ = std::auto_ptr< reco::TrackCollection > ( new reco::TrackCollection() );
  darkPionVertices_ = std::auto_ptr< reco::VertexCollection > ( new reco::VertexCollection() );
  tkvfGlobalOutput_ = std::auto_ptr< reco::VertexCollection > ( new reco::VertexCollection() );
  tkvfLocalOutput_ = std::auto_ptr< reco::VertexCollection > ( new reco::VertexCollection() );
  // Reset Event variables
  vertex_.Init();
  jet_.Init();
  event_.Init();
  genparticle_.Init();
  pv_.Init();
  // Reset object counters
  jet_index_=0;
  track_index_=0;
  vertex_index_=0;
  genparticle_index_=0;
  // Reset Testing counters
  pfjet = 0;
  pfjet_notracks = 0;
  pfjet_nopv = 0;
  pfjet_alphazero = 0;
  pfjet_alphaneg = 0;
  calojet = 0;
  calojet_notracks = 0;
  calojet_nopv = 0;
  calojet_alphazero = 0;
  calojet_alphaneg = 0;

  event_.run   = iEvent.id().run();
  event_.event = iEvent.id().event();
  event_.lumi  = iEvent.id().luminosityBlock();
  event_.bx    = iEvent.bunchCrossing();

  // Retrieve HLT info
  {
    edm::Handle<edm::TriggerResults> trigResults; //our trigger result object
    iEvent.getByToken(hlTriggerResultsToken_, trigResults);
    if (!trigResults.isValid()) {
      if (debug_) std::cout << "HLT TriggerResults not found!";
      return false;
    }

    const edm::TriggerNames& trigNames = iEvent.triggerNames(*trigResults);

    // EXO-16-003 Triggers
    event_.HLT_HT250 = triggerfired(iEvent,trigResults,"HLT_HT250_DisplacedDijet40_DisplacedTrack");
    event_.HLT_HT350 = triggerfired(iEvent,trigResults,"HLT_HT350_DisplacedDijet40_DisplacedTrack");
    event_.HLT_HT400 = triggerfired(iEvent,trigResults,"HLT_HT400_DisplacedDijet40_Inclusive");
    event_.HLT_HT500 = triggerfired(iEvent,trigResults,"HLT_HT500_DisplacedDijet40_Inclusive");

    // Emerging Jets Analysis Triggers
    event_.HLT_PFHT400 = triggerfired(iEvent,trigResults,"HLT_PFHT400_v");
    event_.HLT_PFHT475 = triggerfired(iEvent,trigResults,"HLT_PFHT475_v");
    event_.HLT_PFHT600 = triggerfired(iEvent,trigResults,"HLT_PFHT600_v");
    event_.HLT_PFHT800 = triggerfired(iEvent,trigResults,"HLT_PFHT800_v");
    event_.HLT_PFHT900 = triggerfired(iEvent,trigResults,"HLT_PFHT900_v");
    // if ( event_.HLT_HT250 || event_.HLT_HT350 || event_.HLT_HT400 || event_.HLT_HT500 || event_.HLT_PFHT400 || event_.HLT_PFHT475 || event_.HLT_PFHT600 || event_.HLT_PFHT800 || event_.HLT_PFHT900 ) {
    //   std::cout << "111111111111111\n";
    //   OUTPUT(event_.HLT_PFHT400);
    // }
    // else {
    //   std::cout << "0\n";
    // }
  }

  // Retrieve offline beam spot (Used to constrain vertexing)
  {
    edm::Handle<reco::BeamSpot> theBeamSpotHandle;
    iEvent.getByLabel("offlineBeamSpot", theBeamSpotHandle);
    theBeamSpot_ = theBeamSpotHandle.product();
  }

  // Calculate basic primary vertex and pileup info :EVENTLEVEL:
  {
    iEvent.getByLabel("offlinePrimaryVertices", primary_verticesH_);
    iEvent.getByLabel("offlinePrimaryVerticesWithBS", primary_vertices_withBS_);
    if (!isData_) { // :MCONLY: Add true number of interactions
      edm::Handle<std::vector<PileupSummaryInfo> > PileupInfo;
      iEvent.getByLabel("addPileupInfo", PileupInfo);
      for (auto const& puInfo : *PileupInfo) {
        int bx = puInfo.getBunchCrossing();
        if (bx == 0) {
          event_.nTrueInt = puInfo.getTrueNumInteractions();
        }
      }
    }
    for (auto ipv = primary_verticesH_->begin(); ipv != primary_verticesH_->end(); ++ipv) {
      event_.nVtx ++;
      if ( (ipv->isFake()) || (ipv->ndof() <= 4.) || (ipv->position().Rho() > 2.0) || (fabs(ipv->position().Z()) > 24.0) ) continue; // :CUT: Primary vertex cut for counting
      event_.nGoodVtx++;
    }

    // // Find leading primary vertex
    // Doesn't work since sortedPVs gets destructed
    // reco::VertexCollection sortedPVs = *primary_verticesH_.product();
    // std::sort(sortedPVs.begin(), sortedPVs.end(), VertexHigherPtSquared());
    // primary_vertex_ = &sortedPVs[0];


    // Find leading primary vertex
    // OUTPUT(event_.event);
    // std::cout << "offlinePrimaryVertices\n";
    double pt2sumMax = 0.;
    int pv_indexInColl = -1, pv_index = 0;
    VertexHigherPtSquared vertexPt2Calculator;
    for (auto ipv = primary_verticesH_->begin(); ipv != primary_verticesH_->end(); ++ipv) {
      double pt2sum = vertexPt2Calculator.sumPtSquared(*ipv);
      if (pt2sum > pt2sumMax) {
        pt2sumMax = pt2sum;
        pv_indexInColl = pv_index;
      }
      // OUTPUT(pt2sum);
      pv_index++;
    }
    if (pv_indexInColl != -1) {
      primary_vertex_ = &( primary_verticesH_->at(pv_indexInColl) );
      // std::cout << "offlinePrimaryVerticesWithBS\n";
      // for (auto ipv = primary_vertices_withBS_->begin(); ipv != primary_vertices_withBS_->end(); ++ipv) {
      //   double pt2sum = vertexPt2Calculator.sumPtSquared(*ipv);
      //   OUTPUT(pt2sum);
      // }

      // // Fill primary vertex information
      // double pt2sum = vertexPt2Calculator.sumPtSquared(*primary_vertex_);
      // double nTracks = primary_vertex_->tracksSize();
      // event_.pv_x           = primary_vertex_->x();
      // event_.pv_y           = primary_vertex_->y();
      // event_.pv_z           = primary_vertex_->z();
      // event_.pv_xError      = primary_vertex_->xError();
      // event_.pv_yError      = primary_vertex_->yError();
      // event_.pv_zError      = primary_vertex_->zError();
      // event_.pv_chi2        = primary_vertex_->chi2();
      // event_.pv_ndof        = primary_vertex_->ndof();
      // event_.pv_pt2sum      = pt2sum;
      // event_.pv_nTracks     = nTracks;
      // event_.pv_indexInColl = pv_indexInColl;
    }
  }

  // Fill PDF information :EVENTLEVEL:
  if (!isData_) { // :MCONLY:
    edm::Handle<GenEventInfoProduct> generatorH_;
    iEvent.getByLabel("generator", generatorH_);
    if (generatorH_->hasPDF()) {
      event_.pdf_id1      = generatorH_->pdf()->id.first;
      event_.pdf_id2      = generatorH_->pdf()->id.second;
      event_.pdf_x1       = generatorH_->pdf()->x.first;
      event_.pdf_x2       = generatorH_->pdf()->x.second;
      event_.pdf_pdf1     = generatorH_->pdf()->xPDF.first;
      event_.pdf_pdf2     = generatorH_->pdf()->xPDF.second;
      event_.pdf_scalePDF = generatorH_->pdf()->scalePDF;
    }
    // std::cout << "EmJetAnalyzer: Q, id1, id2, x1, x2, pdf1, pdf2: \n" <<
    //   event_.pdf_scalePDF << " " <<
    //   event_.pdf_id1 << " " <<
    //   event_.pdf_id2 << " " <<
    //   event_.pdf_x1 << " " <<
    //   event_.pdf_x2 << " " <<
    //   event_.pdf_pdf1 << " " <<
    //   event_.pdf_pdf2 << " " <<
    //   std::endl;
    // edm::Handle<LHEEventProduct> lheEventH;
    // iEvent.getByLabel("externalLHEProducer", lheEventH);

    // Testing PDF weight calculation
  }

  // // Testing PDF weight retrieval
  // double pdfWeight_defaultCentral = 0.;
  // {
  //   edm::InputTag pdfWeightTag("pdfWeights:NNPDF30"); // or any other PDF set
  //   edm::Handle<std::vector<double> > weightHandle;
  //   iEvent.getByLabel(pdfWeightTag, weightHandle);

  //   std::vector<double> weights = (*weightHandle);
  //   std::cout << "Event weight for central PDF NNPDF30_nlo_as_0118:" << weights[0] << std::endl;
  //   pdfWeight_defaultCentral = weights[0];
  //   unsigned int nmembers = weights.size();
  //   for (unsigned int j=1; j<nmembers && j<11; j+=2) {
  //     // std::cout << "Event weight for PDF variation +" << (j+1)/2 << ": " << weights[j] << std::endl;
  //     // std::cout << "Event weight for PDF variation -" << (j+1)/2 << ": " << weights[j+1] << std::endl;
  //     // std::cout << "Event weight for PDF variation + (relative to central PDF)" << (j+1)/2 << ": " << weights[j]  / weights[0] << std::endl;
  //     // std::cout << "Event weight for PDF variation - (relative to central PDF)" << (j+1)/2 << ": " << weights[j+1]  / weights[0] << std::endl;
  //   }
  // }
  // {
  //   edm::InputTag pdfWeightTag("pdfWeights:CT14nlo"); // or any other PDF set
  //   edm::Handle<std::vector<double> > weightHandle;
  //   iEvent.getByLabel(pdfWeightTag, weightHandle);

  //   std::vector<double> weights = (*weightHandle);
  //   std::cout << "Event weight for central PDF CT14nlo:" << weights[0] << std::endl;
  //   unsigned int nmembers = weights.size();
  //   for (unsigned int j=1; j<nmembers && j<11; j+=2) {
  //     // std::cout << "Event weight for PDF variation +" << (j+1)/2 << ": " << weights[j] << std::endl;
  //     // std::cout << "Event weight for PDF variation -" << (j+1)/2 << ": " << weights[j+1] << std::endl;
  //     // std::cout << "Event weight for PDF variation + (relative to central PDF)" << (j+1)/2 << ": " << weights[j]  / weights[0] << std::endl;
  //     // std::cout << "Event weight for PDF variation - (relative to central PDF)" << (j+1)/2 << ": " << weights[j+1]  / weights[0] << std::endl;
  //   }
  // }
  // {
  //   edm::InputTag pdfWeightTag("pdfWeights:NNPDF23"); // or any other PDF set
  //   edm::Handle<std::vector<double> > weightHandle;
  //   iEvent.getByLabel(pdfWeightTag, weightHandle);

  //   std::vector<double> weights = (*weightHandle);
  //   std::cout << "Event weight for central PDF NNPDF23:" << weights[0] << std::endl;
  //   unsigned int nmembers = weights.size();
  //   for (unsigned int j=1; j<nmembers && j<11; j+=2) {
  //     std::cout << "Event weight for PDF variation +" << (j+1)/2 << ": " << weights[j] << std::endl;
  //     std::cout << "Event weight for PDF variation -" << (j+1)/2 << ": " << weights[j+1] << std::endl;
  //     // std::cout << "Event weight for PDF variation + (relative to central PDF)" << (j+1)/2 << ": " << weights[j]  / weights[0] << std::endl;
  //     // std::cout << "Event weight for PDF variation - (relative to central PDF)" << (j+1)/2 << ": " << weights[j+1]  / weights[0] << std::endl;
  //   }
  // }

  // Retrieve event level GEN quantities
  if (!isData_) { // :MCONLY:
    // edm::Handle<std::vector<reco::GenMET> > genMetH;
    // iEvent.getByLabel("genMetTrue", genMetH);
    iEvent.getByLabel("genParticles", genParticlesH_);
    // iEvent.getByLabel("ak4GenJets",   genJets_);
    findDarkPionVertices();
  }

  // Calculate MET :EVENTLEVEL:
  {
    edm::Handle<reco::PFMETCollection> pfmet;
    iEvent.getByLabel("pfMet",pfmet);
    event_.met_pt = pfmet->begin()->pt();
    event_.met_phi = pfmet->begin()->phi();
  }

  // Retrieve selectedJets
  edm::Handle<reco::PFJetCollection> pfjetH;
  iEvent.getByToken(jetCollectionToken_, pfjetH);
  selectedJets_ = pfjetH;
  // Retrieve jet correctors
  iEvent.getByToken(jetCorrectorToken_, jetCorrector_);
  // Retrieve JEC uncertainty
  iSetup.get<JetCorrectionsRecord>().get("AK4PFchs",JetCorParColl_);
  JetCorrectorParameters const & JetCorPar = (*JetCorParColl_)["Uncertainty"];
  jecUnc_ = new JetCorrectionUncertainty(JetCorPar);
  // Retrieve b-tag associations
  iEvent.getByLabel("pfCombinedInclusiveSecondaryVertexV2BJetTags", bTagH_);

  // Retrieve TransientTrackBuilder
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",transienttrackbuilderH_);
  // Retrieve generalTracks and build TransientTrackCollection
  edm::Handle<reco::TrackCollection> genTrackH;
  iEvent.getByLabel("generalTracks", genTrackH);
  // Track dump
	// OUTPUT("track dump"); // :DEBUG:
  {
    int itk=0;
    // std::cout << "Dumping tracks\n";
    for ( auto trk = genTrackH->begin(); trk != genTrackH->end(); trk++ ) {
      if (itk==10) break;
      reco::TrackRef trkref(genTrackH, itk);
      bool hasNonZeroWeight = false;
      for (auto ipv = primary_verticesH_->begin(); ipv != primary_verticesH_->end(); ++ipv) {
        double trackWeight = ipv->trackWeight(trkref);
        if (trackWeight>0) hasNonZeroWeight = true;
        // std::cout << "Track weight: " << trackWeight << std::endl;
      } // End of vertex loop
      // OUTPUT(hasNonZeroWeight);
      itk++;
    }

    edm::Handle<reco::PFCandidateCollection> pfcandidatesH;
    iEvent.getByLabel("particleFlow", pfcandidatesH);
    for ( auto cand = pfcandidatesH->begin(); cand != pfcandidatesH->end(); cand++) {
      bool matched = false;
      if (cand->charge()==0) continue;
      auto candtrkref = cand->trackRef();
      for ( auto trk = genTrackH->begin(); trk != genTrackH->end(); trk++ ) {
        reco::TrackRef trkref(genTrackH, itk);
        if (candtrkref==trkref) matched = true;
      }
      // OUTPUT(matched);
    }
  }
  generalTracks_ = transienttrackbuilderH_->build(genTrackH);

  // :GENTRACKMATCHTESTING:
  if (!isData_) //:MCONLY:
  {
    for (auto itk : generalTracks_) {
      auto itrack = itk.track();
      const reco::GenParticle* gp = findMinDistanceGenParticle(genParticlesH_.product(), &itrack);
      if (gp != NULL) {
        double distance = computeGenTrackDistance(gp, &itrack);
        hist_minDistance_RecoToGen_->Fill(TMath::Log10(distance));
      }
      // OUTPUT(distance);
    }
    for (auto gp : *genParticlesH_.product()) {
      const reco::TransientTrack* tk = findMinDistanceTransientTrack(&gp, &generalTracks_);
      if (tk != NULL) {
        double distance = computeGenTrackDistance(&gp, &tk->track());
        hist_minDistance_GenToReco_->Fill(TMath::Log10(distance));
      }
      // OUTPUT(distance);
    }
  }

  // Count number of tracks
  {
    event_.nTracks = generalTracks_.size();
  }

	// Print vertex track info
	{
    edm::ParameterSet TkFilterParameters;
    TkFilterParameters.addParameter<std::string>("algorithm", "filter");
    TkFilterParameters.addParameter<double> ("maxNormalizedChi2", 20.0);
    TkFilterParameters.addParameter<int>("minPixelLayersWithHits", 2);
    TkFilterParameters.addParameter<int> ("minSiliconLayersWithHits", 5);
    TkFilterParameters.addParameter<double> ("maxD0Significance", 5.0);
    TkFilterParameters.addParameter<double> ("minPt", 0.0);
    TkFilterParameters.addParameter<std::string> ("trackQuality", "any");
    auto theTrackFilter= new TrackFilterForPVFinding(TkFilterParameters);
    int ip = 0;
    for (auto ipv = primary_verticesH_->begin(); ipv != primary_verticesH_->end(); ++ipv) {
      // std::cout<<"vertex number "<<ip<<std::endl;
      // std::cout<<"   z       nTracks trksz   isFake  ndof    Rho"<<std::endl;
      // std::cout
      //   <<std::setw(8)<<std::setprecision(4)<<ipv->position().Z()
      //   <<std::setw(8)<<std::setprecision(4)<<ipv->nTracks(0.1)
      //   <<std::setw(8)<<std::setprecision(4)<<ipv->tracksSize()
      //   <<std::setw(8)<<std::setprecision(4)<<ipv->isFake()
      //   <<std::setw(8)<<std::setprecision(4)<<ipv->ndof()
      //   <<std::setw(8)<<std::setprecision(4)<<ipv->position().Rho()
      //   <<std::endl;
      // get tracks with vertex
      // std::cout<<"   with tracks"<<std::endl;
      // std::cout<<"     weight pt    pterror   eta   phi"<<std::endl;
      double pt2sum = 0;
      for(reco::Vertex::trackRef_iterator track=ipv->tracks_begin();track!=ipv->tracks_end();++track) {
        float w        = (*ipv).trackWeight(*track);
        reco::TransientTrack ttk = transienttrackbuilderH_->build(**track);
        bool pass = theTrackFilter->operator()(ttk);
        std::string passfilter = (pass ? "pass" : "fail");
        if (pass) {
          // std::cout<<"  "
          // <<std::setw(8)<<std::setprecision(4)<<w
          // <<std::setw(8)<<std::setprecision(4)<<(*track)->pt()
          // <<std::setw(8)<<std::setprecision(3)<<(*track)->ptError()
          // <<std::setw(8)<<std::setprecision(4)<<(*track)->eta()
          // <<std::setw(8)<<std::setprecision(4)<<(*track)->phi()
          // <<std::setw(8)<<passfilter
          // <<std::endl;
          double pT = (**track).pt();
          double epT=(**track).ptError(); 
          pT=pT>epT ? pT-epT : 0;
          pt2sum += pT*pT;
        }
      }
      // std::cout<<"vertex number "<<ip << "\t";
      // std::cout<<"pt2sum: "<< pt2sum <<std::endl;
      ip++;
    }
	}

  // Test b-tagging
  if(0)
  {
    // Get b tag information
    edm::Handle<reco::JetTagCollection> bTagHandle;
    iEvent.getByLabel("pfCombinedInclusiveSecondaryVertexV2BJetTags", bTagHandle);
    edm::Handle<reco::PFJetCollection> jetH;
    iEvent.getByLabel("ak4PFJetsCHS", jetH);
    const reco::JetTagCollection & bTags = *(bTagHandle.product());
    for (unsigned i = 0; i != bTags.size(); ++i) {
      edm::RefToBase<reco::Jet> obj1 = bTags[i].first;
      TLorentzVector vector1; vector1.SetPxPyPzE(obj1->px(), obj1->py(), obj1->pz(), obj1->energy());
      // cout<<" Jet "<< i
      //     <<" has b tag discriminator = "<<bTags[i].second
      //     << " and obj2 Pt = "<<bTags[i].first->pt()<<endl;
      int nMatched = 0;
      for ( reco::PFJetCollection::const_iterator obj2 = jetH->begin(); obj2 != jetH->end(); obj2++ ) {
        TLorentzVector vector2; vector2.SetPxPyPzE(obj2->px(), obj2->py(), obj2->pz(), obj2->energy());
        double dr = vector1.DeltaR(vector2);
        if (dr < 0.01) {
          nMatched++;
        }
      }
      // if (nMatched!=1) OUTPUT(nMatched);
      // else OUTPUT(bTags[i].second);
    }
  }

  // Reconstruct AVR vertices using all generalTracks passing basic selection
  avrVertices_.clear();
  ConfigurableVertexReconstructor avr (vtxconfig_);
  std::vector<reco::TransientTrack> tracks_for_vertexing;
  for (std::vector<reco::TransientTrack>::iterator itk = generalTracks_.begin(); itk != generalTracks_.end(); ++itk) {
    if (1) {
      if ( selectTrack(*itk) ) // :CUT: Apply basic track selection
        tracks_for_vertexing.push_back(*itk);
    }
    else if (1) {
      reco::Track rtrack = itk->track();
      double distance = 999999;
      const reco::GenParticle* gp = findMinDistanceGenParticle(genParticlesH_.product(), &rtrack);
      if (gp!=NULL) {
        distance = computeGenTrackDistance(gp, &rtrack);
      }
      if ( selectTrack(*itk) && distance < 2.0 ) // :CUT: Apply basic track selection :GENTRACKMATCHTESTING:
      // if ( selectTrack(*itk) && abs(rtrack.vz()) < 1.5 ) // :CUT: Apply basic track selection :GENTRACKMATCHTESTING:
        tracks_for_vertexing.push_back(*itk);
    }
  }
  avrVertices_ = avr.vertices(tracks_for_vertexing);
  for (auto tv : avrVertices_) {
    avrVerticesGlobalOutput_->push_back(reco::Vertex(tv));
    if (tv.hasRefittedTracks()) {
      for (auto rftrk : tv.refittedTracks()) {
        avrVerticesRFTracksGlobalOutput_->push_back(rftrk.track());
      }
    }
  }
  auto result = computeMinVertexDistance(&(*darkPionVertices_), &avrVertices_);
  // vertexdump(result);

  // Calculate Jet-level quantities and fill into jet_ :JETLEVEL:
  for ( reco::PFJetCollection::const_iterator jet = selectedJets_->begin(); jet != selectedJets_->end(); jet++ ) {
    // Fill Jet-level quantities
    prepareJet(*jet, jet_, 1, iSetup); // source = 1 for PF jets :JETSOURCE:

    // Calculate Jet-Track-level quantities and fill into jet_ :JETTRACKLEVEL:
    for (std::vector<reco::TransientTrack>::iterator itk = generalTracks_.begin(); itk != generalTracks_.end(); ++itk) {
      if ( !selectJetTrackDeltaR(*itk, jet_) ) continue; // :CUT: Apply Track selection
      // Fill Jet-Track level quantities
      prepareJetTrack(*itk, jet_, track_, 0); // source = 0 for generalTracks with simple deltaR :TRACKSOURCE:
      fillJetTrack(*itk, jet_, track_);
    }
    for (std::vector<reco::TransientTrack>::iterator itk = generalTracks_.begin(); itk != generalTracks_.end(); ++itk) {
      if ( !selectJetTrack(*itk, jet_, track_) ) continue; // :CUT: Apply Track selection
      // Fill Jet-Track level quantities
      prepareJetTrack(*itk, jet_, track_, 1); // source = 1 for generalTracks :TRACKSOURCE:
      fillJetTrack(*itk, jet_, track_);
    }
    for (std::vector<reco::TransientTrack>::iterator itk = generalTracks_.begin(); itk != generalTracks_.end(); ++itk) {
      if ( !selectJetTrackInnerHit(*itk, jet_, iSetup) ) continue; // :CUT: Apply Track selection
      // Fill Jet-Track level quantities
      prepareJetTrack(*itk, jet_, track_, 5); // source = 5 for generalTracks with inner most hit deltaR :TRACKSOURCE:
      fillJetTrack(*itk, jet_, track_);
    }

    // Per-jet vertex reconstruction
    {
      // Add tracks to be used for vertexing
      std::vector<reco::TransientTrack> tracks_for_vertexing;
      std::vector<reco::TransientTrack> primary_tracks;
      const reco::Vertex& primary_vertex = primary_verticesH_->at(0);
      for(std::vector<reco::TrackBaseRef>::const_iterator iter = primary_vertex.tracks_begin();
          iter != primary_vertex.tracks_end(); iter++) {
        // reco::Track trk = **iter;
        // Cast iter to TrackRef and build into TransientTrack
        reco::TransientTrack ttrk = transienttrackbuilderH_->build(iter->castTo<reco::TrackRef>());
        primary_tracks.push_back(ttrk);
      }
      for (std::vector<reco::TransientTrack>::iterator itk = generalTracks_.begin(); itk != generalTracks_.end(); ++itk) {
        if ( selectJetTrackForVertexing(*itk, jet_, track_) ) // :CUT: Apply Track selection for vertexing
          tracks_for_vertexing.push_back(*itk);
      }

      // Reconstruct vertex from tracks associated with current jet
      std::vector<TransientVertex> vertices_for_current_jet;
      {
        vertices_for_current_jet = vtxmaker_.vertices(primary_tracks, tracks_for_vertexing, *theBeamSpot_);
        // :VERTEXTESTING:
        vector<TransientVertex> vertices_disp;
        for (auto vtx: vertices_for_current_jet) {
          double x = vtx.position().x() - primary_vertex_->position().x();
          double y = vtx.position().y() - primary_vertex_->position().y();
          double z = vtx.position().z() - primary_vertex_->position().z();
          TVector3 vector_to_pv(x, y, z);
          double distance2D_to_pv = vector_to_pv.Perp();
          if (distance2D_to_pv > 1.0) vertices_disp.push_back(vtx);
        }
        reco::VertexCollection darkPionVertices_disp;
        for (auto vtx: *darkPionVertices_) {
          double x = vtx.position().x() - primary_vertex_->position().x();
          double y = vtx.position().y() - primary_vertex_->position().y();
          double z = vtx.position().z() - primary_vertex_->position().z();
          TVector3 vector_to_pv(x, y, z);
          double distance2D_to_pv = vector_to_pv.Perp();
          if (distance2D_to_pv > 1.0) darkPionVertices_disp.push_back(vtx);
        }
        // auto result = computeMinVertexDistance(&(*darkPionVertices_), &vertices);
        auto result = computeMinVertexDistance(&darkPionVertices_disp, &vertices_disp);
        // vertexdump(result);
      }
      for (auto tv : vertices_for_current_jet) {
        avrVerticesLocalOutput_->push_back(reco::Vertex(tv));
        if (tv.hasRefittedTracks()) {
          for (auto rftrk : tv.refittedTracks()) {
            avrVerticesRFTracksLocalOutput_->push_back(rftrk.track());
          }
        }
      }
      // Fill Jet-Vertex level quantities for per-jet vertices
      for (auto vtx : vertices_for_current_jet) {
        if ( !selectJetVertex(vtx, jet_, vertex_) ) continue; // :CUT: Apply Vertex selection
        // Fill Jet-Vertex level quantities
        prepareJetVertex(vtx, jet_, vertex_, 1); // source = 1 for per-jet AVR vertices :VERTEXSOURCE:
        {
          // Fill original tracks from current vertex
          for (auto trk : vtx.originalTracks()) {
            // Fill Jet-Track level quantities (for Tracks from Vertices)
            prepareJetVertexTrack(trk, jet_, track_, vtx, 2, iSetup);
            track_.source = 2; // source = 2 for original tracks from per-jet AVR vertices :TRACKSOURCE:
            // Write current Track to Jet
            jet_.track_vector.push_back(track_);
          }
        }
        if (vtx.hasRefittedTracks()) {
          // Fill refitted tracks from current vertex
          for (auto trk : vtx.refittedTracks()) {
            // Fill Jet-Track level quantities (for Tracks from Vertices)
            prepareJetVertexTrack(trk, jet_, track_, vtx, 3, iSetup);
            // source = 3 for refitted tracks from per-jet AVR vertices :TRACKSOURCE:
            // Write current Track to Jet
            jet_.track_vector.push_back(track_);
          }
        }
        else {
          std::cout << "No refitted tracks for current vertex!\n";
        }
        fillJetVertex(vtx, jet_, vertex_);
      }
    }

    // Fill Jet-Vertex level quantities for globally reconstructed AVR vertices
    for (auto vtx : avrVertices_) {
      if ( !selectJetVertex(vtx, jet_, vertex_) ) continue; // :CUT: Apply Vertex selection
      // Fill Jet-Vertex level quantities
      prepareJetVertex(vtx, jet_, vertex_, 2); // source = 2 for global AVR vertices :VERTEXSOURCE:
      // Write current Vertex to Jet
      jet_.vertex_vector.push_back(vertex_);
      if (vtx.hasRefittedTracks()) {
        // Fill refitted tracks from current vertex
        for (auto trk : vtx.refittedTracks()) {
          // Fill Jet-Track level quantities
          prepareJetVertexTrack(trk, jet_, track_, vtx, 4, iSetup);
          // source = 4 for refitted tracks from global AVR vertices :TRACKSOURCE:
          // Write current Track to Jet
          jet_.track_vector.push_back(track_);
        }
      }
      else {
        std::cout << "No refitted tracks for current vertex!\n";
      }
    }
    fillJet(*jet, jet_);
  }

  // Testing CALO jet association
  if (false)
  {
    edm::Handle<edm::View<reco::CaloJet> > jet_coll;
    edm::Handle<reco::JetTracksAssociationCollection> JetTracksCALO;
    edm::Handle<reco::JetTracksAssociationCollection> JetTracksVTX;
    iEvent.getByToken(jet_collT_, jet_coll);
    iEvent.getByToken(assocCALOToken_, JetTracksCALO);
    iEvent.getByToken(assocVTXToken_, JetTracksVTX);
    for(size_t i=0; i<jet_coll->size()-1; i++){
      edm::RefToBase<reco::CaloJet> jet1_ref = jet_coll->refAt(i);
      if (!(jet1_ref->pt() > 50)) continue; // :CUT: Skip CALO jets with pt < 50
      // Retrieve Jet Tracks Association
      reco::TrackRefVector dijettrks_CALO  = reco::JetTracksAssociation::getValue(*JetTracksCALO, (edm::RefToBase<reco::Jet>)jet1_ref);
      reco::TrackRefVector dijettrks_VTX  = reco::JetTracksAssociation::getValue(*JetTracksVTX, (edm::RefToBase<reco::Jet>)jet1_ref);
      reco::TrackRefVector dijettrks = MergeTracks(dijettrks_CALO, dijettrks_VTX);
      for(size_t j = 0; j<dijettrks.size(); j++){
        const reco::TrackRef trk = dijettrks[j];
        if(!trk->quality(reco::TrackBase::highPurity)) continue;
        if(trk->pt() < 5) continue;
      }
      calojet++;
      double alphaMax = compute_alphaMax(dijettrks);
      if (alphaMax==0) {
        jetdump(dijettrks);
      }

      if (primary_verticesH_->size()==0) calojet_nopv++;
      if (dijettrks.size()==0) calojet_notracks++;
      if (alphaMax==0) calojet_alphazero++;
      if (alphaMax<0) calojet_alphaneg++;
    }
  }

  if (!isData_) { // :MCONLY:
    fillGenParticles();
  }

  fillPrimaryVertices();

  if (jetdump_ && pfjet_alphazero!=0 || pfjet_alphaneg!=0 || calojet_alphazero!=0 || calojet_alphaneg!=0) {
    std::cout << "Event summary:";
    OUTPUT(event_.run);
    OUTPUT(event_.lumi);
    OUTPUT(event_.event);
    OUTPUT(pfjet);
    OUTPUT(pfjet_notracks);
    OUTPUT(pfjet_nopv);
    OUTPUT(pfjet_alphazero);
    OUTPUT(pfjet_alphaneg);
    OUTPUT(calojet);
    OUTPUT(calojet_notracks);
    OUTPUT(calojet_nopv);
    OUTPUT(calojet_alphazero);
    OUTPUT(calojet_alphaneg);
    pfjet_alphazero_total += pfjet_alphazero;
    calojet_alphazero_total += calojet_alphazero;
    std::cout << "\n";
  }

  // Vertex reconstruction testing :VERTEXTESTING:
  {
    KalmanTrimmedVertexFinder finder;
    vector<TransientVertex> vertices = finder.vertices (generalTracks_);
    vector<TransientVertex> vertices_disp;
    for (auto vtx: vertices) {
      double x = vtx.position().x() - primary_vertex_->position().x();
      double y = vtx.position().y() - primary_vertex_->position().y();
      double z = vtx.position().z() - primary_vertex_->position().z();
      TVector3 vector_to_pv(x, y, z);
      double distance2D_to_pv = vector_to_pv.Perp();
      if (distance2D_to_pv > 1.0) vertices_disp.push_back(vtx);
    }
    reco::VertexCollection darkPionVertices_disp;
    for (auto vtx: *darkPionVertices_) {
      double x = vtx.position().x() - primary_vertex_->position().x();
      double y = vtx.position().y() - primary_vertex_->position().y();
      double z = vtx.position().z() - primary_vertex_->position().z();
      TVector3 vector_to_pv(x, y, z);
      double distance2D_to_pv = vector_to_pv.Perp();
      if (distance2D_to_pv > 1.0) darkPionVertices_disp.push_back(vtx);
    }
    // auto result = computeMinVertexDistance(&(*darkPionVertices_), &vertices);
    auto result = computeMinVertexDistance(&darkPionVertices_disp, &vertices_disp);
    vertexdump(result);
    // std::cout << "--------------------------------\n";
    // std::cout << "New event:\n";
    // OUTPUT( primary_vertex_->position().x() );
    // OUTPUT( primary_vertex_->position().y() );
    // OUTPUT( primary_vertex_->position().z() );
    // for (auto tv : vertices) {
    //   std::cout << "\n";
    //   OUTPUT( tv.position().x() );
    //   OUTPUT( tv.position().y() );
    //   OUTPUT( tv.position().z() );
    // }
    // // OUTPUT(vertices.size());
    // // OUTPUT(event_.nVtx);
    // // OUTPUT(event_.nGoodVtx);
    // std::cout << "\n";
    // std::cout << "--------------------------------\n";
    for (auto tv: vertices) {
      tkvfGlobalOutput_->push_back(reco::Vertex(tv));
    }
  }

  // Write current Event to OutputTree
  WriteEventToOutput(event_, &otree_);
  // Write OutputTree to TTree
  tree_->Fill();

#ifdef THIS_IS_AN_EVENT_EXAMPLE
  Handle<ExampleData> pIn;
  iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  ESHandle<SetupData> pSetup;
  iSetup.get<SetupRecord>().get(pSetup);
#endif

  iEvent.put(scanJet_, "scanJet"); // scanJet_
  iEvent.put(scanJetTracks_, "scanJetTracks"); // scanJetTracks_
  iEvent.put(scanJetSelectedTracks_, "scanJetSelectedTracks"); // scanJetSelectedTracks_
  iEvent.put(avrVerticesGlobalOutput_, "avrVerticesGlobalOutput"); // avrVerticesGlobalOutput_
  iEvent.put(avrVerticesLocalOutput_, "avrVerticesLocalOutput"); // avrVerticesLocalOutput_
  iEvent.put(avrVerticesRFTracksGlobalOutput_, "avrVerticesRFTracksGlobalOutput"); // avrVerticesRFTracksGlobalOutput_
  iEvent.put(avrVerticesRFTracksLocalOutput_, "avrVerticesRFTracksLocalOutput"); // avrVerticesRFTracksLocalOutput_
  iEvent.put(darkPionVertices_, "darkPionVertices"); // darkPionVertices_
  iEvent.put(tkvfGlobalOutput_, "tkvfGlobalOutput"); // tkvfGlobalOutput_

  if (scanRandomJet_) return true;
  if (scanMode_) return (pfjet_alphazero>0);
  return true;
}

// ------------ method called once each job just before starting event loop  ------------
void
EmJetAnalyzer::beginJob()
{
  pfjet_alphazero_total = 0;
  calojet_alphazero_total = 0;
}

// ------------ method called once each job just after ending the event loop  ------------
void
EmJetAnalyzer::endJob() {
  OUTPUT(pfjet_alphazero_total);
  OUTPUT(calojet_alphazero_total);
}

// ------------ method called when starting to processes a run  ------------
void
EmJetAnalyzer::beginRun(edm::Run const& iRun, edm::EventSetup const&)
{
  // std::cout << "Start EmJetAnalyzer::beginRun()\n";
  // edm::Handle<LHERunInfoProduct> run;
  // typedef std::vector<LHERunInfoProduct::Header>::const_iterator headers_const_iterator;
  // // iRun.getByToken( lheRunToken_, run );
  // iRun.getByLabel( "externalLHEProducer", run );
  // LHERunInfoProduct myLHERunInfoProduct = *(run.product());
  // for (headers_const_iterator iter=myLHERunInfoProduct.headers_begin(); iter!=myLHERunInfoProduct.headers_end(); iter++){
  //   std::cout << iter->tag() << std::endl;
  //   std::vector<std::string> lines = iter->lines();
  //   for (unsigned int iLine = 0; iLine<lines.size(); iLine++) {
  //     std::cout << lines.at(iLine);
  //   }
  // }
  // std::cout << "End EmJetAnalyzer::beginRun()\n";
}

// ------------ method called when ending the processing of a run  ------------
void
EmJetAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
/*
void
EmJetAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
EmJetAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
EmJetAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

void
EmJetAnalyzer::prepareJet(const reco::PFJet& ijet, Jet& ojet, int source, const edm::EventSetup& iSetup)
{

  pfjet++; // DEBUG
  ojet.Init();
  ojet.index = jet_index_;
  ojet.source = source;
  // Scan one jet at random
  if (scanRandomJet_ && scanJet_->size()==0) {
    if (rand() % 2 == 0) jetscan(ijet);
  }

  // Fill basic kinematic variables
  {
    ojet.ptRaw  = ijet.pt();
    ojet.eta = ijet.eta() ;
    ojet.phi = ijet.phi() ;
    double jec = jetCorrector_->correction(ijet);
    ojet.pt  = ijet.pt() * jec;
    ojet.p4.SetPtEtaPhiM(ojet.pt, ojet.eta, ojet.phi, 0.);
    // Calculate uncertainty
    double ptCor = ojet.pt;
    jecUnc_->setJetEta(ojet.eta);
    jecUnc_->setJetPt(ojet.pt); // here you must use the CORRECTED jet pt
    double unc = jecUnc_->getUncertainty(true);
    // double ptCor_shifted = ptCor(1+shift*unc) ; // shift = +1(up), or -1(down)
    ojet.ptUp = ptCor*(1+unc);
    ojet.ptDown = ptCor*(1-unc);
    // OUTPUT(ojet.pt);
    // OUTPUT(ojet.ptRaw);
    // OUTPUT(ojet.ptUp);
    // OUTPUT(ojet.ptDown);
  }
  // Fill b-tag information
  {
    ojet.csv = compute_btag(ijet);
  }

  // Fill PF Jet specific variables
  {
    ojet.cef = ijet.chargedEmEnergyFraction()     ;
    ojet.nef = ijet.neutralEmEnergyFraction()     ;
    ojet.chf = ijet.chargedHadronEnergyFraction() ;
    ojet.nhf = ijet.neutralHadronEnergyFraction() ;
    ojet.pef = ijet.photonEnergyFraction()        ;
    ojet.mef = ijet.muonEnergyFraction()          ;
  }

  // Fill alphaMax with trackRefs
  {
    reco::TrackRefVector trackRefs = ijet.getTrackRefs();
    ojet.alpha = compute_alpha(trackRefs);
    // ojet.alphaMax = compute_alphaMax(ijet, trackRefs);
    ojet.alphaMax = compute_alphaMax(trackRefs);
    // OUTPUT(ojet.alphaMax);
    ojet.alpha_gen = compute_alpha_gen(ijet);
    ojet.alphaMax_dz100nm = compute_alphaMax_dz(trackRefs, 0.00001, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax_dz200nm = compute_alphaMax_dz(trackRefs, 0.00002, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax_dz500nm = compute_alphaMax_dz(trackRefs, 0.00005, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax_dz1um  = compute_alphaMax_dz(trackRefs, 0.0001, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax_dz2um  = compute_alphaMax_dz(trackRefs, 0.0002, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax_dz5um  = compute_alphaMax_dz(trackRefs, 0.0005, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax_dz10um  = compute_alphaMax_dz(trackRefs, 0.001, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax_dz20um  = compute_alphaMax_dz(trackRefs, 0.002, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax_dz50um  = compute_alphaMax_dz(trackRefs, 0.005, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax_dz100um = compute_alphaMax_dz(trackRefs, 0.01, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax_dz200um = compute_alphaMax_dz(trackRefs, 0.02, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax_dz500um = compute_alphaMax_dz(trackRefs, 0.05, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax_dz1mm   = compute_alphaMax_dz(trackRefs, 0.10, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax_dz2mm   = compute_alphaMax_dz(trackRefs, 0.20, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax_dz5mm   = compute_alphaMax_dz(trackRefs, 0.50, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax_dz1cm   = compute_alphaMax_dz(trackRefs, 1.0, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax_dz2cm   = compute_alphaMax_dz(trackRefs, 2.0, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax_dz5cm   = compute_alphaMax_dz(trackRefs, 5.0, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax_dz10cm   = compute_alphaMax_dz(trackRefs, 10.0, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax_dz20cm   = compute_alphaMax_dz(trackRefs, 20.0, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax_dz50cm   = compute_alphaMax_dz(trackRefs, 50.0, 0.1); // dxy to beam spot < 0.1cm
    if (ojet.alphaMax==0) {
      // jetscan(ijet);
    }
    if (jetdump_ && ojet.alphaMax<=0)
      {
        std::cout << "Dumping jet\n";
        // OUTPUT(event_.run);
        // OUTPUT(event_.lumi);
        // OUTPUT(event_.event);
        // OUTPUT(event_.nTrueInt);
        OUTPUT(primary_verticesH_->size());
        OUTPUT(trackRefs.size());
        // OUTPUT(ojet.pt);
        // OUTPUT(ojet.alphaMax);
        jetdump(trackRefs);
        std::cout << "\n";
      }
    if (primary_verticesH_->size()==0) pfjet_nopv++;
    if (trackRefs.size()==0) pfjet_notracks++;
    if (ojet.alphaMax==0) pfjet_alphazero++;
    if (ojet.alphaMax<0) pfjet_alphaneg++;
  }

  // Fill alphaMax with DeltaR tracks
  {
    auto tracks = getJetTrackVectorDeltaR();
    ojet.alpha2 = compute_alpha(tracks);
    ojet.alphaMax2 = compute_alphaMax(tracks);
    // OUTPUT(ojet.alphaMax2);
    ojet.alphaMax2_dz100nm = compute_alphaMax_dz(tracks, 0.00001, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax2_dz200nm = compute_alphaMax_dz(tracks, 0.00002, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax2_dz500nm = compute_alphaMax_dz(tracks, 0.00005, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax2_dz1um   = compute_alphaMax_dz(tracks, 0.0001, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax2_dz2um   = compute_alphaMax_dz(tracks, 0.0002, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax2_dz5um   = compute_alphaMax_dz(tracks, 0.0005, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax2_dz10um  = compute_alphaMax_dz(tracks, 0.001, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax2_dz20um  = compute_alphaMax_dz(tracks, 0.002, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax2_dz50um  = compute_alphaMax_dz(tracks, 0.005, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax2_dz100um = compute_alphaMax_dz(tracks, 0.01, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax2_dz200um = compute_alphaMax_dz(tracks, 0.02, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax2_dz500um = compute_alphaMax_dz(tracks, 0.05, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax2_dz1mm   = compute_alphaMax_dz(tracks, 0.10, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax2_dz2mm   = compute_alphaMax_dz(tracks, 0.20, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax2_dz5mm   = compute_alphaMax_dz(tracks, 0.50, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax2_dz1cm   = compute_alphaMax_dz(tracks, 1.0, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax2_dz2cm   = compute_alphaMax_dz(tracks, 2.0, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax2_dz5cm   = compute_alphaMax_dz(tracks, 5.0, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax2_dz10cm  = compute_alphaMax_dz(tracks, 10.0, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax2_dz20cm  = compute_alphaMax_dz(tracks, 20.0, 0.1); // dxy to beam spot < 0.1cm
    ojet.alphaMax2_dz50cm  = compute_alphaMax_dz(tracks, 50.0, 0.1); // dxy to beam spot < 0.1cm
  }

  // Fill nDarkPions and nDarkGluons
  {
    ojet.nDarkPions = compute_nDarkPions(ijet);
    ojet.nDarkGluons = compute_nDarkGluons(ijet);
  }

  // Fill theta2D
  {
    ojet.theta2D = compute_theta2D(iSetup);
  }
}

void
EmJetAnalyzer::fillJet(const reco::PFJet& ijet, Jet& ojet)
{
  // Write current Jet to Event
  event_.jet_vector.push_back(ojet);

  jet_index_++;
}

void
EmJetAnalyzer::prepareJetTrack(const reco::TransientTrack& itrack, const Jet& ojet, Track& otrack, int source )
{
  otrack.Init();
  otrack.index = track_index_;
  otrack.source = source;
  otrack.jet_index = jet_index_;
  auto itk = &itrack;
  // Fill basic kinematic variables
  {
    otrack.pt  = itrack.track().pt()  ;
    otrack.eta = itrack.track().eta() ;
    otrack.phi = itrack.track().phi() ;
    otrack.ref_x = itrack.track().vx() ;
    otrack.ref_y = itrack.track().vy() ;
    otrack.ref_z = itrack.track().vz() ;
    otrack.d0Error = itrack.track().d0Error();
    otrack.dzError = itrack.track().dzError();
    otrack.p4.SetPtEtaPhiM(otrack.pt, otrack.eta, otrack.phi, 0.);
  }

  // Fill geometric variables
  {
    const reco::Vertex& primary_vertex = *primary_vertex_;
    GlobalVector direction(ojet.p4.Px(), ojet.p4.Py(), ojet.p4.Pz());
    TrajectoryStateOnSurface pca = IPTools::closestApproachToJet(itk->impactPointState(), primary_vertex, direction, itk->field());
    GlobalPoint closestPoint;
    if (pca.isValid()) {
      closestPoint = pca.globalPosition();
    }
    GlobalVector jetVector = direction.unit();
    Line::PositionType posJet(GlobalPoint(primary_vertex.position().x(),primary_vertex.position().y(),primary_vertex.position().z()));
    Line::DirectionType dirJet(jetVector);
    Line jetLine(posJet, dirJet);
    GlobalVector pcaToJet = jetLine.distance(closestPoint);
    TLorentzVector& trackVector = otrack.p4;
    trackVector.SetPxPyPzE(
                           closestPoint.x() - primary_vertex.position().x(),
                           closestPoint.y() - primary_vertex.position().y(),
                           closestPoint.z() - primary_vertex.position().z(),
                           itk->track().p());
    otrack.dRToJetAxis = trackVector.DeltaR(ojet.p4);
    // Calculate PCA coordinates
    otrack.pca_r   = closestPoint.perp();
    otrack.pca_eta = closestPoint.eta();
    otrack.pca_phi = closestPoint.phi();
    otrack.distanceToJet = pcaToJet.mag();

    // Calculate signed transverse IP, along jet direction
    auto dxy_ipv = IPTools::signedTransverseImpactParameter(*itk, jetVector, primary_vertex);
    auto dxyz_ipv = IPTools::signedImpactParameter3D(*itk, jetVector, primary_vertex);
    otrack.ipXY    = (dxy_ipv.second.value());
    otrack.ipXYSig = (dxy_ipv.second.significance());
    otrack.ip3D    = (dxyz_ipv.second.value());
    otrack.ip3DSig = (dxyz_ipv.second.significance());

    // Calculate hit positions
    TrajectoryStateOnSurface innermost_state;
    {
      const edm::EventSetup& iSetup = *eventSetup_;
      // OUTPUT(eventSetup_);
      // OUTPUT(source);
      // trajectory information for acessing hits
      static GetTrackTrajInfo getTrackTrajInfo;
      std::vector<GetTrackTrajInfo::Result> trajInfo = getTrackTrajInfo.analyze(iSetup, itrack.track());
      // assert(trajInfo.size()>0);
      if(trajInfo.size()>0) {
        innermost_state = trajInfo[0].detTSOS;
        if (innermost_state.isValid()) {
          GlobalPoint innerPos = innermost_state.globalPosition();
          GlobalVector innerPosMom = innermost_state.globalMomentum();
          otrack.innerHit_r = innerPos.perp();
          otrack.innerHit_eta = innerPos.eta();
          otrack.innerHit_phi = innerPos.phi();
        }
      }
    }
  }

  otrack.quality             = itk->track().qualityMask();
  otrack.algo                = itk->track().algo();
  otrack.originalAlgo        = itk->track().originalAlgo();
  otrack.nHits               = itk->numberOfValidHits();
  otrack.nMissInnerHits      = itk->hitPattern().numberOfLostTrackerHits(reco::HitPattern::MISSING_INNER_HITS);
  otrack.nTrkLayers          = itk->hitPattern().trackerLayersWithMeasurement();
  otrack.nMissTrkLayers      = itk->hitPattern().trackerLayersWithoutMeasurement(reco::HitPattern::TRACK_HITS);
  otrack.nMissInnerTrkLayers = itk->hitPattern().trackerLayersWithoutMeasurement(reco::HitPattern::MISSING_INNER_HITS);
  otrack.nMissOuterTrkLayers = itk->hitPattern().trackerLayersWithoutMeasurement(reco::HitPattern::MISSING_OUTER_HITS);
  otrack.nPxlLayers          = itk->hitPattern().pixelLayersWithMeasurement();
  otrack.nMissPxlLayers      = itk->hitPattern().pixelLayersWithoutMeasurement(reco::HitPattern::TRACK_HITS);
  otrack.nMissInnerPxlLayers = itk->hitPattern().pixelLayersWithoutMeasurement(reco::HitPattern::MISSING_INNER_HITS);
  otrack.nMissOuterPxlLayers = itk->hitPattern().pixelLayersWithoutMeasurement(reco::HitPattern::MISSING_OUTER_HITS);

  // :FIXTRACKHITPATTERNTEST:
  static FixTrackHitPattern hits;
  auto result = hits.analyze(*eventSetup_, itk->track());
  // double nMissInnerHits_new = result.innerHitPattern.numberOfLostTrackerHits(reco::HitPattern::MISSING_INNER_HITS);
  // double nMissInnerHits_new = result.innerHitPattern.numberOfHits(reco::HitPattern::MISSING_INNER_HITS);
  // OUTPUT(otrack.nMissInnerHits);
  // OUTPUT(result.innerHitPattern.numberOfValidHits());
  // OUTPUT(result.innerHitPattern.numberOfHits(reco::HitPattern::TRACK_HITS));
  // OUTPUT(result.innerHitPattern.numberOfHits(reco::HitPattern::MISSING_INNER_HITS));
  // OUTPUT(result.innerHitPattern.numberOfHits(reco::HitPattern::MISSING_OUTER_HITS));
  // OUTPUT(nMissInnerHits_new);

  otrack.minVertexDz = compute_track_minVertexDz(itrack);
	if (itk->trackBaseRef().isNull()) {
    // STDOUT("prepareJetTrack: trackBaseRef is null");
    // OUTPUT(otrack.pt);
    // OUTPUT(otrack.eta);
    // OUTPUT(otrack.phi);
    // OUTPUT(otrack.source);
	}
  else {
    otrack.pvWeight = primary_vertex_->trackWeight(itk->trackBaseRef());
  }

  // :GENTRACKMATCHTESTING:
  if (!isData_) { //:MCONLY:
    auto rtrack = itk->track();
    const reco::GenParticle* gp = findMinDistanceGenParticle(genParticlesH_.product(), &rtrack);
    if (gp!=NULL) {
      double distance = computeGenTrackDistance(gp, &rtrack);
      otrack.minGenDistance = distance;
    }
  }

}

void
EmJetAnalyzer::fillJetTrack(const reco::TransientTrack& itrack, const Jet& ojet, Track& otrack)
{
  // Don't save tracks if flag is off
  if (!saveTracks_) return;

  // Write current Track to Jet
  jet_.track_vector.push_back(track_);

  track_index_++;
}


void
EmJetAnalyzer::prepareJetVertexTrack(const reco::TransientTrack& itrack, const Jet& ojet, Track& otrack, const TransientVertex& ivertex, int source, const edm::EventSetup& iSetup)
{
  static CheckHitPattern checkHitPattern;

  prepareJetTrack(itrack, ojet, otrack, source);
  otrack.vertex_index = vertex_index_;
  otrack.vertex_weight = ivertex.trackWeight(itrack);
  bool fixHitPattern = true;
  CheckHitPattern::Result hitInfo = checkHitPattern.analyze(iSetup, itrack.track(), ivertex.vertexState(), fixHitPattern);
  otrack.nHitsInFrontOfVert = hitInfo.hitsInFrontOfVert;
  otrack.missHitsAfterVert  = hitInfo.missHitsAfterVert;
}

void
EmJetAnalyzer::prepareJetVertex(const TransientVertex& ivertex, const Jet& ojet, Vertex& overtex, int source)
{
  overtex.Init();
  overtex.index = vertex_index_;
  overtex.source = source;
  overtex.jet_index = jet_index_;

  auto vtx = reco::Vertex(ivertex);
  const reco::Vertex& primary_vertex = *primary_vertex_;
  TLorentzVector vertexVector;
  vertexVector.SetXYZT(vtx.x(), vtx.y(), vtx.z(), 0.0);
  double deltaR = vertexVector.DeltaR(ojet.p4);
  double Lxy = 0;
  float dx = primary_vertex.position().x() - vtx.position().x();
  float dy = primary_vertex.position().y() - vtx.position().y();
  Lxy = TMath::Sqrt( dx*dx + dy*dy );
  float mass = vtx.p4().mass();
  float pt2sum = compute_pt2Sum(ivertex);
  overtex.x      = ( vtx.x()      );
  overtex.y      = ( vtx.y()      );
  overtex.z      = ( vtx.z()      );
  overtex.xError = ( vtx.xError() );
  overtex.yError = ( vtx.yError() );
  overtex.zError = ( vtx.zError() );
  overtex.deltaR = ( deltaR       );
  overtex.Lxy    = ( Lxy          );
  overtex.mass   = ( mass         );
  overtex.chi2   = ( vtx.chi2()   );
  overtex.ndof   = ( vtx.ndof()   );
  overtex.pt2sum = ( pt2sum       );
}

void
EmJetAnalyzer::fillJetVertex(const TransientVertex& ivertex, const Jet& ojet, Vertex& overtex)
{
  // Write current Vertex to Jet
  jet_.vertex_vector.push_back(vertex_);

  vertex_index_++;
}

bool
EmJetAnalyzer::selectTrack(const reco::TransientTrack& itrack) const
{
  auto itk = &itrack;
  // Skip tracks with pt<1 :CUT:
  if (itk->track().pt() < 1.) return false;
  // Skip tracks failing "high purity" selection
  int quality = itrack.track().qualityMask();
  bool isHighPurity = (quality & 4) > 0;
  if (!isHighPurity) return false;
  return true;
}

bool
EmJetAnalyzer::selectJetTrack(const reco::TransientTrack& itrack, const Jet& ojet, const Track& otrack) const
{
  auto itk = &itrack;
  if (!selectTrack(itrack)) return false; // :CUT: Require track to pass basic selection

  // Skip tracks with invalid point-of-closest-approach :CUT:
  const reco::Vertex& primary_vertex = *primary_vertex_;
  GlobalVector direction(ojet.p4.Px(), ojet.p4.Py(), ojet.p4.Pz());
  TrajectoryStateOnSurface pca = IPTools::closestApproachToJet(itk->impactPointState(), primary_vertex, direction, itk->field());
  if (!pca.isValid()) return false;
  GlobalPoint closestPoint = pca.globalPosition();

  // Skip tracks if point-of-closest-approach has -nan or nan x/y/z coordinates :CUT:
  if ( !( std::isfinite(closestPoint.x()) && std::isfinite(closestPoint.y()) && std::isfinite(closestPoint.z()) ) ) return false;

  // Skip tracks with deltaR > 0.4 w.r.t. current jet :CUT:
  TLorentzVector trackVector;
  trackVector.SetPxPyPzE(
                         closestPoint.x() - primary_vertex.position().x(),
                         closestPoint.y() - primary_vertex.position().y(),
                         closestPoint.z() - primary_vertex.position().z(),
                         itk->track().p());
  float deltaR = trackVector.DeltaR(ojet.p4);
  // if (itrack==1) std::cout << "deltaR: " << deltaR << std::endl;
  if (deltaR > 0.4) return false;
  return true;
}

bool
EmJetAnalyzer::selectJetTrackDeltaR(const reco::TransientTrack& itrack, const Jet& ojet) const
{
  // Gets jet 4 vector from ojet.p4
  auto itk = &itrack;
  if (!selectTrack(itrack)) return false; // :CUT: Require track to pass basic selection

  // Skip tracks with deltaR > 0.4 w.r.t. current jet :CUT:
  TLorentzVector trackVector;
  trackVector.SetPxPyPzE(
                         itk->track().px(),
                         itk->track().py(),
                         itk->track().pz(),
                         itk->track().p());
  float deltaR = trackVector.DeltaR(ojet.p4);
  // if (itrack==1) std::cout << "deltaR: " << deltaR << std::endl;
  if (deltaR > 0.4) return false;

  return true;
}

bool
EmJetAnalyzer::selectJetTrackInnerHit(const reco::TransientTrack& itrack, const Jet& ojet, const edm::EventSetup& iSetup) const
{
  // Gets jet 4 vector from ojet.p4

  if (!selectTrack(itrack)) return false; // :CUT: Require track to pass basic selection
  // Retrieve track position and momentum direction at inner most hit
  // std::cout << "Getting inner most trajectory state\n";
  // TrajectoryStateOnSurface innermost_state = itrack.innermostMeasurementState();
  TrajectoryStateOnSurface innermost_state;
  {
    // trajectory information for acessing hits
    static GetTrackTrajInfo getTrackTrajInfo;
    std::vector<GetTrackTrajInfo::Result> trajInfo = getTrackTrajInfo.analyze(iSetup, itrack.track());
    assert(trajInfo.size()>0);
    innermost_state = trajInfo[0].detTSOS;
    if (!innermost_state.isValid()) {
      return false;
    }
  }
  // std::cout << "Sucessfully got inner most trajectory state\n";
  GlobalPoint innerPosGP = innermost_state.globalPosition();
  GlobalVector innerPosMomGV = innermost_state.globalMomentum();
  TLorentzVector innerPos(innerPosGP.x(), innerPosGP.y(), innerPosGP.z(), 0);
  // Retrieve primary vertex position
  const reco::Vertex& primary_vertex = *primary_vertex_;
  TLorentzVector pvVector(primary_vertex.x(), primary_vertex.y(), primary_vertex.z(), 0);
  TLorentzVector trackVector = (pvVector - innerPos);
  // Skip tracks with deltaR > 0.4 w.r.t. current jet :CUT:
  float deltaR = trackVector.DeltaR(ojet.p4);
  // if (itrack==1) std::cout << "deltaR: " << deltaR << std::endl;
  if (deltaR > 0.4) return false;
  return true;
}

bool
EmJetAnalyzer::selectJetTrackForVertexing(const reco::TransientTrack& itrack, const Jet& ojet, const Track& otrack) const
{
  int quality = itrack.track().qualityMask();
  bool isHighPurity = (quality & 4) > 0;
  if (isHighPurity)
    return selectJetTrackDeltaR(itrack, ojet);
  else
    return false;
}

bool
EmJetAnalyzer::selectJetVertex(const TransientVertex& ivertex, const Jet& ojet, const Vertex& overtex) const
{
  auto vtx = reco::Vertex(ivertex);

  TLorentzVector vertexVector;
  vertexVector.SetXYZT(vtx.x(), vtx.y(), vtx.z(), 0.0);
  double deltaR = vertexVector.DeltaR(ojet.p4);
  if (deltaR > 0.4) return false; // Ignore vertices outside jet cone :CUT:
  return true;
}

void
EmJetAnalyzer::fillGenParticles () {
  for (auto gp = genParticlesH_->begin(); gp != genParticlesH_->end(); ++gp) {
    const reco::Candidate* cand = &(*gp);
    genparticle_.status = gp->status();
    genparticle_.pdgId = gp->pdgId();
    genparticle_.charge = gp->charge();
    genparticle_.mass = gp->mass();
    genparticle_.pt = gp->pt();
    genparticle_.eta = gp->eta();
    genparticle_.phi = gp->phi();
    genparticle_.vx = gp->vx();
    genparticle_.vy = gp->vy();
    genparticle_.vz = gp->vz();
    double Lxy = -1;
    if ( cand->numberOfDaughters()>0 ) {
      const reco::Candidate* dau = cand->daughter(0);
      if (dau) {
        Lxy = TMath::Sqrt( dau->vx()*dau->vx() + dau->vy()*dau->vy() );
      }
    }
    genparticle_.Lxy = Lxy;
    genparticle_.isDark = GenParticleAnalyzer::isDark(cand);
    genparticle_.nDaughters = cand->numberOfDaughters();
    bool hasSMDaughter = false;
    if ( (GenParticleAnalyzer::hasDarkDaughter(cand) == false) && (cand->numberOfDaughters()>1) ) hasSMDaughter = true;
    genparticle_.hasSMDaughter = hasSMDaughter;
    genparticle_.hasDarkMother = GenParticleAnalyzer::hasDarkMother(cand);
    genparticle_.hasDarkPionMother = GenParticleAnalyzer::hasDarkPionMother(cand);
    // if (genparticle_.hasDarkMother && !genparticle_.isDark) std::cout<< "SM particle with Dark mother - status: " << genparticle_.status << "\t pdgId: " <<  (genparticle_.pdgId) << "\t nDau: " << gp->numberOfDaughters() << std::endl;
    bool isTrackable = false;
    int nTrackableDaughters = 0;
    for ( unsigned int i = 0; i != cand->numberOfDaughters(); ++i){
      auto dau = cand->daughter(i);
      if (dau->pt() > 1 && dau->charge()!=0 && !GenParticleAnalyzer::isDark(dau)) nTrackableDaughters++;
    }
    if (nTrackableDaughters>1) isTrackable=true;
    genparticle_.isTrackable = isTrackable;
    // if (cand->pdgId()==4900111) {GenParticleAnalyzer::printParticleMothers(cand->daughter(0));}
    // if (cand->numberOfMothers()>2) {
    //   std::cout<<"GP with more than two mothers\n";
    //   OUTPUT(cand->pdgId());
    //   OUTPUT(cand->status());
    //   OUTPUT(cand->pt());
    //   OUTPUT(cand->numberOfMothers());
    //   GenParticleAnalyzer::printParticleMothers(cand);
    // }
    float min2Ddist = 999999.;
    float min2Dsig  = 999999.;
    float min3Ddist = 999999.;
    float min3Dsig  = 999999.;
    float minDeltaR = 999999.;
    float matched2Ddist = 999999.;
    float matched2Dsig  = 999999.;
    float matched3Ddist = 999999.;
    float matched3Dsig  = 999999.;
    float matchedDeltaR = 999999.;
    // GenParticle vx/vy/vz returns production vertex position, so use first daughter to find decay vertex if it exists
    if (cand->numberOfDaughters()>0) {
      auto decay = cand->daughter(0);
      for (auto vtx: avrVertices_) {
        float dx = decay->vx() - vtx.position().x();
        float dy = decay->vy() - vtx.position().y();
        float dz = decay->vz() - vtx.position().z();
        float exx = vtx.positionError().cxx();
        float eyy = vtx.positionError().cyy();
        float ezz = vtx.positionError().czz();
        float dist2D = TMath::Sqrt( dx*dx + dy*dy );
        float dist3D = TMath::Sqrt( dx*dx + dy*dy + dz*dz );
        float error2D = TMath::Sqrt( exx + eyy );
        float error3D = TMath::Sqrt( exx + eyy + ezz );
        float sig2D = dist2D/error2D;
        float sig3D = dist3D/error3D;
        TLorentzVector decayVector (decay->vx(), decay->vy(), decay->vz(), 0.);
        TLorentzVector vtxVector (vtx.position().x(), vtx.position().y(), vtx.position().z(), 0.);
        float deltaR = decayVector.DeltaR(vtxVector);
        if (dist2D<min2Ddist) {
          min2Ddist = dist2D;
          matched2Ddist = dist2D;
          matched2Dsig = sig2D;
          matched3Ddist = dist3D;
          matched3Dsig = sig3D;
          matchedDeltaR = deltaR;
        }
        if (dist3D<min3Ddist) min3Ddist = dist3D;
        if (sig2D<min2Dsig) min2Dsig = sig2D;
        if (sig3D<min3Dsig) min3Dsig = sig3D;
        if (deltaR<minDeltaR) minDeltaR = deltaR;
      }
    }
    genparticle_.min2Ddist = min2Ddist;
    genparticle_.min2Dsig  = min2Dsig;
    genparticle_.min3Ddist = min3Ddist;
    genparticle_.min3Dsig  = min3Dsig;
    genparticle_.minDeltaR = minDeltaR;
    genparticle_.matched2Ddist = matched2Ddist;
    genparticle_.matched2Dsig  = matched2Dsig;
    genparticle_.matched3Ddist = matched3Ddist;
    genparticle_.matched3Dsig  = matched3Dsig;
    genparticle_.matchedDeltaR = matchedDeltaR;

    event_.genparticle_vector.push_back(genparticle_);
    genparticle_.index++;
  }
}

void
EmJetAnalyzer::fillPrimaryVertices() {
  VertexHigherPtSquared vertexPt2Calculator;
  pv_.index = 0;
  for (auto ipv = primary_verticesH_->begin(); ipv != primary_verticesH_->end(); ++ipv) {
    double pt2sum = vertexPt2Calculator.sumPtSquared(*ipv);
    double nTracks = ipv->tracksSize();
    pv_.x           = ipv->x();
    pv_.y           = ipv->y();
    pv_.z           = ipv->z();
    pv_.xError      = ipv->xError();
    pv_.yError      = ipv->yError();
    pv_.zError      = ipv->zError();
    pv_.chi2        = ipv->chi2();
    pv_.ndof        = ipv->ndof();
    pv_.pt2sum      = pt2sum;
    pv_.nTracks     = nTracks;
    // Fill primary vertex into event
    event_.pv_vector.push_back(pv_);
    pv_.index++;
  }
  // Sort primary vertices in descending order of pt2Sum
  std::sort(event_.pv_vector.begin(), event_.pv_vector.end(), [](const emjet::PrimaryVertex a, const emjet::PrimaryVertex b){ return a.pt2sum > b.pt2sum; });
}

void
EmJetAnalyzer::findDarkPionVertices () {
  for (auto gp = genParticlesH_->begin(); gp != genParticlesH_->end(); ++gp) {
    if (gp->pdgId()==4900111) {
      auto dau = gp->daughter(0);
      double x = dau->vx();
      double y = dau->vy();
      double z = dau->vz();
      reco::Vertex::Error e;
      // e(0, 0) = 0.0015 * 0.0015;
      // e(1, 1) = 0.0015 * 0.0015;
      // e(2, 2) = 15. * 15.;
      reco::Vertex::Point p(x, y, z);
      reco::Vertex dpVertex(p, e, 0, 0, 0);
      darkPionVertices_->push_back(dpVertex);
    }
  }
}

// Calculate jet alphaMax
double
EmJetAnalyzer::compute_alphaMax(const reco::PFJet& ijet, reco::TrackRefVector& trackRefs) const
{
  double alphaMax = -1.;
  // Loop over all tracks and calculate scalar pt-sum of all tracks in current jet
  double jet_pt_sum = 0.;
  for (reco::TrackRefVector::iterator ijt = trackRefs.begin(); ijt != trackRefs.end(); ++ijt) {
    jet_pt_sum += (*ijt)->pt();
  } // End of track loop

  auto ipv_chosen = primary_verticesH_->end(); // iterator to chosen primary vertex
  double max_vertex_pt_sum = 0.; // scalar pt contribution of vertex to jet
  // Loop over all PVs and choose the one with highest scalar pt contribution to jet
  for (auto ipv = primary_verticesH_->begin(); ipv != primary_verticesH_->end(); ++ipv) {
    double vertex_pt_sum = 0.; // scalar pt contribution of vertex to jet
    for (reco::TrackRefVector::iterator ijt = trackRefs.begin(); ijt != trackRefs.end(); ++ijt) {
      double trackWeight = ipv->trackWeight(*ijt);
      if (trackWeight > 0) vertex_pt_sum += (*ijt)->pt();
    } // End of track loop
    if (vertex_pt_sum > max_vertex_pt_sum) {
      max_vertex_pt_sum = vertex_pt_sum;
      ipv_chosen = ipv;
    }
  } // End of vertex loop
  // Calculate alpha
  alphaMax = max_vertex_pt_sum / jet_pt_sum;
  return alphaMax;
}

// Calculate jet alphaMax
double
EmJetAnalyzer::compute_alphaMax(reco::TrackRefVector& trackRefs) const
{
  double alphaMax = -1.;
  // Loop over all tracks and calculate scalar pt-sum of all tracks in current jet
  double jet_pt_sum = 0.;
  for (reco::TrackRefVector::iterator ijt = trackRefs.begin(); ijt != trackRefs.end(); ++ijt) {
    jet_pt_sum += (*ijt)->pt();
  } // End of track loop

  auto ipv_chosen = primary_verticesH_->end(); // iterator to chosen primary vertex
  double max_vertex_pt_sum = 0.; // scalar pt contribution of vertex to jet
  // Loop over all PVs and choose the one with highest scalar pt contribution to jet
  for (auto ipv = primary_verticesH_->begin(); ipv != primary_verticesH_->end(); ++ipv) {
    double vertex_pt_sum = 0.; // scalar pt contribution of vertex to jet
    for (reco::TrackRefVector::iterator ijt = trackRefs.begin(); ijt != trackRefs.end(); ++ijt) {
      double trackWeight = ipv->trackWeight(*ijt);
      if (trackWeight > 0) vertex_pt_sum += (*ijt)->pt();
    } // End of track loop
    if (vertex_pt_sum > max_vertex_pt_sum) {
      max_vertex_pt_sum = vertex_pt_sum;
      ipv_chosen = ipv;
    }
  } // End of vertex loop
  // Calculate alpha
  alphaMax = max_vertex_pt_sum / jet_pt_sum;
  return alphaMax;
}

// Calculate jet alphaMax
double
EmJetAnalyzer::compute_alphaMax(vector<reco::TransientTrack> tracks) const
{
  double alphaMax = -1.;
  // Loop over all tracks and calculate scalar pt-sum of all tracks in current jet
  double jet_pt_sum = 0.;
  for (auto ijt = tracks.begin(); ijt != tracks.end(); ++ijt) {
    jet_pt_sum += (ijt)->track().pt();
  } // End of track loop

  auto ipv_chosen = primary_verticesH_->end(); // iterator to chosen primary vertex
  double max_vertex_pt_sum = 0.; // scalar pt contribution of vertex to jet
  // Loop over all PVs and choose the one with highest scalar pt contribution to jet
  for (auto ipv = primary_verticesH_->begin(); ipv != primary_verticesH_->end(); ++ipv) {
    double vertex_pt_sum = 0.; // scalar pt contribution of vertex to jet
    for (auto ijt = tracks.begin(); ijt != tracks.end(); ++ijt) {
      if ((ijt)->trackBaseRef().isNull()) {
        // trackBaseRef is null
        STDOUT("compute_alpha: trackBaseRef is null");
        continue;
      }
      double trackWeight = ipv->trackWeight((ijt)->trackBaseRef());
      if (trackWeight > 0) vertex_pt_sum += (ijt)->track().pt();
    } // End of track loop
    if (vertex_pt_sum > max_vertex_pt_sum) {
      max_vertex_pt_sum = vertex_pt_sum;
      ipv_chosen = ipv;
    }
  } // End of vertex loop
  // Calculate alpha
  alphaMax = max_vertex_pt_sum / jet_pt_sum;
  return alphaMax;
}

// Calculate jet alpha
double
EmJetAnalyzer::compute_alpha(reco::TrackRefVector& trackRefs) const
{
  double alpha = -1.;
  // Loop over all tracks and calculate scalar pt-sum of all tracks in current jet
  double jet_pt_sum = 0.;
  for (reco::TrackRefVector::iterator ijt = trackRefs.begin(); ijt != trackRefs.end(); ++ijt) {
    jet_pt_sum += (*ijt)->pt();
  } // End of track loop

  const reco::Vertex& primary_vertex = *primary_vertex_;
  double vertex_pt_sum = 0.; // scalar pt contribution of vertex to jet
  for (reco::TrackRefVector::iterator ijt = trackRefs.begin(); ijt != trackRefs.end(); ++ijt) {
    double trackWeight = primary_vertex.trackWeight(*ijt);
    if (trackWeight > 0) vertex_pt_sum += (*ijt)->pt();
  } // End of track loop
  // Calculate alpha
  alpha = vertex_pt_sum / jet_pt_sum;
  return alpha;
}

// Calculate jet alpha
double
EmJetAnalyzer::compute_alpha(vector<reco::TransientTrack> tracks) const
{
  double alpha = -1.;
  // Loop over all tracks and calculate scalar pt-sum of all tracks in current jet
  double jet_pt_sum = 0.;
  for (auto ijt = tracks.begin(); ijt != tracks.end(); ++ijt) {
    jet_pt_sum += (ijt)->track().pt();
  } // End of track loop

  const reco::Vertex& primary_vertex = *primary_vertex_;
  double vertex_pt_sum = 0.; // scalar pt contribution of vertex to jet
  for (auto ijt = tracks.begin(); ijt != tracks.end(); ++ijt) {
    if ((ijt)->trackBaseRef().isNull()) {
      // trackBaseRef is null
      STDOUT("compute_alpha: trackBaseRef is null");
      continue;
    }
    double trackWeight = primary_vertex.trackWeight((ijt)->trackBaseRef());
    if (trackWeight > 0) vertex_pt_sum += (ijt)->track().pt();
  } // End of track loop
  // Calculate alpha
  alpha = vertex_pt_sum / jet_pt_sum;
  return alpha;
}

double
EmJetAnalyzer::compute_alpha_gen(const reco::PFJet& ijet) const
{
  TLorentzVector jetVector;
  jetVector.SetPtEtaPhiM(ijet.pt(),ijet.eta(),ijet.phi(),0.);
  // Calculate pt-sum of charged SM particles coming from prompt vertex vs any vertex
  double prompt_sum = 0.;
  double total_sum  = 0.;
  {
    if (!isData_) {
      for (auto gp = genParticlesH_->begin(); gp != genParticlesH_->end(); ++gp) {
        if ( (gp->status()==1) && (gp->charge()!=0) ) {
          TLorentzVector gpVector;
          gpVector.SetPtEtaPhiM(gp->pt(),gp->eta(),gp->phi(),gp->mass());
          double dist = jetVector.DeltaR(gpVector);
          if (dist > 0.4) continue;
          if (gp->pt() < 1.0) continue;
          total_sum += gp->pt();
          double vr = TMath::Sqrt( gp->vx()*gp->vx() + gp->vy()*gp->vy() );
          if (vr > 0.1) continue;
          prompt_sum += gp->pt();
        }
      }
    }
  }
  return prompt_sum/total_sum;
}

// Calculate jet alphaMax based on dz matching between track and vertex
double
EmJetAnalyzer::compute_alphaMax_dz(reco::TrackRefVector& trackRefs, double max_dz, double max_dxy) const
{
  double alphaMax = -1.;
  // Loop over all tracks and calculate scalar pt-sum of all tracks in current jet
  double jet_pt_sum = 0.;
  for (reco::TrackRefVector::iterator ijt = trackRefs.begin(); ijt != trackRefs.end(); ++ijt) {
    jet_pt_sum += (*ijt)->pt();
  } // End of track loop

  auto ipv_chosen = primary_verticesH_->end(); // iterator to chosen primary vertex
  double max_vertex_pt_sum = 0.; // scalar pt contribution of vertex to jet
  // Loop over all PVs and choose the one with highest scalar pt contribution to jet
  for (auto ipv = primary_verticesH_->begin(); ipv != primary_verticesH_->end(); ++ipv) {
    double vertex_pt_sum = 0.; // scalar pt contribution of vertex to jet
    for (reco::TrackRefVector::iterator ijt = trackRefs.begin(); ijt != trackRefs.end(); ++ijt) {
      double dxy = (*ijt)->dxy(*theBeamSpot_);
      double dz = (*ijt)->dz(ipv->position());
      // OUTPUT(dz);
      if ( fabs(dxy) < max_dxy && fabs(dz) < max_dz ) vertex_pt_sum += (*ijt)->pt();
      // double trackWeight = ipv->trackWeight(*ijt);
      // if (trackWeight > 0) vertex_pt_sum += (*ijt)->pt();
    } // End of track loop
    if (vertex_pt_sum > max_vertex_pt_sum) {
      max_vertex_pt_sum = vertex_pt_sum;
      ipv_chosen = ipv;
    }
  } // End of vertex loop
  // Calculate alpha
  alphaMax = max_vertex_pt_sum / jet_pt_sum;
  return alphaMax;
}

// Calculate jet alphaMax based on dz matching between track and vertex
double
EmJetAnalyzer::compute_alphaMax_dz(vector<reco::TransientTrack> tracks, double max_dz, double max_dxy) const
{
  double alphaMax = -1.;
  // Loop over all tracks and calculate scalar pt-sum of all tracks in current jet
  double jet_pt_sum = 0.;
  for (auto ijt = tracks.begin(); ijt != tracks.end(); ++ijt) {
    jet_pt_sum += (ijt)->track().pt();
  } // End of track loop

  auto ipv_chosen = primary_verticesH_->end(); // iterator to chosen primary vertex
  double max_vertex_pt_sum = 0.; // scalar pt contribution of vertex to jet
  // Loop over all PVs and choose the one with highest scalar pt contribution to jet
  for (auto ipv = primary_verticesH_->begin(); ipv != primary_verticesH_->end(); ++ipv) {
    double vertex_pt_sum = 0.; // scalar pt contribution of vertex to jet
    for (auto ijt = tracks.begin(); ijt != tracks.end(); ++ijt) {
      double dxy = (ijt)->track().dxy(*theBeamSpot_);
      double dz = (ijt)->track().dz(ipv->position());
      // OUTPUT(dz);
      if ( fabs(dxy) < max_dxy && fabs(dz) < max_dz ) vertex_pt_sum += (ijt)->track().pt();
      // double trackWeight = ipv->trackWeight(*ijt);
      // if (trackWeight > 0) vertex_pt_sum += (*ijt)->track().pt();
    } // End of track loop
    if (vertex_pt_sum > max_vertex_pt_sum) {
      max_vertex_pt_sum = vertex_pt_sum;
      ipv_chosen = ipv;
    }
  } // End of vertex loop
  // Calculate alpha
  alphaMax = max_vertex_pt_sum / jet_pt_sum;
  return alphaMax;
}

// Calculate jet median theta2D in radians
double
EmJetAnalyzer::compute_theta2D(const edm::EventSetup& iSetup) const
{
  // std::cout << "Entering compute_theta2D\n";
  // Uses generalTracks_
  // Gets jet information from jet_
  std::vector<float> vector_theta2D;
  for (std::vector<reco::TransientTrack>::const_iterator itk = generalTracks_.begin(); itk != generalTracks_.end(); ++itk) {
    if ( !selectJetTrackDeltaR(*itk, jet_) ) continue; // :CUT: Apply Track selection
    const reco::TransientTrack& itrack = *itk;
    // Retrieve track position and momentum direction at inner most hit
    // std::cout << "Getting inner most trajectory state\n";
    // TrajectoryStateOnSurface innermost_state = itrack.innermostMeasurementState();
    TrajectoryStateOnSurface innermost_state;
    {
      // trajectory information for acessing hits
      static GetTrackTrajInfo getTrackTrajInfo;
      std::vector<GetTrackTrajInfo::Result> trajInfo = getTrackTrajInfo.analyze(iSetup, itrack.track());
      assert(trajInfo.size()>0);
      innermost_state = trajInfo[0].detTSOS;
      if (!innermost_state.isValid()) continue;
    }
    // std::cout << "Sucessfully got inner most trajectory state\n";
    GlobalPoint innerPos = innermost_state.globalPosition();
    GlobalVector innerPosMom = innermost_state.globalMomentum();
    TVector3 innerPos2D(innerPos.x(), innerPos.y(), 0);
    TVector3 innerMom2D(innerPosMom.x(), innerPosMom.y(), 0);
    // Retrieve primary vertex position
    const reco::Vertex& primary_vertex = *primary_vertex_;
    TVector3 pvVector2D(primary_vertex.x(), primary_vertex.y(), 0);
    double theta2D = (-1 * (pvVector2D - innerPos2D)).Angle((innerMom2D));
    vector_theta2D.push_back(theta2D);
  }
  double median_theta2D = get_median(vector_theta2D);
  // OUTPUT(median_theta2D);
  // std::cout << "Exiting compute_theta2D\n";
  return median_theta2D;
}

int
EmJetAnalyzer::compute_nDarkPions(const reco::PFJet& ijet) const
{
  TLorentzVector jetVector;
  jetVector.SetPtEtaPhiM(ijet.pt(),ijet.eta(),ijet.phi(),0.);
  // Count number of dark pions
  int nDarkPions = 0;
  double minDist = 9999.;
  {
    if (!isData_) {
      for (auto gp = genParticlesH_->begin(); gp != genParticlesH_->end(); ++gp) {
        if (fabs(gp->pdgId()) != 4900111) continue;
        TLorentzVector gpVector;
        gpVector.SetPtEtaPhiM(gp->pt(),gp->eta(),gp->phi(),gp->mass());
        double dist = jetVector.DeltaR(gpVector);
        if (dist < 0.4) {
          nDarkPions++;
        }
        if (dist < minDist) minDist = dist;
      }
    }
  }
  return nDarkPions;
}

int
EmJetAnalyzer::compute_nDarkGluons(const reco::PFJet& ijet) const
{
  TLorentzVector jetVector;
  jetVector.SetPtEtaPhiM(ijet.pt(),ijet.eta(),ijet.phi(),0.);
  // Count number of dark pions
  int nDarkPions = 0;
  double minDist = 9999.;
  {
    if (!isData_) {
      for (auto gp = genParticlesH_->begin(); gp != genParticlesH_->end(); ++gp) {
        if (fabs(gp->pdgId()) != 4900021) continue;
        TLorentzVector gpVector;
        gpVector.SetPtEtaPhiM(gp->pt(),gp->eta(),gp->phi(),gp->mass());
        double dist = jetVector.DeltaR(gpVector);
        if (dist < 0.4) {
          nDarkPions++;
        }
        if (dist < minDist) minDist = dist;
      }
    }
  }
  return nDarkPions;
}

void
EmJetAnalyzer::jetdump(reco::TrackRefVector& trackRefs) const
{
  // auto ipv_chosen = primary_verticesH_->end(); // iterator to chosen primary vertex
  // double max_vertex_pt_sum = 0.; // scalar pt contribution of vertex to jet
  // // Loop over all PVs and choose the one with highest scalar pt contribution to jet
  // std::cout << "Track qualities:\n";
  // for (reco::TrackRefVector::iterator ijt = trackRefs.begin(); ijt != trackRefs.end(); ++ijt) {
  //   // OUTPUT((*ijt)->qualityName(reco::TrackBase::highPurity));
  //   // OUTPUT((*ijt)->qualityName(reco::TrackBase::loose));
  //   // OUTPUT((*ijt)->qualityName(reco::TrackBase::tight));
  //   // OUTPUT((*ijt)->qualityName(reco::TrackBase::goodIterative));
  //   OUTPUT((*ijt)->quality(reco::TrackBase::highPurity));
  //   OUTPUT((*ijt)->quality(reco::TrackBase::loose));
  //   OUTPUT((*ijt)->quality(reco::TrackBase::tight));
  //   OUTPUT((*ijt)->quality(reco::TrackBase::goodIterative));
  // }
  // for (auto ipv = primary_verticesH_->begin(); ipv != primary_verticesH_->end(); ++ipv) {
  //   std::cout << "New vertex\n";
  //   for (reco::TrackRefVector::iterator ijt = trackRefs.begin(); ijt != trackRefs.end(); ++ijt) {
  //     double trackWeight = ipv->trackWeight(*ijt);
  //     std::cout << "Track weight: " << trackWeight << std::endl;
  //   } // End of track loop
  // } // End of vertex loop
}

void
EmJetAnalyzer::jetscan(const reco::PFJet& ijet) {
  reco::TrackRefVector trackRefs = ijet.getTrackRefs();
  scanJet_->push_back(ijet);
  for (auto tref : trackRefs) {
    scanJetTracks_->push_back(*tref);
  }
  for (std::vector<reco::TransientTrack>::iterator itk = generalTracks_.begin(); itk != generalTracks_.end(); ++itk) {
    if ( !selectJetTrack(*itk, jet_, track_) ) continue; // :CUT: Apply Track selection
    scanJetSelectedTracks_->push_back(itk->track());
  }
}

double
EmJetAnalyzer::compute_pt2Sum (const TransientVertex& ivertex) const {
  auto vtx = reco::Vertex(ivertex);
  // Modified from reco::Vertex::p4()
  double sum = 0.;
  double pt = 0.;
  if(vtx.hasRefittedTracks()) {
    for(std::vector<reco::Track>::const_iterator iter = vtx.refittedTracks().begin();
        iter != vtx.refittedTracks().end(); ++iter) {
      pt = iter->pt();
      sum += pt*pt;
    }
  }
  else
    {
      for(std::vector<reco::TrackBaseRef>::const_iterator iter = vtx.tracks_begin();
          iter != vtx.tracks_end(); iter++) {
        pt = (*iter)->pt();
        sum += pt*pt;
      }
    }
  return sum;
}

double
EmJetAnalyzer::compute_alpha_global () const {
  return -999.999;
}

// Merge two TrackRefVector objects
reco::TrackRefVector
EmJetAnalyzer::MergeTracks(reco::TrackRefVector trks1,  reco::TrackRefVector trks2){
  reco::TrackRefVector mergedtrks;
  mergedtrks = trks1;
  for(size_t i = 0; i<trks2.size(); i++){
    bool new_trk = true;
    for(size_t j = 0; j< trks1.size(); j++){
      if(trks2[i].index()==trks1[j].index()){
        new_trk = false;
      }
    }
    if(new_trk) mergedtrks.push_back(trks2[i]);
  }
  return mergedtrks;

}

double
EmJetAnalyzer::compute_track_minVertexDz (const reco::TransientTrack& itrack) const
{
  double minVertexDz = 999999;
  double abs_minVertexDz = 999999;
  for (auto ipv = primary_verticesH_->begin(); ipv != primary_verticesH_->end(); ++ipv) {
    double dz = itrack.track().dz(ipv->position());
    if (fabs(dz) < abs_minVertexDz) {
      minVertexDz = dz;
      abs_minVertexDz = fabs(dz);
    }
  }
  return minVertexDz;
}


double
EmJetAnalyzer::compute_btag(const reco::PFJet& ijet) const
{
  const reco::JetTagCollection & bTags = *(bTagH_.product());
  auto obj1 = &ijet;
  TLorentzVector vector1; vector1.SetPxPyPzE(obj1->px(), obj1->py(), obj1->pz(), obj1->energy());
  double btagValue = -999;
  int nMatched = 0;
  for (unsigned i = 0; i != bTags.size(); ++i) {
    edm::RefToBase<reco::Jet> obj2 = bTags[i].first;
    TLorentzVector vector2; vector2.SetPxPyPzE(obj2->px(), obj2->py(), obj2->pz(), obj2->energy());
    double dr = vector1.DeltaR(vector2);
    // OUTPUT(dr);
    if (dr < 0.01) {
      btagValue = bTags[i].second;
      nMatched++;
    }
    // if (nMatched!=1) OUTPUT(nMatched);
    // else OUTPUT(bTags[i].second);
  }
  // OUTPUT(nMatched);
  if (nMatched==1) return btagValue;
  else             return -999;
}

template <class T>
T EmJetAnalyzer::get_median(const vector<T>& input) const
{
  vector<T> input_copy(input);
  std::sort(input_copy.begin(), input_copy.end());
  unsigned size = input_copy.size();
  double median = -1.;
  if (size>0) {
    if ( size % 2 == 0 ) {
      median = (input_copy[size/2 - 1] + input_copy[size/2]) / 2;
    }
    else {
      median = (input_copy[size/2]);
    }
  }
  return median;
}

vector<reco::TransientTrack>
EmJetAnalyzer::getJetTrackVectorDeltaR() const
{
  // Must be called after jet_ kinematic variables have been set
  vector<reco::TransientTrack> tracks;
  for (std::vector<reco::TransientTrack>::const_iterator itk = generalTracks_.begin(); itk != generalTracks_.end(); ++itk) {
    if ( !selectJetTrackDeltaR(*itk, jet_) ) continue; // :CUT: Apply Track selection
    tracks.push_back(*itk);
  }
  return tracks;
}

// :VERTEXTESTING:
void
EmJetAnalyzer::vertexdump(DistanceResults result) const
{
  // std::cout << "--------------------------------\n";
  // std::cout << "GenToReco\n";
  for (auto distance: std::get<0>(result)) {
    hist_LogVertexDistance_GenToReco_->Fill(TMath::Log10(distance));
    // OUTPUT(distance);
  }
  // std::cout << "RecoToGen\n";
  for (auto distance: std::get<2>(result)) {
    hist_LogVertexDistance_RecoToGen_->Fill(TMath::Log10(distance));
    // OUTPUT(distance);
  }
  // std::cout << "GenToReco\n";
  for (auto distance: std::get<1>(result)) {
    hist_LogVertexDistance2D_GenToReco_->Fill(TMath::Log10(distance));
    // OUTPUT(distance);
  }
  // std::cout << "RecoToGen\n";
  for (auto distance: std::get<3>(result)) {
    hist_LogVertexDistance2D_RecoToGen_->Fill(TMath::Log10(distance));
    // OUTPUT(distance);
  }
}


//define this as a plug-in
DEFINE_FWK_MODULE(EmJetAnalyzer);
