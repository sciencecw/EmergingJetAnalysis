#ifndef EmergingJetAnalysis_EmJetAnalyzer_EmJetEvent_h
#define EmergingJetAnalysis_EmJetAnalyzer_EmJetEvent_h

// Class describing the content of output for EmergingJetAnalyzer

#include <vector>
#include <functional>

#include "TLorentzVector.h"

#include "EmergingJetAnalysis/EmJetAnalyzer/interface/OutputTree.h"
#define DEFAULTVALUE -1

#ifndef OUTPUT
#define OUTPUT(x) std::cout<<#x << ": " << x << std::endl
#endif


using std::vector;

namespace emjet
{
  class Track;
  class Vertex;
  class Jet;
  class GenParticle;
  class PrimaryVertex;
  class Event {
  public:
    Event() {}
    ~Event(){
#ifdef DEBUG
      std::cout<<"Deleting event" << std::endl;
      OUTPUT(jet_vector.size());
#endif
    }
    void Init() {
      //[[[cog
      //template_string = "$name = DEFAULTVALUE;"
      //import vars_EmJetAnalyzer as m
      //for vardict in m.event_vardicts: m.replaceSingleLine(template_string, vardict)
      //]]]
      run                  = DEFAULTVALUE;
      lumi                 = DEFAULTVALUE;
      event                = DEFAULTVALUE;
      bx                   = DEFAULTVALUE;
      nVtx                 = DEFAULTVALUE;
      nGoodVtx             = DEFAULTVALUE;
      nTrueInt             = DEFAULTVALUE;
      met_pt               = DEFAULTVALUE;
      met_phi              = DEFAULTVALUE;
      nTracks              = DEFAULTVALUE;
      alpha_event          = DEFAULTVALUE;
      pdf_id1              = DEFAULTVALUE;
      pdf_id2              = DEFAULTVALUE;
      pdf_x1               = DEFAULTVALUE;
      pdf_x2               = DEFAULTVALUE;
      pdf_pdf1             = DEFAULTVALUE;
      pdf_pdf2             = DEFAULTVALUE;
      pdf_scalePDF         = DEFAULTVALUE;
      HLT_PFHT400          = DEFAULTVALUE;
      HLT_PFHT475          = DEFAULTVALUE;
      HLT_PFHT600          = DEFAULTVALUE;
      HLT_PFHT800          = DEFAULTVALUE;
      HLT_PFHT900          = DEFAULTVALUE;
      HLT_HT250            = DEFAULTVALUE;
      HLT_HT350            = DEFAULTVALUE;
      HLT_HT400            = DEFAULTVALUE;
      HLT_HT500            = DEFAULTVALUE;
      //[[[end]]]

      jet_vector.clear();
      genparticle_vector.clear();
      pv_vector.clear();
    };
    //[[[cog
    //template_string = "$cpptype $name;"
    //import vars_EmJetAnalyzer as m
    //for vardict in m.event_vardicts: m.replaceSingleLine(template_string, vardict)
    //]]]
    int    run                 ;
    int    lumi                ;
    int    event               ;
    int    bx                  ;
    int    nVtx                ;
    int    nGoodVtx            ;
    int    nTrueInt            ;
    float  met_pt              ;
    float  met_phi             ;
    int    nTracks             ;
    float  alpha_event         ;
    int    pdf_id1             ;
    int    pdf_id2             ;
    float  pdf_x1              ;
    float  pdf_x2              ;
    float  pdf_pdf1            ;
    float  pdf_pdf2            ;
    float  pdf_scalePDF        ;
    bool   HLT_PFHT400         ;
    bool   HLT_PFHT475         ;
    bool   HLT_PFHT600         ;
    bool   HLT_PFHT800         ;
    bool   HLT_PFHT900         ;
    bool   HLT_HT250           ;
    bool   HLT_HT350           ;
    bool   HLT_HT400           ;
    bool   HLT_HT500           ;
    //[[[end]]]

