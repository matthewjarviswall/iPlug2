/*
 ==============================================================================
 
 This file is part of the iPlug 2 library
 
 Oli Larkin et al. 2018 - https://www.olilarkin.co.uk
 
 iPlug 2 is an open source library subject to commercial or open-source
 licensing.
 
 The code included in this file is provided under the terms of the WDL license
 - https://www.cockos.com/wdl/
 
 ==============================================================================
 */

#ifndef _IPLUGAPI_
#define _IPLUGAPI_

#include "IPlugAPIBase.h"
#include "IPlugProcessor.h"
#include "processor.h"

using namespace WAM;

/** Used to pass various instance info to the API class */
struct IPlugInstanceInfo
{};

/** WebAudioModule (WAM) API base class for an IPlug plug-in
 * @ingroup APIClasses */
class IPlugWAM : public IPlugAPIBase
               , public IPlugProcessor<float>
               , public Processor
{
public:
  IPlugWAM(IPlugInstanceInfo instanceInfo, IPlugConfig config);

  //WAM
  const char* init(uint32_t bufsize, uint32_t sr, void* pDesc) override;
  void terminate() override { DBGMSG("terminate"); }
  void resize(uint32_t bufsize) override { DBGMSG("resize"); }

  virtual void onProcess(WAM::AudioBus* pAudio, void* pData) override;
  virtual void onMidi(byte status, byte data1, byte data2) override;
  virtual void onSysex(byte* msg, uint32_t size) override;
  virtual void onMessage(char* verb, char* res, double data) override;
//  virtual void onMessage(char* verb, char* res, char* data) override;
//  virtual void onMessage(char* verb, char* res, void* data, uint32_t size) override;
  virtual void onParam(uint32_t idparam, double value) override;

  //IPlugProcessor
  void SetLatency(int samples) override {};
  bool SendMidiMsg(const IMidiMsg& msg) override { return false; }
  bool SendSysEx(ISysEx& msg) override { return false; }
  
  //IEditorDelegate - these are overwritten because we need to use WAM messaging system
  void SendControlValueFromDelegate(int controlTag, double normalizedValue) override;
  void SendControlMsgFromDelegate(int controlTag, int messageTag, int dataSize, const void* pData) override;
  void SendParameterValueFromDelegate(int paramIdx, double value, bool normalized) override {} // NOOP in WAM processor?
  void SendArbitraryMsgFromDelegate(int messageTag, int dataSize = 0, const void* pData = nullptr) override {};
  
private:
  int mBlockCounter = 0;
};

IPlugWAM* MakePlug();

#endif