    vector<Jet> jet_vector;
    vector<GenParticle> genparticle_vector;
    vector<PrimaryVertex> pv_vector;
  };
  class Jet {
  public:
    Jet(){
#ifdef DEBUG
      std::cout<<"Constructing jet" << std::endl;
#endif
    }
    ~Jet(){
#ifdef DEBUG
      std::cout<<"Deleting jet" << std::endl;
      OUTPUT(int_vector.size());
      OUTPUT(track_vector.size());
#endif
    }
    void Init(){
      //[[[cog
      //template_string = "$name = DEFAULTVALUE;"
      //import vars_EmJetAnalyzer as m
      //for vardict in m.jet_vardicts: m.replaceSingleLine(template_string, vardict)
      //]]]
      index                = DEFAULTVALUE;
      source               = DEFAULTVALUE;
      ptRaw                = DEFAULTVALUE;
      eta                  = DEFAULTVALUE;
      phi                  = DEFAULTVALUE;
      pt                   = DEFAULTVALUE;
      ptUp                 = DEFAULTVALUE;
      ptDown               = DEFAULTVALUE;
      csv                  = DEFAULTVALUE;
      cef                  = DEFAULTVALUE;
      nef                  = DEFAULTVALUE;
      chf                  = DEFAULTVALUE;
      nhf                  = DEFAULTVALUE;
      pef                  = DEFAULTVALUE;
      mef                  = DEFAULTVALUE;
      missHits             = DEFAULTVALUE;
      muonHits             = DEFAULTVALUE;
      alpha                = DEFAULTVALUE;
      alpha2               = DEFAULTVALUE;
      alphaMax             = DEFAULTVALUE;
      alphaMax2            = DEFAULTVALUE;
      alpha_gen            = DEFAULTVALUE;
      alphaMax_dz100nm     = DEFAULTVALUE;
      alphaMax_dz200nm     = DEFAULTVALUE;
      alphaMax_dz500nm     = DEFAULTVALUE;
      alphaMax_dz1um       = DEFAULTVALUE;
      alphaMax_dz2um       = DEFAULTVALUE;
      alphaMax_dz5um       = DEFAULTVALUE;
      alphaMax_dz10um      = DEFAULTVALUE;
      alphaMax_dz20um      = DEFAULTVALUE;
      alphaMax_dz50um      = DEFAULTVALUE;
      alphaMax_dz100um     = DEFAULTVALUE;
      alphaMax_dz200um     = DEFAULTVALUE;
      alphaMax_dz500um     = DEFAULTVALUE;
      alphaMax_dz1mm       = DEFAULTVALUE;
      alphaMax_dz2mm       = DEFAULTVALUE;
      alphaMax_dz5mm       = DEFAULTVALUE;
      alphaMax_dz1cm       = DEFAULTVALUE;
      alphaMax_dz2cm       = DEFAULTVALUE;
      alphaMax_dz5cm       = DEFAULTVALUE;
      alphaMax_dz10cm      = DEFAULTVALUE;
      alphaMax_dz20cm      = DEFAULTVALUE;
      alphaMax_dz50cm      = DEFAULTVALUE;
      alphaMax2_dz100nm    = DEFAULTVALUE;
      alphaMax2_dz200nm    = DEFAULTVALUE;
      alphaMax2_dz500nm    = DEFAULTVALUE;
      alphaMax2_dz1um      = DEFAULTVALUE;
      alphaMax2_dz2um      = DEFAULTVALUE;
      alphaMax2_dz5um      = DEFAULTVALUE;
      alphaMax2_dz10um     = DEFAULTVALUE;
      alphaMax2_dz20um     = DEFAULTVALUE;
      alphaMax2_dz50um     = DEFAULTVALUE;
      alphaMax2_dz100um    = DEFAULTVALUE;
      alphaMax2_dz200um    = DEFAULTVALUE;
      alphaMax2_dz500um    = DEFAULTVALUE;
      alphaMax2_dz1mm      = DEFAULTVALUE;
      alphaMax2_dz2mm      = DEFAULTVALUE;
      alphaMax2_dz5mm      = DEFAULTVALUE;
      alphaMax2_dz1cm      = DEFAULTVALUE;
      alphaMax2_dz2cm      = DEFAULTVALUE;
      alphaMax2_dz5cm      = DEFAULTVALUE;
      alphaMax2_dz10cm     = DEFAULTVALUE;
      alphaMax2_dz20cm     = DEFAULTVALUE;
      alphaMax2_dz50cm     = DEFAULTVALUE;
      nDarkPions           = DEFAULTVALUE;
      nDarkGluons          = DEFAULTVALUE;
      minDRDarkPion        = DEFAULTVALUE;
      theta2D              = DEFAULTVALUE;
      //[[[end]]]

      track_vector.clear();
      int_vector.clear();
      vertex_vector.clear();
    }
    //[[[cog
    //template_string = "$cpptype $name;"
    //import vars_EmJetAnalyzer as m
    //for vardict in m.jet_vardicts: m.replaceSingleLine(template_string, vardict)
    //]]]
    int    index               ;
    int    source              ;
    float  ptRaw               ;
    float  eta                 ;
    float  phi                 ;
    float  pt                  ;
    float  ptUp                ;
    float  ptDown              ;
    float  csv                 ;
    float  cef                 ;
    float  nef                 ;
    float  chf                 ;
    float  nhf                 ;
    float  pef                 ;
    float  mef                 ;
    int    missHits            ;
    int    muonHits            ;
    float  alpha               ;
    float  alpha2              ;
    float  alphaMax            ;
    float  alphaMax2           ;
    float  alpha_gen           ;
    float  alphaMax_dz100nm    ;
    float  alphaMax_dz200nm    ;
    float  alphaMax_dz500nm    ;
    float  alphaMax_dz1um      ;
    float  alphaMax_dz2um      ;
    float  alphaMax_dz5um      ;
    float  alphaMax_dz10um     ;
    float  alphaMax_dz20um     ;
    float  alphaMax_dz50um     ;
    float  alphaMax_dz100um    ;
    float  alphaMax_dz200um    ;
    float  alphaMax_dz500um    ;
    float  alphaMax_dz1mm      ;
    float  alphaMax_dz2mm      ;
    float  alphaMax_dz5mm      ;
    float  alphaMax_dz1cm      ;
    float  alphaMax_dz2cm      ;
    float  alphaMax_dz5cm      ;
    float  alphaMax_dz10cm     ;
    float  alphaMax_dz20cm     ;
    float  alphaMax_dz50cm     ;
    float  alphaMax2_dz100nm   ;
    float  alphaMax2_dz200nm   ;
    float  alphaMax2_dz500nm   ;
    float  alphaMax2_dz1um     ;
    float  alphaMax2_dz2um     ;
    float  alphaMax2_dz5um     ;
    float  alphaMax2_dz10um    ;
    float  alphaMax2_dz20um    ;
    float  alphaMax2_dz50um    ;
    float  alphaMax2_dz100um   ;
    float  alphaMax2_dz200um   ;
    float  alphaMax2_dz500um   ;
    float  alphaMax2_dz1mm     ;
    float  alphaMax2_dz2mm     ;
    float  alphaMax2_dz5mm     ;
    float  alphaMax2_dz1cm     ;
    float  alphaMax2_dz2cm     ;
    float  alphaMax2_dz5cm     ;
    float  alphaMax2_dz10cm    ;
    float  alphaMax2_dz20cm    ;
    float  alphaMax2_dz50cm    ;
    int    nDarkPions          ;
    int    nDarkGluons         ;
    float  minDRDarkPion       ;
    float  theta2D             ;
    //[[[end]]]
    vector<Track>    track_vector;
    vector<Vertex>   vertex_vector;
    vector<int>    int_vector;
    // Variables used for calculation only
    // These are not written to output
    TLorentzVector p4;
  };
  class Track {
  public:
    Track() {}
    ~Track(){
#ifdef DEBUG
      // std::cout<<"Deleting track" << std::endl;
#endif
    }
    void Init() {
      //[[[cog
      //template_string = "$name = DEFAULTVALUE;"
      //import vars_EmJetAnalyzer as m
      //for vardict in m.jet_track_vardicts: m.replaceSingleLine(template_string, vardict)
      //]]]
      index                = DEFAULTVALUE;
      source               = DEFAULTVALUE;
      jet_index            = DEFAULTVALUE;
      vertex_index         = DEFAULTVALUE;
      vertex_weight        = DEFAULTVALUE;
      nHitsInFrontOfVert   = DEFAULTVALUE;
      missHitsAfterVert    = DEFAULTVALUE;
      pt                   = DEFAULTVALUE;
      eta                  = DEFAULTVALUE;
      phi                  = DEFAULTVALUE;
      ref_x                = DEFAULTVALUE;
      ref_y                = DEFAULTVALUE;
      ref_z                = DEFAULTVALUE;
      d0Error              = DEFAULTVALUE;
      dzError              = DEFAULTVALUE;
      pca_r                = DEFAULTVALUE;
      pca_eta              = DEFAULTVALUE;
      pca_phi              = DEFAULTVALUE;
      innerHit_r           = DEFAULTVALUE;
      innerHit_eta         = DEFAULTVALUE;
      innerHit_phi         = DEFAULTVALUE;
      quality              = DEFAULTVALUE;
      algo                 = DEFAULTVALUE;
      originalAlgo         = DEFAULTVALUE;
      nHits                = DEFAULTVALUE;
      nMissInnerHits       = DEFAULTVALUE;
      nTrkLayers           = DEFAULTVALUE;
      nMissInnerTrkLayers  = DEFAULTVALUE;
      nMissOuterTrkLayers  = DEFAULTVALUE;
      nMissTrkLayers       = DEFAULTVALUE;
      nPxlLayers           = DEFAULTVALUE;
      nMissInnerPxlLayers  = DEFAULTVALUE;
      nMissOuterPxlLayers  = DEFAULTVALUE;
      nMissPxlLayers       = DEFAULTVALUE;
      ipXY                 = DEFAULTVALUE;
      ipZ                  = DEFAULTVALUE;
      ipXYSig              = DEFAULTVALUE;
      ip3D                 = DEFAULTVALUE;
      ip3DSig              = DEFAULTVALUE;
      dRToJetAxis          = DEFAULTVALUE;
      distanceToJet        = DEFAULTVALUE;
      minVertexDz          = DEFAULTVALUE;
      pvWeight             = DEFAULTVALUE;
      minGenDistance       = DEFAULTVALUE;
      //[[[end]]]
    }
    //[[[cog
    //template_string = "$cpptype $name;"
    //import vars_EmJetAnalyzer as m
    //for vardict in m.jet_track_vardicts: m.replaceSingleLine(template_string, vardict)
    //]]]
    int    index               ;
    int    source              ;
    int    jet_index           ;
    int    vertex_index        ;
    float  vertex_weight       ;
    int    nHitsInFrontOfVert  ;
    int    missHitsAfterVert   ;
    float  pt                  ;
    float  eta                 ;
    float  phi                 ;
    float  ref_x               ;
    float  ref_y               ;
    float  ref_z               ;
    float  d0Error             ;
    float  dzError             ;
    float  pca_r               ;
    float  pca_eta             ;
    float  pca_phi             ;
    float  innerHit_r          ;
    float  innerHit_eta        ;
    float  innerHit_phi        ;
    int    quality             ;
    int    algo                ;
    int    originalAlgo        ;
    int    nHits               ;
    int    nMissInnerHits      ;
    int    nTrkLayers          ;
    int    nMissInnerTrkLayers ;
    int    nMissOuterTrkLayers ;
    int    nMissTrkLayers      ;
    int    nPxlLayers          ;
    int    nMissInnerPxlLayers ;
    int    nMissOuterPxlLayers ;
    int    nMissPxlLayers      ;
    float  ipXY                ;
    float  ipZ                 ;
    float  ipXYSig             ;
    float  ip3D                ;
    float  ip3DSig             ;
    float  dRToJetAxis         ;
    float  distanceToJet       ;
    float  minVertexDz         ;
    float  pvWeight            ;
    float  minGenDistance      ;
    //[[[end]]]
    // Variables used for calculation only
    // These are not written to output
    TLorentzVector p4;
  };
  class Vertex {
  public:
    Vertex() {}
    ~Vertex(){
#ifdef DEBUG
      std::cout<<"Deleting Vertex" << std::endl;
#endif
    }
    void Init() {
      //[[[cog
      //template_string = "$name = DEFAULTVALUE;"
      //import vars_EmJetAnalyzer as m
      //for vardict in m.jet_vertex_vardicts: m.replaceSingleLine(template_string, vardict)
      //]]]
      index                = DEFAULTVALUE;
      source               = DEFAULTVALUE;
      jet_index            = DEFAULTVALUE;
      x                    = DEFAULTVALUE;
      y                    = DEFAULTVALUE;
      z                    = DEFAULTVALUE;
      xError               = DEFAULTVALUE;
      yError               = DEFAULTVALUE;
      zError               = DEFAULTVALUE;
      deltaR               = DEFAULTVALUE;
      Lxy                  = DEFAULTVALUE;
      mass                 = DEFAULTVALUE;
      chi2                 = DEFAULTVALUE;
      ndof                 = DEFAULTVALUE;
      pt2sum               = DEFAULTVALUE;
      //[[[end]]]
    }
    //[[[cog
    //template_string = "$cpptype $name;"
    //import vars_EmJetAnalyzer as m
    //for vardict in m.jet_vertex_vardicts: m.replaceSingleLine(template_string, vardict)
    //]]]
    int    index               ;
    int    source              ;
    int    jet_index           ;
    float  x                   ;
    float  y                   ;
    float  z                   ;
    float  xError              ;
    float  yError              ;
    float  zError              ;
    float  deltaR              ;
    float  Lxy                 ;
    float  mass                ;
    float  chi2                ;
    float  ndof                ;
    float  pt2sum              ;
    //[[[end]]]
    // Variables used for calculation only
    // These are not written to output
    TLorentzVector p4;
  };
  class GenParticle {
  public:
    GenParticle() {}
    ~GenParticle() {}
    void Init(){
      //[[[cog
      //template_string = "$name = DEFAULTVALUE;"
      //import vars_EmJetAnalyzer as m
      //for vardict in m.genparticle_vardicts: m.replaceSingleLine(template_string, vardict)
      //]]]
      index                = DEFAULTVALUE;
      status               = DEFAULTVALUE;
      pdgId                = DEFAULTVALUE;
      charge               = DEFAULTVALUE;
      mass                 = DEFAULTVALUE;
      pt                   = DEFAULTVALUE;
      eta                  = DEFAULTVALUE;
      phi                  = DEFAULTVALUE;
      vx                   = DEFAULTVALUE;
      vy                   = DEFAULTVALUE;
      vz                   = DEFAULTVALUE;
      min2Ddist            = DEFAULTVALUE;
      min2Dsig             = DEFAULTVALUE;
      min3Ddist            = DEFAULTVALUE;
      min3Dsig             = DEFAULTVALUE;
      minDeltaR            = DEFAULTVALUE;
      matched2Ddist        = DEFAULTVALUE;
      matched2Dsig         = DEFAULTVALUE;
      matched3Ddist        = DEFAULTVALUE;
      matched3Dsig         = DEFAULTVALUE;
      matchedDeltaR        = DEFAULTVALUE;
      Lxy                  = DEFAULTVALUE;
      isDark               = DEFAULTVALUE;
      nDaughters           = DEFAULTVALUE;
      hasSMDaughter        = DEFAULTVALUE;
      hasDarkMother        = DEFAULTVALUE;
      hasDarkPionMother    = DEFAULTVALUE;
      isTrackable          = DEFAULTVALUE;
      //[[[end]]]
    }
    //[[[cog
    //template_string = "$cpptype $name;"
    //import vars_EmJetAnalyzer as m
    //for vardict in m.genparticle_vardicts: m.replaceSingleLine(template_string, vardict)
    //]]]
    int    index               ;
    int    status              ;
    int    pdgId               ;
    int    charge              ;
    float  mass                ;
    float  pt                  ;
    float  eta                 ;
    float  phi                 ;
    float  vx                  ;
    float  vy                  ;
    float  vz                  ;
    float  min2Ddist           ;
    float  min2Dsig            ;
    float  min3Ddist           ;
    float  min3Dsig            ;
    float  minDeltaR           ;
    float  matched2Ddist       ;
    float  matched2Dsig        ;
    float  matched3Ddist       ;
    float  matched3Dsig        ;
    float  matchedDeltaR       ;
    float  Lxy                 ;
    int    isDark              ;
    int    nDaughters          ;
    int    hasSMDaughter       ;
    int    hasDarkMother       ;
    int    hasDarkPionMother   ;
    int    isTrackable         ;
    //[[[end]]]
  };
  class PrimaryVertex {
  public:
    PrimaryVertex() {}
    ~PrimaryVertex() {}
    void Init(){
      //[[[cog
      //template_string = "$name = DEFAULTVALUE;"
      //import vars_EmJetAnalyzer as m
      //for vardict in m.pv_vardicts: m.replaceSingleLine(template_string, vardict)
      //]]]
      index                = DEFAULTVALUE;
      x                    = DEFAULTVALUE;
      y                    = DEFAULTVALUE;
      z                    = DEFAULTVALUE;
      xError               = DEFAULTVALUE;
      yError               = DEFAULTVALUE;
      zError               = DEFAULTVALUE;
      chi2                 = DEFAULTVALUE;
      ndof                 = DEFAULTVALUE;
      pt2sum               = DEFAULTVALUE;
      nTracks              = DEFAULTVALUE;
      //[[[end]]]
    }
    //[[[cog
    //template_string = "$cpptype $name;"
    //import vars_EmJetAnalyzer as m
    //for vardict in m.pv_vardicts: m.replaceSingleLine(template_string, vardict)
    //]]]
    int    index               ;
    float  x                   ;
    float  y                   ;
    float  z                   ;
    float  xError              ;
    float  yError              ;
    float  zError              ;
    float  chi2                ;
    float  ndof                ;
    float  pt2sum              ;
    int    nTracks             ;
    //[[[end]]]
  };
}

// Turn vector of objects, into vector of member variable by calling func(object)
// Output is placed in provided vector reference
template <typename Object, typename T>
void
vectorize(const vector<Object>& input, std::function<T (const Object &)> func, vector<T>& output)
{
  output.clear();
  output.reserve(input.size()); // Doesn't reduce capacity
  for (const auto& obj : input) {
    output.push_back( func(obj) );
  }
}

// Version of vectorize() that returns a vector object
template <typename Object, typename T>
vector<T>
vectorize_new(const vector<Object>& input, std::function<T (const Object &)> func)
{
  vector<T> output;
  vectorize(input, func, output);
  return output;
}

using emjet::PrimaryVertex;
using emjet::GenParticle;
using emjet::Track;
using emjet::Vertex;
using emjet::Jet;
using emjet::Event;

void
WriteEventToOutput(const Event& event, emjet::OutputTree* otree)
{
  otree->Init(); // Reset all values and clear all vectors
  // Event-level variables, e.g. int, float, etc.
  {
    //[[[cog
    //template_string = "otree->$name = event.$name;"
    //import vars_EmJetAnalyzer as m
    //for vardict in m.event_vardicts: m.replaceSingleLine(template_string, vardict)
    //]]]
    otree->run                  = event.run                 ;
    otree->lumi                 = event.lumi                ;
    otree->event                = event.event               ;
    otree->bx                   = event.bx                  ;
    otree->nVtx                 = event.nVtx                ;
    otree->nGoodVtx             = event.nGoodVtx            ;
    otree->nTrueInt             = event.nTrueInt            ;
    otree->met_pt               = event.met_pt              ;
    otree->met_phi              = event.met_phi             ;
    otree->nTracks              = event.nTracks             ;
    otree->alpha_event          = event.alpha_event         ;
    otree->pdf_id1              = event.pdf_id1             ;
    otree->pdf_id2              = event.pdf_id2             ;
    otree->pdf_x1               = event.pdf_x1              ;
    otree->pdf_x2               = event.pdf_x2              ;
    otree->pdf_pdf1             = event.pdf_pdf1            ;
    otree->pdf_pdf2             = event.pdf_pdf2            ;
    otree->pdf_scalePDF         = event.pdf_scalePDF        ;
    otree->HLT_PFHT400          = event.HLT_PFHT400         ;
    otree->HLT_PFHT475          = event.HLT_PFHT475         ;
    otree->HLT_PFHT600          = event.HLT_PFHT600         ;
    otree->HLT_PFHT800          = event.HLT_PFHT800         ;
    otree->HLT_PFHT900          = event.HLT_PFHT900         ;
    otree->HLT_HT250            = event.HLT_HT250           ;
    otree->HLT_HT350            = event.HLT_HT350           ;
    otree->HLT_HT400            = event.HLT_HT400           ;
    otree->HLT_HT500            = event.HLT_HT500           ;
    //[[[end]]]
  }
  // Jet-level variables, e.g. vector<int>, vector<float>, etc.
  {
    //[[[cog
    //template_string = "vectorize<Jet, $cpptype>(event.jet_vector, [](const emjet::Jet& obj ){return obj.$name;}, otree->jet_$name);"
    //import vars_EmJetAnalyzer as m
    //for vardict in m.jet_vardicts: m.replaceSingleLine(template_string, vardict)
    //]]]
    vectorize<Jet, int   >(event.jet_vector, [](const emjet::Jet& obj ){return obj.index               ;}, otree->jet_index               );
    vectorize<Jet, int   >(event.jet_vector, [](const emjet::Jet& obj ){return obj.source              ;}, otree->jet_source              );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.ptRaw               ;}, otree->jet_ptRaw               );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.eta                 ;}, otree->jet_eta                 );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.phi                 ;}, otree->jet_phi                 );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.pt                  ;}, otree->jet_pt                  );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.ptUp                ;}, otree->jet_ptUp                );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.ptDown              ;}, otree->jet_ptDown              );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.csv                 ;}, otree->jet_csv                 );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.cef                 ;}, otree->jet_cef                 );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.nef                 ;}, otree->jet_nef                 );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.chf                 ;}, otree->jet_chf                 );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.nhf                 ;}, otree->jet_nhf                 );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.pef                 ;}, otree->jet_pef                 );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.mef                 ;}, otree->jet_mef                 );
    vectorize<Jet, int   >(event.jet_vector, [](const emjet::Jet& obj ){return obj.missHits            ;}, otree->jet_missHits            );
    vectorize<Jet, int   >(event.jet_vector, [](const emjet::Jet& obj ){return obj.muonHits            ;}, otree->jet_muonHits            );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alpha               ;}, otree->jet_alpha               );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alpha2              ;}, otree->jet_alpha2              );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax            ;}, otree->jet_alphaMax            );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax2           ;}, otree->jet_alphaMax2           );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alpha_gen           ;}, otree->jet_alpha_gen           );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax_dz100nm    ;}, otree->jet_alphaMax_dz100nm    );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax_dz200nm    ;}, otree->jet_alphaMax_dz200nm    );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax_dz500nm    ;}, otree->jet_alphaMax_dz500nm    );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax_dz1um      ;}, otree->jet_alphaMax_dz1um      );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax_dz2um      ;}, otree->jet_alphaMax_dz2um      );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax_dz5um      ;}, otree->jet_alphaMax_dz5um      );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax_dz10um     ;}, otree->jet_alphaMax_dz10um     );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax_dz20um     ;}, otree->jet_alphaMax_dz20um     );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax_dz50um     ;}, otree->jet_alphaMax_dz50um     );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax_dz100um    ;}, otree->jet_alphaMax_dz100um    );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax_dz200um    ;}, otree->jet_alphaMax_dz200um    );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax_dz500um    ;}, otree->jet_alphaMax_dz500um    );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax_dz1mm      ;}, otree->jet_alphaMax_dz1mm      );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax_dz2mm      ;}, otree->jet_alphaMax_dz2mm      );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax_dz5mm      ;}, otree->jet_alphaMax_dz5mm      );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax_dz1cm      ;}, otree->jet_alphaMax_dz1cm      );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax_dz2cm      ;}, otree->jet_alphaMax_dz2cm      );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax_dz5cm      ;}, otree->jet_alphaMax_dz5cm      );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax_dz10cm     ;}, otree->jet_alphaMax_dz10cm     );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax_dz20cm     ;}, otree->jet_alphaMax_dz20cm     );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax_dz50cm     ;}, otree->jet_alphaMax_dz50cm     );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax2_dz100nm   ;}, otree->jet_alphaMax2_dz100nm   );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax2_dz200nm   ;}, otree->jet_alphaMax2_dz200nm   );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax2_dz500nm   ;}, otree->jet_alphaMax2_dz500nm   );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax2_dz1um     ;}, otree->jet_alphaMax2_dz1um     );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax2_dz2um     ;}, otree->jet_alphaMax2_dz2um     );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax2_dz5um     ;}, otree->jet_alphaMax2_dz5um     );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax2_dz10um    ;}, otree->jet_alphaMax2_dz10um    );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax2_dz20um    ;}, otree->jet_alphaMax2_dz20um    );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax2_dz50um    ;}, otree->jet_alphaMax2_dz50um    );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax2_dz100um   ;}, otree->jet_alphaMax2_dz100um   );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax2_dz200um   ;}, otree->jet_alphaMax2_dz200um   );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax2_dz500um   ;}, otree->jet_alphaMax2_dz500um   );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax2_dz1mm     ;}, otree->jet_alphaMax2_dz1mm     );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax2_dz2mm     ;}, otree->jet_alphaMax2_dz2mm     );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax2_dz5mm     ;}, otree->jet_alphaMax2_dz5mm     );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax2_dz1cm     ;}, otree->jet_alphaMax2_dz1cm     );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax2_dz2cm     ;}, otree->jet_alphaMax2_dz2cm     );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax2_dz5cm     ;}, otree->jet_alphaMax2_dz5cm     );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax2_dz10cm    ;}, otree->jet_alphaMax2_dz10cm    );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax2_dz20cm    ;}, otree->jet_alphaMax2_dz20cm    );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.alphaMax2_dz50cm    ;}, otree->jet_alphaMax2_dz50cm    );
    vectorize<Jet, int   >(event.jet_vector, [](const emjet::Jet& obj ){return obj.nDarkPions          ;}, otree->jet_nDarkPions          );
    vectorize<Jet, int   >(event.jet_vector, [](const emjet::Jet& obj ){return obj.nDarkGluons         ;}, otree->jet_nDarkGluons         );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.minDRDarkPion       ;}, otree->jet_minDRDarkPion       );
    vectorize<Jet, float >(event.jet_vector, [](const emjet::Jet& obj ){return obj.theta2D             ;}, otree->jet_theta2D             );
    //[[[end]]]
  }
  // Jet-Track-level variables
  {
    for (const auto jet : event.jet_vector) {
      //[[[cog
      //import vars_EmJetAnalyzer as m
      //template_string = "auto $name = vectorize_new<Track, $cpptype>(jet.track_vector, [](const Track& obj ){return obj.$name;}); otree->track_$name.push_back($name);"
      //for vardict in m.jet_track_vardicts: m.replaceSingleLine(template_string, vardict)
      //]]]
      auto index                = vectorize_new<Track, int   >(jet.track_vector, [](const Track& obj ){return obj.index               ;}); otree->track_index               .push_back(index               );
      auto source               = vectorize_new<Track, int   >(jet.track_vector, [](const Track& obj ){return obj.source              ;}); otree->track_source              .push_back(source              );
      auto jet_index            = vectorize_new<Track, int   >(jet.track_vector, [](const Track& obj ){return obj.jet_index           ;}); otree->track_jet_index           .push_back(jet_index           );
      auto vertex_index         = vectorize_new<Track, int   >(jet.track_vector, [](const Track& obj ){return obj.vertex_index        ;}); otree->track_vertex_index        .push_back(vertex_index        );
      auto vertex_weight        = vectorize_new<Track, float >(jet.track_vector, [](const Track& obj ){return obj.vertex_weight       ;}); otree->track_vertex_weight       .push_back(vertex_weight       );
      auto nHitsInFrontOfVert   = vectorize_new<Track, int   >(jet.track_vector, [](const Track& obj ){return obj.nHitsInFrontOfVert  ;}); otree->track_nHitsInFrontOfVert  .push_back(nHitsInFrontOfVert  );
      auto missHitsAfterVert    = vectorize_new<Track, int   >(jet.track_vector, [](const Track& obj ){return obj.missHitsAfterVert   ;}); otree->track_missHitsAfterVert   .push_back(missHitsAfterVert   );
      auto pt                   = vectorize_new<Track, float >(jet.track_vector, [](const Track& obj ){return obj.pt                  ;}); otree->track_pt                  .push_back(pt                  );
      auto eta                  = vectorize_new<Track, float >(jet.track_vector, [](const Track& obj ){return obj.eta                 ;}); otree->track_eta                 .push_back(eta                 );
      auto phi                  = vectorize_new<Track, float >(jet.track_vector, [](const Track& obj ){return obj.phi                 ;}); otree->track_phi                 .push_back(phi                 );
      auto ref_x                = vectorize_new<Track, float >(jet.track_vector, [](const Track& obj ){return obj.ref_x               ;}); otree->track_ref_x               .push_back(ref_x               );
      auto ref_y                = vectorize_new<Track, float >(jet.track_vector, [](const Track& obj ){return obj.ref_y               ;}); otree->track_ref_y               .push_back(ref_y               );
      auto ref_z                = vectorize_new<Track, float >(jet.track_vector, [](const Track& obj ){return obj.ref_z               ;}); otree->track_ref_z               .push_back(ref_z               );
      auto d0Error              = vectorize_new<Track, float >(jet.track_vector, [](const Track& obj ){return obj.d0Error             ;}); otree->track_d0Error             .push_back(d0Error             );
      auto dzError              = vectorize_new<Track, float >(jet.track_vector, [](const Track& obj ){return obj.dzError             ;}); otree->track_dzError             .push_back(dzError             );
      auto pca_r                = vectorize_new<Track, float >(jet.track_vector, [](const Track& obj ){return obj.pca_r               ;}); otree->track_pca_r               .push_back(pca_r               );
      auto pca_eta              = vectorize_new<Track, float >(jet.track_vector, [](const Track& obj ){return obj.pca_eta             ;}); otree->track_pca_eta             .push_back(pca_eta             );
      auto pca_phi              = vectorize_new<Track, float >(jet.track_vector, [](const Track& obj ){return obj.pca_phi             ;}); otree->track_pca_phi             .push_back(pca_phi             );
      auto innerHit_r           = vectorize_new<Track, float >(jet.track_vector, [](const Track& obj ){return obj.innerHit_r          ;}); otree->track_innerHit_r          .push_back(innerHit_r          );
      auto innerHit_eta         = vectorize_new<Track, float >(jet.track_vector, [](const Track& obj ){return obj.innerHit_eta        ;}); otree->track_innerHit_eta        .push_back(innerHit_eta        );
      auto innerHit_phi         = vectorize_new<Track, float >(jet.track_vector, [](const Track& obj ){return obj.innerHit_phi        ;}); otree->track_innerHit_phi        .push_back(innerHit_phi        );
      auto quality              = vectorize_new<Track, int   >(jet.track_vector, [](const Track& obj ){return obj.quality             ;}); otree->track_quality             .push_back(quality             );
      auto algo                 = vectorize_new<Track, int   >(jet.track_vector, [](const Track& obj ){return obj.algo                ;}); otree->track_algo                .push_back(algo                );
      auto originalAlgo         = vectorize_new<Track, int   >(jet.track_vector, [](const Track& obj ){return obj.originalAlgo        ;}); otree->track_originalAlgo        .push_back(originalAlgo        );
      auto nHits                = vectorize_new<Track, int   >(jet.track_vector, [](const Track& obj ){return obj.nHits               ;}); otree->track_nHits               .push_back(nHits               );
      auto nMissInnerHits       = vectorize_new<Track, int   >(jet.track_vector, [](const Track& obj ){return obj.nMissInnerHits      ;}); otree->track_nMissInnerHits      .push_back(nMissInnerHits      );
      auto nTrkLayers           = vectorize_new<Track, int   >(jet.track_vector, [](const Track& obj ){return obj.nTrkLayers          ;}); otree->track_nTrkLayers          .push_back(nTrkLayers          );
      auto nMissInnerTrkLayers  = vectorize_new<Track, int   >(jet.track_vector, [](const Track& obj ){return obj.nMissInnerTrkLayers ;}); otree->track_nMissInnerTrkLayers .push_back(nMissInnerTrkLayers );
      auto nMissOuterTrkLayers  = vectorize_new<Track, int   >(jet.track_vector, [](const Track& obj ){return obj.nMissOuterTrkLayers ;}); otree->track_nMissOuterTrkLayers .push_back(nMissOuterTrkLayers );
      auto nMissTrkLayers       = vectorize_new<Track, int   >(jet.track_vector, [](const Track& obj ){return obj.nMissTrkLayers      ;}); otree->track_nMissTrkLayers      .push_back(nMissTrkLayers      );
      auto nPxlLayers           = vectorize_new<Track, int   >(jet.track_vector, [](const Track& obj ){return obj.nPxlLayers          ;}); otree->track_nPxlLayers          .push_back(nPxlLayers          );
      auto nMissInnerPxlLayers  = vectorize_new<Track, int   >(jet.track_vector, [](const Track& obj ){return obj.nMissInnerPxlLayers ;}); otree->track_nMissInnerPxlLayers .push_back(nMissInnerPxlLayers );
      auto nMissOuterPxlLayers  = vectorize_new<Track, int   >(jet.track_vector, [](const Track& obj ){return obj.nMissOuterPxlLayers ;}); otree->track_nMissOuterPxlLayers .push_back(nMissOuterPxlLayers );
      auto nMissPxlLayers       = vectorize_new<Track, int   >(jet.track_vector, [](const Track& obj ){return obj.nMissPxlLayers      ;}); otree->track_nMissPxlLayers      .push_back(nMissPxlLayers      );
      auto ipXY                 = vectorize_new<Track, float >(jet.track_vector, [](const Track& obj ){return obj.ipXY                ;}); otree->track_ipXY                .push_back(ipXY                );
      auto ipZ                  = vectorize_new<Track, float >(jet.track_vector, [](const Track& obj ){return obj.ipZ                 ;}); otree->track_ipZ                 .push_back(ipZ                 );
      auto ipXYSig              = vectorize_new<Track, float >(jet.track_vector, [](const Track& obj ){return obj.ipXYSig             ;}); otree->track_ipXYSig             .push_back(ipXYSig             );
      auto ip3D                 = vectorize_new<Track, float >(jet.track_vector, [](const Track& obj ){return obj.ip3D                ;}); otree->track_ip3D                .push_back(ip3D                );
      auto ip3DSig              = vectorize_new<Track, float >(jet.track_vector, [](const Track& obj ){return obj.ip3DSig             ;}); otree->track_ip3DSig             .push_back(ip3DSig             );
      auto dRToJetAxis          = vectorize_new<Track, float >(jet.track_vector, [](const Track& obj ){return obj.dRToJetAxis         ;}); otree->track_dRToJetAxis         .push_back(dRToJetAxis         );
      auto distanceToJet        = vectorize_new<Track, float >(jet.track_vector, [](const Track& obj ){return obj.distanceToJet       ;}); otree->track_distanceToJet       .push_back(distanceToJet       );
      auto minVertexDz          = vectorize_new<Track, float >(jet.track_vector, [](const Track& obj ){return obj.minVertexDz         ;}); otree->track_minVertexDz         .push_back(minVertexDz         );
      auto pvWeight             = vectorize_new<Track, float >(jet.track_vector, [](const Track& obj ){return obj.pvWeight            ;}); otree->track_pvWeight            .push_back(pvWeight            );
      auto minGenDistance       = vectorize_new<Track, float >(jet.track_vector, [](const Track& obj ){return obj.minGenDistance      ;}); otree->track_minGenDistance      .push_back(minGenDistance      );
      //[[[end]]]
    }
  }
  // Jet-Vertex-level variables
  {
    for (const auto jet : event.jet_vector) {
      //[[[cog
      //import vars_EmJetAnalyzer as m
      //template_string = "auto $name = vectorize_new<Vertex, $cpptype>(jet.vertex_vector, [](const Vertex& obj ){return obj.$name;}); otree->vertex_$name.push_back($name);"
      //for vardict in m.jet_vertex_vardicts: m.replaceSingleLine(template_string, vardict)
      //]]]
      auto index                = vectorize_new<Vertex, int   >(jet.vertex_vector, [](const Vertex& obj ){return obj.index               ;}); otree->vertex_index               .push_back(index               );
      auto source               = vectorize_new<Vertex, int   >(jet.vertex_vector, [](const Vertex& obj ){return obj.source              ;}); otree->vertex_source              .push_back(source              );
      auto jet_index            = vectorize_new<Vertex, int   >(jet.vertex_vector, [](const Vertex& obj ){return obj.jet_index           ;}); otree->vertex_jet_index           .push_back(jet_index           );
      auto x                    = vectorize_new<Vertex, float >(jet.vertex_vector, [](const Vertex& obj ){return obj.x                   ;}); otree->vertex_x                   .push_back(x                   );
      auto y                    = vectorize_new<Vertex, float >(jet.vertex_vector, [](const Vertex& obj ){return obj.y                   ;}); otree->vertex_y                   .push_back(y                   );
      auto z                    = vectorize_new<Vertex, float >(jet.vertex_vector, [](const Vertex& obj ){return obj.z                   ;}); otree->vertex_z                   .push_back(z                   );
      auto xError               = vectorize_new<Vertex, float >(jet.vertex_vector, [](const Vertex& obj ){return obj.xError              ;}); otree->vertex_xError              .push_back(xError              );
      auto yError               = vectorize_new<Vertex, float >(jet.vertex_vector, [](const Vertex& obj ){return obj.yError              ;}); otree->vertex_yError              .push_back(yError              );
      auto zError               = vectorize_new<Vertex, float >(jet.vertex_vector, [](const Vertex& obj ){return obj.zError              ;}); otree->vertex_zError              .push_back(zError              );
      auto deltaR               = vectorize_new<Vertex, float >(jet.vertex_vector, [](const Vertex& obj ){return obj.deltaR              ;}); otree->vertex_deltaR              .push_back(deltaR              );
      auto Lxy                  = vectorize_new<Vertex, float >(jet.vertex_vector, [](const Vertex& obj ){return obj.Lxy                 ;}); otree->vertex_Lxy                 .push_back(Lxy                 );
      auto mass                 = vectorize_new<Vertex, float >(jet.vertex_vector, [](const Vertex& obj ){return obj.mass                ;}); otree->vertex_mass                .push_back(mass                );
      auto chi2                 = vectorize_new<Vertex, float >(jet.vertex_vector, [](const Vertex& obj ){return obj.chi2                ;}); otree->vertex_chi2                .push_back(chi2                );
      auto ndof                 = vectorize_new<Vertex, float >(jet.vertex_vector, [](const Vertex& obj ){return obj.ndof                ;}); otree->vertex_ndof                .push_back(ndof                );
      auto pt2sum               = vectorize_new<Vertex, float >(jet.vertex_vector, [](const Vertex& obj ){return obj.pt2sum              ;}); otree->vertex_pt2sum              .push_back(pt2sum              );
      //[[[end]]]
    }
  }
  // GenParticle-level variables, e.g. vector<int>, vector<float>, etc.
  {
    //[[[cog
    //template_string = "vectorize<GenParticle, $cpptype>(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.$name;}, otree->gp_$name);"
    //import vars_EmJetAnalyzer as m
    //for vardict in m.genparticle_vardicts: m.replaceSingleLine(template_string, vardict)
    //]]]
    vectorize<GenParticle, int   >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.index               ;}, otree->gp_index               );
    vectorize<GenParticle, int   >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.status              ;}, otree->gp_status              );
    vectorize<GenParticle, int   >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.pdgId               ;}, otree->gp_pdgId               );
    vectorize<GenParticle, int   >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.charge              ;}, otree->gp_charge              );
    vectorize<GenParticle, float >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.mass                ;}, otree->gp_mass                );
    vectorize<GenParticle, float >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.pt                  ;}, otree->gp_pt                  );
    vectorize<GenParticle, float >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.eta                 ;}, otree->gp_eta                 );
    vectorize<GenParticle, float >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.phi                 ;}, otree->gp_phi                 );
    vectorize<GenParticle, float >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.vx                  ;}, otree->gp_vx                  );
    vectorize<GenParticle, float >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.vy                  ;}, otree->gp_vy                  );
    vectorize<GenParticle, float >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.vz                  ;}, otree->gp_vz                  );
    vectorize<GenParticle, float >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.min2Ddist           ;}, otree->gp_min2Ddist           );
    vectorize<GenParticle, float >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.min2Dsig            ;}, otree->gp_min2Dsig            );
    vectorize<GenParticle, float >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.min3Ddist           ;}, otree->gp_min3Ddist           );
    vectorize<GenParticle, float >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.min3Dsig            ;}, otree->gp_min3Dsig            );
    vectorize<GenParticle, float >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.minDeltaR           ;}, otree->gp_minDeltaR           );
    vectorize<GenParticle, float >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.matched2Ddist       ;}, otree->gp_matched2Ddist       );
    vectorize<GenParticle, float >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.matched2Dsig        ;}, otree->gp_matched2Dsig        );
    vectorize<GenParticle, float >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.matched3Ddist       ;}, otree->gp_matched3Ddist       );
    vectorize<GenParticle, float >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.matched3Dsig        ;}, otree->gp_matched3Dsig        );
    vectorize<GenParticle, float >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.matchedDeltaR       ;}, otree->gp_matchedDeltaR       );
    vectorize<GenParticle, float >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.Lxy                 ;}, otree->gp_Lxy                 );
    vectorize<GenParticle, int   >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.isDark              ;}, otree->gp_isDark              );
    vectorize<GenParticle, int   >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.nDaughters          ;}, otree->gp_nDaughters          );
    vectorize<GenParticle, int   >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.hasSMDaughter       ;}, otree->gp_hasSMDaughter       );
    vectorize<GenParticle, int   >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.hasDarkMother       ;}, otree->gp_hasDarkMother       );
    vectorize<GenParticle, int   >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.hasDarkPionMother   ;}, otree->gp_hasDarkPionMother   );
    vectorize<GenParticle, int   >(event.genparticle_vector, [](const emjet::GenParticle& obj ){return obj.isTrackable         ;}, otree->gp_isTrackable         );
    //[[[end]]]
  }
  // PrimaryVertex-level variables, e.g. vector<int>, vector<float>, etc.
  {
    //[[[cog
    //template_string = "vectorize<PrimaryVertex, $cpptype>(event.pv_vector, [](const emjet::PrimaryVertex& obj ){return obj.$name;}, otree->pv_$name);"
    //import vars_EmJetAnalyzer as m
    //for vardict in m.pv_vardicts: m.replaceSingleLine(template_string, vardict)
    //]]]
    vectorize<PrimaryVertex, int   >(event.pv_vector, [](const emjet::PrimaryVertex& obj ){return obj.index               ;}, otree->pv_index               );
    vectorize<PrimaryVertex, float >(event.pv_vector, [](const emjet::PrimaryVertex& obj ){return obj.x                   ;}, otree->pv_x                   );
    vectorize<PrimaryVertex, float >(event.pv_vector, [](const emjet::PrimaryVertex& obj ){return obj.y                   ;}, otree->pv_y                   );
    vectorize<PrimaryVertex, float >(event.pv_vector, [](const emjet::PrimaryVertex& obj ){return obj.z                   ;}, otree->pv_z                   );
    vectorize<PrimaryVertex, float >(event.pv_vector, [](const emjet::PrimaryVertex& obj ){return obj.xError              ;}, otree->pv_xError              );
    vectorize<PrimaryVertex, float >(event.pv_vector, [](const emjet::PrimaryVertex& obj ){return obj.yError              ;}, otree->pv_yError              );
    vectorize<PrimaryVertex, float >(event.pv_vector, [](const emjet::PrimaryVertex& obj ){return obj.zError              ;}, otree->pv_zError              );
    vectorize<PrimaryVertex, float >(event.pv_vector, [](const emjet::PrimaryVertex& obj ){return obj.chi2                ;}, otree->pv_chi2                );
    vectorize<PrimaryVertex, float >(event.pv_vector, [](const emjet::PrimaryVertex& obj ){return obj.ndof                ;}, otree->pv_ndof                );
    vectorize<PrimaryVertex, float >(event.pv_vector, [](const emjet::PrimaryVertex& obj ){return obj.pt2sum              ;}, otree->pv_pt2sum              );
    vectorize<PrimaryVertex, int   >(event.pv_vector, [](const emjet::PrimaryVertex& obj ){return obj.nTracks             ;}, otree->pv_nTracks             );
    //[[[end]]]
  }
}

// write_jet_to_event(emjet::Jet jet, emjet::OutputTree* otree)
// {
//   otree.jet_pt.push_back(jet.pt);
//   track_pt = vectorize(jet.track_vector, auto[](auto track){return track.pt;} )
// }

#endif
